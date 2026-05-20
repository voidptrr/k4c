#include <stdio.h>
#include <stdlib.h>

#include "ckit/mem/allocators/allocator.h"

void *ckit_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        ckit_panic("fatal: ckit_malloc out of memory");
    }
    return ptr;
}

void *ckit_realloc(void *ptr, size_t size) {
    if (size == 0U) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        ckit_panic("fatal: ckit_realloc out of memory");
    }
    return new_ptr;
}

_Noreturn void ckit_panic(const char *message) {
    if (message != NULL) {
        fprintf(stderr, "%s\n", message);
    }
    abort();
}
