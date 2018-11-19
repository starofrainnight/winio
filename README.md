# WinIo

The WinIo library allows 32-bit and 64-bit Windows applications to directly access I/O ports and physical memory.

## Copyright

This library was written by Yariv Kaplan, which could be download from www.internals.com.

**WARNING!** This library is strictly copyrighted! All rights are claimed belongs to Yariv Kaplan (even the codes you modified). You must read the LICENESE of WinIo library carefully if you want to use this library (neither generated binaries or source code).

## Why have this repository?

Because www.internals.com is down latest, I couldn't download it, so fork this useful library for backup purpose.

The tag 2.0 is the version that supported win9X.

Lastest viewable content from www.internals.com could be access from [archive.org](http://web.archive.org/web/20160410042019/http://www.internals.com/) .

## Redistributable Files

The redistributable files are provided in the \binaries directory (WinIo32.dll, WinIo32.sys and WinIo64.dll) and should be included in the installation package for your application. The WinIo64.sys file must be replaced with a version that is signed with a public code signing certificate (see below).

The WinIo files must be placed in the same directory as your application's executable file.

## Privilege Requirements

WinIo requires administrative privileges to run properly. This can be achieved by:

- Using WinIo from a service running as LocalSystem (SE_LOAD_DRIVER_NAME privilege must be explicitly enabled).
- Embedding a manifest file in the application that requests privilege elevation.
- Requiring the user to choose the "Run as Administrator" option when launching the application.

The included C# samples demonstrate using an embedded manifest file to request privilege elevation.

## Driver Signing Requirements on 64-bit Systems

64-bit versions of Windows only load device drivers that are signed by a code signing certificate issued by a public CA such as Verisign, Thawte, etc. WinIo64.sys must not be deployed on production machines unless a code signing certificate is obtained and used to sign this file. The bundled copy of WinIo64.sys is signed with a self-signed certificate and can only be used on development/test machines with Windows running in a special "test" mode. In order to use the bundled version of WinIo64.sys, you need to take the following steps:

- Open an elevated command window by right-clicking the icon and clicking "Run as Administrator".
- Type the following command to enable test-signing:

  bcdedit.exe /set TESTSIGNING ON

- Reboot the machine

For more information on Windows driver signing requirements, please refer to http://www.microsoft.com/whdc/winlogo/drvsign/kmcs_walkthrough.mspx.

## More Help

If you want any further advance help information, just read the WinIo.chm which
bundled with released WinIo.zip .
