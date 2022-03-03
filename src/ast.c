#include "ast.h"
#include "memory.h"

/**
 * @brief Make a new AST node and allocate it on memory
 *
 * @param op [in] The operator
 * @param lhs [in] The lhs AST node
 * @param rhs [in] The rhs AST node
 * @param int_value [in] An integer literal value
 * @return 
 */
struct AST_node *make_ast_node(int op, struct AST_node *lhs, struct AST_node *rhs, int int_value) {
    struct AST_node *node;

    // Allocate a new AST node in memory
    node = (struct AST_node *)xmalloc(sizeof(struct AST_node));
    node->op = op;
    node->lhs = lhs;
    node->rhs = rhs;
    node->value.int_value = int_value;

    return node;
}

/**
 * @brief Make a unary AST node with only one children node
 *
 * @param op [in] The operator
 * @param lhs [in] The lhs AST node
 * @param int_value [in] An integer literal value
 */
struct AST_node *make_ast_unary(int op, struct AST_node *lhs, int int_value) {
    return make_ast_node(op, lhs, NULL, int_value);
}

/**
 * @brief Make a leaf AST node with no children nodes
 *
 * @param op [in] The operator
 * @param int_value [in] An integer literal value
 */
struct AST_node *make_ast_leaf(int op, int int_value) {
    return make_ast_node(op, NULL, NULL, int_value);
}
