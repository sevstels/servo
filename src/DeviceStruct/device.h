//---- Signal samples buffers number
#define SCPC_VERSION				0x10

//Required Firmware version
#define FIRMWARE_VERSION_LOWER		0x10 //device firmware
#define ID_TIMER_MONITORING			100
#define MONITORING_PERIOD_MS		2000
#define DEVICE_TYPE_SERVOCONTROL	0x28 //Servo controller
 
//Servo gear setup
//#define PULSE_WITDTH_MIN			12000   
//#define PULSE_WITDTH_MAX			36000
//#define PULSE_WITDTH_CENTER	((PULSE_WITDTH_MIN+PULSE_WITDTH_MAX)/2)
//#define ANGLE_RANGE				60
//#define ANGLE_DIV_FACTOR			4

//-----------------------------------------------
//Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct Setup_struct
 {
   float Power_positive;
   signed short   Angle_ch1;
   signed short   Angle_ch2;
   unsigned char  Mode; 
   unsigned short PWM_width_min;
   unsigned short PWM_width_max;   
   unsigned short Angle_range;
   unsigned char  Angle_div_factor;   
   unsigned char  Bootloader;
   unsigned char  Firmware_version;
   unsigned long  Firmware_build_data;

 }Device_Setup;
#pragma pack(pop)
//-----------------------------------------------
