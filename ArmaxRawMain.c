#include "ArmaxRawMain.h"

C_EXT ARMAX_RAW_EXP BOOL APIENTRY
  DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  HACK_FUNC* hfunc = GetHackFuncs();
  CODE_FUNC* cfunc = GetCodeFuncs();
  hfunc->raw2txt = raw2txtHackArmaxRaw;
  hfunc->txt2raw = txt2rawHackArmaxRaw;
  cfunc->raw2txt = raw2txtCodeArmaxRaw;
  cfunc->txt2raw = txt2rawCodeArmaxRaw;
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
