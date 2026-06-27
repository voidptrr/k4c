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

#include "k4c/logging.h"

int main(void) {
    k4c_log_set_timestamp(LOG_TIMESTAMP_TIME);
    k4c_log_set_prefixes("test", "test-error");
    k4c_log_set_color(false);

    k4c_log_debug("debug %d", 1);
    k4c_log_info("info %d", 2);
    k4c_log_warn("warn %d", 3);
    k4c_log_error("error %d", 4);

    k4c_log_set_level(K4C_LOG_LEVEL_ERROR);
    k4c_log_set_timestamp(LOG_TIMESTAMP_UNIX);
    k4c_log_info("filtered");
    k4c_log_error("visible");

    k4c_log_set_timestamp(LOG_TIMESTAMP_DATETIME);
    k4c_log_error("configured logger");

    return 0;
}
