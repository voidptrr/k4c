#ifndef CKIT_MEMORY_UTILS_H
#define CKIT_MEMORY_UTILS_H

#include <stddef.h>
#include <stdint.h>

#define CKIT_CONTAINER_OF(ptr, type, member) ((type *)((uint8_t *)(ptr) - offsetof(type, member)))
#define CKIT_MEMORY_ALIGN (sizeof(max_align_t))

/*
 * Round value up to the next multiple of alignment.
 * alignment must be a power of two and non-zero.
 */
static inline size_t ckit_align_up(size_t value, size_t alignment) {
    size_t mask = alignment - 1;
    return (value + mask) & ~mask;
}

#endif
