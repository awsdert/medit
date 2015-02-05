#pragma once
#include "../_guiMain.h"

OPEN_C
typedef struct _GUI_TEXT
{
  Ihandle *fset;
  Ihandle *tb;
} GUI_TEXT;
void guiText_SendShowMsg( GUI_TEXT *guiText, char *text );
void guiText_OnInit( GUI_TEXT *guiText, Icallback OnKAny,
                     Icallback OnValueChanged );
SHUT_C
