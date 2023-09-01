//==============================================================================
//File name:    "candll.h"
//Purpose:      Header File
//Version:      9.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
// The following ifdef block is the standard way of creating macros which 
// make exporting from a DLL simpler. All files within this DLL are compiled 
// with the CANDLL_EXPORTS symbol defined on the command line. this symbol 
// should not be defined on any project that uses this DLL. This way any other 
// project whose source files include this file see CANDLL_API functions as 
// being imported from a DLL, whereas this DLL sees symbols defined with this 
// macro as being exported.
#ifndef _CAN_DLL_H_
#define _CAN_DLL_H_

#ifdef  CANDLL_EXPORTS
#define CANDLL_API __declspec(dllexport)
#else
#define CANDLL_API __declspec(dllimport)
#endif

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus
//------------------------------------------------------------------------------
//Adapter USB-Hotplug function:
//------------------------------------------------------------------------------
//Use: Add func afx_msg BOOL OnDeviceChange(UINT EventType, DWORD dwData);
//Add  ON_WM_DEVICECHANGE() to application message_map list
//Put  USBCAN_HotPlugHandler() to OnDeviceChange()
//-----------------------------------------------------------------------------
//USB-CAN Hotplug callback
//-----------------------------------------------------------------------------
//BOOL CCCPCDlg::OnDeviceChange(UINT EventType, DWORD dwData)
//{
//  USBCAN_HotPlugHandler(pCAN, EventType, dwData);  
//  return TRUE;
//}
CANDLL_API int  USBCAN_HotPlugHandler(HANDLE hCAN, unsigned int EventType, DWORD_PTR dwData);
//Here CComboBox pointer from application GUI interface, if not used = NULL
CANDLL_API void USBCAN_SetHotPlugUSB(HANDLE hCAN, HWND hWndParent, HANDLE hAdapterComboBox);

//------------------------------------------------------------------------------
//Adapter control Functions:
//------------------------------------------------------------------------------
//Change Baudrate for CAN-adapter.
//Possible values: 100, 125, 200, 250, 500, 1000, (no supported 0=AUTO)
CANDLL_API int USBCAN_SetBaudrate(HANDLE hCAN, unsigned short baudrate);
CANDLL_API int USBCAN_GetBaudrate(HANDLE hCAN);
CANDLL_API int USBCAN_RunSyncTimer(HANDLE hCAN, int prescaler, int counter);

//------------------------------------------------------------------------------
//API Functions:
//------------------------------------------------------------------------------
//Return connected to PC CAN-adapters list, and put it to ComboBox control 
CANDLL_API int CAN_GetAdapterList(HANDLE lpComboBox, HANDLE pCStringArrayName);
//Return last used adapter name
CANDLL_API int CAN_GetLastUsedAdapterName(char *pBuffer, int length);	
//Call it if need cnahne USB-CAN adapter
CANDLL_API int CAN_ChangeAdapter(HANDLE lpComboBox, HANDLE &lpHandle);

//Open CAN-adapter by name and get lpHandle
//Possible call adapter only with short name: "CAN", without number
//Then can select required number from dialog menu 
CANDLL_API int CAN_Open(HANDLE *hCAN, char const *pCAN_adapter_name, short CAN_speed);

//Close CAN-adapter
CANDLL_API void CAN_Close(HANDLE lpHandle);

//Add a Application Data handler for incoming data from the adapter
//If this is not called, the application will not be able to receive data
CANDLL_API int  CAN_AddRxHandler(HANDLE lpHandle, 
                                 void(*pRXhandler)(char *pBuf, int length));

//Remove Application Data handler from Rx thread
CANDLL_API void CAN_RemoveRxHandler(HANDLE lpHandle);

//Set Msg Acceptance Mask by device ID range 
//For reset all ID, write 0. For accept all msg, write:(0,0xFFFF)
CANDLL_API int  CAN_SetRxFilter(HANDLE lpHandle, 
                                short CAN_ID_From, short CAN_ID_To);

//Modification Msg Acceptance Mask by add new single device ID 
CANDLL_API int  CAN_AddRxID(HANDLE lpHandle, short CAN_DeviceID);

//Set Msg Acceptance Mask directly by bit
//For reset mask, write 0. For pass all msg, write 0xFFFF
CANDLL_API int  CAN_SetRxMask(HANDLE lpHandle, short CAN_ID_Mask);


//Add a Application CAN Event handler for rx events from the adapter
//If this is not called, the application will not be able to receive data
CANDLL_API int  CAN_AddEventHandler(HANDLE lpHandle, void(*pEVhandler)
                                   (int ev_id, char *pBuf, int length));

//Remove Application Data handler from Rx thread
CANDLL_API void CAN_RemoveEventHandler(HANDLE lpHandle);

//Send 3 bytes to device: cmd + param1 + param2
CANDLL_API int  CAN_SendCommand(HANDLE lpHandle,
                   short CAN_device_ID,
                   unsigned char command,
                   unsigned char param1,
                   unsigned char param2);

//Send bytes array to device
CANDLL_API void CAN_SendData(HANDLE lpHandle, 
                   short CAN_device_ID,
                   unsigned char *pBuffer,
                   unsigned char  size);

//Read CAN MSG data from buffer  
CANDLL_API int CAN_ReadData(HANDLE lpHandle,
                  short CAN_device_ID,
                  unsigned char *pBuffer,
                  unsigned char  Rx_length);
                    
//Send bytes array to device and then wait answer with timeout
CANDLL_API int CAN_ReadDevice(HANDLE lpHandle, 
                  short device_id,
                  unsigned char *pBuffer,
                  unsigned short Rx_length,
                  unsigned char *pTxData,
                  unsigned short Tx_length,
                  int timeout_ms);
   
//Then CAN data come, function set event DataAccepted													
CANDLL_API int  CAN_RxChannelOpen(HANDLE lpHandle, 
                                  HANDLE event_DataAccepted,
                                  short CAN_DeviceID);
//Launch RX thread 
CANDLL_API int  CAN_RxChannelClose(HANDLE lpHandle, short CAN_DeviceID);

//Check device presence on CAN Bus
CANDLL_API int CAN_CheckDevice(HANDLE lpHandle, short CAN_device_ID);

//Return devices CAN ID by position in list
CANDLL_API int CAN_ReturnDeviceID(HANDLE lpHandle, int position);

//Return devices count
CANDLL_API int CAN_ReturnDeviceCount(HANDLE lpHandle);

//Return devices list add to ListBox
CANDLL_API int CAN_GetDeviceListLBox(HANDLE lpHandle, 
                                     HANDLE lpListBox,
                                     short device_id_low, 
                                     short device_id_high,
                                     LPVOID pIDs, //CByteArray pointer
                                     LPVOID pNames); //CStringArray pointer

//Return devices list add to ComboBox
CANDLL_API int CAN_GetDeviceListCombo(HANDLE lpHandle, 
                                      HANDLE lpComboBox,
                                      short device_id_low, 
                                      short device_id_high,
                                      LPVOID pIDs, //CByteArray pointer
                                      LPVOID pNames); //CStringArray pointer

CANDLL_API int CAN_GetDLLVersion(void);

//------------------------------------------------------------------------------
//Dialog windows
//Use: CAN_xxx_Dialog(GetSafeHwnd(), pCAN);
//------------------------------------------------------------------------------
//Remote device firmware update dialog
CANDLL_API void CAN_ISP_Dialog(HWND hWndParent, HANDLE lpHandle);

//Communication setup dialog
CANDLL_API void CAN_Setup_Dialog(HWND hWndParent, HANDLE lpHandle);

#ifdef __cplusplus
}
#endif //end __cplusplus

//For CAN protocol communication test, Send array + check array
//CANDLL_API void CAN_ProtocolTest(HANDLE lpHandle, unsigned char CAN_device_ID);

//==============================================================================
//Defines:
//==============================================================================
#define DLL_VERSION			0x09

//---------------------------------------
//Virtual CAN channels
//---------------------------------------
#define CAN_CHANNEL7        0x07  //0b111
#define CAN_CHANNEL6        0x06  //0b110
#define CAN_CHANNEL5        0x05  //0b101
#define CAN_CHANNEL4        0x04  //0b101
#define CAN_CHANNEL3        0x03  //0b011
#define CAN_CHANNEL2        0x02  //0b010
#define CAN_CHANNEL1        0x01  //0b001
#define CAN_CMD_CHANNEL     0x00  //0b000

//---------------------------------------
//Cmd for USB-CAN adapter control
//---------------------------------------
#define USBCAN_DATA_EXCHANGE         0xF5
#define USBCAN_STOP_SYNC_TIMER       0x09
#define USBCAN_RUN_SYNC_TIMER        0x08
#define USBCAN_SET_SPEED_1000        0x07
#define USBCAN_SET_SPEED_500         0x06
#define USBCAN_SET_SPEED_250         0x05
#define USBCAN_SET_SPEED_200         0x04
#define USBCAN_SET_SPEED_125         0x03
#define USBCAN_SET_SPEED_100         0x02
#define USBCAN_SET_SPEED_AUTO        0x01
#define USBCAN_ADAPTER_ALREADY_OPEN  2
#define USBCAN_ADAPTER_CONNECTED     1
#define USBCAN_ADAPTER_NOTFOUND      0

#define PUBLIC_CAN_DEVICE_ID         0xFF 

#endif
