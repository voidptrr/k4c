#ifndef CKIT_COMMON_PANIC_H
#define CKIT_COMMON_PANIC_H

_Noreturn void ckit_panic(const char *message);

#define CKIT_ASSERT(cond, message)                                                                 \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            ckit_panic(message);                                                                   \
        }                                                                                          \
    } while (0)

#endif
