VERSION 5.00
Begin VB.Form FrmVBDumpPort32 
   BackColor       =   &H00FF0000&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "VBDumpPort32"
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
   Begin VB.TextBox TxtPortByte 
      Alignment       =   2  'Center
      Height          =   288
      Left            =   1560
      MaxLength       =   2
      TabIndex        =   10
      Top             =   2652
      Width           =   372
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
   Begin VB.TextBox TxtPortAddr 
      Height          =   288
      Left            =   3300
      MaxLength       =   4
      TabIndex        =   0
      Text            =   "0"
      Top             =   2040
      Width           =   600
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "http://www.internals.com"
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   240
      Left            =   1357
      TabIndex        =   9
      Top             =   1335
      Width           =   2580
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "Copyright 1998-2002 by Yariv Kaplan"
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   240
      Left            =   795
      TabIndex        =   8
      Top             =   885
      Width           =   3690
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
      Caption         =   "VBDumpPort32"
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
      Left            =   945
      TabIndex        =   6
      Top             =   210
      Width           =   3360
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "VBDumpPort32"
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
      Left            =   915
      TabIndex        =   7
      Top             =   165
      Width           =   3360
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
      Caption         =   "Please enter port address (hex):"
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
      Width           =   2928
   End
End
Attribute VB_Name = "FrmVBDumpPort32"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Sub BtnGet_Click()
  Dim Result As Boolean
  Dim PortVal As Long
  
  Result = GetPortVal(Val("&H" + TxtPortAddr), PortVal, 1)
    
  If (Result = False) Then
    MsgBox "Whoops ! There is a problem with GetPortByte.", vbOKOnly + vbCritical, "VBDumpPort32"
    Unload FrmVBDumpPort32
  Else
    TxtPortByte = Hex$(PortVal And &HFF)
  End If
End Sub
Private Sub BtnQuit_Click()
  Unload FrmVBDumpPort32
End Sub
Private Sub BtnSet_Click()
  Dim Result As Boolean
  
  Result = SetPortVal(Val("&H" + TxtPortAddr), Val("&H" + TxtPortByte), 1)
  
  If (Result = False) Then
    MsgBox "Whoops ! There is a problem with SetPortByte.", vbOKOnly + vbCritical, "VBDumpPort32"
    Unload FrmVBDumpPort32
  End If
End Sub
Private Sub Form_Load()
  If InitializeWinIo = False Then
    MsgBox "Whoops ! There is a problem with InitializeWinIo.", vbOKOnly + vbCritical, "VBDumpPort32"
    Unload FrmVBDumpPort32
  Else
    Call BtnGet_Click
  End If
End Sub
Private Sub Form_Unload(Cancel As Integer)
  Call ShutdownWinIo
End Sub
Private Sub TxtPortAddr_KeyPress(KeyAscii As Integer)
  If KeyAscii = 13 Then
    Call BtnGet_Click
  End If
End Sub
