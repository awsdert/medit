#include "guiOrg.h"
// Example app would be PCSX2 which is primary testing ground for Medit v1
#ifndef EMULATOR
GUI_ORG guiOrg = {{NULL}};
void  guiOrg_OnLang( void )
{
  IupSetAttribute( guiOrg.main.fset, IUP_TITLE, appLang->x[ LNG_ORGANISATION ] );
  IupSetAttribute( guiOrg.name.fset, IUP_TITLE, appLang->x[ LNG_NAME         ] );
  IupSetAttribute( guiOrg.file.fset, IUP_TITLE, appLang->x[ LNG_FILE         ] );
}
void  guiOrg_OnInit( void )
{
  guiText_OnInit( &guiOrg.name, (Icallback)guiName_OnKAny, (Icallback)guiName_OnValueChanged );
  guiText_OnInit( &guiOrg.file, (Icallback)guiFile_OnKAny, (Icallback)guiFile_OnValueChanged );
  guiOrg.main.vb   = IupVbox(  guiOrg.name.fset,  guiOrg.file.fset,  NULL );
  guiOrg.main.fset = IupFrame( guiOrg.main.vb );
  IupAppend( guiDlg.vb, guiOrg.main.fset );
  IupMap( guiOrg.main.fset );
  IupSetAttribute( guiOrg.main.fset, IUP_EXPAND, IUP_YES );
  IupSetAttribute( guiOrg.main.fset, "FLOATING", IUP_YES );
  guiOrg_OnLang();
}
void  guiOrg_OnDefPath( char *path );
void  guiOrg_OnDefExt(  char *path );
void  guiOrg_OnLoad( Ipipe *file );
void  guiOrg_OnReset( void );
void  guiOrg_OnSave( Ipipe *file );
void  guiOrg_OnApply( void );
int   guiOrg_OnShow( Ihandle *ih )
{
  srcName = srcOrg.name;
  srcFile = srcOrg.file;
  tmpName = tmpOrg.name;
  tmpFile = tmpOrg.file;
  appMethods.OnDefPath = guiOrg_OnDefPath;
  appMethods.OnDefExt  = guiOrg_OnDefExt;
  appMethods.OnLoad    = guiOrg_OnLoad;
  appMethods.OnReset   = guiOrg_OnReset;
  appMethods.OnSave    = guiOrg_OnSave;
  appMethods.OnApply   = guiOrg_OnApply;
  guiText_SendShowMsg( &guiOrg.name, tmpName );
  guiText_SendShowMsg( &guiOrg.file, tmpFile );
  IupSetAttribute( guiOrg.main.fset, "FLOATING", IUP_NO );
  guiOrg_OnLang();
  return IUP_DEFAULT;
}
void guiOrg_OnDefPath( char *path )
{
  strcat_s( path, PATH_MAX, "org" );
  strcat_s( path, PATH_MAX, appSession.org );
}
void guiOrg_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, "m-org" );
}
void  guiOrg_OnLoad( Ipipe *file ) { ipRdPipe( file, &srcOrg, sizeof(DATA_ORG) ); }
void  guiOrg_OnReset( void ) { tmpOrg = srcOrg; }
void  guiOrg_OnSave( Ipipe *file ) { ipWrPipe( file, &srcOrg, sizeof(DATA_ORG) ); }
void  guiOrg_OnApply( void ) { srcOrg = tmpOrg; }
#endif
