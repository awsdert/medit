#pragma once
#include "guiText.h"
OPEN_C
#ifndef EMULATOR
typedef struct _GUI_ORG
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
} GUI_ORG;

extern GUI_ORG guiOrg;
void  guiOrg_OnLang( void );
int   guiOrg_OnShow( Ihandle *ih );
#endif

extern char *srcName;
extern char *tmpName;
int   guiName_OnKAny( Ihandle *ih, int c );
int   guiName_OnValueChanged( Ihandle *ih );

extern char *srcFile;
extern char *tmpFile;
int   guiFile_OnKAny( Ihandle *ih, int c );
int   guiFile_OnValueChanged( Ihandle *ih );
SHUT_C
