# io.reader

## DESCRIPTION

The reader module defines a small generic byte-reader interface. It does not own
the underlying source. Concrete readers provide a context pointer and a complete
vtable for the reader operations.

This API is byte-oriented. Returned delimiter chunks are not NUL-terminated.
`k4c_file_reader_as_reader` creates a generic reader over an existing file
reader.

## TYPES

### k4c_reader_vtable

```c
typedef struct k4c_reader_vtable {
    k4c_reader_take_byte_fn take_byte;
    k4c_reader_take_delimiter_fn take_delimiter;
} k4c_reader_vtable;
```

- `take_byte`: implementation function for consuming one byte.
- `take_delimiter`: implementation function for consuming through a delimiter.

### k4c_reader

```c
typedef struct k4c_reader {
    void *context;
    const k4c_reader_vtable *vtable;
} k4c_reader;
```

- `context`: caller-owned concrete reader state.
- `vtable`: operation table used by the generic dispatch functions.

## FUNCTIONS

### k4c_reader_create

```c
k4c_reader k4c_reader_create(void *context, const k4c_reader_vtable *vtable);
```

- Parameters: `context`, `vtable`
- Returns: a generic reader value.
- Notes: the reader does not take ownership of `context`. `vtable`,
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
