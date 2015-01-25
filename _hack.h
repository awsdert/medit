#pragma once
#include "hack.h"
#include <ipipe/main.h>
OPEN_C

typedef enum _REGION_T
{
  REGION_ANY = 0,
  REGION_US,
  REGION_EU,
  REGION_UK,
  REGION_AUS,
  REGION_DE,
  REGION_JP,
  /* REG_COUNT Limit is 63 */
  REGION_COUNT
} REGION_T;

typedef struct _HACKL
{
  NAME  *names;
  HACKS *hacks;
} HACKL;

typedef uchar (*_HACK_COM_HACK_FUNC)( HACK *hack, char* line, void *_source );
typedef uchar (*_HACK_COM_CODE_FUNC)( CODE *code, char* line, void *_source );
typedef uchar (*_HACK_COM_LINE_FUNC)( char *line, void *_source );
typedef void  (*_HACK_COM_FILE_FUNC)( FILE *file, char const *dataDir );
typedef uchar (*_HACK_COM_GETBASE)  ( char *name );
typedef void  (*_HACK_COM_RESIZEHL) ( HACKL  *hl, hack_t **indexList, hack_t count );
typedef void  (*_HACK_COM_RESIZECL) ( CODES **cl, hack_t **indexList, hack_t count );

typedef struct _HACK_COM
{
  _HACK_COM_GETBASE   GetBase;
  _HACK_COM_RESIZEHL  ResizeHacks;
  _HACK_COM_RESIZECL  ResizeCodes;
  // File Handling
  _HACK_COM_FILE_FUNC OnLoad;
  _HACK_COM_FILE_FUNC OnSave;
  // Replace with handler of _source when not loading/saving a file
  _HACK_COM_LINE_FUNC RdLine;
  _HACK_COM_LINE_FUNC WrLine;
  // 80 characters per line
  _HACK_COM_HACK_FUNC T2H;
  _HACK_COM_HACK_FUNC H2T;
  _HACK_COM_CODE_FUNC T2C;
  _HACK_COM_CODE_FUNC C2T;
  REGION_T reg;
  ushort   gid;
  HACKL    hl;
  CODES   *cl;
} HACK_COM;

typedef HACK_COM* (*_GETHACKCOM)( void );

SHUT_C
