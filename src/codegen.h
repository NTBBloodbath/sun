#ifndef SUN_CODEGEN_H
#define SUN_CODEGEN_H

void cg_preamble(void);
void cg_postamble(void);
void cg_printint(int r);
int cg_load(int value);

int cg_add(int r1, int r2);
int cg_sub(int r1, int r2);
int cg_mul(int r1, int r2);
int cg_div(int r1, int r2);
#endif
