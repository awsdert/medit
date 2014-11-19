#include "appCfg.h"
static char _cwp[ PATH_MAX ] = {0};
static char _cwd[ PATH_MAX ] = {0};
static char _exe[ NAME_MAX ] = {0};
static char _ver[ NAME_MAX ] = {0};

void  _appInitCwd( char *argv[] )
{
  char
    szSep[] = "\\/",
   *szTok = NULL,
   *szTmp = NULL,
   *szPrv = NULL,
   *szNxt = NULL;
  /* Capture CWD and APPNAME */
  szPrv = strtok_s( argv[0], szSep, &szTok );
  szNxt = strtok_s( NULL,    szSep, &szTok );
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
  strcpy_s( _exe, 25, szTmp );
  szNxt = strtok_s( szTmp, "-", &szTok );
  szNxt = strtok_s( NULL,  DIR_SEP, &szTok );
  strcpy_s( _ver, 25, szNxt );
}
char const* appGetCwp( void ) { return _cwp; }
char const* appGetCwd( void ) { return _cwd; }
char const* appGetExe( void ) { return _exe; }
char const* appGetVer( void ) { return _ver; }
