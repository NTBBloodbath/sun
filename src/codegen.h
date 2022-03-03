#ifndef SUN_CODEGEN_H
#define SUN_CODEGEN_H

void free_all_registers(void);

void cg_preamble(void);
void cg_postamble(void);
void cg_printint(int r);

int cg_load_int(int value);
int cg_load_global(char *identifier);
int cg_store_global(int reg, char *identifier);
void cg_global_sym(char *sym);

int cg_equal(int r1, int r2);
int cg_not_equal(int r1, int r2);
int cg_less_than(int r1, int r2);
int cg_less_equal(int r1, int r2);
int cg_greater_than(int r1, int r2);
int cg_greater_equal(int r1, int r2);

int cg_add(int r1, int r2);
int cg_sub(int r1, int r2);
int cg_mul(int r1, int r2);
int cg_div(int r1, int r2);
#endif
