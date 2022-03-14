// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_TOKENIZER_H
#define SUN_TOKENIZER_H

#include "ast.h"
#include "token.h"

int arithmetic_op(int token);
struct sun_ast_node_st *bin_expr();

#endif
