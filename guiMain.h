#pragma once
#ifndef _GUIMAIN_H
#define _GUIMAIN_H
#include "search.h"

void
RdTxtHacks( HACK_FUNC *hfunc, HACKS *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file );
void
WrTxtHacks( HACK_FUNC *hfunc, HACKS *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file );

#endif // _GUIMAIN_H
