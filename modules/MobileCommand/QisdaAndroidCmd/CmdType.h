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

static const int DT_PKT_BLOCK_SIZE =   32;
static const int DT_USB_BLOCK_SIZE = 2048;

#define DT_PKT_HEADER_SIZE  4

//erorr list
//we may need to verify
//1.which command is fail
//2.command fail reason

//Level 1:  transmission level (RS232/USB)
//The reason may be comport error,driver problem , or target no response
//Is it necessary to handle all kind of situation?
enum CommandTransErrorCode {
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
	QTH_PORT_MAX = 2100 + 1,
};

//Level 3:Composited Command Level:
//        We may combine several commands as a composited command
//        If one of the command fail ,we should know which one is failure
//        and its failure reason
//#define P21_ERR_CMD_BASE 100
enum CommandErrorCode {
	ERR_TYPE_UART           =      0,
	ERR_TYPE_USB            = -10000,

	ERR_OPENDEVICE          =  -100,
	ERR_CLOSEDEVICE         =  -200,
	ERR_READDEVICE          =  -300,
	ERR_WRITEDEVICE         =  -400,
	ERR_USBTIJUMPADDR       =  -500,
	ERR_GENERALCMD          =  -600,
	ERR_GENERALCMDIN        =  -700,
	ERR_GENERALCMDOUT       =  -800,
	ERR_GENERALCMDINOUT     =  -900,
	ERR_NORMAL_COMMAND      = -1500,
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
};

// command table in Nvidia platform
enum NvDtCommand {
	NV_DT_SD     = 1,
	NV_DT_CAMERA = 2,
	NV_DT_LCD    = 3,
	NV_DT_SENSOR = 4,
	NV_DT_AUDIO  = 5,
	NV_DT_MODEM  = 6,
	NV_DT_BT     = 7,
	NV_DT_FLASH  = 8,
	NV_DT_DDR    = 9,
	NV_DT_RTC    = 10,
	NV_DT_USB    = 11,
	NV_DT_CHARGER  = 12,
	NV_DT_KEYPAD   = 13,
	NV_DT_VIBRATOR = 14,
	NV_DT_BATTERY  = 15,
	NV_DT_DOWNLOAD = 16,
	NV_DT_OS       = 17,
	NV_DT_WLAN     = 18,
	NV_DT_LED      = 19,
	NV_DT_ID       = 20,
	NV_DT_REGULATOR = 21,
	NV_DT_WWAN      = 22,
	NV_DT_RD        = 23,
	NV_DT_TV        = 24,
	NV_DT_VERSION   = 25,
	NV_DT_FA        = 26,
	NV_DT_HOUSEKEEP = 27,
};

enum C7FtCom{
    C7_FT_NULL = -1,
    C7_FT_BATT =  1,
    C7_FT_PWR  =  2,
    C7_FT_MCP  =  3,
    C7_FT_LCM  =  4,
    C7_FT_AU   =  5,
    C7_FT_CAM  =  6,
    C7_FT_KEY  =  7,
    C7_FT_SIM  =  8,
    C7_FT_USB  =  9,
    C7_FT_VIB  = 10,
    C7_FT_ACC  = 11,
    C7_FT_MDM  = 12,
    C7_FT_BT   = 13,
    C7_FT_WLAN = 14,
    C7_FT_GPS  = 15,
    C7_FT_FMR  = 16,
    C7_FT_NFC  = 17,
    C7_FT_SNSR = 18,
    C7_FT_LED  = 19,
    C7_FT_SD   = 20,
    C7_FT_ID   = 21,
    C7_FT_OS   = 90,
    C7_FT_VER  = 91,
    C7_FT_FAC  = 92,
};

enum DetroitFtCommand{
   Detroit_FT_NULL = -1,
   Detroit_FT_BATT =  1,
   Detroit_FT_PWR  =  2,
   Detroit_FT_MCP  =  3,
   Detroit_FT_LCM  =  4,
   Detroit_FT_AU   =  5,
   Detroit_FT_CAM  =  6,
   Detroit_FT_KEY  =  7,
   Detroit_FT_SIM  =  8,
   Detroit_FT_USB  =  9,
   Detroit_FT_VIB  = 10,
   Detroit_FT_ACC  = 11,
   Detroit_FT_MDM  = 12,
   Detroit_FT_BT   = 13,
   Detroit_FT_WLAN = 14,
   Detroit_FT_GPS  = 15,
   Detroit_FT_FMR  = 16,
   Detroit_FT_NFC  = 17,
   Detroit_FT_SNSR = 18,
   Detroit_FT_LED  = 19,
   Detroit_FT_SD   = 20,
   Detroit_FT_ID   = 21,
   Detroit_FT_OS   = 90,
   Detroit_FT_VER  = 91,
   Detroit_FT_FAC  = 92,
};

enum AndriodFtCommand{
    Andriod_FT_NULL		= -1,
    Andriod_FT_BATT		=  1,
    Andriod_FT_PWR		=  2,
    Andriod_FT_MCP		=  3,
    Andriod_FT_LCM		=  4,
    Andriod_FT_AU		=  5,
    Andriod_FT_CAM		=  6,
    Andriod_FT_KEY		=  7,
    Andriod_FT_SIM		=  8,
    Andriod_FT_USB		=  9,
    Andriod_FT_VIB		= 10,
    Andriod_FT_ACC		= 11,
    Andriod_FT_MDM		= 12,
    Andriod_FT_BT		= 13,
    Andriod_FT_WLAN		= 14,
    Andriod_FT_GPS		= 15,
    Andriod_FT_FMR		= 16,
    Andriod_FT_NFC		= 17,
    Andriod_FT_SNSR		= 18,
    Andriod_FT_LED		= 19,
    Andriod_FT_SD		= 20,
    Andriod_FT_ID		= 21,
	Andriod_FT_HDCP	= 22,
    Andriod_FT_OS		= 90,
    Andriod_FT_VER		= 91,
    Andriod_FT_FAC		= 92,
	Andriod_FT_MISC		= 93,
};


enum SapporoFtCommand{
    Sapporo_FT_NULL = -1,
    Sapporo_FT_BATT =  1,
    Sapporo_FT_PWR  =  2,
    Sapporo_FT_MCP  =  3,
    Sapporo_FT_LCM  =  4,
    Sapporo_FT_AU   =  5,
    Sapporo_FT_CAM  =  6,
    Sapporo_FT_KEY  =  7,
    Sapporo_FT_SIM  =  8,
    Sapporo_FT_USB  =  9,
    Sapporo_FT_VIB  = 10,
    Sapporo_FT_ACC  = 11,
    Sapporo_FT_MDM  = 12,
    Sapporo_FT_BT   = 13,
    Sapporo_FT_WLAN = 14,
    Sapporo_FT_GPS  = 15,
    Sapporo_FT_FMR  = 16,
    Sapporo_FT_NFC  = 17,
    Sapporo_FT_SNSR = 18,
    Sapporo_FT_LED  = 19,
    Sapporo_FT_SD   = 20,
    Sapporo_FT_ID   = 21,
	Sapporo_FT_SCOVER  = 22,
    Sapporo_FT_OS   = 90,
    Sapporo_FT_VER  = 91,
    Sapporo_FT_FAC  = 92,
	Sapporo_FT_MISC  = 93,
};

#endif /* _DT_CMD_TYPE_H_ */
