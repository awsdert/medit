#pragma once
#include "guiOrg.h"
#include "guiSpin.h"

OPEN_C

typedef struct _GUI_BASE
{
  GUI_MAIN  main;
  GUI_TEXT  name;
  GUI_TEXT  addr;
  GUI_TEXT  size;
  GUI_TEXT depth;
} GUI_BASE;

extern BASE     *srcBase;
extern BASE     *tmpBase;
extern GUI_BASE *guiBase;

void guiBase_OnLang( void );
int  guiBase_OnShow( Ihandle *ih );

int  guiAddr_OnKAny( Ihandle *ih, int c );
int  guiAddr_OnValueChanged( Ihandle *ih );

int  guiSize_OnKAny( Ihandle *ih, int c );
int  guiSize_OnValueChanged( Ihandle *ih );

int  guiDepth_OnKAny( Ihandle *ih, int c );
int  guiDepth_OnValueChanged( Ihandle *ih );

SHUT_C
