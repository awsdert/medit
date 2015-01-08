#include "qry.h"
#define SEARCH( T, C ) \
  ipFdSetPos( mepI.fd, 0, IP_S_SET ); \
  strcpy_s( szPath, 10, szNow ); \
  strcat_s( szPath, 10, T##Ex ); \
  ipFdOpenA( &mepO.fd, szPath, IP_O_FILE | IP_O_RW, IP_D_RW, IP_A_RW ); \
  if ( qNo ) \
  { \
    if ( prev ) \
    { \
      ipFdShut( prev ); \
      ipFdShut( mepP.fd ); \
    } \
    strcpy_s( szPath, 10, szOld ); \
    strcat_s( szPath, 10, T##Ex ); \
    ipFdOpenA( &prev, szPath, IP_O_RW, IP_D_RW, IP_A_RW ); \
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
  ME_PIPE   mepI   = {{0}}, mepO = {{0}}, mepP = {{0}};
  ME_LPN   fpMecM = {0}, fpMecN = {0};
  ME_LINT  siMecM = {0}, siMecN = {0};
  ME_ULINT uiMecM = {0}, uiMecN = {0};
  DWORD  isEof = 0;
  schar iSz[] =
    {
      sizeof(lint),
      sizeof(long),
      sizeof(int),
      sizeof(short),
      sizeof(char)
    }, *uSz = iSz,
    fSz[] =
    {
      sizeof(lpn),
      sizeof(dpn),
      sizeof(fpn)
    };
  int i = 0, c = 5, qNo = 0;
  char szDmp[4] = {0}, szPath[10] = {0}, szNow[10] = "q", szOld[10] = {0};
  char const
    fEx[] = ".mef",
    iEx[] = ".mes",
    uEx[] = ".meu";
  ipFdOpenA( &mepI.fd, "test.xps", IP_O_RW, IP_D_RW, IP_A_RW );
  _itoa_s( qNo, szDmp, 4, 10 );
  strcat_s( szNow, 10, DIR_SEP );
  strcpy_s( szOld, 10, szNow );
  strcat_s( szNow, 10, szDmp );
  if ( qNo )
    _itoa_s( qNo - 1, szDmp, 4, 10 );
  else
    strcpy_s( szDmp, 4, "0" );
  strcat_s( szOld, 10, szDmp );
  strcpy_s( szPath, 10, szNow );
  strcat_s( szPath, 10, ".dmp" );
  ipFdOpenA( &mepO.fd, szPath, IP_O_MKFILE | IP_O_RW, IP_D_RW, IP_A_RW );
  isEof = ipFdRdBuff( mepI.fd, mepI.buff, BUFSIZ );
  do
  {
    ipFdWrBuff( mepO.fd, mepO.buff, BUFSIZ );
    isEof = ipFdRdBuff( mepI.fd, mepI.buff, BUFSIZ );
  }
  while ( isEof );
  ipFdShut( mepI.fd );
  ipFdShut( mepO.fd );
  ipFdOpenA( &mepI.fd, szPath, IP_O_RW, IP_D_RW, IP_A_RW );
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
