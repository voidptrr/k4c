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

#ifndef K4C_ERROR_H
#define K4C_ERROR_H

#define K4C_RETURN_IF_ERROR(expr) \
    do { \
        k4c_status status__ = (expr); \
        if (status__ != K4C_STATUS_OK) { \
            return status__; \
        } \
    } while (0)

typedef enum k4c_status {
    K4C_STATUS_OK = 0,
    K4C_STATUS_INVALID_ARGUMENT,
    K4C_STATUS_NO_MEMORY,
    K4C_STATUS_OVERFLOW,
    K4C_STATUS_OUT_OF_RANGE,
    K4C_STATUS_NOT_FOUND,
    K4C_STATUS_EOF,
    K4C_STATUS_IO,
    K4C_STATUS_INVALID_DATA,
    K4C_STATUS_UNSUPPORTED,
} k4c_status;

const char *k4c_status_message(k4c_status k4c_status);

#endif
