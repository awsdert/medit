#include "ArmaxRawMain.h"

EXT_C ARMAX_RAW_EXP BOOL APIENTRY
  DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  HACK_LIB_COM* hfunc = GetHackFuncs();
  CODE_LIB_COM* cfunc = GetCodeFuncs();
  hfunc->OnLoad  = armaxRawHack_OnLoad;
  hfunc->RdLine  = armaxRawHack_RdLine;
  hfunc->OnSave  = armaxRawHack_OnSave;
  hfunc->WrLine  = armaxRawHack_WrLine;
  hfunc->raw2txt = armaxRawHack_Raw2Txt;
  hfunc->txt2raw = armaxRawHack_Txt2Raw;
  cfunc->raw2txt = armaxRawCode_Raw2Txt;
  cfunc->txt2raw = armaxRawCode_Txt2Raw;
  switch (fdwReason)
  {
  case DLL_PROCESS_ATTACH:
      /* attach to process, return FALSE to fail DLL load */
      break;
  case DLL_PROCESS_DETACH:
      // detach from process
      break;

  case DLL_THREAD_ATTACH:
      // attach to thread
      break;

  case DLL_THREAD_DETACH:
      // detach from thread
      break;
  }
  return TRUE; // succesful
}
