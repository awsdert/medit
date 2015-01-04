#pragma once
#include "guiTar.h"

OPEN_C

typedef struct _GUI_PFL
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  Ihandle *rgRegion;
  Ihandle *rbRegion[ REGION_COUNT ];
} GUI_PFL;

extern GUI_PFL guiPfl;
void guiPfl_OnLang( void );
int  guiPfl_OnShow( Ihandle *ih );

SHUT_C
