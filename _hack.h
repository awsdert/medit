#pragma once
#ifndef __HACK_H
#define __HACK_H
#include "hack.h"
#include <Ipipe.h>
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

#define NAME_LEN  12
#define NAME_LAST 11

/** \brief Object through which DLL and EXE can communicate with each other
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/

typedef struct _HACK_FUNC
{
  void (*getHackName)( usv hi, char       *name, uzv len );
  void (*setHackName)( usv hi, char const *name, uzv len );
  long (*txt2raw)( HACK *raw, HACKS *hl, Ipipe *pipe );
  void (*raw2txt)( HACK *raw, HACKS *hl, Ipipe *pipe, long gid );
} HACK_FUNC;

typedef struct _CODELIST
{
  usv rows;
  usv cols;
  char x[30][50];
} CODELIST;

typedef struct _STR
{
  ucv   c;
  ulv   s;
  char *a;
}
typedef struct _STRA
{
  ucv   c;
  ulv   s;
  STR  *a;
} STRA;

typedef wchar_t wide;
typedef struct _WCS
{
  ucv   c;
  ulv   s;
  wide *a;
} WCS;
typedef struct _WCSA
{
  ulc   c;
  ulv   s;
  WCS  *a;
} WCSA;

#ifdef _WIDE
typedef WCS  TXT;
typedef WCSA TXTA;
typedef wide text;
#else
typedef STR  TXT;
typedef STRA TXTA;
typedef char text;
#endif

/** \brief Object through which DLL and EXE can communicate with each other
  \param getRamNo pointer EXE must fill after acquiring object pointer
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/
typedef struct _CODE_FUNC
{
  ucv  (*getBaseNo)( char* id );
  void (*setRegion)( REGION_T region );
  REGION_T (*getRegion)( void );
  usv  (*txt2raw)( CODE *raw, CODELIST *cl, usv line );
  ucv  (*raw2txt)( CODE *raw, Ipipe *pipe );
} CODE_FUNC;

typedef HACK_FUNC* (*GETHACKFUNCS)( void );
typedef CODE_FUNC* (*GETCODEFUNCS)( void );

C_SHUT

#endif
