#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "scanner.h"

/**
 * @brief Check that the current token is t and fetch the next token, throw an error if not
 *
 * @param t [in] Expected token type
 * @param str [in] Expected token
 */
void match(int t, char *str) {
    if (Token.token != t) {
        fprintf(stderr, "'%s' expected on line '%d', got '%d'\n", str, curr_line, Token.token);
        exit(1);
    }

    scan(&Token);
}

/**
 * @brief Match a semicolon and fetch the next token
 */
void semi() { match(T_SEMI, ";"); }

/**
 * @brief Match a colon and fetch the next token
 */
void colon() { match(T_COLON, ":"); }

/**
 * @brief Match an identifier and fetch the next token
 */
void identifier() { match(T_IDENTIFIER, "identifier"); }
