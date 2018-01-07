#include <windows.h>
#include <stdio.h>
#include "winio.h"

void main()
{
  DWORD dwPortVal;
  DWORD dwMemVal;
  bool bResult;
  HANDLE hPhysicalMemory;
  PBYTE pbLinAddr;

  // Call InitializeWinIo to initialize the WinIo library.

  bResult = InitializeWinIo();

  if (bResult)
  {
    // Under Windows NT/2000/XP, after calling InitializeWinIo,
    // you can call _inp/_outp instead of using GetPortVal/SetPortVal

    GetPortVal(0x378, &dwPortVal, 4);

    SetPortVal(0x378, 10, 4);

    // Map physical addresses 0xA0000 - 0xAFFFF into the linear address space
    // of the application. The value returned from the call to MapPhysToLin is
    // a linear address corresponding to physical address 0xA0000. In case of
    // an error, the return value is NULL.

    pbLinAddr = MapPhysToLin((PBYTE)0xA0000, 65536, &hPhysicalMemory);

    if (pbLinAddr)
    {
      // Now we can use pbLinAddr to access physical address 0xA0000

      *pbLinAddr = 10;

      // When you're done with pbLinAddr, call UnmapPhysicalMemory

      UnmapPhysicalMemory(hPhysicalMemory, pbLinAddr);
    }

    // Instead of using MapPhysToLin, we can use GetPhysLong/SetPhysLong

    GetPhysLong((PBYTE)0xA0000, &dwMemVal);

    SetPhysLong((PBYTE)0xA0000, 10);

    // When you're done using WinIo, call ShutdownWinIo

    ShutdownWinIo();
  }
  else
  {
    printf("Error during initialization of WinIo.\n");
    exit(1);
  }
}

