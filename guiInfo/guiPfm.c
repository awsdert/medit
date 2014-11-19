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
  IupSetAttribute( gui->pfm.fsetEndian,             IUP_TITLE, lang->x[ LNG_ENDIAN ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_SYS ), IUP_TITLE, lang->x[ LNG_SYSTEM ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_L ),   IUP_TITLE, lang->x[ LNG_LITTLE ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_LB ),  IUP_TITLE, pfm_lb );
  IupSetAttribute( guiGetListPos_Lbl( hbox, ENDIAN_B ),   IUP_TITLE, lang->x[ LNG_BIG    ] );
}
void guiPfm_OnInit( void )
{
  GUI *gui = appGetGui();
  gui->pfm.listEndian = meMkList( NULL,
    MSTR( ENDIAN_SYS ), MSTR( ENDIAN_L ), MSTR( ENDIAN_LB ), MSTR( ENDIAN_B ), NULL );
  gui->pfm.fsetEndian = IupFrame( gui->pfm.listEndian );
  IupSetAttribute( gui->pfm.fsetEndian, "FLOATING", IUP_YES );
  IupSetAttribute( gui->pfm.fsetEndian, IUP_EXPAND, IUP_HORIZONTAL );
  // Main
#ifdef GUI_SHARED
  IupAppend( gui->pfm.main.vb, gui->pfm.fsetEndian );
  IupMap( gui->pfm.fsetEndian );
#else
  gui->pfm.tbName = IupText( NULL );
  gui->pfm.tbFile = IupText( NULL );
  gui->pfm.vbName  = IupVbox( gui->pfm.tbName, NULL );
  gui->pfm.vbFile  = IupVbox( gui->pfm.tbFile, NULL );
  IupSetAttribute( gui->pfm.fsetName, "FLOATING", IUP_YES );
  IupSetAttribute( gui->pfm.fsetFile, "FLOATING", IUP_YES );
  IupSetAttribute( gui->pfm.tbName, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( gui->pfm.tbFile, IUP_EXPAND, IUP_HORIZONTAL );
  gui->pfm.fsetName = IupFrame( gui->pfm.vbName );
  gui->pfm.fsetFile = IupFrame( gui->pfm.vbFile );
  gui->pfm.vbMain  = IupVbox( gui->pfm.fsetName, gui->pfm.fsetFile, gui->pfm.fsetEndian, NULL );
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
  IupSetInt( gui->pfm.listEndian, IUP_VALUE, (int)cfg->dst.pfm.endian );
  IupSetAttribute( gui->pfm.main.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.name.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.file.fset,  "FLOATING", IUP_NO );
  IupSetAttribute( gui->pfm.fsetEndian, "FLOATING", IUP_NO );
  IupShow( gui->pfm.name.fset );
  IupShow( gui->pfm.name.fset );
  IupShow( gui->pfm.fsetEndian );
  return IUP_DEFAULT;
}
#endif
