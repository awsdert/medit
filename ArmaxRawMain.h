#pragma once
#include "_hack.h"

#ifdef BUILD_ARMAX_RAW_DLL
    #define ARMAX_RAW_EXP LIB_EXP
#else
    #define ARMAX_RAW_EXP LIB_IMP
#endif

OPEN_C

ARMAX_RAW_EXP HACK_COM* GetHackCOM( void );

void  ARMAX_RAW_EXP armaxRawHack_OnLoad(  FILE *file, char const *dataDir );
void  ARMAX_RAW_EXP armaxRawHack_OnSave(  FILE *file, char const *dataDir );
uchar ARMAX_RAW_EXP armaxRawHack_RdLine(  char *line, void *_source );
uchar ARMAX_RAW_EXP armaxRawHack_WrLine(  char *line, void *_source );
uchar ARMAX_RAW_EXP armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source );
uchar ARMAX_RAW_EXP armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source );
uchar ARMAX_RAW_EXP armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source );
uchar ARMAX_RAW_EXP armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source );

SHUT_C
