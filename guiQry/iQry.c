#include "qry.h"

#define _QCAST( NUM, T, BUFF ) \
  NUM = *((T*)&(BUFF[i]))
#define QCAST( T ) \
  if ( bytes == T##Sz ) \
  { \
    _QCAST( iNum, T, mepI->buff ); \
    _QCAST( pNum, T, mepP->buff ); \
  }
#define QCMP1( CMP_T ) res = ( iNum CMP_T pNum );
#define QCMP2( CMP_T ) res = (( iNum CMP_T pNum ) != pNum);
void iQry( uchar    used,
          ME_PIPE   *mepI,
          ME_PIPE   *mepO,
          ME_PIPE   *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_LINT  *mecM,
          ME_LINT  *mecN )
{
  DWORD cbRead = 0,
    lintSz  = sizeof(lint),
    longSz  = sizeof(long),
    intSz   = sizeof(int),
    shortSz = sizeof(short),
    scharSz = sizeof(char);
  lint
    iNum = 0,
    pNum = 0;
  ulong i = 0, c = BUFSIZ - bytes;
  uchar res = 0;
  schar offset = -( bytes - 1 );
  cbRead = ipRdPipe( &mepI->pipe, mepI->buff, BUFSIZ );
  if ( !prev.pipe )
    memset( mepO->buff, UCHAR_MAX, BUFSIZ );
  do
  {
    if ( !prev.pipe )
      goto iterate;
    if ( i )
    {
      ipSkPipe( &mepI->pipe, offset, FPOS_CUR );
      ipSkPipe( &mepP->pipe, offset, FPOS_CUR );
    }
    ipRdPipe( &prev, mepO->buff, BUFSIZ );
    if ( !mecM )
      goto old;
old:
    i = 0;
    do
    {
      res = mepO->buff[i];
      if ( !res )
        goto next;
      QCAST( lint )
      else QCAST( long  )
      else QCAST( int   )
      else QCAST( short )
      else QCAST( schar )
        c = 0;
      switch ( used )
      {
        case CMP_EQ: QCMP1( == ); break;
        case CMP_NE: QCMP1( != ); break;
        case CMP_MT: QCMP1( >  ); break;
        case CMP_ME: QCMP1( >= ); break;
        case CMP_LT: QCMP1( <  ); break;
        case CMP_LE: QCMP1( <= ); break;
        case CMP_IA: QCMP2( && ); break;
        default: res = 0;
      }
      if ( !res )
      {
        if ( mepO->buff[i] > bytes )
          mepO->buff[i] = bytes;
      }
next:
      ++i;
    }
    while ( i < c );
iterate:
    cbRead = ipRdPipe( &mepI->pipe, mepI->buff, BUFSIZ );
    ipWrPipe( &mepO->pipe, mepO->buff, BUFSIZ );
  }
  while ( cbRead );
}
