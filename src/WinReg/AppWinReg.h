//==============================================================================
//File name:   "AppWinReg.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2016, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _APPLICATION_WINREG_SERVICE_H_
#define _APPLICATION_WINREG_SERVICE_H_

#include "stdafx.h"
#include <afxmt.h>

//Add this MACRO to InitInstance() or OnInitDialog
#define CAppWinReg_ini SetRegistryKey(_T((LPCTSTR)(AfxGetApp()->m_pszExeName)))

//------------------------------------------------------------------------------
// CUSBbuffer class
//------------------------------------------------------------------------------
class CAppWinReg
{
//Construction
public:
  CAppWinReg();
  virtual ~CAppWinReg();

//Operations
public:
  int FirstLaunch(void); //Application first launch
  int GetIntVar(CString var_name, int &pVar);
  int SetIntVar(CString var_name, int value);
  int GetTxtVar(CString var_name, CString &pTxt);
  int SetTxtVar(CString var_name, CString &pTxt);

//Attributes
private:
  //Application pointer указатель на обьект *приложение* (на базовый класс)
  CWinApp *pApp;

//Implementation
protected:
  CString section;

};
#endif
