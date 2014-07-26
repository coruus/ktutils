#ifndef KTUTILS_CONFIG_H

#define KT_DEVRANDOM_FAILURE_ABORTS 0

/* Ugly hacks below. */

#define __STDC_WANT_LIB_EXT1__ 1
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(RSIZE_MAX)
#define KT_NEED_RSIZE_T
#endif  // !defined(RSIZ_MAX)

#ifdef KT_NEED_RSIZE_T
#undef RSIZE_MAX
#define RSIZE_MAX ((SIZE_MAX) << (size_t)1)
#define rsize_t size_t
#endif // KT_NEED_RSIZE_T

#endif  // KTUTILS_CONFIG_H
