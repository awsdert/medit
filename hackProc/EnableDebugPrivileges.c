#include "hackProc.h"
/**
 * Get debug privileges for current process token
 */
BOOL EnableDebugPrivileges( void )
{
  HANDLE token;
  TOKEN_PRIVILEGES priv;
  BOOL ret = FALSE;

  if ( OpenProcessToken( GetCurrentProcess(),
                         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token ) )
  {
    priv.PrivilegeCount = 1;
    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if ( LookupPrivilegeValue( NULL, SE_DEBUG_NAME,
                               &priv.Privileges[0].Luid ) != FALSE &&
         AdjustTokenPrivileges( token, FALSE, &priv, 0, NULL, NULL ) != FALSE )
    {
      ret = TRUE;
    }

    CloseHandle( token );
  }

  return ret;
}
