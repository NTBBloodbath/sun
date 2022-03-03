#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define extern_
#include "globals.h"
#undef extern_
#include "ast.h"
#include "cli.h"
#include "gen.h"
#include "scanner.h"
#include "stmt.h"
#include "tokenizer.h"

static void init_compiler() {
    curr_line = 1;
    go_back = '\n';
}

int main(int argc, char *argv[]) {
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

    // Get the first token from input file
    scan(&Token);
    // Set preamble in output file
    gen_preamble();
    // Parse statements in input file
    statements();
    // Set postamble in output file
    gen_postamble();
    // Close input/output file
    fclose(sun_file);
    fclose(sun_out_file);

    exit(0);
}
