#ifndef CKIT_DATASTRUCT_HASH_COMMON_H
#define CKIT_DATASTRUCT_HASH_COMMON_H

#include <stdbool.h>
#include <stddef.h>

#include "crypto/fnv1a.h"

#define CKIT_HASH_DEFAULT_CAPACITY 16
#define CKIT_HASH_MAX_LOAD 0.75

static inline size_t ckit_hash_bucket_index(const void *value, size_t value_size, size_t capacity) {
    return ckit_internal_fnv1a_hash(value, value_size) % capacity;
}

static inline bool ckit_hash_should_grow(size_t size, size_t capacity) {
    return ((double)(size + 1) / (double)capacity) > CKIT_HASH_MAX_LOAD;
}

#endif
