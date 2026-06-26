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

#include "vstd/memory/allocator.h"
#include "vstd/memory/general_heap.h"
#include "vstd/testing.h"

int main(void) {
    vs_heap *heap = vs_heap_create(1024);
    vs_allocator *allocator = vs_heap_allocator(heap);

    if (vs_test_equal_ptr(allocator->ctx, heap) != 0) {
        return 1;
    }
    if (vs_test_equal(allocator->alloc != NULL, true) != 0) {
        return 1;
    }
    if (vs_test_equal(allocator->realloc != NULL, true) != 0) {
        return 1;
    }
    if (vs_test_equal(allocator->dealloc != NULL, true) != 0) {
        return 1;
    }
    if (vs_test_equal(
            allocator->features == (VS_ALLOCATOR_FEATURE_DEALLOC | VS_ALLOCATOR_FEATURE_REALLOC),
            true
        )
        != 0) {
        return 1;
    }

    if (vs_test_equal(vs_heap_capacity(heap) > 0, true) != 0) {
        return 1;
    }
    if (vs_test_equal(vs_heap_available(heap) > 0, true) != 0) {
        return 1;
    }

    vs_heap_destroy(heap);
    return 0;
}
