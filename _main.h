#pragma once
#ifndef __MAIN_H
#define __MAIN_H

#include "lang.h"
#include "data.h"

typedef struct _MEGUI_MAIN
{
  Ihandle *main_dlg;
  Ihandle *main_vbox;
  Ihandle *main_dd;
} MEGUI_MAIN;

typedef enum   _MECMP
{
  /* ==, != */
  CMP_EQ = '\0',
  CMP_NE,
  /* >, >= */
  CMP_MT, CMP_ME,
  /* <, <= */
  CMP_LT, CMP_LE,
  /* &, !& */
  CMP_IA, CMP_NA,
  /* |, !| */
  CMP_IO, CMP_NO,
  /* ^, !^ */
  CMP_IX, CMP_NX,
  /* ~, !~ */
  CMP_IR, CMP_NR,
  CMP_COUNT,
  CMP_DUMP,
  CMP_UNDO,
  CMP_REDO
} MECMP_T;

typedef struct _MEGUI_CMP
{
  Ihandle *main_fset;
  Ihandle *main_vbox;
  Ihandle *main_list;
  Ihandle *dump_fset;
  Ihandle *redo_fset;
  Ihandle *undo_fset;
} MEGUI_CMP;

typedef enum _MEVAL
{
  VAL_SCHAR = '\0',
  VAL_UCHAR,
  VAL_SSHRT,
  VAL_USHRT,
  VAL_SLONG,
  VAL_ULONG,
  VAL_SHUGE,
  VAL_UHUGE,
  VAL_SINT,
  VAL_UINT,
  VAL_FPN,
  VAL_DPN,
  VAL_LPN,
  VAL_COUNT
} MEVAL_T;

typedef struct _MEGUI_VAL
{
  Ihandle *main_fset;
  Ihandle *main_vbox;
  Ihandle *main_tbox;
  Ihandle *type_lbl;
  Ihandle *type_list;
} MEGUI_VAL;

typedef struct _MEGUI_SEA
{
  Ihandle *main_fset;
  Ihandle *main_vbox;
  Ihandle *main_list;
} MEGUI_SEA;

typedef struct _MEGUI
{
  char font[20];
  char fontSize[5];
  MEGUI_MAIN main;
  MEGUI_SEA  sea;
  MEGUI_VAL  val;
  MEGUI_CMP  cmp;
} MEGUI;

MEGUI* meGetGui( void );

Ihandle* meMkList( Icallback func, ... );

int meSea_OnInit( Ihandle *ih );
int meCmp_OnInit( Ihandle *ih );
int meVal_OnInit( Ihandle *ih );

int meMenu_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status );
int meListP_ButtonCB( Ihandle    *ih, int button, int pressed, int x, int y, char* status );
int meListP_MotionCB( Ihandle *sbox, int x, int y, char* status );
/* Unsafe - Assumes there will be enough char elements, will NULL terminate */
void ftoa( double value, char* dst, int sigDigits );
/* Safer version */
void ftoa_s( double value, char *dst, uzv length, int sigDigits );


#endif
