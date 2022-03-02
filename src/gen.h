#ifndef SUN_GEN_H
#define SUN_GEN_H

int gen_ast(struct AST_node *ast);
void gen_code(struct AST_node *node);

void gen_free_registers(void);
void gen_preamble(void);
void gen_postamble(void);
void gen_printint(int reg);
#endif
