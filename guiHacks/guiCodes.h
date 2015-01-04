#pragma once
#include "guiCode.h"

OPEN_C

typedef struct _GUI_CODES
{
  GUI_MAIN      main;
  GUI_MAIN     codes;
  GUI_CODE      code;
  Ihandle *treeCodes;
  Ihandle   *hlMod;
  Ihandle   *hlPos;
} GUI_CODES;

extern GUI_CODES *guiCodes;
void guiCodes_SetGUI( GUI_CODES *codes, CODES *src, CODES *tmp );
void guiCodes_OnLang( void );
int  guiCodes_OnShow( Ihandle *ih );

SHUT_C
