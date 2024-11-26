#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "jpeg_encode_buffer.h"

/* encode_test infile outfile width height quality
 */
int main(int argc, char* argv[])
{
  FILE* infile;
  void* inbuffer;
  FILE* outfile;
  void* outbuffer;
  long  infilelen;
  int width;
  int heigth;
  int quality;
  void* linebuf;
  size_t outfilelen;

  if(argc != 6)
  {
    printf("encode_test infile outfile width height quality\r\n");
    sleep(1);
    return -1;
  }
  width = atoi(argv[3]);
  heigth = atoi(argv[4]);
  quality = atoi(argv[5]);

  printf("encode %s to %s, width=%d, height=%d, quality=%d\r\n",argv[1], argv[2], width, heigth, quality);

  infile = fopen(argv[1],"rb");
  if(infile == NULL)
  {
    printf("open %s err %s\r\n", argv[1], strerror(errno));
    sleep(1);
    return -1;
  }

  fseek(infile, 0, SEEK_END); 
  infilelen = ftell(infile); 
  fseek(infile, 0, SEEK_SET);

  printf("infilelen=%ld\r\n",infilelen);

  inbuffer = malloc(infilelen);
  if(infilelen != fread(inbuffer, 1, infilelen, infile))
  {
    printf("read infile err\r\n");
    fclose(infile);
    free(inbuffer);
    sleep(1);
    return -1;
  }
  fclose(infile);

  outbuffer = malloc(infilelen);

  linebuf = malloc(width * 3);

  outfilelen = jpeg_encode(inbuffer, outbuffer, width, heigth, quality, linebuf);
  printf("outfilelen=%ld\r\n",outfilelen);

  outfile = fopen(argv[2],"wb+");
  if(outfile != NULL)
  {
    if(outfilelen != fwrite(outbuffer, 1, outfilelen, outfile))
    {
      printf("write outfile err\r\n");
      sleep(1);
    }
  }
  else
  {
    printf("open %s err %s\r\n", argv[1], strerror(errno));
    sleep(1);
  }
  fclose(outfile);

  free(inbuffer);
  free(outbuffer);
  free(linebuf);
}