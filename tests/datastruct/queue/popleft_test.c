#include <stddef.h>
#include <stdio.h>

#include "queue.h"

static int test_queue_popleft_null_queue(void) {
    int out = 0;

    if (queue_popleft(NULL, &out) != QUEUE_ERR_NULL) {
        fprintf(stderr, "queue_popleft(NULL, &out) should return QUEUE_ERR_NULL\n");
        return 1;
    }

    return 0;
}

static int test_queue_popleft_null_out(void) {
    enum queue_status status;
    struct queue q;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    status = queue_popleft(&q, NULL);
    if (status != QUEUE_ERR_NULL) {
        fprintf(stderr, "queue_popleft(&q, NULL) should return QUEUE_ERR_NULL\n");
        queue_free(&q);
        return 1;
    }

    queue_free(&q);
    return 0;
}

static int test_queue_popleft_empty_queue(void) {
    enum queue_status status;
    struct queue q;
    int out = 0;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    status = queue_popleft(&q, &out);
    if (status != QUEUE_ERR_EMPTY) {
        fprintf(stderr, "queue_popleft on empty queue should return QUEUE_ERR_EMPTY\n");
        queue_free(&q);
        return 1;
    }

    queue_free(&q);
    return 0;
}

static int test_queue_popleft_fifo_order(void) {
    enum queue_status status;
    struct queue q;
    int values[] = {7, 11, 13};
    int out = 0;
    size_t i;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    for (i = 0; i < 3; i++) {
        status = queue_push(&q, &values[i]);
        if (status != QUEUE_OK) {
            fprintf(stderr, "queue_push should succeed before queue_popleft checks\n");
            queue_free(&q);
            return 1;
        }
    }

    for (i = 0; i < 3; i++) {
        status = queue_popleft(&q, &out);
        if (status != QUEUE_OK || out != values[i]) {
            fprintf(stderr, "queue_popleft should preserve FIFO order\n");
            queue_free(&q);
            return 1;
        }
    }

    if (queue_size(&q) != 0) {
        fprintf(stderr, "queue should be empty after popping all elements\n");
        queue_free(&q);
        return 1;
    }

    queue_free(&q);
    return 0;
}

int main(void) {
    if (test_queue_popleft_null_queue() != 0) {
        return 1;
    }

    if (test_queue_popleft_null_out() != 0) {
        return 1;
    }

    if (test_queue_popleft_empty_queue() != 0) {
        return 1;
    }

    if (test_queue_popleft_fifo_order() != 0) {
        return 1;
    }

    return 0;
}
