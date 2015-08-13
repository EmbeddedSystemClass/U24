/*==== DECLARATION CONTROL ==================================================*/
#ifndef _DT_CMD_TYPE_H_
#define _DT_CMD_TYPE_H_

#define ASIC_SIZE_UART 58
#define ASIC_SIZE_USB  69

#define DT_START_ADDR    0x10000000

#define UART_LOW_SPEED     0x00
#define PACKET_SIZE_RETURN 3
#define PACKET_SIZE_ANSWER 3
#define PACKET_SIZE_GENCMD 6
#define SECTOR_SIZE        512

//old
//#define LOW_TO_TARGET      0x00
//#define SIZEPACKET         0x03
//#define PCC_COMMAND_RETURN 0x04
//#define DATAPACKET         0x10
//#define ANSWER_PACKET      0x11
//#define UNLIMITED_DATA     0x05
//#define UNEXPECTED_ERROR   -100
//#define TIMEOUT_ERROR         0

enum DtPacketCommandCode {
	DT_PKT_REQUEST    = 0x10,
	DT_PKT_ACK        = 0x11,
	DT_PKT_PROGRESS   = 0x12,
	DT_PKT_RESULT     = 0x13,
	P21_DATADL_REQUEST = 0x20,
	P21_DATADL_ACK     = 0x21,
	P21_DATAUL_REQUEST = 0x30,
	P21_DATAUL_ACK     = 0x31,
};

/* Alex.Chen Modify Start 2011/06/02 */
//static const int DT_PKT_BLOCK_SIZE =   32;
static const int DT_PKT_BLOCK_SIZE = 1024;
/* Alex.Chen Modify End 2011/06/02 */
enum MTKPacketCommandCode {
	MTK_REQ_SEND_AP         = 0x10,
	MTK_REQ_SEND_MODEM_1_2  = 0x11,
	MTK_REQ_SEND_MODEM_2    = 0x12,
};

static const int DT_USB_BLOCK_SIZE = 2048;

#define DT_PKT_HEADER_SIZE  4

//erorr list
//we may need to verify
//1.which command is fail
//2.command fail reason

//Level 1:  transmission level (RS232/USB)
//The reason may be comport error,driver problem , or target no response
//Is it necessary to handle all kind of situation?
enum P21CommandTransErrorCode {
	HS_READ_DATA_FAIL         = -11,
	HS_WRITE_DATA_FAIL        = -12,
	HS_READ_DL_PROTOCOL_FAIL  = -13,
	HS_WRITE_DL_PROTOCOL_FAIL = -14,
	HS_READ_DL_DATA_FAIL      = -15,
	HS_WRITE_DL_DATA_FAIL     = -16,
};

//Level 2: A general command
enum P21CommandReturnCode {
	//FFYang def
	HS_PROGRESS = 88,
	HS_EMPTY	=  2,
	HS_OK       =  1,
	HS_FAIL     =  0, //The command is complete but the target response fail.
	HS_TIMEOUT             = -1,
	HS_NOT_INITIALIZE      = -2,
	HS_OPEN_DEVICE_FAIL    = -3,
	HS_FILE_NOT_FOUND      = -4,
	HS_INVALID_CHECKSUM    = -5,
	HS_INVALID_SIZE        = -6,
	HS_PC_LOAD_FILE_FAIL   = -7,
	//HS_PC_WRITE_FILE_FAIL  = -8, // not found in source code
	//HS_PC_DELETE_FILE_FAIL = -9, // not found in source code
	HS_ADB_OPEN_TCP_FAIL    = -8,  // 2010-06-23 Evonne: error code for open socket
	HS_STARTUP_SOCKET_FAIL  = -9,
	HS_CREATE_SOCKET_FAIL   = -10,
	HS_CONNECT_SOCKET_FAIL  = -11,

	//FFYang def
	HS_GET_DEVICE_FAIL = -98,
	HS_SET_DEVICE_FAIL = -99,
};

enum QthPortType {
	QTH_PORT_COM_MAX = 999, // COM range: 1 ~ 999
	QTH_PORT_USB = 1000, // USB
	QTH_PORT_TCP = 2000, // TCP
	QTH_PORT_LAST = 2000 + 1,
};

//Level 3:Composited Command Level:
//        We may combine several commands as a composited command
//        If one of the command fail ,we should know which one is failure
//        and its failure reason
//#define P21_ERR_CMD_BASE 100
enum P21CompositedCommandErrorCode {
	P21_ERR_TYPE_UART           =      0,
	P21_ERR_TYPE_USB            = -10000,

	P21_ERR_OPENDEVICE          =  -100,
	P21_ERR_CLOSEDEVICE         =  -200, 
	P21_ERR_READDEVICE          =  -300,
	P21_ERR_WRITEDEVICE         =  -400,
	P21_ERR_USBTIJUMPADDR       =  -500,
	P21_ERR_GENERALCMD          =  -600,
	P21_ERR_GENERALCMDIN        =  -700,
	P21_ERR_GENERALCMDOUT       =  -800,
	P21_ERR_GENERALCMDINOUT     =  -900,
	P21_ERR_USBGENDOWNLOADCMD   = -1000,
	P21_ERR_IMAGE_CHECKSUM_FAIL = -1100,
	P21_ERR_TILOAD2ND           = -1200,
	P21_ERR_TILOADDDR           = -1300,
	P21_ERR_USBGENUPLOADCMD     = -1400,
	P21_ERR_NORMAL_COMMAND      = -1500,
};


//The range between modules should be 10000-level. 
//#define P21_ERR_TYPE_BASE  10000
// 2006-09-06 YTT 
//I think the best choice is to use bit format .
//ex: 0xFF 0XFF 0XFF 0XFF
//We can use it to implement 4-level error code  
//Anyway in DLL level I think it would be less usage to handle Command-Level error
//We can implement it in Repair Tool. 

// 2006-09-12 YTT
//I read the error code of XDM:
//XDM seprates error code by "Class" and "feature"
//it uses a base region for specify class
//ex:    DATABASE_BASE 0
//       E2PTREE_ERROR_BASE 100    
//       ERROR_CODE_BASE 10000 
//       RS232_CLASS_ERROR_BASE ERROR_CODE_BASE+0
//
//I think it is not a ideal solution for our project(P21) 
//We have 2 main module 
// -RF TEST(ETM)
//   We can create a base region for RF 
// -BB TEST
//  It is our main control DLL which includes RS232/USB command
// Above express that a "composited" command even if we know it gets RS232_ERROR ,
// we can not clearly know which sub-command get the error .   

//Our p21 dll architecture seprated into two part for USB/RS232 control.
// -USB  command object -> USB  tranmission object
// -UART command object -> UART tranmission object
//Generally everybody thinks that USB/UART command could be merged into one,
//Unfortunrely it is not implemented in P21 because
// 1.USB/UART still have some different control protocol and flow.
//   Even if we would like to create "protocol" class  ,it may lose some flexibility .
// 2. USB/UART transmission interface should be unique.
//  In P31(KongMing) it is almost impossible because of the unique USB control method.
//  In P21 ,the TI Comdriver.dll implemented as the style but.....
// Above all, I think 

// in    Module level          we return -10000 *BASE
// in Composited command level we return -100 *BASE
// ex: inside the DDRRun function api , 
//     #define COMPOSITED_Load2nd -1
//     #define COMPOSITED_LOADDDR -2
//     PCC_API intDDRRun(){
//        iRet =TILoad2nd()
//          ..............
//        iRet = TILoadDDR() 
//        if(iRet !=1){
//          return MODULE_USB_BASE*10000+ COMPOSITED_LOADDDR*100  + iRet ;
//        }  
//          ............  
//     }

// command table in P21
enum P21DtCommand {
	P21_DT_SD     = 1,
	P21_DT_CAMERA = 2,
	P21_DT_LCD    = 3,
	P21_DT_LCD_COLORTEST       = 5,
	P21_DT_LCD_SpecialPpattern = 8,
	P21_DT_IRDA  =  4,
	P21_DT_AUDIO =  5,
	P21_DT_MODEM =  6,
	P21_DT_BT    =  7,
	P21_DT_FLASH =  8,
	P21_DT_SDRAM =  9,
	P21_DT_CLOCK = 10,
	P21_DT_USB   = 11,
	P21_DT_CHARGER  = 12,
	P21_DT_KEYPAD   = 13,
	P21_DT_VIBRATOR = 14,
	P21_DT_BATTERY  = 15,
	P21_DT_DOWNLOAD = 16,
	P21_DT_RUNOS    = 17,
	P21_DT_WLAN     = 18,
	P21_DT_LED      = 19,
	P21_DT_HARDWARE = 20,
	P21_DT_HARDWARE_ADC = 11,
	P21_DT_VERSION      = 25,
	P21_DT_FACTORYAREA  = 26,
	P21_DT_CHANGEMODE   = 30,
};

// command table in Qcom platform
enum QcDtCommand {
	QC_DT_SD     = 1,
	QC_DT_CAMERA = 2,
	QC_DT_LCD    = 3,
	QC_DT_SENSOR = 4,
	QC_DT_AUDIO  = 5,
	QC_DT_SIM    = 6,
	QC_DT_BT     = 7,
	QC_DT_FLASH  = 8,
	QC_DT_DDR    = 9,
	QC_DT_RTC    = 10,
	QC_DT_USB    = 11,
	QC_DT_CHARGER  = 12,
	QC_DT_KEYPAD   = 13,
	QC_DT_VIBRATOR = 14,
	QC_DT_BATTERY  = 15,
	QC_DT_DOWNLOAD = 16,
	QC_DT_OS       = 17,
	QC_DT_WLAN     = 18,
	QC_DT_LED      = 19,
	QC_DT_ID       = 20,
	QC_DT_RD       = 23,
	QC_DT_VERSION  = 25,
	QC_DT_FA       = 26,
	QC_DT_NV	 = 29,
};

// command table in Nvidia platform
//110722 Gilpin modify for Orange project
enum NvDtCommand {
	NV_DT_SD		= 1,
	NV_DT_CAMERA	= 2,
	NV_DT_DISPLAY	= 3,
	NV_DT_SENSOR	= 4,
	NV_DT_AUDIO		= 5,
	NV_DT_GPS		= 6,
	NV_DT_BT		= 7,
	NV_DT_FMR		= 8,
	NV_DT_SIM		= 9,
	NV_DT_CLK		= 10,
	NV_DT_USB		= 11,
	NV_DT_TOUCH		= 12,
	NV_DT_KEYPAD    = 13,
	NV_DT_VIBRATOR  = 14,
	NV_DT_BATTERY   = 15,
	NV_DT_DOWNLOAD  = 16,
	NV_DT_OS        = 17,
	NV_DT_WLAN      = 18,
	NV_DT_LED       = 19,
	NV_DT_ID        = 20,
	NV_DT_REGULATOR = 21,
	NV_DT_RF		= 22,
	NV_DT_WWAN      = 23,
	NV_DT_TV        = 24,
	NV_DT_VERSION   = 25,
	NV_DT_FA        = 26,
	NV_DT_HOUSEKEEP = 27,
	NV_DT_EFS       = 28,
	NV_DT_NV		= 29,
	NV_DT_DIAG		= 30,
	NV_DT_RTC       = 31,
	NV_DT_DRM       = 32,
	NV_DT_EFUSE     = 33,
};

#endif /* _DT_CMD_TYPE_H_ */
