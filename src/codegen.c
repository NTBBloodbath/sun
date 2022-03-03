#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "globals.h"

static int free_reg[4];

/**
 * @brief Registers table
 */
static char *reg_list[4] = {"%r8", "%r9", "%r10", "%r11"};

/**
 * @brief Registers table (8-bit)
 */
static char *breg_list[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};

/**
 * @brief Allocate a free register and die if no available registers
 *
 * @return Number of the register
 */
static int alloc_register() {
    for (int i = 0; i < 4; i++) {
        if (free_reg[i]) {
            free_reg[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Out of registers\n");
    exit(1);
}

/**
 * @brief Free a register and make it available
 *
 * @param reg [in] Number of the register
 */
static void free_register(int reg) {
    if (free_reg[reg] != 0) {
        printf("Register %d status: %s\n", reg, free_reg[reg] ? "available" : "occupied");
        fprintf(stderr, "Error trying to free register %d\n", reg);
        exit(1);
    }
    free_reg[reg] = 1;
}

/**
 * @brief Free all registers
 */
void free_all_registers() {
    free_reg[0] = free_reg[1] = free_reg[2] = free_reg[3] = 1;
    /* for (int i = 0; i < 4; i++) {
        free_reg[i] = 1;
    } */
}

/**
 * @brief Load an integer literal value into a register
 *
 * @param value [in] Integer literal to be loaded into a register
 * @return Number of the register
 */
int cg_load_int(int value) {
    // Get a new register
    int reg = alloc_register();

    // Print out the code to initialize it
    fprintf(sun_out_file, "\tmovq\t$%d, %s\n", value, reg_list[reg]);
    return reg;
}

/**
 * @brief Load a global variable into a register
 *
 * @param identifier [in] The global variable name
 * @return Number of the register
 */
int cg_load_global(char *identifier) {
    // Get a new register
    int reg = alloc_register();

    // Print out the code to initialize it
    fprintf(sun_out_file, "\tmovq\t%s(\%%rip), %s\n", identifier, reg_list[reg]);
    return reg;
}

/**
 * @brief Store a register's value into a global variable
 *
 * @param reg [in,out] Number of the register
 * @param identifier [in] The global variable name
 * @return Number of the register
 */
int cg_store_global(int reg, char *identifier) {
    fprintf(sun_out_file, "\tmovq\t%s, %s(\%%rip)\n", reg_list[reg], identifier);
    return reg;
}

/**
 * @brief Generate a new global symbol
 *
 * @param sym [in] Symbol name
 */
void cg_global_sym(char *sym) { fprintf(sun_out_file, "\t.comm\t%s, 8, 8\n", sym); }

/**
 * @brief Generate Assembly code for comparisons
 *
 * @param r1 [in] First register
 * @param r2 [in,out] Second register
 * @param kind [in] Comparison kind
 * @return Number of the register with the result
 */
static int cg_compare(int r1, int r2, char *kind) {
    // kind can be any setX instruction, that means it accepts:
    // - sete  (==)
    // - setne (!=)
    // - setl  (< )
    // - setle (<=)
    // - setg  (> )
    // - setge (>=)
    //
    // NOTE: 'cmpq r2 r1' is actually 'r1 - r2'
    fprintf(sun_out_file, "\tcmpq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    fprintf(sun_out_file, "\t%s\t%s\n", kind, breg_list[r2]);
    fprintf(sun_out_file, "\tandq\t$255, %s\n", reg_list[r2]);
    free_register(r1);

    return r2;
}

/**
 * @brief Compare two registers and check if they are equal
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_equal(int r1, int r2) { return cg_compare(r1, r2, "sete"); }

/**
 * @brief Compare two registers and check if they are not equal
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_not_equal(int r1, int r2) { return cg_compare(r1, r2, "setne"); }

/**
 * @brief Compare two registers and check if first if less than second
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_less_than(int r1, int r2) { return cg_compare(r1, r2, "setl"); }

/**
 * @brief Compare two registers and check if first if less or equal than second
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_less_equal(int r1, int r2) { return cg_compare(r1, r2, "setle"); }

/**
 * @brief Compare two registers and check if first if greater than second
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_greater_than(int r1, int r2) { return cg_compare(r1, r2, "setg"); }

/**
 * @brief Compare two registers and check if first if greater or equal than second
 *
 * @param r1 [in] First register
 * @param r2 [in] Second register
 * @return Number of the register with the result
 */
int cg_greater_equal(int r1, int r2) { return cg_compare(r1, r2, "setge"); }

/**
 * @brief Add two registers together
 *
 * @param r1 [in] First register
 * @param r2 [in,out] Second register
 * @return Number of the register with the result
 */
int cg_add(int r1, int r2) {
    fprintf(sun_out_file, "\taddq\t%s, %s\n", reg_list[r1], reg_list[r2]);
    free_register(r1);
    return r2;
}

/**
 * @brief Substract the second register from the first one
 *
 * @param r1 [in] First register
 * @param r2 [in,out] Second register
 * @return Number of the register with the result
 */
int cg_sub(int r1, int r2) {
    fprintf(sun_out_file, "\tsubq\t%s, %s\n", reg_list[r1], reg_list[r2]);
    free_register(r2);
    return r1;
}

/**
 * @brief Multiply two registers together
 *
 * @param r1 [in] First register
 * @param r2 [in,out] Second register
 * @return Number of the register with the result
 */
int cg_mul(int r1, int r2) {
    fprintf(sun_out_file, "\timulq\t%s, %s\n", reg_list[r1], reg_list[r2]);
    free_register(r1);
    return r2;
}

/**
 * @brief Add two registers together
 *
 * @param r1 [in] First register
 * @param r2 [in,out] Second register
 * @return Number of the register with the result
 */
int cg_div(int r1, int r2) {
    fprintf(sun_out_file, "\tmovq\t%s, %%rax\n", reg_list[r1]);
    fprintf(sun_out_file, "\tcqo\n");
    fprintf(sun_out_file, "\tidivq\t%s\n", reg_list[r2]);
    fprintf(sun_out_file, "\tmovq\t%%rax, %s\n", reg_list[r1]);
    free_register(r2);
    return r1;
}

/**
 * @brief Set the start of our Assembly code
 */
void cg_preamble() {
    // Make all registers available
    free_all_registers();
    // Set our functions and declare main as the global
    fputs(".LC0:\n"
          "\t.string\t\"%d\\n\"\n"
          "\t.globl\tmain\n"
          "\t.type\tmain, @function\n"
          "\n"
          ".text\n"
          "printint:\n"
          "\tpushq\t%rbp\n"
          "\tmovq\t%rsp, %rbp\n"
          "\tsubq\t$16, %rsp\n"
          "\tmovl\t%edi, -4(%rbp)\n"
          "\tmovl\t-4(%rbp), %eax\n"
          "\tmovl\t%eax, %esi\n"
          "\tleaq	.LC0(%rip), %rdi\n"
          "\tmovl	$0, %eax\n"
          "\tcall	printf@PLT\n"
          "\tnop\n"
          "\tleave\n"
          "\tret\n"
          "\n"
          "main:\n"
          "\tpushq\t%rbp\n"
          "\tmovq	%rsp, %rbp\n",
          sun_out_file);
}

/**
 * @brief Set the end of our Assembly code
 */
void cg_postamble() {
    // Set our return code
    fputs("\tmovl $0, %eax\n"
          "\tpopq %rbp\n"
          "\tret\n",
          sun_out_file);
}

/**
 * @brief Call ASM printint() function with the given register
 *
 * @param r [in] Register to be printed
 */
void cg_printint(int r) {
    fprintf(sun_out_file, "\tmovq\t%s, %%rdi\n", reg_list[r]);
    fprintf(sun_out_file, "\tcall\tprintint\n");
    free_register(r);
}
