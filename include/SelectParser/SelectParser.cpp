#ifndef SELECT_PARSER
#define SELECT_PARSER

#include "models.cpp"
#include "Helper.cpp"
#include "Expression.cpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

class SelectParser
{
private:
    Catalog *_catalog;

    // =========================================================
    // 1. (WHERE Clause Parsing)
    // =========================================================

    // //helper function using to hunt comparison operator
    OpKind get_comparison_operator(const string& s, size_t& pos)
    {
        while (pos < s.size() && isspace(s[pos])) pos++;

        if (s.substr(pos, 2) == "<>") { pos += 2; return OpKind::NEQ; }
        if (s.substr(pos, 2) == "!=") { pos += 2; return OpKind::NEQ; }
        if (s.substr(pos, 2) == ">=") { pos += 2; return OpKind::GTE; }
        if (s.substr(pos, 2) == "<=") { pos += 2; return OpKind::LTE; }
        if (s.substr(pos, 1) == "=")  { pos += 1; return OpKind::EQ; }
        if (s.substr(pos, 1) == ">")  { pos += 1; return OpKind::GT; }
        if (s.substr(pos, 1) == "<")  { pos += 1; return OpKind::LT; }

        return OpKind::EQ;
    }

    //  col = val
    unique_ptr<Expression> parse_primary_expression(const string& s, size_t& pos)
    {
        size_t start = pos;
        while (pos < s.size() && !isspace(s[pos]) && s[pos] != '=' && s[pos] != '<' && s[pos] != '>')
            pos++;

        string column_name = Helper::trim(s.substr(start, pos - start));
        if (column_name.empty()) return nullptr;

        while (pos < s.size() && isspace(s[pos])) pos++;

        OpKind op = get_comparison_operator(s, pos);

        while (pos < s.size() && isspace(s[pos])) pos++;

        start = pos;

        bool in_quote = (s[pos] == '\'' || s[pos] == '"');
        if (in_quote) pos++;

        while (pos < s.size()) {
            if (in_quote && s[pos] == s[start - 1]) {
                pos++;
                break;
            }
            if (!in_quote && isspace(s[pos])) break;
            if (!in_quote && (Helper::to_lower(s.substr(pos, 3)) == "and" || Helper::to_lower(s.substr(pos, 2)) == "or")) break;
            pos++;
        }

        string literal_value = Helper::trim(s.substr(start, pos - start));
        if (literal_value.empty()) return nullptr;

        while (pos < s.size() && isspace(s[pos])) pos++;

        return make_unique<Expression>(Expression{
            Expression::NodeType::COMPARISON,
            op,
            column_name,
            literal_value
        });
    }

    // AND (أولوية أعلى)
    unique_ptr<Expression> parse_and_expression(const string& s, size_t& pos)
    {
        unique_ptr<Expression> left_expr = parse_primary_expression(s, pos);
        if (!left_expr) return nullptr;

        while (pos < s.size())
        {
            string token = Helper::to_lower(s.substr(pos, 3));
            if (token != "and") break;

            pos += 3;
            while (pos < s.size() && isspace(s[pos])) pos++;

            unique_ptr<Expression> right_expr = parse_primary_expression(s, pos);
            if (!right_expr) return nullptr;

            unique_ptr<Expression> new_root = make_unique<Expression>(Expression{
                Expression::NodeType::LOGICAL,
                OpKind::AND,
                "", "",
                move(left_expr),
                move(right_expr)
            });
            left_expr = move(new_root);
        }
        return left_expr;
    }

    // OR (أولوية أقل)
    unique_ptr<Expression> parse_or_expression(const string& s, size_t& pos)
    {
        unique_ptr<Expression> left_expr = parse_and_expression(s, pos);
        if (!left_expr) return nullptr;

        while (pos < s.size())
        {
            string token = Helper::to_lower(s.substr(pos, 2));
            if (token != "or") break;

            pos += 2;
            while (pos < s.size() && isspace(s[pos])) pos++;

            unique_ptr<Expression> right_expr = parse_and_expression(s, pos);
            if (!right_expr) return nullptr;

            unique_ptr<Expression> new_root = make_unique<Expression>(Expression{
                Expression::NodeType::LOGICAL,
                OpKind::OR,
                "", "",
                move(left_expr),
                move(right_expr)
            });
            left_expr = move(new_root);
        }
        return left_expr;
    }


    unique_ptr<Expression> parse_where_clause(const string& where_raw)
    {
        size_t pos = 0;
        return parse_or_expression(where_raw, pos);
    }

    // =========================================================
    // 2. logic of (Expression Evaluation)
    // =========================================================

    const Value& get_column_value(const Table* table, const Row& row, const string& col_name) const
    {
        if (auto index = table->get_column_index(col_name))
        {
            return row.at(index.value());
        }
        throw runtime_error("Column '" + col_name + "' not found in row during evaluation.");
    }

    //  (شغاله بشكل تكراري main function of expression evaluationر)
    bool evaluate_expression(const Table* table, const Row& row, const Expression* expr) const
    {
        if (!expr) return true;

        if (expr->node_type == Expression::NodeType::LOGICAL)
        {
            bool left_result = evaluate_expression(table, row, expr->left.get());
            if (expr->op == OpKind::AND && !left_result) return false;
            if (expr->op == OpKind::OR && left_result) return true;

            bool right_result = evaluate_expression(table, row, expr->right.get());

            if (expr->op == OpKind::AND)
                return left_result && right_result;

            if (expr->op == OpKind::OR)
                return left_result || right_result;
        }
        else if (expr->node_type == Expression::NodeType::COMPARISON)
        {
            const Value& column_val = get_column_value(table, row, expr->column_name);

            // Comparing by models.cpp
            return column_val.compare_with_literal(expr->op, expr->literal_value);
        }

        return false;
    }

    // =========================================================
    // 3. query alayzing &execution
    // =========================================================

    bool parse_select_internal(const string &s, AST &out_ast)
    {
        string lower_s(Helper::to_lower(s));

        size_t from_pos = lower_s.find(" from ");
        if (from_pos == string::npos) {
            cout << "Syntax Error: Missing 'FROM' clause.\n";
            return false;
        }

        // find (COLS)
        string cols_raw = s.substr(6, from_pos - 6);
        auto columns = Helper::split_commas_respecting_quotes(cols_raw);
        if (columns.empty()) return false;

        vector<Text> clean_columns;
        for (const auto& col : columns) {
            clean_columns.push_back(Helper::trim(col));
        }

        // analyze (TABLE_NAME)
        size_t table_start = from_pos + 6;
        size_t where_pos = lower_s.find(" where ", table_start);

        string table_name_raw;
        if (where_pos != string::npos)
        {
            table_name_raw = s.substr(table_start, where_pos - table_start);
        }
        else
        {
            table_name_raw = s.substr(table_start);
        }

        Text table_name = Helper::trim(table_name_raw);
        Table *table = _catalog->getTable(table_name);
        if (table == nullptr)
        {
            cout << "Error: Table '" << table_name << "' not found.\n";
            return false;
        }

        //find WHERE
        Text where_clause_raw;
        if (where_pos != string::npos)
        {
            where_clause_raw = Helper::trim(s.substr(where_pos + 7));
            if (where_clause_raw.empty()) {
                 cout << "Error: Empty WHERE clause.\n";
                 return false;
            }
        }

        //WHERE
        unique_ptr<Expression> where_expression = nullptr;
        if (!where_clause_raw.empty())
        {
            where_expression = parse_where_clause(where_clause_raw);
            if (!where_expression)
            {
                cout << "Error: Invalid WHERE clause syntax.\n";
                return false;
            }
        }

        // AST
        AST ast;
        ast.kind = ASTKind::SELECT;
        AST_Select node;
        node.table_name = table_name;
        node.columns = move(clean_columns);
        node.where_condition = move(where_expression);

        ast.node = move(node);
        out_ast = move(ast);

        // **************** مرحلة التنفيذ الفعلية ****************
        cout << "\nExecuting SELECT on table '" << table->get_name() << "'\n";

        vector<Row> all_rows;
        if (!table->load_rows_from_disk(all_rows)) {
            cout << "Error: Failed to load data from disk.\n";
            return true;
        }

        int rows_matched = 0;

        // headers of cols
        cout << "----------------------------------------\n";
        for (const auto& col_name : node.columns) {
             cout << col_name << "\t";
        }
        cout << "\n----------------------------------------\n";

        for (const auto& row : all_rows)
        {
            if (evaluate_expression(table, row, node.where_condition.get()))
            {
                rows_matched++;

                for (const auto& req_col_name : node.columns)
                {
                    try {
                        const Value& val = get_column_value(table, row, req_col_name);
                        cout << val.to_string() << "\t";
                    } catch (const runtime_error& e) {
                        cout << "Error: " << e.what() << "\n";
                        return false;
                    }
                }
                cout << "\n";
            }
        }

        cout << "----------------------------------------\n";
        cout << rows_matched << " row(s) selected.\n";

        return true;
    }

public:
    SelectParser(Catalog *cat) : _catalog(cat) {}

    bool parse_and_select(const string &line, AST &out_ast)
    {
        string s(Helper::trim(line));
        if (s.empty()) return false;
        if (s.back() == ';') s.pop_back();

        string lower(Helper::to_lower(s));
        if (lower.find("select") != 0) return false;

        return parse_select_internal(s, out_ast);
    }
};

#endif
