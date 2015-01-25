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
char getEndian( void );
char changeEndian( void* source, size_t size, char curEndian, char nxtEndian );
void hacksReSize( HACKL  *hl, hack_t **indexList, hack_t count );
void codesReSize( CODES **cl, uchar  **indexList, uchar  count );

#ifdef HLIST
Ihandle* IupHList( const char *action );
#else
#define IupHList IupList
#endif

#ifdef _WIN
typedef HMODULE HLIB;
#define libOpen(   path )        LoadLibraryA(   path )
#define libOpenEx( path, flags ) LoadLibraryExA( path, NULL, flags )
#define libAddr GetProcAddress
#define libShut FreeLibrary
#define RTLD_LAZY 0
#define RTLD_NOW  0
#else
typedef void*   HLIB;
#define libOpen(   path )        dlopen( path, RTLD_NOW )
#define libOpenEx( path, flags ) dlopen( path, flags    )
#define libAddr dlsym
#define libShut dlcose
#endif
/**
  \brief Loads *.dll, *.so, etc
  \param name 1st part of library filename, version and extension is appended to this
  \param hfunc Pointer to pointer that will receive hack communication objects pointer
  \param cfunc Pointer to pointer that will receive code communication objects pointer
  Loads a library and acquires the objects needed for communicating with it,
  also sets the pointers on the object needed by library to query required info.
 **/
void appLoadLib( char* name );
/** \brief Frees *.dll, *.so, etc
  \param lib reference to library to free
  Frees Library and returns NULL for simple cleaning of variable holding reference
**/
void appFreeLib( void );
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
