// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "token.h"

/**
 * @brief Put back an unwanted character
 */
static void look_behind(int ch) { go_back = ch; }

/**
 * @brief Get the next character from the input file
 *
 * @return next character integer value
 */
static int look_ahead() {
    int ch;

    if (go_back) {
        ch = go_back; // go back if there is a prev character
        go_back = 0;
        return ch;
    }

    ch = fgetc(sun_file);
    if (ch == '\n') {
        curr_line++;
    }

    return ch;
}

/**
 * @brief Skip whitespaces and comments
 *
 * @return Next character after skipping unwanted characters
 */
static int skip() {
    int prev_ch;
    int ch = look_ahead();

    for (;;) {
        // Skip rules:
        // - whitespace
        // - line feed (newline)
        // - tab
        // - carriage return
        // - formfeed
        while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f') {
            ch = look_ahead();
        }

        if (ch != '/')
            break;
        ch = look_ahead();
        if (ch != '*' && ch != '/') {
            look_behind(ch);
            ch = '/';
            break;
        }
        if (ch == '/') {
            // Single-line comments
            while ((ch = look_ahead()) != EOF) {
                if (ch == '\n')
                    break;
            }
        } else {
            prev_ch = 0;
            // Multi-line comments
            while ((ch = look_ahead()) != EOF) {
                if (ch == '/' && prev_ch == '*') {
                    ch = look_ahead();
                    break;
                }
                prev_ch = ch;
            }
        }
    }

    return ch;
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

static int scan_int(int ch) {
    int i, val = 0;

    // Convert each int character into an int value
    while ((i = ch_pos("0123456789", ch)) >= 0) {
        // We multiply the current value and then add the new int to it
        // e.g. 64 gets converted to 6 * 10 + 4 so 60 + 4 and the final result is 64 as expected
        val = val * 10 + i;
        ch = look_ahead();
    }

    // Look behind if we hit a non-integer character
    look_behind(ch);
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
static int scan_identifier(int ch, char *buf, int limit) {
    int i = 0;

    // Allowed characters for declaring identifiers:
    // - Digits
    // - Alpha
    // - Underscores
    while (isdigit(ch) || isalpha(ch) || ch == '_') {
        // Raise an error if we hit the identifier length limit
        // and append it to buf and get next character otherwise
        if (limit - 1 == i) {
            fprintf(stderr, "Identifier too long on line %d\n", curr_line);
            exit(1);
        } else if (i < limit - 1) {
            buf[i++] = ch;
        }
        ch = look_ahead();
    }

    // Put a character back once we hit a non-valid one
    look_behind(ch);
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
    int ch = skip();
    int token_kind;

    switch (ch) {
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
            if ((ch = look_ahead()) == '=') {
                t->token = T_EQ;
            } else {
                look_behind(ch);
                t->token = T_ASSIGN;
            }
            break;
        case '!':
            if ((ch = look_ahead()) == '=') {
                t->token = T_NE;
            } else {
                fprintf(stderr, "Unrecognized character '%c' on line %d\n", ch, curr_line);
                exit(1);
            }
            break;
        case '<':
            if ((ch = look_ahead()) == '=') {
                t->token = T_LE;
            } else {
                look_behind(ch);
                t->token = T_LT;
            }
            break;
        case '>':
            if ((ch = look_ahead()) == '=') {
                t->token = T_GE;
            } else {
                look_behind(ch);
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
            if (isdigit(ch)) {
                t->token = T_INTEGER;
                t->int_value = scan_int(ch);
                break;
            } else if (isalpha(ch) || ch == '_') {
                // Read in a keyword or identifier
                scan_identifier(ch, Text, TEXTLEN);

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
            fprintf(stderr, "Unexpected token '%c' on line %d\n", ch, curr_line);
            exit(1);
    }

    return 1;
}
