// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_AST_H
#define SUN_AST_H

/*
 * @brief Sun AST node types enum
 */
typedef enum {
    NUMBER,
    // SUN_AST_VARIABLE,
    BIN_EXPR,
} sun_ast_node_type_et;

/*
 * @brief Sun AST binary operators enum
 */
typedef enum {
    ADD = 1, // Start at 1 so operators will match tokens enum order
    SUB,
    MUL,
    DIV,
} sun_ast_binop_et;

/*
 * @brief Sun AST type for binary expressions
 */
typedef struct sun_ast_bin_expr_st {
    struct sun_ast_node_st *lhs;
    struct sun_ast_node_st *rhs;
    sun_ast_binop_et operator;
} sun_ast_bin_expr_st;

/*
 * @brief Sun AST for Number type
 */
typedef struct sun_ast_number_st {
    char *type;
    int value;
} sun_ast_number_st;

/*
 * @brief Sun AST Node type
 */
typedef struct sun_ast_node_st {
    sun_ast_node_type_et type;
    union {
        sun_ast_number_st number;
        sun_ast_bin_expr_st bin_expr;
    };
} sun_ast_node_st;

sun_ast_node_st *make_ast_integer(int value);
sun_ast_node_st *make_ast_binexpr(sun_ast_binop_et operator, sun_ast_node_st * lhs,
                                  sun_ast_node_st *rhs);
void free_ast_node(sun_ast_node_st *node);
#endif
