/*
 * MIT License
 *
 * Copyright (c) 2026 Tommaso Bruno
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CK_DATASTRUCT_HASH_COMMON_H
#define CK_DATASTRUCT_HASH_COMMON_H

#include <stdbool.h>
#include <stddef.h>

#include "ckit/compare.h"
#include "ckit/datastruct/linked_list.h"
#include "ckit/memory/allocators/allocator.h"
#include "hash.h"

#define CK_HASH_COMMON_DEFAULT_CAPACITY 16
#define CK_HASH_COMMON_MAX_LOAD 0.75

typedef void (*ck_hash_common_entry_destroy_fn)(ck_linked_list_node *node, ck_allocator *allocator);
typedef const void *(*ck_hash_common_entry_value_fn)(const ck_linked_list_node *node);

/* Allocate and initialize an array of empty bucket lists. */
ck_linked_list **ck_hash_common_buckets_create(size_t capacity, ck_allocator *allocator);

/*
 * Move all nodes into a newly allocated bucket array sized to new_capacity.
 * The old bucket list wrappers and bucket array are destroyed, but entries are
 * preserved and relinked into the new buckets.
 */
ck_linked_list **ck_hash_common_buckets_rehash(
    ck_linked_list **buckets,
    size_t capacity,
    size_t new_capacity,
    size_t value_size,
    ck_allocator *allocator,
    ck_hash_common_entry_value_fn entry_value
);

/* Return the first node whose extracted value equals value, or NULL. */
ck_linked_list_node *ck_hash_common_bucket_find(
    ck_linked_list *bucket,
    const void *value,
    size_t value_size,
    ck_eq_fn value_eq,
    ck_hash_common_entry_value_fn entry_value
);

/*
 * Unlink and return the first matching node from bucket, or NULL when no match
 * is found. The returned node remains owned by the caller.
 */
ck_linked_list_node *ck_hash_common_bucket_remove(
    ck_linked_list *bucket,
    const void *value,
    size_t value_size,
    ck_eq_fn value_eq,
    ck_hash_common_entry_value_fn entry_value
);

/* Destroy all entries, bucket lists, and the bucket array. */
void ck_hash_common_buckets_destroy(
    ck_linked_list **buckets,
    size_t capacity,
    ck_allocator *allocator,
    ck_hash_common_entry_destroy_fn entry_destroy
);

/* Hash value and map it into the bucket range [0, capacity). */
static inline size_t ck_hash_common_bucket_index(
    const void *value,
    size_t value_size,
    size_t capacity
) {
    return ck_fnv1a_hash(value, value_size) % capacity;
}

/* Return true when inserting one more item would exceed the load limit. */
static inline bool ck_hash_common_should_grow(size_t size, size_t capacity) {
    return ((double)(size + 1) / (double)capacity) > CK_HASH_COMMON_MAX_LOAD;
}

#endif
