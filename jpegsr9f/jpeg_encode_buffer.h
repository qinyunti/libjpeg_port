#ifndef JPEG_ENCODE_H
#define JPEG_ENCODE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

size_t jpeg_encode(void *file_buffer_in, void *file_buffer_out, uint32_t width, uint32_t height, uint32_t image_quality, uint8_t * buff);

#ifdef __cplusplus
}
#endif

#endif