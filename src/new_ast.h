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
    SUN_AST_TYPE_NUMBER,
    // SUN_AST_VARIABLE,
    SUN_AST_TYPE_BIN_EXPR,
} SUN_AST_NODE_TYPE_E;

/*
 * @brief Sun AST binary operators enum
 */
typedef enum {
    SUN_BINOP_ADD,
    SUN_BINOP_SUB,
    SUN_BINOP_MUL,
    SUN_BINOP_DIV,
} SUN_AST_BINOP_E;

/*
 * @brief Sun AST type for binary expressions
 */
typedef struct SUN_AST_BIN_EXPR {
    SUN_AST_BINOP_E operator;
    struct SUN_AST_NODE *lhs;
    struct SUN_AST_NODE *rhs;
} SUN_AST_BIN_EXPR;

/*
 * @brief Sun AST for Number type
 */
typedef struct SUN_AST_NUMBER {
    char *type;
    int value;
} SUN_AST_NUMBER;

/*
 * @brief Sun AST Node type
 */
typedef struct SUN_AST_NODE {
    SUN_AST_NODE_TYPE_E type;
    union {
        SUN_AST_NUMBER number;
        SUN_AST_BIN_EXPR bin_expr;
    };
} SUN_AST_NODE;

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
