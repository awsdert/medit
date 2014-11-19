#include "../guiMain.h"
void guiSpin_OnInit( GUI_SPIN *guiSpin, Icallback OnKAny, Icallback OnValueChanged )
{
  guiSpin->tb   = IupText(NULL);
  guiSpin->spin = IupSpinbox( guiSpin->tb );
  guiSpin->fset = IupFrame( guiSpin->spin );
  IupSetAttribute( guiSpin->spin, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiSpin->fset, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiSpin->fset, "FLOATING", IUP_YES );
  if ( OnKAny )
    IupSetCallback( guiSpin->tb, "K_ANY", OnKAny );
  if ( OnValueChanged )
    IupSetCallback( guiSpin->tb, "VALUECHANGED", OnValueChanged );
}
