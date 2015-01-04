#pragma once
#include "../_guiMain.h"

OPEN_C
typedef struct _GUI_SPIN
{
  Ihandle *fset;
  Ihandle *tb;
  Ihandle *spin;
} GUI_SPIN;

void guiSpin_OnInit( GUI_SPIN *guiSpin, Icallback OnKAny, Icallback OnValueChanged );
SHUT_C
