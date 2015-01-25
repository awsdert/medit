#include "ArmaxRawMain.h"

EXT_C ARMAX_RAW_EXP BOOL APIENTRY
  DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
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
