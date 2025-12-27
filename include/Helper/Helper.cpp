#ifndef HELPER
#define HELPER

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <utility>
#include "models.cpp"

using namespace std;
namespace fs = filesystem;

class Helper
{
public:
    static string trim(const string &str)
    {
        int start(0);
        while (start < str.size() && isspace(str[start]))
            ++start;

        if (start == str.size())
            return "";

        int end(str.size() - 1);
        while (end > start && isspace(str[end]))
            --end;

        return str.substr(start, end - start + 1);
    }
    static string to_lower(const string &str)
    {
        string result(str);
        for (char &c : result)
            c = tolower(c);
        return result;
    }

    static pair<int, int> find_top_level_parens(const string &str)
    {
        return {-1, -1};
    }

    static vector<string> split_commas_respecting_quotes(const string &str)
    {
        vector<string> result;
        string current;
        bool in_single_quote(false), in_double_quote(false);
        int paren_depth(0);

        for (int i(0); i < str.size(); ++i)
        {
            char c = str[i];

            if (c == '\'' && !in_double_quote) { in_single_quote = !in_single_quote; current += c; }
            else if (c == '"' && !in_single_quote) { in_double_quote = !in_double_quote; current += c; }
            else if (c == '\\') { current += c; current += str[++i]; }
            else if (!in_single_quote && !in_double_quote)
            {
                if (c == '(') { ++paren_depth; current += c; }
                else if (c == ')') { if (paren_depth > 0) --paren_depth; current += c; }
                else if (c == ',' && !paren_depth)
                {
                    result.push_back(trim(current));
                    current.clear();
                } else {
                    current += c;
                }
            } else {
                current += c;
            }
        }

        if (!trim(current).empty()) result.push_back(trim(current));
        return result;
    }

    static fs::path csv_path(const string &table_name)
    {
        return fs::path("../data") / table_name / (table_name + ".csv");
    }

    static fs::path meta_path(const string &table_name)
    {
        return fs::path("../data") / table_name / (table_name + ".meta");
    }

    static void ensure_data_dir()
    {
        fs::path data_dir = "../data";
        if (!fs::exists(data_dir))
            fs::create_directory(data_dir);
    }
};

#endif // HELPER
