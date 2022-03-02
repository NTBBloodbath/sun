#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define extern_
#include "globals.h"
#undef extern_
#include "cli.h"
#include "ast.h"
#include "gen.h"
#include "scanner.h"
#include "tokenizer.h"

static void init_compiler() {
    curr_line = 1;
    go_back = '\n';
}

int main (int argc, char *argv[]) {
    struct AST_node *node;

    if (argc != 2) {
        help();
        exit(1);
    }

    init_compiler();

    // Open input file
    sun_file = fopen(argv[1], "r");
    if (sun_file == NULL) {
        fprintf(stderr, "Unable to open '%s' file: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    // Create output file
    sun_out_file = fopen("out.s", "w");
    if (sun_out_file == NULL) {
        fprintf(stderr, "Unable to create 'out.sun' file: %s\n", strerror(errno));
        exit(1);
    }

    scan(&Token);
    node = bin_expr(0);

    fclose(sun_file);
    gen_code(node);

    exit(0);
}
