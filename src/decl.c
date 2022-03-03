#include "ast.h"
#include "gen.h"
#include "sym.h"
#include "token.h"
#include "utils.h"
#include "scanner.h"
#include "globals.h"
#include "tokenizer.h"

/**
 * @brief Declare a variable and save it into the global scope
 */
void var_declaration() {
    // The global slot where the variable is going to be saved
    int var_slot;

    // Ensure we have a 'let' token followed by an identifier,
    // a colon, a type and a semicolon. Text now has the identifier's name.
    // Add it as a known identifier.
    match(T_LET, "let");
    identifier();
    // Save the global variable slot
    var_slot = add_glob(Text);
    gen_global_sym(Text);
    colon();
    match(T_INT, "int");

    // If we have found EQ token then let's also declare the variable value
    if (Token.token == T_EQ) {
        scan(&Token);

        struct AST_node *lhs, *rhs, *tree;
        // Get the global variable identifier (name)
        char *var_name = Gsym[var_slot].name;
        int id = find_glob(var_name);

        rhs = make_ast_leaf(A_LVIDENT, id);
        lhs = bin_expr(0);
        tree = make_ast_node(A_ASSIGN, lhs, rhs, 0);

        gen_ast(tree, -1);
        gen_free_registers();
    }

    semi();
}
