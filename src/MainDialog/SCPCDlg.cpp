//==============================================================================
//File name:    "SCPCDlg.cpp"
//Purpose:      Source implementation file
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "SCPC.h"
#include "SCPCDlg.h"
#include "afxdialogex.h"

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _WIN64
  #ifdef _DEBUG
  #pragma comment(lib,"./src/CAN/x64/candll64.lib")
  #else
  #pragma comment(lib,"./src/CAN/x64/candll64.lib") //For 64bit WinOS
  #endif
#else
  #ifdef _DEBUG
  #pragma comment(lib,"./src/CAN/x86/candll.lib")
  #else
  #pragma comment(lib,"./src/CAN/x86/candll.lib") //For 32bit WinOS
  #endif
#endif

//------------------------------------------------------------------------------
// CAboutDlg dialog used for App About
//------------------------------------------------------------------------------
class CAboutDlg : public CDialogEx
{
  public:
  CAboutDlg();

  //Dialog Data
  enum { IDD = IDD_ABOUTBOX };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
  protected:
  DECLARE_MESSAGE_MAP()
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
// CSCPCDlg dialog
CSCPCDlg::CSCPCDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CSCPCDlg::IDD, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  pThread = NULL;
  pCAN = NULL;
  m_first_launch = 0;
  CAN_device_ID = 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CSCPCDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON_ADAPTER, m_but_adapter);
  DDX_Control(pDX, IDC_BUTTON_SET_ANGLE, m_but_set_angle);
  DDX_Control(pDX, IDC_DEVICE_LIST, m_devices);
  DDX_Control(pDX, IDC_EDIT_ANGLE_CH1, m_edit_angle_ch1);
  DDX_Control(pDX, IDC_EDIT_ANGLE_CH2, m_edit_angle_ch2);
  DDX_Control(pDX, IDC_SLIDER_ANGLE_CH1, m_slider_angle_ch1);
  DDX_Control(pDX, IDC_SLIDER_ANGLE_CH2, m_slider_angle_ch2);
  DDX_Control(pDX, IDC_BUTTON_FW_UPDATE, m_but_fw_update);
  DDX_Control(pDX, IDC_BUTTON_SAVE, m_but_save);
  DDX_Control(pDX, IDC_CHECK_NPR, m_check_continuously);
  DDX_Control(pDX, IDC_CHECK_STARTUP_INI, m_check_startupini);
  DDX_Control(pDX, IDC_STATIC_DEVICE_POWER, m_txt_device_power);
  DDX_Control(pDX, IDC_STATIC_DEVICE_ID, m_txt_device_id);
}

//------------------------------------------------------------------------------
//  CSCPCDlg message handlers
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSCPCDlg, CDialogEx)

  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_WINDOWPOSCHANGED()
  ON_WM_DEVICECHANGE()

  ON_BN_CLICKED(IDC_BUTTON_ADAPTER, &CSCPCDlg::OnButtonAdapter)
  ON_BN_CLICKED(IDC_BUTTON_FW_UPDATE, &CSCPCDlg::OnButtonFwUpdate)
  ON_BN_CLICKED(IDC_BUTTON_SET_ANGLE, &CSCPCDlg::OnButtonSetAngle)
  ON_LBN_SELCHANGE(IDC_DEVICE_LIST, &CSCPCDlg::OnSelchangeDeviceList)
  ON_LBN_DBLCLK(IDC_DEVICE_LIST, &CSCPCDlg::OnDblclkDeviceList)
  ON_BN_CLICKED(IDC_CHECK_NPR, &CSCPCDlg::OnClickedCheckNpr)
  ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSCPCDlg::OnButtonSave)
  ON_WM_VSCROLL()

  ON_BN_CLICKED(IDC_CHECK_STARTUP_INI, &CSCPCDlg::OnCheckStartUpIni)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Function:	 
//------------------------------------------------------------------------------
BOOL CSCPCDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  POS.SetWindowPositon(this->m_hWnd);
  ControlsEnable(OFF);
  ControlsIni();
  
  if(REG.FirstLaunch()==1)
  {
    REG.SetIntVar("continuously", 0);
  }

  // Add "About..." menu item to system menu.
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if(!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

  // Set the icon for this dialog.  The framework does this automatically
  // when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);	// Set big icon
  SetIcon(m_hIcon, FALSE);	// Set small icon
  
  //Run RX Thread open
  ThreadLaunch();

  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  struct prj_tooltip tooltips[] = 
  { 
    {&m_slider_angle_ch1, "Ch1, continuously"},
    {&m_slider_angle_ch2, "Ch2, continuously"},
    {&m_edit_angle_ch1, "Ch1 value: +/-"},
    {&m_edit_angle_ch2, "Ch2 value: +/-"},
    {&m_but_save, "Save device setup to file"},
    {&m_check_continuously,"Change angle by slider move"},
    {&m_check_startupini,"Reinit gear when device power up"},
    {&m_but_adapter, "CAN adapter setup"},
    {&m_but_fw_update, "Firmware update"},
    {&m_devices, "Click: Device Select, Double: Rescan All"},
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));

  //==============================================
  // CAN
  //==============================================  
  //Check if already opened
  if(pCAN!=NULL) return TRUE;  
  
  //Open CAN Port
  int result = CAN_Open(&pCAN, "CAN", 500); 
    
  //Check
  if(result<0) return TRUE;
    
  USBCAN_SetHotPlugUSB(pCAN, this->m_hWnd, NULL);

  //Get devices list
  int num = CAN_GetDeviceListLBox(pCAN, &m_devices, CANID_FIRST, CANID_LAST, NULL, NULL);
  
  //Auto select last used device  
  if(num>0)
  {	  
     //Read last used device id from registry 
     int last_id;
     int result = REG.GetIntVar("last_device_id", last_id);
     if(result)
     {
      for(int i=0; i<num; i++)
      {
        //Get new Sw data
        int ID = CAN_ReturnDeviceID(pCAN, i);
        if(last_id==ID) 
          {
            m_devices.SetCaretIndex(i);
            m_devices.SetCurSel(i);
            CAN_device_ID = ID;
            GetDeviceSetup(ID);
            break;
          } 
        } //not found any device
      }	
    }

  // return TRUE  unless you set the focus to a control
  return TRUE;  
}

//------------------------------------------------------------------------------
//Function:	 
//------------------------------------------------------------------------------
void CSCPCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

//------------------------------------------------------------------------------
//Function:	 
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
//------------------------------------------------------------------------------
void CSCPCDlg::OnPaint()
{
  if(IsIconic())
   {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
 else
  {
    CDialogEx::OnPaint();
  }
}

//------------------------------------------------------------------------------
//Function:
// The system calls this function to obtain the cursor to display while the 
// user drags the minimized window.
//------------------------------------------------------------------------------
HCURSOR CSCPCDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CSCPCDlg::CloseAll(void)
{ 
   //Broadcast message for all device
   //CAN_SendCommand(pCAN, PUBLIC_CAN_DEVICE_ID, CAN_CMD_CHANNEL,
   //                CMD_PC_STOP_MONITORING, 0, 0);

   //CAN_RxChannelClose(pCAN, PC_CAN_MONITORING_ID, CAN_CMD_CHANNEL);   
   CAN_Close(pCAN);

   ThreadStop();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CSCPCDlg::OnCancel()
{
  CloseAll();
  CDialogEx::OnCancel();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnButtonAdapter()
{
  CAN_Setup_Dialog(GetSafeHwnd(), pCAN);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnButtonFwUpdate()
{
  //off
  ControlsEnable(OFF);

  //DLL
  ///CAN_ChannelClose(pCAN, CAN_device_ID, CAN_CHANNEL1);
  
  //Save opened device id
  unsigned char last_used_id = CAN_device_ID;

  HWND hWndParent = GetSafeHwnd();
  CAN_ISP_Dialog(hWndParent, pCAN);

  //DLL open CAN Rx channel 
  ///CAN_ChannelOpen(pCAN, &ev_DataAccepted, CAN_device_ID, CAN_CHANNEL1);
  
  //Update device list
  int num = CAN_GetDeviceListLBox(pCAN, &m_devices, CANID_FIRST, CANID_LAST, NULL, NULL);
  if(num>0)
  {
    //DLL, Ask device status again
    GetDeviceSetup(last_used_id);
  }
}

