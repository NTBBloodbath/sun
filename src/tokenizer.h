#ifndef SUN_TOKENIZER_H
#define SUN_TOKENIZER_H

#include "token.h"
#include "ast.h"

int arithmetic_op(int token);
struct AST_node *bin_expr();

#endif
