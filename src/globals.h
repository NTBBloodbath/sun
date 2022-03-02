#ifndef SUN_DATA_H
#define SUN_DATA_H
#include <stdio.h>
#include "token.h"

#ifndef extern_
    #define extern_ extern
#endif

extern_ int curr_line;
extern_ int go_back;
extern_ struct token Token;
extern_ FILE *sun_file;
extern_ FILE *sun_out_file;
extern_ char Text[TEXTLEN + 1];
#endif
