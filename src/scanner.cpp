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
    if (pos + count > source.length()) {
        // BUG: this seems to be doing nothing, it is returning a newline anyway
        return EOF;
    }

    ch = source.at(pos);
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

    // Restore current line to its initial value
    state.current_ln = 0;
}

/**
 * @brief Scan a number
 *
 * @param[in] state Compiler state
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
        ch = look_ahead(state, 1);
    }

    look_behind(state);
    return value;
}

/**
 * @brief Skip comments on source code
 *
 * @param[in] state Compiler state
 *
 * @private
 */
static void skip_comment(State &state) {
    // Check if this is a potential comment
    char ch = look_ahead(state, 1);
    if (ch != '/') { return; }
    
    // //, /* or not comment
    switch (ch) {
        case '/':
            state.current_ln++;
            break;
        case '*':
            break;
        default:
            look_behind(state);
    }
}

/**
 * @brief Scan a buffer
 *
 * @param[in] state Compiler state
 * @param[in] token
 *
 * @return True if token is valid, false if there are no more tokens left
 */
bool scan(State &state, sun::Token *t) {
    // Skip whitespaces
    skip_whitespace(state);
    // Decrease current position on file
    look_behind(state);
    // Skip comments
    skip_comment(state);

    // Get next character on file to scan it
    char ch = look_ahead(state, 1);

    // Set line and column where the token was found
    t->line = state.current_ln;
    t->column = state.file_pos;

    switch (ch) {
        case '\n': // FIXME: we should check for EOF instead, we need to find a way to do this
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

            std::string err_msg = "Unrecognized character '";
            err_msg.append(1, ch);
            err_msg.append("' on line " + std::to_string(state.current_ln + 1));
            sun::logger::err_fatal(err_msg);
    }

    return true;
}
