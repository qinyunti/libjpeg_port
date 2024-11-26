#include <string.h>
#include "jpeglib.h"
#include  "jioport.h"
#include "jpeg_encode_buffer.h"

/* This struct contains the JPEG compression parameters */
static struct jpeg_compress_struct cinfo; 
/* This struct represents a JPEG error handler */
static struct jpeg_error_mgr jerr; 

static size_t s_read_index = 0;
static size_t s_write_index = 0;

static size_t jioport_read(void * __ptr, size_t __size, size_t __n, void * __stream)
{
  memcpy(__ptr, (const void*)((uint8_t*)__stream + s_read_index), __size * __n);
  s_read_index += __size * __n;
  return __size * __n;
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

static jioport_st jioport =
{
  .read = jioport_read,
  .write = jioport_write,
  .flush = jioport_flush,
  .error = jioport_error,
};

/**
  * @brief  Jpeg Encode
  * @param  file_buffer_in:        pointer to the bmp file(must be rgb888) or input buffer
  * @param  file_buffer_out:       pointer to the jpg file or output buffer
  * @param  width:         image width
  * @param  height:        image height
  * @param  image_quality: image quality
  * @param  buff:          pointer to the image line
  * @retval None
  */
size_t jpeg_encode(void *file_buffer_in, void *file_buffer_out, uint32_t width, uint32_t height, uint32_t image_quality, uint8_t * buff)
{ 
  s_read_index = 0;
  s_write_index = 0;

  jioport_set(&jioport);  

  /* Encode BMP Image to JPEG */  
  JSAMPROW row_pointer;    /* Pointer to a single row */
  uint32_t index;
  
  /* Step 1: allocate and initialize JPEG compression object */
  /* Set up the error handler */
  cinfo.err = jpeg_std_error(&jerr);
  
  /* Initialize the JPEG compression object */  
  jpeg_create_compress(&cinfo);
  
  /* Step 2: specify data destination */
  jpeg_stdio_dest(&cinfo, file_buffer_out);
  
  /* Step 3: set parameters for compression */
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  cinfo.num_components = 2;
  cinfo.jpeg_color_space = JCS_YCbCr;
  /* Set default compression parameters */
  jpeg_set_defaults(&cinfo);
  
  cinfo.dct_method  = JDCT_FLOAT;    
  
  jpeg_set_quality(&cinfo, image_quality, TRUE);
  
  /* Step 4: start compressor */
  jpeg_start_compress(&cinfo, TRUE);
  
  /* Get bitmap data address offset */
  memcpy(buff, file_buffer_in, 14);

  index = *(volatile uint16_t *) (buff + 10);
  index |= (*(volatile uint16_t *) (buff + 12)) << 16;

  while (cinfo.next_scanline < cinfo.image_height)
  {          
    /* In this application, the input file is a BMP, which first encodes the bottom of the picture */
    /* JPEG encodes the highest part of the picture first. We need to read the lines upside down   */
    /* Move the read pointer to 'last line of the picture - next_scanline'    */
    memcpy(buff, (const void*)((uint8_t*)file_buffer_in+((cinfo.image_height-1-cinfo.next_scanline)*width*3)+index), width*3);
    row_pointer = (JSAMPROW)buff;

    /* switch R and B */
    uint8_t tmp;
    for(uint32_t i=0; i<width; i++)
    {
      tmp = buff[3*i+0];
      buff[3*i+0] = buff[3*i+2];
      buff[3*i+2] = tmp;
    }
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);          
  }

  /* Step 5: finish compression */
  jpeg_finish_compress(&cinfo);
  
  /* Step 6: release JPEG compression object */
  jpeg_destroy_compress(&cinfo);
    
  return s_write_index;
}