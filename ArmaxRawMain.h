#pragma once
#include "_hack.h"

OPEN_C

HACK_LIB_EXP HACK_COM* GetHackCOM( void );

void  HACK_LIB_EXP armaxRawHack_OnLoad(  FILE *file, char const *dataDir );
void  HACK_LIB_EXP armaxRawHack_OnSave(  FILE *file, char const *dataDir );
uchar HACK_LIB_EXP armaxRawHack_RdLine(  char *line, void *_source );
uchar HACK_LIB_EXP armaxRawHack_WrLine(  char *line, void *_source );
uchar HACK_LIB_EXP armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source );
uchar HACK_LIB_EXP armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source );
uchar HACK_LIB_EXP armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source );
uchar HACK_LIB_EXP armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source );

SHUT_C
