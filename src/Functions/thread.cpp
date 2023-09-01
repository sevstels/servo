//==============================================================================
//File name:    "thread.cpp"
//Purpose:      Source implementation file
//Version:      1.00
//Copyright:    (c) 2017, Akimov Vladimir  E-mail: decoder@rambler.ru	
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

//==============================================================================
//Thread: Monitoring CAN bus, receive CAN messages
//==============================================================================
//Thread function, обьявление функций потоков
DWORD THREAD_DATA_RX(LPVOID lParam)
{
 //Get pointer to Class
 CSCPCDlg *pDlg = (CSCPCDlg*)lParam;
 //ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CLPESDlg)));

 //Synchronisation objects
 CSyncObject *pSyncObj[2];
 pSyncObj[0] = &pDlg->ev_DataAccepted;
 pSyncObj[1] = &pDlg->ev_Quit;
  
 unsigned char LBuf[2]={0,0};
 unsigned char val_old = 0;
 int buffer_count;

 //---- launch Tread loop 
 for(;;)
    {
        //waiting event
        CMultiLock Lock(pSyncObj, 2);
                
        switch (Lock.Lock(INFINITE, FALSE))
        {			
           //------ EVENT from flag_bufferNotEmpty
           case WAIT_OBJECT_0:
	
           //Get data from ring USB buffer
           do 
           {
             //Get data from USB buffer
             buffer_count = CAN_ReadData(pDlg->pCAN,  
                                         pDlg->CAN_device_ID,
										 LBuf, 1);	  
                
           } while(buffer_count>1);
              /*   
            //Unpacking data 
            pDlg->channel_1 = LBuf[0]&0x0F;
            pDlg->channel_2 = LBuf[0]>>4;
            pDlg->BitmapLED(ON);	 */
		    pDlg->ControlsUpdate(0);
            pDlg->ControlsEnable(ON);	
            //================
            break;
            
            //------ EVENT from ev_Quit
            case WAIT_OBJECT_0+1:
            //------ EVENT for all next case
            default:
            //End Tread	loop 
            AfxEndThread(0, TRUE);
            return 0; 
        }
    }
}
