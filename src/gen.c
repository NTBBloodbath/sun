#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

/**
 * @brief Generate Assembly code recursively from given AST
 *
 * @param node [in] AST node
 * @return Assembly code
 */
int gen_ast(struct AST_node *node) {
    int l_reg = 0;
    int r_reg = 0;

    // Get the left and right sub-tree values
    if (node->lhs) {
        l_reg = gen_ast(node->lhs);
    }
    if (node->rhs) {
        r_reg = gen_ast(node->rhs);
    }

    switch (node->op) {
        case A_ADD:
            return (l_reg + r_reg);
        case A_MINUS:
          return (l_reg - r_reg);
        case A_MULTIPLY:
            return (l_reg * r_reg);
        case A_DIVIDE:
            return (l_reg / r_reg);
        case A_INT:
            return (node->int_value);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", node->op);
            exit(1);
    }
    exit(1);
}
