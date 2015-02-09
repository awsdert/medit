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
            szTmp[NAME_MAX] = {0},
                              szPrv[NAME_MAX] = {0},
                                  szNxt[NAME_MAX] = {0};
  char path[ PATH_MAX ] = {0};
  STR_POS tok = {0, path};
  copystri( path, argv[0], PATH_MAX );
  /* Capture CWD and APPNAME */
  tok = searchstr( szPrv, NAME_MAX, tok, szSep );
  tok = searchstr( szNxt, NAME_MAX, tok, szSep );

  do
  {
    memcpy( szTmp, szPrv, NAME_MAX );
    memcpy( szPrv, szNxt, NAME_MAX );
    tok = searchstr( szNxt, NAME_MAX, tok, szSep );
    appendstr( _cwd,   szTmp, PATH_MAX );
    appendstr( _cwd, DIR_SEP, PATH_MAX );
  }
  while ( *tok.p );

  tok.i = 0;
  tok.p = szPrv;
  (void)searchstr( szTmp, NAME_MAX, tok, "." );
  tok.i = 0;
  tok.p = szTmp;
  tok = searchstr( szNxt, NAME_MAX, tok, "-" );
  copystri( _exe, szNxt, NAME_MAX * 2 );
  (void)searchstr( szNxt, NAME_MAX, tok, DIR_SEP );
  copystri( _ver, szNxt, NAME_MAX * 2 );
  copystri( _name, appName, NAME_MAX );
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
