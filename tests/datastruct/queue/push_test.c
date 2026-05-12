#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

static int test_queue_push_appends_value(void) {
    enum queue_status status;
    struct queue q;
    int value = 42;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    status = queue_push(&q, &value);
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_push(&q, &value) should return QUEUE_OK\n");
        free(q.buffer);
        return 1;
    }

    if (q.size != 1) {
        fprintf(stderr, "queue_push should increment queue size\n");
        free(q.buffer);
        return 1;
    }

    if (q.head != 0 || q.tail != 1) {
        fprintf(stderr, "queue_push should update head/tail correctly\n");
        free(q.buffer);
        return 1;
    }

    if (((int *)q.buffer)[0] != value) {
        fprintf(stderr, "queue_push should copy pushed value into buffer\n");
        free(q.buffer);
        return 1;
    }

    free(q.buffer);
    return 0;
}

static int test_queue_push_null_pointers(void) {
    enum queue_status status;
    struct queue q;
    int value = 1;

    status = queue_push(NULL, &value);
    if (status != QUEUE_ERR_NULL) {
        fprintf(stderr, "queue_push(NULL, &value) should return QUEUE_ERR_NULL\n");
        return 1;
    }

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    status = queue_push(&q, NULL);
    if (status != QUEUE_ERR_NULL) {
        fprintf(stderr, "queue_push(&q, NULL) should return QUEUE_ERR_NULL\n");
        free(q.buffer);
        return 1;
    }

    free(q.buffer);
    return 0;
}

static int test_queue_push_wraparound_growth(void) {
    enum queue_status status;
    struct queue q;
    int pushed = 99;
    int popped = 0;
    int i;

    status = queue_init(&q, sizeof(int));
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_init(&q, ...) should return QUEUE_OK\n");
        return 1;
    }

    for (i = 0; i < 50; i++) {
        int value = i;
        status = queue_push(&q, &value);
        if (status != QUEUE_OK) {
            fprintf(stderr, "queue_push should fill queue up to capacity\n");
            queue_free(&q);
            return 1;
        }
    }

    for (i = 0; i < 20; i++) {
        status = queue_popleft(&q, &popped);
        if (status != QUEUE_OK || popped != i) {
            fprintf(stderr, "queue_popleft should dequeue initial FIFO values\n");
            queue_free(&q);
            return 1;
        }
    }

    for (i = 50; i < 70; i++) {
        int value = i;
        status = queue_push(&q, &value);
        if (status != QUEUE_OK) {
            fprintf(stderr, "queue_push should wrap and refill queue\n");
            queue_free(&q);
            return 1;
        }
    }

    status = queue_push(&q, &pushed);
    if (status != QUEUE_OK) {
        fprintf(stderr, "queue_push should succeed on wrapped full queue\n");
        queue_free(&q);
        return 1;
    }

    if (q.capacity != 100 || q.size != 51 || q.head != 0 || q.tail != 51) {
        fprintf(stderr, "queue_push should grow and normalize wrapped queue indices\n");
        queue_free(&q);
        return 1;
    }

    for (i = 20; i < 70; i++) {
        status = queue_popleft(&q, &popped);
        if (status != QUEUE_OK || popped != i) {
            fprintf(stderr, "queue should preserve FIFO order after wrapped growth\n");
            queue_free(&q);
            return 1;
        }
    }

    status = queue_popleft(&q, &popped);
    if (status != QUEUE_OK || popped != pushed) {
        fprintf(stderr, "queue should keep newly pushed element after growth\n");
        queue_free(&q);
        return 1;
    }

    queue_free(&q);
    return 0;
}

int main(void) {
    if (test_queue_push_appends_value() != 0) {
        return 1;
    }

    if (test_queue_push_null_pointers() != 0) {
        return 1;
    }

    if (test_queue_push_wraparound_growth() != 0) {
        return 1;
    }

    return 0;
}
