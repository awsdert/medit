#include "search.h"
#define SEARCH( T, C ) \
  IupSkPipe( &mepI.pipe, 0, FPOS_SOF ); \
  strcpy_s( szPath, 10, szNow ); \
  strcat_s( szPath, 10, T##Ex ); \
  mepO.pipe = IupMkFile( szPath, 0666, 0, ACTION_OPEN_NEW, NULL ); \
  if ( qNo ) \
  { \
    if ( prev.pipe ) \
    { \
      IupShutPipe( &prev ); \
      IupShutPipe( &mepP.pipe ); \
    } \
    strcpy_s( szPath, 10, szOld ); \
    strcat_s( szPath, 10, T##Ex ); \
    prev = IupOpenFile( szPath, 0666, 0 ); \
  } \
  i = 0; \
  do \
  { \
    T##Qry( used & UCHAR_MAX, &mepI, &mepO, &mepP, prev, T##Sz[i], NULL, NULL ); \
    ++i; \
  } \
  while ( i < C ); \
  IupShutPipe( &mepO.pipe );

void search( usv used )
{
  Ipipe prev = {0};
  MEPIPE mepI = {{0}}, mepO = {{0}}, mepP = {{0}};
  ME_LPV fpMecM = {0}, fpMecN = {0};
  ME_SHV siMecM = {0}, siMecN = {0};
  ME_UHV uiMecM = {0}, uiMecN = {0};
  DWORD  isEof = 0;
  scv siSz[] =
    {
      sizeof(huge),
      sizeof(long),
      sizeof(int),
      sizeof(short),
      sizeof(char)
    }, *uiSz = siSz,
    fpSz[] =
    {
      sizeof(lpv),
      sizeof(dpv),
      sizeof(fpv)
    };
  int i = 0, c = 5, qNo = 0;
  char szDmp[4] = {0}, szPath[10] = {0}, szNow[10] = "q", szOld[10] = {0};
  char const
    fpEx[] = ".mef",
    siEx[] = ".mes",
    uiEx[] = ".meu";
  mepI.pipe = IupOpenFile( "test.xps", 0666, SHARE_READ );
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
  mepO.pipe = IupMkFile( szPath, 0666, 0, ACTION_OPEN_NEW, NULL );
  isEof = IupRdPipe( &mepI.pipe, mepI.buff, BUFSIZ );
  do
  {
    IupWrPipe( &mepO.pipe, mepO.buff, BUFSIZ );
    isEof = IupRdPipe( &mepI.pipe, mepI.buff, BUFSIZ );
  }
  while ( isEof );
  IupShutPipe( &mepI.pipe );
  IupShutPipe( &mepO.pipe );
  mepI.pipe = IupOpenFile( szPath, 0666, 0 );
  SEARCH( fp, 3 );
  SEARCH( si, 5 );
  SEARCH( ui, 5 );
  /* Close Down Pipes */
  if ( prev.pipe )
  {
    IupShutPipe( &prev );
    IupShutPipe( &mepP.pipe );
  }
  IupShutPipe( &mepI.pipe );
}
