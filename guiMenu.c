#include "_guiMain.h"
int guiMenu_OnValueChanged( Ihandle *ih )
{
  GUI   *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *kid = IupGetChild( ih, 0 ), *fset[] = {
       gui->org.main.fset,
       gui->pfm.main.fset,
  gui->pfm.base.base.fset,
       gui->tar.main.fset,
       gui->qry.main.fset
  };
  Icallback cb[] = {
    guiOrg_OnShow,
    guiPfm_OnShow,
   guiBase_OnShow,
    guiTar_OnShow,
    guiQry_OnShow
  };
  char  *title = IupGetAttribute( kid, IUP_TITLE );
  int i = 1, c = IupGetChildCount( gui->vbMain ), j[] = {
    LNG_ORGANISATION,
    LNG_PLATFORM,
    LNG_TARGET,
    LNG_SEARCH
  };
  // Hide all groups first
  for ( ; i < c; ++i )
  {
    kid = IupGetChild( gui->vbMain, i );
    IupSetAttribute( kid, "FLOATING", IUP_YES );
    IupHide( kid );
  }
  for ( i = 0, c = IupGetChildCount( gui->org.main.vb ); i < c; ++i )
  {
    kid = IupGetChild( gui->org.main.vb, i );
    IupSetAttribute( kid, "FLOATING", IUP_YES );
    IupHide( kid );
  }
  // Find and show appropriate group
  i = IupGetInt( ih, IUP_VALUE );
  if ( _strcmpi( title, lang->x[ LNG_MEMORY ] ) == 0 )
  {
#ifdef GUI_SHARED
    gui->base = &gui->pfm.base;
#else
    /// FIXME: Implement unshared pfm.base in menu selection
#endif
  }
  IupShow( fset[i] );
  cb[i]( fset[i] );
  // Force Update GUI
  IupRefresh( gui->vbMain );
  IupRedraw( gui->vbMain, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
