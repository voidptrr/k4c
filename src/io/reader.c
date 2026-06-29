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

#include <stdint.h>

#include "k4c/assert.h"
#include "k4c/buffer/cursor.h"
#include "k4c/error.h"
#include "k4c/io/reader.h"

k4c_reader k4c_reader_create(void *context, const k4c_reader_vtable *vtable) {
    K4C_ASSERT(vtable != NULL, "fatal: k4c_reader_create invalid arguments");
    K4C_ASSERT(vtable->take_byte != NULL, "fatal: k4c_reader_create invalid arguments");
    K4C_ASSERT(vtable->take_delimiter != NULL, "fatal: k4c_reader_create invalid arguments");

    return (k4c_reader){
        .context = context,
        .vtable = vtable,
    };
}

k4c_status k4c_reader_take_byte(k4c_reader *reader, uint8_t *out) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_reader_take_byte invalid arguments");
    K4C_ASSERT(reader->vtable != NULL, "fatal: k4c_reader_take_byte invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_reader_take_byte invalid arguments");

    return reader->vtable->take_byte(reader->context, out);
}

k4c_status k4c_reader_take_delimiter(k4c_reader *reader, uint8_t delimiter, k4c_buf_cursor *out) {
    K4C_ASSERT(reader != NULL, "fatal: k4c_reader_take_delimiter invalid arguments");
    K4C_ASSERT(reader->vtable != NULL, "fatal: k4c_reader_take_delimiter invalid arguments");
    K4C_ASSERT(out != NULL, "fatal: k4c_reader_take_delimiter invalid arguments");

    return reader->vtable->take_delimiter(reader->context, delimiter, out);
}
