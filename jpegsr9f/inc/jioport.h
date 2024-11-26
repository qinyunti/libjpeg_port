#ifndef JPEG_IOPORT_H
#define JPEG_IOPORT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct
{
  size_t (*read)(void * __ptr, size_t __size, size_t __n, void * __stream);
  size_t (*write)(const void * __ptr, size_t __size, size_t __n, void * __stream);
  int    (*flush)(void * __stream);
  int    (*error)(void * __fp);
  size_t infile_len;
} jioport_st;

size_t jfread(void * __ptr, size_t __size, size_t __n, void * __stream);
size_t jfwrite(const void * __ptr, size_t __size, size_t __n, void * __stream);
int    jfflush(void * __stream);
int    jferror(void * __fp);

void jioport_set(jioport_st* port);

#ifdef __cplusplus
}
#endif

#endif