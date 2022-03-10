// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <stdio.h>

#include "ast.h"
#include "memory.h"

/**
 * @brief Make an AST node for an integer
 *
 * @param value [in] Integer value
 * @return AST Node
 */
sun_ast_node_st *make_ast_integer(int value) {
    sun_ast_node_st *node = xmalloc(sizeof(sun_ast_node_st));
    node->type = NUMBER;
    node->number.value = value;

    return node;
}

/**
 * @brief Make an AST node for a binary expression
 *
 * @param operator [in] Binary expression operator
 * @param lhs [in] Left-side expression
 * @param rhs [in] Right-side expression
 * @return AST Node
 */
sun_ast_node_st *make_ast_binexpr(sun_ast_binop_et operator, sun_ast_node_st * lhs,
                                  sun_ast_node_st *rhs) {
    sun_ast_node_st *node = xmalloc(sizeof(sun_ast_node_st));
    node->type = BIN_EXPR;
    node->bin_expr.lhs = lhs;
    node->bin_expr.rhs = rhs;
    node->bin_expr.operator= operator;

    return node;
}

/**
 * @brief Free an AST node from the memory
 *
 * @param node [in] AST Node
 */
void free_ast_node(sun_ast_node_st *node) {
    if (!node) {
        return;
    }

    // Recursively free child nodes
    switch (node->type) {
        case NUMBER:
            break;
        case BIN_EXPR:
            if (node->bin_expr.lhs) {
                free_ast_node(node->bin_expr.lhs);
            }
            if (node->bin_expr.rhs) {
                free_ast_node(node->bin_expr.rhs);
            }
            break;
    }

    xfree(node);
}
