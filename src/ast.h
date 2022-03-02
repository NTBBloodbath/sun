#ifndef SUN_AST_H
#define SUN_AST_H

struct AST_node {
    int op;
    struct AST_node *lhs;
    struct AST_node *rhs;
    int int_value;
};

enum {
    A_ADD,      // +
    A_MINUS,    // -
    A_MULTIPLY, // *
    A_DIVIDE,   // /
    A_INT,      // 10
};


struct AST_node *make_ast_node(int op, struct AST_node *left, struct AST_node *right, int int_value);
struct AST_node *make_ast_unary(int op, struct AST_node *left, int int_value);
struct AST_node *make_ast_leaf(int op, int int_value);
#endif
