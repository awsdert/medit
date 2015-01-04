#include "guiHack.h"
GUI_HACK guiHack = {{NULL}};
HACK_LIB_COM *hCOM = NULL;
HACK *srcHack = NULL;
HACK *tmpHack = NULL;
CODES srcHackCodes = {0};
CODES tmpHackCodes = {0};
void guiHack_OnLang( void )
{
  IupSetAttribute( guiHack.main.fset, IUP_TITLE, appLang->x[ LNG_HACK ] );
  IupSetAttribute( guiHack.name.fset, IUP_TITLE, appLang->x[ LNG_NAME ] );
}
extern void guiCodes_OnInit( void );
void guiHack_OnInit( void )
{
  guiCodes_SetGUI( &guiHack.codes, &srcHackCodes, &tmpHackCodes );
  guiCodes_OnInit();
}
int guiHack_OnShow( Ihandle *ih )
{
  return IUP_DEFAULT;
}
