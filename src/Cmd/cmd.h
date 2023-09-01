//==============================================================================
//For All devices
//==============================================================================
#define CMD_NO_OPERATION				0x00
#define CMD_CAN_GETDATA					0x01
#define CMD_BEEP						0x02
#define CMD_POWER_ON					0x03
#define CMD_POWER_OFF					0x04
#define CMD_READ_INFO_LENGTH			0x05
#define CMD_READ_DEVICE_INFO			0x06 
      
#define CMD_READ_MEMORY					0x10
#define CMD_READ_EEPROM					0x11
#define CMD_WRITE_EEPROM				0x12
#define CMD_GET_SETUP_LENGTH			0x13
#define CMD_GET_DEVICE_SETUP			0x14
#define CMD_SAVE_DEVICE_SETUP			0x15
#define CMD_SET_CAN_ID					0x16
#define CMD_SET_DEVICE_NAME				0x17
#define CMD_JUMP_TO_BOOTLOADER			0xAA 

//==============================================================================
//Device control
//==============================================================================
#define CMD_SET_SETUP_TO_DEFAULT		0xFD
#define CMD_DEVICE_BLINK				0xFC
#define CMD_SET_ANGLE_CH2				0xFB
#define CMD_SET_ANGLE_CH1				0xFA
#define CMD_SET_ALL_ANGLE				0xF9
#define CMD_SET_MODE_INI				0xF8
#define CMD_PC_MONITORING				0xF7

//==========================================
//From remote device, (AVR) project
//Error messages for monitoring
//==========================================
#define BATTERY_POWER_LOW				0xE2
#define PC_CAN_MONITORING_ID			0xFE

//CAN address range 
//Servo controllers area - Address start from 0xA0
#define CANID_FIRST						0xA0     //Address start from 0xA0
#define CANID_LAST						0xA0+25  //Last Address
