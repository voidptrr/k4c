# memory.general_heap

## DESCRIPTION

The k4c_heap module provides an in-process free-list k4c_allocator backed by an
internal contiguous region. It supports allocation, deallocation, and reallocation
using a coalescing free-list strategy.

This API is fail-fast for required initialization/teardown preconditions.

## TYPES

### k4c_heap

```c
typedef struct k4c_heap k4c_heap;
```

- Notes: `k4c_heap` is opaque. Use the functions below to inspect or mutate
  k4c_heap state.

## FUNCTIONS

### k4c_heap_create

```c
k4c_status k4c_heap_create(size_t capacity, k4c_heap **out);
```

- Parameters: `capacity`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: k4c_heap pointer to `*out` on success.

### k4c_heap_allocator

```c
k4c_allocator *k4c_heap_allocator(k4c_heap *k4c_heap);
```

- Parameters: `k4c_heap`
- Returns: generic k4c_allocator view owned by `k4c_heap`.
- Notes: the returned k4c_allocator is owned by `k4c_heap` and advertises `K4C_ALLOCATOR_FEATURE_DEALLOC | K4C_ALLOCATOR_FEATURE_REALLOC`.

### k4c_heap_alloc

```c
void *k4c_heap_alloc(k4c_heap *k4c_heap, size_t size);
```

- Parameters: `k4c_heap`, `size`
- Returns: allocated pointer, or `NULL` when allocation cannot be satisfied.

### k4c_heap_dealloc

```c
void k4c_heap_dealloc(k4c_heap *k4c_heap, void *ptr);
```

- Parameters: `k4c_heap`, `ptr`
- Behavior: frees `ptr` if valid.

### k4c_heap_realloc

```c
void *k4c_heap_realloc(k4c_heap *k4c_heap, void *ptr, size_t size);
```

- Parameters: `k4c_heap`, `ptr`, `size`
- Returns: resized pointer, or `NULL` on failure.
- Notes: `ptr == NULL` behaves like allocation; `size == 0` frees `ptr` and returns `NULL`.

### k4c_heap_capacity

```c
size_t k4c_heap_capacity(const k4c_heap *k4c_heap);
```

- Parameters: `k4c_heap`
- Returns: total managed bytes.

### k4c_heap_available

```c
size_t k4c_heap_available(const k4c_heap *k4c_heap);
```

- Parameters: `k4c_heap`
- Returns: sum of currently free payload bytes.

### k4c_heap_destroy

```c
void k4c_heap_destroy(k4c_heap *k4c_heap);
```

- Parameters: `k4c_heap`
- Returns: none.
- Behavior: releases the backing buffer and k4c_heap handle.
