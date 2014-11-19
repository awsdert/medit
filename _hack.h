#pragma once
#ifndef __HACK_H
#define __HACK_H
#include "hack.h"
#include <ipipe/main.h>
OPEN_C

typedef enum _REGION_T
{
  REGION_US = 0,
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
  HACKS hacks;
  char  names[HACKS_COUNT][ NAME_MAX ];
} HACKL;

/** \brief Object through which DLL and EXE can communicate with each other
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/

typedef struct _HACK_FUNC
{
  void (*getHackName)( ushort hi, char       *name, size_t len );
  void (*setHackName)( ushort hi, char const *name, size_t len );
  long (*txt2raw)( HACK *raw, HACKL *hl, Ipipe *pipe );
  void (*raw2txt)( HACK *raw, HACKL *hl, Ipipe *pipe, long gid );
} HACK_FUNC;

typedef struct _CODELIST
{
  ushort rows;
  ushort cols;
  char x[30][50];
} CODELIST;

/** \brief Object through which DLL and EXE can communicate with each other
  \param getRamNo pointer EXE must fill after acquiring object pointer
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/
typedef struct _CODE_FUNC
{
  uchar    (*getBaseNo)( char* id );
  void     (*setRegion)( REGION_T region );
  REGION_T (*getRegion)( void );
  ushort   (*txt2raw)( CODE *raw, CODELIST *cl, ushort line );
  uchar    (*raw2txt)( CODE *raw, Ipipe *pipe );
} CODE_FUNC;

typedef HACK_FUNC* (*GETHACKFUNCS)( void );
typedef CODE_FUNC* (*GETCODEFUNCS)( void );

SHUT_C

#endif
