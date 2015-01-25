#include "guiOrg.h"
// Example app would be PCSX2 which is primary testing ground for Medit v1
#ifndef EMULATOR
GUI_ORG guiOrg = {{NULL}};
void  guiOrg_OnLang ( void )
{
  IupSetAttribute ( guiOrg.main.fset, IUP_TITLE, appLang->a[ LNG_ORGANISATION ].a );
  IupSetAttribute ( guiOrg.name.fset, IUP_TITLE, appLang->a[ LNG_NAME         ].a );
  IupSetAttribute ( guiOrg.file.fset, IUP_TITLE, appLang->a[ LNG_FILE         ].a );
}
void  guiOrg_OnInit ( void )
{
  guiText_OnInit ( &guiOrg.name, ( Icallback ) guiName_OnKAny, ( Icallback ) guiName_OnValueChanged );
  guiText_OnInit ( &guiOrg.file, ( Icallback ) guiFile_OnKAny, ( Icallback ) guiFile_OnValueChanged );
  guiOrg.main.vb   = IupVbox ( guiOrg.name.fset,  guiOrg.file.fset,  NULL );
  guiOrg.main.fset = IupFrame ( guiOrg.main.vb );
  IupAppend ( guiDlg.vb, guiOrg.main.fset );
  IupMap ( guiOrg.main.fset );
  guiOrg_OnLang();
}
void guiOrg_OnDefPath ( char *path )
{
  mkdir ( path );
  strcat_s ( path, PATH_MAX, DIR_SEP );
  strcat_s ( path, PATH_MAX, appSession.org );
}
void guiOrg_OnDefExt ( char *path )
{
  strcat_s ( path, PATH_MAX, "m-org" );
}
void  guiOrg_OnLoad ( int fd )
{
  ipFdRdBuff ( fd, &srcOrg, sizeof ( DATA_ORG ) );
}
void  guiOrg_OnSave ( int fd )
{
  ipFdWrBuff ( fd, &srcOrg, sizeof ( DATA_ORG ) );
}
void  guiOrg_OnApply ( void )
{
  srcOrg = tmpOrg;
  strcpy_s ( appSession.org, NAME_MAX, tmpOrg.file );
}
void  guiOrg_OnReset ( void )
{
  tmpOrg = srcOrg;
  strcpy_s ( appSession.org, NAME_MAX, srcOrg.file );
}
int   guiOrg_OnShow ( Ihandle *ih )
{
  appMethods.OnDefPath = guiOrg_OnDefPath;
  appMethods.OnDefExt  = guiOrg_OnDefExt;
  appMethods.OnLoad    = guiOrg_OnLoad;
  appMethods.OnReset   = guiOrg_OnReset;
  appMethods.OnSave    = guiOrg_OnSave;
  appMethods.OnApply   = guiOrg_OnApply;
  if ( !ih )
    return IUP_DEFAULT;
  IupSetAttribute ( guiOrg.name.tb, "MEDIT_SRC_NAME", srcOrg.name );
  IupSetAttribute ( guiOrg.name.tb, "MEDIT_TMP_NAME", tmpOrg.name );
  IupSetAttribute ( guiOrg.file.tb, "MEDIT_SRC_FILE", srcOrg.file );
  IupSetAttribute ( guiOrg.file.tb, "MEDIT_TMP_FILE", tmpOrg.file );
  guiText_SendShowMsg ( &guiOrg.name, tmpOrg.name );
  guiText_SendShowMsg ( &guiOrg.file, tmpOrg.file );
  IupSetAttribute ( guiOrg.main.fset, "FLOATING", IUP_NO );
  guiOrg_OnLang();
  return IUP_DEFAULT;
}
#endif
