// ---------------------------------------------------- //
//                      WinIo v2.0                      //
//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //
//           Copyright 1998-2002 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

#include <windows.h>
#include <winioctl.h>
#include "phys32.h"
#include "..\drv\nt\winio_nt.h"
#include "winio.h"

// VWIN32 DPMI services

#define VWIN32_DPMI_CALL 0x2A0029

// KERNEL32's undocumented VxDCall function is exported as ordinal #1

DWORD (WINAPI *VxDCall)(DWORD Service, DWORD EAX_Reg, DWORD ECX_Reg);

#define DPMICall(EAX_Reg, ECX_Reg) VxDCall(VWIN32_DPMI_CALL, EAX_Reg, ECX_Reg)


PBYTE _stdcall MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle)
{
  PBYTE pbLinAddr;
  tagPhys32Struct Phys32Struct;
  DWORD dwBytesReturned;

  if (!IsWinIoInitialized)
    return false;

  if (IsNT)
  {
    Phys32Struct.dwPhysMemSizeInBytes = dwPhysSize;
    Phys32Struct.pvPhysAddress = pbPhysAddr;

    if (!DeviceIoControl(hDriver, IOCTL_WINIO_MAPPHYSTOLIN, &Phys32Struct,
                         sizeof(tagPhys32Struct), &Phys32Struct, sizeof(tagPhys32Struct),
                         &dwBytesReturned, NULL))
      return NULL;
    else
    {
      pbLinAddr = (PBYTE)((DWORD)Phys32Struct.pvPhysMemLin + (DWORD)pbPhysAddr - (DWORD)Phys32Struct.pvPhysAddress);
      *pPhysicalMemoryHandle = Phys32Struct.PhysicalMemoryHandle;
    }
  }
  else
  {
    _asm
    {
      Mov BX, WORD PTR [pbPhysAddr + 2]
      Mov CX, WORD PTR [pbPhysAddr]
      Mov SI, WORD PTR [dwPhysSize + 2]
      Mov DI, WORD PTR [dwPhysSize]
    }

    // Call DPMI function MapPhysicalToLinear

    DPMICall(0x800, (DWORD)pbPhysAddr);    

    _asm
    {
      Jnc Success
      Xor BX, BX
      Xor CX, CX

    Success:

      Mov WORD PTR [pbLinAddr + 2], BX
      Mov WORD PTR [pbLinAddr], CX
    }
  }

  return pbLinAddr;
}


bool _stdcall UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr)
{
  tagPhys32Struct Phys32Struct;
  DWORD dwBytesReturned;

  if (IsNT)
  {
    if (!IsWinIoInitialized)
      return false;

    Phys32Struct.PhysicalMemoryHandle = PhysicalMemoryHandle;
    Phys32Struct.pvPhysMemLin = pbLinAddr;

    if (!DeviceIoControl(hDriver, IOCTL_WINIO_UNMAPPHYSADDR, &Phys32Struct,
                         sizeof(tagPhys32Struct), NULL, 0, &dwBytesReturned, NULL))
      return false;
  }

  return true;
}

// Support functions for Visual-Basic users

bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal)
{
  PDWORD pdwLinAddr;
  HANDLE PhysicalMemoryHandle;

  if (!IsWinIoInitialized)
    return false;

  pdwLinAddr = (PDWORD)MapPhysToLin(pbPhysAddr, 4, &PhysicalMemoryHandle);

  if (pdwLinAddr == NULL)
    return false;

  *pdwPhysVal = *pdwLinAddr;

  UnmapPhysicalMemory(PhysicalMemoryHandle, (PBYTE)pdwLinAddr);

  return true;
}


bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal)
{
  PDWORD pdwLinAddr;
  HANDLE PhysicalMemoryHandle;

  if (!IsWinIoInitialized)
    return false;

  pdwLinAddr = (PDWORD)MapPhysToLin(pbPhysAddr, 4, &PhysicalMemoryHandle);

  if (pdwLinAddr == NULL)
    return false;

  *pdwLinAddr = dwPhysVal;

  UnmapPhysicalMemory(PhysicalMemoryHandle, (PBYTE)pdwLinAddr);

  return true;
}
