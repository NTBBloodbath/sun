// Enable use of strdup() in C11+ standard
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "sym.h"

// Position of next free global symbol slot
static int Globs = 0;

/**
 * @brief Determine if the symbol 'name' is in the global symbol table
 *
 * @param name [in] Symbol to search for
 * @return Symbol slot position or -1 if not found
 */
int find_glob(char *name) {
    for (int i = 0; i < Globs; i++) {
        if (*name == *Gsym[i].name && !strcmp(name, Gsym[i].name)) {
            return i;
        }
    }

    return -1;
}

static int new_glob() {
    int slot_pos;
    if ((slot_pos = Globs++) >= NSYM) {
        fprintf(stderr, "Too many symbols in the global scope\n");
        exit(1);
    }

    return slot_pos;
}

int add_glob(char *name) {
    int slot_pos;

    // Fast return slot position if this symbol is already in the symbol table
    if ((slot_pos = find_glob(name)) != -1) {
        return slot_pos;
    }

    // Get a new symbol slot and fill it then return the slot number
    slot_pos = new_glob();
    Gsym[slot_pos].name = _strdup(name);
    return slot_pos;
}
