#include "hackProc.h"
int      hackProc( HACKDATA *data,  HACKINFO *info, CODES *codesBuff, int hi,
                   ulong ci )
{
#define RD_ADDR( ADDR, SIZE, DATA ) \
          if ( SIZE == 2 ) DATA = *((ushort*)ADDR); \
          else if ( SIZE == 4 ) DATA = *((uint*)ADDR); \
          else if ( SIZE == 8 ) DATA = *((ulint*)ADDR); \
          else DATA = *((uchar*)ADDR)
#define WR_ADDR( ADDR, SIZE, DATA ) \
          if ( SIZE == 2 ) *((ushort*)ADDR) = DATA & USHRT_MAX; \
          else if ( SIZE == 4 ) *((uint*)ADDR) = DATA & UINT_MAX; \
          else if ( SIZE == 8 )  *((ulint*)ADDR) = DATA; \
          else *((uchar*)ADDR) = DATA & UCHAR_MAX
  CODES *codes = ( hi < 0 ) ? &( info->codes ) : codesBuff;
  CODE *code;
  uint i = 0;
  ushort j = 0, cmp = 0;
  DWORD sys = 0;
  ulint addr = 0, tmpa = 0;
  NUM val = {0}, mod = {0};

  if ( hi >= 0 )
  {
    sfseek( data->codes, sizeof( CODES ) * hi, SEEK_SET );
    sfread( codesBuff, sizeof( CODES ), 1, data->codes );
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
          WR_ADDR( addr, code->dataSize, val.u_lint[0] );

          if ( code->dataType == DATA_TYPE_SI )
          {
            val.s_lint[0] += mod.s_lint[0];
          }
          else
          {
            val.u_lint[0] += mod.u_lint[0];
          }

          addr += code->addr[1];
          ++i;
        }
        while ( i < code->loop );
      }
      else
      {
        // List Write / Slide List Write
        do
        {
          j = 0;
          tmpa = addr;

          do
          {
            WR_ADDR( tmpa, code->dataSize, code->a[j].u_lint[0] );
            tmpa += code->dataSize;
            ++j;
          }
          while ( j < code->info );

          addr += code->addr[1];
          ++i;
        }
        while ( i < code->loop );
      }

      break;

    case CODE_INC:

      // Increment / Slide Increment / List Increment / Slide List Increment
      do
      {
        j = 0;
        tmpa = addr;

        do
        {
          RD_ADDR( addr, code->dataSize, val.u_lint[0] );

          if ( code->dataType == DATA_TYPE_SI )
          {
            val.s_lint[0] += code->a[j].s_lint[0];
          }
          else
          {
            val.u_lint[0] += code->a[j].u_lint[0];
          }

          WR_ADDR( addr, code->dataSize, val.u_lint[0] );
          tmpa += code->dataSize;
          ++j;
        }
        while ( j < code->info );

        addr += code->addr[1];
        ++i;
      }
      while ( i < code->loop );

      break;

    case CODE_DEC:

      // Decrement / Slide Decrement / List Decrement / Slide List Decrement
      do
      {
        j = 0;
        tmpa = addr;

        do
        {
          RD_ADDR( addr, code->dataSize, val.u_lint[0] );

          if ( code->dataType == DATA_TYPE_SI )
          {
            val.s_lint[0] -= code->a[j].s_lint[0];
          }
          else
          {
            val.u_lint[0] -= code->a[j].u_lint[0];
          }

          WR_ADDR( addr, code->dataSize, val.u_lint[0] );
          tmpa += code->dataSize;
          ++j;
        }
        while ( j < code->info );

        addr += code->addr[1];
        ++i;
      }
      while ( i < code->loop );

      break;

    case CODE_CPY:
      RD_ADDR( addr, code->dataSize, val.u_lint[0] );
      WR_ADDR( code->a[0].u_lint[0], code->dataSize, val.u_lint[0] );
      break;

    case CODE_JOKER:
    case CODE_CMP:
      // Test for value
      RD_ADDR( addr, code->dataSize, val.u_lint[0] );
      cmp = ( code->info & 0xF0 ) >> 4;

      if ( code->info & 0x8 )
      {
        if  ( cmp == CMP_EQ )
        {
          mod.u_lint[0] =  ( val.s_lint[0] == code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_NE )
        {
          mod.u_lint[0] =  ( val.s_lint[0] != code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_MT )
        {
          mod.u_lint[0] =  ( val.s_lint[0] >  code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_ME )
        {
          mod.u_lint[0] =  ( val.s_lint[0] >= code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_LT )
        {
          mod.u_lint[0] =  ( val.s_lint[0] <  code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_LE )
        {
          mod.u_lint[0] =  ( val.s_lint[0] <= code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_IA )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] &  code->a[0].s_lint[0] ) ==
                            code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_NA )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] &  code->a[0].s_lint[0] ) !=
                            code->a[0].s_lint[0] );
        }
        else if ( cmp == CMP_IO )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] |  code->a[0].s_lint[0] ) != 0 );
        }
        else if ( cmp == CMP_NO )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] |  code->a[0].s_lint[0] ) == 0 );
        }
        else if ( cmp == CMP_IX )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] ^  code->a[0].s_lint[0] ) != 0 );
        }
        else if ( cmp == CMP_NX )
        {
          mod.u_lint[0] = ( ( val.s_lint[0] ^  code->a[0].s_lint[0] ) == 0 );
        }
        else
        {
          mod.u_lint[0] = 0;
        }
      }
      else
      {
        if  ( cmp == CMP_EQ )
        {
          mod.u_lint[0] =  ( val.u_lint[0] == code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_NE )
        {
          mod.u_lint[0] =  ( val.u_lint[0] != code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_MT )
        {
          mod.u_lint[0] =  ( val.u_lint[0] >  code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_ME )
        {
          mod.u_lint[0] =  ( val.u_lint[0] >= code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_LT )
        {
          mod.u_lint[0] =  ( val.u_lint[0] <  code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_LE )
        {
          mod.u_lint[0] =  ( val.u_lint[0] <= code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_IA )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] &  code->a[0].u_lint[0] ) ==
                            code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_NA )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] &  code->a[0].u_lint[0] ) !=
                            code->a[0].u_lint[0] );
        }
        else if ( cmp == CMP_IO )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] |  code->a[0].u_lint[0] ) != 0 );
        }
        else if ( cmp == CMP_NO )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] |  code->a[0].u_lint[0] ) == 0 );
        }
        else if ( cmp == CMP_IX )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] ^  code->a[0].u_lint[0] ) != 0 );
        }
        else if ( cmp == CMP_NX )
        {
          mod.u_lint[0] = ( ( val.u_lint[0] ^  code->a[0].u_lint[0] ) == 0 );
        }
        else
        {
          mod.u_lint[0] = 0;
        }
      }

      if ( code->type == CODE_JOKER )
      {
        // Switch on/off if result is true
        code->info ^= ( mod.u_lint[0] << 2 );
        // Use on/off state as result
        mod.u_lint[0] = ( code->info & 4 );
      }

      if ( mod.u_lint[0] )
        // Continue
      {
        break;
      }
      else if ( code->loop )
      {
        // Continue but skip # codes due to result of false
        ci += code->loop;
        break;
      }

      // Skip children & remaining codes, move onto next sibling
      return ( info->hacks.a[hi]._ni || -1 );

    default:
      ci = codes->c;
    }
  }

  if ( hi >= 0 )
  {
    // Purely for remembering Joker switches
    sfseek( data->codes, sizeof( CODES ) * hi, SEEK_SET );
    sfwrite( codesBuff, sizeof( CODES ), 1, data->codes );
    sfflush( data->codes );
    hi = info->hacks.a[hi]._fi;
  }
  else
  {
    sfwrite( info, sizeof( HACKINFO ), 1, data->hacks );
    sfflush( data->hacks );
    hi = 0;
  }

  while ( hi >= 0 )
  {
    hi = hackProc( data, info, codesBuff, hi, 0 );
  }

  return ( info->hacks.a[hi]._ni || -1 );
}
