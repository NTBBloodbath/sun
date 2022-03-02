#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define extern_
#include "globals.h"
#undef extern_
#include "cli.h"
#include "ast.h"
#include "interp.h"
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

    sun_file = fopen(argv[1], "r");
    if (sun_file == NULL) {
        fprintf(stderr, "Unable to open '%s' file: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    scan(&Token);
    node = bin_expr();

    printf("%d\n", gen_ast(node));
    fclose(sun_file);

    exit(0);
}
