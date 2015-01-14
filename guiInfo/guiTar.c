#include "guiTar.h"
#ifndef EMULATOR
GUI_TAR guiTar = {{NULL}};
void guiTar_OnDefPath( char *path, uchar saveFile );
void guiTar_OnDefExt(  char *path );
void guiTar_OnLoad( int fd, FILE *file );
void guiTar_OnReset( void );
void guiTar_OnSave( int fd, FILE *file );
void guiTar_OnApply( void );
void guiTar_OnLang( void )
{
  IupSetAttribute( guiTar.main.fset, IUP_TITLE, appLang->a[ LNG_TARGET ].a );
  IupSetAttribute( guiTar.name.fset, IUP_TITLE, appLang->a[ LNG_NAME   ].a );
  IupSetAttribute( guiTar.file.fset, IUP_TITLE, appLang->a[ LNG_FILE   ].a );
  IupSetAttribute( guiTar.path.fset, IUP_TITLE, appLang->a[ LNG_PATH   ].a );
  IupSetAttribute( guiTar.targ.fset, IUP_TITLE, appLang->a[ LNG_TARGET ].a );
  IupSetAttribute( guiTar.indx.fset, IUP_TITLE, appLang->a[ LNG_MEMORY ].a );
  guiBase = &guiTar.base;
  guiBase_OnLang();
}
extern void  guiPfm_OnInit( void );
extern void guiBase_OnInit( void );
int guiPath_OnKAny( Ihandle *ih, int c );
int guiTarg_OnKAny( Ihandle *ih, int c );
int guiPath_OnValueChanged( Ihandle *ih );
int guiTarg_OnValueChanged( Ihandle *ih );
void  guiTar_OnInit( void )
{
  guiPfm_OnInit();
  guiText_OnInit( &guiTar.path, (Icallback)guiPath_OnKAny, guiPath_OnValueChanged );
  guiText_OnInit( &guiTar.targ, (Icallback)guiTarg_OnKAny, guiTarg_OnValueChanged );
  guiBase = &guiTar.base;
  guiBase_OnInit();
#ifdef GUI_SHARED
  guiTar.main =  guiOrg.main;
  guiTar.name =  guiOrg.name;
  guiTar.file =  guiOrg.file;
  IupAppend( guiTar.main.vb, guiTar.path.fset );
  IupAppend( guiTar.main.vb, guiTar.targ.fset );
  IupAppend( guiTar.main.vb, guiTar.base.main.fset );
  IupMap( guiTar.path.fset );
  IupMap( guiTar.targ.fset );
  IupMap( guiTar.base.main.fset );
#else
  guiText_OnInit( &guiTar.name, NULL, NULL );
  guiText_OnInit( &guiTar.file, NULL, NULL );
  guiTar.main.vb   = IupVbox(  guiTar.name.fset, guiTar.file.fset,
                                guiTar.path.fset, guiTar.targ.fset,
                                guiTar.base.main.fset, NULL );
  guiTar.main.fset = IupFrame( guiTar.main.vb );
  IupSetAttribute( guiTar.main.fset, "FLOATING", IUP_YES );
#endif
  guiTar_OnLang();
}

int guiTar_OnShow( Ihandle *ih )
{
  srcName = srcTar.name;
  srcFile = srcTar.file;
  tmpName = tmpTar.name;
  tmpFile = tmpTar.file;
  appMethods.OnDefPath = guiTar_OnDefPath;
  appMethods.OnDefExt  = guiTar_OnDefExt;
  appMethods.OnLoad    = guiTar_OnLoad;
  appMethods.OnReset   = guiTar_OnReset;
  appMethods.OnSave    = guiTar_OnSave;
  appMethods.OnApply   = guiTar_OnApply;
  guiText_SendShowMsg( &guiTar.name, tmpName );
  guiText_SendShowMsg( &guiTar.file, tmpFile );
  guiText_SendShowMsg( &guiTar.path, tmpTar.path );
  guiText_SendShowMsg( &guiTar.targ, tmpTar.targ );
  IupSetAttribute( guiTar.main.fset, "FLOATING", IUP_NO );
  if ( IupGetInt( guiTar.indx.spin, IUP_VALUE ) > 0 )
  {
    guiBase = &guiTar.base;
    IupShow( guiTar.base.main.fset );
  }
  guiTar_OnLang();
  return IUP_DEFAULT;
}
void guiTar_OnLoad( int fd, FILE *file )
{
  ipFdRdBuff( fd, &srcTar, sizeof(DATA_TAR) );
  changeEndian( &srcTar.bases, sizeof(BASES), 'L', getEndian() );
}
void guiTar_OnReset( void ) { tmpTar = srcTar; }
void guiTar_OnSave( int fd, FILE *file )
{
  changeEndian( &srcTar.bases, sizeof(BASES), getEndian(), 'L' );
  ipFdWrBuff( fd, &srcTar, sizeof(DATA_TAR) );
}
void guiTar_OnApply( void ) { srcTar = tmpTar; }
extern void guiPfm_OnDefPath( char *path );
void guiTar_OnDefPath( char *path )
{
  guiPfm_OnDefPath( path );
  mkdir( path );
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, srcTar.file );
}
void guiTar_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, "m-tar" );
}
int guiTarg_OnKAny( Ihandle *ih, int c )
{
  c ^= 0xF0000000;
  if ( c < ' ' )
    return IUP_DEFAULT;
  else if ( strlen( tmpTar.targ ) == NAME_MAX )
    return IUP_CLOSE;
  return IUP_DEFAULT;
}
int guiTarg_OnValueChanged( Ihandle *ih )
{
  strcpy_s( tmpTar.targ, NAME_MAX, IupGetAttribute( ih, IUP_VALUE ) );
  return IUP_DEFAULT;
}
int guiPath_OnKAny( Ihandle *ih, int c )
{
  c ^= 0xF0000000;
  if ( c < ' ' )
    return IUP_DEFAULT;
  else if ( strlen( tmpTar.path ) == PATH_MAX )
    return IUP_CLOSE;
  return IUP_DEFAULT;
}
int guiPath_OnValueChanged( Ihandle *ih )
{
  strcpy_s( tmpTar.path, PATH_MAX, IupGetAttribute( ih, IUP_VALUE ) );
  return IUP_DEFAULT;
}
#endif
