#pragma once
#ifndef _GUIMAIN_H
#define _GUIMAIN_H
#include "guiQry.h"

OPEN_C

void RdTxtHacks(
  HACK_FUNC *hfunc, HACKL *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file );
void WrTxtHacks(
  HACK_FUNC *hfunc, HACKL *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file );

SHUT_C

#endif // _GUIMAIN_H
