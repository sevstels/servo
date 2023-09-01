//==============================================================================
//File name:   "SCPC.h"
//Purpose:      Header File	for the PROJECT_NAME application
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

// main symbols
#include "resource.h"		

// CSCPCApp:
// See SCPC.cpp for the implementation of this class
class CSCPCApp : public CWinApp
{
  public:
  CSCPCApp();

  // Overrides
  public:
  virtual BOOL InitInstance();
  // Implementation
  DECLARE_MESSAGE_MAP()
};

extern CSCPCApp theApp;
