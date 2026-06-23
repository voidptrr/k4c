# datastruct.iterator

## DESCRIPTION

The iterator module provides a generic iterator that can walk all vstd data
structures with iterator support. Data structures own their iterator state,
construction, and advancement functions, while the generic iterator module owns
common adapters such as filter, map, and take-while.

Iterators do not allocate. They borrow the data structure or caller-owned
callback context they point at. Adapter iterators borrow their source iterator,
so the source iterator must outlive any adapter built from it.

The `context` parameter is intentionally generic. For `vs_iterator_from_callback`
it is usually cursor state, such as an index into a vector. For adapter
callbacks it is caller data for the predicate or map function. Pass `NULL` when
a callback does not need extra data.

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

## FUNCTIONS

### vs_iterator_from_callback

```c
vs_iterator vs_iterator_from_callback(void *context, vs_iterator_next_fn next);
```

- Parameters: `context`, `next`
- Returns: iterator backed by caller-owned state.
- Notes: use this to integrate custom data structures with vstd adapters.
- Example: `vs_iterator iter = vs_iterator_from_callback(&state, next_fn);`

### vs_iterator_next

```c
const void *vs_iterator_next(vs_iterator *iter);
```

- Parameters: `iter`
- Returns: next item pointer, or `NULL` when exhausted.
- Example: `while ((item = vs_iterator_next(&iter)) != NULL) { }`

### vs_iterator_filter

```c
vs_iterator vs_iterator_filter(vs_iterator *source,
                               vs_iterator_predicate_fn predicate,
                               void *context);
```

- Parameters: `source`, `predicate`, `context`
- Returns: iterator that yields only source items accepted by `predicate`.
- Notes: `source` must outlive the returned iterator.
- Example: `vs_iterator even = vs_iterator_filter(&base, int_is_even, NULL);`

### vs_iterator_map

```c
vs_iterator vs_iterator_map(vs_iterator *source, vs_iterator_map_fn map, void *context);
```

- Parameters: `source`, `map`, `context`
- Returns: iterator that yields `map(context, item)` for each source item.
- Notes: `source` must outlive the returned iterator.
- Example: `vs_iterator squares = vs_iterator_map(&base, int_square, &scratch);`

### vs_iterator_take_while

```c
vs_iterator vs_iterator_take_while(vs_iterator *source,
                                   vs_iterator_predicate_fn predicate,
                                   void *context);
```

- Parameters: `source`, `predicate`, `context`
- Returns: iterator that stops after the first source item rejected by `predicate`.
- Notes: `source` must outlive the returned iterator.
- Example: `vs_iterator prefix = vs_iterator_take_while(&base, int_less_than, &limit);`
