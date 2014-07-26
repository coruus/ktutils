#ifndef DEVRANDOM_CONFIG_H
#define DEVRANDOM_CONFIG_H
#include "ktutils-config.h"

#if KT_DEVRANDOM_FAILURE_ABORTS == 1
#define KT_DEVRANDOM_FAILED() do { abort(); } while (0)
#else // DEV_RANDOM_FAILURE_ABORTS != 1
#define KT_DEVRANDOM_FAILED() do { return -1; } while (0)
#endif // DEVRANDOM_FAILURE_ABORTS
#endif  // DEVRANDOM_CONFIG_H
