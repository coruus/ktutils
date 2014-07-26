#ifndef KTUTILS_H
#define KTUTILS_H
#include "ktutils-config.h"

extern size_t kt_memcompare(const uint8_t* s1, const rsize_t s1max,
                            const uint8_t* s2, const rsize_t s2max,
                            size_t oplen);
extern size_t kt_memnonzero(const uint8_t* s1, rsize_t s1max, size_t oplen);
extern int kt_memcpy_s(void* dst, rsize_t dstlen, void* src, rsize_t srclen, rsize_t oplen);
extern int kt_memset_s(void* dst, rsize_t dstlen, int val, rsize_t oplen);
extern void kt_memclear(void* dst, size_t length);
extern void kt_memscribble(void* dst, size_t length);

extern int kt_toint(size_t zerr);
extern size_t kt_err_itoz(int err);
extern int kt_err_ztoi(size_t zerr);
extern size_t kt_rsizeor0(size_t size);

extern int kt_selectb(uint8_t* out, uint8_t in, uint8_t q[][2], uint8_t qlen, uint8_t onfail);

extern int randombytes(uint8_t* out, size_t len);
extern int urandombytes(uint8_t* out, size_t len);

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct mmapped_file {
  uint8_t* mem;
  size_t length;
  int fd;
  int prot;
} mmfile;
extern int mmfile_open(mmfile* mf, const char* filename, const int mode);
extern int mmfile_create(mmfile* mf, const char* filename, const size_t length);
extern int mmfile_close(mmfile* mf);

#define memscribble kt_memscribble
#define memclear kt_memclear
#endif  // KTUTILS_H
