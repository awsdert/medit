#pragma once
#include "../guiInfo/guiSpin.h"
#include "../guiHacks/guiCodes.h"

OPEN_C
typedef struct _GUI_RES
{
  GUI_MAIN     main;
  GUI_SPIN    index;
  GUI_SPIN   maxcmp;
  GUI_CODES   codes;
  Ihandle *gridData;
} GUI_RES;

extern GUI_RES guiRes;
void guiRes_OnLang( void );
int  guiRes_OnShow( Ihandle *ih );
SHUT_C
