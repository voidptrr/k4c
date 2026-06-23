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

#include <stddef.h>

#include "vstd/assert.h"
#include "vstd/datastruct/binary_heap.h"
#include "vstd/datastruct/deque.h"
#include "vstd/datastruct/doubly_linked_list.h"
#include "vstd/datastruct/hashmap.h"
#include "vstd/datastruct/hashset.h"
#include "vstd/datastruct/iterator.h"
#include "vstd/datastruct/linked_list.h"
#include "vstd/datastruct/string.h"
#include "vstd/datastruct/vector.h"

static const void *vs_iterator_next_callback(vs_iterator *iter) {
    return iter->as.callback.next(iter->as.callback.context);
}

static const void *vs_iterator_next_filter(vs_iterator *iter) {
    const void *item;

    while ((item = vs_iterator_next(iter->as.filter.source)) != NULL) {
        if (iter->as.filter.predicate(iter->as.filter.context, item)) {
            return item;
        }
    }

    return NULL;
}

static const void *vs_iterator_next_map(vs_iterator *iter) {
    const void *item = vs_iterator_next(iter->as.map.source);

    if (item == NULL) {
        return NULL;
    }

    return iter->as.map.map(iter->as.map.context, item);
}

static const void *vs_iterator_next_take_while(vs_iterator *iter) {
    const void *item;

    if (iter->as.take_while.done) {
        return NULL;
    }

    item = vs_iterator_next(iter->as.take_while.source);
    if (item == NULL || !iter->as.take_while.predicate(iter->as.take_while.context, item)) {
        iter->as.take_while.done = true;
        return NULL;
    }

    return item;
}

const void *vs_iterator_next(vs_iterator *iter) {
    VSTD_ASSERT(iter != NULL, "fatal: vs_iterator_next invalid arguments");

    switch (iter->type) {
        case VS_ITERATOR_CALLBACK:
            return vs_iterator_next_callback(iter);
        case VS_ITERATOR_BINARY_HEAP:
            return vs_binary_heap_iterator_next(iter);
        case VS_ITERATOR_DEQUE:
            return vs_deque_iterator_next(iter);
        case VS_ITERATOR_DOUBLY_LINKED_LIST:
            return vs_doubly_linked_list_iterator_next(iter);
        case VS_ITERATOR_VECTOR:
            return vs_vector_iterator_next(iter);
        case VS_ITERATOR_FILTER:
            return vs_iterator_next_filter(iter);
        case VS_ITERATOR_HASHMAP:
            return vs_hashmap_iterator_next(iter);
        case VS_ITERATOR_HASHSET:
            return vs_hashset_iterator_next(iter);
        case VS_ITERATOR_LINKED_LIST:
            return vs_linked_list_iterator_next(iter);
        case VS_ITERATOR_MAP:
            return vs_iterator_next_map(iter);
        case VS_ITERATOR_STRING:
            return vs_string_iterator_next(iter);
        case VS_ITERATOR_TAKE_WHILE:
            return vs_iterator_next_take_while(iter);
    }

    VSTD_ASSERT(false, "fatal: vs_iterator_next invalid iterator type");
    return NULL;
}

vs_iterator vs_iterator_from_callback(void *context, vs_iterator_next_fn next) {
    vs_iterator out;

    VSTD_ASSERT(next != NULL, "fatal: vs_iterator_from_callback invalid arguments");

    out.type = VS_ITERATOR_CALLBACK;
    out.as.callback.context = context;
    out.as.callback.next = next;
    return out;
}

vs_iterator vs_iterator_filter(
    vs_iterator *source,
    vs_iterator_predicate_fn predicate,
    void *context
) {
    vs_iterator out;

    VSTD_ASSERT(source != NULL, "fatal: vs_iterator_filter invalid arguments");
    VSTD_ASSERT(predicate != NULL, "fatal: vs_iterator_filter invalid arguments");

    out.type = VS_ITERATOR_FILTER;
    out.as.filter.source = source;
    out.as.filter.predicate = predicate;
    out.as.filter.context = context;
    return out;
}

vs_iterator vs_iterator_map(vs_iterator *source, vs_iterator_map_fn map, void *context) {
    vs_iterator out;

    VSTD_ASSERT(source != NULL, "fatal: vs_iterator_map invalid arguments");
    VSTD_ASSERT(map != NULL, "fatal: vs_iterator_map invalid arguments");

    out.type = VS_ITERATOR_MAP;
    out.as.map.source = source;
    out.as.map.map = map;
    out.as.map.context = context;
    return out;
}

vs_iterator vs_iterator_take_while(
    vs_iterator *source,
    vs_iterator_predicate_fn predicate,
    void *context
) {
    vs_iterator out;

    VSTD_ASSERT(source != NULL, "fatal: vs_iterator_take_while invalid arguments");
    VSTD_ASSERT(predicate != NULL, "fatal: vs_iterator_take_while invalid arguments");

    out.type = VS_ITERATOR_TAKE_WHILE;
    out.as.take_while.source = source;
    out.as.take_while.predicate = predicate;
    out.as.take_while.context = context;
    out.as.take_while.done = false;
    return out;
}
