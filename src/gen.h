// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_GEN_H
#define SUN_GEN_H

int gen_ast(struct AST_node *ast, int reg);

void gen_free_registers(void);
void gen_preamble(void);
void gen_postamble(void);
void gen_printint(int reg);
void gen_global_sym(char *sym);
#endif
