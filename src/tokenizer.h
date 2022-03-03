#ifndef SUN_TOKENIZER_H
#define SUN_TOKENIZER_H

#include "ast.h"
#include "token.h"

int arithmetic_op(int token);
struct AST_node *bin_expr();

#endif
