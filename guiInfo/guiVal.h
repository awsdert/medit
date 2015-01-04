#pragma once
#include "../_guiMain.h"

OPEN_C
typedef enum _MEVAL
{
  VAL_SCHAR = '\0',
  VAL_UCHAR,
  VAL_SHORT,
  VAL_USHORT,
  VAL_LONG,
  VAL_ULONG,
  VAL_LINT,
  VAL_ULINT,
  VAL_INT,
  VAL_UINT,
  VAL_FPN,
  VAL_DPN,
  VAL_LPN,
  VAL_COUNT
} MEVAL_T;
typedef struct _GUI_VAL
{
  GUI_MAIN    main;
  GUI_MAIN   value;
  Ihandle *tbValue;
  Ihandle  *hlType;
} GUI_VAL;

void guiVal_SetGUI( GUI_VAL *val );
void guiVal_OnLang( void );
int  guiVal_OnKAny( Ihandle *ih, int c );
int  guiVal_OnValueChanged( Ihandle *ih );
SHUT_C
