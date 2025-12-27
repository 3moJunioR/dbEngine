#ifndef EXPRESSION
#define EXPRESSION

#include <string>
#include <vector>
#include <memory>
#include "models.cpp"

using namespace std;

enum class OpKind
{
    EQ, NEQ, LT, LTE, GT, GTE,
    AND, OR
};

struct Expression
{
    enum class NodeType { COMPARISON, LOGICAL };

    NodeType node_type;
    OpKind op;

    string column_name;
    string literal_value;

    unique_ptr<Expression> left;
    unique_ptr<Expression> right;
};

#endif // EXPRESSION
