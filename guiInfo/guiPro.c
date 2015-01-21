#include "guiPro.h"
GUI_PRO guiPro = {{NULL}};
void guiPro_OnLang( void )
{
  IupSetAttribute( guiPro.main.fset, IUP_TITLE, appLang->a[ LNG_PROFILE ].a );
  IupSetAttribute( guiPro.name.fset, IUP_TITLE, appLang->a[ LNG_NAME    ].a );
  IupSetAttribute( guiPro.file.fset, IUP_TITLE, appLang->a[ LNG_FILE    ].a );
}
extern void guiTar_OnInit( void );
void guiPro_OnInit( void )
{
  guiTar_OnInit();
#ifdef GUI_SHARED
  guiPro.main = guiOrg.main;
  guiPro.name = guiOrg.name;
  guiPro.file = guiOrg.file;
#else
  guiText_OnInit( &guiPro.name, (Icallback)guiName_OnKAny, guiName_OnValueChanged );
  guiText_OnInit( &guiPro.file, (Icallback)guiFile_OnKAny, guiFile_OnValueChanged );
  guiPro.main.vb = IupVbox( guiPro.name.fset, guiPro.file.fset, NULL );
  guiPro.main.fset = IupFrane( guiPro.main.vb );
#endif
  guiPro_OnLang();
}
extern void guiTar_OnDefPath( char *path );
void guiPro_OnDefPath( char *path )
{
  guiTar_OnDefPath( path );
  mkdir( path );
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, srcPro.file );
}
void guiPro_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, "m-pro" );
}
void guiPro_OnSave( int fd )
{
  ipFdWrBuff( fd, &srcPro, sizeof(DATA_PRO) );
}
void guiPro_OnApply( void )
{
  srcPro = tmpPro;
}
void guiPro_OnLoad( int fd )
{
  ipFdRdBuff( fd, &srcPro, sizeof(DATA_PRO) );
}
void guiPro_OnReset( void )
{
  tmpPro = srcPro;
}
int guiPro_OnShow( Ihandle *ih )
{
  IupSetAttribute( guiPro.name.tb, "MEDIT_SRC_NAME", srcPro.name );
  IupSetAttribute( guiPro.name.tb, "MEDIT_TMP_NAME", tmpPro.name );
  IupSetAttribute( guiPro.file.tb, "MEDIT_SRC_FILE", srcPro.file );
  IupSetAttribute( guiPro.file.tb, "MEDIT_TMP_FILE", tmpPro.file );
  appMethods.OnDefPath = guiPro_OnDefPath;
  appMethods.OnDefExt  = guiPro_OnDefExt;
  appMethods.OnLoad    = guiPro_OnLoad;
  appMethods.OnReset   = guiPro_OnReset;
  appMethods.OnSave    = guiPro_OnSave;
  appMethods.OnApply   = guiPro_OnApply;
  guiText_SendShowMsg( &guiPro.name, tmpPro.name );
  guiText_SendShowMsg( &guiPro.file, tmpPro.file );
  IupSetAttribute( guiPro.main.fset, "FLOATING", IUP_NO );
  guiPro_OnLang();
  return IUP_DEFAULT;
}
