// ---------------------------------------------------- //
//                      WinIo v2.0                      //
//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //
//           Copyright 1998-2002 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#include <windows.h>
#include <winioctl.h>
#include "k32call.h"
#include "phys32.h"
#include "..\drv\nt\winio_nt.h"
#include "winio.h"

HANDLE hDriver = INVALID_HANDLE_VALUE;
bool IsNT;
bool IsWinIoInitialized = false;
char szWinIoDriverPath[MAX_PATH];


bool IsWinNT()
{
  OSVERSIONINFO OSVersionInfo;

  OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  GetVersionEx(&OSVersionInfo);

  return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


bool GetDriverPath()
{
  PSTR pszSlash;

  if (!GetModuleFileName(GetModuleHandle(NULL), szWinIoDriverPath, sizeof(szWinIoDriverPath)))
    return false;

  pszSlash = strrchr(szWinIoDriverPath, '\\');

  if (pszSlash)
    pszSlash[1] = 0;
  else
    return false;

  strcat(szWinIoDriverPath, "winio.sys");

  return true;
}


bool _stdcall InitializeWinIo()
{
  bool bResult;
  DWORD dwBytesReturned;

  IsNT = IsWinNT();

  if (IsNT)
  {
    hDriver = CreateFile("\\\\.\\WINIO",
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    // If the driver is not running, install it

    if (hDriver == INVALID_HANDLE_VALUE)
    {
      GetDriverPath();

      bResult = InstallWinIoDriver(szWinIoDriverPath, true);

      if (!bResult)
        return false;

      bResult = StartWinIoDriver();

      if (!bResult)
        return false;

      hDriver = CreateFile("\\\\.\\WINIO",
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);

      if (hDriver == INVALID_HANDLE_VALUE)
        return false;
    }

    // Enable I/O port access for this process

    if (!DeviceIoControl(hDriver, IOCTL_WINIO_ENABLEDIRECTIO, NULL,
                         0, NULL, 0, &dwBytesReturned, NULL))
      return false;

  }
  else
  {
    VxDCall = (DWORD (WINAPI *)(DWORD,DWORD,DWORD))GetK32ProcAddress(1);

    hDriver = CreateFile("\\\\.\\WINIO.VXD", 0, 0, 0, CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, 0);

    if (hDriver == INVALID_HANDLE_VALUE)
      return false;
  }

  IsWinIoInitialized = true;

  return true;
}


void _stdcall ShutdownWinIo()
{
  DWORD dwBytesReturned;

  if (IsNT)
  {
    if (hDriver != INVALID_HANDLE_VALUE)
    {
      // Disable I/O port access

      DeviceIoControl(hDriver, IOCTL_WINIO_DISABLEDIRECTIO, NULL,
                      0, NULL, 0, &dwBytesReturned, NULL);

      CloseHandle(hDriver);

    }

    RemoveWinIoDriver();
  }
  else
    CloseHandle(hDriver);

  IsWinIoInitialized = false;
}
