// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "logger.hpp"
#include "scanner.hpp"
#include "state.hpp"
#include "token.hpp"

/**
 * @brief Looks back by `count` amount of characters in the source buffer
 *
 * @param[in] state Scanner state
 *
 * @private
 */
static char lookbehind(const State &state, size_t count = 1) {
    if (state.file_pos - count < 0)
        return '\0';

    return state.sun_source_file[state.file_pos - count];
}

/**
 * @brief Looks ahead by `count` amount of characters in the source buffer
 *
 * @param[in] state Scanner state
 * @param[in] count Characters count to look ahead
 * @return Next character in buffer
 *
 * @private
 */
static char lookahead(const State &state, size_t count = 1) {
    int pos = state.file_pos;

    // If current position plus count to look ahead
    // is larger than source file then return EOF
    if (pos + count > state.sun_source_file.length()) {
        return '\0';
    }

    return state.sun_source_file[pos + count];
}

static bool advance(State &state) {
    if (state.file_pos != -1 && state.file_pos >= state.sun_source_file.length())
        return false;

    if (lookahead(state) == '\n')
        state.current_ln++;

    state.file_pos++;
    return true;
}

/**
 * @brief Skip whitespaces on source code
 *
 * @param[in] state Scanner state
 *
 * @private
 */
static void skip_whitespace(State &state) {
    // Skip rules:
    // - whitespace
    // - line feed (newline)
    // - tab
    // - carriage return
    // - formfeed
    for (char c = lookahead(state); (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f'); c = lookahead(state))
        advance(state);
}

/**
 * @brief Skip comments on source code
 *
 * @param[in] state Scanner state
 *
 * @private
 */
static void skip_comment(State &state) {
    // Check if this is a potential comment
    char ch = lookahead(state);
    if (ch != '/') { return; }
    
    // //, /* or not comment
    switch (ch) {
        case '/':
            if (lookahead(state, 2) == '/') {
                for (char c = lookahead(state); (c != '\0'); c = lookahead(state)) {
                    if (c == '\n') {
                        state.file_pos++;
                        break;
                    }
                    advance(state);
                }
            }
            break;
        case '*':
            break;
        default:
            lookbehind(state);
    }
}

/**
 * @brief Scan a number
 *
 * @param[in] state Scanner state
 * @param[in] ch 
 * @return A number value
 *
 * @private
 */
static int scan_number(State &state, int ch) {
    int curr_int = 0;
    int value = 0;

    while ('0' <= ch && ch <= '9') {
        // Convert the current character to its integer value
        curr_int = ch - '0';
        value = value * 10 + curr_int;
        ch = lookahead(state);
        advance(state);
    }

    return value;
}

/**
 * @brief Scan a buffer
 *
 * @param[in] state Scanner state
 * @param[in] token
 *
 * @return True if token is valid, false if there are no more tokens left
 */
bool scan(State &state, sun::Token *t) {
    // Skip whitespace
    skip_whitespace(state);
    // skip comments
    skip_comment(state);

    // Get next character on file to scan it
    char ch = lookahead(state);

    // Set line and column where the token was found
    t->line = state.current_ln;
    t->column = state.file_pos;

    advance(state);

    switch (ch) {
        case '\0':
            return false;
        case '+':
            t->type = sun::TokenType::Plus;
            break;
        case '-':
            t->type = sun::TokenType::Minus;
            break;
        case '*':
            t->type = sun::TokenType::Times;
            break;
        case '/':
            t->type = sun::TokenType::Slash;
            break;
        default:
            // Scan int literals
            if (std::isdigit(ch)) {
                t->type = sun::TokenType::Integer;
                t->value = std::to_string(scan_number(state, ch));
                break;
            }

            sun::logger::err_fatal("Unrecognized character '"s + ch + "' on line " + std::to_string(state.current_ln + 1));
    }

    return true;
}
