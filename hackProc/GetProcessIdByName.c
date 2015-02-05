#include "hackProc.h"
/**
 * Return the ID of a process from its name
 * @param Name of target process
 * @return Process ID
 */
pid_t GetProcessIdByNameA( LPCSTR name )
{
  PROCESSENTRY32 pe32;
  HANDLE snapshot = NULL;
  pid_t pid = 0;

  snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

  if ( snapshot != INVALID_HANDLE_VALUE )
  {
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    if ( Process32First( snapshot, &pe32 ) )
    {
      do
      {
        if ( !lstrcmp( pe32.szExeFile, name ) )
        {
          pid = pe32.th32ProcessID;
          break;
        }
      }
      while ( Process32Next( snapshot, &pe32 ) );
    }

    CloseHandle( snapshot );
  }

  return pid;
}
