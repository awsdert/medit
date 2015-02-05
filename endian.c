#include <Ipipe/types.h>
void _revBytes( uchar *src, size_t size )
{
  uchar byte;
  size_t i = 0, j = size - 1;

  for ( ; i < j; ++i, --j )
  {
    byte = src[i];
    src[i] = src[j];
    src[j] = byte;
  }
}
void _swapBytes( uchar *src, size_t size )
{
  uchar byte;
  size_t i = 0, j = 1;

  for ( ; j < size; i += 2, j += 2 )
  {
    byte = src[i];
    src[i] = src[j];
    src[j] = byte;
  }
}

char getEndian( void )
{
  ulong test = 0x0000FEFF;
  uchar *t = ( uchar* )&test;

  switch ( t[0] )
  {
  case 0xfe:
    return 'P';

  case 0xff:
    return 'L';
  }

  return 'B';
}

char changeEndian( void* source, size_t size, char curEndian, char nxtEndian )
{
  uchar *src = source;

  switch ( curEndian )
  {
  case 'L':
  case 'P':
  case 'B':
    break;

  case 'l':
    curEndian = 'L';
    break;

  case 'b':
    curEndian = 'B';
    break;

  case 'm':
  case 'M':
  case 'p':
    curEndian = 'M';
    break;

  default:
    return 'E';
  }

  switch ( nxtEndian )
  {
  case 'L':
  case 'P':
  case 'B':
    break;

  case 'l':
    nxtEndian = 'L';
    break;

  case 'b':
    nxtEndian = 'B';
    break;

  case 'm':
  case 'M':
  case 'p':
    nxtEndian = 'M';
    break;

  default:
    return 'E';
  }

  if ( !size )
  {
    return curEndian + 'S';
  }

  if ( !source )
  {
    return curEndian + 'N';
  }

  if ( curEndian == nxtEndian )
  {
    return curEndian;
  }
  else if ( curEndian == 'M' )
  {
    _swapBytes( src, size );

    if ( nxtEndian == 'B' )
    {
      _revBytes( src, size );
    }
  }
  else if ( nxtEndian == 'M' )
  {
    if ( curEndian == 'B' )
    {
      _revBytes( src, size );
    }

    _swapBytes( src, size );
  }
  else
  {
    _revBytes( src, size );
  }

  return nxtEndian;
}
