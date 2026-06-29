# io.file_reader

## DESCRIPTION

The file reader module provides a fixed-buffer reader over an existing `FILE *`.
It is useful when a program wants to walk file contents incrementally without
loading the whole file into memory.

The reader does not own the file stream or the backing buffer. The caller opens
and closes the `FILE *`, provides the byte storage, and keeps both alive for as
long as the reader is used.

`k4c_file_reader_create` initializes the file reader and returns the generic
`k4c_reader` owned by it. Use `k4c_reader_take_byte` to consume one byte, or
`k4c_reader_take_delimiter` to consume through a delimiter.

This API is byte-oriented. Returned chunks are not NUL-terminated.

## TYPES

### k4c_file_reader

```c
typedef struct k4c_file_reader {
    k4c_reader reader;
    FILE *file;
    uint8_t *data;
    size_t len;
    size_t buffer_capacity;
    size_t buffer_pos;
    size_t buffer_len;
} k4c_file_reader;
```

- `reader`: generic reader view owned by this file reader.
- `file`: caller-owned `FILE *`.
- `data`: caller-owned byte buffer used as reader storage.
- `len`: number of bytes in the most recent returned chunk.
- `buffer_capacity`: maximum bytes held in `data` for the buffered window.
- `buffer_pos`: current unread offset inside `data`.
- `buffer_len`: number of buffered bytes currently held in `data`.

## FUNCTIONS

### k4c_file_reader_create

```c
k4c_status k4c_file_reader_create(
    k4c_file_reader *reader,
    FILE *file,
    uint8_t *data,
    size_t buffer_capacity,
    k4c_reader **out
);
```

- Parameters: `reader`, `file`, `data`, `buffer_capacity`, `out`
- Returns: `K4C_STATUS_OK` on success, or `K4C_STATUS_INVALID_ARGUMENT` when
  `file` is `NULL`, `data` is `NULL`, `buffer_capacity` is zero, or `out` is
  `NULL`.
- Writes: the generic reader owned by `reader` to `*out`.
- Notes: the reader does not take ownership of `file` or `data`.

### k4c_file_reader_close

```c
k4c_status k4c_file_reader_close(k4c_file_reader *reader);
```

- Parameters: `reader`
- Returns: `K4C_STATUS_OK`.
- Notes: resets the reader. It does not call `fclose` and does not release the
  caller-provided buffer.

## EXAMPLES

### Read bytes

```c
FILE *file = fopen("data.bin", "rb");
if (file == NULL) {
    /* handle error */
}

uint8_t buffer[4096];
k4c_file_reader file_reader;
k4c_reader *reader = NULL;
if (k4c_file_reader_create(&file_reader, file, buffer, sizeof(buffer), &reader)
    != K4C_STATUS_OK) {
    fclose(file);
    /* handle error */
}

uint8_t byte = 0;
k4c_status st = K4C_STATUS_OK;
while ((st = k4c_reader_take_byte(reader, &byte)) == K4C_STATUS_OK) {
    fwrite(&byte, 1, 1, stdout);
}

k4c_file_reader_close(&file_reader);
fclose(file);

if (st != K4C_STATUS_EOF) {
    /* handle read error */
}
```

### Read lines

```c
FILE *file = fopen("access.log", "rb");
if (file == NULL) {
    /* handle error */
}

uint8_t buffer[64 * 1024];
k4c_file_reader file_reader;
k4c_reader *reader = NULL;
if (k4c_file_reader_create(&file_reader, file, buffer, sizeof(buffer), &reader)
    != K4C_STATUS_OK) {
    fclose(file);
    /* handle error */
}

k4c_buf_cursor line;
k4c_status st = K4C_STATUS_OK;
while ((st = k4c_reader_take_delimiter(reader, '\n', &line)) == K4C_STATUS_OK) {
    fwrite(line.data, 1, line.len, stdout);
}

k4c_file_reader_close(&file_reader);
fclose(file);

if (st == K4C_STATUS_OVERFLOW) {
    /* a line was longer than the configured buffer */
} else if (st != K4C_STATUS_EOF) {
    /* handle read error */
}
```

Use `fwrite` or another length-aware API for output. `line.data` is not
NUL-terminated, so it must not be printed with `%s`.
