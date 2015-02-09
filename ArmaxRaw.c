#include "ArmaxRawMain.h"

HACK_COM _armaxRawCOM = {0};

hack_t _armaxRawHack_OnLoad( FILE *file, char *path, char const *dir,
                             char *line, hack_t hi, hack_t oi )
{
  FILE *data;
  uchar ci = 0;
  size_t len = 0;
  CODE *code;
  HACK *indx, *prev = hi ? &_armaxRawCOM.hl.hacks->a[hi++] : NULL;

  if ( feof( file ) )
  {
    return hi;
  }

  while ( fgets( line, 80, file ) )
  {
    if ( len = ferror( file ) )
    {
      break;
    }

    if ( *line < '"' )
    {
      continue;
    }

    if ( *line == '"' || *line == '\'' )
    {
      copystri( line, &line[1], 80 );
    }

    len = strlen( line );
    --len;

    if ( line[len] == '"' || line[len] == '\'' )
    {
      line[len] = 0;
    }

    if ( hi == _armaxRawCOM.hl.hacks->size.total )
    {
      // Plenty of codelists of 100s of cheats so optimise for speed
      _armaxRawCOM.ResizeHacks( &_armaxRawCOM.hl, hi + 500 );
    }

    indx = &_armaxRawCOM.hl.hacks->a[hi];
    indx->id = hi;
    indx->oi = oi;
    copystri( _armaxRawCOM.hl.names[hi].a, line, NAME_MAX );
    fgets( line, 80, file );

    if ( !armaxRawHack_Txt2Raw( indx, line, file ) && oi )
    {
      return ++hi;
    }
    len = 0;
    if ( *line > '"' )
    {
      _armaxRawCOM.ResizeCodes( &_armaxRawCOM.cl, 0 );
      _armaxRawCOM.cl.codes->size.count = 0;

      for ( ci = 0; fgets( line, 80, file ); )
      {

        if ( len = ferror( file ) || *line < '"' )
        {
          break;
        }

        if ( ci == _armaxRawCOM.cl.codes->size.total )
        {
          _armaxRawCOM.ResizeCodes( &_armaxRawCOM.cl, ci + 10 );
        }

        code = &_armaxRawCOM.cl.codes->a[ci++];
        _armaxRawCOM.cl.codes->size.count = ci;
        armaxRawCode_Txt2Raw( code, line, file );
      }

      memset( line, 0, 80 );
      snprintf( line, 80, "%X", hi );
      memset( path, 0, PATH_MAX );
      copystri( path, dir, PATH_MAX );
      appendstr( path, line, PATH_MAX );
      appendstr( path, ".src-codes", PATH_MAX );

      if ( _access( path, 0 ) != 0 )
      {
        _creat( path, _S_IREAD | _S_IWRITE );
      }

      data = fopen( path, "wb" );
      fwrite( _armaxRawCOM.cl.codes, _armaxRawCOM.cl.codes->size.bytes + sizeof(VCODES), 1, data );
      fflush( data );
      fclose( data );
    }

    if ( prev )
    {
      if ( !indx->cid || indx->coi == prev->cid )
      {
        prev->fi = hi;
        indx->oi = prev->id;
        hi = _armaxRawHack_OnLoad( file, path, dir, line, hi, prev->id );
      }
      else
      {
        indx->pi = prev->id;
        prev->ni = hi;
        ++hi;
      }
    }
    else
    {
      ++hi;
    }

    prev = indx;

    if ( len = ferror( file ) || feof( file ) )
    {
      break;
    }
  }

  return hi;
}

void  HACK_LIB_EXP armaxRawHack_OnLoad( char *_path, char const *dir )
{
  FILE *file = fopen( _path, "r" );
  char line[80] = {0};
  char path[PATH_MAX] = {0};

  if ( !file )
  {
    return;
  }

  _armaxRawCOM.RdError = ( _HACK_COM_GERR )ferror;
  _armaxRawCOM.RdLine  = ( _HACK_COM_GETS )fgets;
  _armaxRawCOM.WrLine  = ( _HACK_COM_PUTS )fputs;
  _armaxRawCOM.ResizeHacks( &_armaxRawCOM.hl, 0 );
  _armaxRawCOM.hl.hacks->size.count = _armaxRawHack_OnLoad( file, path, dir, line, 0, 0 );
  _armaxRawCOM.hl.hacks->size.index = _armaxRawCOM.hl.hacks->size.count - 1;
  fclose( file );
}
void  HACK_LIB_EXP armaxRawHack_OnSave( char *_path, char const *dir )
{
  // TODO: Implement Armax Raw Saver
}

uchar HACK_LIB_EXP armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source )
{
  hack_t         i = ( *line > '"' ) ? 1 : 0;
  char*        tok = i ? (insertstr( line, " ", 80, 8 ).p ? line : NULL) : NULL;
  ulong         p1 = i ? strtoul( line, &tok, 16 ) : 0;
  ulong         p2 = i ? strtoul( tok, NULL, 16 ) : 0;
  ulong        pid = i ? p2 & 0xFFFFF : 0x800;
  ushort const gid = i ? ( ushort )( p1 >> 16 ) : _armaxRawCOM.gid;
  uchar  const reg = i ? ( uchar )( ( p2 >> 24 ) & 0xF ) :
                     ( _armaxRawCOM.reg == REGION_US ? 0 : ( _armaxRawCOM.reg == REGION_UK ? 1 :
                         2 ) );
  uchar  const ibr = i ? ( uchar )( ( p2 >> 20 ) & 0xF ) : 5;
  // GameID

  if ( !_armaxRawCOM.gid )
  {
    _armaxRawCOM.gid = gid;
  }
  else if ( gid != _armaxRawCOM.gid )
  {
    return 0;
  }

  // HackID

  hack->cid = ( ushort )( ( ( p1 << 4 ) & 0xFFF0 ) | ( p2 >> 28 ) );

  // Region
  if ( !_armaxRawCOM.reg )
  {
    switch ( reg & 0x3 )
    {
    case 0x0:
      _armaxRawCOM.reg = REGION_US;
      break;

    case 0x1:
      _armaxRawCOM.reg = REGION_UK;
      break;

    case 0x2:
      _armaxRawCOM.reg = REGION_JP;
      break;

    case 0x3:
      return 0;
    }
  }

  // Parent ID
  if ( pid )
  {
    if ( !hack->id )
    {
      hack->irl = 0;
      // FIXME: Support (M) Info
    }
    else if ( ibr == 0x5 )
    {
      // Parent is (M) so check if is actually radio list
      hack->irl = ( pid & 0x1000 ) ? 1 : 0;
    }
    else
    {
      // Check if is SubFolder
      if ( !( pid & 0x1 ) )
      {
        hack->irl = 0;

        if ( !( *line ) )
        {
          return 0;
        }

        if ( !pid )
        {
          return 1;
        }

        // SubFolder type stored on next line
        if ( _armaxRawCOM.RdLine( line, 80, _source ) )
        {
          // Code line starts with A0 for some reason
          switch ( line[2] )
          {
          case '1':
            break;

          case '3':
            hack->irl = 1;
            break;

          default:
            return 0;
          }
        }
        else
        {
          return 0;
        }
      }

      hack->coi = ( ushort )( pid >> 1 );
    }
  }

  // Let Parent ID be defined elsewhere
  return 1;
}
uchar HACK_LIB_EXP armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source )
{
  uchar r;
  uchar ibr = ( hack->fi && !hack->oi ) ? 0x5 : 0x4;
  ulong oi = hack->oi;
  oi <<= 1;

  if ( !hack->fi )
  {
    oi |= 0x1;
  }

  // Region
  switch ( _armaxRawCOM.reg )
  {
  case REGION_US:
    r = 0;
    break;

  case REGION_UK:
    r = 1;
    break;

  case REGION_JP:
    r = 2;
    break;

  default:
    return 0;
  }

  if ( !hack->id )
  {
    r |= 0x8;
  }

  // Line
  if ( !hack->id || hack->oi || !hack->fi  )
  {
    snprintf( line, 5, "%04X%05X%01X%01X%05X", _armaxRawCOM.gid, hack->id, r, ibr,
              oi );
  }
  else
  {
    snprintf( line, 5, "%04X%05X%01X%01X00800", _armaxRawCOM.gid, hack->id, r,
              ibr );

    if ( hack->irl )
    {
      line[13] = '1';
    }
  }

  line[4] = hack->fi ? '8' : '9';
  insertstr( line, " ", 80, 8 );

  if ( hack->oi && hack->fi )
  {
    _armaxRawCOM.WrLine( line, _source );
    memset( line, '0', 17 );
    line[0] = 'A';
    line[2] = hack->irl ? '3' : '1';
    line[8] = ' ';
  }

  return 1;
}
uchar  _armaxRawCode_Txt2RawRd( char *line, void *_source, ulong *p1,
                                ulong *p2 )
{
  char *tok = line;

  if ( _armaxRawCOM.RdLine( line, 80, _source ) && *line > '"' )
  {
    insertstr( line, " ", 80, 8 );
    *p1 = strtoul( line, &tok, 16 );
    *p2 = strtoul( tok, NULL, 16 );
    return 1;
  }
  else
  {
    *p1 = 0;
    *p2 = 0;
    return 0;
  }
}
void _armaxRawCode_Txt2RawMaster( CODE *code, char *line, void *_source,
                                  ulong *p1, ulong *p2 )
{
  code->type = CODE_MASTER;
  *p2 >>= 8;
  code->loop = *p2 & 0xFF;
  *p2 >>= 8;
  code->addr[2] = *p2;
}
void _armaxRawCode_Txt2RawWrite( CODE *code, char *line, void *_source,
                                 ulong *p1, ulong *p2 )
{
  code->type = CODE_W;

  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = ( *p2 & 0xFF );
    code->loop = ( *p2 >>= 8 );
    break;

  case 2:
    code->a[0]._ulint[0] = ( *p2 & 0xFFFF );
    code->loop = ( *p2 >>= 16 );
    break;

  default:
    code->a[0]._ulint[0] = *p2;
  }
}

void _armaxRawCode_Txt2RawInc( CODE *code, char *line, void *_source,
                               ulong *p1, ulong *p2 )
{
  code->type = CODE_INC;

  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = ( *p2 & 0xFF );
    code->loop = ( *p2 >>= 8 );
    break;

  case 2:
    code->a[0]._ulint[0] = ( *p2 & 0xFFFF );
    code->loop = ( *p2 >>= 16 );
    break;

  default:
    code->a[0]._ulint[0] = *p2;
  }
}
void _armaxRawCode_Txt2RawPtr( CODE *code, char *line, void *_source,
                               ulong *p1, ulong *p2 )
{
  code->type = CODE_W;

  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = ( *p2 & 0xFF );
    code->addr[2] = ( *p2 >>= 8 );
    break;

  case 2:
    code->a[0]._ulint[0] = ( *p2 & 0xFFFF );
    code->addr[2] = ( *p2 >>= 16 );
    break;

  default:
    code->a[0]._ulint[0] = *p2;
  }
}

void _armaxRawCode_Txt2RawCmp( CODE *code, char *line, void *_source,
                               ulong *p1, ulong *p2 )
{
  code->a[0]._ulint[0] = *p2;
  *p1 >>= 24;
  code->type = ( ( *p1 & 6u ) == 6u ) ? CODE_JOKER : CODE_CMP;

  switch ( *p1 & 0x38 )
  {
  case 0x10:
    code->info = CMP_NE;
    break;

  case 0x18:
    code->info = CMP_LT;
    code->format = 'i';
    break;

  case 0x20:
    code->info = CMP_MT;
    code->format = 'i';
    break;

  case 0x28:
    code->info = CMP_LT;
    break;

  case 0x30:
    code->info = CMP_MT;
    break;

  case 0x38:
    code->info = CMP_IA;
    break;

  default:
    code->info = CMP_EQ;
  }

  switch ( *p1 & 0xC0 )
  {
  case 0x40:
    code->loop = 2;
    break;

  case 0x80:
    code->loop = 0;
    break;

  default:
    code->loop = 1;
  }
}
void _armaxRawCode_Txt2RawLoop( CODE *code, char *line, void *_source,
                                ulong *p1, ulong *p2 )
{
  code->type = CODE_W;
  code->addr[0] = *p2 & 0x1FFFFFFF;

  switch ( ( *p2 >> 24 ) & 0xE )
  {
  case 0:
    code->size = 1;
    break;

  case 2:
    code->size = 2;
    break;

  case 4:
    code->size = 4;
    break;
  }

  if ( !_armaxRawCode_Txt2RawRd( line, _source, code->a[0]._ulong, p2 ) )
  {
    return;
  }

  code->a[1]._ulong[0] = *p2 >> 24;
  code->loop = ( *p2 >> 16 ) & 0xFF;
  code->addr[1] = *p2 & 0xFFFFF;
}
uchar HACK_LIB_EXP armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source )
{
  uchar i = ( *line > '"' ) ? 1 : 0;
  char *tok = i ? line : NULL;
  ulong p1 = i ? strtoul( insertstr( line, " ", 80, 8 ).p, &tok, 16 ) : 0;
  ulong p2 = i ? strtoul( tok,  NULL, 16 ) : 0;

  if ( p1 == 0xDEADC0DE )
  {
    return 0;
  }
  else if ( !p1 )
  {
    if ( p2 & 0x80000000 )
    {
      _armaxRawCode_Txt2RawLoop( code, line, _source, &p1, &p2 );
    }
    else
    {
      code->size = 1;
      _armaxRawCode_Txt2RawWrite( code, line, _source, &p1, &p2 );
    }
  }
  else if ( p1 & 0x3C000000 )
  {
    _armaxRawCode_Txt2RawCmp( code, line, _source, &p1, &p2 );
  }
  else
  {
    code->addr[0] = ( p1 & 0x1FFFFFF );

    switch ( ( p1 >> 24 ) & 0xE )
    {
    case 0:
      code->size = 1;
      break;

    case 2:
      code->size = 2;
      break;

    case 4:
      code->size = 4;
      break;
    }

    switch ( p1 >> 30 )
    {
    case 0:
      _armaxRawCode_Txt2RawWrite(  code, line, _source, &p1, &p2 );
      break;

    case 1:
      _armaxRawCode_Txt2RawPtr(    code, line, _source, &p1, &p2 );
      break;

    case 2:
      _armaxRawCode_Txt2RawInc(    code, line, _source, &p1, &p2 );
      break;

    case 3:
      _armaxRawCode_Txt2RawMaster( code, line, _source, &p1, &p2 );
      break;
    }
  }

  return 1;
}

uchar _armaxRawCode_Raw2TxtWr( char *line, void *_source, ulong *p1, ulong *p2 )
{
  char temp[9] = {0};
  schar i = 8, j = 8;
  snprintf( temp, 9, "%lX", *p1 );

  while ( i > -1 && !temp[i] )
  {
    --i;
  }

  for ( ; j > -1; --j, --i )
  {
    line[j] = ( i > -1 ) ? temp[i] : '0';
  }

  line[9] = line[8];
  line[8] = ' ';
  memset( temp, 0, 9 );
  snprintf( temp, 9, "%lX", *p1 );
  i = 8;

  while ( i > -1 && !temp[i] )
  {
    --i;
  }

  for ( j = 16; j > 8; --j, --i )
  {
    line[j] = ( i > -1 ) ? temp[i] : '0';
  }

  if ( _armaxRawCOM.WrLine( line, _source ) )
  {
    return 1;
  }

  return 0;
}
uchar _armaxRawCode_Raw2TxtWrite( CODE *code, char *line, void *_source,
                                  ulong *p1, ulong *p2  )
{
  switch ( code->size )
  {
  case 1:
    if ( code->loop > 0xffffff )
    {
      return 0;
    }

    *p2 = code->loop << 8;
    break;

  case 2:
    if ( code->loop > 0xffff )
    {
      return 0;
    }

    *p2 = code->loop << 16;
    break;

  case 8:
    *p2 = code->a[0]._ulong[0];
    _armaxRawCode_Raw2TxtWr( line, _source, p1, p2 );
    *p1 += 4;
    *p2 = code->a[0]._ulong[1];
    break;

  default:
    *p2 = code->a[0]._ulong[1];
  }

  return 1;
}
uchar _armaxRawCode_Raw2TxtLoop( CODE *code, char *line, void *_source,
                                 ulong *p1, ulong *p2  )
{
  if ( code->loop > 0xff || code->size > 4 ||
       code->a[1]._ulint[0] > 0xff ||
       code->addr[1] > 0xffff )
  {
    return 0;
  }

  *p2 = *p1 | 0x80000000;
  *p1 = 0;
  _armaxRawCode_Raw2TxtWr( line, _source, p1, p2 );
  *p1 = code->a[0]._ulong[1];
  *p2 = code->a[1]._ulong[1] << 24;
  *p2 |= code->loop << 16;
  *p2 |= ( ulong )code->addr[1];
  return 1;
}
uchar _armaxRawCode_Raw2TxtCmp( CODE *code, char *line, void *_source,
                                ulong *p1, ulong *p2  )
{
  // TODO: Implement _armaxRawCode_Raw2TxtCmp()
  return 0;
}
uchar HACK_LIB_EXP armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source )
{
  ulong p1 = 0, p2 = 0;

  if ( code->addr[0] > 0x1ffffff )
  {
    return 0;
  }

  if ( code->size <= 4 )
  {
    p1 = code->size;
    p1 <<= 24;
  }

  p1 |= ( ulong )code->addr[0];

  switch ( code->type )
  {
  case CODE_W:
    if ( code->a[1]._ulong[1] || ( code->loop && code->size == 4 ) )
    {
      _armaxRawCode_Raw2TxtLoop( code, line, _source, &p1, &p2 );
    }
    else
    {
      _armaxRawCode_Raw2TxtWrite( code, line, _source, &p1, &p2 );
    }

    break;

  case CODE_CMP:
  case CODE_JOKER:
    _armaxRawCode_Raw2TxtCmp( code, line, _source, &p1, &p2 );

  //break;
  default:
    return 0;
  }

  _armaxRawCode_Raw2TxtWr( line, _source, &p1, &p2 );
  return 1;
}

HACK_LIB_EXP HACK_COM* GetHackCOM( void )
{
  HACK_COM* com = &_armaxRawCOM;
  com->OnLoad  = armaxRawHack_OnLoad;
  com->OnSave  = armaxRawHack_OnSave;
  /*
  com->RdError = (_HACK_COM_GERR)ferror;
  com->RdLine  = (_HACK_COM_GETS)fgets;
  com->WrLine  = (_HACK_COM_PUTS)fputs;
  */
  com->H2T     = armaxRawHack_Raw2Txt;
  com->T2H     = armaxRawHack_Txt2Raw;
  com->C2T     = armaxRawCode_Raw2Txt;
  com->T2C     = armaxRawCode_Txt2Raw;
  return &_armaxRawCOM;
}
