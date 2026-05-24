#include <string.h>

#include "ckit/common/panic.h"
#include "ckit/datastruct/hashmap.h"
#include "ckit/datastruct/linked_list.h"
#include "ckit/memory/allocators/allocator.h"
#include "ckit/memory/utils.h"
#include "datastruct/hash_common.h"

typedef struct ckit_hashmap_entry {
    ckit_linked_list_node node;
    void *key;
    void *value;
} ckit_hashmap_entry;

struct ckit_hashmap {
    size_t size;
    size_t key_size;
    size_t value_size;
    size_t capacity;
    ckit_linked_list **buckets;
    ckit_hashmap_key_eq_fn key_eq;
    ckit_allocator *allocator;
};

static void ckit_hashmap_rehash(ckit_hashmap *map, size_t new_capacity) {
    size_t alloc_size = sizeof(*map->buckets) * new_capacity;
    ckit_linked_list **new_buckets = (ckit_linked_list **)ckit_malloc(map->allocator, alloc_size);
    memset((void *)new_buckets, 0, alloc_size);
    for (size_t i = 0; i < new_capacity; i++) {
        new_buckets[i] = ckit_linked_list_init(map->allocator);
    }

    for (size_t i = 0; i < map->capacity; i++) {
        ckit_linked_list_node *node = ckit_linked_list_head(map->buckets[i]);
        while (node != NULL) {
            ckit_linked_list_node *next = node->next;
            ckit_hashmap_entry *entry = CKIT_CONTAINER_OF(node, ckit_hashmap_entry, node);
            size_t bucket = ckit_hash_bucket_index(entry->key, map->key_size, new_capacity);

            ckit_linked_list_pushfront(new_buckets[bucket], &entry->node);
            node = next;
        }
        ckit_linked_list_free(map->buckets[i]);
    }

    ckit_dealloc(map->allocator, (void *)map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}

ckit_hashmap *ckit_hashmap_init(size_t key_size, size_t value_size, ckit_hashmap_key_eq_fn key_eq,
                                ckit_allocator *allocator) {
    CKIT_ASSERT(key_eq != NULL, "fatal: ckit_hashmap_init invalid arguments");
    CKIT_ASSERT(key_size > 0, "fatal: ckit_hashmap_init invalid arguments");
    CKIT_ASSERT(value_size > 0, "fatal: ckit_hashmap_init invalid arguments");

    ckit_hashmap *map = ckit_malloc(allocator, sizeof(*map));
    map->allocator = allocator;
    size_t alloc_size = sizeof(*map->buckets) * CKIT_HASH_DEFAULT_CAPACITY;
    map->buckets = (ckit_linked_list **)ckit_malloc(allocator, alloc_size);
    memset((void *)map->buckets, 0, alloc_size);
    for (size_t i = 0; i < CKIT_HASH_DEFAULT_CAPACITY; i++) {
        map->buckets[i] = ckit_linked_list_init(allocator);
    }

    map->size = 0;
    map->key_size = key_size;
    map->value_size = value_size;
    map->capacity = CKIT_HASH_DEFAULT_CAPACITY;
    map->key_eq = key_eq;

    return map;
}

void ckit_hashmap_put(ckit_hashmap *map, const void *key, const void *value) {
    CKIT_ASSERT(map != NULL, "fatal: ckit_hashmap_put invalid arguments");
    CKIT_ASSERT(key != NULL, "fatal: ckit_hashmap_put invalid arguments");
    CKIT_ASSERT(value != NULL, "fatal: ckit_hashmap_put invalid arguments");

    size_t bucket = ckit_hash_bucket_index(key, map->key_size, map->capacity);
    ckit_linked_list_node *node = ckit_linked_list_head(map->buckets[bucket]);

    while (node != NULL) {
        ckit_hashmap_entry *entry = CKIT_CONTAINER_OF(node, ckit_hashmap_entry, node);
        if (map->key_eq(entry->key, key, map->key_size)) {
            memcpy(entry->value, value, map->value_size);
            return;
        }
        node = node->next;
    }

    if (ckit_hash_should_grow(map->size, map->capacity)) {
        ckit_hashmap_rehash(map, map->capacity * 2);
        bucket = ckit_hash_bucket_index(key, map->key_size, map->capacity);
    }

    ckit_hashmap_entry *entry = ckit_malloc(map->allocator, sizeof(*entry));
    entry->key = ckit_malloc(map->allocator, map->key_size);
    entry->value = ckit_malloc(map->allocator, map->value_size);

    memcpy(entry->key, key, map->key_size);
    memcpy(entry->value, value, map->value_size);

    ckit_linked_list_pushfront(map->buckets[bucket], &entry->node);
    map->size += 1;
}

const void *ckit_hashmap_get(const ckit_hashmap *map, const void *key) {
    CKIT_ASSERT(map != NULL, "fatal: ckit_hashmap_get invalid arguments");
    CKIT_ASSERT(key != NULL, "fatal: ckit_hashmap_get invalid arguments");

    size_t bucket = ckit_hash_bucket_index(key, map->key_size, map->capacity);
    ckit_linked_list_node *node = ckit_linked_list_head(map->buckets[bucket]);
    while (node != NULL) {
        ckit_hashmap_entry *entry = CKIT_CONTAINER_OF(node, ckit_hashmap_entry, node);
        if (map->key_eq(entry->key, key, map->key_size)) {
            return entry->value;
        }
        node = node->next;
    }
    return NULL;
}

void *ckit_hashmap_remove(ckit_hashmap *map, const void *key) {
    CKIT_ASSERT(map != NULL, "fatal: ckit_hashmap_remove invalid arguments");
    CKIT_ASSERT(key != NULL, "fatal: ckit_hashmap_remove invalid arguments");

    size_t bucket = ckit_hash_bucket_index(key, map->key_size, map->capacity);
    ckit_linked_list_node *prev = NULL;
    ckit_linked_list_node *node = ckit_linked_list_head(map->buckets[bucket]);

    while (node != NULL) {
        ckit_hashmap_entry *entry = CKIT_CONTAINER_OF(node, ckit_hashmap_entry, node);
        if (map->key_eq(entry->key, key, map->key_size)) {
            ckit_linked_list_remove_after(map->buckets[bucket], prev);

            void *value = entry->value;
            ckit_dealloc(map->allocator, entry->key);
            ckit_dealloc(map->allocator, entry);
            map->size -= 1;
            return value;
        }
        prev = node;
        node = node->next;
    }

    return NULL;
}

void ckit_hashmap_free(ckit_hashmap *map) {
    CKIT_ASSERT(map != NULL, "fatal: ckit_hashmap_free invalid arguments");

    ckit_allocator *allocator = map->allocator;
    if (map->buckets) {
        for (size_t i = 0; i < map->capacity; i++) {
            ckit_linked_list_node *node = ckit_linked_list_head(map->buckets[i]);
            while (node != NULL) {
                ckit_linked_list_node *next = node->next;
                ckit_hashmap_entry *entry = CKIT_CONTAINER_OF(node, ckit_hashmap_entry, node);
                ckit_dealloc(map->allocator, entry->key);
                ckit_dealloc(map->allocator, entry->value);
                ckit_dealloc(map->allocator, entry);
                node = next;
            }
            ckit_linked_list_free(map->buckets[i]);
        }
    }

    ckit_dealloc(map->allocator, (void *)map->buckets);
    ckit_dealloc(allocator, map);
}

size_t ckit_hashmap_size(const ckit_hashmap *map) {
    CKIT_ASSERT(map != NULL, "fatal: ckit_hashmap_size invalid arguments");

    return map->size;
}
