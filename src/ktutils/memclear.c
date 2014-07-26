#include "ktutils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void kt_memclear(void* dst, size_t length) {
  __builtin_memset(dst, 0, length);
}

void kt_memscribble(void* dst, size_t length) {
  __builtin_memset(dst, 0xce, length);
}

errno_t kt_memset_s(void* dst, rsize_t dstlen, int val, rsize_t oplen) {
  int err = 0;
  if ((dst == NULL) || (dstlen > RSIZE_MAX)) {
    return -1;
  } else if (oplen > dstlen) {
    oplen = dstlen;
    val = 0;
    err = -1;
  }
  __builtin_memset(dst, val, oplen);
  return err;
}

errno_t kt_memcpy_s(void* dst, rsize_t dstlen,
                    void* src, rsize_t srclen,
                    rsize_t oplen) {
  if ((dst == NULL) || (dstlen > RSIZE_MAX)) {
    return -1;
  } else if ((oplen > dstlen) || (oplen > srclen)
             || (src == NULL) || (srclen > RSIZE_MAX)) {
    __builtin_memset(dst, 0, dstlen);
    return -1;
  }
  //@assert oplen < RSIZE_MAX;
  __builtin_memcpy(dst, src, oplen);
  return 0;
}
