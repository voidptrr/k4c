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
#include <stdio.h>
#include <string.h>

#include "k4c/assert.h"
#include "k4c/buffer/cursor.h"
#include "k4c/error.h"
#include "k4c/io/file_reader.h"
#include "k4c/io/reader.h"

static void k4c_file_reader_reset(k4c_file_reader *reader) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_reset invalid arguments");

    reader->file = NULL;
    reader->data = NULL;
    reader->len = 0;
    reader->buffer_capacity = 0;
    reader->buffer_pos = 0;
    reader->buffer_len = 0;
}

static k4c_status k4c_file_reader_fill(k4c_file_reader *reader) {
    if (reader->buffer_pos > 0 && reader->buffer_pos < reader->buffer_len) {
        reader->buffer_len -= reader->buffer_pos;
        memmove(reader->data, reader->data + reader->buffer_pos, reader->buffer_len);
        reader->buffer_pos = 0;
    } else if (reader->buffer_pos >= reader->buffer_len) {
        reader->buffer_pos = 0;
        reader->buffer_len = 0;
    }

    size_t free_capacity = reader->buffer_capacity - reader->buffer_len;
    size_t read_len = fread(reader->data + reader->buffer_len, 1, free_capacity, reader->file);
    reader->buffer_len += read_len;
    if (read_len > 0) {
        return K4C_STATUS_OK;
    }
    if (ferror(reader->file)) {
        return K4C_STATUS_IO;
    }
    return K4C_STATUS_EOF;
}

static k4c_status k4c_file_reader_vtable_take_byte(void *context, uint8_t *out) {
    k4c_file_reader *reader = context;

    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_vtable_take_byte invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_file_reader_vtable_take_byte invalid arguments");
    K4C_ASSERT(reader->file != NULL, "fatal: k4c_file_reader_vtable_take_byte invalid arguments");
    K4C_ASSERT(reader->data != NULL, "fatal: k4c_file_reader_vtable_take_byte invalid arguments");
    K4C_ASSERT(
        reader->buffer_capacity > 0,
        "fatal: k4c_file_reader_vtable_take_byte invalid arguments"
    );

    reader->len = 0;
    if (reader->buffer_pos >= reader->buffer_len) {
        k4c_status st = k4c_file_reader_fill(reader);
        if (st != K4C_STATUS_OK) {
            return st;
        }
    }

    *out = reader->data[reader->buffer_pos];
    reader->buffer_pos += 1;
    reader->len = 1;
    return K4C_STATUS_OK;
}

static k4c_status k4c_file_reader_vtable_take_delimiter(
    void *context,
    uint8_t delimiter,
    k4c_buf_cursor *out
) {
    k4c_file_reader *reader = context;

    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_vtable_take_delimiter invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_file_reader_vtable_take_delimiter invalid arguments");
    K4C_ASSERT(
        reader->file != NULL,
        "fatal: k4c_file_reader_vtable_take_delimiter invalid arguments"
    );
    K4C_ASSERT(
        reader->data != NULL,
        "fatal: k4c_file_reader_vtable_take_delimiter invalid arguments"
    );
    K4C_ASSERT(
        reader->buffer_capacity > 0,
        "fatal: k4c_file_reader_vtable_take_delimiter invalid arguments"
    );

    reader->len = 0;

    while (true) {
        if (reader->buffer_pos >= reader->buffer_len) {
            k4c_status st = k4c_file_reader_fill(reader);
            if (st != K4C_STATUS_OK) {
                return st;
            }
        }

        uint8_t *start = reader->data + reader->buffer_pos;
        size_t available = reader->buffer_len - reader->buffer_pos;
        uint8_t *found = memchr(start, delimiter, available);
        if (found != NULL) {
            reader->len = (size_t)(found - start) + 1;
            reader->buffer_pos += reader->len;
            *out = k4c_buf_cursor_create(start, reader->len);
            return K4C_STATUS_OK;
        }

        if (available == reader->buffer_capacity) {
            reader->len = available;
            reader->buffer_pos = reader->buffer_len;
            return K4C_STATUS_OVERFLOW;
        }

        k4c_status st = k4c_file_reader_fill(reader);
        if (st == K4C_STATUS_EOF && reader->buffer_len > 0) {
            reader->len = reader->buffer_len;
            reader->buffer_pos = reader->buffer_len;
            *out = k4c_buf_cursor_create(reader->data, reader->len);
            return K4C_STATUS_OK;
        }
        if (st != K4C_STATUS_OK) {
            return st;
        }
    }
}

static const k4c_reader_vtable k4c_file_reader_vtable = {
    .take_byte = k4c_file_reader_vtable_take_byte,
    .take_delimiter = k4c_file_reader_vtable_take_delimiter,
};

k4c_status k4c_file_reader_init(
    k4c_file_reader *reader,
    FILE *file,
    uint8_t *data,
    size_t buffer_capacity
) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_init invalid arguments");

    k4c_file_reader_reset(reader);
    if (file == NULL || data == NULL || buffer_capacity == 0) {
        return K4C_STATUS_INVALID_ARGUMENT;
    }

    reader->file = file;
    reader->data = data;
    reader->len = 0;
    reader->buffer_capacity = buffer_capacity;
    reader->buffer_pos = 0;
    reader->buffer_len = 0;
    return K4C_STATUS_OK;
}

k4c_reader k4c_file_reader_as_reader(k4c_file_reader *reader) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_as_reader invalid arguments");

    return k4c_reader_create(reader, &k4c_file_reader_vtable);
}

k4c_status k4c_file_reader_close(k4c_file_reader *reader) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_file_reader_close invalid arguments");

    k4c_file_reader_reset(reader);
    return K4C_STATUS_OK;
}
