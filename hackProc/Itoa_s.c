#include "hackProc.h"
BOOL    Itoa_s( int value, char* dst, size_t dstSize, unsigned char base )
{
  int val = 0;
  size_t i = 0, d = 0;
  char sign = 0, tmp[66] = {0}, buff[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  if ( value < 0 )
  {
    value = -value;
    sign = '-';
  }
  while ( value > 0 && i < 66 )
  {
    value = Div( value, base, &val );
    tmp[i] = buff[ val ]; ++i;
  }
  while ( i > 0 && d < dstSize )
  {
    dst[d] = tmp[i];
    ++d; --i;
  }
  dst[d] = tmp[i];
  if ( d < (dstSize - 1))
    ++d;
  dst[d] = sign;
  return (i > 0);
}
int Div( int num, int dem, int *rest )
{
  int i = ((sizeof(int) * CHAR_BITS) - 1),
    val = 0, quo = 0, bit = (1 << i);
  while ( num > dem && bit > 0 )
  {
    quo <<= 1;
    val = (num & bit) >> i;
    quo |= ( val > dem );
    num ^= bit;
    bit >>= 1; --i;
  }
  if ( rest )
    *rest = num;
  return quo;
}
