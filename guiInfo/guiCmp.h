#pragma once
#include "../_guiMain.h"

OPEN_C
typedef struct _GUI_CMP
{
  GUI_MAIN    main;
  Ihandle *listCmp;
} GUI_CMP;

void guiCmp_SetGUI( GUI_CMP *cmp );
void guiCmp_OnLang( void );

SHUT_C
