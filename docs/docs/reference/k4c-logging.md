# k4c.logging

## DESCRIPTION

The logging module provides a small printf-backed global logger with severity levels,
optional ANSI colors, optional C-library timestamps, and configurable prefixes.
Messages include a bracketed level marker such as `[INFO]`, `[WARN]`, `[ERROR]`,
or `[DEBUG]`.

## TYPES

### k4c_log_level

```c
typedef enum k4c_log_level {
    K4C_LOG_LEVEL_DEBUG = 0,
    K4C_LOG_LEVEL_INFO = 1,
    K4C_LOG_LEVEL_WARN = 2,
    K4C_LOG_LEVEL_ERROR = 3,
} k4c_log_level;
```

Levels are ordered by severity and are filtered through the global minimum level.

### k4c_log_timestamp

```c
typedef enum k4c_log_timestamp {
    K4C_LOG_TIMESTAMP_NONE = 0,
    K4C_LOG_TIMESTAMP_TIME = 1,
    K4C_LOG_TIMESTAMP_DATETIME = 2,
    K4C_LOG_TIMESTAMP_UNIX = 3,
} k4c_log_timestamp;
```

Timestamp values use the C library `time`, `localtime`, and `strftime` APIs.

## FUNCTIONS

### k4c_log_set_level

```c
void k4c_log_set_level(k4c_log_level level);
```

Sets the minimum level to print. Messages below this level are ignored.

### k4c_log_set_timestamp

```c
void k4c_log_set_timestamp(k4c_log_timestamp timestamp);
```

Sets the timestamp format used before each message.

### k4c_log_set_prefixes

```c
void k4c_log_set_prefixes(const char *message_prefix, const char *error_prefix);
```

Sets the normal and error prefixes. `message_prefix` is used for debug, info,
and warn messages. `error_prefix` is used for error messages. Set either prefix
to `NULL` or an empty k4c_string to omit it.

### k4c_log_set_color

```c
void k4c_log_set_color(bool enabled);
```

Enables or disables ANSI color output.

### k4c_log_debug

```c
void k4c_log_debug(const char *fmt, ...);
```

Logs a debug message.

### k4c_log_info

```c
void k4c_log_info(const char *fmt, ...);
```

Logs an info message.

### k4c_log_warn

```c
void k4c_log_warn(const char *fmt, ...);
```

Logs a warning message.

### k4c_log_error

```c
void k4c_log_error(const char *fmt, ...);
```

Logs an error message.

## EXAMPLE

```c
#include <k4c/logging.h>

int main(void) {
    k4c_log_set_timestamp(K4C_LOG_TIMESTAMP_DATETIME);
    k4c_log_set_prefixes("app", "app-error");

    k4c_log_info("started");
    k4c_log_warn("using global logger");
    return 0;
}
```
