#include <stdint.h>

int kt_selectb(uint8_t* out, uint8_t in, uint8_t q[][2], uint8_t qlen, uint8_t onfail);
int kt_selectb(uint8_t* out, uint8_t in,
               uint8_t q[][2], uint8_t qlen,
               uint8_t onfail) {
  uint8_t match = 0;
  uint8_t o = onfail;
  for (int i = 0; i < qlen; i++) {
    uint8_t mask = ((uint8_t)(q[i][0] != in) - 1);
    o = (o & ~mask) | (mask & q[i][1]);
    match |= mask;
  }
  *out = o;
  return (int)(match == 0);
}

