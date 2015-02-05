#pragma once
#include "../_guiMain.h"
#include "guiText.h"

OPEN_C
typedef struct _GUI_SPIN
{
  Ihandle *fset;
  Ihandle *tb;
  Ihandle *spin;
} GUI_SPIN;

void guiSpin_OnInit( GUI_TEXT *guiSpin, Icallback OnKAny,
                     Icallback OnValueChanged );
SHUT_C
