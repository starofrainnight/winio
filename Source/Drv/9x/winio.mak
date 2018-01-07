# winio.mak - makefile for WinIo VxD
# Copyright (c) 1998-2002 Yariv Kaplan

DEVICENAME = WINIO
DYNAMIC = 1
FRAMEWORK = C
DEBUG = 0
OBJECTS = WINIO.OBJ

!include $(VTOOLSD)\include\vtoolsd.mak
!include $(VTOOLSD)\include\vxdtarg.mak

winio.obj: winio.c winio.h
