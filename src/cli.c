// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘

#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

void version() {
    printf("sun %s\n\n%s\n", VERSION, COPYRIGHT);
    exit(0);
}

void help() { fprintf(stderr, "Usage: sun file.sun\n"); }
