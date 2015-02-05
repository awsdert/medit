#include "hackProc.h"
size_t  sfwrite(  void* buffer, size_t elSize, int elCount, SYSF* file )
{
  DWORD writ = 0;
  WriteFile( file->hFile, buffer, elSize * elCount, &writ, NULL );
  return writ;
}
