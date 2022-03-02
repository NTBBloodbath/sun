#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "globals.h"

static int free_reg[4];

/**
 * @brief Registers table
 */
static char *reg_list[4] = { "%r8", "%r9", "%r10", "%r11" };

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
int cg_load(int value) {
    // Get a new register
    int reg = alloc_register();

    // Print out the code to initialise it
    fprintf(sun_out_file, "\tmovq\t$%d, %s\n", value, reg_list[reg]);
    return reg;
}

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
    fputs(
        ".LC0:\n"
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
    fputs(
        "\tmovl $0, %eax\n"
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
