#include <iostream>
#include <string>
#include <sstream>

// تضمين جميع المكونات
#include "../include/models.cpp"
#include "../include/Helper.cpp"
#include "../include/Expression.cpp"
#include "../include/CreateParser.cpp"
#include "../include/InsertParser.cpp"
#include "../include/SelectParser.cpp"

using namespace std;

void load_existing_tables(Catalog* catalog) {
    Helper::ensure_data_dir();
}

void execute_query(const string& query, Catalog* catalog) {
    string lower_query = Helper::to_lower(query);
    AST ast;

    // using Parser whitch exist at include
    if (lower_query.find("create table") == 0) {
        CreateParser parser(catalog);
        if (parser.parse_and_create(query, ast)) {
            cout << "Query OK.\n";
        } else {
            cout << "Query FAILED.\n";
        }
    }
    else if (lower_query.find("insert into") == 0) {
        InsertParser parser(catalog);
        if (parser.parse_and_insert(query, ast)) {
            cout << "Query OK.\n";
        } else {
            cout << "Query FAILED.\n";
        }
    }
    else if (lower_query.find("select") == 0) {
        SelectParser parser(catalog);
        // SelectParser here executing and show results immidiate
        if (!parser.parse_and_select(query, ast)) {
            cout << "Query FAILED.\n";
        }
    }
    else {
        cout << "\nError: Unknown command or syntax error.\n";
    }
}

int main() {
    Catalog catalog;
    load_existing_tables(&catalog);

    cout << "Mini Database Engine Initialized. Enter SQL queries.\n";

    string line;
    while (true) {
        cout << "\ndb> ";
        getline(cin, line);

        string clean_line = Helper::trim(line);

        if (Helper::to_lower(clean_line) == "exit" || Helper::to_lower(clean_line) == "quit") {
            cout << "Exiting database engine. Goodbye!\n";
            break;
        }

        if (!clean_line.empty()) {
            execute_query(clean_line, &catalog);
        }
    }

    return 0;
}
