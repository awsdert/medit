#include "guiMain.h"
//static FILE *hLang = NULL;
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

extern void _appInitCwd( char *argv[], char *appName );
// This is the only place that needs these so no header required
extern void guiOpen( uchar saveFile );
extern void   _appInitLang( void );
extern void guiMenu_OnInit( void );
extern void  guiQry_OnInit( void );
extern int  guiMenu_OnValueChanged( Ihandle *ih );

int main( int argc, char *argv[] )
{
  int    ret = 0;
  long   i = 0, p = 0, j = 0, k = 0, c = 0, l = 0, lines = 100;
  int fd;
  char
    text[100][20] = {{0}},
    szSep[] = "\\/",
    path[ PATH_MAX ] = {0},
    *szTok = NULL, *szPrv = NULL, *szNow = NULL, *szTmp = NULL;
  DWORD size = 0;
  char **lngx = (char**)appLang;
  /* Initialise IUP */
  if ( IupOpen(&argc, &argv) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }
  _appInitCwd( argv, "medit" );
  // Capture default font
  szTok = NULL;
  szTmp = IupGetAttribute( NULL, "DEFAULTFONT" );
  strcpy_s( path, PATH_MAX, szTmp );
  szTmp = strtok_s( path, ", ", &szTok );
  strcat_s( appGui.font, 20, szTmp );
  szTmp = strtok_s( NULL, ", ", &szTok );
  strcat_s( appGui.fontSize, 5, szTmp );
  // Prep directories in case they don't exist
  strcpy_s( path, PATH_MAX, ipGetUsrDir() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" );
  if ( _access( path, 0 ) != 0 )
    ipMkDir( path );
  strcat_s( path, PATH_MAX, DIR_SEP "lang" );
  if ( _access( path, 0 ) != 0 )
    ipMkDir( path );
  // Just force a NULL character
  strcpy_s( path, PATH_MAX, ipGetUsrDir() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP "default.m-session" );
  if ( _access( path, 0 ) == 0 )
  {
    ipFdOpen( &fd, path, IP_O_FILE | IP_O_RW, IP_D_RW, IP_A_RW );
    ipFdRdBuff( fd, &appSession, sizeof( SESSION ) );
    ipFdShut( fd );
    appSession.lang[6] = 0;
    guiOrg_OnShow( NULL );
    guiOpen(2);
    guiPfm_OnShow( NULL );
    guiOpen(2);
    guiTar_OnShow( NULL );
    guiOpen(2);
    guiPro_OnShow( NULL );
    guiOpen(2);
  }
  appLoadLang( appSession.lang );
  // Get on with GUI
  /* Main List */
  guiDlg.vb = IupVbox( NULL );
  guiDlg.fset = IupDialog( guiDlg.vb );
  guiMenu_OnInit();
  IupSetAttribute( guiDlg.fset, IUP_TITLE, "Medit" );
  IupSetAttribute( guiDlg.fset, IUP_SIZE,  "320x320"  );
  /* Show all */
  IupRefresh( guiDlg.fset );
  IupFlush();
  IupShow( guiDlg.fset );
  IupSetInt ( guiMenu, IUP_VALUE, 1 );
  (void)guiMenu_OnValueChanged( guiMenu );
  IupSetInt ( guiData, IUP_VALUE, 0 );
  ret = IupMainLoop();
  strcpy_s( path, PATH_MAX, ipGetUsrDir() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP "default.m-session" );
  ipFdOpen( &fd, path, IP_O_MKFILE | IP_O_RW, IP_D_RW, IP_A_RW );
  ipFdWrBuff( fd, &appSession, sizeof( SESSION ) );
  ipFdShut( fd );
  appFreeLib();
  IupClose();
  return ret;
}
