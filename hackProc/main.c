#include "hackProc.h"
/**
 * Injected program entry point after Runtime library is initialized
 * Can call any runtime and system routines.
 */
STARTUPINFO si = { sizeof(si), 0 };
HACKDATA data = {0};
HACKINFO hackInfo = {0};
CODES codes = {0};
DWORD main()
{
  int   procId = s_getpid();
  char  procIdTxt[ 10 ] = {0};
  char  procTmp[ MAX_PATH ] = {0};
  char tmp[ MAX_PATH ] = {0};
  char *procTmp2 = procTmp, *tmp2 = tmp;
  UZ   envRetSize = 0;
  GetStartupInfo(&si);
  if ( si.lpReserved2 && ((DWORD*)si.lpReserved2)[0] == 0 && ((DWORD*)si.lpReserved2)[0] == 0 )
    entryPoint( ((DWORD*)si.lpReserved2)[2] );
  else
  {
    Itoa_s( procId, procIdTxt, 10, 16 );
#ifdef _WIN
    GetEnvironmentVariableA( "TMP", data.folder, MAX_PATH );
#else
    getenv_s( &envRetSize, data.folder, MAX_PATH, "TMP" );
#endif
    sstrcat_s( data.folder, MAX_PATH, "\\" );
    sstrcat_s( data.folder, MAX_PATH, procIdTxt );
    sstrcat_s( data.folder, MAX_PATH, "\\" );
    sstrcpy_s( data.file, MAX_PATH, data.folder );
    sstrcat_s( data.file, MAX_PATH, "hacks.bin" );
    MessageBoxA( NULL, procTmp, NULL, 0 );
    if ( PathFileExistsA( data.folder ) )
    {
      if ( sfopen_s( &data.hacks, data.file, "rb" ) == 0 )
      {
        sstrcpy_s( data.file, MAX_PATH, data.folder );
        sstrcat_s( data.file, MAX_PATH, "codes.bin" );
        if( sfopen_s( &data.codes, data.file, "rwb" ) == 0 &&
           sfread( &hackInfo, sizeof( HACKINFO ), 1, data.hacks ) != 0 )
        {
          // Start Loading codes
          hackProc( &data, &hackInfo, &codes, -1, 0 );
          sfclose( data.codes );
        }
        sfclose( data.hacks );
      }
    }
  }
  ExitThread(0);
  return 0;
}
