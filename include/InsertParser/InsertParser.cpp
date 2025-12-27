// داخل InsertParser.cpp
#ifndef INSERT_PARSER
#define INSERT_PARSER
#include "models.cpp"
class InsertParser {
private: Catalog* _catalog; public:
    InsertParser(Catalog *cat) : _catalog(cat) {}
    bool parse_and_insert(const string& line, AST& out_ast) {
        //logic insert here
        return false;
    }
};
#endif
