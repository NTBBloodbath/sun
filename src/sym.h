#ifndef SUN_SYM_H
#define SUN_SYM_H

/*
 * @brief Symbol table
 */
struct symbol_table {
    char *name;
};

/*
 * @brief Number of symbols table entries
 */
#define NSYM 1024

int find_glob(char *name);
int add_glob(char *name);
#endif
