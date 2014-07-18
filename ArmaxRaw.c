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
  if ( IupEof( pipe ) || IupRdLine( pipe, text, 20 ) == 0 )
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
  IupWrLine( pipe, text, 20, "\r\n" );
}
ucv  txt2rawCodeArmaxRawRead( ulv *cp1, ulv *cp2, Ipipe *pipe )
{
  char text[ 20 ] = {0};
  char temp[ 10 ] = {0};
  char i = 0, j = 0;
  if ( IupRdLine( pipe, text, 20 ) == 0 )
    return 0;
  do { temp[i] = text[i]; ++i; } while ( i < 8 );
  *cp1 = strtoul( temp, NULL, 16 );
  ++i;
  do { temp[j] = text[i]; ++i; ++j; } while ( j < 8 );
  *cp2 = strtoul( temp, NULL, 16 );
  return 1;
}
void txt2rawCodeArmaxRaw( CODE *raw, Ipipe *pipe )
{
  long i = 0, j = 0;
  ulv  cp1 = 0, cp2 = 0;
  ulv tmp = 0;
  ucv nxt = 0, type = 0;
  if ( !_codeFuncArmaxRaw.getRamNo || txt2rawCodeArmaxRawRead( &cp1, &cp2, pipe ) )
  {
    /* Force fail list as we cannot acquire necessary info */
    IupSkPipe( pipe, 0, SEEK_END );
    return;
  }
  type = cp1 >> 28;
  tmp = ( cp1 & 0xF000000 ) >> 24;
  nxt =  ( tmp >= 8 && tmp <= 0xD );
  if ( type >= 4 && type < 0xC )
    raw->type = CODE_JOKER;
  else
    raw->type = CODE_CMP;
  raw->loop = 1;
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
}
void raw2txtCodeArmaxRaw( CODE *raw, Ipipe *pipe )
{
  /* TODO: Implement Code 2 Armax Raw */
}

ARMAX_RAW_EXP HACK_FUNC* GetHackFuncs( void ) { return &_hackFuncArmaxRaw; }
ARMAX_RAW_EXP CODE_FUNC* GetCodeFuncs( void ) { return &_codeFuncArmaxRaw; }
