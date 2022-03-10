// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "globals.h"
#include "scanner.h"
#include "token.h"

/**
 * @brief Tokens precedence table
 */
static int op_prec[] = {
    0,  // EOF
    10, // +
    10, // -
    20, // *
    20, // /
    30, // ==
    30, // !=
    40, // <
    40, // <=
    40, // >
    40, // >=
};

/**
 * @brief Get the precedence of the given token kind
 *
 * @param token_kind [in] Token kind to get the precedence from
 * @return Token precedence (int)
 */
static int op_precedence(int token_kind) {
    int prec = op_prec[token_kind];
    if (prec == 0) {
        fprintf(stderr, "Syntax error on line %d, token %d\n", curr_line, token_kind);
        exit(1);
    }

    return prec;
}

/**
 * @brief Convert a token into an AST operation
 *
 * @param token [in] Token to be converted
 * @return AST operation
 */
int arithmetic_op(int token) {
    if (token > T_EOF && token < T_INTEGER) {
        return token;
    }
    fprintf(stderr, "Unknown token '%d' in 'arithmetic_op' on line %d\n", token, curr_line);
    exit(1);
}

/**
 * @brief Parse a primary factor and return an AST node representing it
 *
 * @return AST node
 */
static struct sun_ast_node_st *primary() {
    struct sun_ast_node_st *node;

    switch (Token.token) {
        case T_INTEGER:
            node = make_ast_leaf(NUMBER, Token.int_value);
            break;
        default:
            fprintf(stderr, "Syntax error on line %d, got token '%d'\n", curr_line, Token.token);
            exit(1);
    }
    // Scan in the next token
    scan(&Token);

    return node;
}

/**
 * @brief Make an AST tree whose root is a binary operator
 *
 * @param prev_token_prec [in] Previous token precedence
 */
struct sun_ast_node_st *bin_expr(int prev_token_prec) {
    struct sun_ast_node_st *lhs, *rhs;
    int token_type;

    // Get the integer literal on the left.
    // Fetch the next token at the same time.
    lhs = primary();

    token_type = Token.token;
    // If no tokens left, return just the left node
    if (token_type == T_SEMI) {
        return lhs;
    }

    // While the precedence of this token is more than that
    // of the previous token precedence then recursively make
    // a sub-tree
    while (op_precedence(token_type) > prev_token_prec) {
        // Fetch in the next integer literal
        scan(&Token);

        // Make a sub-tree with the precedence of our current token
        rhs = bin_expr(op_prec[token_type]);

        // Join our sub-tree with our tree and convert the token into an AST operation
        lhs = make_ast_node(arithmetic_op(token_type), lhs, rhs, 0);

        // Update our current token and return just the left node if no more tokens left
        token_type = Token.token;
        if (token_type == T_SEMI) {
            return lhs;
        }
    }

    // Return the tree we have when the precedence is the same or lower (associativity)
    return lhs;
}
