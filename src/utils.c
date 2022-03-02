#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "globals.h"

/**
 * @brief Check that the current token is t and fetch the next token, throw an error if not
 *
 * @param t [in] Expected token type
 * @param str [in] Expected token
 */
void match(int t, char *str) {
    if (Token.token != t) {
        fprintf(stderr, "'%s' expected on line '%d'\n", str, curr_line);
        exit(1);
    }

    scan(&Token);
}

/**
 * @brief Match a semicolon and fetch the next token
 */
void semi() {
    match(T_SEMI, ";");
}
