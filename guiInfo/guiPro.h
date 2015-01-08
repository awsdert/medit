#pragma once
#include "guiTar.h"

OPEN_C

typedef struct _GUI_PRO
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  Ihandle *rgRegion;
  Ihandle *rbRegion[ REGION_COUNT ];
} GUI_PRO;

extern GUI_PRO guiPro;
void guiPro_OnLang( void );
int  guiPro_OnShow( Ihandle *ih );

SHUT_C
