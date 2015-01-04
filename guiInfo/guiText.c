#include "guiText.h"
void guiText_SendShowMsg( GUI_TEXT *guiText, char *text )
{
  IupSetAttribute( guiText->tb, IUP_VALUE, text );
  IupSetAttribute( guiText->fset, "FLOATING", IUP_NO );
  IupShow( guiText->fset );
}
void guiText_OnInit( GUI_TEXT *guiText, Icallback OnKAny, Icallback OnValueChanged )
{
  guiText->tb   = IupText(NULL);
  guiText->fset = IupFrame( guiText->tb );
  IupSetAttribute( guiText->tb,   IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiText->fset, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiText->fset, "FLOATING", IUP_YES );
  if ( OnKAny )
    IupSetCallback( guiText->tb, "K_ANY", OnKAny );
  if ( OnValueChanged )
    IupSetCallback( guiText->tb, "VALUECHANGED", OnValueChanged );
}
