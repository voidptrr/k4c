#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vector.h"

#define DEFAULT_CAPACITY 16

enum vector_status vector_init(struct vector *vector, size_t elem_size) {
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    void *buffer = malloc(elem_size * DEFAULT_CAPACITY);
    if (buffer == NULL) {
        return VECTOR_ERR_OOM;
    }

    vector->buffer = buffer;
    vector->size = 0;
    vector->elem_size = elem_size;
    vector->capacity = DEFAULT_CAPACITY;

    return VECTOR_OK;
}

enum vector_status vector_push(struct vector *vector, const void *element) {
    if (vector == NULL || element == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector->size == vector->capacity) {
        size_t new_capacity = vector->capacity *= 2;
        void *tmp = realloc(vector->buffer, new_capacity * vector->elem_size);

        if (tmp == NULL) {
            return VECTOR_ERR_OOM;
        }

        vector->buffer = tmp;
        vector->capacity = new_capacity;
    }

    uint8_t *base = (uint8_t *)vector->buffer;
    void *dst = base + (vector->size * vector->elem_size);
    memcpy(dst, element, vector->elem_size);
    vector->size += 1;

    return VECTOR_OK;
}

enum vector_status vector_pop(struct vector *vector, void *out) {
    if (vector == NULL || out == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector->size == 0) {
        return VECTOR_ERR_EMPTY;
    }

    vector->size -= 1;

    uint8_t *base = (uint8_t *)vector->buffer;
    void *src = base + (vector->size * vector->elem_size);
    memcpy(out, src, vector->elem_size);

    return VECTOR_OK;
}

enum vector_status vector_free(struct vector *vector) {
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    free(vector->buffer);
    vector->buffer = NULL;
    vector->size = 0;
    vector->capacity = 0;

    return VECTOR_OK;
}

size_t vector_size(struct vector *vector) {
    if (vector == NULL) {
        return 0;
    }

    return vector->size;
}
