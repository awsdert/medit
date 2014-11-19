#include "_guiMain.h"
int meMenu_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  GUI   *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *kid = IupGetChild( ih, 0 ), *fset[] = {
    gui->org.main.fset,
    gui->pfm.main.fset,
    gui->tar.main.fset,
    gui->qry.main.fset
  };
  Icallback cb[] = {
    guiOrg_OnShow,
    guiPfm_OnShow,
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
  for ( i = 0; i < 4; ++i )
  {
    if ( strcmp( lang->x[ j[i] ] , title ) == 0 )
    {
      IupShow( fset[i] );
      cb[i]( fset[i] );
      break;
    }
  }
  // Force Update GUI
  IupRefresh( gui->vbMain );
  IupRedraw( gui->vbMain, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
