#pragma once
#include "guiOrg.h"

OPEN_C
#ifndef EMULATOR
typedef struct _GUI_PFM
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  Ihandle *hlEndian;
} GUI_PFM;

extern GUI_PFM guiPfm;
void  guiPfm_OnLang( void );
int   guiPfm_OnShow( Ihandle *ih );
int   guiEndian_OnValueChanged( Ihandle *ih );
#endif
SHUT_C
