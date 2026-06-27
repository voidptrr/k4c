# ds.k4c_iterator

## DESCRIPTION

The k4c_iterator module provides a generic k4c_iterator that can walk all k4c data
structures with k4c_iterator support. Data structures own their k4c_iterator state,
construction, and advancement functions. The generic k4c_iterator module only owns
the basic callback k4c_iterator shape and eager collection helpers.

Iterators do not allocate. They borrow the data structure or caller-owned
callback context they point at.

The `context` parameter is intentionally generic. For `k4c_iterator_from_callback`
it is usually cursor state, such as an index into a k4c_vector. Pass `NULL` when a
callback does not need extra data.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### k4c_iterator

```c
typedef struct k4c_iterator k4c_iterator;
```

Iterator value. Store it by value and pass its address to `k4c_iterator_next`.

### k4c_iterator_next_fn

```c
typedef const void *(*k4c_iterator_next_fn)(void *context);
```

Callback used by custom iterators. Return the next item pointer, or `NULL` when
the k4c_iterator is exhausted.

### k4c_iterator_map_into_fn

```c
typedef void (*k4c_iterator_map_into_fn)(void *context, const void *src, void *dst);
```

Map callback used by `collect_map`. Write the mapped value into `dst`. The
destination storage has the element size passed to `k4c_iterator_collect_map`.

## FUNCTIONS

### K4C_ITER_NEXT_AS

```c
#define K4C_ITER_NEXT_AS(type, iter)
```

- Parameters: `type`, `iter`
- Returns: typed pointer from `k4c_iterator_next`.
- Example:

```c
const int *item = K4C_ITER_NEXT_AS(int, &iter);
```

### k4c_iterator_for_each

```c
#define k4c_iterator_for_each(type, item, iter)
```

- Parameters: `type`, `item`, `iter`
- Notes: declares `const type *item` scoped to the loop body.
- Example:

```c
k4c_iterator_for_each(int, item, &iter) {
    /* use *item */
}
```

### k4c_iterator_from_callback

```c
k4c_iterator k4c_iterator_from_callback(void *context, k4c_iterator_next_fn next);
```

- Parameters: `context`, `next`
- Returns: k4c_iterator backed by caller-owned state.
- Notes: use this to integrate custom data structures with k4c helpers such as
  `k4c_iterator_collect`.
- Example:

```c
typedef struct range_state {
    int current;
    int end;
} range_state;

static const void *range_next(void *context) {
    range_state *state = context;
    if (state->current >= state->end) {
        return NULL;
    }
    state->current += 1;
    return &state->current;
}

range_state state = {.current = 0, .end = 10};
k4c_iterator iter = k4c_iterator_from_callback(&state, range_next);
```

### k4c_iterator_next

```c
const void *k4c_iterator_next(k4c_iterator *iter);
```

- Parameters: `iter`
- Returns: next item pointer, or `NULL` when exhausted.
- Example:

```c
const int *item;
while ((item = (const int *)k4c_iterator_next(&iter)) != NULL) {
    /* use *item */
}
```

### k4c_iterator_set_size_hint

```c
void k4c_iterator_set_size_hint(k4c_iterator *iter, size_t size_hint);
```

- Parameters: `iter`, `size_hint`
- Returns: none.
- Notes: sets a conservative remaining-item count used by collect helpers to
  reserve output storage.

### k4c_iterator_size_hint

```c
size_t k4c_iterator_size_hint(const k4c_iterator *iter);
```

- Parameters: `iter`
- Returns: remaining-item hint, or zero when unknown.

### k4c_iterator_collect

```c
k4c_status k4c_iterator_collect(k4c_iterator *source,
                              size_t elem_size,
                              k4c_allocator *k4c_allocator,
                              k4c_vector **out);
```

- Parameters: `source`, `elem_size`, `k4c_allocator`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: k4c_vector containing copies of the remaining source items to `*out`.
- Notes: consumes `source`. The returned k4c_vector owns its storage and can outlive
  the original data structure.
- Example:

```c
k4c_vector *values = NULL;
if (k4c_vector_create(sizeof(int), NULL, &values) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
int one = 1;
int two = 2;
if (k4c_vector_push(values, &one) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
if (k4c_vector_push(values, &two) != K4C_STATUS_OK) {
    /* handle allocation failure */
}

k4c_iterator iter = k4c_vector_get_iterator(values);
k4c_vector *copy = NULL;
if (k4c_iterator_collect(&iter, sizeof(int), NULL, &copy) != K4C_STATUS_OK) {
    /* handle allocation failure */
}

k4c_vector_destroy(values);
/* copy still owns the collected ints. */
k4c_vector_destroy(copy);
```

### k4c_iterator_collect_map

```c
k4c_status k4c_iterator_collect_map(k4c_iterator *source,
                                  size_t dst_elem_size,
                                  k4c_iterator_map_into_fn map,
                                  void *context,
                                  k4c_allocator *k4c_allocator,
                                  k4c_vector **out);
```

- Parameters: `source`, `dst_elem_size`, `map`, `context`, `k4c_allocator`, `out`
- Returns: `K4C_STATUS_OK` on success, or an error k4c_status.
- Writes: k4c_vector containing mapped copies of the remaining source items to `*out`.
- Notes: consumes `source`. Use this when the output element type or size is
  different from the source element type.
- Example:

```c
static void int_to_double(void *context, const void *src, void *dst) {
    (void)context;
    *(double *)dst = (double)*(const int *)src;
}

k4c_vector *ints = NULL;
if (k4c_vector_create(sizeof(int), NULL, &ints) != K4C_STATUS_OK) {
    /* handle allocation failure */
}
int value = 42;
if (k4c_vector_push(ints, &value) != K4C_STATUS_OK) {
    /* handle allocation failure */
}

k4c_iterator iter = k4c_vector_get_iterator(ints);
k4c_vector *doubles = NULL;
if (k4c_iterator_collect_map(&iter, sizeof(double), int_to_double, NULL, NULL, &doubles) != K4C_STATUS_OK) {
    /* handle allocation failure */
}

const double *mapped = (const double *)k4c_vector_get(doubles, 0);
/* *mapped == 42.0 */

k4c_vector_destroy(ints);
k4c_vector_destroy(doubles);
```
