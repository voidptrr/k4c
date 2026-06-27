# ds.k4c_vector

## DESCRIPTION

The k4c_vector module provides a generic contiguous growable array for fixed-size elements.
Elements are copied into k4c_vector-owned storage on push.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### k4c_vector_iterator

```c
typedef struct k4c_vector_iterator k4c_vector_iterator;
```

Typed cursor for walking k4c_vector elements.

## FUNCTIONS

### k4c_vector_for_each

```c
#define k4c_vector_for_each(type, item, k4c_vector)
```

- Parameters: `type`, `item`, `k4c_vector`
- Notes: declares `const type *item` scoped to the loop body.
- Example:

```c
k4c_vector_for_each(int, item, k4c_vector) {
    /* use *item */
}
```

### k4c_vector_create

```c
k4c_status k4c_vector_create(size_t elem_size, k4c_allocator *k4c_allocator, k4c_vector **out);
```

- Parameters: `elem_size`, `k4c_allocator`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: opaque k4c_vector handle to `*out` on success.
- Notes: the k4c_vector stores `k4c_allocator` and reuses it for growth and destroy.
  When `k4c_allocator` is `NULL`, k4c_vector uses the C library k4c_heap through
  `k4c_alloc`/`k4c_resize`.
- Example:

```c
k4c_vector *k4c_vector = NULL;
if (k4c_vector_create(sizeof(int), NULL, &k4c_vector) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
```

### k4c_vector_create_with_capacity

```c
k4c_status k4c_vector_create_with_capacity(size_t elem_size,
                                        size_t capacity,
                                        k4c_allocator *k4c_allocator,
                                        k4c_vector **out);
```

- Parameters: `elem_size`, `capacity`, `k4c_allocator`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: opaque k4c_vector handle with at least `capacity` slots allocated to `*out`.
- Example:

```c
k4c_vector *k4c_vector = NULL;
if (k4c_vector_create_with_capacity(sizeof(int), 128, NULL, &k4c_vector) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
```

### k4c_vector_reserve

```c
k4c_status k4c_vector_reserve(k4c_vector *k4c_vector, size_t capacity);
```

- Parameters: `k4c_vector`, `capacity`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Notes: grows backing storage when needed; existing items are preserved.
- Example:

```c
if (k4c_vector_reserve(k4c_vector, 1024) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
```

### k4c_vector_push

```c
k4c_status k4c_vector_push(k4c_vector *k4c_vector, const void *element);
```

- Parameters: `k4c_vector`, `element`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Example:

```c
int value = 42;
if (k4c_vector_push(k4c_vector, &value) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
```

### k4c_vector_pop

```c
void *k4c_vector_pop(k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: pointer to the removed element within k4c_vector-managed storage, or `NULL` when the k4c_vector is empty.
- Example:

```c
int *last = (int *)k4c_vector_pop(k4c_vector);
if (last != NULL) {
    /* use *last */
}
```

### k4c_vector_get

```c
void *k4c_vector_get(const k4c_vector *k4c_vector, size_t index);
```

- Parameters: `k4c_vector`, `index`
- Returns: pointer to item at `index`, or `NULL` when out of range.
- Example:

```c
int *item = (int *)k4c_vector_get(k4c_vector, 0);
if (item != NULL) {
    *item = 10;
}
```

### k4c_vector_elem_size

```c
size_t k4c_vector_elem_size(const k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: configured element size.
- Example:

```c
size_t elem_size = k4c_vector_elem_size(k4c_vector);
```

### k4c_vector_capacity

```c
size_t k4c_vector_capacity(const k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: number of elements that fit without another allocation.

### k4c_vector_data

```c
void *k4c_vector_data(const k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: pointer to k4c_vector backing storage.
- Notes: the pointer may be invalidated by later growth.

### k4c_vector_swap_remove

```c
void *k4c_vector_swap_remove(k4c_vector *k4c_vector, size_t index);
```

- Parameters: `k4c_vector`, `index`
- Returns: pointer to the slot where the removed item lived, or `NULL` when out of range.
- Notes: removal does not preserve order; the last item is moved into `index`.
- Example:

```c
int *slot = (int *)k4c_vector_swap_remove(k4c_vector, 0);
if (slot != NULL) {
    /* slot now contains the item moved from the old last position */
}
```

### k4c_vector_size

```c
size_t k4c_vector_size(const k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: current element count.
- Example:

```c
size_t count = k4c_vector_size(k4c_vector);
```

### k4c_vector_get_iterator

```c
k4c_iterator k4c_vector_get_iterator(const k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: k4c_iterator over k4c_vector elements from index `0` to `size - 1`.
- Notes: yielded pointers refer to k4c_vector-managed storage. Do not mutate the
  k4c_vector while iterating.
- Example:

```c
k4c_iterator iter = k4c_vector_get_iterator(k4c_vector);

const int *item;
while ((item = (const int *)k4c_iterator_next(&iter)) != NULL) {
    /* use *item */
}
```

### k4c_vector_lower_bound

```c
size_t k4c_vector_lower_bound(const k4c_vector *k4c_vector,
                             const void *key,
                             k4c_vector_cmp_fn cmp);
```

- Parameters: `k4c_vector`, `key`, `cmp`
- Returns: first sorted index whose item is not less than `key`.
- Notes: `k4c_vector` must already be sorted according to `cmp`.
- Example:

```c
static int cmp_int(const void *lhs, const void *rhs) {
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;
    return (a > b) - (a < b);
}

int key = 20;
size_t index = k4c_vector_lower_bound(k4c_vector, &key, cmp_int);
```

### k4c_vector_upper_bound

```c
size_t k4c_vector_upper_bound(const k4c_vector *k4c_vector,
                             const void *key,
                             k4c_vector_cmp_fn cmp);
```

- Parameters: `k4c_vector`, `key`, `cmp`
- Returns: first sorted index whose item is greater than `key`.
- Notes: `k4c_vector` must already be sorted according to `cmp`.
- Example:

```c
int key = 20;
size_t index = k4c_vector_upper_bound(k4c_vector, &key, cmp_int);
```

### k4c_vector_binary_search

```c
size_t k4c_vector_binary_search(const k4c_vector *k4c_vector,
                               const void *key,
                               k4c_vector_cmp_fn cmp);
```

- Parameters: `k4c_vector`, `key`, `cmp`
- Returns: sorted index containing `key`, or `k4c_vector_size(k4c_vector)` when absent.
- Notes: when duplicates exist, returns the lower-bound index for `key`.
- Example:

```c
int key = 20;
size_t index = k4c_vector_binary_search(k4c_vector, &key, cmp_int);
if (index != k4c_vector_size(k4c_vector)) {
    const int *found = (const int *)k4c_vector_get(k4c_vector, index);
}
```

### k4c_vector_destroy

```c
void k4c_vector_destroy(k4c_vector *k4c_vector);
```

- Parameters: `k4c_vector`
- Returns: none.
- Notes: releases k4c_vector storage and the opaque handle. Do not use `k4c_vector` after this call.
- Example:

```c
k4c_vector_destroy(k4c_vector);
```
