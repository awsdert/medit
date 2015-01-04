#include "guiMain.h"
static FILE *hLang = NULL;
GUI          appGui = {0};
GUI_MAIN     guiDlg = {NULL};
SESSION  appSession = {"en",""};
DATA_ORG     srcOrg = {""};
DATA_ORG     tmpOrg = {""};
DATA_PFM     srcPfm = {""};
DATA_PFM     tmpPfm = {""};
DATA_TAR     tmpTar = {""};
DATA_TAR     srcTar = {""};
DATA_PRO     srcPro = {""};
DATA_PRO     tmpPro = {""};
METHODS  appMethods = {NULL};

void* guiCfg_OnOpen( char *path, size_t *size )
{
  strcat_s( path, PATH_MAX, "session.m-cfg" );
  *size = sizeof(SESSION);
  return &appSession;
}
extern void _appInitCwd( char *argv[], char *appName );
#ifdef TEST_HL_LIB
extern void testHLLib( void );
#endif // TEST_HL_LIB
// This is the only place that needs these so no header required
extern void guiOpen( uchar saveFile );
extern void   _appInitLang( void );
extern void guiMenu_OnInit( void );
extern void  guiQry_OnInit( void );

int main( int argc, char *argv[] )
{
  int    ret = 0;
  long   i = 0, p = 0, j = 0, k = 0, c = 0, l = 0, lines = 100;
  Ipipe pipe = {0};
  char
    text[100][20] = {{0}},
    szSep[] = "\\/",
    szLine[ PATH_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  HMODULE      lib = NULL;
  HACK_LIB_COM* hfunc = NULL;
  CODE_LIB_COM* cfunc = NULL;
  DWORD   size = 0;
  char **lngx = (char**)appLang->x;
  /* Initialise IUP */
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  ipInitPiping( PP_TXT( WORKSPACE_NAME ), PP_TXT( TARGET_NAME ) );
  _appInitCwd( argv, "medit" );
  // Capture default font
  szTok = NULL;
  memset( szLine, 0, PATH_MAX );
  szTmp = IupGetAttribute( NULL, "DEFAULTFONT" );
  strcpy_s( szLine, PATH_MAX, szTmp );
  szTmp = strtok_s( szLine, ", ", &szTok );
  strcat_s( appGui.font, 20, szTmp );
  szTmp = strtok_s( NULL, ", ", &szTok );
  strcat_s( appGui.fontSize, 5, szTmp );
  // Prep directories in case they don't exist
  pipe = ipMkDir( "", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  ipShutPipe( &pipe );
  pipe = ipMkDir( "lang", 0666, SHARE_READ | SHARE_WRITE, ACTION_OPEN_NEW );
  ipShutPipe( &pipe );
  // Just force a NULL character
  appSession.lang[6] = 0;
  appLoadLang( appSession.lang );
#ifdef TEST_HL_LIB
  // Change string to match library being tested
  testHLLib();
  lib = appLoadLib( "ArmaxRaw", &hfunc, &cfunc );
  if ( lib )
  {
    memset( szLine, 0, PATH_MAX );
    hacks->c = HACKS_COUNT;
    memset( hacks->a, 0, BUFSIZ * sizeof(HACK) );
    pipe = ipOpenFile( "C:\\p\\Omniconvert\\ArmaxRaw\\ff12.txt", 0666, SHARE_READ | SHARE_WRITE );
    RdTxtHacks( hfunc, &hl, cfunc, &codes, &pipe );
    ipShutPipe( &pipe );
    memset( &pipe, 0, sizeof( Ipipe ) );
  }
#endif
  // Get on with GUI
  /* Main List */
  guiDlg.vb = IupVbox( NULL );
  guiDlg.fset = IupDialog( guiDlg.vb );
  guiMenu_OnInit();
  IupSetAttribute( guiDlg.fset, IUP_TITLE,   "Medit" );
  IupSetAttribute( guiDlg.fset, IUP_SIZE,    "320x320"  );
  /* Show all */
  IupRefresh( guiDlg.fset );
  IupFlush();
  IupShow( guiDlg.fset );
  ret = IupMainLoop();
  pipe = ipMkFile("default.m-session", 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
  ipWrPipe( &pipe, &appSession, sizeof( SESSION ) );
  ipShutPipe( &pipe );
#ifdef TEST_HL_LIB
  lib = appFreeLib( lib );
#endif
  IupClose();
  return ret;
}
