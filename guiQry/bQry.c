#include "qry.h"
void* bQry( pid_t pid, size_t size )
{
  HANDLE ph = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pid );
  void* addr = NULL;
  MEMORY_BASIC_INFORMATION mbi = {0};
  size_t bytes = 0;
  while ( VirtualQueryEx( ph, addr, &mbi, bytes ) )
  {
    if ( (mbi.AllocationProtect & 0xF) == PAGE_READWRITE && mbi.RegionSize == size )
      return mbi.BaseAddress;
    addr += mbi.RegionSize;
  }
  CloseHandle( ph );
  return NULL;
}
