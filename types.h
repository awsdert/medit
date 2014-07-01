#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifndef HUGE
#ifdef _MSC_VER
#define HUGE __int64
#define H( NUM ) NUM##I64
#else
#define HUGE long long
#define H( NUM ) NUM##LL
#endif
typedef HUGE huge;
#endif

typedef unsigned  HUGE uhv;
typedef   signed  HUGE shv;
typedef unsigned  long ulv;
typedef   signed  long slv;
typedef unsigned   int uiv;
typedef   signed   int siv;
typedef unsigned short usv;
typedef   signed short ssv;
typedef unsigned  char ucv;
typedef   signed  char scv;

typedef        SSIZE_T szv;
typedef         SIZE_T uzv;
typedef        INT_PTR spv;
typedef       UINT_PTR upv;

typedef          float fpv;
typedef         double dpv;
typedef    long double lpv;

#endif // _ME_TYPES_H
