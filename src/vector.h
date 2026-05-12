#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

enum vector_status { VECTOR_OK, VECTOR_ERR_OOM, VECTOR_ERR_NULL };

struct vector {
    void *buffer;
    size_t size;
    size_t elem_size;
    size_t capacity;
};

enum vector_status vector_init(struct vector *vector, size_t elem_size);
enum vector_status vector_push(struct vector *vector, const void *element);
enum vector_status vector_free(const struct vector *vector);

size_t vector_size(struct vector *vector);

#endif
