#ifndef CSTD_MEM_BYTES_H
#define CSTD_MEM_BYTES_H

#include <stddef.h>

/* Swap size bytes between memory regions a and b. */
void cstd_memswap(void *a, void *b, size_t size);

#endif
