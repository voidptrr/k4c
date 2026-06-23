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
 * Generic iterator over caller-owned data.
 *
 * Iterator constructors return fully initialized iterator values. Adapters keep
 * pointers to their source iterators, so the source iterator must outlive any
 * adapter built from it.
 *
 * vs_iterator_next returns the next item pointer, or NULL when exhausted.
 */
typedef const void *(*vs_iterator_next_fn)(void *context);
typedef bool (*vs_iterator_predicate_fn)(void *context, const void *item);
typedef const void *(*vs_iterator_map_fn)(void *context, const void *item);

typedef enum vs_iterator_type {
    VS_ITERATOR_CALLBACK,
    VS_ITERATOR_FILTER,
    VS_ITERATOR_MAP,
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
            struct vs_iterator *source;
            vs_iterator_predicate_fn predicate;
            void *context;
        } filter;

        struct {
            struct vs_iterator *source;
            vs_iterator_map_fn map;
            void *context;
        } map;

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
