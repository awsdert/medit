#include "qry.h"
#define SEARCH( T, C ) \
  ipSkPipe( &mepI.pipe, 0, FPOS_SOF ); \
  strcpy_s( szPath, 10, szNow ); \
  strcat_s( szPath, 10, T##Ex ); \
  mepO.pipe = ipMkFile( szPath, 0666, 0, ACTION_OPEN_NEW, NULL ); \
  if ( qNo ) \
  { \
    if ( prev.pipe ) \
    { \
      ipShutPipe( &prev ); \
      ipShutPipe( &mepP.pipe ); \
    } \
    strcpy_s( szPath, 10, szOld ); \
    strcat_s( szPath, 10, T##Ex ); \
    prev = ipOpenFile( szPath, 0666, 0 ); \
  } \
  i = 0; \
  do \
  { \
    T##Qry( used & UCHAR_MAX, &mepI, &mepO, &mepP, prev, T##Sz[i], NULL, NULL ); \
    ++i; \
  } \
  while ( i < C ); \
  ipShutPipe( &mepO.pipe );

void search( ushort used )
{
  Ipipe prev = {0};
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
  mepI.pipe = ipOpenFile( "test.xps", 0666, SHARE_READ );
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
  mepO.pipe = ipMkFile( szPath, 0666, 0, ACTION_OPEN_NEW, NULL );
  isEof = ipRdPipe( &mepI.pipe, mepI.buff, BUFSIZ );
  do
  {
    ipWrPipe( &mepO.pipe, mepO.buff, BUFSIZ );
    isEof = ipRdPipe( &mepI.pipe, mepI.buff, BUFSIZ );
  }
  while ( isEof );
  ipShutPipe( &mepI.pipe );
  ipShutPipe( &mepO.pipe );
  mepI.pipe = ipOpenFile( szPath, 0666, 0 );
  SEARCH( f, 3 );
  SEARCH( i, 5 );
  SEARCH( u, 5 );
  /* Close Down Pipes */
  if ( prev.pipe )
  {
    ipShutPipe( &prev );
    ipShutPipe( &mepP.pipe );
  }
  ipShutPipe( &mepI.pipe );
}
