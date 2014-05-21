#pragma once
#ifndef _MESEARCH_H
#define _MESEARCH_H

#include "_main.h"
#include "pipe.h"

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

typedef enum   _METEST
{
  /* ==, != */
  T_EQ =
  '\0',  T_NE,
  /* >, >= */
  T_MT, T_ME,
  /* <, <= */
  T_LT, T_LE,
  /* &, !& */
  T_IA, T_NA,
  /* |, !| */
  T_IO, T_NO,
  /* ^, !^ */
  T_IX, T_NX,
  /* ~, !~ */
  T_IR, T_NR,
  T_COUNT,
  T_DUMP,
  T_UNDO,
  T_REDO
} METEST;

typedef struct _MEPIPE
{
  ucv   buff[BUFSIZ];
  Ipipe pipe;
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
  LPV  buff[T_COUNT];
  ucv  used[T_COUNT];
} ME_LPV;

typedef struct _ME_UHV
{
  uhv  buff[T_COUNT];
  ucv  used[T_COUNT];
} ME_UHV;

typedef struct _ME_SHV
{
  shv  buff[T_COUNT];
  ucv  used[T_COUNT];
} ME_SHV;



int meSea_OnShow( Ihandle *ih );
int meSeaMenu_OnClick( Ihandle *ih, int button, int pressed, int x, int y, char* status );

void fpQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_LPV *mecM, ME_LPV *mecN );
void siQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_SHV *mecM, ME_SHV *mecN );
void uiQry( ucv   used, MEPIPE *mepI,  MEPIPE  *mepO, MEPIPE  *mepP,
            Ipipe prev, scv    bytes, ME_UHV *mecM, ME_UHV *mecN );

#endif // _MESEARCH_H
