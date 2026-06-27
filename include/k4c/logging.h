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

#ifndef K4C_LOGGING_H
#define K4C_LOGGING_H

#include <stdbool.h>

/* Severity levels used by the global logger. */
typedef enum k4c_log_level {
    K4C_LOG_LEVEL_DEBUG = 0,
    K4C_LOG_LEVEL_INFO = 1,
    K4C_LOG_LEVEL_WARN = 2,
    K4C_LOG_LEVEL_ERROR = 3,
} k4c_log_level;

/* Timestamp formats available for log messages. */
typedef enum k4c_log_timestamp {
    K4C_LOG_TIMESTAMP_NONE = 0,
    LOG_TIMESTAMP_TIME = 1,
    LOG_TIMESTAMP_DATETIME = 2,
    LOG_TIMESTAMP_UNIX = 3,
} k4c_log_timestamp;

/* Setter functions for the global logger configuration. */
void k4c_log_set_level(k4c_log_level level);
void k4c_log_set_timestamp(k4c_log_timestamp timestamp);
void k4c_log_set_prefixes(const char *message_prefix, const char *error_prefix);
void k4c_log_set_color(bool enabled);

/* Log a DEBUG-level formatted message. */
void k4c_log_debug(const char *fmt, ...);

/* Log an INFO-level formatted message. */
void k4c_log_info(const char *fmt, ...);

/* Log a WARN-level formatted message. */
void k4c_log_warn(const char *fmt, ...);

/* Log an ERROR-level formatted message. */
void k4c_log_error(const char *fmt, ...);

#endif
