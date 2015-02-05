#include "hackProc.h"
/**
 * Thread which will be called in remote process after injection
 */
//void mainCRTStartup();
DWORD WINAPI entryThread( LPVOID param )
{
  DWORD newModuleD = ( DWORD )param;
  MessageBox( NULL, "Injection success. Now initializing main().", NULL, 0 );
  main();
  MessageBox( NULL, "This should never be reached.", NULL, 0 );
  return 0;
}
