#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "jpeg_decode_buffer.h"

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
  void* linebuf;
  size_t outfilelen;

  if(argc != 4)
  {
    printf("decode_test infile outfile width\r\n");
    sleep(1);
    return -1;
  }
  width = atoi(argv[3]);

  printf("decode %s to %s, width=%d\r\n",argv[1], argv[2], width);

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
  fread(inbuffer, 1, infilelen, infile);
  fclose(infile);

  outbuffer = malloc(infilelen*40); /* 按照40倍数压缩率预留空间 */

  linebuf = malloc(width * 3);

  outfilelen = jpeg_decode(inbuffer, outbuffer, width, linebuf, infilelen);
  printf("outfilelen=%ld\r\n",outfilelen);

  outfile = fopen(argv[2],"wb+");
  if(outfile != NULL)
  {
    fwrite(outbuffer, 1, outfilelen, outfile);
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