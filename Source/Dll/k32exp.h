/* 
K32EXP.H -- Get32ProcAddress
Win32 code to import by ordinal from KERNEL32.DLL in Windows 95

Andrew Schulman
Senior editor, O'Reilly & Associates (Sebastopol CA)
andrew@ora.com

August 1995

See K32EXP.C for more information
*/

// KERNEL32.1 through KERNEL32.9 are all VxDCall
#define VXDCALL_ORD                 1

// found these by examining RUNDLL32.EXE
#define LOADLIBRARY16_ORD           35
#define FREELIBRARY16_ORD           36
#define GETPROCADDRESS16_ORD        37

// found these by examining WOW32.DLL
// actually, these are documented: see GENTHUNK.TXT ("Windows NT
// Generic Thunk Overview") and WOWNT32.H in Microsoft's Win32 SDK
#define WOWCallback16_ORD           54                      
#define WOWCallback16Ex_ORD         55                      
#define WOWGetVDMPointer_ORD        56                      
#define WOWHandle32_ORD             57                      
#define WOWHandle16_ORD             58                      
#define WOWGlobalAlloc16_ORD        59                      
#define WOWGlobalLock16_ORD         60                      
#define WOWGlobalUnlock16_ORD       61                      
#define WOWGlobalFree16_ORD         62                      
#define WOWGlobalAllocLock16_ORD    63                      
#define WOWGlobalUnlockFree16_ORD   64                      
#define WOWGlobalLockSize16_ORD     65                      
#define WOWYield16_ORD              66                      
#define WOWDirectedYield16_ORD      67                      
#define WOWGetVDMPointerFix_ORD     68                      
#define WOWGetVDMPointerUnfix_ORD   69                      
#define WOW32_1_ORD                 70

// found these by examining NTDLL.DLL (Win95)
#define RtlLargeIntegerAdd_ORD              72 
#define RtlEnlargedIntegerMultiply_ORD      73 
#define RtlEnlargedUnsignedMultiply_ORD     74 
#define RtlEnlargedUnsignedDivide_ORD       75 
#define RtlExtendedLargeIntegerDivide_ORD   76 
#define RtlExtendedMagicDivide_ORD          77 
#define RtlExtendedIntegerMultiply_ORD      78 
#define RtlLargeIntegerShiftLeft_ORD        79 
#define RtlLargeIntegerShiftRight_ORD       80 
#define RtlLargeIntegerArithmeticShift_ORD  81 
#define RtlLargeIntegerNegate_ORD           82 
#define RtlLargeIntegerSubtract_ORD         83 
#define RtlConvertLongToLargeInteger_ORD    84 
#define RtlConvertUlongToLargeInteger_ORD   85 

#define GETPWIN16LOCK_ORD           93
#define ENTERSYSLEVEL_ORD           97
#define LEAVESYSLEVEL_ORD           98

DWORD WINAPI GetK32ProcAddress(int ord);

// VxDCall is probably the most important undocumented Win32 API
DWORD (WINAPI *VxDCall)(DWORD srvc, DWORD eax, DWORD ecx);

// Walking VxD chain shows that these are the VxDs that provide
// Win32 services
#define VMM_ID          0x0001             // 41 services
#define REBOOT_ID       0x0009             // 2
#define VNETBIOS_ID     0x0014             // 2
#define VWIN32_ID       0x002A             // 79
#define VCOMM_ID        0x002B             // 27
#define VCOND_ID        0x0038             // 53 -- Virtual CON (Console)

// found these by examining ADVAPI32.DLL and REGSERV.EXE
// REGSERV.EXE calls these directly for remote registry services
#define RegOpenKeyEx_CALL                   0x10011
#define RegCreatKeyA_CALL                   0x10012
#define RegCloseKey_CALL                    0x10013
#define RegDeleteKey_CALL                   0x10014
#define RegSetValueA_CALL                   0x10015
#define RegDeleteValueA_CALL                0x10016
#define RegQueryValueA_CALL                 0x10017
#define RegEnumKeyA_CALL                    0x10018
#define RegQueryValueEx_CALL                0x1001A
#define RegSetValueExA_CALL                 0x1001B
#define RegFlushKey_CALL                    0x1001C
#define RegLoadKeyA_CALL                    0x10021
#define RegUnloadKeyA_CALL                  0x10022
#define RegSaveKeyA_CALL                    0x10023
#define RegRemapPreDefKey_CALL              0x10024
#define RegQueryMultipleValuesA_CALL        0x10026
#define RegReplaceKeyA_CALL                 0x10027

// two important VWIN32 services
#define VWIN32_INT21_CALL       0x2A0010
#define VWIN32_INT31_CALL       0x2A0029
#define DosCall(eax, ecx)       VxDCall(VWIN32_INT21_CALL, (eax), (ecx))
#define DPMICall(eax, ecx)      VxDCall(VWIN32_INT31_CALL, (eax), (ecx))

// other undocumented interfaces
HINSTANCE (WINAPI *LoadLibrary16)(LPCSTR lpLibFileName);
int (WINAPI *FreeLibrary16)(HINSTANCE hModule);
FARPROC (WINAPI *GetProcAddress16)(HINSTANCE hModule, LPCSTR lpProcName);
void (WINAPI *GetpWin16Lock)(DWORD *pWin16Lock);
// int (WINAPI *_ConfirmWin16Lock)(void);  // returns current lock count
void (WINAPI *_EnterSysLevel)(DWORD lock);
void (WINAPI *_LeaveSysLevel)(DWORD lock);



