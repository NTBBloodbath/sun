#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

int interpret_ast(struct AST_node *node) {
    int lhs = 0;
    int rhs = 0;

    // Get the left and right sub-tree values
    if (node->lhs) {
        lhs = interpret_ast(node->lhs);
    }
    if (node->rhs) {
        rhs = interpret_ast(node->rhs);
    }

    // DEBUG: Print what we are about to do
    // List of AST operators
    /* static char *AST_op[] = { "+", "-", "*", "/" };
    if (node->op == A_INT) {
        printf("int %d\n", node->int_value);
    } else {
        printf("%d %s %d\n", lhs, AST_op[node->op], rhs);
    } */

    switch (node->op) {
        case A_ADD:
            return (lhs + rhs);
        case A_MINUS:
          return (lhs - rhs);
        case A_MULTIPLY:
            return (lhs * rhs);
        case A_DIVIDE:
            return (lhs / rhs);
        case A_INT:
            return (node->int_value);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", node->op);
            exit(1);
    }
    return 1;
}
