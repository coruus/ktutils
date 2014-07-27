#ifndef POSIX_MMAPFILE_H
#define POSIX_MMAPFILE_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*BEGIN EXPORT*/
typedef struct mmapped_file {
  uint8_t* mem;
  size_t length;
  int fd;
  int prot;
  int oflags;
} mmfile;

int mmfile_open(mmfile* mf, const char* filename, const int oflag);
int mmfile_create(mmfile* mf, const char* filename, const size_t length);
int mmfile_close(mmfile* mf);
/*END EXPORT*/

#endif  // POSIX_MMAPFILE_H
