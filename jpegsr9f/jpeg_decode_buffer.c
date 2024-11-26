#include <string.h>
#include "jpeglib.h"
#include  "jioport.h"
#include "jpeg_decode_buffer.h"

/* This struct contains the JPEG decompression parameters */
struct jpeg_decompress_struct cinfo;
/* This struct represents a JPEG error handler */
struct jpeg_error_mgr jerr;

static size_t s_read_index = 0;
static size_t s_write_index = 0;

static size_t jioport_read(void * __ptr, size_t __size, size_t __n, void * __stream);
static size_t jioport_write(const void * __ptr, size_t __size, size_t __n, void * __stream);
static int    jioport_flush(void * __stream);
static int    jioport_error(void * __fp);

static jioport_st jioport =
{
  .read = jioport_read,
  .write = jioport_write,
  .flush = jioport_flush,
  .error = jioport_error,
};

static size_t jioport_read(void * __ptr, size_t __size, size_t __n, void * __stream)
{
  size_t tordlen;
  if(s_read_index >= jioport.infile_len)
  {
    return 0;
  }
  else
  {
    if((jioport.infile_len - s_read_index) >= __size * __n)
    {
      tordlen = __size * __n;
    }
    else
    {
      tordlen = jioport.infile_len - s_read_index;
    }
  }
  memcpy(__ptr, (const void*)((uint8_t*)__stream + s_read_index), __size * __n);
  s_read_index += __size * __n;
  return tordlen;
}

static size_t jioport_write(const void * __ptr, size_t __size, size_t __n, void * __stream)
{
  memcpy((void*)((uint8_t*)__stream + s_write_index), (const void*)__ptr, __size * __n);
  s_write_index += __size * __n;
  return __size * __n;
}

static int    jioport_flush(void * __stream)
{
  (void)__stream;
  return 0;
}

static int    jioport_error(void * __fp)
{
  (void)__fp;
  return 0;
}

/**
  * @brief  Jpeg Decode
  * @param  file_buffer_out: pointer to the jpg file
  * @param  width:    image width
  * @param  buff:     pointer to the image line    
  * @retval None
  */
size_t jpeg_decode(void *file_buffer_in, void *file_buffer_out, uint32_t width, uint8_t * buff, size_t infilelen)
{ 
  s_read_index = 0;
  s_write_index = 0;

  jioport.infile_len = infilelen;
  jioport_set(&jioport);  
  
  size_t len = 0;
  /* Decode JPEG Image */
  JSAMPROW buffer[2] = {0}; /* Output row buffer */
  
  buffer[0] = buff;

  /* Step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);

  /* Initialize the JPEG decompression object */  
  jpeg_create_decompress(&cinfo);
  
  jpeg_stdio_src (&cinfo, file_buffer_in);

  /* Step 3: read image parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);

  /* Step 4: set parameters for decompression */
  cinfo.dct_method = JDCT_FLOAT;

  /* Step 5: start decompressor */
  jpeg_start_decompress(&cinfo);

  while (cinfo.output_scanline < cinfo.output_height)
  {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    #if JPEG_LOG
    jprintf("jpeg_decode dst:%p size:%d src:%p\r\n",(uint8_t*)file_buffer_out+len, width * 3,buffer[0]);
    #endif
    memcpy((uint8_t*)file_buffer_out+len, buffer[0], width * 3);
    len += width * 3;
  }

  /* Step 6: Finish decompression */
  jpeg_finish_decompress(&cinfo);

  /* Step 7: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);
    
  return len;
}