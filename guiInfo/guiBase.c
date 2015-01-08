#include "guiBase.h"
BASE *srcBase = NULL;
BASE *tmpBase = NULL;
GUI_BASE *guiBase = NULL;
uchar GetBase( char *name )
{
  for ( uchar i = 0; i < tmpTar.bases.c; ++i )
  {
    if ( _strcmpi( name, tmpTar.bname[i].a ) == 0 )
      return i;
  }
  return BASES_COUNT;
}
void  guiBase_OnLoad(  int fd, FILE *file );
void  guiBase_OnSave(  int fd, FILE *file );
void  guiBase_OnApply( void );
void  guiBase_OnReset( void );
void  guiBase_OnLang( void )
{
  IupSetAttribute( guiBase->main.fset,  IUP_TITLE, appLang->a[ LNG_MEMORY  ].a );
  IupSetAttribute( guiBase->name.fset,  IUP_TITLE, appLang->a[ LNG_NAME    ].a );
  IupSetAttribute( guiBase->addr.fset,  IUP_TITLE, appLang->a[ LNG_ADDRESS ].a );
  IupSetAttribute( guiBase->size.fset,  IUP_TITLE, appLang->a[ LNG_SIZE    ].a );
  IupSetAttribute( guiBase->depth.fset, IUP_TITLE, appLang->a[ LNG_DEPTH   ].a );
}
void guiBase_OnInit( void )
{
  guiText_OnInit( &guiBase->name,
                 (Icallback)guiName_OnKAny,  guiName_OnValueChanged  );
  guiSpin_OnInit( &guiBase->addr,
                 (Icallback)guiAddr_OnKAny,  guiAddr_OnValueChanged  );
  guiSpin_OnInit( &guiBase->size,
                 (Icallback)guiSize_OnKAny,  guiSize_OnValueChanged  );
  guiSpin_OnInit( &guiBase->depth,
                 (Icallback)guiDepth_OnKAny, guiDepth_OnValueChanged );
  guiBase->main.vb = IupVbox(
    guiBase->name.fset,
    guiBase->addr.fset,
    guiBase->size.fset,
    guiBase->depth.fset, NULL );
  guiBase->main.fset = IupFrame( guiBase->main.vb );
}
extern void guiTar_OnDefPath( char *path, uchar saveFile );
extern void guiTar_OnDefExt(  char *path );
int guiBase_OnShow( Ihandle *ih )
{
  appMethods.OnDefPath = guiTar_OnDefPath;
  appMethods.OnDefExt  = guiTar_OnDefExt;
  appMethods.OnLoad    = guiBase_OnLoad;
  appMethods.OnReset   = guiBase_OnReset;
  appMethods.OnSave    = guiBase_OnSave;
  appMethods.OnApply   = guiBase_OnApply;
  IupSetAttribute( guiBase->size.fset, "FLOATING", IUP_NO );
  IupShow( guiBase->size.fset );
  return IUP_DEFAULT;
}
void  guiBase_OnLoad( int fd, FILE *file )
{
  uchar i = GetBase( appSession.base );
  ipFdSetPos( fd, (int)((uintptr_t)&srcTar.bases.a[i] - (uintptr_t)&srcTar), IP_S_SET );
  ipFdRdBuff( fd, &srcTar.bases.a[i], sizeof(BASE) );
}
void  guiBase_OnReset( void ) { tmpTar = srcTar; }
void  guiBase_OnSave( int fd, FILE *file )
{
  uchar i = GetBase( appSession.base );
  ipFdSetPos( fd, (int)((uintptr_t)&srcTar.bases.a[i] - (uintptr_t)&srcTar), SEEK_SET );
  ipFdRdBuff( fd, &srcTar.bases.a[i], sizeof(BASE) );
}
void  guiBase_OnApply( void ) { srcTar = tmpTar; }
/// FIXME: Implement Memory Event handlers
int  guiAddr_OnKAny( Ihandle *ih, int c )
{
  if
  (
    c < ' ' ||
    ( c >= '0' && c <= '9' ) ||
    ( c >= 'A' && c <= 'F' ) ||
    ( c >= 'a' && c <= 'f' )
  )
    return IUP_DEFAULT;
  return IUP_CLOSE;
}
int  guiSize_OnKAny( Ihandle *ih, int c )
{
  if
  (
    c < ' ' ||
    ( c >= '0' && c <= '9' ) ||
    ( c >= 'A' && c <= 'F' ) ||
    ( c >= 'a' && c <= 'f' )
  )
    return IUP_DEFAULT;
  return IUP_CLOSE;
}
int guiDepth_OnKAny( Ihandle *ih, int c )
{
  if ( c < ' ' || ( c >= '0' && c <= '9' ) )
    return IUP_DEFAULT;
  return IUP_CLOSE;
}
int  guiAddr_OnValueChanged( Ihandle *ih )
{
  char *valStr = IupGetAttribute( ih, IUP_VALUE );
  tmpBase->addr   = strtoull( valStr, NULL, 16 );
  if ( tmpBase->addr > ( UINTPTR_MAX - tmpBase->size ) )
  {
    tmpBase->addr = (UINTPTR_MAX - tmpBase->size);
    _litoa_s( tmpBase->addr, valStr, strlen(valStr), 16 );
    IupSetStrAttribute( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
int  guiSize_OnValueChanged( Ihandle *ih )
{
  char *valStr = IupGetAttribute( ih, IUP_VALUE );
  tmpBase->size   = strtoull( valStr, NULL, 16 );
  if ( tmpBase->size > ( UINTPTR_MAX - tmpBase->addr ) )
  {
    tmpBase->size = (UINTPTR_MAX - tmpBase->addr);
    _litoa_s( tmpBase->size, valStr, strlen(valStr), 16 );
    IupSetStrAttribute( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
int guiDepth_OnValueChanged( Ihandle *ih )
{
  char *valStr = IupGetAttribute( ih, IUP_VALUE );
  tmpBase->depth  = (uchar)strtoul( valStr, NULL, 10 );
  if ( tmpBase->depth > 0xF )
  {
    tmpBase->depth = 0xF;
    _litoa_s( tmpBase->size, valStr, strlen(valStr), 10 );
    IupSetStrAttribute( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
