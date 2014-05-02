#include "pipe.h"
static char  szCwd[ FILENAME_MAX ] = {0};
static char  szAppName[ FILENAME_MAX ] = {0};
static FILE *hLang = NULL;
FILE* IupFOpen( char const *szPathFromCwd, char const *szMode );
typedef struct _MECFG
{
  char lang[7];
} MECFG;
int main( int argc, char *argv[] )
{
  MECFG cfg = {0};
  int ret = 0;
  Ihandle *hMain, *hArea, *hProc;
  FILE *hMedit = NULL;
  MELANG *lang = meGetLang();
  Ipipe   pipe = NULL;
  char // Open in "rb,ccs=UTF-16LE"
    szSep[] = "\\/",
    szLine[ FILENAME_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  szPrv = strtok_s( argv[0], szSep, &szTok );
  szNow = strtok_s( NULL,   szSep, &szTok );
  do
  {
    szTmp = szPrv;
    szPrv = szNow;
    szNow = strtok_s( NULL, szSep, &szTok );
    strcat_s( szCwd, FILENAME_MAX, szTmp );
    strcat_s( szCwd, FILENAME_MAX, "\\"  );
  }
  while ( szNow );
  szTmp = strtok_s( szPrv, ".", &szTok );
  strcat_s( szAppName, FILENAME_MAX, szTmp );
  pipe = IupMkDir( "", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  IupShutPipe( pipe );
  pipe = IupMkDir( "lang", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  IupShutPipe( pipe );
  pipe = IupMkFile("medit.mecfg", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  if ( !pipe )
    goto mkGui;
  IupRdPipe( pipe, &cfg, sizeof(MECFG) );
  cfg.lang[6] = 0; // Just force a NULL character
  IupShutPipe( pipe );
  if ( cfg.lang[0] && _strcmpi( cfg.lang, "en" ) >= 0 )
  {
    memset(   szLine, 0, FILENAME_MAX );
    strcat_s( szLine,    FILENAME_MAX, "lang"   );
    strcat_s( szLine,    FILENAME_MAX, DIR_SEP  );
    strcat_s( szLine,    FILENAME_MAX, cfg.lang );
    strcat_s( szLine,    FILENAME_MAX, ".melng" );
    pipe = IupMkFile( szLine, 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
    IupRdPipe( pipe, lang, sizeof(MELANG) );
    IupShutPipe( pipe );
  }
  else
  {
    memset( cfg.lang, 0, 7 );
    cfg.lang[0] = 'e';
    cfg.lang[1] = 'n';
  }
mkGui:
  hProc = IupFrame(NULL);
  hArea = IupList(NULL);
  IupSetAttribute( hArea, "DROPDOWN", "YES" );
  IupSetAttribute( hArea, "EXPAND", "HORIZONTAL" );
  IupSetAttribute( hArea, "SIZE", "300x20" );
  IupSetAttribute( hArea, "1", lang->mainList.tar );
  IupSetAttribute( hArea, "2", lang->mainList.pro );
  IupSetAttribute( hArea, "3", lang->mainList.sea );
  IupSetAttribute( hArea, "4", lang->mainList.res );
  IupSetAttribute( hArea, "5", lang->mainList.edi );
  IupSetAttribute( hArea, "6", lang->mainList.hac );
  IupSetAttribute( hArea, "7", lang->mainList.abo );
  hMain = IupDialog( IupVbox( hArea ) );
  IupSetAttribute( hMain, "TITLE", "Medit" );
  IupSetAttribute( hMain, "SIZE",  "320x320"  );
  IupShow( hMain );
  ret = IupMainLoop();
  pipe = IupMkFile("medit.mecfg", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  IupWrPipe( pipe, &cfg, sizeof( MECFG ) );
  IupShutPipe( pipe );
  IupClose();
  return ret;
}
char const* IupGetCwd( void )
{
  return szCwd;
}
char const* IupGetAppName( void )
{
  return szAppName;
}
void IupGetLDataDirFromName( char *path, DWORD size )
{
  IupGetLDataDir( path );
  strcat_s( path, size, DIR_SEP );
  strcat_s( path, size, IupGetAppName() );
  strcat_s( path, size, DIR_SEP );
}
FILE* IupFOpen( char const *szPathFromCwd, char const *szMode )
{
  FILE *file = NULL;
  char szPath[ FILENAME_MAX ] = {0};
  IupGetLDataDirFromName( szPath, FILENAME_MAX );
  strcat_s( szPath, FILENAME_MAX, szPathFromCwd );
  if ( fopen_s( &file, szPath, szMode ) == 0 )
    return file;
  if ( file )
    fclose( file );
  return NULL;
}
