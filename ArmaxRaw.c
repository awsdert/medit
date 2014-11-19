#include "ArmaxRawMain.h"

HACK_FUNC _hackFuncArmaxRaw = {0};
CODE_FUNC _codeFuncArmaxRaw = {0};

long txt2rawHackArmaxRaw( HACK *raw, HACKS *hl, Ipipe *pipe )
{
  char text[ 20 ] = {0};
  char temp[  6 ] = {0};
  char t = 0;
  int tmp = 0, txt = 0;
  long gid = 0;
  if ( ipEof( pipe ) || ipRdLine( pipe, text, 20 ) == 0 )
    return -1;
  /* GameID */
  do
  {
    temp[tmp] = text[txt];
    ++tmp; ++txt;
  }
  while ( tmp < 4 );
  gid = strtol( temp, NULL, 16 );
  /* HackID */
  memset( temp, 0, 6 );
  do
  {
    temp[tmp] = text[txt];
    ++tmp; ++txt;
  }
  while ( tmp < 4 );
  if ( text[txt] == ' ' )
    ++txt;
  temp[4] = text[txt];
  ++txt;
  raw->id = strtoul( temp, NULL, 16 ) & UINT_LEAST16_MAX;
  return gid;
}
void raw2txtHackArmaxRaw( HACK *raw, HACKS* hl, Ipipe *pipe, long gid )
{
  char text[ 20 ] = {0};
  char temp[  6 ] = {0};
  int i = 0, j = 0;
  /* Game Id */
  if ( gid > 0xFFFF )
    return;
  _ltoa_s( gid, temp, 4, 16 );
  do
  {
    text[i] = temp[i];
    ++i;
  }
  while ( i < 4 );
  /* Hack ID */
  if ( raw->id > 0xFFFFF )
    return;
  memset( temp, 0, 4 );
  _ltoa_s( raw->id, temp, 5, 16 );
  do
  {
    text[i] = temp[j];
    ++i; ++j;
  }
  while ( j < 4 );
  text[i] = ' ';
  ++i; j = 0;
  text[i] = temp[4];
  ++i;
  /* Region */
  switch ( _codeFuncArmaxRaw.getRegion() )
  {
    case REGION_US: text[i] = '1'; break;
    case REGION_EU: text[i] = '0'; break;
    default: return;
  }
  ++i;
  /* RadioList or CheckList */
  if ( !raw->_fi )
    text[i] = '0';
  else if ( raw->radioList )
    text[i] = '2';
  else
    text[i] = '1';
  ++i;
  /* Parent ID */
  memset( temp, 0, 5 );
  _ltoa_s( hl->a[ raw->_pi ].id, temp, 5, 16 );
  do
  {
    text[i] =  temp[j];
    ++i; ++j;
  }
  while ( j < 5 );
  /* Append/Insert Text */
  ipWrLine( pipe, text, 20, "\r\n" );
}
ssv  txt2rawCodeArmaxRawRead( ulv *cp1, ulv *cp2, CODELIST *cl, usv *line )
{
  char *text = NULL;
  char temp[ 10 ] = {0};
  char i = 0, j = 0;
  if ( *line == cl->rows )
    return 0;
  text = cl->x[*line];
  do { temp[i] = text[i]; ++i; } while ( i < 8 );
  *cp1 = strtoul( temp, NULL, 16 );
  ++i;
  do { temp[j] = text[i]; ++i; ++j; } while ( j < 8 );
  *cp2 = strtoul( temp, NULL, 16 );
  ++(*line);
  return 1;
}
void _txt2rawCodeArmaxRawMaster( CODE *raw, CODELIST *cl, ulv p1, ulv p2 )
{
  raw->type = CODE_MASTER;
  if ( p2 & 0xFF )
    _codeFuncArmaxRaw.setRegion( REGION_EU );
  else
    _codeFuncArmaxRaw.setRegion( REGION_US );
  p2 >>= 8;
  raw->loop = p2 & 0xFF;
  p2 >>= 8;
  raw->addr[2] = p2;
}
void _txt2rawCodeArmaxRawWrite( CODE *raw, ulv p1, ulv p2 )
{
  raw->type = CODE_W;
  switch ( raw->dataSize )
  {
  case 1:
    raw->a[0].u = (p2 & 0xff);
    raw->loop = (p2 >>= 8);
    break;
  case 2:
    raw->a[0].u = (p2 & 0xffff);
    raw->loop = (p2 >>= 16);
    break;
  default:
    raw->a[0].u = p2;
    raw->loop = 0;
  }
}

void _txt2rawCodeArmaxRawAdd( CODE *raw, ulv p1, ulv p2 )
{
  raw->type = CODE_INC;
  switch ( raw->dataSize )
  {
  case 1:
    raw->a[0].u = (p2 & 0xFF);
    raw->loop = (p2 >>= 8);
    break;
  case 2:
    raw->a[0].u = (p2 & 0xFFFF);
    raw->loop = (p2 >>= 16);
    break;
  default:
    raw->a[0].u = p2;
    raw->loop = 0;
  }
}

void _txt2rawCodeArmaxRawPtr( CODE *raw, ulv p1, ulv p2 )
{
  raw->type = CODE_W;
  switch ( raw->dataSize )
  {
  case 1:
    raw->a[0].u = (p2 & 0xFF);
    raw->addr[2] = (p2 >>= 8);
    break;
  case 2:
    raw->a[0].u = (p2 & 0xFFFF);
    raw->addr[2] = (p2 >>= 16);
    break;
  default:
    raw->a[0].u = p2;
    raw->addr[2] = 0;
  }
}

void _txt2rawCodeArmaxRawTest( CODE *raw, CODELIST *cl, usv *line, ulv p1, ulv p2 )
{
  raw->type = CODE_CMP;
  raw->addr[0] = (p1 & 0xFFFFFF);
  raw->a[0].u = p2;
  p1 >>= 24;
  raw->loop = 1;
  raw->info = ((p1 & 6u) == 6u) ? CODE_JOKER : CODE_CMP;
  raw->dataSize = (p1 & 0x7);
  switch ( p1 & 0x38 )
  {
    case 0x10: raw->info = CMP_NE; break;
    case 0x18: raw->info = CMP_LT; raw->dataType = DATA_TYPE_SI; break;
    case 0x20: raw->info = CMP_MT; raw->dataType = DATA_TYPE_SI; break;
    case 0x28: raw->info = CMP_LT; break;
    case 0x30: raw->info = CMP_MT; break;
    case 0x38: raw->info = CMP_IA; break;
  }
  switch ( p1 & 0xC0 )
  {
    case 0x40: raw->loop = 2; break;
    case 0x80: raw->loop = 0; break;
  }
}

void _txt2rawCodeArmaxRawLoop( CODE *raw, CODELIST *cl, usv *line, ulv p2 )
{
  ulv p3 = 0, p4 = 0;
  raw->type = CODE_W;
  if ( txt2rawCodeArmaxRawRead( &p3, &p4, cl, line ) )
  {
    raw->addr[0] = (p2 & 0x1FFFFFF);
    raw->a[0].u = p3;
    raw->a[1].u = (p4 & 0xFFF);
    p2 >>= 25;
    p4 >>= 16;
    raw->dataSize = (p2 & 0x7);
    p2 >>= 3;
    raw->loop = (p4 & 0xFF);
  }
  else
    memset( raw, 0, sizeof( CODE ) );
}

usv txt2rawCodeArmaxRaw( CODE *raw, CODELIST* cl, usv line )
{
  long i = 0, j = 0;
  ulv  p1 = 0, p2 = 0;
  ulv tmp = 0;
  ucv nxt = 0, type = 0;
  if ( !_codeFuncArmaxRaw.getBaseNo || !_codeFuncArmaxRaw.setRegion ||
    !txt2rawCodeArmaxRawRead( &p1, &p2, cl, &line ) )
    /* Codelist unusable, move on to next hack */
    return cl->rows;
  else if ( p1 == 0xDEADC0DE )
    return line;
  else if ( !p1 )
  {
    if ( p2 & 0x80000000 )
      _txt2rawCodeArmaxRawLoop( raw, cl, &line, p2 );
    else
      _txt2rawCodeArmaxRawWrite( raw, p1, p2 );
  }
  else if ( p1 & 0x3C000000 )
    _txt2rawCodeArmaxRawTest( raw, cl, &line, p1, p2 );
  else
  {
    raw->addr[0] = (p1 & 0x1FFFFFF);
    p1 >>= 25;
    raw->dataSize = (p1 & 0x7);
    p1 >>= 3;
    switch ( p1 >> 30 )
    {
    case 0:   _txt2rawCodeArmaxRawWrite( raw, p1, p2 );  break;
    case 1:   _txt2rawCodeArmaxRawPtr( raw, p1, p2 );    break;
    case 2:   _txt2rawCodeArmaxRawAdd( raw, p1, p2 );    break;
    case 3: _txt2rawCodeArmaxRawMaster( raw, cl, p1, p2 ); break;
    }
  }
  return line;
}
void _newCodeLine( STR *line, STRA *cl )
{
  line->c = 20;
  line->s = line->c;
  line->a = (char*)malloc( line->s );
  memset( line->a, 0, line->s );
  cl->s += sizeof( STR );
  cl->a = (STR*)realloc( cl->a, cl->s );
  cl->a[ cl->c ] = *line;
  ++(cl->c);
}
void _fillCodeLine( STR *line, ulv p1, ulv p2 )
{
  _ultoa_s( p1,  line->a,     9, 16 );
  line->a[9] = ' ';
  _ultoa_s( p1, &line->a[10], 9, 16 );
}
ucv _raw2txtCodeArmaxRawWrite( CODE *raw, STRA *cl, CODE *c1, CODE *c2 )
{
  STR l1 = {0};
  ulv p1 = 0, p2 = 0, i = 0;
  switch ( raw->dataSize )
  {
    case 1:
      if ( raw->loop > 0xffffff )
        return 0;
      p2 = raw->loop << 8;
      break;
    case 2:
      if ( raw->loop > 0xffff )
        return 0;
      p2 = raw->loop << 16;
      break;
    case 8:
    if ( raw->info )
    {
      do
      {
         c1->a[i].u = raw->a[i].u & 0xffffffff;
         c2->a[i].u = raw->a[i].u >> 32;
        _raw2txtCodeArmaxRawWrite( &c1, cl );
        _raw2txtCodeArmaxRawWrite( &c2, cl );
        c1.addr[0] += 8;
        c2.addr[0] += 8;
      }
      while ( i < raw->info );
    }
    else
    {
      _raw2txtCodeArmaxRawWrite( &c1, cl, c1, c2 );
      _raw2txtCodeArmaxRawWrite( &c2, cl, c1, c2 );
    }
    return 1;
  }
  p1 = raw->addr[0] & 0x1ffffff;
  p1 |= ((ulv)raw->dataSize << 24);
  p2 |= (ulv)raw->a[0].u;
  _newCodeLine( &l1, cl );
  _fillCodeLine( &l1, p1, p2 );
  return 1;
}
ucv _raw2txtCodeArmaxRawLoop( CODE *raw, STRA *cl, CODE *c1, CODE *c2 )
{
  ulv p1 = 0, p2 = 0, p3 = 0, p4 = 0, i = 0;
  STRA l1 = {0}, l2 = {0};
  _newCodeLine( &l1, cl );
  _newCodeLine( &l2, cl );
  if ( raw->loop > 0xff )
    return 0;
  if ( raw->dataSize == 8 )
  {
    if ( raw->info )
    {
      c1->a[1].u = 0;
      c2->a[1].u = 0;
      do
      {
        c1->a[0].u = (ulv)raw->a[i].u;
        c2->a[0].u = (ulv)(raw->a[i].u >> 32);
        _raw2txtCodeArmaxRawLoop( c1, cl, c1, c2 );
        _raw2txtCodeArmaxRawLoop( c2, cl, c1, c2 );
        c1->addr[0] += 8;
        c2->addr[0] += 8;
        ++i;
      }
      while ( i < raw->info );
    }
    else
    {
      _raw2txtCodeArmaxRawLoop( c1, cl, c1, c2 );
      _raw2txtCodeArmaxRawLoop( c2, cl, c1, c2 );
    }
    return 1;
  }
  p2 = (ulv)raw->addr[0];
  p2 |= 0x80000000;
  if ( raw->dataSize > 1 )
    p2 |= ((ulv)raw->dataSize) << 24;
  p3 = (ulv)raw->a[0].u;
  p4 = (ulv)raw->addr[1];
  p4 |= raw->loop << 16;
  p4 |= (ulv)(raw->a[1].u << 24);
  _fillCodeLine( &l1, p1, p2 );
  _fillCodeLine( &l2, p3, p3 );
  return 1;
}
ucv raw2txtCodeArmaxRaw( CODE *raw, STRA *cl )
{
  CODE c1 = *raw, c2 = *raw;
  if ( raw->addr > 0x1ffffff || raw->dataType )
    return 0;
  c1.info = 0;
  c1.dataSize = 4;
  c1.a[0].u = (ulv)raw->a[0].u;
  c1.a[1].u = (ulv)raw->a[1].u;
  c2.info = 0;
  c2.dataSize = 4;
  c2.a[0].u = (ulv)(raw->a[0].u >> 32);
  c2.a[1].u = (ulv)(raw->a[1].u >> 32);
  c2.addr[0] += 4;
  switch ( raw->type )
  {
    case CODE_W:
      if ( raw->loop && ( raw->dataSize >= 4 || raw->a[1].u ) )
        return _raw2txtCodeArmaxRawLoop( raw, cl );
      else if ( !raw->addr[2] )
        return _raw2txtCodeArmaxRawWrite( raw, cl );
    break;
    case CODE_INC: break;
    case CODE_CMP: case CODE_JOKER: break;
    case CODE_MASTER: break;
  }
}

ARMAX_RAW_EXP HACK_FUNC* GetHackFuncs( void ) { return &_hackFuncArmaxRaw; }
ARMAX_RAW_EXP CODE_FUNC* GetCodeFuncs( void ) { return &_codeFuncArmaxRaw; }
