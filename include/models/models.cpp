#ifndef MODELS
#define MODELS

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <variant>
#include <optional>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

enum class OpKind;
struct Expression;

using Text = string;

struct Date
{
    int year = 0;
    int month = 0;
    int day = 0;

    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}
    string to_string() const
    {
        if (year == 0) return "NULL";
        return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
    }
    bool operator==(const Date& other) const
    {
        return year == other.year && month == other.month && day == other.day;
    }
    bool operator<(const Date& other) const
    {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    bool operator>(const Date& other) const
    {
        return other < *this;
    }
    bool operator<=(const Date& other) const
    {
        return (*this < other) || (*this == other);
    }
    bool operator>=(const Date& other) const
    {
        return (*this > other) || (*this == other);
    }
    bool operator!=(const Date& other) const
    {
        return !(*this == other);
    }
};

using DataVariant = variant<monostate, int, double, string, Date>;

class Value
{
private:
    DataVariant data;
public:
    Value() : data(monostate{}) {}
    Value(int i) : data(i) {}
    Value(double d) : data(d) {}
    Value(const string& s) : data(s) {}
    Value(const Date& d) : data(d) {}

    bool is_null() const
    {
        return holds_alternative<monostate>(data);
    }

    string to_string() const
    {
        if (is_null()) return "NULL";
        if (holds_alternative<int>(data)) return std::to_string(get<int>(data));
        if (holds_alternative<double>(data)) return std::to_string(get<double>(data));
        if (holds_alternative<string>(data)) return get<string>(data);
        if (holds_alternative<Date>(data)) return get<Date>(data).to_string();
        return "";
    }

    bool compare_with_literal(OpKind op, const string& literal) const;
};

class Column
{
private:
    Text _name;
    Text _type;
    int _char_length = 0;
    bool _is_null = true;
    bool _is_pk = false;

public:
    Column(const Text& name, const Text& type, int len = 0, bool is_null = true, bool is_pk = false)
        : _name(name), _type(type), _char_length(len), _is_null(is_null), _is_pk(is_pk) {}

    const Text& get_name() const
    {
        return _name;
    }
    const Text& get_type() const
    {
        return _type;
    }
    int get_char_length() const
    {
        return _char_length;
    }
    bool is_null() const
    {
        return _is_null;
    }
    bool is_primary_key() const
    {
        return _is_pk;
    }
};

using Row = vector<Value>;

class Table
{
private:
    Text _name;
    vector<Column> _columns;
    vector<int> _pk_indices;
    map<string, Row> _primary_keys;

public:
    Table(const Text& name) : _name(name) {}

    void add_column(const Column& col)
    {
        _columns.push_back(col);
    }

    void set_primary_key(const vector<string>& pk_cols)
    {
        _pk_indices.clear();
        for (const auto& pk_name : pk_cols)
        {
            for (size_t i = 0; i < _columns.size(); ++i)
            {
                if (_columns[i].get_name() == pk_name)
                {
                    _pk_indices.push_back(i);
                    break;
                }
            }
        }
    }

    bool load_rows_from_disk(vector<Row>& out_rows) const
    {
        return true;
    }

    void insert_row(const Row& row)
    {
    }

    const Text& get_name() const
    {
        return _name;
    }
    const vector<Column>& get_columns() const
    {
        return _columns;
    }
    optional<int> get_column_index(const string& col_name) const
    {
        for (size_t i = 0; i < _columns.size(); ++i)
        {
            if (_columns[i].get_name() == col_name) return i;
        }
        return {};
    }
};

class Catalog
{
private:
    map<Text, unique_ptr<Table>> _tables;

public:
    Table* getTable(const Text& name)
    {
        auto it = _tables.find(name);
        if (it != _tables.end()) return it->second.get();
        return nullptr;
    }

    bool addTable(unique_ptr<Table> table)
    {
        if (getTable(table->get_name())) return false;
        _tables[table->get_name()] = move(table);
        return true;
    }

    const map<Text, unique_ptr<Table>>& get_all_tables() const
    {
        return _tables;
    }
};

enum class ASTKind { NONE, CREATE, INSERT, SELECT };

struct AST_Create
{
    Text table_name;
    vector<Column> columns;
    vector<Text> primary_key_cols;
};

struct AST_Insert
{
    Text table_name;
    Row row;
};

struct AST_Select
{
    Text table_name;
    vector<Text> columns;
    unique_ptr<Expression> where_condition;
};


using ASTNode = variant<monostate, AST_Create, AST_Insert, AST_Select>;

struct AST
{
    ASTKind kind = ASTKind::NONE;
    ASTNode node;
};


#endif
