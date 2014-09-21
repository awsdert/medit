#include "hackProc.h"
pid_t GetProcessParentId( pid_t pid )
{
  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pid_t res = PID_MIN;
  pe.dwSize = sizeof(PROCESSENTRY32);
  if( Process32First( h, &pe ) )
  {
    do
    {
      if ( pe.th32ProcessID == pid )
      {
        res = pe.th32ParentProcessID;
        break;
      }
    } while( Process32Next( h, &pe ) );
  }
  CloseHandle(h);
  return res;
}
