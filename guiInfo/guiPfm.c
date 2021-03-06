#include "guiPfm.h"
#ifndef EMULATOR
GUI_PFM guiPfm = {{NULL}};
void  guiPfm_OnLang ( void )
{
  IupSetAttribute ( guiPfm.main.fset, IUP_TITLE, appLang->a[ LNG_PLATFORM ].a );
  IupSetAttribute ( guiPfm.name.fset, IUP_TITLE, appLang->a[ LNG_NAME     ].a );
  IupSetAttribute ( guiPfm.file.fset, IUP_TITLE, appLang->a[ LNG_FILE     ].a );
  IupSetAttribute ( guiPfm.hlEndian,  IUP_TITLE, appLang->a[ LNG_ENDIAN   ].a );
  IupSetAttribute ( guiPfm.hlEndian,  "1", appLang->a[ LNG_SYSTEM   ].a );
  IupSetAttribute ( guiPfm.hlEndian,  "2", appLang->a[ LNG_LITTLE   ].a );
  IupSetAttribute ( guiPfm.hlEndian,  "3", appLang->a[ LNG_PDP      ].a );
  IupSetAttribute ( guiPfm.hlEndian,  "4", appLang->a[ LNG_BIG      ].a );
}
extern void guiOrg_OnInit ( void );
void  guiPfm_OnInit ( void )
{
  guiOrg_OnInit();
  // Endian
  guiPfm.hlEndian = IupHList ( NULL );
  IupSetAttribute ( guiPfm.hlEndian, "FLOATING", IUP_YES );
  IupSetAttribute ( guiPfm.hlEndian, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback ( guiPfm.hlEndian, "VALUECHANGED_CB", guiEndian_OnValueChanged );
  // Main
#ifdef GUI_SHARED
  guiPfm.main = guiOrg.main;
  guiPfm.name = guiOrg.name;
  guiPfm.file = guiOrg.file;
  IupAppend ( guiPfm.main.vb, guiPfm.hlEndian );
  IupMap ( guiPfm.hlEndian );
#else
  guiText_OnInit ( &guiPfm.name, ( Icallback ) guiName_OnKAny,
                   guiName_OnValueChanged );
  guiText_OnInit ( &guiPfm.file, ( Icallback ) guiFile_OnKAny,
                   guiFile_OnValueChanged );
  guiPfm.main.vb   = IupVbox ( guiPfm.name.fset, guiPfm.file.fset,
                               guiPfm.listtEndian, NULL );
  guiPfm.main.fset = IupFrame ( guiPfm.main.vb );
  IupSetAttribute ( guiPfm.fsetMain, "FLOATING", IUP_YES );
#endif
  guiPfm_OnLang();
}
extern void guiOrg_OnDefPath ( char *path );
void guiPfm_OnDefPath ( char *path )
{
  guiOrg_OnDefPath ( path );
  ipMkDir ( path );
  appendstr ( path, DIR_SEP, PATH_MAX );
  appendstr ( path, appSession.pfm, PATH_MAX );
}
void guiPfm_OnDefExt ( char *path )
{
  appendstr ( path, "m-pfm", PATH_MAX );
}
void guiPfm_OnLoad ( char *path )
{
  FILE *file = fopen( path, "rb" );
  fread ( &srcPfm, sizeof ( DATA_PFM ), 1, file );
  fclose( file );
}

void guiPfm_OnSave ( char *path )
{
  FILE *file = fopen( path, "wb" );
  fwrite ( &srcPfm, sizeof ( DATA_PFM ), 1, file );
  fclose( file );
}
void guiPfm_OnApply ( void )
{
  srcPfm = tmpPfm;
  copystri ( appSession.pfm, tmpPfm.file, NAME_MAX );
}
void guiPfm_OnReset ( void )
{
  tmpPfm = srcPfm;
  copystri ( appSession.pfm, srcPfm.file, NAME_MAX );
}
int   guiPfm_OnShow ( Ihandle *ih )
{
  appMethods.OnDefPath = guiPfm_OnDefPath;
  appMethods.OnDefExt  = guiPfm_OnDefExt;
  appMethods.OnLoad    = guiPfm_OnLoad;
  appMethods.OnReset   = guiPfm_OnReset;
  appMethods.OnSave    = guiPfm_OnSave;
  appMethods.OnApply   = guiPfm_OnApply;

  if ( !ih )
  {
    return IUP_DEFAULT;
  }

  IupSetAttribute ( guiPfm.name.tb, "MEDIT_SRC_NAME", srcPfm.name );
  IupSetAttribute ( guiPfm.name.tb, "MEDIT_TMP_NAME", tmpPfm.name );
  IupSetAttribute ( guiPfm.file.tb, "MEDIT_SRC_FILE", srcPfm.file );
  IupSetAttribute ( guiPfm.file.tb, "MEDIT_TMP_FILE", tmpPfm.file );
  guiText_SendShowMsg ( &guiPfm.name, tmpPfm.name );
  guiText_SendShowMsg ( &guiPfm.file, tmpPfm.file );
  IupSetInt ( guiPfm.hlEndian, IUP_VALUE, tmpPfm.endian );
  IupSetAttribute ( guiPfm.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute ( guiPfm.hlEndian, "FLOATING", IUP_NO );
  IupShow ( guiPfm.hlEndian );
  guiPfm_OnLang();
  return IUP_DEFAULT;
}
int guiEndian_OnValueChanged ( Ihandle *ih )
{
  tmpPfm.endian = IupGetInt ( ih, IUP_VALUE );
  return IUP_DEFAULT;
}
#endif
