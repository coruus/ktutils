#ifndef UTIL_DEVRANDOM_RANDOM_H
#define UTIL_DEVRANDOM_RANDOM_H
#include "devrandom/config.h"

#include <stdint.h>
#include <stdlib.h>
errno_t randombytes(uint8_t* out, size_t len);
errno_t urandombytes(uint8_t* out, size_t len);
int randombytes(uint8_t* out, size_t len);
#endif  // UTIL_DEVRANDOM_RANDOM_H
