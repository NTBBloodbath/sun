#ifndef SUN_MEMORY_H
#define SUN_MEMORY_H

#include <stddef.h>

void *xmalloc(size_t size);
void xfree(void *ptr);
#endif
