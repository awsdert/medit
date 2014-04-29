#include "lang.h"
static char  szCwd[ FILENAME_MAX ] = {0};
static FILE *hLang = NULL;
FILE* IupFOpen( char const *szPathFromCwd, char const *szMode );

int main( int argc, char *argv[] )
{
  int ret = 0;
  Ihandle *hMain, *hArea, *hProc;
  FILE *hMedit = NULL;
  MELANG *lang = meGetLang();
  char // Open in "rb,ccs=UTF-16LE"
    szLang[ FILENAME_MAX ] = {0}, szSep[] = "\\/",
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
  hMedit = IupFOpen( "medit.ini", "rb" );
  fgets( szLine, FILENAME_MAX, hMedit );
  memset( szLang, 0, FILENAME_MAX );
  szTok = NULL;
  szPrv = strtok_s( szLine, "=", &szTok );
  szNow = strtok_s( NULL,   "=", &szTok );
  if ( _strcmpi( szPrv, "lang" ) >= 0 && szNow && szNow[0] )
    strcat_s( szLang, FILENAME_MAX, szNow );
    /*
      We'll finish with current file before opening another,
      this just puts the string to one side and lets us know
      later on if we have a file to work with
    */
  fclose( hMedit );
  hMedit = NULL;
  if ( szLang[0] && _strcmpi( szLang, "en" ) < 0 )
  {
    hLang = IupFOpen( szLang, "rb" );
    fread( lang, sizeof( MELANG ), 1, hLang );
    fclose( hLang );
    hLang = NULL;
  }
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
  IupClose();
  return ret;
}
char const* IupCwd( void )
{
  return szCwd;
}
FILE* IupFOpen( char const *szPathFromCwd, char const *szMode )
{
  FILE *file = NULL;
  char szPath[ FILENAME_MAX ] = {0};
  strcat_s( szPath, FILENAME_MAX, szCwd );
  strcat_s( szPath, FILENAME_MAX, szPathFromCwd );
  if ( fopen_s( &file, szPath, szMode ) == 0 )
    return file;
  if ( file )
    fclose( file );
  return NULL;
}
