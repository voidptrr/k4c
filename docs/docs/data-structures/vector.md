# datastruct.vector

## DESCRIPTION

The vector module provides a generic contiguous growable array for fixed-size elements.
Elements are copied into vector-owned storage on push.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### vs_vector_iterator_state

```c
typedef struct vs_vector_iterator_state vs_vector_iterator_state;
```

Caller-owned cursor state for `vs_vector_iterator`.

## FUNCTIONS

### vs_vector_create

```c
vs_vector *vs_vector_create(size_t elem_size, vs_allocator *allocator);
```

- Parameters: `elem_size`, `allocator`
- Returns: opaque vector handle.
- Notes: the vector stores `allocator` and reuses it for growth and destroy.
  When `allocator` is `NULL`, vector uses the C library heap through
  `vs_malloc`/`vs_realloc`.
- Example: `vs_vector *vector = vs_vector_create(sizeof(int), NULL);`

### vs_vector_push

```c
void vs_vector_push(vs_vector *vector, const void *element);
```

- Parameters: `vector`, `element`
- Returns: none.
- Example: `vs_vector_push(vector, &value);`

### vs_vector_pop

```c
void *vs_vector_pop(vs_vector *vector);
```

- Parameters: `vector`
- Returns: pointer to the removed element within vector-managed storage, or `NULL` when the vector is empty.
- Example: `int *last = (int *)vs_vector_pop(vector);`

### vs_vector_get

```c
void *vs_vector_get(vs_vector *vector, size_t index);
```

- Parameters: `vector`, `index`
- Returns: pointer to item at `index`, or `NULL` when out of range.
- Example: `int *item = (int *)vs_vector_get(vector, 0);`

### vs_vector_get_const

```c
const void *vs_vector_get_const(const vs_vector *vector, size_t index);
```

- Parameters: `vector`, `index`
- Returns: const pointer to item at `index`, or `NULL` when out of range.
- Example: `const int *item = (const int *)vs_vector_get_const(vector, 0);`

### vs_vector_elem_size

```c
size_t vs_vector_elem_size(const vs_vector *vector);
```

- Parameters: `vector`
- Returns: configured element size.
- Example: `size_t elem_size = vs_vector_elem_size(vector);`

### vs_vector_swap_remove

```c
void *vs_vector_swap_remove(vs_vector *vector, size_t index);
```

- Parameters: `vector`, `index`
- Returns: pointer to the slot where the removed item lived, or `NULL` when out of range.
- Notes: removal does not preserve order; the last item is moved into `index`.
- Example: `int *slot = (int *)vs_vector_swap_remove(vector, 0);`

### vs_vector_size

```c
size_t vs_vector_size(const vs_vector *vector);
```

- Parameters: `vector`
- Returns: current element count.
- Example: `size_t count = vs_vector_size(vector);`

### vs_vector_iterator

```c
vs_iterator vs_vector_iterator(vs_vector_iterator_state *state, const vs_vector *vector);
```

- Parameters: `state`, `vector`
- Returns: iterator over vector elements from index `0` to `size - 1`.
- Notes: `state` must outlive the returned iterator. Yielded pointers refer to
  vector-managed storage. Do not mutate the vector while iterating.
- Example: `vs_iterator iter = vs_vector_iterator(&state, vector);`

### vs_vector_lower_bound

```c
size_t vs_vector_lower_bound(const vs_vector *vector,
                             const void *key,
                             vs_vector_cmp_fn cmp);
```

- Parameters: `vector`, `key`, `cmp`
- Returns: first sorted index whose item is not less than `key`.
- Notes: `vector` must already be sorted according to `cmp`.
- Example: `size_t index = vs_vector_lower_bound(vector, &key, cmp_int);`

### vs_vector_upper_bound

```c
size_t vs_vector_upper_bound(const vs_vector *vector,
                             const void *key,
                             vs_vector_cmp_fn cmp);
```

- Parameters: `vector`, `key`, `cmp`
- Returns: first sorted index whose item is greater than `key`.
- Notes: `vector` must already be sorted according to `cmp`.
- Example: `size_t index = vs_vector_upper_bound(vector, &key, cmp_int);`

### vs_vector_binary_search

```c
size_t vs_vector_binary_search(const vs_vector *vector,
                               const void *key,
                               vs_vector_cmp_fn cmp);
```

- Parameters: `vector`, `key`, `cmp`
- Returns: sorted index containing `key`, or `vs_vector_size(vector)` when absent.
- Notes: when duplicates exist, returns the lower-bound index for `key`.
- Example: `size_t index = vs_vector_binary_search(vector, &key, cmp_int);`

### vs_vector_destroy

```c
void vs_vector_destroy(vs_vector *vector);
```

- Parameters: `vector`
- Returns: none.
- Notes: releases vector storage and the opaque handle. Do not use `vector` after this call.
- Example: `vs_vector_destroy(vector);`

