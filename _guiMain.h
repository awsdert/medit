#pragma once
#ifndef __MAIN_H
#define __MAIN_H

#include <iup.h>
#include "lang.h"
#include "data.h"

typedef struct _MEGUI_MAIN
{
  Ihandle *main_dlg;
  Ihandle *main_vbox;
  Ihandle *main_dd;
} MEGUI_MAIN;

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

typedef struct _MEGUI_RES
{
  Ihandle *main_fset;
  Ihandle *main_vbox;
  /* Matrix Box */
  Ihandle *main_mbox;
} MEGUI_RES;

typedef struct _MEGUI
{
  char font[20];
  char fontSize[5];
  MEGUI_MAIN main;
  MEGUI_SEA  sea;
  MEGUI_RES  res;
  MEGUI_VAL  val;
  MEGUI_CMP  cmp;
} MEGUI;

MEGUI* meGetGui( void );

Ihandle* meMkList( Icallback func, ... );

int meSea_OnInit( Ihandle *ih );
int meCmp_OnInit( Ihandle *ih );
int meVal_OnInit( Ihandle *ih );
/**
  \brief Loads *.dll, *.so, etc
  \param name 1st part of library filename, version and extension is appended to this
  \param hfunc Pointer to pointer that will receive hack communication objects pointer
  \param cfunc Pointer to pointer that will receive code communication objects pointer
  \return Reference to library loaded
  Loads a library and acquires the objects needed for communicating with it,
  also sets the pointers on the object needed by library to query required info.
 **/
HMODULE meLoadLib( char* name, HACK_FUNC **hfunc, CODE_FUNC **cfunc );
/** \brief Frees *.dll, *.so, etc
  \param lib reference to library to free
  \return NULL or 0
  Frees Library and returns NULL for simple cleaning of variable holding reference
**/
HMODULE meFreeLib( HMODULE lib );

int meMenu_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status );
int meListP_ButtonCB( Ihandle    *ih, int button, int pressed, int x, int y, char* status );
int meListP_MotionCB( Ihandle *sbox, int x, int y, char* status );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text (assumed to be at least _CVTBUFSIZE length)
  \param sigDigits How many digits after decimal point to include
  Unsafe - Assumes there will be enough char elements, will NULL terminate dst
**/
void ftoa( double value, char* dst, int sigDigits );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text
  \param limit on number of characters in text
  \param sigDigits How many digits after decimal point to include
  Will NULL terminate dst
**/
void ftoa_s( double value, char *dst, uzv length, int sigDigits );


#endif
