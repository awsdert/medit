#include "_main.h"

void ftoa( double value, char* dst, int sigDigits )
{
  char tmp[ _CVTBUFSIZE ] = {0};
  int dec = 0, sign = 0, i = 0, d = 0;
  _fcvt_s( tmp, _CVTBUFSIZE, value, sigDigits, &dec, &sign );
  if ( !tmp[0] )
    tmp[0] = '0';
  if ( sign != 0 )
  {
    dst[0] = '-';
    ++i;
  }
  if ( dec <= 0 )
  {
    dec = -dec;
    dst[i] = '0';
    dst[++i] = '.';
    for ( d = 0; d < dec; ++d )
      dst[++i] = '0';
    d = 0;
  }
  else
  {
    for ( ; d < dec; ++d, ++i )
      dst[i] = tmp[d];
    dst[++i] = '.';
  }
  while ( d < _CVTBUFSIZE && tmp[d] )
  {
    dst[++i] = tmp[d];
    ++d;
  }
  dst[++i] = 0;
}

void ftoa_s( double value, char* dst, uzv length, int sigDigits )
{
  char tmp[ _CVTBUFSIZE ] = {0};
  int dec = 0, sign = 0;
  uzv i = 0, d = 0, c = 0;
  if ( length < 1 )
    return;
  memset( dst, 0, length );
  --length;
  _fcvt_s( tmp, _CVTBUFSIZE, value, sigDigits, &dec, &sign );
  if ( !tmp[0] )
    tmp[0] = '0';
  if ( sign != 0 )
  {
    dst[0] = '-';
    ++i;
  }
  if ( dec <= 0 )
  {
    c = -dec;
    if ( i < length )
      dst[i] = '0';
    if ( ++i < length )
      dst[i] = '.';
    for ( d = 0, ++i; d < c && i < length; ++d, ++i )
      dst[i] = '0';
    d = 0;
  }
  else
  {
    c = dec;
    for ( ; d < c && i < length; ++d, ++i )
      dst[i] = tmp[d];
    if ( i < length )
      dst[++i] = '.';
  }
  while ( d < _CVTBUFSIZE && tmp[d] && i < length )
  {
    dst[++i] = tmp[d];
    ++d;
  }
  dst[++i] = 0;
}
