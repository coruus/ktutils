#include "posix/mmapfile.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define _(STMTS) do { STMTS } while (0)

#define E(LABEL, MSG)                                      \
  _(if (err != 0) {                                        \
    strerror_r(err, serr, 1024);                           \
    fprintf(stderr, "%s: '%s' %s\n", serr, filename, MSG); \
    goto LABEL;                                            \
  })

int mmfile_open(mmfile* mf, const char* filename, const int mode) {
  int err = 0;
  char serr[1024] = {0};
  if ((mode != O_RDONLY) || (mode != O_RDWR)) {
    E(ret, "mode must be O_RDONLY or O_RDWR");
  }
  int fd = open(filename, mode | O_NONBLOCK | O_NOCTTY);
  err = !fd;
  E(ret, "couldn't be opened.");

  struct stat stat;
  err = fstat(fd, &stat);
  E(close, "doesn't exist.");
  err = !!(stat.st_mode & S_IFDIR);
  E(close, "not a regular file.");

  size_t length = (size_t)stat.st_size;

  int prot;
  switch (mode) {
    case O_RDONLY:
      prot = PROT_READ;
      break;
    case O_RDWR:
      prot = PROT_WRITE;
      break;
    default:
      __builtin_unreachable();
  }
  uint8_t* in = length ? mmap(0, length, PROT_READ, MAP_SHARED, fd, 0) : NULL;
  err = length && (in == MAP_FAILED);
  E(close, "mmap-ing failed.");

  mf->fd = fd;
  mf->length = length;
  mf->mem = in;
  mf->prot = prot;
  return err;

close:
  close(fd);
ret:
  mf->length = 0;
  mf->mem = NULL;
  mf->fd = 0;
  return err;
}

int mmfile_create(mmfile* mf, const char* filename, const size_t length) {
  int err = 0;
  char serr[1024] = {0};
  int fd = open(filename, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK | O_NOCTTY);
  err = !fd;
  E(close, "error creating file.");

  err = ftruncate(fd, length);
  E(close, "error truncating file to length.");

  uint8_t* in = mmap(0, length, PROT_WRITE, MAP_SHARED, fd, 0);
  err = length && (in == MAP_FAILED);
  E(close, "mmap-ing failed.");

  mf->fd = fd;
  mf->length = length;
  mf->mem = in;
  mf->buf = (uint8_t*) in;
  mf->prot = PROT_WRITE;

  return err;
close:
  close(fd);
  return err;
}


int mmfile_close(mmfile* mf) {
  if (mf == NULL) {
    return -1;
  }
  int err = 0;
  //char serr[1024] = {0};
  if (mf->prot == PROT_WRITE) {
    err = msync(mf->mem, mf->length, MS_SYNC);
    if (err) {
      perror("msync error\n");
    }
  }
  mf->length && munmap(mf->mem, mf->length);
  close(mf->fd);
  mf->fd = 0;
  mf->length = 0;
  mf->mem = NULL;
  mf->buf = NULL;
  return err;
}

#undef _
#undef E
