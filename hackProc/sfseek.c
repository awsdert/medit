#include "hackProc.h"
int  sfseek(  SYSF *file, int distance, int from )
{
  return SetFilePointer( file->hFile, distance, NULL, from );
}
