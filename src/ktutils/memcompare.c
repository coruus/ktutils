#include "ktutils.h"
#include "ktutils/rsize-private.h"

#include <stdint.h>
#include <stdlib.h>

size_t kt_memcompare(const uint8_t* s1, const rsize_t s1max,
                     const uint8_t* s2, const rsize_t s2max,
                     rsize_t oplen) {
  check2operand(s1, s2);
  size_t diffs = 0;
  while (oplen) {
    diffs += (*s1 != *s2);
    oplen--;
    s1++;
    s2++;
  }
  return diffs;

rtc_violation:
  return SIZE_MAX;
}

rsize_t kt_memnonzero(const uint8_t* s1, rsize_t s1max,
                      rsize_t oplen) {
  check1operand(s1);
  size_t diffs = 0;
  while (oplen) {
    diffs += (*s1 != 0);
    oplen--;
    s1++;
  }
  return diffs;

rtc_violation:
  return SIZE_MAX;
}
