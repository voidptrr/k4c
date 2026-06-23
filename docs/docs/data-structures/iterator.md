# datastruct.iterator

## DESCRIPTION

The iterator module provides a generic iterator that can walk all vstd data
structures with iterator support. Data structures own their iterator state,
construction, and advancement functions, while the generic iterator module owns
common adapters such as filter, map, and take-while.

Iterators do not allocate. They borrow the data structure or caller-owned
callback context they point at. Adapter iterators borrow their source iterator,
so the source iterator must outlive any adapter built from it.

This API is fail-fast: invalid required arguments are programmer errors and are asserted.

## TYPES

### vs_iterator

```c
typedef struct vs_iterator vs_iterator;
```

Iterator value. Store it by value and pass its address to `vs_iterator_next`.

### vs_iterator_next_fn

```c
typedef const void *(*vs_iterator_next_fn)(void *context);
```

Callback used by custom iterators. Return the next item pointer, or `NULL` when
the iterator is exhausted.

### vs_iterator_predicate_fn

```c
typedef bool (*vs_iterator_predicate_fn)(void *context, const void *item);
```

Predicate callback used by `filter` and `take_while`.

### vs_iterator_map_fn

```c
typedef const void *(*vs_iterator_map_fn)(void *context, const void *item);
```

Map callback used by `map`. The returned pointer must remain valid long enough
for the caller to consume it.

### vs_hashmap_entry_view

```c
typedef struct vs_hashmap_entry_view {
    const void *key;
    const void *value;
} vs_hashmap_entry_view;
```

Hashmap iterators yield pointers to this view. The view is stored inside the
iterator and is overwritten by the next `vs_iterator_next` call on that iterator.

## FUNCTIONS

### vs_iterator_from_callback

```c
vs_iterator vs_iterator_from_callback(void *context, vs_iterator_next_fn next);
```

- Parameters: `context`, `next`
- Returns: iterator backed by caller-owned state.
- Notes: use this to integrate custom data structures with vstd adapters.

### vs_iterator_next

```c
const void *vs_iterator_next(vs_iterator *iter);
```

- Parameters: `iter`
- Returns: next item pointer, or `NULL` when exhausted.

### vs_iterator_filter

```c
vs_iterator vs_iterator_filter(vs_iterator *source,
                               vs_iterator_predicate_fn predicate,
                               void *context);
```

- Parameters: `source`, `predicate`, `context`
- Returns: iterator that yields only source items accepted by `predicate`.
- Notes: `source` must outlive the returned iterator.

### vs_iterator_map

```c
vs_iterator vs_iterator_map(vs_iterator *source, vs_iterator_map_fn map, void *context);
```

- Parameters: `source`, `map`, `context`
- Returns: iterator that yields `map(context, item)` for each source item.
- Notes: `source` must outlive the returned iterator.

### vs_iterator_take_while

```c
vs_iterator vs_iterator_take_while(vs_iterator *source,
                                   vs_iterator_predicate_fn predicate,
                                   void *context);
```

- Parameters: `source`, `predicate`, `context`
- Returns: iterator that stops after the first source item rejected by `predicate`.
- Notes: `source` must outlive the returned iterator.

## EXAMPLE

```c
#include <vstd/datastruct/iterator.h>
#include <vstd/datastruct/vector.h>

static bool int_is_even(void *context, const void *item) {
    (void)context;
    return (*(const int *)item % 2) == 0;
}

int main(void) {
    vs_vector *vector = vs_vector_create(sizeof(int), NULL);
    for (int i = 0; i < 4; i++) {
        vs_vector_push(vector, &i);
    }

    vs_vector_iterator_state vector_state;
    vs_iterator base = vs_vector_iterator(&vector_state, vector);
    vs_iterator even = vs_iterator_filter(&base, int_is_even, NULL);

    const int *item;
    while ((item = (const int *)vs_iterator_next(&even)) != NULL) {
        /* use *item */
    }

    vs_vector_destroy(vector);
    return 0;
}
```

## CUSTOM ITERATORS

```c
typedef struct range_iter {
    int current;
    int end;
} range_iter;

static const void *range_next(void *context) {
    range_iter *iter = context;
    if (iter->current >= iter->end) {
        return NULL;
    }

    iter->current += 1;
    return &iter->current;
}

int main(void) {
    range_iter state = {.current = 0, .end = 10};
    vs_iterator iter = vs_iterator_from_callback(&state, range_next);
    while (vs_iterator_next(&iter) != NULL) {
    }
    return 0;
}
```
