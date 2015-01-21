#include "guiBase.h"
char *srcBaseName = NULL;
char *tmpBaseName = NULL;
BASE *srcBase = NULL;
BASE *tmpBase = NULL;
GUI_BASE *guiBase = NULL;
uchar GetBase ( char *name )
{
  for ( uchar i = 0; i < tmpTar.bases.c; ++i )
  {
    if ( _strcmpi ( name, tmpTar.bname[i].a ) == 0 )
    {
      return i;
    }
  }
  return BASES_COUNT;
}
void  guiBase_OnLang ( void )
{
  IupSetAttribute ( guiBase->main.fset,  IUP_TITLE, appLang->a[ LNG_MEMORY  ].a );
  IupSetAttribute ( guiBase->name.fset,  IUP_TITLE, appLang->a[ LNG_NAME    ].a );
  IupSetAttribute ( guiBase->addr.fset,  IUP_TITLE, appLang->a[ LNG_ADDRESS ].a );
  IupSetAttribute ( guiBase->size.fset,  IUP_TITLE, appLang->a[ LNG_SIZE    ].a );
  IupSetAttribute ( guiBase->depth.fset, IUP_TITLE, appLang->a[ LNG_DEPTH   ].a );
}
void guiBase_OnInit ( void )
{
  guiText_OnInit ( &guiBase->name,
                   ( Icallback ) guiName_OnKAny,  guiName_OnValueChanged );
  guiSpin_OnInit ( &guiBase->addr,
                   ( Icallback ) guiAddr_OnKAny,  guiAddr_OnValueChanged );
  IupSetfAttribute( guiBase->addr.tb, "SPINMAX", "%" PRI_LI "u", LI( ~0u ) );
  guiSpin_OnInit ( &guiBase->size, ( Icallback ) guiSize_OnKAny,  guiSize_OnValueChanged );
  IupSetStrf( guiBase->size.tb, "SPINMAX", "%" PRI_LI "u", LI( ~0u ) );
  guiSpin_OnInit ( &guiBase->depth, ( Icallback ) guiDepth_OnKAny, guiDepth_OnValueChanged );
  IupSetStrAttribute( guiBase->depth.tb, "SPINMAX", "15" );
  guiBase->main.vb = IupVbox (
                       guiBase->name.fset,
                       guiBase->addr.fset,
                       guiBase->size.fset,
                       guiBase->depth.fset, NULL );
  guiBase->main.fset = IupFrame ( guiBase->main.vb );
  guiBase_OnLang();
}
extern void guiTar_OnDefPath ( char *path );
extern void guiTar_OnDefExt ( char *path );
void  guiBase_OnLoad ( int fd )
{
  uchar i = GetBase ( appSession.base );
  ipFdSetPos ( fd, ( int ) ( ( uintptr_t ) &srcTar.bases.a[i] - ( uintptr_t ) &srcTar ), IP_S_SET );
  ipFdRdBuff ( fd, &srcTar.bases.a[i], sizeof ( BASE ) );
}
void  guiBase_OnReset ( void )
{
  tmpTar = srcTar;
}
void  guiBase_OnSave ( int fd )
{
  uchar i = GetBase ( appSession.base );
  ipFdSetPos ( fd, ( int ) ( ( uintptr_t ) &srcTar.bases.a[i] - ( uintptr_t ) &srcTar ), SEEK_SET );
  ipFdRdBuff ( fd, &srcTar.bases.a[i], sizeof ( BASE ) );
}
void  guiBase_OnApply ( void )
{
  srcTar = tmpTar;
}
int guiBase_OnShow ( Ihandle *ih )
{
  appMethods.OnDefPath = guiTar_OnDefPath;
  appMethods.OnDefExt  = guiTar_OnDefExt;
  appMethods.OnLoad    = guiBase_OnLoad;
  appMethods.OnReset   = guiBase_OnReset;
  appMethods.OnSave    = guiBase_OnSave;
  appMethods.OnApply   = guiBase_OnApply;
  IupSetAttribute ( guiBase->main.fset,  "FLOATING", IUP_NO );
  IupSetAttribute ( guiBase->name.tb, "MEDIT_SRC_NAME", srcBaseName );
  IupSetAttribute ( guiBase->name.tb, "MEDIT_TMP_NAME", tmpBaseName );
  guiBase_OnLang();
  return IUP_DEFAULT;
}
/// FIXME: Implement Memory Event handlers
int  guiAddr_OnKAny ( Ihandle *ih, int c )
{
  if
  (
    c < ' ' ||
    ( c >= '0' && c <= '9' ) ||
    ( c >= 'A' && c <= 'F' ) ||
    ( c >= 'a' && c <= 'f' )
  )
  {
    return IUP_DEFAULT;
  }
  return IUP_CLOSE;
}
int  guiSize_OnKAny ( Ihandle *ih, int c )
{
  if
  (
    c < ' ' ||
    ( c >= '0' && c <= '9' ) ||
    ( c >= 'A' && c <= 'F' ) ||
    ( c >= 'a' && c <= 'f' )
  )
  {
    return IUP_DEFAULT;
  }
  return IUP_CLOSE;
}
int guiDepth_OnKAny ( Ihandle *ih, int c )
{
  if ( c < ' ' || ( c >= '0' && c <= '9' ) )
  {
    return IUP_DEFAULT;
  }
  return IUP_CLOSE;
}
int  guiAddr_OnValueChanged ( Ihandle *ih )
{
  char *valStr = IupGetAttribute ( ih, IUP_VALUE );
  tmpBase->addr   = strtoull ( valStr, NULL, 16 );
  if ( tmpBase->addr > ( UINTPTR_MAX - tmpBase->size ) )
  {
    tmpBase->addr = ( UINTPTR_MAX - tmpBase->size );
    sprintf_s ( valStr, strlen ( valStr ), "%" PRI_LI "X", tmpBase->addr );
    IupSetStrAttribute ( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
int  guiAddr_OnSpin( Ihandle *ih, int inc )
{
  char valStr[22] = {0};
  tmpBase->addr += inc;
  if ( tmpBase->addr > ( UINTPTR_MAX - tmpBase->size ) )
    tmpBase->addr = ( UINTPTR_MAX - tmpBase->size );
  sprintf_s ( valStr, 22, "%" PRI_LI "X", tmpBase->addr );
  IupSetStrAttribute ( ih, IUP_VALUE, valStr );
  return IUP_DEFAULT;
}
int  guiSize_OnValueChanged ( Ihandle *ih )
{
  char *valStr = IupGetAttribute ( ih, IUP_VALUE );
  tmpBase->size   = strtoull ( valStr, NULL, 16 );
  if ( tmpBase->size > ( UINTPTR_MAX - tmpBase->addr ) )
  {
    tmpBase->size = ( UINTPTR_MAX - tmpBase->addr );
    sprintf_s ( valStr, strlen ( valStr ), "%" PRI_LI "X", tmpBase->size );
    IupSetStrAttribute ( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
int guiSize_OnSpin( Ihandle *ih, int inc )
{
  char valStr[22] = {0};
  tmpBase->size += inc;
  if ( tmpBase->size > ( UINTPTR_MAX - tmpBase->addr ) )
    tmpBase->size = ( UINTPTR_MAX - tmpBase->addr );
  sprintf_s ( valStr, 22, "%" PRI_LI "X", tmpBase->size );
  IupSetStrAttribute ( ih, IUP_VALUE, valStr );
  return IUP_DEFAULT;
}
int guiDepth_OnValueChanged ( Ihandle *ih )
{
  char *valStr = IupGetAttribute ( ih, IUP_VALUE );
  tmpBase->depth  = ( uchar ) strtoul ( valStr, NULL, 10 );
  if ( tmpBase->depth > 0xF )
  {
    tmpBase->depth = 0xF;
    sprintf_s ( valStr, strlen ( valStr ), "%" PRI_LI "X", tmpBase->size );
    IupSetStrAttribute ( ih, IUP_VALUE, valStr );
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}
