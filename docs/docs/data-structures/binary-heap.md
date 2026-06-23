# datastruct.binary_heap

## DESCRIPTION

The binary_heap module provides a generic binary heap backed by contiguous vector storage.
Heap ordering is defined by a caller callback. When the callback is `NULL`,
ordering is byte-wise with `memcmp` over stored element bytes.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### vs_binary_heap_iterator_state

```c
typedef struct vs_binary_heap_iterator_state vs_binary_heap_iterator_state;
```

Caller-owned cursor state for `vs_binary_heap_iterator`.

## FUNCTIONS

### vs_binary_heap_create

```c
vs_binary_heap *vs_binary_heap_create(size_t elem_size,
                                        vs_binary_heap_cmp_fn cmp,
                                        vs_allocator *allocator);
```

- Parameters: `elem_size`, `cmp`, `allocator`
- Returns: opaque binary-heap handle.
- Notes: the binary heap stores `allocator` and reuses it for growth and
  destroy. When `allocator` is `NULL`, binary heap storage uses the C library
  heap through `vs_malloc`/`vs_realloc`. When `cmp` is `NULL`, element ordering
  uses byte comparison.
- Example: `vs_binary_heap *heap = vs_binary_heap_create(sizeof(int), cmp_int, NULL);`

### vs_binary_heap_push

```c
void vs_binary_heap_push(vs_binary_heap *heap, const void *element);
```

- Parameters: `heap`, `element`
- Returns: none.
- Example: `vs_binary_heap_push(heap, &value);`

### vs_binary_heap_pop

```c
void *vs_binary_heap_pop(vs_binary_heap *heap);
```

- Parameters: `heap`
- Returns: pointer to removed top element in heap-managed storage, or `NULL` when empty.
- Example: `int *top = (int *)vs_binary_heap_pop(heap);`

### vs_binary_heap_peek

```c
const void *vs_binary_heap_peek(const vs_binary_heap *heap);
```

- Parameters: `heap`
- Returns: pointer to top element in heap-managed storage, or `NULL` when empty.
- Example: `const int *top = (const int *)vs_binary_heap_peek(heap);`

### vs_binary_heap_size

```c
size_t vs_binary_heap_size(const vs_binary_heap *heap);
```

- Parameters: `heap`
- Returns: current element count.
- Example: `size_t count = vs_binary_heap_size(heap);`

### vs_binary_heap_iterator

```c
vs_iterator vs_binary_heap_iterator(vs_binary_heap_iterator_state *state,
                                    const vs_binary_heap *heap);
```

- Parameters: `state`, `heap`
- Returns: iterator over the heap's backing-storage order.
- Notes: `state` must outlive the returned iterator. Iteration order is not
  sorted order. Do not mutate the heap while iterating.
- Example: `vs_iterator iter = vs_binary_heap_iterator(&state, heap);`

### vs_binary_heap_destroy

```c
void vs_binary_heap_destroy(vs_binary_heap *heap);
```

- Parameters: `heap`
- Returns: none.
- Notes: releases heap storage and the opaque handle. Do not use `heap` after this call.
- Example: `vs_binary_heap_destroy(heap);`

