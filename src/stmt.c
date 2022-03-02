#include "ast.h"
#include "gen.h"
#include "utils.h"
#include "globals.h"
#include "token.h"
#include "tokenizer.h"

void statements() {
    int reg;
    struct AST_node *tree;

    while (1) {
        // Match a 'print' word as the first token
        match(T_PRINT, "print");

        // Parse the following expression and generate
        // the Assembly code for it
        tree = bin_expr(0);
        reg = gen_ast(tree);
        gen_printint(reg);
        gen_free_registers();

        // Match the following semicolon and stop parsing
        // if we reach EOF
        semi();
        if (Token.token == T_EOF) {
            return;
        }
    }
}
