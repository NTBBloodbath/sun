// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_DATA_H
#define SUN_DATA_H
#include "sym.h"
#include "token.h"
#include <stdio.h>

#ifndef extern_
#define extern_ extern
#endif

#define NSYM 1024

extern_ int curr_line;
extern_ int go_back;
extern_ struct token Token;
extern_ FILE *sun_file;
extern_ FILE *sun_out_file;
extern_ char Text[TEXTLEN + 1];
extern_ struct symbol_table Gsym[NSYM];
#endif
