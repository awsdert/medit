#pragma once
#ifndef _MESEARCH_H
#define _MESEARCH_H

#include "_main.h"
#include <Ipipe.h>

#define CAST_DMP( T ) \
  tNum = *((T*)&tar.buff[i]); \
  dNum = *((T*)&dmp.buff[i])
#define CAST_CMP( T ) \
  mNum = *((T*)&mecM->buff[c]); \
  nNum = *((T*)&mecN->buff[c])
#define COMP( CMP_T ) \
  rBl = (tNum CMP_T dNum)
#define COMPARE( CMP_T ) \
  if ( mecM->used[c] ) rBl = ( rBl &&  (tNum CMP_T mNum)); \
  if ( mecN->used[c] ) rBl = ( rBl && !(tNum CMP_T nNum))



typedef struct _MEPIPE
{
  Ipipe pipe;
  ucv   buff[BUFSIZ];
} MEPIPE;

typedef union _FPV
{
  fpv val;
  ucv buf[ sizeof( fpv ) ];
} FPV;
typedef union _DPV
{
  dpv val;
  ucv buf[ sizeof( dpv ) ];
} DPV;
typedef union _LPV
{
  lpv val;
  ucv buf[ sizeof( lpv ) ];
} LPV;

typedef struct _ME_LPV
{
  ucv  used[CMP_COUNT];
  LPV  buff[CMP_COUNT];
} ME_LPV;

typedef struct _ME_UHV
{
  ucv  used[CMP_COUNT];
  uhv  buff[CMP_COUNT];
} ME_UHV;

typedef struct _ME_SHV
{
  ucv  used[CMP_COUNT];
  shv  buff[CMP_COUNT];
} ME_SHV;

int meSeaMenu_OnClick( Ihandle *ih, int button, int pressed, int x, int y, char* status );

void fpQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_LPV *mecM, ME_LPV *mecN );
void siQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_SHV *mecM, ME_SHV *mecN );
void uiQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_UHV *mecM, ME_UHV *mecN );

#endif // _MESEARCH_H
