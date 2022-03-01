#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

// List of AST operators
// static char *AST_op[] = { "+", "-", "*", "/" };

int interpret_ast(struct AST_node *ast) {
    int lhs = 0;
    int rhs = 0;

    // Get the left and right sub-tree values
    if (ast->left) {
        lhs = interpret_ast(ast->left);
    }
    if (ast->right) {
        rhs = interpret_ast(ast->right);
    }

    // DEBUG: Print what we are about to do
    /* if (ast->op == A_INT) {
        printf("int %d\n", ast->int_value);
    } else {
        printf("%d %s %d\n", lhs, AST_op[ast->op], rhs);
    } */

    switch (ast->op) {
        case A_ADD:
            return (lhs + rhs);
        case A_MINUS:
          return (lhs - rhs);
        case A_MULTIPLY:
            return (lhs * rhs);
        case A_DIVIDE:
            return (lhs / rhs);
        case A_INT:
            return (ast->int_value);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", ast->op);
            exit(1);
    }
    return 1;
}
