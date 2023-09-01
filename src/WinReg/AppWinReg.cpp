//==============================================================================
//File name:    "AppWinReg.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2016, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "AppWinReg.h"

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

//------------------------------------------------------------------------------ 
//Constructor 
//------------------------------------------------------------------------------
CAppWinReg::CAppWinReg()
{	
  //Get windows application pointer
  pApp = AfxGetApp();   
  //section name 
  section = "Variables";
}

//------------------------------------------------------------------------------
//Destructor 
//------------------------------------------------------------------------------
CAppWinReg::~CAppWinReg(){}

//------------------------------------------------------------------------------
//Function: инициализация реестра переменными при инсталляции программы
//------------------------------------------------------------------------------
int CAppWinReg::FirstLaunch(void)
{   
  //подготовка для записи в реестр установок, по умолчанию название *.exe
  CString exe_name(pApp->m_pszExeName);
  
  //считать переменную, если её нет - возвратит число "237"
  int ret = pApp->GetProfileIntA(section,"first", 1);

  //если программа не инсталлилась на машину, проинициализировать 
  if(ret != 214)
   {
	  BOOL ret;

	  //---- Common variables
	  pApp->WriteProfileInt(section, "first", 214);
      ret = pApp->GetProfileIntA(section,"first", 1);

      //Если переменная не прописалась - сообщение
      ASSERT(ret == 214);
      return 1;
  }

  return 0;
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CAppWinReg::GetTxtVar(CString txt, CString &pTxt)
{ 
  CString temp;
  
  //read from WinRegistry
  temp = pApp->GetProfileStringA(section, txt, "");
  if (temp=="") return 0;

  //copy txt to comsumer
  pTxt = temp;
  return 1;
}  

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CAppWinReg::SetTxtVar(CString txt, CString &pTxt)
{
  //write to WinRegistry
  return pApp->WriteProfileStringA(section, txt, "");
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CAppWinReg::GetIntVar(CString var_name, int &pVar)
{  
  UINT res; 
  //read from WinRegistry
  res = pApp->GetProfileIntA(section, var_name, -1);
  
  if (res==-1) return 0;
  pVar = res;

  return 1;
}  

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CAppWinReg::SetIntVar(CString var_name, int value)
{
  //write to WinRegistry
  return (int) pApp->WriteProfileInt(section, var_name, value);
}
