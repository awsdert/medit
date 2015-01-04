#include "appData.h"
static char _cwp[ PATH_MAX ] = {0};
static char _cwd[ PATH_MAX ] = {0};
static char _exe[ NAME_MAX * 2 ] = {0};
static char _ver[ NAME_MAX * 2 ] = {0};
static char _name[ NAME_MAX ] = {0};

void  _appInitCwd( char *argv[], char *appName )
{
  char
    szSep[] = "\\/",
   *szTok = NULL,
   *szTmp = NULL,
   *szPrv = NULL,
   *szNxt = NULL;
  char path[ PATH_MAX ] = {0};
  strcpy_s( path, PATH_MAX, argv[0] );
  /* Capture CWD and APPNAME */
  szPrv = strtok_s( path, szSep, &szTok );
  szNxt = strtok_s( NULL, szSep, &szTok );
  do
  {
    szTmp = szPrv;
    szPrv = szNxt;
    szNxt = strtok_s( NULL, szSep, &szTok );
    strcat_s( _cwd, PATH_MAX, szTmp );
    strcat_s( _cwd, PATH_MAX, DIR_SEP  );
  }
  while ( szNxt );
  szTmp = strtok_s( szPrv, ".", &szTok );
  szNxt = strtok_s( szTmp, "-", &szTok );
  strcpy_s( _exe, NAME_MAX * 2, szNxt );
  szNxt = strtok_s( NULL,  DIR_SEP, &szTok );
  strcpy_s( _ver, NAME_MAX * 2, szNxt );
  strcpy_s( _name, NAME_MAX, appName );
}
char const* appGetCwp( void ) { return _cwp; }
char const* appGetCwd( void ) { return _cwd; }
char const* appGetExe( void ) { return _exe; }
char const* appGetVer( void ) { return _ver; }
char const* appGetName( void ) { return _name; }
