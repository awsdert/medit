#include "search.h"

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
void siQry( ucv used, MEPIPE *mepI, MEPIPE *mepO, MEPIPE *mepP, Ipipe prev, scv bytes, ME_SHV *mecM, ME_SHV *mecN )
{
  DWORD cbRead = 0,
    shvSz = sizeof(shv),
    slvSz = sizeof(slv),
    sivSz = sizeof(siv),
    ssvSz = sizeof(ssv),
    scvSz = sizeof(scv);
  shv
    iNum = 0,
    pNum = 0;
  DWORD i = 0, c = BUFSIZ - bytes;
  ucv res = 0;
  scv offset = -( bytes - 1 );
  cbRead = IupRdPipe( mepI->pipe, mepI->buff, BUFSIZ );
  if ( !prev )
    memset( mepO->buff, UCHAR_MAX, BUFSIZ );
  do
  {
    if ( !prev )
      goto iterate;
    if ( i )
    {
      IupSkPipe( mepI->pipe, offset, FPOS_CUR );
      IupSkPipe( mepP->pipe, offset, FPOS_CUR );
    }
    IupRdPipe( prev, mepO->buff, BUFSIZ );
    if ( !mecM )
      goto old;
old:
    i = 0;
    do
    {
      res = mepO->buff[i];
      if ( !res )
        goto next;
      QCAST( shv )
      else QCAST( slv )
      else QCAST( siv )
      else QCAST( ssv )
      else QCAST( scv )
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
    cbRead = IupRdPipe( mepI->pipe, mepI->buff, BUFSIZ );
    IupWrPipe( mepO->pipe, mepO->buff, BUFSIZ );
  }
  while ( cbRead );
}
