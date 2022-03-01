#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "token.h"
 
/**
 * @brief Put back an unwanted character
 */
static void look_behind(int c) {
    go_back = c;
}

/**
 * @brief Get the next character from the input file
 *
 * @return next character integer value
 */
static int look_ahead() {
    int c;

    if (go_back) {
        c = go_back; // go back if there is a prev character
        go_back = 0;
        return c;
    }

    c = fgetc(sun_file);
    if (c == '\n') {
        curr_line++;
    }

    return c;
}

/**
 * @brief Skip whitespaces
 * 
 * @return Next character after skipping whitespaces
 */
static int skip_whitespace() {
    int c = look_ahead();

    // Skip rules:
    // - whitespace
    // - line feed (newline)
    // - tab
    // - carriage return
    // - formfeed
    while (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f') {
        c = look_ahead();
    }
    return c;
}

/**
 * @brief Get a character position in a string
 *
 * @param str [in] String to check character position
 * @param ch [in] Character to be looked for
 * @return Character position or 1 if character was not found
 */
static int ch_pos(char *str, int ch) {
    char *pos = strchr(str, ch);

    return (pos ? pos - str : -1);
}

static int scan_int(int c) {
    int i, val = 0;

    // Convert each int character into an int value
    while ((i = ch_pos("0123456789", c)) >= 0) {
        // We multiply the current value and then add the new int to it
        // e.g. 64 gets converted to 6 * 10 + 4 so 60 + 4 and the final result is 64 as expected
        val = val * 10 + i;
        c = look_ahead();
    }

    // Look behind if we hit a non-integer character
    look_behind(c);
    return val;
}

/**
 * @brief Scan and return the next token in the input file
 *
 * @param t [in] Token structure
 * @return 1 if token is valid, 0 otherwise
 */
int scan(struct token *t) {
    int c = skip_whitespace();
    switch(c) {
        case EOF:
            t->token = T_EOF;
            return 0;
        case '+':
            t->token = T_PLUS;
            break;
        case '-':
            t->token = T_MINUS;
            break;
        case '*':
            t->token = T_STAR;
            break;
        case '/':
            t->token = T_SLASH;
            break;
        default:
            // Scan int literals
            if (isdigit(c)) {
                t->token = T_INT;
                t->int_value = scan_int(c);
                break;
            }

            // Raise an Unexpected token error
            printf("Unexpected token '%d' on line '%d'\n", c, curr_line);
            exit(1);
    }

    return 1;
}
