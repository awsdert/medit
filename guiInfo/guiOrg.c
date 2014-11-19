#include "../guiMain.h"
// Example app would be PCSX2 which is primary testing ground for Medit v1
#ifndef EMULATOR
void guiOrg_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  IupSetAttribute( gui->org.main.fset, IUP_TITLE, lang->x[ LNG_ORGANISATION ] );
  IupSetAttribute( gui->org.name.fset, IUP_TITLE, lang->x[ LNG_NAME         ] );
  IupSetAttribute( gui->org.file.fset, IUP_TITLE, lang->x[ LNG_FILE         ] );
}

void guiOrg_OnInit( void )
{
  GUI *gui = appGetGui();
  guiText_OnInit( &gui->org.name, (Icallback)guiName_OnKAny, (Icallback)guiName_OnValueChanged );
  guiText_OnInit( &gui->org.file, (Icallback)guiFile_OnKAny, (Icallback)guiFile_OnValueChanged );
  gui->org.main.vb   = IupVbox(  gui->org.name.fset,  gui->org.file.fset,  NULL );
  gui->org.main.fset = IupFrame( gui->org.main.vb );
  IupSetAttribute( gui->org.main.fset, "FLOATING", IUP_YES );
  guiOrg_OnLang();
}

int guiOrg_OnShow( Ihandle *ih )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  CFG *cfg = appGetCfg();
  cfg->src.name = cfg->src.org.name;
  cfg->src.file = cfg->src.org.file;
  cfg->dst.name = cfg->dst.org.name;
  cfg->dst.file = cfg->dst.org.file;
  IupSetAttribute( gui->org.main.fset,  IUP_TITLE, appGetText( LNG_ORGANISATION ) );
  IupSetStrAttribute( gui->org.name.tb, IUP_TEXT,  cfg->dst.name );
  IupSetStrAttribute( gui->org.file.tb, IUP_TEXT,  cfg->dst.file );
  IupSetAttribute( gui->org.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->org.name.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->org.file.fset, "FLOATING", IUP_NO );
  IupShow( gui->org.name.fset );
  IupShow( gui->org.file.fset );
  return IUP_DEFAULT;
}
#endif
