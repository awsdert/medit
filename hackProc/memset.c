#include "hackProc.h"

#pragma function(memset)
void* __cdecl memset( void *dst, int val, size_t size )
{
  size_t i = 0;
  unsigned char *tmp = ( unsigned char* )dst;

  while ( i < size )
  {
    tmp[i] = val & MAXBYTE;
  }

  return dst;
}

#pragma function(memcpy)
void* __cdecl memcpy( void *dst, void const *src, size_t size )
{
  size_t i = 0;
  unsigned char *tmpd = ( unsigned char* )dst;
  unsigned char const *tmps = ( unsigned char const* )src;

  while ( i < size )
  {
    tmpd[i] = tmps[i];
  }

  return dst;
}
