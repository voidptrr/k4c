#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

static int test_queue_init_null_pointer(void) {
    enum queue_status status;

    status = queue_init(NULL, sizeof(int));
    if (status != QUEUE_ERR_NULL) {
        fprintf(stderr, "queue_init(NULL, ...) should return QUEUE_ERR_NULL\n");
        return 1;
    }

    return 0;
}

static int test_queue_init_valid_pointer(void) {
    enum queue_status status;
    struct queue q;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    if (q.buffer == NULL || q.size != 0 || q.elem_size != sizeof(int) || q.capacity != 16 ||
        q.head != 0 || q.tail != 0) {
        fprintf(stderr, "queue_init(&q, ...) returned invalid queue state\n");
        free(q.buffer);
        return 1;
    }

    free(q.buffer);
    return 0;
}

int main(void) {
    if (test_queue_init_null_pointer() != 0) {
        return 1;
    }

    if (test_queue_init_valid_pointer() != 0) {
        return 1;
    }

    return 0;
}
