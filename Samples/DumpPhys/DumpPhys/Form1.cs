// ---------------------------------------------------- //
//                      WinIo v3.0                      //
//          Direct Hardware Access Under Windows        //
//           Copyright 1998-2010 Yariv Kaplan           //
//               http://www.internals.com               //
// ---------------------------------------------------- //

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace DumpPhys
{
    public unsafe partial class Form1 : Form
    {
        [DllImport("kernel32.dll")]
        private extern static IntPtr LoadLibrary(String DllName);

        [DllImport("kernel32.dll")]
        private extern static IntPtr GetProcAddress(IntPtr hModule, String ProcName);

        [DllImport("kernel32")]
        private extern static bool FreeLibrary(IntPtr hModule);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool InitializeWinIoType();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool GetPhysLongType(IntPtr PhysAddr, UInt32 *pPhysVal);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool SetPhysLongType(IntPtr PhysAddr, UInt32 PhysVal);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool ShutdownWinIoType();

        IntPtr hMod;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Check if this is a 32 bit or 64 bit system
            if (IntPtr.Size == 4)
            {
                hMod = LoadLibrary("WinIo32.dll");
                txtPhysAddr.MaxLength = 8;
                txtPhysAddr.Text = "00000000";
                
            }
            else if (IntPtr.Size == 8)
            {
                hMod = LoadLibrary("WinIo64.dll");
                txtPhysAddr.MaxLength = 16;
                txtPhysAddr.Text = "0000000000000000";
            }

            if (hMod == IntPtr.Zero)
            {
                MessageBox.Show("Can't find WinIo dll.\nMake sure the WinIo library files are located in the same directory as your executable file.", "DumpPhys", MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
            }

            IntPtr pFunc = GetProcAddress(hMod, "InitializeWinIo");

            if (pFunc != IntPtr.Zero)
            {
                InitializeWinIoType InitializeWinIo = (InitializeWinIoType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(InitializeWinIoType));
                bool Result = InitializeWinIo();

                if (!Result)
                {
                    MessageBox.Show("Error returned from InitializeWinIo.\nMake sure you are running with administrative privileges and that the WinIo library files are located in the same directory as your executable file.", "DumpPhys", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    FreeLibrary(hMod);
                    this.Close();
                }
            }

            btnGetValue_Click(this, null);
        }

        private void btnGetValue_Click(object sender, EventArgs e)
        {
            IntPtr pFunc = GetProcAddress(hMod, "GetPhysLong");

            if (pFunc != IntPtr.Zero)
            {
                UInt32 PhysVal;
                UInt64 PhysAddr64;

                PhysAddr64 = UInt64.Parse(txtPhysAddr.Text, System.Globalization.NumberStyles.HexNumber);

                GetPhysLongType GetPhysLong = (GetPhysLongType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(GetPhysLongType));

                // Call WinIo to get value
                bool Result = GetPhysLong((IntPtr)PhysAddr64, &PhysVal);

                if (Result)
                {
                    txtValue.Text = PhysVal.ToString("X");
                }
                else
                {
                    MessageBox.Show("Error returned from GetPhysLong", "DumpPhys", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnQuit_Click(object sender, EventArgs e)
        {
            IntPtr pFunc = GetProcAddress(hMod, "ShutdownWinIo");

            if (pFunc != IntPtr.Zero)
            {
                ShutdownWinIoType ShutdownWinIo = (ShutdownWinIoType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(ShutdownWinIoType));

                ShutdownWinIo();
                FreeLibrary(hMod);
            }
            
            this.Close();
        }

        private void btnSetValue_Click(object sender, EventArgs e)
        {
            IntPtr pFunc = GetProcAddress(hMod, "SetPhysLong");

            if (pFunc != IntPtr.Zero)
            {
                UInt32 PhysVal;
                UInt64 PhysAddr64;

                PhysAddr64 = UInt64.Parse(txtPhysAddr.Text, System.Globalization.NumberStyles.HexNumber);
                PhysVal = UInt32.Parse(txtValue.Text, System.Globalization.NumberStyles.HexNumber);

                SetPhysLongType SetPhysLong = (SetPhysLongType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(SetPhysLongType));

                // Call WinIo to set value
                bool Result = SetPhysLong((IntPtr)PhysAddr64, PhysVal);

                if (!Result)
                {
                    MessageBox.Show("Error returned from SetPhysLong", "DumpPhys", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}