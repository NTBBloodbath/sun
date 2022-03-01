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
