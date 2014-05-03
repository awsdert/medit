#pragma once
#ifndef __MAIN_H
#define __MAIN_H
#include <iup.h>

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
  MEGUI_MAIN   main;
  MEGUI_SEARCH sea;
} MEGUI;

MEGUI* meGetGui( void );

#endif
