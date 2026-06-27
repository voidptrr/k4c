# k4c.assert

## DESCRIPTION

The assert module provides fail-fast runtime assertions.

## RUNTIME ASSERTIONS

### K4C_ASSERT

```c
#include <k4c/assert.h>

#define K4C_ASSERT(cond, message)
```

- Parameters: `cond`, `message`
- Behavior: calls `k4c_panic(message)` when `cond` is false.

### k4c_panic

```c
#include <k4c/assert.h>

_Noreturn void k4c_panic(const char *message);
```

- Parameters: `message`
- Behavior: prints `message` when it is not `NULL`, then aborts.
