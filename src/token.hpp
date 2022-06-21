// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#pragma once
#include <iostream>

/**
 * @brief Sun namespace
 *
 * @namespace sun
 */
namespace sun {
    /**
     * @brief Token types
     *
     * @enum sun::TokenType
     */
    enum class TokenType {
        Plus = 1,
        Minus,
        Times,
        Slash,
        Eof,
    };

    /**
     * @brief Token struct
     *
     * @struct sun::Token
     */
    struct Token {
        TokenType type;    // Token type
        std::string value; // Token value
        std::string file;  // File where the token was found
        int line;          // Line where the token was found
        int column;        // Column where the token was found
    };
} // namespace sun
