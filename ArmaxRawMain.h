#pragma once
#include "_hack.h"

#ifdef _WIN

#ifdef BUILD_ARMAX_RAW_DLL
    #define ARMAX_RAW_EXP __declspec(dllexport)
#else
    #define ARMAX_RAW_EXP __declspec(dllimport)
#endif

#else

#ifdef BUILD_ARMAX_RAW_DLL
#define ARMAX_RAW_EXP static
#else
#define ARMAX_RAW_EXP extern
#endif

#endif

OPEN_C

ARMAX_RAW_EXP HACK_LIB_COM* GetHackFuncs( void );
ARMAX_RAW_EXP CODE_LIB_COM* GetCodeFuncs( void );

void  armaxRawHack_OnLoad( Ipipe *file, char const *dataDir );
void  armaxRawHack_OnSave( Ipipe *file, char const *dataDir );
uchar armaxRawHack_RdLine( char  *line, void *_source );
uchar armaxRawHack_WrLine( char  *line, void *_source );
uchar armaxRawHack_Txt2Raw( HACK *hack, char *line, void *_source );
uchar armaxRawHack_Raw2Txt( HACK *hack, char *line, void *_source );
uchar armaxRawCode_Txt2Raw( CODE *code, char *line, void *_source );
uchar armaxRawCode_Raw2Txt( CODE *code, char *line, void *_source );

SHUT_C
