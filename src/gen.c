#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "codegen.h"

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
            return cg_add(l_reg, r_reg);
        case A_MINUS:
          return cg_sub(l_reg, r_reg);
        case A_MULTIPLY:
            return cg_mul(l_reg, r_reg);
        case A_DIVIDE:
            return cg_div(l_reg, r_reg);
        case A_INT:
            return cg_load(node->int_value);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", node->op);
            exit(1);
    }
    exit(1);
}

/**
 * @brief Call cg_preamble()
 */
void gen_preamble() {
    cg_preamble();
}

/**
 * @brief Call cg_postamble()
 */
void gen_postamble() {
    cg_postamble();
}

/**
 * @brief Call free_all_registers()
 */
void gen_free_registers() {
    free_all_registers();
}

/**
 * @brief Call cg_printint(int r)
 *
 * @param reg [in] Number of the register
 */
void gen_printint(int reg) {
    cg_printint(reg);
}

void gen_code(struct AST_node *node) {
    int reg;

    // Set the start of our ASM code (setting global, functions, etc)
    cg_preamble();
    // Get the register from our generated ASM
    reg = gen_ast(node);
    // Print the register with the result as an integer
    cg_printint(reg);
    // Set the end of our ASM code (return code)
    cg_postamble();
}
