// Safeguard against forced c++ compile
#include <ipipe/c.h>
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif
USING( std::memset )
USING( std::fopen_s )
USING( std::getenv_s )
USING( std::strcat_s )
USING( std::strcpy_s )
USING( std::strtok_s )
USING( std::string )
// This will be used for short-cuts & starting x86/x64 versions
#define SEG_LENGTH 128
#define SEG_LEN    127
#define CMD_LENGTH 8192
#define CMD_LEN    8191
#define __STR( STRING ) #STRING
#define _STR( STRING ) __STR( STRING )
#define STR( STRING ) _STR( STRING )
char path[CMD_LENGTH] = {0};
char cmd[CMD_LENGTH] = {0};
int main( int argc, char const *argv[] )
{
  int segc = 0;
#ifdef _WIN32
  FILE *test = NULL;
#endif // _WIN32
  size_t length = 0;
  char *tokSeg = NULL, *tmp = cmd,
        *tmpSeg = NULL,
         *prvSeg = NULL,
          *curSeg = NULL,
           arcSeg[SEG_LENGTH] = {0};
  strcpy_s( path, CMD_LEN, argv[0] );
  curSeg = strtok_s( path, "\\/", &tokSeg );
  cmd[0] = '"';

  do
  {
    ++segc;
    tmpSeg = prvSeg;
    prvSeg = curSeg;
    curSeg = strtok_s( NULL, "\\/", &tokSeg );

    if ( tmpSeg )
    {
      strcat_s( cmd, CMD_LEN, tmpSeg );
      strcat_s( cmd, CMD_LEN, SEG_DELIM );
    }
  }
  while ( curSeg );

  memset( path, 0, CMD_LEN );
#ifdef _WIN32
  getenv_s( &length, path, CMD_LEN, "SYSTEM" );
  strcat_s( path, CMD_LEN, "wowsys64" );

  if ( fopen_s( &test, path, "r" ) == 0 )
  {
    strcat_s( arcSeg, SEG_LEN, "msw-x64" );
  }
  else
  {
    strcat_s( arcSeg, SEG_LEN, "msw-x86" );
  }

#else
#error Not supported
#endif
#ifdef _DEBUG
  strcat_s( arcSeg, SEG_LEN, "-d" );
#endif
  // Add compiler
  strcat_s( arcSeg, SEG_LEN,
#ifdef _MSC_VER
            "-vc" );
#else
            "-gcc" );
#endif
  strcat_s( cmd, CMD_LEN, "meditGui-" );
  strcat_s( cmd, CMD_LEN, arcSeg );
  strcat_s( cmd, CMD_LEN, "\"" );
  return 0;
}
