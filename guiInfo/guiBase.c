#include "../guiMain.h"
void guiBase_OnLang( void )
{
  GUI_BASE *guiBase = appGetGui()->base;
  LANG const *lang = appGetLang();
  IupSetAttribute( guiBase->base.fset, IUP_TITLE, lang->x[ LNG_MEMORY ] );
}
void guiBase_OnInit( void )
{
  GUI_BASE *guiBase = appGetGui()->base;
  guiSpin_OnInit( &guiBase->addr,  (Icallback)guiAddr_OnKAny, guiAddr_OnValueChanged );
  guiSpin_OnInit( &guiBase->size,  (Icallback)guiSize_OnKAny, guiSize_OnValueChanged );
  guiSpin_OnInit( &guiBase->depth, (Icallback)guiDepth_OnKAny, guiDepth_OnValueChanged );
  guiBase->base.vb = IupVbox(
#ifndef GUI_SHARED
    guiBase->name.fset,
#endif
    guiBase->addr.fset,
    guiBase->size.fset,
    guiBase->depth.fset, NULL );
  guiBase->base.fset = IupFrame( guiBase->base.vb );
  IupAppend( guiBase->main.vb, guiBase->base.fset );
  IupMap( guiBase->base.fset );
}

int guiBase_OnShow( Ihandle *ih )
{
  GUI_BASE *guiBase = appGetGui()->base;
  IupSetAttribute( guiBase->size.fset, "FLOATING", IUP_NO );
  IupShow( guiBase->size.fset );
  return IUP_DEFAULT;
}

/// FIXME: Implement Memory Event handlers
int  guiAddr_OnKAny( Ihandle *ih, int c ) { return IUP_DEFAULT; }
int  guiSize_OnKAny( Ihandle *ih, int c ) { return IUP_DEFAULT; }
int guiDepth_OnKAny( Ihandle *ih, int c ) { return IUP_DEFAULT; }
int  guiAddr_OnValueChanged( Ihandle *ih ) { return IUP_DEFAULT; }
int  guiSize_OnValueChanged( Ihandle *ih ) { return IUP_DEFAULT; }
int guiDepth_OnValueChanged( Ihandle *ih ) { return IUP_DEFAULT; }
