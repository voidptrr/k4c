# memory.allocators

## DESCRIPTION

The allocators module provides a generic allocator interface plus fail-fast
allocation helpers used by `ckit` internals.

## TYPES

### ckit_allocator

```c
typedef struct ckit_allocator {
    void *ctx;
    ckit_alloc_fn alloc;
    ckit_realloc_fn realloc;
    ckit_dealloc_fn dealloc;
} ckit_allocator;
```

- Fields: `ctx`, `alloc`, `realloc`, `dealloc`
- Notes: containers can use these callbacks when a custom allocator is provided.

## FUNCTIONS

### ckit_malloc

```c
void *ckit_malloc(size_t size);
```

- Parameters: `size`
- Returns: pointer to allocated memory.
- Behavior: prints a fatal message and aborts on out-of-memory.

### ckit_realloc

```c
void *ckit_realloc(void *ptr, size_t size);
```

- Parameters: `ptr`, `size`
- Returns: pointer to resized memory.
- Behavior: prints a fatal message and aborts on allocation failure.
- Notes: when `size == 0`, behavior follows C library `realloc` semantics.
