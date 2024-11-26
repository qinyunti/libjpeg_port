#include "jconfig.h"
#include "jioport.h"

jioport_st* s_jioport = (jioport_st*)0;

size_t jfread(void * __ptr, size_t __size, size_t __n, void * __stream)
{
  #if JPEG_LOG
  jprintf("jfread ptr:%p size:%ld n:%ld stream:%p\r\n",__ptr,__size,__n,__stream);
  #endif
  if((s_jioport != (jioport_st*)0) && (s_jioport->read != 0))
  {
    return s_jioport->read(__ptr, __size, __n, __stream);
  }
  return 0;
}

size_t jfwrite(const void * __ptr, size_t __size, size_t __n, void * __stream)
{
  #if JPEG_LOG
  jprintf("jfwrite ptr:%p size:%ld n:%ld stream:%p\r\n",__ptr,__size,__n,__stream);
  #endif
  if((s_jioport != (jioport_st*)0) && (s_jioport->write != 0))
  {
    return s_jioport->write(__ptr, __size, __n, __stream);
  }
  return 0;
}

int    jfflush(void * __stream)
{
  #if JPEG_LOG
  jprintf("jfflush stream:%p\r\n",__stream);
  #endif
  if((s_jioport != (jioport_st*)0) && (s_jioport->flush != 0))
  {
    return s_jioport->flush(__stream);
  }
  return 0;
}

int    jferror(void * __fp)
{
  #if JPEG_LOG
  jprintf("jferror fp:%p\r\n",__fp);
  #endif
  if((s_jioport != (jioport_st*)0) && (s_jioport->error != 0))
  {
    return s_jioport->flush(__fp);
  }
  return 0;
}

void jioport_set(jioport_st* port)
{
  s_jioport = port;
}