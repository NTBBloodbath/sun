// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cstdio>
#include <iostream>

#include "logger.hpp"
#include "scanner.hpp"
#include "state.hpp"

static void look_behind(State &state) {
    if (state.file_pos > 0)
        state.file_pos -= 1;
}

static char look_ahead(State &state, int count) {
    char ch;
    int pos = state.file_pos;
    std::string source = state.sun_source_file;

    // If current position plus count to look ahead
    // is larger than source file then return EOF
    // sun::logger::dbg(std::to_string(pos + count));
    // if (pos + count > source.length()) {
    //     sun::logger::dbg("Reached EOF");
    //     return EOF;
    // }

    ch = source[pos];
    if (ch == '\n')
        state.current_ln += 1;

    // Increase position in file
    state.file_pos += 1;

    return ch;
}

static void skip(State &state) {
    int prev_ch;
    char ch = look_ahead(state, 1);

    for (;;) {
        // Skip rules:
        // - whitespace
        // - line feed (newline)
        // - tab
        // - carriage return
        // - formfeed
        while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f') {
            ch = look_ahead(state, 1);
        }

        if (ch != '/')
            break;
        ch = look_ahead(state, 1);

        if (ch != '*' && ch != '/') {
            look_behind(state);
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

void scan(State &state) {
    for (int i = 0; i < state.sun_source_file.length(); i++)
        std::cout << "Current character: '" << look_ahead(state, i + 1) << "'" << std::endl;
}
