#include "guiMain.h"
//static FILE *hLang = NULL;
GUI          appGui = {{0}};
GUI_MAIN     guiDlg = {NULL};
SESSION  appSession = {"en", ""};
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
extern void guiLoad( uchar useDlg );
extern void   _appInitLang( void );
extern void guiMenu_OnInit( void );
extern void  guiQry_OnInit( void );
extern int  guiMenu_OnValueChanged( Ihandle *ih );

int main( int argc, char *argv[] )
{
  int    ret = 0;
  FILE *file = NULL;
  char path[ PATH_MAX] = {0};
  char const *szTok = NULL;

  /* Initialise IUP */
  if ( IupOpen( &argc, &argv ) == IUP_ERROR )
  {
    fprintf( stderr, "Error Opening IUP." );
    return -1;
  }

  _appInitCwd( argv, "medit" );
  // Capture default font
  szTok = IupGetAttribute( NULL, "DEFAULTFONT" );
  strncpyi( path, szTok, PATH_MAX );
  szTok = strntok( appGui.font,    20, path, ", ",  NULL );
  szTok = strntok( appGui.fontSize, 5, path, ", ", szTok );
  // Prep directories in case they don't exist
  strncpyi( path, ipGetUsrDir(), PATH_MAX );
  appendstr( path, DIR_SEP ".medit", PATH_MAX );

  if ( _access( path, 0 ) != 0 )
  {
    ipMkDir( path );
  }

  appendstr( path, DIR_SEP "lang", PATH_MAX );

  if ( _access( path, 0 ) != 0 )
  {
    ipMkDir( path );
  }

  // Just force a NULL character
  strncpyi( path, ipGetUsrDir(), PATH_MAX );
  appendstr( path, DIR_SEP ".medit" DIR_SEP "default.m-session", PATH_MAX );

  if ( _access( path, 0 ) == 0 && (file = fopen( path, "rb" )) )
  {
    fread( &appSession, sizeof(SESSION), 1, file );
    fclose( file );
    appSession.lang[6] = 0;
    guiOrg_OnShow( NULL );
    guiLoad( 0 );
    guiPfm_OnShow( NULL );
    guiLoad( 0 );
    guiTar_OnShow( NULL );
    guiLoad( 0 );
    guiPro_OnShow( NULL );
    guiLoad( 0 );
    appLoadLib( appSession.lib );
  }
initLang:
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
  ( void )guiMenu_OnValueChanged( guiMenu );
  IupSetInt ( guiData, IUP_VALUE, 0 );
  ret = IupMainLoop();
  strncpyi( path, ipGetUsrDir(), PATH_MAX );
  appendstr( path, DIR_SEP ".medit" DIR_SEP "default.m-session", PATH_MAX );
  if ( file = fopen( path, "wb") )
  {
    fwrite( &appSession, sizeof( SESSION ), 1, file );
    fclose( file );
  }
  appLoadLib( NULL );
  IupClose();
  return ret;
}
