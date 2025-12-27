#ifndef CREATE_PARSER
#define CREATE_PARSER
#include "models.cpp"
class CreateParser {
private: Catalog* _catalog; public:
    CreateParser(Catalog *cat) : _catalog(cat) {}
    bool parse_and_create(const string& line, AST& out_ast) {
        //create logic here
        return false;
    }
};
#endif
