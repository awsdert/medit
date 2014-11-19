#include "../guiMain.h"
#ifndef EMULATOR
schar const pfm_lb_max = NAME_MAX * 2;
char pfm_lb[NAME_MAX*2] = {0};
void guiPfm_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *hbox = IupGetChild( gui->pfm.listEndian, 0 );
  memset( pfm_lb, 0, pfm_lb_max );
  strcpy_s( pfm_lb, pfm_lb_max, lang->x[ LNG_LITTLE ] );
  strcat_s( pfm_lb, pfm_lb_max, " " );
  strcat_s( pfm_lb, pfm_lb_max, lang->x[ LNG_BIG ] );
  IupSetAttribute( gui->pfm.main.fset,              IUP_TITLE, lang->x[ LNG_PLATFORM ] );
  IupSetAttribute( gui->pfm.listEndian,             IUP_TITLE, lang->x[ LNG_ENDIAN ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_SYS ), IUP_TITLE, lang->x[ LNG_SYSTEM ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_L ),   IUP_TITLE, lang->x[ LNG_LITTLE ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_LB ),  IUP_TITLE, pfm_lb );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_B ),   IUP_TITLE, lang->x[ LNG_BIG    ] );
#ifdef GUI_SHARED
  gui->base = &gui->pfm.base;
  guiBase_OnLang();
#endif
}
extern guiBase_OnInit( void );
void guiPfm_OnInit( void )
{
  GUI *gui = appGetGui();
  // Endian
  gui->pfm.listEndian = meMkList( guiEndian_OnValueChanged,
    MSTR( ENDIAN_SYS ), MSTR( ENDIAN_L ), MSTR( ENDIAN_LB ), MSTR( ENDIAN_B ), NULL );
  IupSetAttribute( gui->pfm.listEndian, "FLOATING", IUP_YES );
  IupSetAttribute( gui->pfm.listEndian, IUP_EXPAND, IUP_HORIZONTAL );
  // Main
#ifdef GUI_SHARED
  gui->base = &gui->pfm.base;
  gui->base->main.fset = gui->dlgMain;
  gui->base->main.vb   = gui->vbMain;
  guiBase_OnInit();
  IupAppend( gui->pfm.main.vb, gui->pfm.listEndian );
  IupMap( gui->pfm.listEndian );
#else
  guiText_OnInit( &gui->pfm.name, (Icallback)guiName_OnKAny, guiName_OnValueChanged );
  guiText_OnInit( &gui->pfm.file, (Icallback)guiFile_OnKAny, guiFile_OnValueChanged );
  gui->pfm.vbMain  = IupVbox( gui->pfm.name.fset, gui->pfm.file.fset, gui->pfm.listtEndian, NULL );
  IupSetAttribute( gui->pfm.fsetMain, "FLOATING", IUP_YES );
#endif
  guiPfm_OnLang();
}

int guiPfm_OnShow( Ihandle *ih )
{
  GUI *gui = appGetGui();
  CFG *cfg = appGetCfg();
  cfg->src.name = cfg->src.pfm.name;
  cfg->src.file = cfg->src.pfm.file;
  cfg->dst.name = cfg->dst.pfm.name;
  cfg->dst.file = cfg->dst.pfm.file;
  IupSetAttribute( gui->org.main.fset, IUP_TITLE, appGetText( LNG_PLATFORM ) );
  IupSetAttribute( gui->pfm.name.tb, IUP_TEXT, cfg->dst.name );
  IupSetAttribute( gui->pfm.file.tb, IUP_TEXT, cfg->dst.file );
  IupSetInt( gui->pfm.listEndian, IUP_VALUE, cfg->dst.pfm.endian );
  IupSetAttribute( gui->pfm.main.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.name.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.file.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.listEndian, "FLOATING", IUP_NO );
  IupShow( gui->pfm.name.fset );
  IupShow( gui->pfm.file.fset );
  IupShow( gui->pfm.listEndian );
#ifdef GUI_SHARED
  gui->base = &gui->pfm.base;
  IupShow( gui->base->base.fset );
#endif
  return IUP_DEFAULT;
}

int guiEndian_OnValueChanged( Ihandle *ih )
{
  CFG *cfg = appGetCfg();
  cfg->dst.pfm.endian = IupGetInt( ih, IUP_VALUE );
  return IUP_DEFAULT;
}
#endif
