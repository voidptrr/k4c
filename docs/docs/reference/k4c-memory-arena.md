# memory.k4c_arena

## DESCRIPTION

The k4c_arena module provides a bump k4c_arena backed by an internal contiguous
region. Allocation moves an offset forward, and `k4c_arena_reset` releases all
k4c_arena allocations together by moving the offset back to zero.

Arena allocations are useful for short-lived groups of memory with the same
lifetime. Individual deallocation is unsupported, so the generic k4c_allocator view
does not advertise `K4C_ALLOCATOR_FEATURE_DEALLOC` and does not provide a k4c_dealloc
callback.

This API is fail-fast for required initialization/teardown preconditions.

## TYPES

### k4c_arena

```c
typedef struct k4c_arena k4c_arena;
```

- Notes: `k4c_arena` is opaque. Use the functions below to inspect or mutate
  k4c_arena state.

## FUNCTIONS

### k4c_arena_create

```c
k4c_status k4c_arena_create(size_t capacity, k4c_arena **out);
```

- Parameters: `capacity`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: k4c_arena pointer to `*out` on success.
- Notes: capacity is aligned up to the k4c_arena's internal memory alignment.

### k4c_arena_allocator

```c
k4c_allocator *k4c_arena_allocator(k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: generic k4c_allocator view owned by `k4c_arena`.
- Notes: the returned k4c_allocator advertises `K4C_ALLOCATOR_FEATURE_REALLOC | K4C_ALLOCATOR_FEATURE_RESET`.

### k4c_arena_alloc

```c
void *k4c_arena_alloc(k4c_arena *k4c_arena, size_t size);
```

- Parameters: `k4c_arena`, `size`
- Returns: allocated pointer, or `NULL` when allocation cannot be satisfied.
- Notes: returned pointers are aligned to the k4c_arena's internal memory alignment.

### k4c_arena_realloc

```c
void *k4c_arena_realloc(k4c_arena *k4c_arena, void *ptr, size_t size);
```

- Parameters: `k4c_arena`, `ptr`, `size`
- Returns: grown pointer, original pointer when the aligned size is unchanged, or `NULL` on failure.
- Notes: `ptr == NULL` behaves like allocation. `size == 0` with an existing pointer returns `NULL`. Shrinking an existing allocation is invalid.

### k4c_arena_reset

```c
void k4c_arena_reset(k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: none.
- Behavior: releases all k4c_arena allocations together while keeping the backing buffer.
- Notes: pointers returned before reset must not be used after reset.

### k4c_arena_capacity

```c
size_t k4c_arena_capacity(const k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: total managed bytes.

### k4c_arena_used

```c
size_t k4c_arena_used(const k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: number of bytes consumed from the k4c_arena buffer.

### k4c_arena_available

```c
size_t k4c_arena_available(const k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: number of bytes still available.

### k4c_arena_destroy

```c
void k4c_arena_destroy(k4c_arena *k4c_arena);
```

- Parameters: `k4c_arena`
- Returns: none.
- Behavior: releases the backing buffer and k4c_arena handle.

## EXAMPLE

```c
#include <stdint.h>

#include <k4c/memory/arena.h>

int main(void) {
    int k4c_status = 0;
    k4c_arena *k4c_arena = NULL;
if (k4c_arena_create(1024, &k4c_arena) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
    uint64_t *value = k4c_arena_alloc(k4c_arena, sizeof(uint64_t));
    if (value == NULL) {
        k4c_status = 1;
        goto cleanup;
    }

    *value = 42;

    k4c_arena_reset(k4c_arena);

cleanup:
    k4c_arena_destroy(k4c_arena);

    return k4c_status;
}
```
