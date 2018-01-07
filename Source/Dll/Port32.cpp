// ---------------------------------------------------- //
//                      WinIo v3.0                      //
//				 Direct Hardware Access Under Windows	//
//           Copyright 1998-2010 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#include <windows.h>
#include <winioctl.h>
#include <conio.h>
#include "port32.h"
#include "..\drv\winio_nt.h"
#include "winio.h"


bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize)
{
	if (!IsWinIoInitialized)
	{
		return false;
	}

#ifdef _WIN64
	tagPortStruct PortStruct;
	DWORD dwBytesReturned;

	PortStruct.bSize = bSize;
	PortStruct.wPortAddr = wPortAddr;

	return DeviceIoControl(hDriver, IOCTL_WINIO_READPORT, &PortStruct, sizeof(PortStruct),
		pdwPortVal, sizeof(DWORD), &dwBytesReturned, NULL);


#elif _WIN32
	// If this is a 64 bit OS, we must use the driver to access I/O ports even if the application is 32 bit
	if (g_Is64BitOS)
	{
		tagPortStruct PortStruct;
		DWORD dwBytesReturned;

		PortStruct.bSize = bSize;
		PortStruct.wPortAddr = wPortAddr;

		return DeviceIoControl(hDriver, IOCTL_WINIO_READPORT, &PortStruct, sizeof(PortStruct),
			pdwPortVal, sizeof(DWORD), &dwBytesReturned, NULL);
	}
	else
	{
		switch (bSize)
		{
		case 1:

			*pdwPortVal = _inp(wPortAddr);

			break;

		case 2:

			*pdwPortVal = _inpw(wPortAddr);

			break;

		case 4:

			*pdwPortVal = _inpd(wPortAddr);

			break;
		}
	}
#endif

	return true;
}


bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize)
{
	if (!IsWinIoInitialized)
	{
		return false;
	}

#ifdef _WIN64
	tagPortStruct PortStruct;
	DWORD dwBytesReturned;

	PortStruct.bSize = bSize;
	PortStruct.dwPortVal = dwPortVal;
	PortStruct.wPortAddr = wPortAddr;

	return DeviceIoControl(hDriver, IOCTL_WINIO_WRITEPORT, &PortStruct, sizeof(PortStruct),
		NULL, 0, &dwBytesReturned, NULL);
#elif _WIN32
	// If this is a 64 bit OS, we must use the driver to access I/O ports even if the application is 32 bit
	if (g_Is64BitOS)
	{
		tagPortStruct PortStruct;
		DWORD dwBytesReturned;

		PortStruct.bSize = bSize;
		PortStruct.dwPortVal = dwPortVal;
		PortStruct.wPortAddr = wPortAddr;

		return DeviceIoControl(hDriver, IOCTL_WINIO_WRITEPORT, &PortStruct, sizeof(PortStruct),
			NULL, 0, &dwBytesReturned, NULL);
	}
	else
	{
		switch (bSize)
		{
		case 1:

			_outp(wPortAddr, dwPortVal);

			break;

		case 2:

			_outpw(wPortAddr, (WORD)dwPortVal);

			break;

		case 4:

			_outpd(wPortAddr, dwPortVal);

			break;
		}
	}
#endif

	return true;
}
