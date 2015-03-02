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

// Some common oflags to ensure sane behavior.
#define OFLAGS_COMMON \
  (O_NONBLOCK | O_NOCTTY | O_NOFOLLOW | O_CLOEXEC)

/** Open an mmfile.
 *
 * If oflag & O_RDRW, the memory is mmaped PROT_WRITE.
 * Otherewise, the memory if mmaped PROT_READ.
 *
 * It is an error if oflag & O_CREAT or oflag & O_TRUNC.
 * (Zero-length files cannot be mmaped. Please use
 * mmfile_create for this.)
 *
 * It is not, however, an error to attempt to mmfile_open
 * a file which is zero bytes; this results in a file
 * descriptor being opened, but no memory region being
 * mapped. mf->mem will be NULL.
 *
 * @param mf        [out] A pointer to the mmfile.
 * @param filename  [in]  The filename. Must not be a symlink.
 * @param oflag     [in]  O_RDONLY | O_RDRW
 *
 * Returns 0 on success. Other return values indicate an error;
 * their meaning is unspecified.
 */
int mmfile_open(mmfile* mf, const char* filename, const int oflag) {
  int err = 0;
  char serr[1024] = {0};
  if ((oflag & O_CREAT) | (oflag & O_TRUNC)) {
    E(ret, "oflag has O_CREAT or O_TRUNC set");
  }
  int oflags = oflag | OFLAGS_COMMON;
  int fd = open(filename, oflags);
  err = !fd;
  E(ret, "couldn't be opened.");

  // Stat the file descriptor, not the filename.
  // (This avoids most dangerous race conditions.)
  struct stat stat;
  err = fstat(fd, &stat);
  E(close, "couldn't be statted.");
  // Check that the file descriptor points to either
  // a block device or a regular file.
  switch (stat.st_mode & S_IFMT) {
    case S_IFBLK:
    case S_IFREG:
      break;
    default:
      err = -1;
      E(close, "not a regular file.");
      __builtin_unreachable();
   }

  size_t length = (size_t)stat.st_size;

  int prot = PROT_READ;
  if (oflag & O_RDWR) { prot = PROT_WRITE; }

  // We only attempt to mmap the memory region if the length is non-zero.
  //                     mmap(addr, len,    prot, flags,      fd, offset)
  uint8_t* in = length ? mmap(0,    length, prot, MAP_SHARED, fd, 0) : NULL;
  err = length && (in == MAP_FAILED);
  E(close, "mmap-ing failed.");

  // On the assumption that the file will be used soon,
  // notify the system that it should begin paging.
  // Errors are ignored. Justification:
  //   - The file has been opened and mmaped succesfully
  //   - The only reason the posix_madvise call can (legitimately)
  //     fail at this point are
  //     - The system doesn't fully implement posix_madvise, and
  //       therefore POSIX_MADV_WILLNEED is unsupported
  //     - There is a bug in the system's implementation of
  //       posix_madvise.
  // So the mmfile can be used safely, regardless of whether
  // an error has occurred.
  length && posix_madvise(in, length, POSIX_MADV_WILLNEED);

  mf->fd = fd;
  mf->length = length;
  mf->mem = in;
  mf->prot = prot;
  mf->oflags = oflags;
  return err;

close:
  close(fd);
ret:
  mf->length = 0;
  mf->mem = NULL;
  mf->fd = 0;
  return err;
}


/** Create a new mmfile at 'filename' and ftruncate it length,
 *  synchronously.
 *
 *  It is an error if the file already exists.
 */
int mmfile_create(mmfile* mf, const char* filename, const size_t length) {
  int err = 0;
  char serr[1024] = {0};
  int oflags = O_RDWR | O_CREAT | O_EXCL | OFLAGS_COMMON;
  int fd = open(filename, oflags);
  err = !fd;
  E(close, "error creating file.");

  err = ftruncate(fd, length);
  E(close, "error truncating file to length.");

  uint8_t* in = mmap(0, length, PROT_WRITE, MAP_SHARED, fd, 0);
  err = length && (in == MAP_FAILED);
  E(close, "mmap-ing failed.");

  mf->fd = fd;
  mf->length = length;
  mf->mem = (uint8_t*)in;
  mf->prot = PROT_WRITE;
  mf->oflags = oflags;

  return err;
close:
  close(fd);
  return err;
}

/** Close a mmmaped file.
 *
 * Attempts to fchmod the file to mf->mode if mf->mode is
 * not zero.
 *
 * @param mf [in,out] The mmfile to close.
 */
int mmfile_close(mmfile* mf) {
  if (mf == NULL) {
    return -1;
  }
  int err = 0;
  //char serr[1024] = {0};
  //
  if (mf->prot & PROT_WRITE) {
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
  return err;
}

#undef _
#undef E
