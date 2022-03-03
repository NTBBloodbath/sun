#include <stdlib.h>

#include "ast.h"
#include "gen.h"
#include "sym.h"
#include "decl.h"
#include "utils.h"
#include "globals.h"
#include "token.h"
#include "tokenizer.h"

void print_statement() {
    struct AST_node *tree;
    int reg;

    // Match a 'print' word as the first token
    match(T_PRINT, "print");

    // Parse the following expression and generate
    // the Assembly code for it
    tree = bin_expr(0);
    reg = gen_ast(tree, -1);
    gen_printint(reg);
    gen_free_registers();

    // Match the following semicolon
    semi();
}

void assignment_statement(void) {
    struct AST_node *lhs, *rhs, *tree;
    int id;

    // Ensure we have an identifier
    identifier();

    // Check it's been defined then make a leaf node for it
    if ((id = find_glob(Text)) == -1) {
        fprintf(stderr, "Undeclared variable '%s' on line %d\n", Text, curr_line);
        exit(1);
    }

    rhs = make_ast_leaf(A_LVIDENT, id);

    // Ensure we have an equals sign
    match(T_ASSIGN, "=");

    // Parse the following expression
    lhs = bin_expr(0);

    // Make an assignment AST tree
    tree = make_ast_node(A_ASSIGN, lhs, rhs, 0);

    // Generate the assembly code for the assignment
    gen_ast(tree, -1);
    gen_free_registers();

    // Match the following semicolon
    semi();
}

void statements() {
    while (1) {
        switch (Token.token) {
            case T_PRINT:
                print_statement();
                break;
            case T_LET:
                var_declaration();
                break;
            case T_IDENTIFIER:
                assignment_statement();
                break;
            case T_EOF:
                return;
            default:
                fprintf(stderr, "Syntax error, token '%d' on line '%d'\n", Token.token, curr_line);
                exit(1);
        }
    }
}
