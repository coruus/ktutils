#include "config.h"

#include <stdint.h>
#include <stdlib.h>

/*@ assigns \nothing;
  @ ensures    ((zerr != 0) ==> (\result == (size_t)-1))
  @         && ((zerr == 0) ==> (\result == 0));
 */
int kt_toint(size_t zerr);
int kt_toint(size_t zerr) {
  return (zerr != 0) - 1;
}

int kt_err_ztoi(size_t zerr);
int kt_err_ztoi(size_t zerr) {
  return (int)(zerr != 0) - 1;
}

int kt_err_64toi(uint64_t u64err);
int kt_err_64toi(uint64_t u64err) {
  return (int)(u64err != 0) - 1;
}

size_t kt_err_itoz(int err);
size_t kt_err_itoz(int err) {
  return (size_t)(err != 0) - (size_t)1;
}

/*@ assigns \nothing;
  @ ensures \result <= RSIZE_MAX;
*/
size_t kt_rsizeor0(size_t size);
size_t kt_rsizeor0(size_t size) {
  return ((size_t)(size > RSIZE_MAX) - (size_t)1) & size;
}

#include <assert.h>
int test_kt_rsizeor0(void);
int test_kt_rsizeor0(void) {
  int err = 0;
  err += kt_rsizeor0(0) != 0;
  err += kt_rsizeor0(SIZE_MAX) != (size_t)0;
  err += kt_rsizeor0(RSIZE_MAX) != RSIZE_MAX;
  err += kt_rsizeor0(RSIZE_MAX + 1) != (size_t)0;
  err += kt_rsizeor0(1) != (size_t)1;
  err += kt_rsizeor0(0xabcdef) != (size_t)0xabcdef;
  assert(err == 0);
  return err;
}
