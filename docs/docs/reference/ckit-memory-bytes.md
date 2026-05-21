# memory.bytes

## DESCRIPTION

The bytes module provides low-level byte-region helpers.

## FUNCTIONS

### ckit_memswap

```c
void ckit_memswap(void *a, void *b, size_t size);
```

- Parameters: `a`, `b`, `size`
- Behavior: swaps `size` bytes between regions `a` and `b`.
