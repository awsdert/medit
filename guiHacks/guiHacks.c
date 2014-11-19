#include "../guiMain.h"
void execHacksId( pid_t pid )
{
  pid_t buf[3] = {0, 0, pid};
  STARTUPINFO si = { sizeof(si), 0 };
  PROCESS_INFORMATION pi = {0};
  si.lpReserved2 = (BYTE*)buf;
#ifdef _DEBUG
#if _WIN == 32
  CreateProcess( "hackProc-msw-x86-d-vc.exe",
#else
  CreateProcess( "hackProc-msw-x64-d-vc.exe",
#endif
#else
#if _WIN == 32
  CreateProcess( "hackProc-msw-x86-vc.exe",
#else
  CreateProcess( "hackProc-msw-x64-vc.exe",
#endif
#endif
    NULL, NULL, NULL, 0, 0, NULL, NULL, &si, &pi );
}
void guiHacks_OnLang( void )
{
  LANG const
  *lang = appGetLang();
  GUI
   *gui = appGetGui();
}
void guiHacks_OnInit( void )
{
  GUI
   *gui = appGetGui();
}
