#include "guiMain.h"
static FILE *hLang = NULL;
static MEGUI _gui = {0};

void IupInitCwd( char *argv[] );
int main( int argc, char *argv[] )
{
  MECFG  cfg = {0};
  MEGUI *gui = &_gui;
  int    ret = 0;
  long   i = 0, p = 0, j = 0, k = 0, c = 0, l = 0, lines = 100;
  LANG *lang = meGetLang();
  Ipipe pipe = {0};
  char
    text[100][20] = {{0}},
    szSep[] = "\\/",
    szLine[ FILENAME_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  HMODULE lib = NULL;
  HACK_FUNC* hfunc = NULL;
  CODE_FUNC* cfunc = NULL;
  DWORD size = 0;
  HACKS hacks = {0};
  CODES codes = {0};
  /* Initialise IUP */
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  ipInitPiping( PP_TXT( WORKSPACE_NAME ), PP_TXT( TARGET_NAME ) );
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
  pipe = ipMkDir( "", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  ipShutPipe( &pipe );
  pipe = ipMkDir( "lang", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  ipShutPipe( &pipe );
  pipe = ipMkFile("medit.mecfg", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  if ( !pipe.pipe )
    goto mkGui;
  ipRdPipe( &pipe, &cfg, sizeof(MECFG) );
  cfg.lang[6] = 0; // Just force a NULL character
  ipShutPipe( &pipe );
  if ( cfg.lang[0] && _strcmpi( cfg.lang, "en" )!= 0 )
  {
    memset(   szLine, 0, FILENAME_MAX );
    strcat_s( szLine,    FILENAME_MAX, "lang"   );
    strcat_s( szLine,    FILENAME_MAX, DIR_SEP  );
    strcat_s( szLine,    FILENAME_MAX, cfg.lang );
    strcat_s( szLine,    FILENAME_MAX, ".melng" );
    pipe = ipMkFile( szLine, 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
    ipRdPipe( &pipe, lang, sizeof(LANG) );
    ipShutPipe( &pipe );
  }
  else
  {
    memset( cfg.lang, 0, 7 );
    cfg.lang[0] = 'e';
    cfg.lang[1] = 'n';
  }
mkGui:
  lib = meLoadLib( "ArmaxRaw", &hfunc, &cfunc );
  if ( lib )
  {
    memset( szLine, 0, FILENAME_MAX );
    hacks.size = HACKS_COUNT;
    memset( hacks.buff, 0, BUFSIZ * sizeof(HACK) );
    pipe = ipOpenFile( "C:\\p\\Omniconvert\\ArmaxRaw\\ff12.txt", 0666, SHARE_READ | SHARE_WRITE );
    RdTxtHacks( hfunc, &hacks, cfunc, &codes, &pipe );
    ipShutPipe( &pipe );
    memset( &pipe, 0, sizeof( Ipipe ) );
  }
  /* Get on with GUI */
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
      gui->sea.main_fset, gui->cmp.main_fset, gui->val.main_fset, NULL );
  gui->main.main_dlg = IupDialog( gui->main.main_vbox );
  IupSetAttribute( gui->main.main_dlg, IUP_TITLE,   "Medit" );
  IupSetAttribute( gui->main.main_dlg, IUP_SIZE,    "320x320"  );
  /* Show all */
  IupShow( gui->main.main_dlg );
  ret = IupMainLoop();
  pipe = ipMkFile("medit.mecfg", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  ipWrPipe( &pipe, &cfg, sizeof( MECFG ) );
  ipShutPipe( &pipe );
  lib = meFreeLib( lib );
  IupClose();
  return ret;
}

char szAppCwd[ FILENAME_MAX ] = {0};
char szAppExe[ 25 ] = {0};
char szAppVer[ 25 ] = {0};

void        IupInitCwd( char *argv[] )
{
  char
    szCwd[FILENAME_MAX] = {0},
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
    strcat_s( szAppCwd, FILENAME_MAX, szTmp );
    strcat_s( szAppCwd, FILENAME_MAX, DIR_SEP  );
  }
  while ( szNxt );
  szTmp = strtok_s( szPrv, ".", &szTok );
  strcpy_s( szAppExe, 25, szTmp );
  szNxt = strtok_s( szTmp, "-", &szTok );
  szNxt = strtok_s( NULL,  DIR_SEP, &szTok );
  strcpy_s( szAppVer, 25, szNxt );
  IupSetGlobal( "APP_CWD", szAppCwd );
  IupSetGlobal( "APP_EXE", szAppExe );
  IupSetGlobal( "APP_VER", szAppVer );
}
MEGUI* meGetGui( void )
{
  return &_gui;
}