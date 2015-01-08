#pragma once
#include <iup.h>
#include "appLng.h"
#include "appData.h"
#include <ipipe/main.h>

OPEN_C
typedef struct _GUI_MAIN
{
  Ihandle *fset;
  Ihandle *vb;
} GUI_MAIN;

typedef enum _LIST_T
{
  LIST_FIRST = 0,
  LIST_PREV,
  LIST_NEXT,
  LIST_FINAL,
  LIST_NEW,
  LIST_DEL,
  LIST_MOV,
  LIST_COUNT
} LIST_T;

typedef struct _GUI
{
  char     font[20];
  char  fontSize[5];
} GUI;
extern GUI_MAIN guiDlg;
extern GUI appGui;
extern Ihandle *guiMenu;
extern Ihandle *guiData;
void hacksReSize( HACKL     *hl, void  **indexList, hack_t count );
void codesReSize( CODES **codes, uchar **indexList, uchar  count );

#ifdef HLIST
Ihandle* IupHList( const char *action );
#else
#define IupHList IupList
#endif

#ifdef _WIN
typedef HMODULE HLIB;
#else
typedef void*   HLIB;
#endif
extern HACK_LIB_COM *hCOM;
extern CODE_LIB_COM *cCOM;
/**
  \brief Loads *.dll, *.so, etc
  \param name 1st part of library filename, version and extension is appended to this
  \param hfunc Pointer to pointer that will receive hack communication objects pointer
  \param cfunc Pointer to pointer that will receive code communication objects pointer
  \return Reference to library loaded
  Loads a library and acquires the objects needed for communicating with it,
  also sets the pointers on the object needed by library to query required info.
 **/
HLIB appLoadLib( char* name, HACK_LIB_COM **hfunc, CODE_LIB_COM **cfunc );
/** \brief Frees *.dll, *.so, etc
  \param lib reference to library to free
  \return NULL or 0
  Frees Library and returns NULL for simple cleaning of variable holding reference
**/
HLIB appFreeLib( HLIB lib );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text (assumed to be at least _CVTBUFSIZE length)
  \param sigDigits How many digits after decimal point to include
  Unsafe - Assumes there will be enough char elements, will NULL terminate dst
**/
void ftoa(   double value, char* dst,                int sigDigits );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text
  \param limit on number of characters in text
  \param sigDigits How many digits after decimal point to include
  Will NULL terminate dst
**/
void ftoa_s( double value, char *dst, size_t length, int sigDigits );

SHUT_C
