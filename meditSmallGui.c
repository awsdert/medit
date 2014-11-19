#include "guiMain.h"
static FILE *hLang = NULL;
static GUI _gui = {0};
static CFG _cfg = {0};
GUI* appGetGui( void ) { return &_gui; }
CFG* appGetCfg( void ) { return &_cfg; }
void _appInitCwd( char *argv[] );

// This is the only place that needs these so no header required
extern void    _appInitLang( void );
extern void guiHacks_OnInit( void );
extern void   guiOrg_OnInit( void );
extern void   guiPfm_OnInit( void );
extern void   guiTar_OnInit( void );
extern void   guiQry_OnInit( void );

int main( int argc, char *argv[] )
{
  GUI *gui = &_gui;
  CFG *cfg = &_cfg;
  int    ret = 0;
  long   i = 0, p = 0, j = 0, k = 0, c = 0, l = 0, lines = 100;
  LANG const *lang = appGetLang();
  Ipipe pipe = {0};
  char
    text[100][20] = {{0}},
    szSep[] = "\\/",
    szLine[ FILENAME_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  HMODULE      lib = NULL;
  HACK_FUNC* hfunc = NULL;
  CODE_FUNC* cfunc = NULL;
  DWORD   size = 0;
  HACKL     hl = {0};
  HACKS *hacks = &hl.hacks;
  CODES  codes = {0};
  char **lngx = (char**)lang->x;
  /* Initialise IUP */
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  ipInitPiping( PP_TXT( WORKSPACE_NAME ), PP_TXT( TARGET_NAME ) );
  _appInitCwd( argv );
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
  ipRdPipe( &pipe, &cfg, sizeof(CFG) );
  ipShutPipe( &pipe );
  cfg->lang[6] = 0; // Just force a NULL character
  appLoadLang( cfg->lang );
mkGui:
  //lib = meLoadLib( "ArmaxRaw", &hfunc, &cfunc );
  if ( lib )
  {
    memset( szLine, 0, FILENAME_MAX );
    hacks->c = HACKS_COUNT;
    memset( hacks->a, 0, BUFSIZ * sizeof(HACK) );
    pipe = ipOpenFile( "C:\\p\\Omniconvert\\ArmaxRaw\\ff12.txt", 0666, SHARE_READ | SHARE_WRITE );
    RdTxtHacks( hfunc, &hl, cfunc, &codes, &pipe );
    ipShutPipe( &pipe );
    memset( &pipe, 0, sizeof( Ipipe ) );
  }
  /* Get on with GUI */
  // Organisation
  guiOrg_OnInit();
  // Platform
#ifdef GUI_SHARED
  gui->pfm.main = gui->org.main;
  gui->pfm.name = gui->org.name;
  gui->pfm.file = gui->org.file;
#endif
  guiPfm_OnInit();
  // Target
#ifdef GUI_SHARED
  gui->tar.main = gui->org.main;
  gui->tar.name = gui->org.name;
  gui->tar.file = gui->org.file;
#endif
  guiTar_OnInit();
  // Hacks
#ifdef GUI_SHARED
  gui->hacks.main = gui->org.main;
  gui->hacks.hack.main = gui->org.main;
  gui->hacks.hack.name = gui->org.name;
#endif
  guiHacks_OnInit();
  // Search
  guiQry_OnInit();
#ifdef GUI_SHARED
  gui->res.main  = gui->qry.main;
  gui->res.codes = gui->hacks.codes;
#endif
  /* Main List */
  gui->listMain =
    meMkList( (Icallback)meMenu_ButtonCB,
            lang->x[ LNG_ORGANISATION ],
            lang->x[ LNG_PLATFORM ],
            lang->x[ LNG_TARGET ],
            lang->x[ LNG_PROFILE ],
            lang->x[ LNG_SEARCH ],
            lang->x[ LNG_RESULTS ],
            lang->x[ LNG_EDITOR ],
            lang->x[ LNG_HACKS ],
            lang->x[ LNG_ABOUT ], NULL );
  gui->vbMain =
    IupVbox( gui->listMain, gui->qry.main.fset, gui->org.main.fset, NULL );
  gui->dlgMain = IupDialog( gui->vbMain );
  IupSetAttribute( gui->dlgMain, IUP_TITLE,   "Medit" );
  IupSetAttribute( gui->dlgMain, IUP_SIZE,    "320x320"  );
  /* Show all */
  IupRefresh( gui->dlgMain );
  IupFlush();
  IupShow( gui->dlgMain );
  ret = IupMainLoop();
  pipe = ipMkFile("medit.mecfg", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  ipWrPipe( &pipe, &cfg, sizeof( CFG ) );
  ipShutPipe( &pipe );
  lib = meFreeLib( lib );
  IupClose();
  return ret;
}

char szAppCwd[ FILENAME_MAX ] = {0};
char szAppExe[ 25 ] = {0};
char szAppVer[ 25 ] = {0};

void        _appInitCwd( char *argv[] )
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
