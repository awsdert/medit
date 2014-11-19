#include "../guiMain.h"

void guiTar_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  IupSetAttribute( gui->tar.main.fset, IUP_TITLE, lang->x[ LNG_TARGET ] );
  IupSetAttribute( gui->tar.name.fset, IUP_TITLE, lang->x[ LNG_NAME   ] );
  IupSetAttribute( gui->tar.file.fset, IUP_TITLE, lang->x[ LNG_FILE   ] );
  IupSetAttribute( gui->tar.path.fset, IUP_TITLE, lang->x[ LNG_PATH   ] );
  IupSetAttribute( gui->tar.targ.fset, IUP_TITLE, lang->x[ LNG_TARGET ] );
}

void guiTar_OnInit( void )
{
  GUI *gui = appGetGui();
  guiText_OnInit( &gui->tar.path, NULL, NULL );
  guiText_OnInit( &gui->tar.targ, NULL, NULL );
#ifndef GUI_SHARED
  guiText_OnInit( &gui->tar.name, NULL, NULL );
  guiText_OnInit( &gui->tar.file, NULL, NULL );
  gui->tar.main.vb   = IupVbox( gui->tar.name.fset, gui->tar.file.fset,
                                gui->tar.path.fset, gui->tar.targ.fset, NULL );
  gui->tar.main.fset = IupFrame( gui->tar.main.vb );
  IupSetAttribute( gui->tar.main.fset, "FLOATING", IUP_YES );
#else
  gui->tar.main = gui->org.main;
  gui->tar.name = gui->org.name;
  gui->tar.file = gui->org.file;
  IupAppend( gui->tar.main.vb, gui->tar.path.fset );
  IupAppend( gui->tar.main.vb, gui->tar.targ.fset );
  IupMap( gui->tar.path.fset );
  IupMap( gui->tar.targ.fset );
#endif
  guiTar_OnLang();
}

int guiTar_OnShow( Ihandle *ih )
{
  GUI *gui = appGetGui();
  CFG *cfg = appGetCfg();
  cfg->src.name = cfg->src.tar.name;
  cfg->src.file = cfg->src.tar.file;
  cfg->dst.name = cfg->dst.tar.name;
  cfg->dst.file = cfg->dst.tar.file;
  IupSetAttribute( gui->tar.main.fset, IUP_TITLE, appGetText( LNG_TARGET ) );
  IupSetAttribute( gui->tar.name.tb, IUP_TEXT, cfg->dst.name );
  IupSetAttribute( gui->tar.file.tb, IUP_TEXT, cfg->dst.file );
  IupSetAttribute( gui->tar.path.tb, IUP_TEXT, cfg->dst.tar.path );
  IupSetAttribute( gui->tar.targ.tb, IUP_TEXT, cfg->dst.tar.targ );
  IupSetAttribute( gui->tar.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->tar.name.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->tar.file.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->tar.path.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->tar.targ.fset, "FLOATING", IUP_NO );
  IupShow( gui->tar.name.fset );
  IupShow( gui->tar.file.fset );
  IupShow( gui->tar.path.fset );
  IupShow( gui->tar.targ.fset );
  return IUP_DEFAULT;
}

