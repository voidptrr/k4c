/*
 * MIT License
 *
 * Copyright (c) 2026 Tommaso Bruno
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef VSTD_ITERATOR_H
#define VSTD_ITERATOR_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Tagged generic iterator over caller-owned data.
 *
 * Iterator constructors return fully initialized iterator values. Adapters keep
 * pointers to their source iterators, so the source iterator must outlive any
 * adapter built from it.
 *
 * vs_iterator_next returns the next item pointer, or NULL when exhausted.
 */
typedef struct vs_binary_heap vs_binary_heap;
typedef struct vs_deque vs_deque;
typedef struct vs_doubly_linked_list_node vs_doubly_linked_list_node;
typedef struct vs_hashmap vs_hashmap;
typedef struct vs_hashmap_entry_view {
    const void *key;
    const void *value;
} vs_hashmap_entry_view;
typedef struct vs_hashset vs_hashset;
typedef struct vs_linked_list_node vs_linked_list_node;
typedef struct vs_vector vs_vector;

typedef const void *(*vs_iterator_next_fn)(void *context);
typedef bool (*vs_iterator_predicate_fn)(void *context, const void *item);
typedef const void *(*vs_iterator_map_fn)(void *context, const void *item);

typedef enum vs_iterator_type {
    VS_ITERATOR_CALLBACK,
    VS_ITERATOR_BINARY_HEAP,
    VS_ITERATOR_DEQUE,
    VS_ITERATOR_DOUBLY_LINKED_LIST,
    VS_ITERATOR_VECTOR,
    VS_ITERATOR_FILTER,
    VS_ITERATOR_HASHMAP,
    VS_ITERATOR_HASHSET,
    VS_ITERATOR_LINKED_LIST,
    VS_ITERATOR_MAP,
    VS_ITERATOR_STRING,
    VS_ITERATOR_TAKE_WHILE,
} vs_iterator_type;

typedef struct vs_iterator {
    vs_iterator_type type;

    union {
        struct {
            void *context;
            vs_iterator_next_fn next;
        } callback;

        struct {
            const vs_binary_heap *heap;
            size_t index;
        } binary_heap;

        struct {
            const vs_deque *deque;
            size_t index;
        } deque;

        struct {
            vs_doubly_linked_list_node *node;
        } doubly_linked_list;

        struct {
            const vs_vector *vector;
            size_t index;
        } vector;

        struct {
            struct vs_iterator *source;
            vs_iterator_predicate_fn predicate;
            void *context;
        } filter;

        struct {
            const vs_hashmap *map;
            size_t bucket;
            vs_linked_list_node *node;
            vs_hashmap_entry_view entry;
        } hashmap;

        struct {
            const vs_hashset *set;
            size_t bucket;
            vs_linked_list_node *node;
        } hashset;

        struct {
            vs_linked_list_node *node;
        } linked_list;

        struct {
            struct vs_iterator *source;
            vs_iterator_map_fn map;
            void *context;
        } map;

        struct {
            const char *string;
            size_t index;
        } string;

        struct {
            struct vs_iterator *source;
            vs_iterator_predicate_fn predicate;
            void *context;
            bool done;
        } take_while;
    } as;
} vs_iterator;

/* Return an iterator backed by caller-owned context and a next callback. */
vs_iterator vs_iterator_from_callback(void *context, vs_iterator_next_fn next);

/* Return the next item from iter, or NULL when exhausted. */
const void *vs_iterator_next(vs_iterator *iter);

/* Return an iterator that yields only source items accepted by predicate. */
vs_iterator vs_iterator_filter(
    vs_iterator *source,
    vs_iterator_predicate_fn predicate,
    void *context
);

/* Return an iterator that yields map(context, item) for each source item. */
vs_iterator vs_iterator_map(vs_iterator *source, vs_iterator_map_fn map, void *context);

/* Return an iterator that stops after the first source item rejected by predicate. */
vs_iterator vs_iterator_take_while(
    vs_iterator *source,
    vs_iterator_predicate_fn predicate,
    void *context
);

#endif
