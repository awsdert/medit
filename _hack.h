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

typedef struct _HACK_LIB_COM
{
  void    (*ReSize)( HACKL *hl, hack_t **indexList, hack_t count );
  // File Handling
  void    (*OnLoad)(  FILE *file, char const *dataDir );
  void    (*OnSave)(  FILE *file, char const *dataDir );
  // Replace with handler of _source when not loading/saving a file
  uchar   (*RdLine)(  char *line, void *_source );
  uchar   (*WrLine)(  char *line, void *_source );
  // 80 characters per line
  uchar   (*txt2raw)( HACK *hack, char *line, void *_source );
  uchar   (*raw2txt)( HACK *hack, char *line, void *_source );
  HACKL    hl;
  REGION_T region;
  ulong gid;
} HACK_LIB_COM;

typedef struct _CODE_LIB_COM
{
  uchar  (*GetBase)( char *name );
  uchar* (*ReSize)( CODES **codes, uchar *indexList, uchar count );
  // 80 characters per line
  uchar  (*txt2raw)( CODE *code, char *line, void *_source );
  uchar  (*raw2txt)( CODE *code, char *line, void *_source );
  CODES  *codes;
} CODE_LIB_COM;

typedef HACK_LIB_COM* (*HACKLIBCOM)( void );
typedef CODE_LIB_COM* (*CODELIBCOM)( void );

SHUT_C
