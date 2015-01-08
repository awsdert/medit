#include "guiPfm.h"
#ifndef EMULATOR
GUI_PFM guiPfm = {{NULL}};
void  guiPfm_OnLang( void )
{
  Ihandle *hbox = IupGetChild( guiPfm.hlEndian, 0 );
  IupSetAttribute( guiPfm.main.fset, IUP_TITLE, appLang->a[ LNG_PLATFORM ].a );
  IupSetAttribute( guiPfm.name.fset, IUP_TITLE, appLang->a[ LNG_NAME     ].a );
  IupSetAttribute( guiPfm.file.fset, IUP_TITLE, appLang->a[ LNG_FILE     ].a );
  IupSetAttribute( guiPfm.hlEndian,  IUP_TITLE, appLang->a[ LNG_ENDIAN   ].a );
  IupSetAttribute( guiPfm.hlEndian,  "1", appLang->a[ LNG_SYSTEM   ].a );
  IupSetAttribute( guiPfm.hlEndian,  "2", appLang->a[ LNG_LITTLE   ].a );
  IupSetAttribute( guiPfm.hlEndian,  "3", appLang->a[ LNG_PDP      ].a );
  IupSetAttribute( guiPfm.hlEndian,  "4", appLang->a[ LNG_BIG      ].a );
}
extern void guiOrg_OnInit( void );
void  guiPfm_OnInit( void )
{
  guiOrg_OnInit();
  // Endian
  guiPfm.hlEndian = IupHList(NULL);
  IupSetAttribute( guiPfm.hlEndian, "FLOATING", IUP_YES );
  IupSetAttribute( guiPfm.hlEndian, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback( guiPfm.hlEndian, "VALUECHANGED_CB", guiEndian_OnValueChanged );
  // Main
#ifdef GUI_SHARED
  guiPfm.main = guiOrg.main;
  guiPfm.name = guiOrg.name;
  guiPfm.file = guiOrg.file;
  IupAppend( guiPfm.main.vb, guiPfm.hlEndian );
  IupMap( guiPfm.hlEndian );
#else
  guiText_OnInit( &guiPfm.name, (Icallback)guiName_OnKAny, guiName_OnValueChanged );
  guiText_OnInit( &guiPfm.file, (Icallback)guiFile_OnKAny, guiFile_OnValueChanged );
  guiPfm.main.vb   = IupVbox(  guiPfm.name.fset, guiPfm.file.fset,
                                guiPfm.listtEndian, NULL );
  guiPfm.main.fset = IupFrame( guiPfm.main.vb );
  IupSetAttribute( guiPfm.fsetMain, "FLOATING", IUP_YES );
#endif
  guiPfm_OnLang();
}
void  guiPfm_OnDefPath( char *path, uchar saveFile );
void  guiPfm_OnDefExt(  char *path );
void  guiPfm_OnLoad( int fd, FILE *file );
void  guiPfm_OnReset( void );
void  guiPfm_OnSave( int fd, FILE *file );
void  guiPfm_OnApply( void );
int   guiPfm_OnShow( Ihandle *ih )
{
  srcName = srcPfm.name;
  srcFile = srcPfm.file;
  tmpName = tmpPfm.name;
  tmpFile = tmpPfm.file;
  appMethods.OnDefPath = guiPfm_OnDefPath;
  appMethods.OnDefExt  = guiPfm_OnDefExt;
  appMethods.OnLoad    = guiPfm_OnLoad;
  appMethods.OnReset   = guiPfm_OnReset;
  appMethods.OnSave    = guiPfm_OnSave;
  appMethods.OnApply   = guiPfm_OnApply;
  guiText_SendShowMsg( &guiPfm.name, tmpName );
  guiText_SendShowMsg( &guiPfm.file, tmpFile );
  IupSetInt( guiPfm.hlEndian, IUP_VALUE, tmpPfm.endian );
  IupSetAttribute( guiPfm.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( guiPfm.hlEndian, "FLOATING", IUP_NO );
  IupShow( guiPfm.hlEndian );
  guiPfm_OnLang();
  return IUP_DEFAULT;
}
extern void guiOrg_OnDefPath( char *path, uchar saveFile );
void guiPfm_OnDefPath( char *path, uchar saveFile )
{
  guiOrg_OnDefPath( path, 1 );
  if ( !saveFile )
    return;
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, srcPfm.file );
}
void guiPfm_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, "m-pfm" );
}
void guiPfm_OnLoad( int fd, FILE *file ) { ipFdRdBuff( fd, &srcPfm, sizeof(DATA_PFM) ); }
void guiPfm_OnApply( void ) { srcPfm = tmpPfm; }
void guiPfm_OnSave( int fd, FILE *file ) { ipFdWrBuff( fd, &srcPfm, sizeof(DATA_PFM) ); }
void guiPfm_OnReset( void ) { tmpPfm = srcPfm; }
int guiEndian_OnValueChanged( Ihandle *ih )
{
  tmpPfm.endian = IupGetInt( ih, IUP_VALUE );
  return IUP_DEFAULT;
}
#endif
