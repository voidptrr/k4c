# datastruct.hashmap

## DESCRIPTION

The hashmap module provides fixed-size key/value mapping with separate chaining for collisions.
Hashing uses an internal FNV-1a function over stored key bytes. Key equality
uses byte comparison when `key_eq` is `NULL`, or a caller callback when one is
provided.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### vs_hashmap_entry_view

```c
typedef struct vs_hashmap_entry_view {
    const void *key;
    const void *value;
} vs_hashmap_entry_view;
```

Hashmap iterators yield pointers to this view. The view is stored inside the
iterator state and is overwritten by the next `vs_iterator_next` call on that
iterator.

### vs_hashmap_iterator_state

```c
typedef struct vs_hashmap_iterator_state vs_hashmap_iterator_state;
```

Caller-owned cursor state for `vs_hashmap_iterator`.

## FUNCTIONS

### vs_hashmap_create

```c
vs_hashmap *vs_hashmap_create(size_t key_size,
                                size_t value_size,
                                vs_hashmap_key_eq_fn key_eq,
                                vs_allocator *allocator);
```

- Parameters: `key_size`, `value_size`, `key_eq`, `allocator`
- Returns: opaque hashmap handle.
- Notes: the hashmap stores `allocator` and reuses it for entries, buckets,
  rehashing, and destroy. When `allocator` is `NULL`, hashmap uses the C
  library heap through `vs_malloc`. Custom `key_eq` callbacks must be
  consistent with the byte hash used for bucket selection.
- Example: `vs_hashmap *map = vs_hashmap_create(sizeof(uint64_t), sizeof(uint64_t), NULL, NULL);`

### vs_hashmap_put

```c
void vs_hashmap_put(vs_hashmap *map, const void *key, const void *value);
```

- Parameters: `map`, `key`, `value`
- Returns: none.
- Example: `vs_hashmap_put(map, &key, &value);`

### vs_hashmap_get

```c
void *vs_hashmap_get(vs_hashmap *map, const void *key);
```

- Parameters: `map`, `key`
- Returns: pointer to stored value in map-managed storage, or `NULL` when key is missing.
- Example: `uint64_t *value = (uint64_t *)vs_hashmap_get(map, &key);`

### vs_hashmap_get_const

```c
const void *vs_hashmap_get_const(const vs_hashmap *map, const void *key);
```

- Parameters: `map`, `key`
- Returns: pointer to stored value in map-managed storage, or `NULL` when key is missing.
- Example: `const uint64_t *value = (const uint64_t *)vs_hashmap_get_const(map, &key);`

### vs_hashmap_remove

```c
void vs_hashmap_remove(vs_hashmap *map, const void *key);
```

- Parameters: `map`, `key`
- Returns: none.
- Notes: missing keys are ignored.
- Example: `vs_hashmap_remove(map, &key);`

### vs_hashmap_size

```c
size_t vs_hashmap_size(const vs_hashmap *map);
```

- Parameters: `map`
- Returns: current entry count.
- Example: `size_t count = vs_hashmap_size(map);`

### vs_hashmap_iterator

```c
vs_iterator vs_hashmap_iterator(vs_hashmap_iterator_state *state, const vs_hashmap *map);
```

- Parameters: `state`, `map`
- Returns: iterator over key/value entry views in bucket order.
- Notes: `state` must outlive the returned iterator. Yielded pointers are
  `const vs_hashmap_entry_view *`. The view is stored inside `state` and is
  overwritten by the next `vs_iterator_next` call on that iterator. Do not
  mutate the hashmap while iterating.
- Example: `vs_iterator iter = vs_hashmap_iterator(&state, map);`

### vs_hashmap_destroy

```c
void vs_hashmap_destroy(vs_hashmap *map);
```

- Parameters: `map`
- Returns: none.
- Notes: releases entries, buckets, and the opaque handle. Do not use `map` after this call.
- Example: `vs_hashmap_destroy(map);`

