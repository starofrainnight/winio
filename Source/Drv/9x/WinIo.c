// ---------------------------------------------------- //
//                      WinIo v2.0                      //
//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //
//           Copyright 1998-2002 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#define DEVICE_MAIN

#include "winio_9x.h"

#undef DEVICE_MAIN

#include "winio.h"

Declare_Virtual_Device(WINIO)

DefineControlHandler(SYS_DYNAMIC_DEVICE_INIT, OnSysDynamicDeviceInit);
DefineControlHandler(SYS_DYNAMIC_DEVICE_EXIT, OnSysDynamicDeviceExit);
DefineControlHandler(W32_DEVICEIOCONTROL, OnW32DeviceIoControl);

BOOL ControlDispatcher(DWORD dwControlMessage, DWORD EBX, DWORD EDX, DWORD ESI, DWORD EDI, DWORD ECX)
{
  START_CONTROL_DISPATCH

  ON_SYS_DYNAMIC_DEVICE_INIT(OnSysDynamicDeviceInit);
  ON_SYS_DYNAMIC_DEVICE_EXIT(OnSysDynamicDeviceExit);
  ON_W32_DEVICEIOCONTROL(OnW32DeviceIoControl);

  END_CONTROL_DISPATCH

  return TRUE;
}


BOOL OnSysDynamicDeviceInit()
{
  return TRUE;
}


BOOL OnSysDynamicDeviceExit()
{
  return TRUE;
}


DWORD OnW32DeviceIoControl(PIOCTLPARAMS p)
{
  struct tagPort32Struct *pPort32Struct;
  unsigned int *pdwPortVal;

  switch (p->dioc_IOCtlCode)
  {
    case DIOC_OPEN:
    case DIOC_CLOSEHANDLE:
    break;

    case WINIO_WRITEPORT:
      
      pPort32Struct = p->dioc_InBuf;

      switch (pPort32Struct->bSize)
      {
        case 1:
          _outp(pPort32Struct->wPortAddr, pPort32Struct->dwPortVal);
        break;

        case 2:
          _outpw(pPort32Struct->wPortAddr, pPort32Struct->dwPortVal);
        break;

        case 4:
          _outpd(pPort32Struct->wPortAddr, pPort32Struct->dwPortVal);
        break;
      }
      
    break;

    case WINIO_READPORT:

      pPort32Struct = p->dioc_InBuf;
      pdwPortVal = p->dioc_OutBuf;

      switch (pPort32Struct->bSize)
      {
        case 1:
          *pdwPortVal = _inp(pPort32Struct->wPortAddr);
        break;

        case 2:
          *pdwPortVal = _inpw(pPort32Struct->wPortAddr);
        break;

        case 4:
          *pdwPortVal = _inpd(pPort32Struct->wPortAddr);
        break;
      }

    break;
  }

  return 0;
}
