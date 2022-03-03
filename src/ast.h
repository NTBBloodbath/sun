#ifndef SUN_AST_H
#define SUN_AST_H

struct AST_node {
    int op;
    struct AST_node *lhs;
    struct AST_node *rhs;
    union {
        int id;        // Symbol slot number, A_IDENTIFIER
        int int_value; // Integer value, A_INT
    } value;
};

enum {
    A_ADD = 1,    // +
    A_MINUS,      // -
    A_MULTIPLY,   // *
    A_DIVIDE,     // /
    A_EQ,         // ==
    A_NE,         // !=
    A_LT,         // <
    A_GT,         // >
    A_LE,         // <=
    A_GE,         // >=
    A_INTEGER,    // 10
    A_IDENTIFIER, // hello
    A_LVIDENT,    // left-hand side variable
    A_ASSIGN,     // =
};


struct AST_node *make_ast_node(int op, struct AST_node *left, struct AST_node *right, int int_value);
struct AST_node *make_ast_unary(int op, struct AST_node *left, int int_value);
struct AST_node *make_ast_leaf(int op, int int_value);
#endif
