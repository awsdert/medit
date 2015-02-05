#include "ArmaxRawMain.h"

#if 0
EXT_C LIB_EXP BOOL APIENTRY
DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
  switch ( fdwReason )
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
#else
EXT_C LIB_EXP int main ( void )
{
  return 0;
}
#endif
