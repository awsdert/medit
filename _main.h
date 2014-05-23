#pragma once
#ifndef __MAIN_H
#define __MAIN_H

//#include <varargs.h>
#include "lang.h"

typedef struct _MEGUI_MAIN
{
  Ihandle *main_dlg;
  Ihandle *main_vb;
  Ihandle *main_dd;
} MEGUI_MAIN;

typedef struct _MEGUI_SEARCH
{
  Ihandle *main_vb;
  Ihandle *stype_sbox;
  Ihandle *stype_hbox;
  Ihandle *value_tbox;
  Ihandle *vtype_dd;
} MEGUI_SEARCH;

typedef struct _MEGUI
{
  char font[20];
  char fontSize[5];
  MEGUI_MAIN   main;
  MEGUI_SEARCH sea;
} MEGUI;

MEGUI* meGetGui( void );

Ihandle* meMkList( Icallback func, ... );
int     meListCB( Ihandle    *ih, int button, int pressed, int x, int y, char* status );
/* Unsafe - Assumes there will be enough char elements, will NULL terminate */
void ftoa( double value, char* dst, int sigDigits );
/* Safer version */
void ftoa_s( double value, char *dst, uzv length, int sigDigits );

#endif
