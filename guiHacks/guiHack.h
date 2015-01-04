#pragma once
#include "guiCodes.h"
#include "../guiInfo/guiOrg.h"

OPEN_C

typedef struct _GUI_HACK
{
  GUI_MAIN   main;
  GUI_TEXT   name;
  GUI_CODES codes;
  Ihandle *spinID;
} GUI_HACK;

extern GUI_HACK guiHack;
void guiHack_OnLang( void );
int  guiHack_OnShow( Ihandle *ih );

SHUT_C
