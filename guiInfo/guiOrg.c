#include "guiOrg.h"
// Example app would be PCSX2 which is primary testing ground for Medit v1
#ifndef EMULATOR
GUI_ORG guiOrg = {{NULL}};
void  guiOrg_OnLang ( void )
{
  IupSetAttribute ( guiOrg.main.fset, IUP_TITLE,
                    appLang->a[ LNG_ORGANISATION ].a );
  IupSetAttribute ( guiOrg.name.fset, IUP_TITLE,
                    appLang->a[ LNG_NAME         ].a );
  IupSetAttribute ( guiOrg.file.fset, IUP_TITLE,
                    appLang->a[ LNG_FILE         ].a );
}
void  guiOrg_OnInit ( void )
{
  guiText_OnInit ( &guiOrg.name, ( Icallback ) guiName_OnKAny,
                   ( Icallback ) guiName_OnValueChanged );
  guiText_OnInit ( &guiOrg.file, ( Icallback ) guiFile_OnKAny,
                   ( Icallback ) guiFile_OnValueChanged );
  guiOrg.main.vb   = IupVbox ( guiOrg.name.fset,  guiOrg.file.fset,  NULL );
  guiOrg.main.fset = IupFrame ( guiOrg.main.vb );
  IupAppend ( guiDlg.vb, guiOrg.main.fset );
  IupMap ( guiOrg.main.fset );
  guiOrg_OnLang();
}
void guiOrg_OnDefPath ( char *path )
{
  ipMkDir ( path );
  appendstr ( path, DIR_SEP, PATH_MAX );
  appendstr ( path, appSession.org, PATH_MAX );
}
void guiOrg_OnDefExt ( char *path )
{
  appendstr ( path, "m-org", PATH_MAX );
}
void  guiOrg_OnLoad ( char *path )
{
  FILE *file = fopen( path, "rb" );
  fread ( &srcOrg, sizeof ( DATA_ORG ), 1, file );
  fclose( file );
}
void  guiOrg_OnSave ( char *path )
{
  FILE *file = fopen( path, "wb" );
  fwrite ( &srcOrg, sizeof ( DATA_ORG ), 1, file );
  fclose( file );
}
void  guiOrg_OnApply ( void )
{
  srcOrg = tmpOrg;
  copystri ( appSession.org, tmpOrg.file, NAME_MAX );
}
void  guiOrg_OnReset ( void )
{
  tmpOrg = srcOrg;
  copystri ( appSession.org, srcOrg.file, NAME_MAX );
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
  {
    return IUP_DEFAULT;
  }

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
