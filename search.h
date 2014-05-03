#pragma once
#ifndef _MESEARCH_H
#define _MESEARCH_H

#include "_main.h"
#include "pipe.h"

#define CAST_DMP( T ) \
  tNum = *((T*)&tar.buff[i]); \
  dNum = *((T*)&dmp.buff[i])
#define CAST_CMP( T ) \
  mNum = *((T*)&mComp.buff[c]); \
  nNum = *((T*)&nComp.buff[c])
#define COMP( CMP_T ) \
  rBl = (tNum CMP_T dNum)
#define COMPARE( CMP_T ) \
  if ( mComp.used[c] ) rBl = ( rBl &&  (tNum CMP_T mNum)); \
  if ( nComp.used[c] ) rBl = ( rBl && !(tNum CMP_T nNum))


typedef struct _mePIPE
{
  ucv  *buff;
  Ipipe pipe;
} mePIPE;

typedef struct _meCOMP
{
  ucv *buff;
  ucv *used;
} meCOMP;

int meSea_OnShow( Ihandle *ih );
int meSeaMenu_OnClick( Ihandle *ih );

#endif // _MESEARCH_H
