# memory.k4c_allocator

## DESCRIPTION

The memory k4c_allocator API provides a generic k4c_allocator interface plus checked
allocation helpers used by `k4c` internals.

Owning APIs follow a construction-time k4c_allocator convention: pass an k4c_allocator
to `*_create`, and the created object stores that k4c_allocator pointer for later
growth and `*_destroy`. Mutation and destroy functions do not take another
k4c_allocator argument because freeing with a different k4c_allocator than the one used
for allocation is unsafe. The k4c_allocator object must outlive every object that
captures it. Passing `NULL` selects the C library k4c_heap.

## TYPES

### k4c_allocator

```c
typedef struct k4c_allocator {
    void *ctx;
    unsigned int features;
    alloc_fn k4c_alloc;
    realloc_fn realloc;
    dealloc_fn k4c_dealloc;
} k4c_allocator;
```

- Fields: `ctx`, `features`, `k4c_alloc`, `realloc`, `k4c_dealloc`
- Notes: owning containers store this pointer at create time and reuse it for
  later allocation, reallocation, and destroy work.

### k4c_allocator_features

```c
typedef enum k4c_allocator_features {
    K4C_ALLOCATOR_FEATURE_DEALLOC = 1 << 0,
    K4C_ALLOCATOR_FEATURE_REALLOC = 1 << 1,
    K4C_ALLOCATOR_FEATURE_RESET = 1 << 2,
} k4c_allocator_features;
```

- `K4C_ALLOCATOR_FEATURE_DEALLOC`: k4c_allocator supports freeing individual allocations.
- `K4C_ALLOCATOR_FEATURE_REALLOC`: k4c_allocator supports resizing allocations.
- `K4C_ALLOCATOR_FEATURE_RESET`: k4c_allocator supports releasing all allocations together.

## FUNCTIONS

### k4c_alloc

```c
k4c_status k4c_alloc(k4c_allocator *k4c_allocator, size_t size, void **out);
```

- Parameters: `k4c_allocator`, `size`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: allocated memory pointer to `*out` on success; `NULL` on failure.
- Notes: when `k4c_allocator` is `NULL` or has no `k4c_alloc` callback, uses the C library k4c_heap.

### k4c_resize

```c
k4c_status k4c_resize(k4c_allocator *k4c_allocator, void *ptr, size_t size, void **out);
```

- Parameters: `k4c_allocator`, `ptr`, `size`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: resized memory pointer to `*out` on success; `NULL` when `size == 0` or allocation fails.
- Notes: when `k4c_allocator` is `NULL` or has no `realloc` callback, uses the C library k4c_heap. When `size == 0`, deallocates `ptr`.

### k4c_dealloc

```c
void k4c_dealloc(k4c_allocator *k4c_allocator, void *ptr);
```

- Parameters: `k4c_allocator`, `ptr`
- Returns: none.
- Notes: when `k4c_allocator` is `NULL`, uses the C library k4c_heap. If a custom k4c_allocator does not advertise `K4C_ALLOCATOR_FEATURE_DEALLOC`, this function returns without calling a k4c_dealloc callback.
