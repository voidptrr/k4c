# error

## DESCRIPTION

The error module defines the shared k4c_status type used by fallible `k4c` APIs.
Invalid required arguments are still programmer errors and are asserted; expected
runtime failures, such as allocation failure, are reported with `k4c_status`.

## MACROS

### K4C_RETURN_IF_ERROR

```c
#define K4C_RETURN_IF_ERROR(expr)
```

- Parameters: `expr`
- Notes: evaluates a `k4c_status` expression and returns the k4c_status immediately
  when it is not `K4C_STATUS_OK`.

## TYPES

### k4c_status

```c
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
```

- `K4C_STATUS_OK`: operation succeeded.
- `K4C_STATUS_NO_MEMORY`: allocation failed.
- `K4C_STATUS_OVERFLOW`: size arithmetic overflowed before an operation could run.
- `K4C_STATUS_INVALID_DATA`: input data was malformed.
- `K4C_STATUS_IO`: an I/O operation failed.

## FUNCTIONS

### k4c_status_message

```c
const char *k4c_status_message(k4c_status k4c_status);
```

- Parameters: `k4c_status`
- Returns: static human-readable message for `k4c_status`.
