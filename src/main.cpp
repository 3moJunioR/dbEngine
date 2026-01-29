#include "../include/CreateParse.cpp"
#include "../include/InsertParser.cpp"
#include "../include/SelectParser.cpp"
#include "../include/UpdateParser.cpp"
#include "../include/DeleteParser.cpp"

int main()
{
    cout << "\nWelcome to mini database engine\n"
         << "Type 'help' for commands\n";

    CreateParser create_parser;
    Catalog *catalog(&create_parser.catalog());
    InsertParser insert_parser(catalog);
    SelectParser select_parser(catalog);
    UpdateParser update_parser(catalog);
    DeleteParser delete_parser(catalog);

    Helper::load_existing_tables(catalog);

    string line;
    cout << "SQL> ";
    while (getline(cin, line))
    {
        string cmd(Helper::trim(line));
        if (cmd.empty())
        {
            cout << "SQL> ";
            continue;
        }

        string lower(Helper::to_lower(cmd));

        if (lower == "help" || lower == "?")
        {
            Helper::show_help();
            cout << "SQL> ";
            continue;
        }

        if (lower == "exit" || lower == "quit")
        {
            cout << "\nGoodbye!\n";
            break;
        }

        AST ast;
        bool success(false);

        if (Helper::starts_with_prefix(cmd, "create"))
            success = create_parser.parse_and_create(line, ast);
        else if (Helper::starts_with_prefix(cmd, "insert"))
            success = insert_parser.parse_and_insert(line, ast);
        else if (Helper::starts_with_prefix(cmd, "select"))
            success = select_parser.parse_and_select(line, ast);
        else if (Helper::starts_with_prefix(cmd, "update"))
            success = update_parser.parse_and_update(line, ast);
        else if (Helper::starts_with_prefix(cmd, "delete"))
            success = delete_parser.parse_and_delete(line, ast);
        else
        {
            cout << "\nUnknown SQL command: '" << cmd << "'\n"
                 << "Type 'help' to see available commands\n";
        }

        if (!success && (Helper::starts_with_prefix(cmd, "create") || Helper::starts_with_prefix(cmd, "insert") || Helper::starts_with_prefix(cmd, "select") || Helper::starts_with_prefix(cmd, "update") || Helper::starts_with_prefix(cmd, "delete")))
        {
            cout << "Syntax error. Type 'help' for correct syntax\n";
        }

        cout << "SQL> ";
    }

    return (0);
}
