#ifndef CSTD_MEM_ALLOCATOR_H
#define CSTD_MEM_ALLOCATOR_H

#include <stddef.h>

/*
 * Allocate size bytes.
 * Panics (prints fatal message and aborts) on out-of-memory.
 */
void *cstd_malloc(size_t size);

/*
 * Resize ptr to size bytes.
 * Panics (prints fatal message and aborts) on out-of-memory when size > 0.
 */
void *cstd_realloc(void *ptr, size_t size);

#endif
