#include "guiRes.h"
GUI_RES guiRes = {{0}};
CODEL resCodes = {NULL};
int guiRes_OnShow( Ihandle *ih )
{
  guiCodes_SetGUI( &guiRes.codes, &resCodes );
  return IUP_DEFAULT;
}
