#include "hackProc.h"
size_t  sfread(  void* buffer, size_t elSize, int elCount, SYSF* file )
{
  DWORD read = 0;
  ReadFile( file->hFile, buffer, elSize * elCount, &read, NULL );
  return read;
}
