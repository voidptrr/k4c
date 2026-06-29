# io.reader

## DESCRIPTION

The reader module defines a small generic byte-reader interface. It does not own
the underlying source. Concrete readers provide a context pointer and vtable
used by the generic dispatch helpers.

This API is byte-oriented. Returned delimiter chunks are not NUL-terminated.

## TYPES

### k4c_reader

```c
typedef struct k4c_reader {
    void *ctx;
    const k4c_reader_vtable *vtable;
} k4c_reader;
```

- `ctx`: caller-owned concrete reader state.
- `vtable`: operation table used by generic reader helpers.

### k4c_reader_vtable

```c
typedef struct k4c_reader_vtable {
    k4c_reader_take_byte_fn take_byte;
    k4c_reader_take_delimiter_fn take_delimiter;
} k4c_reader_vtable;
```

- `take_byte`: implementation function for consuming one byte.
- `take_delimiter`: implementation function for consuming through a delimiter.

## FUNCTIONS

### k4c_reader_create

```c
k4c_reader k4c_reader_create(void *ctx, const k4c_reader_vtable *vtable);
```

- Parameters: `ctx`, `vtable`
- Returns: a generic reader value.
- Notes: the reader does not take ownership of `ctx`. `vtable`,
  `vtable->take_byte`, and `vtable->take_delimiter` must not be `NULL`.

### k4c_reader_take_byte

```c
k4c_status k4c_reader_take_byte(k4c_reader *reader, uint8_t *out);
```

- Parameters: `reader`, `out`
- Returns: `K4C_STATUS_OK` when one byte was consumed, `K4C_STATUS_EOF` when no
  byte is available, or another status from the concrete reader.
- Writes: the consumed byte to `*out` on success.

### k4c_reader_take_delimiter

```c
k4c_status k4c_reader_take_delimiter(
    k4c_reader *reader,
    uint8_t delimiter,
    k4c_buf_cursor *out
);
```

- Parameters: `reader`, `delimiter`, `out`
- Returns: `K4C_STATUS_OK` when bytes were consumed, `K4C_STATUS_EOF` when no
  bytes are available, or another status from the concrete reader.
- Writes: a cursor over the consumed bytes to `*out` on success.
- Notes: the generic wrapper dispatches to the concrete reader. The concrete
  reader defines whether final unterminated data is returned as a last chunk.

## EXAMPLES

### Implement a memory reader

Concrete readers keep their own state and expose it through a `k4c_reader`
value. The vtable functions consume from that concrete state.

```c
#include <stddef.h>
#include <stdint.h>

#include <k4c/buffer/cursor.h>
#include <k4c/error.h>
#include <k4c/io/reader.h>

typedef struct memory_reader {
    k4c_buf_cursor cursor;
} memory_reader;

static k4c_status memory_take_byte(void *context, uint8_t *out);
static k4c_status memory_take_delimiter(
    void *context,
    uint8_t delimiter,
    k4c_buf_cursor *out
);

static const k4c_reader_vtable memory_reader_vtable = {
    .take_byte = memory_take_byte,
    .take_delimiter = memory_take_delimiter,
};

static k4c_status memory_take_byte(void *context, uint8_t *out) {
    memory_reader *reader = context;

    return k4c_buf_cursor_next(&reader->cursor, out);
}

static k4c_status memory_take_delimiter(
    void *context,
    uint8_t delimiter,
    k4c_buf_cursor *out
) {
    memory_reader *reader = context;
    size_t start = reader->cursor.pos;

    while (!k4c_buf_cursor_at_end(&reader->cursor)) {
        uint8_t byte = 0;
        K4C_RETURN_IF_ERROR(k4c_buf_cursor_next(&reader->cursor, &byte));
        if (byte == delimiter) {
            *out = k4c_buf_cursor_create(
                reader->cursor.data + start,
                reader->cursor.pos - start
            );
            return K4C_STATUS_OK;
        }
    }

    if (reader->cursor.pos > start) {
        *out = k4c_buf_cursor_create(reader->cursor.data + start, reader->cursor.pos - start);
        return K4C_STATUS_OK;
    }
    return K4C_STATUS_EOF;
}

int main(void) {
    memory_reader memory = {
        .cursor = k4c_buf_cursor_create_from_cstr("first\nsecond"),
    };
    k4c_reader reader = k4c_reader_create(&memory, &memory_reader_vtable);

    k4c_buf_cursor line;
    while (k4c_reader_take_delimiter(&reader, '\n', &line) == K4C_STATUS_OK) {
        /* line.data points into memory.cursor.data and has line.len bytes */
    }
    return 0;
}
```

### Consume any reader

Functions can accept `k4c_reader *` when they only care about the byte stream,
not whether it comes from memory, a file reader, or another source.

```c
#include <stddef.h>
#include <stdint.h>

#include <k4c/error.h>
#include <k4c/io/reader.h>

k4c_status count_bytes(k4c_reader *reader, size_t *out) {
    uint8_t byte = 0;
    size_t count = 0;

    k4c_status st = K4C_STATUS_OK;
    while ((st = k4c_reader_take_byte(reader, &byte)) == K4C_STATUS_OK) {
        count += 1;
    }

    if (st != K4C_STATUS_EOF) {
        return st;
    }

    *out = count;
    return K4C_STATUS_OK;
}
```
