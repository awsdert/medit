#include "qry.h"

#define QCMP1( CMP_T ) res =  ( iNum CMP_T pNum )
#define QCMP2( CMP_T ) res = (( iNum CMP_T pNum ) == iNum)
#define QCMP3( CMP_T ) res = (( iNum CMP_T pNum ) != iNum)

void iQry( uchar    used,
           ME_PIPE  *mepI,
           ME_PIPE  *mepO,
           ME_PIPE  *mepP,
           int       prev,
           schar    bytes,
           ME_LINT  *mecM,
           ME_LINT  *mecN )
{
  size_t cbRead = 0;
  lint
  iNum = 0,
  pNum = 0;
  ulong i = 0, c = BUFSIZ - bytes;
  uchar res = 0;
  schar offset = -( bytes - 1 );
  cbRead = ipFdRdBuff( mepI->fd, mepI->buff, BUFSIZ );

  if ( !prev )
  {
    memset( mepO->buff, UCHAR_MAX, BUFSIZ );
  }

  do
  {
    if ( !prev )
    {
      goto iterate;
    }

    if ( i )
    {
      ipFdSetPos( mepI->fd, offset, IP_S_CUR );
      ipFdSetPos( mepP->fd, offset, IP_S_CUR );
    }

    ipFdRdBuff( prev, mepO->buff, BUFSIZ );

    if ( !mecM )
    {
      goto old;
    }

old:
    i = 0;

    do
    {
      res = mepO->buff[i];

      if ( !res )
      {
        goto next;
      }

      memcpy( &iNum, &mepI->buff[i], bytes );
      memcpy( &pNum, &mepP->buff[i], bytes );

      switch ( used )
      {
      case CMP_EQ:
        QCMP1( == );
        break;

      case CMP_NE:
        QCMP1( != );
        break;

      case CMP_MT:
        QCMP1( >  );
        break;

      case CMP_ME:
        QCMP1( >= );
        break;

      case CMP_LT:
        QCMP1( <  );
        break;

      case CMP_LE:
        QCMP1( <= );
        break;

      case CMP_IA:
        QCMP2( && );
        break;

      default:
        res = 0;
      }

      if ( !res )
      {
        if ( mepO->buff[i] > bytes )
        {
          mepO->buff[i] = bytes;
        }
      }

next:
      ++i;
    }
    while ( i < c );

iterate:
    cbRead = ipFdRdBuff( mepI->fd, mepI->buff, BUFSIZ );
    ipFdWrBuff( mepO->fd, mepO->buff, BUFSIZ );
  }
  while ( cbRead );
}
