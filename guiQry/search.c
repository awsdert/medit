#include "qry.h"
#define SEARCH( T, C ) \
  ipFdSetPos( mepI.fd, 0, IP_S_SET ); \
  copystri( szPath, szNow, 10 ); \
  appendstr( szPath, T##Ex, 10 ); \
  ipFdOpen( mepO.fd, szPath, IP_O_FILE | IP_O_RW, IP_D_RW ); \
  if ( qNo ) \
  { \
    if ( prev ) \
    { \
      ipFdShut( prev ); \
      ipFdShut( mepP.fd ); \
    } \
    copystri( szPath, szOld, 10 ); \
    appendstr( szPath, T##Ex, 10 ); \
    ipFdOpen( prev, szPath, IP_O_RW, IP_D_RW ); \
  } \
  i = 0; \
  do \
  { \
    T##Qry( used & UCHAR_MAX, &mepI, &mepO, &mepP, prev, T##Sz[i], NULL, NULL ); \
    ++i; \
  } \
  while ( i < C ); \
  ipFdShut( mepO.fd );

void search( ushort used )
{
  int prev = 0;
  ME_PIPE mepI = {0}, mepO = {0}, mepP = {0};
  DWORD  isEof = 0;
  schar iSz[] =
  {
    sizeof( lint ),
    sizeof( long ),
    sizeof( int ),
    sizeof( short ),
    sizeof( char )
  }, *uSz = iSz,
  fSz[] =
  {
    sizeof( lpn ),
    sizeof( dpn ),
    sizeof( fpn )
  };
  int i = 0,// c = 5,
      qNo = 0;
  char szDmp[4] = {0}, szPath[10] = {0}, szNow[10] = "q", szOld[10] = {0};
  char const
  fEx[] = ".mef",
          iEx[] = ".mes",
                  uEx[] = ".meu";
  ipFdOpen( mepI.fd, "test.xps", IP_O_RW, IP_D_RW );
  snprintf( szDmp, 4, "%i", qNo );
  appendstr( szNow, DIR_SEP, 10 );
  copystri( szOld, szNow, 10 );
  appendstr( szNow, szDmp, 10 );

  if ( qNo )
  {
    snprintf( szDmp, 4, "%i", qNo - 1 );
  }
  else
  {
    copystri( szDmp, "0", 4 );
  }

  appendstr( szOld, szDmp, 10 );
  copystri( szPath, szNow, 10 );
  appendstr( szPath, ".dmp", 10 );
  ipFdOpen( mepO.fd, szPath, IP_O_MKFILE | IP_O_RW, IP_D_RW );
  isEof = ipFdRdBuff( mepI.fd, mepI.buff, BUFSIZ );

  do
  {
    ipFdWrBuff( mepO.fd, mepO.buff, BUFSIZ );
    isEof = ipFdRdBuff( mepI.fd, mepI.buff, BUFSIZ );
  }
  while ( isEof );

  ipFdShut( mepI.fd );
  ipFdShut( mepO.fd );
  ipFdOpen( mepI.fd, szPath, IP_O_RW, IP_D_RW );
  SEARCH( f, 3 );
  SEARCH( i, 5 );
  SEARCH( u, 5 );

  /* Close Down Pipes */
  if ( prev )
  {
    ipFdShut( prev );
    ipFdShut( mepP.fd );
  }

  ipFdShut( mepI.fd );
}
