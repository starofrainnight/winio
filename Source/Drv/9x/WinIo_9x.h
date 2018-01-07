#ifndef WINIO9X_H
#define WINIO9X_H

#define WINIO_WRITEPORT CTL_CODE(FILE_DEVICE_UNKNOWN, 1, METHOD_NEITHER, FILE_ANY_ACCESS)
#define WINIO_READPORT CTL_CODE(FILE_DEVICE_UNKNOWN, 2, METHOD_NEITHER, FILE_ANY_ACCESS)

#pragma pack(1)

struct tagPort32Struct
{
  unsigned short wPortAddr;
  unsigned int dwPortVal;
  unsigned char bSize;
};

#endif
