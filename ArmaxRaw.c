#include "ArmaxRawMain.h"

HACK_LIB_COM _hackCOM = {0};
CODE_LIB_COM _codeCOM = {0};

void  armaxRawHack_OnLoad( FILE *file, char const *dataDir )
{
  hack_t hi = 0;
  uchar ci = 0;
  size_t len = 0;
  int data = 0;
  HACK *hack;
  CODE *code;
  char line[80] = {0};
  char path[PATH_MAX] = {0};
  _hackCOM.RdLine = armaxRawHack_RdLine;
  _hackCOM.WrLine = armaxRawHack_WrLine;
  _hackCOM.ReSize( &_hackCOM.hl, NULL, 0 );
  for ( ; !ipEof( file ); ++hi )
  {
    if ( hi == _hackCOM.hl.hacks->_c )
      _hackCOM.ReSize( &_hackCOM.hl, NULL, hi + 10 );
    hack = &_hackCOM.hl.hacks->a[hi];
    while ( !line[0] || line[0] == ';' || line[0] == '\n' || line[0] == '\r' )
    {
      if ( ipEof( file ) )
      {
        _hackCOM.hl.hacks->c = hi;
        _hackCOM.hl.hacks->c = --hi;
        _codeCOM.codes->c = 0;
        return;
      }
      armaxRawHack_RdLine( line, file );
    }
    if ( line[0] == '"' || line[0] == '\'' )
      strcpy_s( line, 80, &line[1] );
    len = strlen( line );
    --len;
    if ( line[len] == '"' || line[len] == '\'' )
      line[len] = 0;
    strcpy_s( _hackCOM.hl.names[hi].a, NAME_MAX, line );
    armaxRawHack_RdLine( line, file );
    armaxRawHack_Txt2Raw( hack, line, file );
    _codeCOM.ReSize( &_codeCOM.codes, NULL, 0 );
    for ( ci = 0; !ipEof( file ); ++ci )
    {
      if ( ci == _codeCOM.codes->_c )
        _codeCOM.ReSize( &_codeCOM.codes, NULL, ci + 10 );
      code = &_codeCOM.codes->a[ci];
      armaxRawHack_RdLine( line, file );
      armaxRawCode_Txt2Raw( code, line, file );
    }
    _strset_s( line, 80, 0 );
    _ultoa_s( hi, line, 80, 16 );
    _strset_s( path, PATH_MAX, 0 );
    strcpy_s( path, PATH_MAX, dataDir );
    strcat_s( path, PATH_MAX, line );
    strcat_s( path, PATH_MAX, ".src-codes" );
    ipFdOpen( &data, path, IP_O_MKTEMP | IP_O_FILE, IP_D_RW, IP_A_RW );
    ipFdRdBuff( data, _codeCOM.codes, _codeCOM.codes->s + CODES_SIZE );
    ipFdShut( data );
  }
  _hackCOM.hl.hacks->c = hi;
  _hackCOM.hl.hacks->i = --hi;
}
void  armaxRawHack_OnSave( FILE *file, char const *dataDir )
{
  // TODO: Implement Armax Raw Saver
}
uchar armaxRawHack_RdLine( char  *line, void *_source )
{
  _strset_s( line, 80, 0 );
  if ( ipRdLineA( line, 80, (FILE*)_source ) )
    return 1;
  return 0;
}
uchar armaxRawHack_WrLine( char  *line, void *_source )
{
 if ( ipWrLineA( line, (FILE*)_source ) )
    return 1;
  return 0;
}

uchar armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source )
{
  ulint info = strtoull( line, NULL, 16 );
  char temp[6] = {0};
  uchar j = 0;
  ulong pid;
  size_t i = 0;
  // GameID
  for ( j = 0; j < 4; ++j, ++i )
    temp[j] = line[i];
  if ( !_hackCOM.gid )
    _hackCOM.gid = strtol( temp, NULL, 16 );
  else if ( _hackCOM.gid != strtol( temp, NULL, 16 ) )
    return 0;
  // HackID
  _strset_s( temp, 6, 0 );
  for ( j = 0; j < 5; ++j, ++i )
    temp[j] = line[i];
  if ( line[8] == ' ' )
  {
    temp[4] = line[9];
    ++i;
  }
  hack->id = strtoul( temp, NULL, 16 );
  // Region
  if ( !_hackCOM.region )
    _hackCOM.region = (line[i] == '1') ? REGION_EU :REGION_US;
  ++i;
  // List Type
  hack->irl = (line[i] == '2') ? 1 : 0;
  ++i;
  // Parent ID
  pid = strtoul( &line[i], NULL, 16 );
  for ( j = 0; j < _hackCOM.hl.hacks->c; ++j )
  {
    if ( _hackCOM.hl.hacks->a[j].id == pid )
    {
      hack->oi = j;
      return 1;
    }
  }
  // Let Parent ID be defined elsewhere
  return 1;
}
uchar armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source )
{
  schar i = 4, j = 4;
  char temp[6] = {0};
  // We start with GameID anyway so just shove it straight on
  _ultoa_s( _hackCOM.gid, temp, 6, 16 );
  while ( i > -1 && !temp[i] ) --i;
  for ( ; j > -1; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  // HackID
  _strset_s( temp, 6, 0 );
  _ultoa_s( hack->id, temp, 6, 16 );
  i = 5;
  while ( i > -1 && !temp[i] ) --i;
  for ( j = 8; j > 3; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  line[9] = line[8];
  line[8] = ' ';
  // Region
  line[10] = ( _hackCOM.region == REGION_US || !_hackCOM.region ) ? '0' : '1';
  // List Type
  line[11] = hack->fi ? hack->irl + '1' : '0';
  // Parent ID
  if ( hack->ci )
  {
    _strset_s( temp, 6, 0 );
    _ultoa_s( _hackCOM.hl.hacks->a[hack->oi].id, temp, 6, 16 );
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
  if ( _hackCOM.RdLine( line, _source ) )
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
  _hackCOM.region = ( *p2 & 0xFF ) ? REGION_EU : REGION_US;
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
uchar armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source )
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
  _ultoa_s( *p1, temp, 9, 16 );
  while ( i > -1 && !temp[i] ) --i;
  for ( ; j > -1; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  line[9] = line[8];
  line[8] = ' ';
  _strset_s( temp, 9, 0 );
  _ultoa_s( *p1, temp, 9, 16 );
  i = 8;
  while ( i > -1 && !temp[i] ) --i;
  for ( j = 16; j > 8; --j, --i )
    line[j] = (i > -1) ? temp[i] : '0';
  if ( _hackCOM.WrLine( line, _source ) )
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
uchar armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source )
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

ARMAX_RAW_EXP HACK_LIB_COM* GetHackFuncs( void ) { return &_hackCOM; }
ARMAX_RAW_EXP CODE_LIB_COM* GetCodeFuncs( void ) { return &_codeCOM; }
