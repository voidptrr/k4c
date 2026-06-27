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

#include "k4c/assert.h"
#include "k4c/ds/iterator.h"
#include "k4c/ds/vector.h"
#include "k4c/error.h"
#include "k4c/memory/allocator.h"

k4c_iterator k4c_iterator_from_callback(void *context, k4c_iterator_next_fn next) {
    k4c_iterator out;

    K4C_ASSERT(next != NULL, "fatal: k4c_iterator_from_callback invalid arguments");

    out.context = context;
    out.next = next;
    out.size_hint = 0;
    return out;
}

k4c_iterator k4c_iterator_from_state(k4c_iterator_next_fn next) {
    k4c_iterator out;

    K4C_ASSERT(next != NULL, "fatal: k4c_iterator_from_state invalid arguments");

    out.context = NULL;
    out.next = next;
    out.size_hint = 0;
    return out;
}

void *k4c_iterator_state(k4c_iterator *iter) {
    K4C_ASSERT(iter != NULL, "fatal: k4c_iterator_state invalid arguments");

    return iter->state.bytes;
}

void k4c_iterator_set_size_hint(k4c_iterator *iter, size_t size_hint) {
    K4C_ASSERT(iter != NULL, "fatal: k4c_iterator_set_size_hint invalid arguments");

    iter->size_hint = size_hint;
}

size_t k4c_iterator_size_hint(const k4c_iterator *iter) {
    K4C_ASSERT(iter != NULL, "fatal: k4c_iterator_size_hint invalid arguments");

    return iter->size_hint;
}

const void *k4c_iterator_next(k4c_iterator *iter) {
    K4C_ASSERT(iter != NULL, "fatal: k4c_iterator_next invalid arguments");
    K4C_ASSERT(iter->next != NULL, "fatal: k4c_iterator_next invalid arguments");

    const void *item = iter->next(iter->context != NULL ? iter->context : iter->state.bytes);
    if (item != NULL && iter->size_hint > 0) {
        iter->size_hint -= 1;
    }
    return item;
}

k4c_status k4c_iterator_collect(
    k4c_iterator *source,
    size_t elem_size,
    k4c_allocator *k4c_allocator,
    k4c_vector **out
) {
    K4C_ASSERT(source != NULL, "fatal: k4c_iterator_collect invalid arguments");
    K4C_ASSERT(elem_size > 0, "fatal: k4c_iterator_collect invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_iterator_collect invalid arguments");

    *out = NULL;

    k4c_vector *k4c_vector = NULL;
    k4c_status st = k4c_vector_create(elem_size, k4c_allocator, &k4c_vector);
    if (st != K4C_STATUS_OK) {
        return st;
    }
    if (source->size_hint > 0) {
        st = k4c_vector_reserve(k4c_vector, source->size_hint);
        if (st != K4C_STATUS_OK) {
            k4c_vector_destroy(k4c_vector);
            return st;
        }
    }
    const void *item;
    while ((item = k4c_iterator_next(source)) != NULL) {
        st = k4c_vector_push(k4c_vector, item);
        if (st != K4C_STATUS_OK) {
            k4c_vector_destroy(k4c_vector);
            return st;
        }
    }

    *out = k4c_vector;
    return K4C_STATUS_OK;
}

k4c_status k4c_iterator_collect_map(
    k4c_iterator *source,
    size_t dst_elem_size,
    k4c_iterator_map_into_fn map,
    void *context,
    k4c_allocator *k4c_allocator,
    k4c_vector **out
) {
    K4C_ASSERT(source != NULL, "fatal: k4c_iterator_collect_map invalid arguments");
    K4C_ASSERT(dst_elem_size > 0, "fatal: k4c_iterator_collect_map invalid arguments");
    K4C_ASSERT(map != NULL, "fatal: k4c_iterator_collect_map invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_iterator_collect_map invalid arguments");

    *out = NULL;

    k4c_vector *k4c_vector = NULL;
    k4c_status st = k4c_vector_create(dst_elem_size, k4c_allocator, &k4c_vector);
    if (st != K4C_STATUS_OK) {
        return st;
    }
    if (source->size_hint > 0) {
        st = k4c_vector_reserve(k4c_vector, source->size_hint);
        if (st != K4C_STATUS_OK) {
            k4c_vector_destroy(k4c_vector);
            return st;
        }
    }
    void *dst = NULL;
    st = k4c_alloc(k4c_allocator, dst_elem_size, &dst);
    if (st != K4C_STATUS_OK) {
        k4c_vector_destroy(k4c_vector);
        return st;
    }

    const void *item;
    while ((item = k4c_iterator_next(source)) != NULL) {
        map(context, item, dst);
        st = k4c_vector_push(k4c_vector, dst);
        if (st != K4C_STATUS_OK) {
            k4c_dealloc(k4c_allocator, dst);
            k4c_vector_destroy(k4c_vector);
            return st;
        }
    }
    k4c_dealloc(k4c_allocator, dst);

    *out = k4c_vector;
    return K4C_STATUS_OK;
}
