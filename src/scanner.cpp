// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cctype>
#include <cstdio>
#include <iostream>

#include "logger.hpp"
#include "scanner.hpp"
#include "state.hpp"

/**
 * @brief Decrease current position in buffer
 *
 * @param[in] state Compiler state
 *
 * @private
 */
static void look_behind(State &state) {
    if (state.file_pos > 0)
        state.file_pos -= 1;
}

/**
 * @brief Increase current possition in buffer by N count of characters
 *
 * @param[in] state Compiler state
 * @param[in] count Characters count to look ahead
 * @return Next character in buffer
 *
 * @private
 */
static char look_ahead(State &state, unsigned long count) {
    char ch;
    int pos = state.file_pos;
    std::string source = state.sun_source_file;

    // If current position plus count to look ahead
    // is larger than source file then return EOF
    // sun::logger::dbg(std::to_string(pos + count));
    if (pos + count > source.length()) {
        // sun::logger::dbg("Reached EOF");
        return EOF;
    }

    ch = source[pos];
    if (ch == '\n')
        state.current_ln += 1;

    // Increase position in file
    state.file_pos += 1;

    return ch;
}

/**
 * @brief Skip whitespaces on source code
 *
 * @param[in] state Compiler state
 *
 * @private
 */
static void skip_whitespace(State &state) {
    char ch = look_ahead(state, 1);

    // Skip rules:
    // - whitespace
    // - line feed (newline)
    // - tab
    // - carriage return
    // - formfeed
    while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f') {
        ch = look_ahead(state, 1);
    }
}

/**
 * @brief Scan a buffer
 *
 * @param[in] state Compiler state
 * @param[in] token 
 */
void scan(State &state, Token *token) {
    // Skip whitespaces
    skip_whitespace(state);
    // Decrease current position on file
    look_behind(state);

    // Get next character on file and scan it
    char ch = look_ahead(state, 1);
    switch (ch) {
        case EOF:
            token->kind = T_EOF;
            break;
        default:
            // Scan int literals
            if (std::isdigit(ch)) {
                token->kind = T_INTEGER;
                token->number.value = 0;
            }
    }
}
