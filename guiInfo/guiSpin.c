#include "guiSpin.h"
void guiSpin_OnInit( GUI_TEXT *guiSpin, Icallback OnKAny,
                     Icallback OnValueChanged )
{
  guiText_OnInit( guiSpin, OnKAny, OnValueChanged );
  IupSetAttribute( guiSpin->tb, "SPIN", IUP_YES );
}
