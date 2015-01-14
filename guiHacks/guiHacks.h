#pragma once
#include "guiHack.h"
#include "../guiInfo/guiPro.h"

OPEN_C

typedef struct _GUI_HACKS
{
  GUI_MAIN   main;
  Ihandle *fsetFormat;
  Ihandle *ddFormat;
  Ihandle *treeHacks;
} GUI_HACKS;

extern GUI_HACKS guiHacks;
void guiHacks_OnLang( void );
int  guiHacks_OnShow( Ihandle *ih );
void execHacksId( pid_t pid );

SHUT_C
