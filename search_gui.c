#include "search.h"

int meSeaMenu_OnClick( Ihandle *ph, int button, int pressed, int x, int y, char* status )
{
  MEGUI  *gui = meGetGui();
  int i = 0, c = 0, used = IupGetInt( ph, IUP_VALUE ) - 1;
  //search( used );
  return IUP_DEFAULT;
}

