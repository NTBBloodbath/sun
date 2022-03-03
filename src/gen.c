#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "codegen.h"
#include "globals.h"
#include "sym.h"

/**
 * @brief Generate Assembly code recursively from given AST
 *
 * @param node [in] AST node
 * @return Assembly code
 */
int gen_ast(struct AST_node *node, int reg) {
    int l_reg = 0;
    int r_reg = 0;

    // Get the left and right sub-tree values
    if (node->lhs) {
        l_reg = gen_ast(node->lhs, -1);
    }
    if (node->rhs) {
        r_reg = gen_ast(node->rhs, l_reg);
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
    case A_EQ:
        return cg_equal(l_reg, r_reg);
    case A_NE:
        return cg_not_equal(l_reg, r_reg);
    case A_LT:
        return cg_less_than(l_reg, r_reg);
    case A_LE:
        return cg_less_equal(l_reg, r_reg);
    case A_GT:
        return cg_greater_than(l_reg, r_reg);
    case A_GE:
        return cg_greater_equal(l_reg, r_reg);
    case A_INTEGER:
        return cg_load_int(node->value.int_value);
    case A_IDENTIFIER:
        return cg_load_global(Gsym[node->value.id].name);
    case A_LVIDENT:
        return cg_store_global(reg, Gsym[node->value.id].name);
    case A_ASSIGN:
        return r_reg;
    default:
        fprintf(stderr, "Unknown AST operator %d\n", node->op);
        exit(1);
    }
    exit(1);
}

/**
 * @brief Call cg_global_sym(char *sym)
 *
 * @param sym [in] Symbol name
 */
void gen_global_sym(char *sym) { cg_global_sym(sym); }

/**
 * @brief Call cg_preamble()
 */
void gen_preamble() { cg_preamble(); }

/**
 * @brief Call cg_postamble()
 */
void gen_postamble() { cg_postamble(); }

/**
 * @brief Call free_all_registers()
 */
void gen_free_registers() { free_all_registers(); }

/**
 * @brief Call cg_printint(int r)
 *
 * @param reg [in] Number of the register
 */
void gen_printint(int reg) { cg_printint(reg); }
