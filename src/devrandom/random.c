#include "devrandom/config.h"
#include "devrandom/random.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char* devrandom = "/dev/random";
static const char* devurandom = "/dev/urandom";

static inline errno_t _randombytes(uint8_t* out, size_t len, const char* device) {
  FILE* f = fopen(device, "r");
  if (f == NULL) {
    fprintf(stderr, "couldn't open %s, device\n", device);
    KT_DEVRANDOM_FAILED();
  }
  size_t read = fread(out, 1, len, f);
  if (read != len) {
    fprintf(stderr, "%s didn't yield enough bytes (%zu of %zu)\n", device, read, len);
    KT_DEVRANDOM_FAILED();
  }
  if (fclose(f) != 0) {
    perror("error closing random device\n");
  }
  return 0;
}

errno_t randombytes(uint8_t* out, size_t len) {
  return _randombytes(out, len, devrandom);
}

errno_t urandombytes(uint8_t* out, size_t len) {
  return _randombytes(out, len, devurandom);
}
