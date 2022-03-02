#ifndef SUN_TOKEN_H
#define SUN_TOKEN_H

/*
 * @brief Length of symbols in input
 */
#define TEXTLEN 512

struct token {
    int token;
    int int_value;
};

enum {
    T_EOF,   // EOF
    // ===== ARITHMETIC
    T_PLUS,  // +
    T_MINUS, // -
    T_STAR,  // *
    T_SLASH, // /
    T_INT,   // 10
    T_SEMI,  // ;
    // ===== KEYWORDS
    T_PRINT  // print
};
#endif
