#ifndef SUN_DATA_H
#define SUN_DATA_H
#include <stdio.h>

#ifndef extern_
    #define extern_ extern
#endif

extern_ int curr_line;
extern_ int go_back;
extern_ struct token Token;
extern_ FILE *sun_file;
#endif
