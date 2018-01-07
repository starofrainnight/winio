// ---------------------------------------------------- //
//                      WinIo v2.0                      //
//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //
//           Copyright 1998-2002 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#include <windows.h>
#include <winioctl.h>
#include <conio.h>
#include "port32.h"
#include "..\drv\nt\winio_nt.h"
#include "..\drv\9x\winio_9x.h"
#include "winio.h"


bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize)
{
  tagPort32Struct Port32Struct;
  DWORD dwBytesReturned;
  
  if (!IsWinIoInitialized)
    return false;

  if (IsNT)
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
  else
  {
   Port32Struct.bSize = bSize;
   Port32Struct.wPortAddr = wPortAddr;
   
   return DeviceIoControl(hDriver, WINIO_READPORT, &Port32Struct, sizeof(Port32Struct),
                          pdwPortVal, sizeof(DWORD), &dwBytesReturned, NULL);
  }

  return true;
}


bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize)
{
  tagPort32Struct Port32Struct;
  DWORD dwBytesReturned;

  if (!IsWinIoInitialized)
    return false;

  if (IsNT)
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
  else
  {
   Port32Struct.bSize = bSize;
   Port32Struct.dwPortVal = dwPortVal;
   Port32Struct.wPortAddr = wPortAddr;
   
   return DeviceIoControl(hDriver, WINIO_WRITEPORT, &Port32Struct, sizeof(Port32Struct),
                          NULL, 0, &dwBytesReturned, NULL);
  }
  
  return true;
}
