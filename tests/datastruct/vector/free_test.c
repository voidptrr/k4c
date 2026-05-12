#include <stddef.h>
#include <stdio.h>

#include "vector.h"

static int test_vector_free_null_pointer(void) {
    enum vector_status status;

    status = vector_free(NULL);
    if (status != VECTOR_ERR_NULL) {
        fprintf(stderr, "vector_free(NULL) should return VECTOR_ERR_NULL\n");
        return 1;
    }

    return 0;
}

static int test_vector_free_null_buffer(void) {
    enum vector_status status;
    struct vector v = {0};

    status = vector_free(&v);
    if (status != VECTOR_ERR_NULL) {
        fprintf(stderr, "vector_free(&v) with NULL buffer should return VECTOR_ERR_NULL\n");
        return 1;
    }

    return 0;
}

static int test_vector_free_valid_vector(void) {
    enum vector_status status;
    struct vector v;

    status = vector_init(&v, sizeof(int));
    if (status != VECTOR_OK) {
        fprintf(stderr, "vector_init(&v, ...) should return VECTOR_OK\n");
        return 1;
    }

    status = vector_free(&v);
    if (status != VECTOR_OK) {
        fprintf(stderr, "vector_free(&v) should return VECTOR_OK\n");
        return 1;
    }

    return 0;
}

int main(void) {
    if (test_vector_free_null_pointer() != 0) {
        return 1;
    }

    if (test_vector_free_null_buffer() != 0) {
        return 1;
    }

    if (test_vector_free_valid_vector() != 0) {
        return 1;
    }

    return 0;
}
