#include <stdio.h>
#include <stdlib.h>

#include "cstd/mem/allocator.h"

void *cstd_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fputs("fatal: cstd_malloc out of memory\n", stderr);
        abort();
    }
    return ptr;
}

void *cstd_realloc(void *ptr, size_t size) {
    if (size == 0U) {
        return realloc(ptr, size);
    }

    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fputs("fatal: cstd_realloc out of memory\n", stderr);
        abort();
    }
    return new_ptr;
}
