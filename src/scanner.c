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
 * @return Character position or -1 if character was not found
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
 * @brief Scan an identifier from the input file and store it
 *
 * @param c [in] Current character
 * @param buf [in] Buffer to store the identifier
 * @param limit [in] Identifier length limit
 * @return Identifier length
 */
static int scan_identifier(int c, char *buf, int limit) {
    int i = 0;

    // Allowed characters for declaring identifiers:
    // - Digits
    // - Alpha
    // - Underscores
    while (isdigit(c) || isalpha(c) || c == '_') {
        // Raise an error if we hit the identifier length limit
        // and append it to buf and get next character otherwise
        if (limit - 1 == i) {
            fprintf(stderr, "Identifier too long on line %d\n", curr_line);
            exit(1);
        } else if (i < limit - 1) {
            buf[i++] = c;
        }
        c = look_ahead();
    }

    // Put a character back once we hit a non-valid one
    look_behind(c);
    // Null-terminate the buf and return its length
    buf[i] = '\0';
    return i;
}

/**
 * @brief Scan given word and look for a matching keyword on it
 *
 * @param str [in] Word to check if it's a keyword
 * @return Matching token number or 0 if word is not a keyword
 */
static int scan_keyword(char *str) {
    // Code extracted from SubC compiler, this code has optimizations.
    // These optimizations are the following:
    //
    // Matches against the first letter before doing the 'strcmp()',
    // this speeds up the comparison against dozens of keywords.
    //
    // NOTE: This is going to be useful in the future.
    switch (*str) {
        case 'p':
            if (!strcmp(str, "print")) {
                return T_PRINT;
            }
            break;
        case 'l':
            if (!strcmp(str, "let")) {
                return T_LET;
            }
            break;
        case 'i':
            if (!strcmp(str, "int")) {
                return T_INT;
            }
            break;
        break;
    }

    return 0;
}

/**
 * @brief Scan and return the next token in the input file
 *
 * @param t [in] Token structure
 * @return 1 if token is valid, 0 otherwise
 */
int scan(struct token *t) {
    int c = skip_whitespace();
    int token_kind;

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
        case '=':
            if ((c = look_ahead()) == '=') {
                t->token = T_EQ;
            } else {
                look_behind(c);
                t->token = T_ASSIGN;
            }
            break;
        case '!':
            if ((c = look_ahead()) == '=') {
                t->token = T_NE;
            } else {
                fprintf(stderr, "Unrecognized character '%c' on line %d\n", c, curr_line);
                exit(1);
            }
            break;
        case '<':
            if ((c = look_ahead()) == '=') {
                t->token = T_LE;
            } else {
                look_behind(c);
                t->token = T_LT;
            }
            break;
        case '>':
            if ((c = look_ahead()) == '=') {
                t->token = T_GE;
            } else {
                look_behind(c);
                t->token = T_GT;
            }
            break;
        case ':':
            t->token = T_COLON;
            break;
        case ';':
            t->token = T_SEMI;
            break;
        default:
            // Scan int literals
            if (isdigit(c)) {
                t->token = T_INTEGER;
                t->int_value = scan_int(c);
                break;
            } else if (isalpha(c) || c == '_') {
                // Read in a keyword or identifier
                scan_identifier(c, Text, TEXTLEN);

                // If it's a recognized keyword, return its token
                if ((token_kind = scan_keyword(Text))) {
                    t->token = token_kind;
                    break;
                }

                // Not a recognized keyword nor identifier, return an identifier
                t->token = T_IDENTIFIER;
                break;
            }

            // Raise an Unexpected token error
            fprintf(stderr, "Unexpected token '%d' on line '%d'\n", c, curr_line);
            exit(1);
    }

    return 1;
}
