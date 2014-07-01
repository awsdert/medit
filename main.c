#include "search.h"
static char  szCwd[ FILENAME_MAX ] = {0};
static char  szAppName[ FILENAME_MAX ] = {0};
static FILE *hLang = NULL;
static MEGUI _gui = {0};


void IupInitCwd( char *argv[] );
int main( int argc, char *argv[] )
{
  MECFG  cfg = {0};
  MEGUI *gui = &_gui;
  int    ret = 0;
  LANG *lang = meGetLang();
  Ipipe pipe = NULL;
  char
    szSep[] = "\\/",
    szLine[ FILENAME_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  /* Initialise IUP */
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  IupInitCwd( argv );
  /* Capture default font */
  szTok = NULL;
  memset( szLine, 0, FILENAME_MAX );
  szTmp = IupGetAttribute( NULL, "DEFAULTFONT" );
  strcpy_s( szLine, FILENAME_MAX, szTmp );
  szTmp = strtok_s( szLine, ", ", &szTok );
  strcat_s( gui->font, 20, szTmp );
  szTmp = strtok_s( NULL, ", ", &szTok );
  strcat_s( gui->fontSize, 5, szTmp );
  /* Open Configuration/Language Files */
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
    IupRdPipe( pipe, lang, sizeof(LANG) );
    IupShutPipe( pipe );
  }
  else
  {
    memset( cfg.lang, 0, 7 );
    cfg.lang[0] = 'e';
    cfg.lang[1] = 'n';
  }
mkGui:
  gui->main.main_dd =
    meMkList( (Icallback)meMenu_ButtonCB,
            lang->x[ LANG_TARGET ],
            lang->x[ LANG_PROFILE ],
            lang->x[ LANG_SEARCH ],
            lang->x[ LANG_RESULTS ],
            lang->x[ LANG_EDITOR ],
            lang->x[ LANG_HACKS ],
            lang->x[ LANG_ABOUT ], NULL );
  /* Comparison Fieldset */
  gui->cmp.main_vbox = IupVbox( NULL );
  gui->cmp.main_fset = IupFrame( gui->cmp.main_vbox );
  IupSetAttribute( gui->cmp.main_fset, IUP_TITLE, lang->x[ LANG_COMPARISON ] );
  IupSetAttribute( gui->cmp.main_fset, IUP_SIZE, "100x30" );
  IupSetAttribute( gui->cmp.main_fset, IUP_EXPAND, IUP_HORIZONTAL );
  meCmp_OnInit( gui->cmp.main_fset );
  /* Value Fieldset */
  gui->val.main_vbox = IupVbox( NULL );
  gui->val.main_fset = IupFrame( gui->val.main_vbox );
  IupSetAttribute( gui->val.main_fset, IUP_TITLE, lang->x[ LANG_VALUE ] );
  IupSetAttribute( gui->val.main_fset, IUP_SIZE, "100x30" );
  IupSetAttribute( gui->val.main_fset, IUP_EXPAND, IUP_HORIZONTAL );
  meVal_OnInit( gui->val.main_fset );
  /* Search List */
  gui->sea.main_vbox = IupVbox( NULL );
  gui->sea.main_fset = IupFrame( gui->sea.main_vbox );
  IupSetAttribute( gui->sea.main_fset, IUP_TITLE, lang->x[ LANG_SEARCH ] );
  IupSetAttribute( gui->sea.main_fset, IUP_SIZE, "100x30" );
  IupSetAttribute( gui->sea.main_fset, IUP_EXPAND, IUP_HORIZONTAL );
  meSea_OnInit( gui->sea.main_fset );
  /* Main List */
  gui->main.main_vbox =
    IupVbox( gui->main.main_dd,
      gui->sea.main_fset, gui->cmp.main_fset, gui->val.main_fset );
  gui->main.main_dlg = IupDialog( gui->main.main_vbox );
  IupSetAttribute( gui->main.main_dlg, IUP_TITLE,   "Medit" );
  IupSetAttribute( gui->main.main_dlg, IUP_SIZE,    "320x320"  );
  /* Show all */
  IupShow( gui->main.main_dlg );
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
void        IupInitCwd( char *argv[] )
{
  char
    szSep[] = "\\/",
   *szTok = NULL,
   *szTmp = NULL,
   *szPrv = NULL,
   *szNxt = NULL;
  /* Capture CWD and APPNAME */
  szPrv = strtok_s( argv[0], szSep, &szTok );
  szNxt = strtok_s( NULL,   szSep, &szTok );
  do
  {
    szTmp = szPrv;
    szPrv = szNxt;
    szNxt = strtok_s( NULL, szSep, &szTok );
    strcat_s( szCwd, FILENAME_MAX, szTmp );
    strcat_s( szCwd, FILENAME_MAX, DIR_SEP  );
  }
  while ( szNxt );
  szTmp = strtok_s( szPrv, ".", &szTok );
  strcat_s( szAppName, FILENAME_MAX, szTmp );
}
char const* IupGetAppName( void )
{
  return szAppName;
}
MEGUI* meGetGui( void )
{
  return &_gui;
}
void IupGetLDataDirFromName( char *path, DWORD size )
{
  IupGetLDataDir( path );
  strcat_s( path, size, DIR_SEP );
  strcat_s( path, size, IupGetAppName() );
  strcat_s( path, size, DIR_SEP );
}
