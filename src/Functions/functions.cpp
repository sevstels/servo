//==============================================================================
//File name:    "functions.cpp"
//Purpose:      Source implementation file
//Version:      1.00
//Copyright:    (c) 2017, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "SCPC.h"
#include "SCPCDlg.h"
#include "afxdialogex.h"
#include "files.h"

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

extern DWORD THREAD_DATA_RX(LPVOID lParam);

//------------------------------------------------------------------------------
//Function:	Dialog position saver
//------------------------------------------------------------------------------
void CSCPCDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
BOOL CSCPCDlg::PreTranslateMessage(MSG* pMsg)
{
 //Check message from press ENTER 
 if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {
    //---------------------------------------------------------
    // Set Angle
    //---------------------------------------------------------
    if(pMsg->hwnd==m_edit_angle_ch1.m_hWnd || 
	   pMsg->hwnd==m_edit_angle_ch2.m_hWnd)
       {
         OnButtonSetAngle();
         return TRUE;
       }
  }	

  //Tooltip msg handler
  ToolTip.PreTranslateMessage(pMsg);
  return CDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
//USB Hotplug callback
//-----------------------------------------------------------------------------
BOOL CSCPCDlg::OnDeviceChange(UINT EventType, DWORD_PTR dwData)
{ 
  int result = USBCAN_HotPlugHandler(pCAN, EventType, dwData);
  int count, device;

  //USB-CAN come, connected
  if(result==1)
    {
      //Get devices list 
      count = CAN_GetDeviceListLBox(pCAN, &m_devices, CANID_FIRST, 
								    CANID_LAST, NULL, NULL);
      //If devices > 0
	  if(count>0)
	  {
	   	  device = CAN_CheckDevice(pCAN, CAN_device_ID);
		  if(device)
		  {
		    //DLL, Ask device status again
            GetDeviceSetup(CAN_device_ID);
		  }
		  else
		  {
		  	//old device not found
		  }
	  }
    }

  //USB-CAN leave, disconnected
  if(result==-1)
   { 
     m_devices.ResetContent();
	 m_devices.AddString(_T("ERROR:")); 
	 m_devices.AddString(_T("USB-CAN adapter"));
	 m_devices.AddString(_T("disconnected"));
	 
	 //Disable all
     ControlsEnable(OFF);
   }   

  return TRUE;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::ThreadLaunch(void)
{
 //Stop threads 
 ThreadStop();

 //Restart all threads
 pThread = AfxBeginThread((AFX_THREADPROC)THREAD_DATA_RX, this, 
                           THREAD_PRIORITY_TIME_CRITICAL);

 pThread->m_bAutoDelete = FALSE; 
}

//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
void CSCPCDlg::ThreadStop(void)
{
 ev_Quit.SetEvent();

 if(pThread)
 {
    WaitForSingleObject(pThread->m_hThread, INFINITE);
    delete pThread; pThread = NULL;
 }
 ev_Quit.ResetEvent();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::ControlsUpdate(char number)
{
  CString txt;
  float angle; 

  //
  angle = CalcAngle(DEV.Angle_ch1);
  txt.Format("%3.2f", angle);
  m_slider_angle_ch1.SetPos(DEV.Angle_ch1);
  m_edit_angle_ch1.SetWindowTextA(txt);
  //
  angle = CalcAngle(DEV.Angle_ch2);
  txt.Format("%3.2f", angle);
  m_slider_angle_ch2.SetPos(DEV.Angle_ch2);
  m_edit_angle_ch2.SetWindowTextA(txt);
  
  //
  if(DEV.Mode!=0) 
  {
    m_check_startupini.SetCheck(TRUE);
  }
  else{m_check_startupini.SetCheck(FALSE);}
  
  //
  txt.Format("DevID: 0x%X", CAN_device_ID);
  m_txt_device_id.SetWindowTextA(txt);
  
  //
  if(DEV.Power_positive<4.6)
  {
	txt.Format("  Error: %2.2fv!", DEV.Power_positive);
    m_txt_device_power.SetBkColor(RGB(10,0,0));
    m_txt_device_power.SetBlinkTextColors(RGB(255,0,0), RGB(160,0,0));
	m_txt_device_power.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST); 
  }
  else
  {
	txt.Format(" Power: %2.2fv", DEV.Power_positive);
	m_txt_device_power.SetBkColor();
    m_txt_device_power.SetTextColor();
	m_txt_device_power.StartTextBlink(FALSE, CColorStaticST::ST_FLS_FAST); 
  }	   
  
  m_txt_device_power.SetWindowTextA(txt);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::ControlsIni(void)
{
  //  
  ///m_slider_angle_ch1.SetRange(12000, 36000, TRUE); //
  m_slider_angle_ch1.SetRange(-30*4, 30*4, TRUE); //
  m_slider_angle_ch1.SetPageSize(4);
  m_slider_angle_ch1.SetLineSize(1); //step +/-
  m_slider_angle_ch1.SetPos(0);
  //
  m_slider_angle_ch2.SetRange(-30*4, 30*4, TRUE); //
  m_slider_angle_ch2.SetPageSize(4);
  m_slider_angle_ch2.SetLineSize(1); //step +/-
  m_slider_angle_ch2.SetPos(0);  
  
  //
  m_edit_angle_ch1.SetWindowTextA("");
  m_edit_angle_ch2.SetWindowTextA("");
  m_check_startupini.SetCheck(FALSE);

  //  
  int value;
  REG.GetIntVar("continuously", value);
  
  if(value>0) 
  {
    m_check_continuously.SetCheck(TRUE);
    m_check = 1;
  }
  else{m_check_continuously.SetCheck(FALSE); m_check=0;}
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::ControlsEnable(BOOL on_off)
{
  m_edit_angle_ch1.EnableWindow(on_off);
  m_edit_angle_ch2.EnableWindow(on_off);
  m_slider_angle_ch1.EnableWindow(on_off);
  m_slider_angle_ch2.EnableWindow(on_off);
  m_but_set_angle.EnableWindow(on_off);
  m_but_save.EnableWindow(on_off);
  m_check_continuously.EnableWindow(on_off);
  m_check_startupini.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
float CSCPCDlg::CalcAngle(int handle_value)
{
  float angle =(float)handle_value;
  angle /=4;

  return angle;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
unsigned short CSCPCDlg::CalcPWMvalue(float angle)
{  /*
  float angle =(float)handle_value;
  angle /=4;

	  /*
  //---- Check in
  if(result>0)
  {
	steps = CalcSteps(&txt2, result_ch2);
    if(result_ch2==0)
    { 
      msg = "Channel " 
	  txt.Format("%d", channel);
	  msg += txt + " Value must be: -30°<->30°";      
	  ::AfxMessageBox(msg);
      return;
    }
    else
    {
      DEV.Angle_ch2 = steps_ch2;
    }
  }
  else
  { 
    ::AfxMessageBox("Error: Channel 2 must have value!");
    return;
  }	  */



			
  return 1;
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CSCPCDlg::Check_Limits(CString *pAngle, int channel)
{
  CString msg = "Channel ", txt;
  double value;	int ret;
  txt.Format("%d", channel);

  //if edit box empty
  if(*pAngle=="")
  { 
    msg += txt +" must have value!";
    ::AfxMessageBox(msg);
	return -1;
  }

  //(read value in any format)
  ret = sscanf_s(*pAngle,"%Lf",(double*)&value);

  if(ret<1)
  {
    msg += txt +" is not digit!";  
	::AfxMessageBox(msg);
	return -1;
  }
  
  if(value>30 || value<-30)
  {
	msg += txt + " Value must be between: -30°... +30°";      
    ::AfxMessageBox(msg);
    return -1;
  }

  value *= 4;

  //save 
  if(channel==1) DEV.Angle_ch1 = (signed short)value;
  if(channel==2) DEV.Angle_ch2 = (signed short)value;

  return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnButtonSetAngle()
{
  CString txt1, txt2;
  unsigned char data[5];
  int result_ch1, result_ch2, result=0;
  int dlgid_ch1 = m_edit_angle_ch1.GetDlgCtrlID();
  int dlgid_ch2 = m_edit_angle_ch2.GetDlgCtrlID();

  //int value = GetDlgItemInt(dlgid, &result, TRUE);
  //GetDlgItemTxt(dlgid, &result, TRUE);

  result_ch1 = GetDlgItemTextA(dlgid_ch1, txt1);
  result_ch2 = GetDlgItemTextA(dlgid_ch2, txt2);

  if(result_ch1>0) result = Check_Limits(&txt1, 1);
  else return;
  if(result<0) return;

  if(result_ch2>0) result = Check_Limits(&txt2, 2);
  else return;
  if(result<0) return;

  //----- Send to device
  data[0] = CMD_SET_ALL_ANGLE;
  memcpy(&data[1], &DEV.Angle_ch1, 4);

  //Send 5 bytes array to device
  CAN_SendData(pCAN, CAN_device_ID, data, 5);

  ControlsUpdate(0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  //Conversion to CSliderCtrl*
  CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;

  //Get current hWnd
  HWND hWnd = pSlider->m_hWnd;

  //---- Check slider hWnd
  if(hWnd == m_slider_angle_ch1.m_hWnd)
   {  
     signed short steps = (signed short)m_slider_angle_ch1.GetPos();
     DEV.Angle_ch1 = steps;
     
     float angle = CalcAngle(steps);
     CString txt;
     txt.Format("%3.2f", angle); 
     
     m_edit_angle_ch1.SetWindowTextA(txt);

     if(m_check){
     //DLL, command 
     CAN_SendCommand(pCAN, CAN_device_ID, CMD_SET_ANGLE_CH1,
                    (unsigned char)steps,(unsigned char)(steps>>8));}
   }
  
  //---- Check slider hWnd
  if(hWnd == m_slider_angle_ch2.m_hWnd)
   {  
     signed short steps = (signed short)m_slider_angle_ch2.GetPos();
     DEV.Angle_ch2 = steps;
     
     float angle = CalcAngle(steps);
     CString txt;
     txt.Format("%3.2f", angle);
     
     m_edit_angle_ch2.SetWindowTextA(txt);

     if(m_check){
     //DLL, command 
     CAN_SendCommand(pCAN, CAN_device_ID, CMD_SET_ANGLE_CH2,
                    (unsigned char)steps,(unsigned char)(steps>>8));}
   }

  CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnSelchangeDeviceList()
{
  int ID;
  int current_device = m_devices.GetCaretIndex();
  if(current_device<0) return;

  //Get id
  ID = CAN_ReturnDeviceID(pCAN, current_device);
  if(ID<0) return; //if no devices

  if(CAN_device_ID !=((unsigned char)ID) || m_first_launch==0)
  {	 
    //Get new Sw data
    CAN_device_ID = (unsigned char)ID;
    
    //Save last used device id
    REG.SetIntVar("last_device_id",ID);
    //--
    GetDeviceSetup(CAN_device_ID);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnDblclkDeviceList()
{
  //off
  ControlsEnable(OFF);
   
  m_first_launch = 0;
  CAN_device_ID = 0;

  //Get new devices list
  CAN_GetDeviceListLBox(pCAN, &m_devices, CANID_FIRST, CANID_LAST, NULL, NULL);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnClickedCheckNpr()
{
  m_check = m_check_continuously.GetCheck();
  REG.SetIntVar("continuously", m_check);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnCheckStartUpIni()
{
  int check = m_check_startupini.GetCheck();

  //DLL, command 
  CAN_SendCommand(pCAN, CAN_device_ID,CMD_SET_MODE_INI,(unsigned char)check, 0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CSCPCDlg::GetDeviceSetup(unsigned char device_id) 
{ 
  int res;
  unsigned char cmd;
  
  //DLL
  cmd = CMD_GET_DEVICE_SETUP;
  res = CAN_ReadDevice(pCAN, device_id, 
                      (unsigned char*)&DEV, sizeof(DEV), &cmd, 1, 100);

  if(res>0)
   {
     ControlsUpdate(0);
     ControlsEnable(ON);
	 //m_txt_device_id.SetBkColor();
	 m_txt_device_id.SetTextColor();
   }
  else 
  {
	ControlsEnable(OFF);
   	m_txt_device_id.SetWindowTextA(" Select device");
	m_txt_device_power.SetWindowTextA("");
    //m_txt_device_id.SetBkColor(RGB(0,0,0));
    m_txt_device_id.SetTextColor(RGB(255,0,0));
  }

  return res;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CSCPCDlg::OnButtonSave()
{
  CString file, msg, txt;
  //open file 
  file.Format("DeviceID 0x%X", CAN_device_ID);
  
  //prep info
  msg = "Remote device:\t\tServo \"ES08-MD\"\r\n";
  txt.Format("Device CAN ID:\t\t0x%X", CAN_device_ID);
  msg += txt +" = "; 
  txt.Format("%d",CAN_device_ID);
  msg += txt + "\r\n";
    
  //--
  txt.Format("Servo CH1, angle:\t%3.2f°", (float)DEV.Angle_ch1/4);
  msg += txt +"\r\n";
  //--
  txt.Format("Servo CH2, angle:\t%3.2f°", (float)DEV.Angle_ch2/4);
  msg += txt +"\r\n";

  //--
  txt.Format("PWM width Min:\t\t%d uS", 
	        (unsigned short)((float)DEV.PWM_width_min*62.5)/10);
  msg += txt +"\r\n";
  
  //--
  txt.Format("PWM width Max:\t\t%d uS", 
	        (unsigned short)((float)DEV.PWM_width_max*62.5)/10);
  msg += txt +"\r\n";
  
  //--
  txt.Format("Gear range:\t\t%d°", DEV.Angle_range);
  msg += txt +"\r\n";
  
  //--
  txt.Format("Angle scale div:\t%d", DEV.Angle_div_factor);
  msg += txt +"\r\n";

  //--
  msg += "Firmware version:\t";
  txt.Format("ver %d.",(DEV.Firmware_version>>4));                                     
  msg += txt;
  txt.Format("%d",(DEV.Firmware_version)&0x0F);
  msg += txt + "\r\n";
 
  //--
  msg += "Firmware build data:\t";
  txt.Format("%d%d", 20, DEV.Firmware_build_data);
  txt.Insert(4,"."); txt.Insert(7,"."); txt.Insert(10," ");	txt.Insert(13,":");
  msg += txt + "\r\n";

  //--
  msg += "Firmware bootloader:\t";
  if(DEV.Bootloader>0) msg += "On board";                                   
  else  msg += "Absence";

  //Save data to file 
  Save_File(file, (unsigned char*)msg.GetBuffer(), 0, msg.GetLength());
}
