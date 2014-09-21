#include "hackProc.h"
S4      hackProc( HACKDATA *data,  HACKINFO *info, CODES *codesBuff, S4 hi, U2 ci )
{
#define RD_ADDR( ADDR, SIZE, DATA ) \
          if ( SIZE == 2 ) DATA = *((U2*)ADDR); \
          else if ( SIZE == 4 ) DATA = *((U4*)ADDR); \
          else if ( SIZE == 8 ) DATA = *((U8*)ADDR); \
          else DATA = *((U1*)ADDR)
#define WR_ADDR( ADDR, SIZE, DATA ) \
          if ( SIZE == 2 ) *((U2*)ADDR) = DATA & U2_MAX; \
          else if ( SIZE == 4 ) *((U4*)ADDR) = DATA & U4_MAX; \
          else if ( SIZE == 8 )  *((U8*)ADDR) = DATA; \
          else *((U1*)ADDR) = DATA & U1_MAX
  CODES *codes = ( hi < 0 ) ? &(info->codes) : codesBuff;
  CODE *code;
  int i = 0;
  U2 j = 0, cmp = 0;
  DWORD sys = 0;
  U8 addr = 0, tmpa = 0;
  RLN val = {0}, mod = {0};
  if ( hi >= 0 )
  {
    sfseek( data->codes, sizeof(CODES) * hi, SEEK_SET );
    sfread( codesBuff, sizeof(CODES), 1, data->codes );
  }
  for ( ; ci < codes->c; ++ci )
  {
    code = &codes->a[ci];
    i = 0;
    val = code->a[0];
    mod = code->a[1];
    addr = code->addr[0];
    switch ( code->type )
    {
    case CODE_W:
      if ( code->info < 2 )
      {
        // Write / Slide Write
        do
        {
          WR_ADDR( addr, code->dataSize, val.u );
          if ( code->dataType == DATA_TYPE_SI )
            val.s += mod.s;
          else
            val.u += mod.u;
          addr += code->addr[1]; ++i;
        }
        while ( i < code->loop );
      }
      else
      {
        // List Write / Slide List Write
        do
        {
          j = 0; tmpa = addr;
          do
          {
            WR_ADDR( tmpa, code->dataSize, code->a[j].u );
            tmpa += code->dataSize; ++j;
          }
          while ( j < code->info );
          addr += code->addr[1]; ++i;
        }
        while ( i < code->loop );
      }
      break;
    case CODE_INC:
      // Increment / Slide Increment / List Increment / Slide List Increment
      do
      {
        j = 0; tmpa = addr;
        do
        {
          RD_ADDR( addr, code->dataSize, val.u );
          if ( code->dataType == DATA_TYPE_SI )
            val.s += code->a[j].s;
          else
            val.u += code->a[j].u;
          WR_ADDR( addr, code->dataSize, val.u );
          tmpa += code->dataSize; ++j;
        }
        while ( j < code->info );
        addr += code->addr[1]; ++i;
      }
      while ( i < code->loop );
      break;
    case CODE_DEC:
      // Decrement / Slide Decrement / List Decrement / Slide List Decrement
      do
      {
        j = 0; tmpa = addr;
        do
        {
          RD_ADDR( addr, code->dataSize, val.u );
          if ( code->dataType == DATA_TYPE_SI )
            val.s -= code->a[j].s;
          else
            val.u -= code->a[j].u;
          WR_ADDR( addr, code->dataSize, val.u );
          tmpa += code->dataSize; ++j;
        }
        while ( j < code->info );
        addr += code->addr[1]; ++i;
      }
      while ( i < code->loop );
      break;
    case CODE_CPY:
      RD_ADDR( addr, code->dataSize, val.u );
      WR_ADDR( code->a[0].u, code->dataSize, val.u );
      break;
    case CODE_JOKER:
    case CODE_CMP:
      // Test for value
      RD_ADDR( addr, code->dataSize, val.u );
      cmp = ( code->info & 0xF0 ) >> 4;
      if ( code->info & 0x8 )
      {
        if  ( cmp == CMP_EQ )
          mod.u =  ( val.s == code->a[0].s );
        else if ( cmp == CMP_NE )
          mod.u =  ( val.s != code->a[0].s );
        else if ( cmp == CMP_MT )
          mod.u =  ( val.s >  code->a[0].s );
        else if ( cmp == CMP_ME )
          mod.u =  ( val.s >= code->a[0].s );
        else if ( cmp == CMP_LT )
          mod.u =  ( val.s <  code->a[0].s );
        else if ( cmp == CMP_LE )
          mod.u =  ( val.s <= code->a[0].s );
        else if ( cmp == CMP_IA )
          mod.u = (( val.s &  code->a[0].s ) == code->a[0].s );
        else if ( cmp == CMP_NA )
          mod.u = (( val.s &  code->a[0].s ) != code->a[0].s );
        else if ( cmp == CMP_IO )
          mod.u = (( val.s |  code->a[0].s ) != 0 );
        else if ( cmp == CMP_NO )
          mod.u = (( val.s |  code->a[0].s ) == 0 );
        else if ( cmp == CMP_IX )
          mod.u = (( val.s ^  code->a[0].s ) != 0 );
        else if ( cmp == CMP_NX )
          mod.u = (( val.s ^  code->a[0].s ) == 0 );
        else
          mod.u = 0;
      }
      else
      {
        if  ( cmp == CMP_EQ )
          mod.u =  ( val.u == code->a[0].u );
        else if ( cmp == CMP_NE )
          mod.u =  ( val.u != code->a[0].u );
        else if ( cmp == CMP_MT )
          mod.u =  ( val.u >  code->a[0].u );
        else if ( cmp == CMP_ME )
          mod.u =  ( val.u >= code->a[0].u );
        else if ( cmp == CMP_LT )
          mod.u =  ( val.u <  code->a[0].u );
        else if ( cmp == CMP_LE )
          mod.u =  ( val.u <= code->a[0].u );
        else if ( cmp == CMP_IA )
          mod.u = (( val.u &  code->a[0].u ) == code->a[0].u );
        else if ( cmp == CMP_NA )
          mod.u = (( val.u &  code->a[0].u ) != code->a[0].u );
        else if ( cmp == CMP_IO )
          mod.u = (( val.u |  code->a[0].u ) != 0 );
        else if ( cmp == CMP_NO )
          mod.u = (( val.u |  code->a[0].u ) == 0 );
        else if ( cmp == CMP_IX )
          mod.u = (( val.u ^  code->a[0].u ) != 0 );
        else if ( cmp == CMP_NX )
          mod.u = (( val.u ^  code->a[0].u ) == 0 );
        else
          mod.u = 0;
      }
      if ( code->type == CODE_JOKER )
      {
        // Switch on/off if result is true
        code->info ^= (mod.u << 2);
        // Use on/off state as result
        mod.u = (code->info & 4);
      }
      if ( mod.u )
        // Continue
        break;
      else if ( code->loop )
      {
        // Continue but skip # codes due to result of false
        ci += code->loop;
        break;
      }
      // Skip children & remaining codes, move onto next sibling
      return ( info->hacks.a[hi].ni || -1 );
    default: ci = codes->c;
    }
  }
  if ( hi >= 0 )
  {
    // Purely for remembering Joker switches
    sfseek( data->codes, sizeof(CODES) * hi, SEEK_SET );
    sfwrite( codesBuff, sizeof(CODES), 1, data->codes );
    sfflush( data->codes );
    hi = info->hacks.a[hi].fi;
  }
  else
  {
    sfwrite( info, sizeof(HACKINFO), 1, data->hacks );
    sfflush( data->hacks );
    hi = 0;
  }
  while ( hi >= 0 )
    hi = hackProc( data, info, codesBuff, hi, 0 );
  return ( info->hacks.a[hi].ni || -1 );
}
