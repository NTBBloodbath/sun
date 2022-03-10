// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
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
struct sun_ast_node_st *make_ast_node(int op, struct sun_ast_node_st *lhs, struct sun_ast_node_st *rhs, int int_value) {
    struct sun_ast_node_st *node;

    // Allocate a new AST node in memory
    node = (struct sun_ast_node_st *)xmalloc(sizeof(struct sun_ast_node_st));
    node->bin_expr.operator = op;
    node->bin_expr.lhs = lhs;
    node->bin_expr.rhs = rhs;
    node->number.value = int_value;

    return node;
}

/**
 * @brief Make a unary AST node with only one children node
 *
 * @param op [in] The operator
 * @param lhs [in] The lhs AST node
 * @param int_value [in] An integer literal value
 */
struct sun_ast_node_st *make_ast_unary(int op, struct sun_ast_node_st *lhs, int int_value) {
    return make_ast_node(op, lhs, NULL, int_value);
}

/**
 * @brief Make a leaf AST node with no children nodes
 *
 * @param op [in] The operator
 * @param int_value [in] An integer literal value
 */
struct sun_ast_node_st *make_ast_leaf(int op, int int_value) {
    return make_ast_node(op, NULL, NULL, int_value);
}
