#ifndef JPEG_DECODE_H
#define JPEG_DECODE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

size_t jpeg_decode(void *file_buffer_in, void *file_buffer_out, uint32_t width, uint8_t * buff, size_t infilelen);

#ifdef __cplusplus
}
#endif

#endif