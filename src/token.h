#ifndef SUN_TOKEN_H
#define SUN_TOKEN_H

struct token {
    int token;
    int int_value;
};

enum {
    T_EOF,   // EOF
    T_PLUS,  // +
    T_MINUS, // -
    T_STAR,  // *
    T_SLASH, // /
    T_INT,   // 10
};
#endif
