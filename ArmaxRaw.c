#include "ArmaxRawMain.h"

HACK_COM _armaxRawCOM = {0};

void  HACK_LIB_EXP armaxRawHack_OnLoad( FILE *file, char const *dataDir )
{
  hack_t hi = 0;
  uchar ci = 0;
  size_t len = 0;
  int data = 0;
  HACK *hack;
  CODE *code;
  char line[80] = {0};
  char path[PATH_MAX] = {0};
  _armaxRawCOM.RdLine = armaxRawHack_RdLine;
  _armaxRawCOM.WrLine = armaxRawHack_WrLine;
  _armaxRawCOM.ResizeHacks( &_armaxRawCOM.hl, NULL, 0 );
  for ( ; !ipEof( file ); ++hi )
  {
    if ( hi == _armaxRawCOM.hl.hacks->_c )
      _armaxRawCOM.ResizeHacks( &_armaxRawCOM.hl, NULL, hi + 10 );
    hack = &_armaxRawCOM.hl.hacks->a[hi];
    while ( !line[0] || line[0] == ';' || line[0] == '\n' || line[0] == '\r' )
    {
      if ( ipEof( file ) )
      {
        _armaxRawCOM.hl.hacks->c = hi;
        _armaxRawCOM.hl.hacks->c = --hi;
        _armaxRawCOM.cl->c = 0;
        return;
      }
      armaxRawHack_RdLine( line, file );
    }
    if ( line[0] == '"' || line[0] == '\'' )
      strncpy( line, &line[1], 80 );
    len = strlen( line );
    --len;
    if ( line[len] == '"' || line[len] == '\'' )
      line[len] = 0;
    strncpy( _armaxRawCOM.hl.names[hi].a, line, NAME_MAX );
    armaxRawHack_RdLine( line, file );
    armaxRawHack_Txt2Raw( hack, line, file );
    _armaxRawCOM.ResizeCodes( &_armaxRawCOM.cl, NULL, 0 );
    for ( ci = 0; !ipEof( file ); ++ci )
    {
      if ( ci == _armaxRawCOM.cl->_c )
        _armaxRawCOM.ResizeCodes( &_armaxRawCOM.cl, NULL, ci + 10 );
      code = &_armaxRawCOM.cl->a[ci];
      armaxRawHack_RdLine( line, file );
      armaxRawCode_Txt2Raw( code, line, file );
    }
    strnset( line, 0, 80 );
    snprintf( line, 80, "%X", hi );
    strnset( path, 0, PATH_MAX );
    strncpy( path, dataDir, PATH_MAX );
    strncat( path, line, PATH_MAX );
    strncat( path, ".src-codes", PATH_MAX );
    if ( _access( path, 0 ) != 0 )
      _creat( path, _S_IREAD | _S_IWRITE );
    ipFdOpen( data, path, IP_O_MKTEMP | IP_O_FILE, IP_D_RW );
    ipFdRdBuff( data, _armaxRawCOM.cl, _armaxRawCOM.cl->s + CODES_SIZE );
    ipFdShut( data );
  }
  _armaxRawCOM.hl.hacks->c = hi;
  _armaxRawCOM.hl.hacks->i = --hi;
}
void  HACK_LIB_EXP armaxRawHack_OnSave( FILE *file, char const *dataDir )
{
  // TODO: Implement Armax Raw Saver
}
uchar HACK_LIB_EXP armaxRawHack_RdLine( char  *line, void *_source )
{
  strnset( line, 0, 80 );
  if ( ipRdLineA( line, 79, (FILE*)_source ) )
    return 1;
  return 0;
}
uchar HACK_LIB_EXP armaxRawHack_WrLine( char  *line, void *_source )
{
 if ( ipWrLineA( line, (FILE*)_source ) )
    return 1;
  return 0;
}

uchar HACK_LIB_EXP armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source )
{
  char temp[6] = {0};
  uchar j = 0;
  ulong pid;
  size_t i = 0;
  // GameID
  for ( j = 0; j < 4; ++j, ++i )
    temp[j] = line[i];
  if ( !_armaxRawCOM.gid )
    _armaxRawCOM.gid = (ushort)strtoul( temp, NULL, 16 );
  else if ( _armaxRawCOM.gid != strtol( temp, NULL, 16 ) )
    return 0;
  // HackID
  strnset( temp, 0, 6 );
  for ( j = 0; j < 5; ++j, ++i )
    temp[j] = line[i];
  if ( line[8] == ' ' )
  {
    temp[4] = line[9];
    ++i;
  }
  hack->id = strtoul( temp, NULL, 16 );
  // Region
  if ( !_armaxRawCOM.reg )
    _armaxRawCOM.reg = (line[i] == '1') ? REGION_EU :REGION_US;
  ++i;
  // List Type
  hack->irl = (line[i] == '2') ? 1 : 0;
  ++i;
  // Parent ID
  pid = strtoul( &line[i], NULL, 16 );
  for ( j = 0; j < _armaxRawCOM.hl.hacks->c; ++j )
  {
    if ( _armaxRawCOM.hl.hacks->a[j].id == pid )
    {
      hack->oi = j;
      return 1;
    }
  }
  // Let Parent ID be defined elsewhere
  return 1;
}
uchar HACK_LIB_EXP armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source )
{
  schar i = 4, j = 4;
  char temp[6] = {0};
  // We start with GameID anyway so just shove it straight on
  snprintf( temp, 6, "%X", _armaxRawCOM.gid );
  while ( i > -1 && !temp[i] ) --i;
  for ( ; j > -1; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  // HackID
  strnset( temp, 0, 6 );
  snprintf( temp, 6, "%lX", hack->id );
  i = 5;
  while ( i > -1 && !temp[i] ) --i;
  for ( j = 8; j > 3; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  line[9] = line[8];
  line[8] = ' ';
  // Region
  line[10] = ( _armaxRawCOM.reg == REGION_US || !_armaxRawCOM.reg ) ? '0' : '1';
  // List Type
  line[11] = hack->fi ? hack->irl + '1' : '0';
  // Parent ID
  if ( hack->ci )
  {
    strnset( temp, 0, 6 );
    snprintf( temp, 6, "%lX", _armaxRawCOM.hl.hacks->a[hack->oi].id );
    i = 5;
    while ( i > -1 && !temp[i] ) --i;
  }
  else i = -1;
  for ( j = 16; j > 11; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  return 1;
}
uchar  _armaxRawCode_Txt2RawRd( char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  uchar r = 1;
  if ( _armaxRawCOM.RdLine( line, _source ) )
    *info = strtoull( line, NULL, 16 );
  else
  {
    *info = LI( 0u );
    r = 0;
  }
  *p1 = (ulong)(*info >> 32);
  *p2 = (ulong)*info;
  return r;
}
void _armaxRawCode_Txt2RawMaster( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->type = CODE_MASTER;
  _armaxRawCOM.reg = ( *p2 & 0xFF ) ? REGION_EU : REGION_US;
  *p2 >>= 8;
  code->loop = *p2 & 0xFF;
  *p2 >>= 8;
  code->addr[2] = *p2;
}
void _armaxRawCode_Txt2RawWrite( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->type = CODE_W;
  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = (*p2 & 0xFF);
    code->loop = (*p2 >>= 8);
    break;
  case 2:
    code->a[0]._ulint[0] = (*p2 & 0xFFFF);
    code->loop = (*p2 >>= 16);
    break;
  default:
    code->a[0]._ulint[0] = *p2;
  }
}

void _armaxRawCode_Txt2RawInc( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->type = CODE_INC;
  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = (*p2 & 0xFF);
    code->loop = (*p2 >>= 8);
    break;
  case 2:
    code->a[0]._ulint[0] = (*p2 & 0xFFFF);
    code->loop = (*p2 >>= 16);
    break;
  default:
    code->a[0]._ulint[0] = *p2;
  }
}
void _armaxRawCode_Txt2RawPtr( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->type = CODE_W;
  switch ( code->size )
  {
  case 1:
    code->a[0]._ulint[0] = (*p2 & 0xFF);
    code->addr[2] = (*p2 >>= 8);
    break;
  case 2:
    code->a[0]._ulint[0] = (*p2 & 0xFFFF);
    code->addr[2] = (*p2 >>= 16);
    break;
  default:
    code->a[0]._ulint[0] = *p2;
  }
}

void _armaxRawCode_Txt2RawCmp( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->a[0]._ulint[0] = *p2;
  *p1 >>= 24;
  code->type = ((*p1 & 6u) == 6u) ? CODE_JOKER : CODE_CMP;
  switch ( *p1 & 0x38 )
  {
    case 0x10: code->info = CMP_NE; break;
    case 0x18: code->info = CMP_LT; code->format = 'i'; break;
    case 0x20: code->info = CMP_MT; code->format = 'i'; break;
    case 0x28: code->info = CMP_LT; break;
    case 0x30: code->info = CMP_MT; break;
    case 0x38: code->info = CMP_IA; break;
    default:   code->info = CMP_EQ;
  }
  switch ( *p1 & 0xC0 )
  {
    case 0x40: code->loop = 2; break;
    case 0x80: code->loop = 0; break;
    default:   code->loop = 1;
  }
}
void _armaxRawCode_Txt2RawLoop( CODE *code, char *line, void *_source, ulint *info, ulong *p1, ulong *p2 )
{
  code->type = CODE_W;
  code->addr[0] = *p2 & 0x1FFFFFFF;
  switch ( (*p2 >> 24) & 0xE )
  {
    case 0: code->size = 1; break;
    case 2: code->size = 2; break;
    case 4: code->size = 4; break;
  }
  if ( !_armaxRawCode_Txt2RawRd( line, _source, info, code->a[0]._ulong, p2 ) )
    return;
  code->a[1]._ulong[0] = *p2 >> 24;
  code->loop = (*p2 >> 16) & 0xFF;
  code->addr[1] = *p2 & 0xFFFFF;
}
uchar HACK_LIB_EXP armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source )
{
  ulint info = strtoull( line, NULL, 16 );
  ulong p1 = (ulong)(info >> 32), p2 = (ulong)info;
  if ( p1 == 0xDEADC0DE )
    return 0;
  else if ( !p1 )
  {
    if ( p2 & 0x80000000 )
      _armaxRawCode_Txt2RawLoop( code, line, _source, &info, &p1, &p2 );
    else
    {
      code->size = 1;
      _armaxRawCode_Txt2RawWrite( code, line, _source, &info, &p1, &p2 );
    }
  }
  else if ( p1 & 0x3C000000 )
    _armaxRawCode_Txt2RawCmp( code, line, _source, &info, &p1, &p2 );
  else
  {
    code->addr[0] = (p1 & 0x1FFFFFF);
    switch ( (p1 >> 24) & 0xE )
    {
      case 0: code->size = 1; break;
      case 2: code->size = 2; break;
      case 4: code->size = 4; break;
    }
    switch ( p1 >> 30 )
    {
    case 0: _armaxRawCode_Txt2RawWrite(  code, line, _source, &info, &p1, &p2 ); break;
    case 1: _armaxRawCode_Txt2RawPtr(    code, line, _source, &info, &p1, &p2 ); break;
    case 2: _armaxRawCode_Txt2RawInc(    code, line, _source, &info, &p1, &p2 ); break;
    case 3: _armaxRawCode_Txt2RawMaster( code, line, _source, &info, &p1, &p2 ); break;
    }
  }
  return 1;
}

uchar _armaxRawCode_Raw2TxtWr( char *line, void *_source, ulong *p1, ulong *p2 )
{
  char temp[9] = {0};
  schar i = 8, j = 8;
  snprintf( temp, 9, "%lX", *p1 );
  while ( i > -1 && !temp[i] ) --i;
  for ( ; j > -1; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  line[9] = line[8];
  line[8] = ' ';
  strnset( temp, 0, 9 );
  snprintf( temp, 9, "%lX", *p1 );
  i = 8;
  while ( i > -1 && !temp[i] ) --i;
  for ( j = 16; j > 8; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  if ( _armaxRawCOM.WrLine( line, _source ) )
    return 1;
  return 0;
}
uchar _armaxRawCode_Raw2TxtWrite( CODE *code, char *line, void *_source, ulong *p1, ulong *p2  )
{
  switch ( code->size )
  {
  case 1:
    if ( code->loop > 0xffffff )
      return 0;
    *p2 = code->loop << 8;
    break;
  case 2:
    if ( code->loop > 0xffff )
      return 0;
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
uchar _armaxRawCode_Raw2TxtLoop( CODE *code, char *line, void *_source, ulong *p1, ulong *p2  )
{
  if ( code->loop > 0xff || code->size > 4 ||
      code->a[1]._ulint[0] > 0xff ||
      code->addr[1] > 0xffff )
    return 0;
  *p2 = *p1 | 0x80000000;
  *p1 = 0;
  _armaxRawCode_Raw2TxtWr( line, _source, p1, p2 );
  *p1 = code->a[0]._ulong[1];
  *p2 = code->a[1]._ulong[1] << 24;
  *p2 |= code->loop << 16;
  *p2 |= (ulong)code->addr[1];
  return 1;
}
uchar _armaxRawCode_Raw2TxtCmp( CODE *code, char *line, void *_source, ulong *p1, ulong *p2  )
{
  // TODO: Implement _armaxRawCode_Raw2TxtCmp()
  return 0;
}
uchar HACK_LIB_EXP armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source )
{
  ulong p1 = 0, p2 = 0;
  if ( code->addr[0] > 0x1ffffff )
    return 0;
  if ( code->size <= 4 )
  {
    p1 = code->size;
    p1 <<= 24;
  }
  p1 |= (ulong)code->addr[0];
  switch ( code->type )
  {
  case CODE_W:
    if ( code->a[1]._ulong[1] || (code->loop && code->size == 4) )
      _armaxRawCode_Raw2TxtLoop( code, line, _source, &p1, &p2 );
    else
      _armaxRawCode_Raw2TxtWrite( code, line, _source, &p1, &p2 );
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
  com->RdLine  = armaxRawHack_RdLine;
  com->OnSave  = armaxRawHack_OnSave;
  com->WrLine  = armaxRawHack_WrLine;
  com->H2T     = armaxRawHack_Raw2Txt;
  com->T2H     = armaxRawHack_Txt2Raw;
  com->C2T     = armaxRawCode_Raw2Txt;
  com->T2C     = armaxRawCode_Txt2Raw;
  return &_armaxRawCOM;
}
