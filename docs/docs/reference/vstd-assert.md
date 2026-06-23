# vstd.assert

## DESCRIPTION

The assert module provides fail-fast runtime assertions.

## RUNTIME ASSERTIONS

### VSTD_ASSERT

```c
#include <vstd/assert.h>

#define VSTD_ASSERT(cond, message)
```

- Parameters: `cond`, `message`
- Behavior: calls `vs_panic(message)` when `cond` is false.

### vs_panic

```c
#include <vstd/assert.h>

_Noreturn void vs_panic(const char *message);
```

- Parameters: `message`
- Behavior: prints `message` when it is not `NULL`, then aborts.
