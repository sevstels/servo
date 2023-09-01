//==============================================================================
//File name:   "SCPCDlg.h.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "ColorStaticST.h"
#include "AppWinReg.h" 
#include "dialog-pos.h"
#include "tooltip.h"
#include "device.h"
#include "candll.h"
#include "cmd.h"

#pragma once

//For debuging set path to dll folder: 
//Configuration Properties --> Debugging --> Environment
//PATH=$(ProjectDir)candll\x86/x64;$(PATH);

#define ON		1
#define OFF		0

// CSCPCDlg dialog
class CSCPCDlg : public CDialogEx
{
  // Construction
  public:
  // standard constructor
  CSCPCDlg(CWnd* pParent = NULL);	

  // Dialog Data
  enum{IDD = IDD_SCPC_DIALOG };
  
  HANDLE pCAN;	
  CWinThread *pThread;
  CAppWinReg REG;
  CToolTip ToolTip; 
  CDPOS POS;
  CEvent ev_Quit;
  CEvent ev_DataAccepted;
  Device_Setup DEV;

  //Controls
  CListBox m_devices;
  CEdit m_edit_angle_ch1;
  CEdit m_edit_angle_ch2;
  CSliderCtrl m_slider_angle_ch1;
  CSliderCtrl m_slider_angle_ch2;
  CButton m_but_adapter;
  CButton m_but_set_angle;
  CButton m_but_fw_update;
  CButton m_but_save;
  CButton m_check_continuously;
  CButton m_check_startupini;
   
  CColorStaticST m_txt_device_id;
  CColorStaticST m_txt_device_power;

  unsigned char CAN_device_ID;
  int m_first_launch;
  int m_check;

  void ThreadLaunch(void);
  void ThreadStop(void);
  void SendtoDevice(int command);
  void CloseAll(void);
  void ControlsIni(void);
  void ControlsEnable(BOOL on_off);
  void ControlsUpdate(char number);
  int  GetDeviceSetup(unsigned char device_id); 
  float CalcAngle(int steps);
  unsigned short CalcPWMvalue(float angle);
  int Check_Limits(CString *pAngle, int channel);

  // Implementation
  protected:
  HICON m_hIcon;

  // DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void OnCancel();

  // Generated message map functions
  virtual BOOL OnInitDialog(); 	
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
  afx_msg BOOL OnDeviceChange(UINT EventType, DWORD_PTR dwData);

  DECLARE_MESSAGE_MAP()

  public:
  afx_msg void OnButtonAdapter();
  afx_msg void OnButtonFwUpdate();
  afx_msg void OnButtonSetAngle();

  afx_msg void OnSelchangeDeviceList();
  afx_msg void OnDblclkDeviceList();
  afx_msg void OnClickedCheckNpr();
  afx_msg void OnCheckStartUpIni();
  afx_msg void OnButtonSave();

  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
