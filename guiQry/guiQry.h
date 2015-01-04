#pragma once
#include "../guiInfo/guiOrg.h"
#include "guiRes.h"
#include "qry.h"

OPEN_C
typedef enum _QRY_T
{
  QRY_ANY = 0,
  QRY_MUST,
  QRY_NONE,
  QRY_COUNT
} QRY_T;

typedef struct _GUI_QRY
{
  GUI_MAIN     main;
  GUI_CMP       cmp;
  GUI_SPIN    index;
  Ihandle    *hlQry;
  Ihandle   *hlBase;
  uchar q; uchar  c;
#ifdef GUI_SHARED
  GUI_VAL val;
#else
  GUI_VAL val[QRY_COUNT][CMP_COUNT];
#endif
} GUI_QRY;

extern GUI_QRY guiQry;
void  guiQry_OnLang( void );
int   guiQry_OnShow( Ihandle *ih );
int   guiQryMenu_OnClick( Ihandle *ih,
        int button, int pressed, int x, int y, char* status );

SHUT_C
