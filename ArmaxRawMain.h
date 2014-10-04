#ifndef __ARMAX_RAW_MAIN_H__
#define __ARMAX_RAW_MAIN_H__

#include "_hack.h"

/*  To use this exported function of dll, include this header
 *  in your project.
 */
#if defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )

#ifdef BUILD_ARMAX_RAW_DLL
    #define ARMAX_RAW_EXP __declspec(dllexport)
#else
    #define ARMAX_RAW_EXP __declspec(dllimport)
#endif

#else
#define ARMAX_RAW_EXP extern
#endif

C_OPEN

ARMAX_RAW_EXP HACK_FUNC* GetHackFuncs( void );
ARMAX_RAW_EXP CODE_FUNC* GetCodeFuncs( void );
long txt2rawHackArmaxRaw( HACK *raw, HACKS *hl, Ipipe *pipe );
void raw2txtHackArmaxRaw( HACK *raw, HACKS *hl, Ipipe *pipe, long gid );
usv  txt2rawCodeArmaxRaw( CODE *raw, CODELIST *cl, usv line );
ucv  raw2txtCodeArmaxRaw( CODE *raw, Ipipe *pipe );

C_SHUT

#endif // __MAIN_H__
