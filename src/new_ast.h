// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_NEW_AST_H
#define SUN_NEW_AST_H

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
    ADD,
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

// enum {
//     A_ADD = 1,    // +
//     A_MINUS,      // -
//     A_MULTIPLY,   // *
//     A_DIVIDE,     // /
//     A_EQ,         // ==
//     A_NE,         // !=
//     A_LT,         // <
//     A_GT,         // >
//     A_LE,         // <=
//     A_GE,         // >=
//     A_INTEGER,    // 10
//     A_IDENTIFIER, // hello
//     A_LVIDENT,    // left-hand side variable
//     A_ASSIGN,     // =
// };
//
// struct AST_node *make_ast_node(int op, struct AST_node *left, struct AST_node *right,
//                                int int_value);
// struct AST_node *make_ast_unary(int op, struct AST_node *left, int int_value);
// struct AST_node *make_ast_leaf(int op, int int_value);
#endif
