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
#include <stdint.h>
#include <string.h>

#include "k4c/buffer/cursor.h"
#include "k4c/error.h"
#include "k4c/io/reader.h"
#include "k4c/testing.h"

typedef struct test_reader_context {
    k4c_buf_cursor cursor;
} test_reader_context;

static k4c_status test_reader_take_byte(void *context, uint8_t *out);
static k4c_status test_reader_take_delimiter(void *context, uint8_t delimiter, k4c_buf_cursor *out);

static const k4c_reader_vtable test_reader_vtable = {
    .take_byte = test_reader_take_byte,
    .take_delimiter = test_reader_take_delimiter,
};

static k4c_status test_reader_take_byte(void *context, uint8_t *out) {
    test_reader_context *reader = context;

    return k4c_buf_cursor_next(&reader->cursor, out);
}

static k4c_status test_reader_take_delimiter(
    void *context,
    uint8_t delimiter,
    k4c_buf_cursor *out
) {
    test_reader_context *reader = context;
    size_t start = reader->cursor.pos;

    while (!k4c_buf_cursor_at_end(&reader->cursor)) {
        uint8_t byte = 0;
        K4C_RETURN_IF_ERROR(k4c_buf_cursor_next(&reader->cursor, &byte));
        if (byte == delimiter) {
            *out = k4c_buf_cursor_create(reader->cursor.data + start, reader->cursor.pos - start);
            return K4C_STATUS_OK;
        }
    }

    if (reader->cursor.pos > start) {
        *out = k4c_buf_cursor_create(reader->cursor.data + start, reader->cursor.pos - start);
        return K4C_STATUS_OK;
    }
    return K4C_STATUS_EOF;
}

static int cursor_equal(k4c_buf_cursor *cursor, const char *text) {
    size_t len = strlen(text);
    if (k4c_test_equal(cursor->len, len) != 0) {
        return 1;
    }
    if (memcmp(cursor->data, text, len) != 0) {
        return 1;
    }
    return 0;
}

K4C_TEST(create_tracks_context_and_vtable) {
    test_reader_context context = {
        .cursor = k4c_buf_cursor_create_from_cstr("abc"),
    };
    k4c_reader reader = k4c_reader_create(&context, &test_reader_vtable);

    if (k4c_test_equal_ptr(reader.ctx, &context) != 0) {
        return 1;
    }
    if (k4c_test_equal_ptr(reader.vtable, &test_reader_vtable) != 0) {
        return 1;
    }
    return 0;
}

K4C_TEST(take_byte_dispatches_to_vtable) {
    test_reader_context context = {
        .cursor = k4c_buf_cursor_create_from_cstr("ab"),
    };
    k4c_reader reader = k4c_reader_create(&context, &test_reader_vtable);

    uint8_t byte = 0;
    if (k4c_test_status_ok(k4c_reader_take_byte(&reader, &byte)) != 0) {
        return 1;
    }
    if (k4c_test_equal(byte, 'a') != 0) {
        return 1;
    }
    if (k4c_test_status_ok(k4c_reader_take_byte(&reader, &byte)) != 0) {
        return 1;
    }
    if (k4c_test_equal(byte, 'b') != 0) {
        return 1;
    }
    if (k4c_test_equal(k4c_reader_take_byte(&reader, &byte), K4C_STATUS_EOF) != 0) {
        return 1;
    }
    return 0;
}

K4C_TEST(take_delimiter_dispatches_to_vtable) {
    test_reader_context context = {
        .cursor = k4c_buf_cursor_create_from_cstr("one\ntwo\nthree"),
    };
    k4c_reader reader = k4c_reader_create(&context, &test_reader_vtable);

    k4c_buf_cursor line;
    if (k4c_test_status_ok(k4c_reader_take_delimiter(&reader, '\n', &line)) != 0) {
        return 1;
    }
    if (cursor_equal(&line, "one\n") != 0) {
        return 1;
    }
    if (k4c_test_status_ok(k4c_reader_take_delimiter(&reader, '\n', &line)) != 0) {
        return 1;
    }
    if (cursor_equal(&line, "two\n") != 0) {
        return 1;
    }
    if (k4c_test_status_ok(k4c_reader_take_delimiter(&reader, '\n', &line)) != 0) {
        return 1;
    }
    if (cursor_equal(&line, "three") != 0) {
        return 1;
    }
    if (k4c_test_equal(k4c_reader_take_delimiter(&reader, '\n', &line), K4C_STATUS_EOF) != 0) {
        return 1;
    }
    return 0;
}

K4C_TEST_MAIN(
    K4C_TEST_CASE(create_tracks_context_and_vtable),
    K4C_TEST_CASE(take_byte_dispatches_to_vtable),
    K4C_TEST_CASE(take_delimiter_dispatches_to_vtable)
)
