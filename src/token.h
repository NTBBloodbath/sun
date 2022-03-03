#ifndef SUN_TOKEN_H
#define SUN_TOKEN_H

/*
 * @brief Length of symbols in input
 */
#define TEXTLEN 512

/*
 * @brief Token table
 */
struct token {
    int token;
    int int_value;
};

enum {
    T_EOF,        // EOF
    // ARITHMETIC
    T_PLUS,       // +
    T_MINUS,      // -
    T_STAR,       // *
    T_SLASH,      // /
    T_EQ,         // ==
    T_NE,         // !=
    T_LT,         // <
    T_GT,         // >
    T_LE,         // <=
    T_GE,         // >=
    T_INTEGER,    // 10
    T_SEMI,       // ;
    T_COLON,      // :
    T_ASSIGN,     // =
    T_IDENTIFIER, // hello
    // KEYWORDS
    T_PRINT,      // print
    T_LET,        // let
    T_INT,        // int
};
#endif
