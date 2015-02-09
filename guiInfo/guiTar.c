#include "guiTar.h"
#include <math.h>
#ifndef EMULATOR
GUI_TAR guiTar = {{NULL}};
void guiTar_OnLang ( void )
{
  IupSetAttribute ( guiTar.main.fset, IUP_TITLE, appLang->a[ LNG_TARGET ].a );
  IupSetAttribute ( guiTar.name.fset, IUP_TITLE, appLang->a[ LNG_NAME   ].a );
  IupSetAttribute ( guiTar.file.fset, IUP_TITLE, appLang->a[ LNG_FILE   ].a );
  IupSetAttribute ( guiTar.path.fset, IUP_TITLE, appLang->a[ LNG_PATH   ].a );
  IupSetAttribute ( guiTar.targ.fset, IUP_TITLE, appLang->a[ LNG_TARGET ].a );
  IupSetAttribute ( guiTar.indx.fset, IUP_TITLE, appLang->a[ LNG_MEMORY ].a );
}
extern void  guiPfm_OnInit ( void );
extern void guiBase_OnInit ( void );
int guiPath_OnKAny ( Ihandle *ih, int c );
int guiTarg_OnKAny ( Ihandle *ih, int c );
int guiPath_OnValueChanged ( Ihandle *ih );
int guiTarg_OnValueChanged ( Ihandle *ih );
void  guiTar_OnInit ( void )
{
  guiPfm_OnInit();
  guiText_OnInit ( &guiTar.path, ( Icallback ) guiPath_OnKAny,
                   guiPath_OnValueChanged );
  guiText_OnInit ( &guiTar.targ, ( Icallback ) guiTarg_OnKAny,
                   guiTarg_OnValueChanged );
  guiTar.indx.spin = IupVal ( IUP_HORIZONTAL );
  guiTar.indx.fset = IupFrame ( guiTar.indx.spin );
  //guiSpin_OnInit( &guiTar.indx, NULL, NULL );
  IupSetInt ( guiTar.indx.spin, "MAX", 15 );
  IupSetInt ( guiTar.indx.spin, "SHOWTICKS", BASES_COUNT );
  IupSetFloat ( guiTar.indx.spin, "STEP", 1.0f / BASES_COUNT );
  IupSetAttribute ( guiTar.indx.spin, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute ( guiTar.indx.fset, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback ( guiTar.indx.spin, "VALUECHANGED_CB", guiTar_OnShow );
  guiBase = &guiTar.base;
  guiBase_OnInit();
  //IupSetAttribute( guiBase->main.fset, IUP_EXPAND, IUP_YES );
#ifdef GUI_SHARED
  guiTar.main = guiOrg.main;
  guiTar.name = guiOrg.name;
  guiTar.file = guiOrg.file;
  IupAppend ( guiTar.main.vb, guiTar.path.fset );
  IupMap ( guiTar.path.fset );
  IupAppend ( guiTar.main.vb, guiTar.targ.fset );
  IupMap ( guiTar.targ.fset );
  IupAppend ( guiTar.main.vb, guiTar.indx.fset );
  IupMap ( guiTar.indx.fset );
  IupAppend ( guiTar.main.vb, guiBase->main.fset );
  IupMap ( guiBase->main.fset );
#else
  guiText_OnInit ( &guiTar.name, NULL, NULL );
  guiText_OnInit ( &guiTar.file, NULL, NULL );
  guiTar.main.vb   = IupVbox ( guiTar.name.fset, guiTar.file.fset,
                               guiTar.path.fset, guiTar.targ.fset,
                               guiTar.indx.fset,
                               guiBase->main.fset, NULL );
  guiTar.main.fset = IupFrame ( guiTar.main.vb );
#endif
  guiTar_OnLang();
}

void guiTar_OnLoad ( char *path )
{
  FILE *file = fopen( path, "rb" );
  fread ( &srcTar, sizeof ( DATA_TAR ), 1, file );
  changeEndian ( &srcTar.bases, sizeof ( BASES ), 'L', getEndian() );
  fclose( file );
}

void guiTar_OnSave ( char *path )
{
  FILE *file = fopen( path, "wb" );
  changeEndian ( &srcTar.bases, sizeof ( BASES ), getEndian(), 'L' );
  fwrite ( &srcTar, sizeof ( DATA_TAR ), 1, file );
  fclose( file );
}
void guiTar_OnApply ( void )
{
  srcTar = tmpTar;
  copystri ( appSession.tar, tmpTar.file, NAME_MAX );
}
void guiTar_OnReset ( void )
{
  tmpTar = srcTar;
  copystri ( appSession.tar, srcTar.file, NAME_MAX );
}
extern void guiPfm_OnDefPath ( char *path );
void guiTar_OnDefPath ( char *path )
{
  guiPfm_OnDefPath ( path );
  ipMkDir ( path );
  appendstr ( path, DIR_SEP, PATH_MAX );
  appendstr ( path, appSession.tar, PATH_MAX );
}
void guiTar_OnDefExt ( char *path )
{
  appendstr ( path, "m-tar", PATH_MAX );
}
extern char *srcBaseName;
extern char *tmpBaseName;
int guiTar_OnShow ( Ihandle *ih )
{
  float vi;
  appMethods.OnDefPath = guiTar_OnDefPath;
  appMethods.OnDefExt  = guiTar_OnDefExt;
  appMethods.OnLoad    = guiTar_OnLoad;
  appMethods.OnReset   = guiTar_OnReset;
  appMethods.OnSave    = guiTar_OnSave;
  appMethods.OnApply   = guiTar_OnApply;

  if ( !ih )
  {
    return IUP_DEFAULT;
  }

  IupSetAttribute ( guiTar.name.tb, "MEDIT_SRC_NAME", srcTar.name );
  IupSetAttribute ( guiTar.name.tb, "MEDIT_TMP_NAME", tmpTar.name );
  IupSetAttribute ( guiTar.file.tb, "MEDIT_SRC_FILE", srcTar.file );
  IupSetAttribute ( guiTar.file.tb, "MEDIT_TMP_FILE", tmpTar.file );
  IupSetAttribute ( guiTar.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute ( guiTar.indx.fset, "FLOATING", IUP_NO );
  guiText_SendShowMsg ( &guiTar.name, tmpTar.name );
  guiText_SendShowMsg ( &guiTar.file, tmpTar.file );
  guiText_SendShowMsg ( &guiTar.path, tmpTar.path );
  guiText_SendShowMsg ( &guiTar.targ, tmpTar.targ );
  IupShow ( guiTar.indx.fset );
  vi = IupGetFloat ( guiTar.indx.spin, IUP_VALUE );

  if ( vi > 0.0f )
  {
    int i = ( int ) ceilf ( vi ) - 1;
    guiBase = &guiTar.base;
    srcBase = &srcTar.bases.a[i];
    srcBaseName = srcTar.bname[i].a;
    tmpBase = &tmpTar.bases.a[i];
    tmpBaseName = tmpTar.bname[i].a;
    IupShow ( guiBase->main.fset );
    guiBase_OnShow ( guiBase->main.fset );
  }

  guiTar_OnLang();
  IupRefresh ( guiTar.main.fset );
  return IUP_DEFAULT;
}
int guiTarg_OnKAny ( Ihandle *ih, int c )
{
  c ^= 0xF0000000;

  if ( c < ' ' )
  {
    return IUP_DEFAULT;
  }
  else if ( strlen ( tmpTar.targ ) == NAME_MAX )
  {
    return IUP_CLOSE;
  }

  return IUP_DEFAULT;
}
int guiTarg_OnValueChanged ( Ihandle *ih )
{
  copystri ( tmpTar.targ, IupGetAttribute ( ih, IUP_VALUE ), NAME_MAX );
  return IUP_DEFAULT;
}
int guiPath_OnKAny ( Ihandle *ih, int c )
{
  c ^= 0xF0000000;

  if ( c < ' ' )
  {
    return IUP_DEFAULT;
  }
  else if ( strlen ( tmpTar.path ) == PATH_MAX )
  {
    return IUP_CLOSE;
  }

  return IUP_DEFAULT;
}
int guiPath_OnValueChanged ( Ihandle *ih )
{
  copystri ( tmpTar.path, IupGetAttribute ( ih, IUP_VALUE ), PATH_MAX );
  return IUP_DEFAULT;
}
#endif
