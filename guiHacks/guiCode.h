#pragma once
#include "../guiInfo/guiSpin.h"
#include "../guiInfo/guiVal.h"
#include "../guiInfo/guiCmp.h"

OPEN_C
typedef struct _GUI_CODE
{
  GUI_MAIN main;
  GUI_MAIN code;
  GUI_CMP   cmp;
  GUI_TEXT indx;
  GUI_VAL   val;
  Ihandle *type;
} GUI_CODE;
extern GUI_CODE *guiCode;
void guiCode_SetGUI( GUI_CODE *code, CODE *src, CODE *tmp );
void guiCode_OnLang( void );
int  guiCode_OnShow( Ihandle *ih );

SHUT_C
