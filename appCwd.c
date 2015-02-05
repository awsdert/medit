#include "appData.h"
static char _cwp[ PATH_MAX ] = {0};
static char _cwd[ PATH_MAX ] = {0};
static char _exe[ NAME_MAX * 2 ] = {0};
static char _ver[ NAME_MAX * 2 ] = {0};
static char _name[ NAME_MAX ] = {0};

void  _appInitCwd( char *argv[], char *appName )
{
  char const *szTok = NULL;
  char
  szSep[] = "\\/",
            szTmp[NAME_MAX] = {0},
                              szPrv[NAME_MAX] = {0},
                                  szNxt[NAME_MAX] = {0};
  char path[ PATH_MAX ] = {0};
  strncpyi( path, argv[0], PATH_MAX );
  /* Capture CWD and APPNAME */
  szTok = strntok( szPrv, NAME_MAX, path, szSep, NULL );
  szTok = strntok( szNxt, NAME_MAX, path, szSep, szTok );

  do
  {
    memcpy( szTmp, szPrv, NAME_MAX );
    memcpy( szPrv, szNxt, NAME_MAX );
    szTok = strntok( szNxt, NAME_MAX, path, szSep, szTok );
    appendstr( _cwd,   szTmp, PATH_MAX );
    appendstr( _cwd, DIR_SEP, PATH_MAX );
  }
  while ( szTok );

  szTok = strntok( szTmp, NAME_MAX, szPrv, ".", NULL );
  szTok = strntok( szNxt, NAME_MAX, szTmp, "-", NULL );
  strncpyi( _exe, szNxt, NAME_MAX * 2 );
  szTok = strntok( szNxt, NAME_MAX, szTmp, DIR_SEP, szTok );
  strncpyi( _ver, szNxt, NAME_MAX * 2 );
  strncpyi( _name, appName, NAME_MAX );
}
char const* appGetCwp( void )
{
  return _cwp;
}
char const* appGetCwd( void )
{
  return _cwd;
}
char const* appGetExe( void )
{
  return _exe;
}
char const* appGetVer( void )
{
  return _ver;
}
char const* appGetName( void )
{
  return _name;
}
