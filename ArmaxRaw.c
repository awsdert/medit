#include "ArmaxRawMain.h"

HACK_FUNC _hackFuncArmaxRaw = {0};
CODE_FUNC _codeFuncArmaxRaw = {0};

long txt2rawHackArmaxRaw( HACK *raw, Ipipe *pipe )
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
  raw->id = strtoul( temp, NULL, 16 );
  /* Region ID */
  switch ( text[txt] )
  {
  case '1': raw->info = REG_EU; break;
  default:  raw->info = REG_US;
  }
  return gid;
}
void raw2txtHackArmaxRaw( HACK *raw, Ipipe *pipe, long gid )
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
  switch ( raw->info  & 0xF )
  {
    case REG_US: text[i] = '1'; break;
    case REG_EU: text[i] = '0'; break;
    default: return;
  }
  ++i;
  /* RadioList or CheckList */
  if ( !raw->first )
    text[i] = '0';
  else if ( raw->info & HACK_RADIOL )
    text[i] = '2';
  else
    text[i] = '1';
  ++i;
  /* Parent ID */
  memset( temp, 0, 5 );
  _ltoa_s( raw->pid, temp, 5, 16 );
  do
  {
    text[i] =  temp[j];
    ++i; ++j;
  }
  while ( j < 5 );
  /* Append/Insert Text */
  ipWrLine( pipe, text, 20, "\r\n" );
}
ssv  txt2rawCodeArmaxRawRead( ulv *cp1, ulv *cp2, CODELIST *cl, ssv *line )
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
usv txt2rawCodeArmaxRaw( CODE *raw, CODELIST* cl, usv line )
{
  long i = 0, j = 0;
  ulv  cp1 = 0, cp2 = 0, cp3 = 0, cp4 = 0;
  ulv tmp = 0;
  ucv nxt = 0, type = 0;
  if ( !_codeFuncArmaxRaw.getRamNo || !txt2rawCodeArmaxRawRead( &cp1, &cp2, cl, &line ) || ( !cp1 && !txt2rawCodeArmaxRawRead( &cp3, &cp4, cl, &line ) ) )
    /* Codelist unusable, move on to next hack */
    return cl->rows;
  type = cp1 >> 28;
  tmp = ( cp1 & 0xF000000 ) >> 24;
  nxt =  ( tmp >= 8 && tmp <= 0xD );
  if ( type >= 4 && type < 0xC )
    raw->type = CODE_JOKER;
  else
    raw->type = CODE_CMP;
  raw->loop = 1;
  if ( cp3 )
  switch ( type )
  {
    /* TODO: Add other code types once supported */
    case 0:
      if ( tmp <= 5 )
        raw->type = CODE_W;
      else if ( nxt )
        raw->info = CMP_EQ;
    break;
    case 1:
      if ( tmp <= 5 )
        raw->info = CMP_NE;
      else if ( nxt )
      {
        raw->sval = 1;
        raw->info = CMP_LT;
      }
    break;
    case 2:
      if ( tmp <= 5 )
      {
        raw->sval = 1;
        raw->info = CMP_MT;
      }
      else if ( nxt )
        raw->info = CMP_LT;
    break;
    case 3:
      if ( tmp <= 5 )
        raw->info = CMP_MT;
    break;
    case 4:
      if ( tmp <= 5 )
      {
        raw->type = CODE_W;
        raw->ptr  = 1;
      }
    break;
    case 8:
      if ( tmp <= 5 )
        raw->type = CODE_INC;
    break;
    case 0xC:
      if ( tmp <= 5 )
        raw->type = CODE_MASTER;
      else if ( nxt )
      {
        raw->info = CMP_NE;
        raw->loop = 0;
      }
    break;
    case 0xD:
      if ( tmp <= 5 )
      {
        raw->info = CMP_EQ;
        raw->loop = 0;
      }
      else if ( nxt )
      {
        raw->info = CMP_MT;
        raw->loop = 0;
        raw->sval = 1;
      }
    break;
    case 0xE:
      if ( tmp <= 5 )
      {
        raw->info = CMP_LT;
        raw->loop = 0;
        raw->sval = 0;
      }
      else if ( nxt )
      {
        raw->info = CMP_MT;
        raw->loop = 0;
      }
    break;
    case 0xF:
      if ( tmp <= 5 )
      {
        raw->info = CMP_LT;
        raw->loop = 0;
      }
  }
  switch ( tmp )
  {
    case 0: case 1: case 8:   case 9:   raw->size = 8;  break;
    case 2: case 3: case 0xA: case 0xB: raw->size = 16; break;
    default: raw->size = 32; break;
  }
  raw->addr[1] = ( cp1 & 0xFFFFFF );
  switch ( tmp )
  {
  case 1: case 3:   case 5:
  case 9: case 0xB: case 0xD:
    raw->addr[1] |= 0x1000000;
  }
  return line;
}
scv  _raw2txtCodeArmaxRaw( CODE* raw, Ipipe *pipe, ucv *I )
{
  ucv lines = 1, size = 4, repeat = 0, size2 = 0;
  ulv line[3][2] = {0}, val = 0;
  if ( raw->addr[0] > 0x1FFFFFF || raw->addr[1] > 0xFFFF || raw->loop > 0xFF ||
      ( raw->type = CODE_W && raw->loop && raw->buff[1] > 0xFF ) ||
      raw->tmem != _codeFuncArmaxRaw.getRamNo("ee") ) return 0;
  switch ( raw->size )
  {
    case 1:
      size = 0;
      val = raw->buff[*I] & 0xFF;
      raw->buff[*I] >>= 8;
    break;
    case 2: case 3:
      size = 2;
      val = raw->buff[*I] & 0xFFFF;
      raw->buff[*I] >>= 16;
    break;
    default:
      size = 4;
      val = raw->buff[*I] & 0xFFFFFFFF;
      raw->buff[*I] >>= 32;
  }
  raw->size -= size;
  repeat = ( raw->size > 0 );
  if ( raw->buff[*I] == 0 )
    ++(*I);
  line[0][0] = raw->addr[0];
  line[0][0] = (size << 24);
  switch ( raw->type )
  {
    case CODE_W:
      if ( !raw->loop )
        line[0][1] = val;
      else
      {
        line[0][1] = line[0][0] | 0x80000000;
        line[0][0] = 0;
        line[1][0] = val;
        /* Only list codes use the info parameter so is safe for checking */
        line[1][1] = raw->info ? 0 : ( ( raw->buff[1] & 0xFF ) << 24 );
        /* Increment Address, may need looking at */
        line[1][1] |= raw->addr[1];
        /* Ensure our next slide write starts at right address */
        raw->addr[0] += size;
      }
    break;
    case CODE_INC:
      if ( raw->loop )
        return -1;
      line[0][0] |= 0x10000000;
      line[0][1] = val;
    break;
    case CODE_DEC:
      if ( raw->loop )
        return -1;
      line[0][0] |= 0x20000000;
      line[0][1] = val;
    break;
    case CODE_CMP:
      line[0][0] |= 0xA0000000;
    break;
    case CODE_JOKER:
      line[0][0] |= 0xB0000000;
    break;
    case CODE_MASTER:
      line[0][0] |= 0xC0000000;
      line[0][1]  = raw->loop << 16;
    break;
    /* Unable to convert */
    default: return -1;
  }
  return repeat;
}
ucv raw2txtCodeArmaxRaw( CODE *raw, Ipipe *pipe )
{
  CODE tmp = *raw;
  ucv I = 0,
    /* This just prevents us doing more than we should */
    C = (raw->type == CODE_CMP) ? 1 : ( raw->info && 0xFF );
  scv res = 0;
  if ( C > 20 ) C = 20;
  do
  {
    res = _raw2txtCodeArmaxRaw( &tmp, pipe, &I );
    if ( res < 0 )
      /* Unsupported, Hack should be omitted from codelist */
      return 0;
  }
  while ( res == 1 && I < C );
  return 1;
}

ARMAX_RAW_EXP HACK_FUNC* GetHackFuncs( void ) { return &_hackFuncArmaxRaw; }
ARMAX_RAW_EXP CODE_FUNC* GetCodeFuncs( void ) { return &_codeFuncArmaxRaw; }
