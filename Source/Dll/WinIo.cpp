// ---------------------------------------------------- //
//                      WinIo v3.0                      //
//				 Direct Hardware Access Under Windows	//
//           Copyright 1998-2010 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winioctl.h>
#include "phys32.h"
#include "..\drv\winio_nt.h"
#include "winio.h"

HANDLE hDriver = INVALID_HANDLE_VALUE;
bool IsWinIoInitialized = false;
wchar_t szWinIoDriverPath[32768];
bool g_Is64BitOS;


typedef UINT (WINAPI* GETSYSTEMWOW64DIRECTORY)(LPTSTR, UINT);

BOOL Is64BitOS()
{
#ifdef _WIN64
	return TRUE;
#else
	GETSYSTEMWOW64DIRECTORY getSystemWow64Directory;
	HMODULE hKernel32;
	TCHAR Wow64Directory[32767];

	hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
	if (hKernel32 == NULL)
	{
		//
		// This shouldn't happen, but if we can't get 
		// kernel32's module handle then assume we are 
		// on x86. We won't ever install 32-bit drivers
		// on 64-bit machines, we just want to catch it 
		// up front to give users a better error message.
		//
		return FALSE;
	}

	getSystemWow64Directory = (GETSYSTEMWOW64DIRECTORY)GetProcAddress(hKernel32, "GetSystemWow64DirectoryW");

	if (getSystemWow64Directory == NULL)
	{
		//
		// This most likely means we are running 
		// on Windows 2000, which didn't have this API 
		// and didn't have a 64-bit counterpart.
		//
		return FALSE;
	}

	if ((getSystemWow64Directory(Wow64Directory, _countof(Wow64Directory)) == 0) &&
		(GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)) {
			return FALSE;
	}

	//
	// GetSystemWow64Directory succeeded 
	// so we are on a 64-bit OS.
	//
	return TRUE;
#endif
}


bool GetDriverPath()
{
	PWSTR pszSlash;

	if (!GetModuleFileName(GetModuleHandle(NULL), szWinIoDriverPath, sizeof(szWinIoDriverPath)))
		return false;

	pszSlash = wcsrchr(szWinIoDriverPath, '\\');

	if (pszSlash)
		pszSlash[1] = 0;
	else
		return false;

	if (g_Is64BitOS)
		wcscat(szWinIoDriverPath, L"winio64.sys");
	else
		wcscat(szWinIoDriverPath, L"winio32.sys");

	return true;
}


bool __stdcall InitializeWinIo()
{
	bool bResult;
	DWORD dwBytesReturned;

	g_Is64BitOS = Is64BitOS();

	hDriver = CreateFile(L"\\\\.\\WINIO",
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

		hDriver = CreateFile(L"\\\\.\\WINIO",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
	}

	// Enable I/O port access for this process if running on a 32 bit OS

	if (!g_Is64BitOS)
	{
		if (!DeviceIoControl(hDriver, IOCTL_WINIO_ENABLEDIRECTIO, NULL,
			0, NULL, 0, &dwBytesReturned, NULL))
		{
			return false;
		}
	}

	IsWinIoInitialized = true;

	return true;
}


void _stdcall ShutdownWinIo()
{
	DWORD dwBytesReturned;

	if (hDriver != INVALID_HANDLE_VALUE)
	{
		// Disable I/O port access if running on a 32 bit OS

		if (!g_Is64BitOS)
		{
			DeviceIoControl(hDriver, IOCTL_WINIO_DISABLEDIRECTIO, NULL,
				0, NULL, 0, &dwBytesReturned, NULL);
		}

		CloseHandle(hDriver);

	}

	RemoveWinIoDriver();

	IsWinIoInitialized = false;
}
