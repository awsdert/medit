#pragma once
#include "guiPfm.h"
#include "guiBase.h"

OPEN_C
#ifndef EMULATOR
typedef struct _GUI_TAR
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  GUI_TEXT path;
  GUI_TEXT targ;
  GUI_SPIN indx;
  GUI_BASE base;
} GUI_TAR;

extern GUI_TAR guiTar;
void guiTar_OnLang( void );
int  guiTar_OnShow( Ihandle *ih );
#endif
SHUT_C
