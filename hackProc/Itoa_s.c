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
    tmp[i] = buff[ val ];
    ++i;
  }

  while ( i > 0 && d < dstSize )
  {
    dst[d] = tmp[i];
    ++d;
    --i;
  }

  dst[d] = tmp[i];

  if ( d < ( dstSize - 1 ) )
  {
    ++d;
  }

  dst[d] = sign;
  return ( i > 0 );
}
int Div( int num, int dem, int *rest )
{
  int i = ( ( sizeof( int ) * CHAR_BITS ) - 1 ),
      val = 0, quo = 0, bit = ( 1 << i );

  while ( num > dem && bit > 0 )
  {
    quo <<= 1;
    val = ( num & bit ) >> i;
    quo |= ( val > dem );
    num ^= bit;
    bit >>= 1;
    --i;
  }

  if ( rest )
  {
    *rest = num;
  }

  return quo;
}
#ifdef _MSC_VER
#define __ASM__ __asm
#elif defined( __GCC__ )
#define __ASM__ __asm__
#else
#define __ASM__ asm
#endif
#ifdef __ARM__
#define AND( DST, SRC )        and  %DST,%SRC
#define CMP( SRC, TO )         cmp  %SRC,%TO
#else
#define AND( DST, SRC )        and  DST,SRC
#define CMP( SRC, TO )         cmp  SRC,TO
#define JAE( TYPE, TO )        jae  TYPE TO
#define MOV( DST, SRC )        mov  DST,SRC
#define RET( SRC )             ret  SRC
#define SHL( DST, BITS )       shl  DST,BITS
#define SHLD( DST, SRC, BITS ) shld DST,SRC,BITS
#define XOR( DST, SRC )        xor  DST,SRC
#endif // __X86__
#if _WIN == 32
// Copied from microsoft research
__declspec( naked ) void __cdecl _allshl( void )
{
  __ASM__
  {
// Handle shifts of 64 or more bits (all get 0)
    cmp     cl, 64
    jae     short RETZERO
// Handle shifts of between 0 and 31 bits
    cmp     cl, 32
    jae     short MORE32
    shld    edx, eax, cl
    shl     eax, cl
    ret
// Handle shifts of between 32 and 63 bits
MORE32:
    mov     edx, eax
    xor     eax, eax
    and     cl, 31
    shl     edx, cl
    ret
// return 0 in edx:eax
RETZERO:
    xor     eax, eax
    xor     edx, edx
    ret
  }
}
#endif
