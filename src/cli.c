#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

void version() {
    printf("sun %s\n\n%s\n", VERSION, COPYRIGHT);
    exit(0);
}

void help() {
    fprintf(stderr, "Usage: sun file.sun\n");
}
