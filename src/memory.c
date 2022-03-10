// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
// Microsoft has deprecated lots of calls which use buffers to improve code security.
// Using this line to remove deprecation notice
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief A malloc() wrapper that checks the results and dies in case of error
 */
void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "allocating memory; %s\n", strerror(errno));
        exit(errno);
    }

    return ptr;
}

/**
 * @brief A free() wrapper that dies if fed with NULL pointer
 */
void xfree(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "allocating memory; %s\n", strerror(errno));
        exit(errno);
    }

    free(ptr);
}
