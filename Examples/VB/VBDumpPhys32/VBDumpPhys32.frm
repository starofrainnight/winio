VERSION 5.00
Begin VB.Form FrmVBDumpPhys32 
   BackColor       =   &H00FF0000&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "VBDumpPhys32"
   ClientHeight    =   4215
   ClientLeft      =   2295
   ClientTop       =   2385
   ClientWidth     =   5295
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   4215
   ScaleWidth      =   5295
   Begin VB.TextBox TxtPhysVal 
      Height          =   288
      Left            =   1560
      MaxLength       =   8
      TabIndex        =   10
      Text            =   "0"
      Top             =   2640
      Width           =   975
   End
   Begin VB.CommandButton BtnQuit 
      Caption         =   "&Quit"
      Height          =   372
      Left            =   4020
      TabIndex        =   5
      Top             =   3540
      Width           =   972
   End
   Begin VB.CommandButton BtnSet 
      Caption         =   "&Set Value"
      Height          =   372
      Left            =   2124
      TabIndex        =   4
      Top             =   3528
      Width           =   972
   End
   Begin VB.CommandButton BtnGet 
      Caption         =   "&Get Value"
      Height          =   372
      Left            =   348
      TabIndex        =   1
      Top             =   3504
      Width           =   852
   End
   Begin VB.TextBox TxtPhysAddr 
      Height          =   288
      Left            =   3720
      MaxLength       =   8
      TabIndex        =   0
      Text            =   "0"
      Top             =   2052
      Width           =   975
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "http://www.internals.com"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   240
      Left            =   1582
      TabIndex        =   9
      Top             =   1305
      Width           =   2130
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "Copyright 1998-2002 by Yariv Kaplan"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   240
      Left            =   1005
      TabIndex        =   8
      Top             =   885
      Width           =   3270
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00FFFF80&
      BorderWidth     =   2
      X1              =   12
      X2              =   5292
      Y1              =   1788
      Y2              =   1788
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "VBDumpPhys32"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   24
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   555
      Left            =   885
      TabIndex        =   6
      Top             =   210
      Width           =   3540
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "VBDumpPhys32"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   24
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00800000&
      Height          =   555
      Left            =   885
      TabIndex        =   7
      Top             =   165
      Width           =   3540
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FF0000&
      Caption         =   "Value (hex):"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   252
      Left            =   360
      TabIndex        =   3
      Top             =   2652
      Width           =   1212
   End
   Begin VB.Label Label1 
      BackColor       =   &H00FF0000&
      Caption         =   "Please enter physical address (hex):"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   177
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   252
      Left            =   360
      TabIndex        =   2
      Top             =   2052
      Width           =   3252
   End
End
Attribute VB_Name = "FrmVBDumpPhys32"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Sub BtnGet_Click()
  Dim Result As Boolean
  Dim PhysVal As Long
 
  Result = GetPhysLong(Val("&H" + TxtPhysAddr), PhysVal)
  
  If (Result = False) Then
    MsgBox "Whoops ! There is a problem with GetPhysLong.", vbOKOnly + vbCritical, "VBDumpPhys32"
    Unload FrmVBDumpPhys32
  Else
    TxtPhysVal = Hex$(PhysVal)
  End If
End Sub
Private Sub BtnQuit_Click()
  Unload FrmVBDumpPhys32
End Sub
Private Sub BtnSet_Click()
  Dim Result As Boolean
  
  Result = SetPhysLong(Val("&H" + TxtPhysAddr), Val("&H" + TxtPhysVal))
  
  If Result = False Then
    MsgBox "Whoops ! There is a problem with SetPhysLong.", vbOKOnly + vbCritical, "VBDumpPhys32"
    Unload FrmVBDumpPhys32
  End If
End Sub
Private Sub Form_Load()
  If InitializeWinIo = False Then
    MsgBox "Whoops ! There is a problem with InitializeWinIo.", vbOKOnly + vbCritical, "VBDumpPhys32"
    Unload FrmVBDumpPhys32
  Else
    Call BtnGet_Click
  End If
End Sub
Private Sub Form_Unload(Cancel As Integer)
  Call ShutdownWinIo
End Sub
Private Sub TxtPhysAddr_KeyPress(KeyAscii As Integer)
  If KeyAscii = 13 Then
    Call BtnGet_Click
  End If
End Sub
