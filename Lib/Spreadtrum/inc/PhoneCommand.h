#ifndef _PHONECOMMAND_H_BY_XPJ_20100928_33F51A91_858A_4779_BCC6_4AD48583B6BD_
#define _PHONECOMMAND_H_BY_XPJ_20100928_33F51A91_858A_4779_BCC6_4AD48583B6BD_


/*******************************************************************************************/
//
//  History:
//  -------------
//  1   V2.0.0000   2010/09/28 Xiaoping.Jing
//                  New release based on 'DiagChan'.
//
/*******************************************************************************************/

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PHONECOMMAND_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PHONECOMMAND_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SP_EXPORT
#undef SP_EXPORT
#endif

#ifdef __cplusplus
    #ifdef PHONECOMMAND_EXPORTS
        #define SP_EXPORT extern "C" __declspec (dllexport)
    #else
		#define SP_EXPORT extern "C" __declspec (dllimport)
    #endif
#else
    #ifdef PHONECOMMAND_EXPORTS
        #define SP_EXPORT __declspec (dllexport)
    #else
        #define SP_EXPORT __declspec (dllimport)
    #endif
#endif

// Include files
#include "ICommChannel.h"

//
#ifdef  SP_API
#undef  SP_API
#endif
#define SP_API  __stdcall 

// HANDLE 
typedef PVOID   SP_HANDLE;
#define INVALID_PHONE_HANDLE                (NULL)


#ifdef  __SS__
#include "DefPhoneAlarmInformation.h"
#define SPERR_BASE                          (DEF_ALARM_PHONE_SPRD_BEGIN_ERROR)
#else
#define SPERR_BASE                          (0)
#endif

////===================================================================================================
//// Add the following form ICommChannel.h by Jack
////===================================================================================================
////only for pc_lint zero warning
////lint ++flb
//
////////////////////////////////////////////////////////////////////////////
////define events
//#define CHE_NO_EVENT			((DWORD)-1)
//// define for socket
//#define SC_CLIENT_CLOSED		(0)
//#define SC_CLIENT_CONNECT		(1)
//
//#define CHE_SC_CLIENT_CLOSED    (0)
//#define CHE_SC_CLIENT_CONNECT	(1)
//#define CHE_SC_SERVER_CLOSED	(2)
//// define for UART
//#define CHE_DEVICE_PLUGOUT      (3)
//#define CHE_DEVICE_PLUGIN       (4)
//
//// define for File
//#define CHE_FILE_READ_OVER      (5) 
//#define CHE_FILE_READ_STOP      (6) 
////////////////////////////////////////////////////////////////////////////
//#define CH_EVNET_DATA_SIZE      (sizeof(DWORD)*2)
//
//#define SC_TYPE_SERVER     (0)
//#define SC_TYPE_CLIENT     (1)
//
//#define INVALID_VALUE      ((DWORD)-1)
//
//
//typedef enum CHANNEL_TYPE
//{
//	CHANNEL_TYPE_COM      = 0,
//	CHANNEL_TYPE_SOCKET   = 1,
//	CHANNEL_TYPE_FILE     = 2,
//	CHANNEL_TYPE_USBMON   = 3         // USB monitor
//};
//
//typedef enum CHANNEL_PROPERTIES {
//	// common property
//	CH_PROP_DEBUGMODE = 0,
//	CH_PROP_LAST_ERROR,
//
//	// for uart
//	CH_PROP_BAUD = 100,
//	CH_PROP_WATCH_DEV_CHANGE,
//
//	// for socket
//	CH_PROP_SOCKET_CLIENTS_COUNT = 200,
//	CH_PROP_SOCKET_CLIENTS_INFO,
//	CH_PROP_SOCKET_CLIENTS_INFO2,
//
//	// for file	
//	CH_PROP_FILE_PACK_SIZE = 300,
//	CH_PROP_FILE_PACK_FREQ 
//};
//
//// channel attributes
//typedef struct _CHANNEL_ATTRIBUTE
//{
//	CHANNEL_TYPE ChannelType;
//
//	union {
//		// ComPort
//		struct {
//			DWORD  dwPortNum;
//			DWORD  dwBaudRate;
//		} Com;
//
//		// Socket
//		struct {			
//			DWORD dwPort;
//			DWORD dwIP;
//			DWORD dwFlag; //[in]: 0, Server; 1, Client; [out]: client ID.		               
//		} Socket;
//
//		// File
//		struct {			
//			DWORD  dwPackSize;
//			DWORD  dwPackFreq;
//			WCHAR  *pFilePath;
//		} File;
//	};
//
//} CHANNEL_ATTRIBUTE, * PCHANNEL_ATTRIBUTE;
//
//typedef const PCHANNEL_ATTRIBUTE PCCHANNEL_ATTRIBUTE;
//
//typedef struct _CHANNEL_INFO
//{
//	DWORD              dwClientCount;
//	PCHANNEL_ATTRIBUTE pClients;
//}CHANNEL_INFO,*CHANNEL_INFO_PTR;
//
////===================================================================================================

/*******************************************************************************************/
//                              
//                                 Error Code
//
/*******************************************************************************************/
typedef enum
{
    SPERR_SUCC     = 0,                     //  0: Operation success
    SPERR_FAIL     = SPERR_BASE + 1,        //  1: Operation failed
    SPERR_UNKNOWN,                          //  2: Unknown error
    SPERR_INVALID_PARAMETER,                //  3: Invalid parameters, such as NULL pointer
    SPERR_SYSTEM_FAIL,                      //  4: System operate error, such as CreateThread API failed 
    SPERR_INVALID_PHONE_HANDLE,             //  5: Invalid handle of phone 
    SPERR_CHANNEL_OPEN_FAIL,                //  6: DIAG. Channel open failed
    SPERR_CHANNEL_NOT_OPEN,                 //  7: DIAG. Channel not open, Invoke SP_BeginPhoneTest first     
    SPERR_OPERATION_TIMEOUT,                //  8: Operation timeout
    SPERR_BUFFER_TOO_SMALL,                 //  9: Input buffer is too small
    SPERR_WRITE_FAIL,                       // 10: Write buffer into port failed
    SPERR_NO_RESPONSE_RECEIVED,             // 11: No response received from port
    SPERR_INVALID_RESPONSE_LEN,             // 12: Invalid length of packet or data received
    SPERR_INVALID_RESPONSE_DATA,            // 13: Invalid data received
    SPERR_INVALID_RESPONSE_CRC,             // 14: Invalid CRC of received packet
    SPERR_RECV_TYPE_IS_DIFFERENT,           // 15: 'type'    of received packet is different from request packet
    SPERR_RECV_SUBTYPE_IS_DIFFERENT,        // 16: 'subtype' of received packet is different from request packet
    SPERR_INVALID_MAGIC_NUMBER,             // 17: Invalid magic number of phase check
    SPERR_REQUEST_OPERATION_FAIL,           // 18: Request operation failed 
    
    SPERR_MAX
}SPERR_ERROR;

#define _SPOK(e)                    (SPERR_SUCC == (e))

/*******************************************************************************************/
//                              
//                                 Macro definition
//
/*******************************************************************************************/

// TimeOut definition
#define TIMEOUT_1S                    1000
#define TIMEOUT_2S                    2000
#define TIMEOUT_3S                    3000          // default timeout 
#define NO_TIMEOUT                    0

// attribute 
typedef enum
{
    SP_ATTR_TIMEOUT         = 1,
    SP_ATTR_LOG             = 2,
    SP_ATTR_ENDIAN          = 3,
    SP_ATTR_SOCKET          = 4,
    SP_ATTR_MAGICNUM        = 5         //  [11/8/2011 Xiaoping.Jing] phase check magic number:
                                        //  1. #define SP09_SPPH_MAGIC_NUMBER    (0X53503039)    // "SP09"
                                        //  2. #define SP05_SPPH_MAGIC_NUMBER    (0X53503035)    // "SP05"
        
} SP_ATTR_ENUM;

//////////////////////////////////////////////////////////////////////////
// Phone Mode 
#define RM_RUNMODE_REQ_COMMAND        0x01
#define RM_COMMAND_T                  0xFE
#define	RM_ENABLE_SLEEP               0x40          // enable arm deep sleep
#define	RM_U0_AS_DIAG                 0x80          // uart0 used as diag port(debug)
typedef enum
{      
    RM_NORMAL_MODE                  = 0x00,

    // GSM 
    RM_CALIBRATION_MODE             = 0x01,         // in calibration mode.
    RM_CALIBR_POST_MODE             = 0x05,         // in calibration post mode
    RM_CALIBR_POST_NO_LCM_MODE      = 0x06,         // in calibration post mode without lcm,app
    
    // TD
    RM_TD_CALITRATION_MODE          = 0x07,         // in td calibration mode.        
    RM_TD_CALIBR_POST_MODE          = 0x08,         // in td calibration post mode  
    RM_TD_CALIBR_POST_NO_LCM_MODE   = 0x09,         // in td calibration post mode without lcm, app 
    
    RM_CALIBR_NV_ACCESS_MODE        = 0x0A          // in fast write nv mode
        
}RM_MODE_ENUM;


// Register access mode 
typedef enum
{
    PC_BYTE_ACCESS	= 0,		
    PC_WORD_ACCESS, 		
    PC_DWORD_ACCESS	
        
} PC_ADDR_ACESS_MODE_E;


//////////////////////////////////////////////////////////////////////////
// Keyboard: virtual key code
#define SCI_VK_0            		('0')
#define SCI_VK_1            		('1')
#define SCI_VK_2            		('2')
#define SCI_VK_3            		('3')
#define SCI_VK_4            		('4')
#define SCI_VK_5            		('5')
#define SCI_VK_6            		('6')
#define SCI_VK_7            		('7')
#define SCI_VK_8           		    ('8')
#define SCI_VK_9            		('9')
#define SCI_VK_STAR        		    ('*')
#define SCI_VK_POUND      	    	('#')

#define SCI_VK_CALL       		    0x01
// If a key is used as both SCI_VK_POWER and SCI_VK_CANCEL, only use SCI_VK_POWER
#define SCI_VK_POWER     		    0x02   
#define SCI_VK_CANCEL    		    0x03    
#define SCI_VK_UP           		0x04
#define SCI_VK_DOWN      		    0x05
#define SCI_VK_LEFT        		    0x06
#define SCI_VK_RIGHT       		    0x07

#define SCI_VK_MENU_SELECT  	    0x08
#define SCI_VK_MENU_CANCEL  	    0x09
#define SCI_VK_SIDE_UP      	    0x0A
#define SCI_VK_SIDE_DOWN    	    0x0B
#define SCI_VK_OK           	    0x0C
#define SCI_VK_WEB          	    0x0D
#define SCI_VK_CAMERA       	    0x0E

//////////////////////////////////////////////////////////////////////////
// IMEI 
#define MAX_IMEI_STR_LENGTH         16
#define MAX_IMEI_NV_LENGTH          8
#define NVID_IMEI1                  0x5
#define NVID_IMEI2                  0x179

//////////////////////////////////////////////////////////////////////////
// BlueTooth
#define MAX_BT_ADDR_NV_LENGTH       6
#define MAX_BT_ADDR_STR_LENGTH      13


/* BT commands */
#define BT_ENTER_DUT_MODE           0
#define BT_LEAVE_DUT_MODE           1
#define BT_SET_BD_ADDR              2
#define BT_GET_BD_ADDR              3
#define BT_START_RADIO_TEST         4
#define BT_STOP_RADIO_TEST          5
#define BT_START_AFC                6
#define BT_SET_DAC_VALUE            7
#define BT_STOP_AFC                 8
#define BT_SAVE_DAC_VALUE           9
#define BT_LOAD_DAC_VALUE          10

typedef struct _tagRADIO_PARAM
{
    unsigned int test_item;
    unsigned int channel;
    unsigned int payload_type;
    unsigned int packet_type;

} RADIO_PARAM;

typedef union
{
    unsigned char   bd_addr[MAX_BT_ADDR_NV_LENGTH];
    RADIO_PARAM     radio_param;
    unsigned int    channel;
    unsigned int    dac;

} MSG_BT_T;

//////////////////////////////////////////////////////////////////////////
// WIFI
#define MAX_WIFI_ADDR_NV_LENGTH         6
#define MAX_WIFI_ADDR_STR_LENGTH        13

typedef enum
{
    WIFI_DR_1MBPS   = 1,    // 1    Mbps
    WIFI_DR_2MBPS   = 2,    // 2    Mbps
    WIFI_DR_5MBPS   = 5,    // 5.5  Mbps
    WIFI_DR_6MBPS   = 6,    // 6    Mbps
    WIFI_DR_7MPBS   = 7,    // 6.5  Mbps
    WIFI_DR_9MBPS   = 9,    // 9    Mbps
    WIFI_DR_11MBPS  = 11,   // 11   Mbps
    WIFI_DR_12MBPS  = 12,   // 12   Mbps
    WIFI_DR_13MBPS  = 13,   // 13   Mbps
    WIFI_DR_18MBPS  = 18,   // 18   Mbps
    WIFI_DR_19MBPS  = 19,   // 19.5 Mbps
    WIFI_DR_24MBPS  = 24,   // 24   Mbps
    WIFI_DR_26MBPS  = 26,   // 26   Mbps
    WIFI_DR_36MBPS  = 36,   // 36   Mbps
    WIFI_DR_39MBPS  = 39,   // 39   Mbps
    WIFI_DR_48MBPS  = 48,   // 48   Mbps
    WIFI_DR_52MBPS  = 52,   // 52   Mbps
    WIFI_DR_54MBPS  = 54,   // 54   Mbps
    WIFI_DR_58MBPS  = 58,   // 58.5 Mbps
    WIFI_DR_65MBPS  = 65,   // 65   Mbps

    WIFI_DATE_RATE_MAX

} WIFI_DATA_RATE_ENUM;

typedef enum
{
    WIFI_MODE_DSSS_BPSK  = 0,	  // 0， DSSS BPSK，   adapt for ratio = 1Mbps
    WIFI_MODE_DSSS_DQPSK,         // 1， DSSS DQPSK，  adapt for ratio = 2Mbps
    WIFI_MODE_DSSS_CCK,           // 2， DSSS CCK，    adapt for ratio = 5.5Mbps/11Mbps
    WIFI_MODE_OFDM_BPSK,          // 3， OFDM BPSK，   adapt for ratio = 6Mbps/9Mbps
    WIFI_MODE_OFDM_DQPSK,         // 4， OFDM_DQPSK，  adapt for ratio = 12Mbps/18Mbps
    WIFI_MODE_16QAM,              // 5， 16QAM，       adapt for ratio = 24Mbps/36Mbps
    WIFI_MODE_64QAM,              // 6， 64QAM，       adapt for ratio = 48Mbps/54Mbps
    
    WIFI_MODE_MAX

}WIFI_MODE_ENUM;                  // RF调制方式


typedef enum
{
    WIFI_LOAD_FACTOR = 0,
    WIFI_SAVE_FACTOR,
    WIFI_SET_FACOTR

} WIFI_FACTOR_ACTION;

typedef struct _tagWIFI_PWR_FACTOR_SET_T
{
    unsigned int   nOperRes;        // 0: Operation pass, Other: Operation failed
    unsigned int   nPwrLevel;       // Power Control Level
    unsigned int   nFactor;         // the WIFI TX factor
    
} WIFI_PWR_FACTOR_SET_T, *LPWIFI_PWR_FACTOR_SET_T; 

//////////////////////////////////////////////////////////////////////////
// CMMB
typedef struct  _tagTV_PARAM_T
{
    unsigned short  nParam1;
    unsigned short  nParam2;
    unsigned short  nParam3;
    unsigned short  nParam4;
    unsigned short  nParam5;
    unsigned short  nParam6;
    unsigned short  nParam7;
    unsigned short  nParam8;
    unsigned short  nParam9;
    unsigned short  nDummy ;

} TV_PARAM_T, *pTV_PARAM_T;

typedef enum 
{
    SC6600V_MODE_NORMAL,
    SC6600V_MODE_DEEPSLEEP,  	    // Deep Sleep模式
    SC6600V_MODE_POWER_OFF,		    // Power Off 模式
    SC6600V_MODE_DISPLAYTEST        // Display Test

} SC6600V_MODE_ENUM;

typedef enum _tagTOOL_TV_MSG_ID_E
{
    TOOL_TV_SET_FREQ = 0,			    // 设置频点
    TOOL_TV_GET_DEMODULATION,		    // 获得解调文件信息
    TOOL_TV_GET_FREQERROR,			    // 获得频率偏差
    TOOL_TV_GET_RSSI,				    // 获得RSSI大小
    TOOL_TV_GET_BLOCKERROR,			    // 获得接收到的总块数，出错的总块数
    TOOL_TV_SET_REGISTER,			    // 写入寄存器值
    TOOL_TV_GET_REGISTER,			    // 读取寄存器值
    TOOL_TV_GET_PLL,           		    // 读取PLL的锁定信息
    TOOL_TV_RESET_MODE,				    // 设置SC6600V模式
    TOOL_TV_GET_MODE,				    // 查询当前SC6600V的模式
    TOOL_TV_BB_SHOW_PICTURE,		    // 控制LCD
    TOOL_TV_START_RAM_ROM_TEST,		    // 测试RAM/ROM
    TOOL_TV_DISPLAY_PICTURE,		    // 显示图片
    TOOL_TV_PERIPHERAL_TEST,		    // Peripheral测试
    TOOL_TV_START_AV_DECODE_TEST,	    // 发起Audio Vedio Decode测试
    TOOL_TV_GET_AV_DECODE_TEST_STATE,   //查询测试状态,确认是否测试完成
    TOOL_TV_GET_AV_DECODE_TEST_RESULT, // Audio Vedio Decode测试结果
    TOOL_TV_START_CMMB_DEMOD,		    // 发起CMMB Demod测试
    TOOL_TV_START_DATA_DISPLAY,		    //开始数据回放
    TOOL_TV_LCD_DISPLAY,			    //LCD显示控制
    TOOL_TV_READ_NV,                    // 读NV参数
    TOOL_TV_WRITE_NV,                   // 写NV参数
    TOOL_TV_GET_SERVICE_INFO,           // 获取频点及频道信息
    TOOL_TV_PLAY_BY_PARAM,              // 根据给定频点及频道信息进行播放
    TOOL_TV_SEARCH_NETWORK,             // 搜索频道
    TOOL_TV_SMD_CHECK,                  // SMD检测
    TOOL_TV_CLOSE_WAITING_WIN,          // 关闭等待窗口(非工具命令)
    TOOL_TV_CMD_INVALID                 // 无效命令
} TOOL_TV_MSG_ID_E;


typedef enum _tagPERIPHERAL_TEST_ENUM
{
    UART0_UART0 = 0,
    RW_I2C,
    RW_SPI,
    RW_GPIOS,
    RW_SIM,
    BUS_MONITOR
} PERIPHERAL_TEST_ENUM;

typedef enum _tagAV_DECODE_ENUM
{
    MP3 = 1,
    H264_AAC,
    AVS_AAC
} AV_DECODE_ENUM;

typedef enum _tagLCD_TEST_ENUM
{
    LCD_TEST_RED,
    LCD_TEST_YELLOW,
    LCD_TEST_BLUE,
    LCD_TEST_GREEN,
    LCD_TEST_VERTICAL,	//竖条
    LCD_TEST_CROSS		//横条
} LCD_TEST_ENUM;

//////////////////////////////////////////////////////////////////////////
// DSP debug
typedef enum
{
    DSPDBG_ACCESS_BYTE  = 1,
    DSPDBG_ACCESS_WORD  = 2,
    DSPDBG_ACCESS_DWORD = 4

} DSPDBG_MEMO_ACCESS_MODE;

typedef enum
{
    NORMAL_NULL_FRAME,
    DSP_DEBUG_NORMAL_TX_FRAME,
    DSP_DEBUG_RACH_FRAME,
    DSP_DEBUG_MONITOR_FRAME,
    DSP_DEBUG_IDLE_FRAME,
    DSP_DEBUG_END
        
} FRAME_TYPE_NULLSTATE_E;


//////////////////////////////////////////////////////////////////////////
// GPS
typedef struct
{
    BYTE    byEngineSn[24];
    DWORD   dwMapVersion;
    BYTE    byActivateCode[16];

} GPS_NV_INFO_T, *LPGPS_NV_INFO_T;

//////////////////////////////////////////////////////////////////////////
// Fast NV access structure 
#define FNAMASK_WRITE_IMEI1             0x01        // BIT0
#define FNAMASK_WRITE_IMEI2             0x02        // BIT1
#define FNAMASK_WRITE_BTADDR            0x04        // BIT2
#define FNAMASK_WRITE_GPSINFO           0x08        // BIT3
#define FNAMASK_WRITE_IMEI3             0x10        // BIT4
#define FNAMASK_WRITE_IMEI4             0x20        // BIT5
#define FNAMASK_WRITE_WIFIADDR          0x40        // BIT6

typedef struct _tagFASTNVACCESS_T 
{
    DWORD           dwOperMask;
    
    unsigned char   szImei1   [MAX_IMEI_STR_LENGTH     ];
    unsigned char   szImei2   [MAX_IMEI_STR_LENGTH     ];
    unsigned char   szImei3   [MAX_IMEI_STR_LENGTH     ];
    unsigned char   szImei4   [MAX_IMEI_STR_LENGTH     ];
    unsigned char   szBTAddr  [MAX_BT_ADDR_STR_LENGTH  ];
    unsigned char   szWIFIAddr[MAX_WIFI_ADDR_STR_LENGTH]; 
    GPS_NV_INFO_T	GpsInfo;

}FASTNVACCESS_T, *LPFASTNVACCESS_T;

//////////////////////////////////////////////////////////////////////////
// Calibration and final test flag (adc_reserved[7])
#define FLAG_PCS_AGC_CALIBRATED     0x00000001      // PCS AGC calibrated pass
#define FLAG_PCS_APC_CALIBRATED     0x00000002      // PCS APC calibrated pass
#define FLAG_DCS_AGC_CALIBRATED     0x00000004      // DCS AGC calibrated pass
#define FLAG_DCS_APC_CALIBRATED     0x00000008      // DCS APC calibrated pass
#define FLAG_GSM_AGC_CALIBRATED     0x00000010      // GSM AGC calibrated pass
#define FLAG_GSM_APC_CALIBRATED     0x00000020      // GSM APC calibrated pass
#define FLAG_850_AGC_CALIBRATED     0x00000040      // 850 AGC calibrated pass
#define FLAG_850_APC_CALIBRATED     0x00000080      // 850 APC calibrated pass
#define FLAG_GSM_AFC_CALIBRATED     0x00000100      // GSM AFC calibrated pass
#define FLAG_ADC_CALIBRATED         0x00000200      // ADC calibrated pass
#define FLAG_PCS_FINAL_TESTED       0x00000400      // PCS final tested pass
#define FLAG_DCS_FINAL_TESTED       0x00000800      // DCS final tested pass
#define FLAG_GSM_FINAL_TESTED       0x00001000      // GSM final tested pass
#define FLAG_850_FINAL_TESTED       0x00002000      // 850 final tested pass
#define FLAG_TD_AFC_CALIBRATED      0x00004000      // TD AFC calibrated pass
#define FLAG_TD_APC_CALIBRATED      0x00008000      // TD APC calibrated pass
#define FLAG_TD_AGC_CALIBRATED      0x00010000      // TD AGC calibrated pass
#define FLAG_TD_FINAL_TESTED        0x00020000      // TD final tested pass
#define FLAG_ANTENNA_TESTED         0x00040000      // Antenna tested pass
#define FLAG_CMMB_TESTED            0x00080000      // CMMB tested pass
#define FLAG_RESERVED1              0x00100000      // Reserved
#define FLAG_RESERVED2              0x00200000      // Reserved
#define FLAG_RESERVED3              0x00400000      // Reserved
#define FLAG_RESERVED4              0x00800000      // Reserved
#define FLAG_RESERVED5              0x01000000      // Reserved
#define FLAG_RESERVED6              0x02000000      // Reserved
#define FLAG_RESERVED7              0x04000000      // Reserved
#define FLAG_MMI_TESTED             0x08000000      // MMI CIT tested pass
#define FLAG_CUSTOM1                0x10000000      // For customer definition
#define FLAG_CUSTOM2                0x20000000      // For customer definition
#define FLAG_CUSTOM3                0x40000000      // For customer definition
#define FLAG_CUSTOM4                0x80000000      // For customer definition



#define UNUSED_PARAM                        0x00

#define NV_GSM_RF_AERO2_AFC_RANGE_NUM       (17)
#define NV_GSM_RF_POWER_FACTOR_NUM          (80)

typedef enum                                 
{                                           
    CALI_CALIBRATION_ALL = 0X0,              
    CALI_ADC,                       //01    'adc'
    CALI_AFC_CENTER,                //02    'aero1_RF_AFC_STRUCT.afc_center'       
    CALI_AFC_HZ_6BITS,              //03    'aero1_RF_AFC_STRUCT.afc_slop_hz_per_6bits'
    CALI_AFC_SLOPE,                 //04    'aero2_RF_AFC_STRUCT.slope_struct.slope_value_arr[17]'
    CALI_AFC_SLOPE_LEN,             //05    'aero2_RF_AFC_STRUCT.slope_length' 
    CALI_AFC_CDAC,                  //06    'aero2_RF_AFC_STRUCT.CDAC'
    CALI_AFC_CAFC,                  //07    'aero2_RF_AFC_STRUCT.CAFC' 
    CALI_RAMP_TABLE,                //08    'RF_ramp_table_T.stageX.RF_ramp_up_table_stageX[20]' 
    CALI_RAMP_TABLE_CONST_UP,       //09    'rf_ramp_param_constant_up[16]'
    CALI_RAMP_TABLE_CONST_DN,       //0A    'rf_ramp_param_constant_down[16]' 
    CALI_RAMP_UP_PARAM_NUM,         //0B    'rf_ramp_up_param_num' 
    CALI_RAMP_DELTA_TIMING,         //0C    'RF_ramp_delta_timing[16]'
    CALI_RAMP_DUARATOIN,            //0D    'RF_ramp_PA_power_on_duration[16]'
    CALI_AFC_RANGE,                 //0E    'aero2_RF_AFC_STRUCT.slope_struct.afc_range_arr[17]' 
    CALI_AFC_SLOPE_RECI,            //0F    'aero2_RF_AFC_STRUCT.slope_struct.slope_value_reciprocal_arr[17]' 
    CALI_TX_COMP_ARFCN_INDEX,       //10    'TX_compensate_boundary_arfcn_index[5]'  
    CALI_RX_COMP_ARFCN_INDEX,       //11    '' 
    CALI_RX_COMP_VALUE,             //12    'RX_compensate_value[75]' 
    CALI_TX_ARFCN,                  //13    
    CALI_TX_PCL,                    //14    'RF_ramppwr_step_factor[80]' 
    CALI_TX_PWR_STEP_FACTOR,        //15     
    CALI_RX_ARFCN,                  //16     
    CALI_AGC_CRTL_WORD,             //17    'agc_ctrl_word[91]'
    CALI_MAX_RF_GAIN_INDEX,         //18     
    CALI_RSSI,                      //19
    CALI_RXLEV,                     //1A    
    CALI_ACTIVE_DEACTIVE,	        //1B    
    CALI_TX_ON_OFF, 		        //1C    
    CALI_RX_ON_OFF,	                //1D    
    CALI_IMEI,                      //1E    
    CALI_CHIP_ID,                   //1F	         
    CALI_SAVE_TO_NV,                //20 
    CALI_RF_SOLUTION,				//21       
    CALI_MODE_READY,			    //22    refered by ref ucom cr108228               
    CALI_IMEI1,                     //23
    //for MCS5-9
    CALI_8PSK_TX_PWR_STEP_FACTOR = 0x24,  //0x24    '8psk_step_factor'
    CALI_8PSK_TX_PWR_TX_GAIN,             //0x25    '8psk_tx_gain_table.8psk_tx_gain[5]'
    CALI_8PSK_RAMP_DELTA_TIMING,          //0x26
    CALI_8PSK_RAMP_DUARATOIN,             //0x27  
    CALI_NOSINGAL_RXTXLOOP = 0x28,        //0x28 (value defined by tools team)
    CALI_RXADC_DCVOLTAGE,                 //0x29
    CALI_MODE_T2G_INIT,                   //0x2A
    CALI_MODE_T2G,                        //0x2B
    CALI_FDT = 0x2D,                      //0x2D

    //====================
    CALI_OSCI_TEMP_COM_IN_DAC = 0x30,
    //    int16  osci_temp_comp_in_dac[ NUM_TEMP_BANDS ];     // In Hz
    
    CALI_TEMPERATURE_GSM_TX_COMPENSATE,
    CALI_TEMPERATURE_EDGE_TX_COMPENSATE,
    CALI_TEMPERATURE_GSM_TXPOWER_COMPENSATE,
    CALI_TEMPERATURE_EDGE_TXPOWER_COMPENSATE,
    //      int16 temperature_gsm_tx_compensate_value[9];
    //  int16 temperature_edge_tx_compensate_value[9];
    // int16 volatge_gsm_txpower_compensate_value[6];
    // int16 volatge_edge_txpower_compensate_value[6];
    CALI_TX_COMPENSATE_BOUNDARY_ARFCN_INDEX,
    //    int16 TX_compensate_boundary_arfcn_index[4];
    CALI_TEMPERATURE_RX_COMPENSATE_ADC,
    CALI_TEMPERATURE_RX_LEVEL_COMPENSATE,
    CALI_TEMPERATURE_TX_COMPENSATE_ADC,
    CALI_TEMPERATURE_TX_RAMPWR_STEP_FACTOR,
    // int16 temperature_RX_compensate_boundary_ADC[4];
    // int16 temperature_rx_level_compensate_value[5];
    // int16 temperature_TX_compensate_boundary_ADC[4];
    // int16 temperature_TX_rampwr_step_factor[5];
    CALI_RF_EDGE_DELTA_TIMING,
    CALI_RF_EDGE_POWER_ON_DURATION,
    //byhwt-5  change for edge
    // int16 rf_edge_delta_timing;    //int16 ramp_switch_level;  
    //int16 rf_edge_power_on_duration;//uint16 afc_alg_bias;


    CALI_END

} L1_CALIBRATION_PARAM_E; 


typedef enum
{
    DSP_TX_TYPE_ALL_ZERO,
    DSP_TX_TYPE_ALL_SET,
    DSP_TX_TYPE_RANDOM,
    DSP_TX_TYPE_RACH,
    DSP_TX_TYPE_EDGE

} DSP_TX_TYPE_E;


//on tool side:MCS1-9 use value 0-8,arm layer1 should trans this value to TX_CODE_MCS1-9
enum
{
    TOOL_TX_CODE_MCS1			= 0x00,
    TOOL_TX_CODE_MCS2			= 0x01,
    TOOL_TX_CODE_MCS3			= 0x02,
    TOOL_TX_CODE_MCS4			= 0x03,
    TOOL_TX_CODE_MCS5			= 0x04,
    TOOL_TX_CODE_MCS6			= 0x05,
    TOOL_TX_CODE_MCS7			= 0x06,
    TOOL_TX_CODE_MCS8			= 0x07,
    TOOL_TX_CODE_MCS9			= 0x08
};

typedef struct _tagGSM_CELL_INFO
{
    int     nMcc;
    int     nMnc;
    int     nLac;
    int     nCellId;
    int     nBsic;
    int     nArfcn;
    int     nRxlev;
    int     nRxqual;

} GSM_CELL_INFO, *LPGSM_CELL_INFO;


/*******************************************************************************************/
//                              
//                               Calibration Structure
//
/*******************************************************************************************/

// Band definition
typedef enum	
{
    PC_CALI_INVALID_BAND    = -1,                   // Invalid Band
    PC_CALI_EGSM900 		=  0,                   // EGSM900
    PC_CALI_DCS1800		    =  1,                   // DCS1800
    PC_CALI_PCS1900		    =  3,                   // PCS1900
    PC_CALI_GSM850		    =  4,                   // GSM850M
    PC_CALI_TD				= 15                    // TD
    
} PC_CALI_MS_BAND_E;

// ADC
#define MAX_ADC_CHANNEL_NUM         8
typedef enum
{
    SUCCESS_COMPATIBLE = 0x00,
    INVALID_ADC_SOURCE = 0x01,
    NEED_CALIBRATION   = 0x02,
    DNOT_CALIBRATION   = 0x0F
        
} ADC_CALIBRATION_FLAG;

// ADC NV index
enum
{
    INDEX_ADC0          = 0,            //  0 : adc0
    INDEX_ADC1,                         //  1 : adc1  
    INDEX_BATTERY0,                     //  2 : battery0        V-BATT 4.2V
    INDEX_BATTERY1,                     //  3 : battery1        V-BATT 3.6V    
    INDEX_RESERVED0,                    //  4 : adc_reserved0   
    INDEX_RESERVED1,                    //  5 : adc_reserved1
    INDEX_RESERVED2,                    //  6 : adc_reserved2
    INDEX_RESERVED3,                    //  7 : adc_reserved3
    INDEX_RESERVED4,                    //  8 : adc_reserved4
    INDEX_RESERVED5,                    //  9 : adc_reserved5
    INDEX_RESERVED6,                    // 10 : adc_reserved6
    INDEX_RESERVED7                     // 11 : adc_reserved7   calibration flag
};

typedef struct _tagPC_ADC_T
{
    unsigned char   channel;
    unsigned short  adc;
    unsigned char   califlag;
    unsigned char   nvoffset;

} PC_ADC_T, *LPPC_ADC_T;

// GSM TX
typedef struct _tagPC_TX_PARAM_T 
{
    DSP_TX_TYPE_E       data_type;
    unsigned short      coding_scheme;
    unsigned short      training_sequence;
    unsigned short      puncture_type;  

} PC_TX_PARAM_T, *LPPC_TX_PARAM_T;

typedef struct _tagPC_PWR_FACTOR_PARAM_T
{
    PC_CALI_MS_BAND_E   eBand;      // Band
    unsigned short      uArfcn;     // ARFCN
    unsigned short      uPCL;       // Power Level
    unsigned short      uFactor;    // Power Factor [0 - 32767]

} PC_PWR_FACTOR_PARAM_T, *LPPC_PWR_FACTOR_PARAM_T;

// AFC
typedef struct _tagPC_AFC_REQ_T
{
    PC_CALI_MS_BAND_E   eBand;      // Band
    unsigned short      uPCL;       // Power Control Level, namely , PCL
    unsigned short      uArfcn;     // Traffic Channel 
    bool                bTCXO;      // TCXO or DCXO
    unsigned short      uDacVal;    // CDAC
    unsigned short      uAfcVal;    // CAFC

} PC_AFC_REQ_T, *LPPC_AFC_REQ_T;

// APC
typedef struct _tagPC_APC_REQ_T
{
    PC_PWR_FACTOR_PARAM_T pwrFactor;

} PC_APC_REQ_T, *LPPC_APC_REQ_T;

// AGC
typedef struct _tagPC_AGC_REQ_T
{
    PC_CALI_MS_BAND_E   eBand;      // Band
    unsigned short      uArfcn;     // arfcn 
    unsigned short      uGainIndex; // gain index
    unsigned short      uGainValue; // gain value

} PC_AGC_REQ_T, *LPPC_AGC_REQ_T;

#define MAX_AGC_RESULT_NUM   40
typedef struct _tagPC_AGC_VALUE_CNF_T
{
    int                 bValid;
    unsigned short      nRxlev;
    unsigned short      nRssi; 	

    // ------------ Add for TD ------------ [[[ 
    int                 nRssiNum;
    unsigned short      nRssiState[MAX_AGC_RESULT_NUM];
    unsigned short      nRssiVal[MAX_AGC_RESULT_NUM];
    // ------------ End for TD ------------ ]]]
} PC_AGC_VALUE_CNF_T, *LPPC_AGC_VALUE_CNF_T;


// GSM NonSignal Final Test Command
typedef enum
{
    MODE_RXTXLOOP_A,
    MODE_RXTXLOOP_B,
    MODE_RXTXLOOP_C

} L1_NONSIGNAL_MODE_E;


typedef enum
{
    CMD_RXTXLOOP_NULL,              //= 0x00,  used  as a state, real command should not be this value
    CMD_RXTXLOOP_INITIATE,
    CMD_RXTXLOOP_STARTLOOP,
    CMD_RXTXLOOP_STOPLOOP,
    CMD_RXTXLOOP_CHANGE_PCL,        // change power level(must used in tch loopbacking state)
    CMD_RXTXLOOP_CHANGE_TCHARFCN,
    CMD_RXTXLOOP_GET_RXLEV,
    CMD_RXTXLOOP_UPDATE_CALINV,
    CMD_RXTXLOOP_MAX

} L1_RXTXLOOP_COMMAND_E;

// result enum of rxtx-loop state(sub action refer to sub state ), to notify diag command process result.
typedef enum
{
    RET_RXTXLOOP_OK,        //
    RET_RXTXLOOP_WAKEDSP_FAIL,
    RET_RXTXLOOP_MEASUREMENT_FAIL,
    RET_RXTXLOOP_SCHSEARCH_FAIL,
    RET_RXTXLOOP_SCHREAD_FAIL,
    RET_RXTXLOOP_CAMP_FAIL,
    RET_RXTXLOOP_STARTLOOP_FAIL,
    RET_RXTXLOOP_CHANGEPCL_FAIL,
    RET_RXTXLOOP_INIT_FAIL,
    RET_RXTXLOOP_CHANGEARFCN_FAIL,
    RET_RXTXLOOP_EXCEPTION,    
    RET_RXTXLOOP_GETRXLEV_FAIL,
    RET_RXTXLOOP_MAX

} L1_RXTXLOOP_RESULT_E;  

typedef struct _tagPC_NONSIGNAL_REQ_T
{
    L1_NONSIGNAL_MODE_E     eMode;
    L1_RXTXLOOP_COMMAND_E   eCmd;    
    PC_CALI_MS_BAND_E       eBand;  
    unsigned short          nArfcn;      
    unsigned short          nTrafficTn;
    unsigned short          nTsc;
    unsigned short          nPcl;

} PC_NONSIGNAL_REQ_T, *LPPC_NONSIGNAL_REQ_T; 


// struct for l1 to send  confirm to diag(pc tool)
typedef struct _tagPC_NONSIGNAL_CNF_T
{ 
    unsigned short  nResult;    // L1_RXTXLOOP_RESULT_E
    unsigned short  nRxLevel;      
    unsigned short  nRxQual;
    unsigned short  nDummy0;         

} PC_NONSIGNAL_CNF_T, *LPPC_NONSIGNAL_CNF_T;


//////////////////////////////////////////////////////////////////////////
// TD
#define TD_MAX_DATA_SIZE        0xC00   
typedef enum _TD_NV_TYPE_E	   
{
    TD_NV_TYPE_AFC,                 // AFC param, len = 1
    TD_NV_TYPE_AGC_WORD,            // AGC WORD param, len = 142
    TD_NV_TYPE_AGC_CTRL_WORD,       // AGC CTRL WORD param, len = 142
    TD_NV_TYPE_PWR_STEP_FACTOR,     // APC param, len = 90
    TD_NV_TYPE_AGC_COMPENSATION,    // AGC compensation table
    TD_NV_TYPE_APC_COMPENSATION,    // APC compensation table
    TD_NV_TYPE_COMPENSATION_PARA,   // change AGC or APC compensation para seperately
    TD_NV_TYPE_ADC_PARA

} TD_NV_TYPE_E;

typedef struct _tagPC_TD_CAL_PARAM_T
{   
    TD_NV_TYPE_E    eNvType;                    // data type
    unsigned short  nDataNum;                   // data len	
    unsigned short  nData[TD_MAX_DATA_SIZE];    // data

} PC_TD_CAL_PARAM_T, *LPPC_TD_CAL_PARAM_T;	  


typedef struct _tagPC_TD_TX_SET_T
{
    PC_CALI_MS_BAND_E	eBand;
    unsigned short		nPwrLevel;      // Power Control Level
    unsigned short		nFactor;        // the apc factor
    unsigned int		nFreq;          // the index 	
    unsigned short		nMode;  	
    unsigned short		nTxNumber;	    // 
    unsigned short 		nFactorStep;    // 
    unsigned short 		nSlotCount;
    unsigned short		nTrigLevel;

} PC_TD_TX_SET_T, *LPPC_TD_TX_SET_T;   


#define MAX_FACTOR_NUMBER		8
typedef struct _tagFACTOR_TX_T
{
    unsigned short 	nFactor;	
    unsigned short 	nTxNumber;	        // 表示连续发送的点数

} FACTOR_TX_T;

typedef struct _tagPC_TD_TX_SWEEP_T
{
    DWORD			dwFreq;				// directly use freq
    unsigned short 	nFactorStep; 		// 表示发送功率的步径
    unsigned short 	nTriggerLevel; 
    unsigned short	nFactorNumber;
    FACTOR_TX_T 	FactorArr[MAX_FACTOR_NUMBER]; // actual max number is 3

} PC_TD_TX_SWEEP_T, *LPPC_TD_TX_SWEEP_T;

// TD Nonsignal Command
typedef enum
{
    CMD_RXTXLOOP_TDNULL,
    CMD_RXTXLOOP_UPDATE_TDRF_PARAM_DSP,
    CMD_RXTXLOOP_TDINITIATE,
    CMD_RXTXLOOP_TDSTARTLOOP,
    CMD_RXTXLOOP_TDSTOPLOOP,
    CMD_RXTXLOOP_TDCHANGE_ARFCN,
    CMD_RXTXLOOP_TDMODE_CHANGE,
    CMD_RXTXLOOP_TDMAX

} TD_NONSIGNAL_COMMAND_E;

typedef enum
{
    TDCC1_1,
    TDCC2_1,
    TDCC2_2,
    TDCC4_1,
    TDCC4_2,
    TDCC4_3,
    TDCC4_4,
    TDCC8_1,
    TDCC8_2,
    TDCC8_3,
    TDCC8_4,
    TDCC8_5,
    TDCC8_6,
    TDCC8_7,
    TDCC8_8,
    TDCC16_1,
    TDCC16_2,
    TDCC16_3,
    TDCC16_4,
    TDCC16_5,
    TDCC16_6,
    TDCC16_7,
    TDCC16_8,
    TDCC16_9,
    TDCC16_10,
    TDCC16_11,
    TDCC16_12,
    TDCC16_13,
    TDCC16_14,
    TDCC16_15,
    TDCC16_16,
    TDCHANN_CODE_MAX

} TD_CHAN_CODE_E;


typedef struct _tagPC_TD_NONSIGNAL_REQ_T
{
    TD_CHAN_CODE_E	ul_chan_code[16];
    TD_CHAN_CODE_E	dl_chan_code[16];
    unsigned short  arfcn;
    unsigned short  cell_param_id;   
    unsigned short  ul_slot ;
    unsigned short  ul_midamble_alloc_mode;
    unsigned short  ul_midamble;
    unsigned short  ul_midamble_shift ;
    unsigned short  ul_kcell ;
    unsigned short  dl_slot ;
    unsigned short  dl_midamble_alloc_mode;
    unsigned short  dl_midamble;
    unsigned short  dl_midamble_shift ;
    unsigned short  dl_kcell ;
    char	   	    prx_up_dpch_des;
    unsigned char   pccpch_tx_power;
    unsigned char   ul_chan_code_num;
    unsigned char   dl_chan_code_num;
    unsigned short  reserved[7];

} PC_TD_NONSIGNAL_REQ_T,*LPPC_TD_NONSIGNAL_REQ_T;


// TD Fast Device Tune (FDT)
#define MAX_CHANNEL_NUM  13
#define  MAX_FDT_RX_REPEAT_NUM 5
#define MAX_FDT_AGC_RESULT_NUM  ((MAX_CHANNEL_NUM) * (MAX_FDT_RX_REPEAT_NUM) * (10))  // from zhongping
typedef enum  _SYN_FLAG_E
{
	SYN_TS0 =1,     //仅ts0同步
	SYN_DWPTS,      //仅dwpts同步
	SYN_TS0_DWPTS,  //ts0和dwpts均需要同步
}SYN_FLAG_E;

typedef struct _PC_ARR_TX_T
{
	unsigned short  nStartFactor;	            //起始控制字
	BYTE  	nFram;	                            //此频点下APC总共需要发射的控制字数目
	BYTE  	cal_apc_step;	                    //控制字步进
}PC_ARR_TX_T,*pPC_ARR_TX_T;

#define MAX_MODE_NUMBER 4
typedef struct _ARFCN_T
{ 
	unsigned short  	nArfcn;	            //频点
	unsigned short	    nApcArrNum;         //增益模式个数
	PC_ARR_TX_T         ApcTxArr[MAX_MODE_NUMBER]; 
}ARFCN_T,*pARFCN_T;


typedef struct _PC_FDT_TX_T
{
	unsigned int nChanelNum;          //校准频点总数目,不超过  MAX_CHANNEL_NUM        
	ARFCN_T    Arfcn[MAX_CHANNEL_NUM];	
}PC_FDT_TX_T,*pPC_FDT_TX_T;

typedef struct _PC_FDT_RX_T
{
	unsigned short              nStartGain;     //起始接受Gain值
	unsigned short              nStep;          //步进
	unsigned short	            nRxGainNum;     //此频点下需要接收的个数
	unsigned short              nRxRepeat;      //每个Gain值重复接收次数
}PC_FDT_RX_T,*pPC_FDT_RX_T;

typedef struct _CALI_FDT_AGC_RESULT_T
{
    unsigned short   nArfcn;
    short     nRssi;
}CALI_FDT_AGC_RESULT_T,*pCALI_FDT_AGC_RESULT_T;

typedef struct _PC_TD_FDT_TXRX
{
	SYN_FLAG_E          eSysn;
	unsigned short      nafc_value;         //afc频偏
	unsigned short      nafc_slop;          //afc斜率
	unsigned short      nTrigNum;           //trig帧数目
	unsigned short      nTxRxFlag;  
	PC_FDT_TX_T         txFDTpara;          
	PC_FDT_RX_T         rxFDTpara;    
}PC_TD_FDT_TXRX,*pPC_TD_FDT_TXRX;

typedef struct _tagTDFDT_AGC_VALUE_CNF_T
{
    unsigned                 nReultNum;
    CALI_FDT_AGC_RESULT_T  cali_FDT_AGC_result_arr[MAX_FDT_AGC_RESULT_NUM];
    // ------------ End for TD ------------ ]]]
} TDFDT_AGC_VALUE_CNF_T, *LPTDFDT_AGC_VALUE_CNF_T;

//
typedef enum
{
    CUSTOMER_DISK_FORMAT  = 0,
    CUSTOMER_FACTORY_RESET,
    CUSTOMER_TCARD_CLEAR,

} CUSTOMER_PHONE_STATE_OPER;

//////////////////////////////////////////////////////////////////////////
// GSM Fast Device Tune (FDT)
#define MAX_USED_SLOT_COUNT		4
#define AGC_NSYNC_REP_COUNT    21
#define AGC_LSYNC_REP_COUNT    15
#define APC_DEF_DIAG_PCL       10

typedef enum
{
    FDT_SET_TX_PARAM        = 0,
    FDT_SET_RX_PARAM        = 1,
    FDT_START_TX            = 3,
    FDT_START_RX            = 4,
    FDT_RECV_RX             = 5,
    FASTCALI_SET_TX__8PSK   = 6,
    FASTCALI_START_TX_8PSK  = 7,
    FDT_RECV_TX_PARAM_CNF   = 8,
    FASTCALI_INVALID_CMD
} FDT_CMD;

typedef struct
{
    PC_CALI_MS_BAND_E   band;                               // Band 
    unsigned short      arfcn;                              // ARFCN
    BYTE                slot_mask;                          // (BIT MAP) which slot of 0-7 is used, (Slot < MAX_USED_SLOT_COUNT)
    unsigned short      factor[MAX_USED_SLOT_COUNT];        // Factor table of used slots
//  unsigned short      pcl[MAX_USED_SLOT_COUNT];           // PCL, No used
    unsigned short      factor_index[MAX_USED_SLOT_COUNT];  // Index of factor table [0-79] 
    double              dExpPwr[8];

} FDT_TX_FRAME;

typedef enum
{
    FCB  = 0,
    FSB,
    PRBS,
    DUMMY

} FDT_SLOT_TYPE;

typedef struct
{
    PC_CALI_MS_BAND_E   band;                               // Band 
    unsigned short      arfcn;                              // ARFCN
    BYTE                slot_mask;                          // (BIT MAP) which slot of 0-7 is used, (Slot < MAX_USED_SLOT_COUNT)
    unsigned short      gain_value[MAX_USED_SLOT_COUNT];
    unsigned short      gain_index[MAX_USED_SLOT_COUNT];
    FDT_SLOT_TYPE       slot_type[MAX_USED_SLOT_COUNT];
    
} FDT_RX_FRAME;

typedef enum
{
    FDT_SUCC = 0,
    FDT_FAIL,
    FDT_PARAM_SET_ERROR,
    FDT_NORXFRM_ERROR

} FDT_STATE_E;	

#define MAX_FDT_DATA_SIZE		350
typedef struct
{
    FDT_STATE_E     state;
    unsigned int    data_num;
    int             data[MAX_FDT_DATA_SIZE];

} FDT_RES;

typedef struct
{
    FDT_CMD         cmd;          // FDT command
	unsigned int    nParam1;      // Reserved :(When set rx) Repeat Count of 'Dummy' Frame on normal Sync_cycle (Default value = 21)
	unsigned int    nParam2;      // Reserved :(When set rx) Sync_timeout 
	unsigned int    nParam3;      // Reserved :(When set rx) Repeat Count  of 'Dummy' Frame on last Sync_cycle (Default value = 15)
	unsigned int    nParam4;      // Reserved 
    unsigned int    frame_count;  // Frame number
    void           *pFrames;      // Frame parameters

} FDT_REQ;

//***********************/
/*
/*   Following for LTE
//***********************/
typedef enum _LTE_BAND_E
{
	BAND38,
	BAND40,
}LTE_BAND_E;

typedef enum _LTE_BW_E
{
	BW_1_4,
	BW_3,
	BW_5,
	BW_10,
	BW_15,
	BW_20,
}LTE_BW_E;

typedef enum _LTE_REF_CLK_E
{
	RefClk_192,
	RefClk_26,
	RefClk_384,
}LTE_REF_CLK_E;

typedef struct _TD_LTE_BAND_INFO_T
{
    LTE_BAND_E		eBand;   // Power Control Level, namely , PCL
    LTE_BW_E 		eBW;     // 
    LTE_REF_CLK_E 	eRefClk;     // 
	UINT            nArfcn;      // Traffic Channel 
}TD_LTE_BAND_INFO_T;

typedef enum _LTE_LAN_E
{
    LTE_LNA_Mode_1,
	LTE_LNA_Mode_2,
	LTE_LNA_Mode_3,
	LTE_LNA_Mode_4,
}LTE_LAN_E;

typedef struct _PC_LTE_AFC_PARAM
{   
	TD_LTE_BAND_INFO_T  BandInfo;
	unsigned short              dac_val;
	unsigned short              tx_gain;
}PC_LTE_AFC_PARAM, *pPC_LTE_AFC_PARAM;

typedef struct _PC_LTE_APC_PARAM
{
    TD_LTE_BAND_INFO_T  BandInfo;
	unsigned short	    nArryNumber;    //mode number
    unsigned short 	    nFactorStep;    //word step
    FACTOR_TX_T 	    FactorArr[3];   //3种增益模式 
}PC_LTE_APC_PARAM, *pPC_LTE_APC_PARAM;

typedef struct _PC_LTE_AGC_PARAM
{
	TD_LTE_BAND_INFO_T  BandInfo;
	LTE_LAN_E           Lan_mode;
    unsigned short		Agc_index[2];   //	
    unsigned short      Reserverd[2];
}PC_LTE_AGC_PARAM, *pPC_LTE_AGC_PARAM;

typedef struct _PC_LTE_AGC_RET
{
    unsigned int  rssi_value[4];
}PC_LTE_AGC_RET, *pPC_LTE_AGC_RET;

/**-------------------------------------------------------------------------------------*
 **                         PC <-> ARM NV parameter interface  							*
 **-------------------------------------------------------------------------------------*/
typedef enum _LTE_NV_TYPE_E	   
{
    LTE_NV_TYPE_ANTENNA_SWITCH,
    LTE_NV_TYPE_DEFAULT_REGISTER,
    LTE_NV_TYPE_FAST_PLLLOCK,
    LTE_NV_TYPE_AGC_CONTROL_WORD,
    LTE_NV_TYPE_AGC_GAIN_MAIN,
    LTE_NV_TYPE_AGC_GAIN_DIV,
    LTE_NV_TYPE_AFC_CONTROL,
    LTE_NV_TYPE_APC_TX_FIR_POWER,
    LTE_NV_TYPE_APC_CONTROL_WORD_MAIN,
    LTE_NV_TYPE_APC_CONTROL_WORD_DIV,
} LTE_NV_TYPE_E;

typedef enum _LTE_NV_ID_E
{
    LTE_NV_ANTENNA_SWITCH_ID = 2500,
    LTE_NV_DEFAULT_REGISTER_ID,
    LTE_NV_FAST_PLLLOCK_ID,
    LTE_NV_AGC_CONTROL_WORD_ID,
    LTE_NV_AGC_GAIN_MAIN_ID,
    LTE_NV_AGC_GAIN_DIV_ID,
    LTE_NV_AFC_CONTROL_ID,
    LTE_NV_APC_TX_FIR_POWER_ID,
    LTE_NV_APC_CONTROL_WORD_MAIN_ID,
    LTE_NV_APC_CONTROL_WORD_DIV_ID,
}LTE_NV_ID_E;

typedef struct _LTE_NV_DATA_WRITE_T
{   
    LTE_NV_TYPE_E   eNvType;        //data type
    unsigned short  nDataSize;      //data len	
    unsigned short  nData[3000];    //buffer
} LTE_NV_CALDATA_WRITE_T,*LPPC_LTE_CAL_PARAM_T;	

typedef struct _LTE_NV_DATA_READ_T
{
    LTE_NV_TYPE_E   eNvType;        //data type
    unsigned short  nDataSize;      //data len	  
} LTE_NV_CALDATA_READ_T;

typedef enum  _DEVICE_AUTOTEST_ID_E
{
    DEVICE_AUTOTEST_KEYPAD = 1,     
		DEVICE_AUTOTEST_LCD_PARALLEL = 2,  
		DEVICE_AUTOTEST_LCD_SPI = 3,  
		DEVICE_AUTOTEST_CAMERA_IIC = 4,  
		DEVICE_AUTOTEST_CAMERA_PARALLEL = 5,  
		DEVICE_AUTOTEST_CAMERA_SPI = 6,  
		DEVICE_AUTOTEST_GPIO =7,  //and TP test     
		DEVICE_AUTOTEST_TF = 8, 
		DEVICE_AUTOTEST_SIM = 9,
		DEVICE_AUTOTEST_MIC = 10,
		DEVICE_AUTOTEST_SPEAK = 11, //speak&&receiver&&earphone
		DEVICE_AUTOTEST_MISC = 12,    //lcd backlight,vibrator,keypadbacklight   
		DEVICE_AUTOTEST_FM = 13,
		DEVICE_AUTOTEST_ATV = 14, 
		DEVICE_AUTOTEST_BT = 15,
		DEVICE_AUTOTEST_WIFI = 16,     
		
		DEVICE_AUTOTEST_IIC_DEV= 17, //speak&&receiver&&earphone
		
		DEVICE_AUTOTEST_CHARGE= 18,
		
		DEVICE_AUTOTEST_RESERVE_1= 19,
		DEVICE_AUTOTEST_RESERVE_2= 20,
		DEVICE_AUTOTEST_RESERVE_3= 21,
		DEVICE_AUTOTEST_RESERVE_4= 22,
		DEVICE_AUTOTEST_RESERVE_5= 23,
		DEVICE_AUTOTEST_RESERVE_6= 24,
		DEVICE_AUTOTEST_RESERVE_7= 25,
		DEVICE_AUTOTEST_RESERVE_8= 26,
		DEVICE_AUTOTEST_RESERVE_9= 27,
		DEVICE_AUTOTEST_MAX_F
}DEVICE_AUTOTEST_ID_E;
/*******************************************************************************************/
//                              
//                                 API definition
//
/*******************************************************************************************/

#ifndef __SS__

SP_EXPORT SP_HANDLE SP_API SP_CreatePhone   (void *pLogUtil);
SP_EXPORT void      SP_API SP_ReleasePhone  (SP_HANDLE hDiagPhone);

// Property
SP_EXPORT int       SP_API SP_SetProperty   (SP_HANDLE hDiagPhone, long lProperty, long lFlag, const void *lpValue);
SP_EXPORT int       SP_API SP_GetProperty   (SP_HANDLE hDiagPhone, long lProperty, long lFlag, void *lpValue);

// Open / Close Port
SP_EXPORT int       SP_API SP_BeginPhoneTest(SP_HANDLE hDiagPhone, PCCHANNEL_ATTRIBUTE pOpenArgument);
SP_EXPORT int       SP_API SP_EndPhoneTest  (SP_HANDLE hDiagPhone);
SP_EXPORT int       SP_API SP_GetUsbPort    (SP_HANDLE hDiagPhone);

// Mode Operation
SP_EXPORT int       SP_API SP_EnterModeProcess  (SP_HANDLE     hDiagPhone, 
                                                 bool          bUsbDevice, 
                                                 int           nExpPort, 
                                                 RM_MODE_ENUM  ePhoneMode, 
                                                 HANDLE        hMonitorEvent, 
                                                 unsigned long ulTimeOut);
SP_EXPORT int       SP_API SP_ResetModeProcess  (SP_HANDLE     hDiagPhone, 
                                                 bool          bUsbDevice, 
                                                 int           nExpPort, 
                                                 RM_MODE_ENUM  ePhoneMode, 
                                                 HANDLE        hMonitorEvent, 
                                                 unsigned long ulTimeOut);
SP_EXPORT int       SP_API SP_StopModeProcess   (SP_HANDLE hDiagPhone);
SP_EXPORT int       SP_API SP_RestartPhone      (SP_HANDLE hDiagPhone, RM_MODE_ENUM ePhoneMode);
SP_EXPORT int       SP_API SP_EnterDeepSleepMode(SP_HANDLE hDiagPhone);
SP_EXPORT int       SP_API SP_SetPhoneStartMode (SP_HANDLE hDiagPhone, RM_MODE_ENUM ePhoneMode);
//
SP_EXPORT int       SP_API SP_PowerOff          (SP_HANDLE hDiagPhone);

// Write / Read Operation
SP_EXPORT int       SP_API SP_Write(SP_HANDLE hDiagPhone, const void *lpValue, unsigned long nbrOfBytesToWrite);
SP_EXPORT int       SP_API SP_Read (SP_HANDLE hDiagPhone, void       *lpValue, unsigned long nbrOfBytesToRead, unsigned long  ulTimeOut);

// Phone Information
SP_EXPORT int       SP_API SP_GetMsVersion  (SP_HANDLE hDiagPhone, void *lpBuff, unsigned long ulBuffLen);
SP_EXPORT int       SP_API SP_GetMsVoltage  (SP_HANDLE hDiagPhone, unsigned long *pVoltage);
SP_EXPORT int       SP_API SP_EnableArmLog  (SP_HANDLE hDiagPhone, bool bEnable);
SP_EXPORT int       SP_API SP_EnableDspLog  (SP_HANDLE hDiagPhone, bool bEnable);
SP_EXPORT int       SP_API SP_PressKeyboard (SP_HANDLE hDiagPhone, unsigned long ulKeyCode);
SP_EXPORT int       SP_API SP_VibrateOn     (SP_HANDLE hDiagPhone, bool bOn);
SP_EXPORT int       SP_API SP_LedOn         (SP_HANDLE hDiagPhone, bool bOn);
SP_EXPORT int       SP_API SP_GetMsADC      (SP_HANDLE hDiagPhone, LPPC_ADC_T padc);


// R/W NV 
SP_EXPORT int       SP_API SP_ReadNV (SP_HANDLE hDiagPhone, 
                                      unsigned short uNvID, 
                                      void *lpData, 
                                      unsigned long ulDataLen, 
                                      unsigned long *pulDataLen);
SP_EXPORT int       SP_API SP_WriteNV(SP_HANDLE hDiagPhone, 
                                      unsigned short uNvID, 
                                      const void *lpData, 
                                      unsigned long ulDataLen);

// Set USB Auto Mode
SP_EXPORT int       SP_API SP_SetUsbAutoMode(SP_HANDLE hDiagPhone, bool bUsbAuto);

// R/W product info. flash
SP_EXPORT int       SP_API SP_WriteProductFlash   (SP_HANDLE hDiagPhone, 
                                                   const void *lpDataBuff, 
                                                   unsigned long nbrOfBytesToWrite, 
                                                   unsigned long ulTimeOut);
SP_EXPORT int       SP_API SP_ReadProductFlash    (SP_HANDLE hDiagPhone, 
                                                   void *lpDataBuff, 
                                                   unsigned long nbrOfBytesToRead, 
                                                   unsigned long *pnbrOfBytesRead, 
                                                   unsigned long ulTimeOut);

SP_EXPORT int       SP_API SP_NvWriteDirect       (
                                                   SP_HANDLE hDiagPhone, 
                                                   const LPFASTNVACCESS_T pNvInfo
                                                  );
SP_EXPORT int       SP_API SP_FastWriteProductInfo(SP_HANDLE hDiagPhone, 
                                                   const void *lpData, 
                                                   unsigned long uLength);
SP_EXPORT int       SP_API SP_FastReadProductInfo (SP_HANDLE hDiagPhone, 
                                                   void *lpData, 
                                                   unsigned long uLength);

// Fast NV access
SP_EXPORT int       SP_API SP_FastWriteNvInfo(SP_HANDLE hDiagPhone, const LPFASTNVACCESS_T pNvInfo);
SP_EXPORT int       SP_API SP_FastReadNvInfo (SP_HANDLE hDiagPhone, LPFASTNVACCESS_T pNvInfo);

SP_EXPORT int       SP_API SP_ReadImeiCount  (SP_HANDLE hDiagPhone, int *pnCount);
SP_EXPORT int       SP_API SP_FastReadVersion(SP_HANDLE hDiagPhone, void *lpBuff, int nBuffLen);

// R/W SN
SP_EXPORT int       SP_API SP_ReadSN (SP_HANDLE hDiagPhone, bool sn1, void *pSN, unsigned long uLen);
SP_EXPORT int       SP_API SP_WriteSN(SP_HANDLE hDiagPhone, bool sn1, const void *pSN, unsigned long uLen);



// R/W Register 

SP_EXPORT int       SP_API SP_ReadRegister (SP_HANDLE            hDiagPhone, 
                                            PC_ADDR_ACESS_MODE_E eAcessMode, 
                                            unsigned long        ulRegAddr, 
                                            unsigned long        ulAccSize, 
                                            void                *lpRegData);
SP_EXPORT int       SP_API SP_WriteRegister(SP_HANDLE            hDiagPhone, 
                                            PC_ADDR_ACESS_MODE_E eAcessMode, 
                                            unsigned long        ulRegAddr, 
                                            unsigned long        ulAccSize, 
                                            const void          *lpRegData);
                                            
// R/W IMEI
SP_EXPORT int       SP_API SP_ReadImei (SP_HANDLE hDiagPhone, unsigned short ImeiNvID, unsigned char imei[MAX_IMEI_STR_LENGTH]);
SP_EXPORT int       SP_API SP_WriteImei(SP_HANDLE hDiagPhone, unsigned short ImeiNvID, unsigned char imei[MAX_IMEI_STR_LENGTH]);


// R/W calibration and FT testing flag (adc_reserved[7])
SP_EXPORT int       SP_API SP_LoadCaliAndTestFlag(SP_HANDLE hDiagPhone, unsigned long *puFlag);
SP_EXPORT int       SP_API SP_SaveCaliAndTestFlag(SP_HANDLE hDiagPhone, unsigned long   uFlag);


SP_EXPORT int       SP_API SP_SendATCommand(SP_HANDLE      hDiagPhone, 
                                            const char    *lpATCommand, 
                                            bool           bWantReply, 
                                            void          *lpReplyString, 
                                            unsigned long  ulReplyStringLen, 
                                            unsigned long *pulResponseLen, 
                                            unsigned long  ulTimeOut);

SP_EXPORT int       SP_API SP_GetRegCellInfo(SP_HANDLE hDiagPhone, GSM_CELL_INFO *pCellInfo);




SP_EXPORT int       SP_API SP_SetCampParam(SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eBand, unsigned short uArfcn);

// gsm calibration operation
SP_EXPORT const PC_TX_PARAM_T *SP_API SP_gsmSetTxParam(SP_HANDLE hDiagPhone, const PC_TX_PARAM_T *pParam);
SP_EXPORT int       SP_API SP_gsmActive         (SP_HANDLE hDiagPhone, bool bActive);

SP_EXPORT int       SP_API SP_gsmAfcStart       (SP_HANDLE hDiagPhone, bool bAfcStart, const PC_AFC_REQ_T *pParam);
SP_EXPORT int       SP_API SP_gsmApcStart       (SP_HANDLE hDiagPhone, bool bApcStart, const PC_APC_REQ_T *pParam);
SP_EXPORT int       SP_API SP_gsmAgcStart       (SP_HANDLE hDiagPhone, bool bAgcStart, const PC_AGC_REQ_T *pReq, PC_AGC_VALUE_CNF_T *pAck);

SP_EXPORT int       SP_API SP_gsmCalDirectCurr  (SP_HANDLE hDiagPhone, unsigned short *pI, unsigned short *pQ);
SP_EXPORT int       SP_API SP_gsmNonSignalCmd   (SP_HANDLE hDiagPhone, const PC_NONSIGNAL_REQ_T *pReq, PC_NONSIGNAL_CNF_T *pAck);

// GSM calibration advance operation
SP_EXPORT int       SP_API SP_gsmSetArfcn       (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, unsigned short uArfcn);
SP_EXPORT int       SP_API SP_gsmSetPCL         (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, unsigned short uPCL);
SP_EXPORT int       SP_API SP_gsmSetPwrFactor   (SP_HANDLE hDiagPhone, const PC_PWR_FACTOR_PARAM_T *pParam);
SP_EXPORT int       SP_API SP_gsmTxOn           (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, bool bOn);
SP_EXPORT int       SP_API SP_gsmRxOn           (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, bool bOn, \
                                                 unsigned short uGainIndex, unsigned short uGainValue, unsigned short uSampleCount);
SP_EXPORT int       SP_API SP_gsmGetRSSI        (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, unsigned short *prssi);
SP_EXPORT int       SP_API SP_gsmGetRxLevel     (SP_HANDLE hDiagPhone, PC_CALI_MS_BAND_E eMsBand, unsigned short *prxlev);

// R/W gsm calibration parameters 
SP_EXPORT int       SP_API SP_gsmReadCalibrationParam (SP_HANDLE              hDiagPhone, 
                                                       bool                   bFromFlash, 
                                                       L1_CALIBRATION_PARAM_E eType, 
                                                       PC_CALI_MS_BAND_E      eBand, 
                                                       unsigned short         uIndex, 
                                                       unsigned short         uLength, 
                                                       void                  *lpData);
SP_EXPORT int       SP_API SP_gsmWriteCalibrationParam(SP_HANDLE              hDiagPhone, 
                                                       bool                   bIntoFlash,
                                                       L1_CALIBRATION_PARAM_E eType, 
                                                       PC_CALI_MS_BAND_E      eBand, 
                                                       unsigned short         uIndex, 
                                                       unsigned short         uLength, 
                                                       const void            *lpData);
// Save gsm calibration parameters into flash
SP_EXPORT int       SP_API SP_gsmSaveToFlash(SP_HANDLE hDiagPhone, unsigned long ulTimeOut);

// FDT
SP_EXPORT int       SP_API SP_gsmFDT(SP_HANDLE hDiagPhone, const FDT_REQ *pFDTReq, FDT_RES *pFDTRes, unsigned long ulTimeOut);

// TD calibration operation
SP_EXPORT int       SP_API SP_tdActive      (SP_HANDLE hDiagPhone, bool bActive);
SP_EXPORT int       SP_API SP_tdAfcStart    (SP_HANDLE hDiagPhone, bool bAfcStart, const PC_AFC_REQ_T *pParam);
SP_EXPORT int       SP_API SP_tdAgcStart    (SP_HANDLE hDiagPhone, bool bAgcStart, const PC_AGC_REQ_T *pReq, PC_AGC_VALUE_CNF_T	*pAck);
SP_EXPORT int       SP_API SP_tdStartTx     (SP_HANDLE hDiagPhone, const PC_TD_TX_SET_T *pParam);
SP_EXPORT int       SP_API SP_tdTxGroupSweep(SP_HANDLE hDiagPhone, const PC_TD_TX_SWEEP_T *pParam);
SP_EXPORT int       SP_API SP_tdStopTxRx    (SP_HANDLE hDiagPhone, unsigned long ulTimeOut);
SP_EXPORT int       SP_API SP_tdNonSignalCmd(SP_HANDLE hDiagPhone, TD_NONSIGNAL_COMMAND_E eCmd, const PC_TD_NONSIGNAL_REQ_T *pReq);
SP_EXPORT int       SP_API SP_tdFDTTxRx(SP_HANDLE hDiagPhone, const PC_TD_FDT_TXRX *pParam,TDFDT_AGC_VALUE_CNF_T *pAck);
// R/W TD calibration parameters
SP_EXPORT int       SP_API SP_tdReadCalibrationParam (SP_HANDLE hDiagPhone, LPPC_TD_CAL_PARAM_T      pParam);
SP_EXPORT int       SP_API SP_tdWriteCalibrationParam(SP_HANDLE hDiagPhone, const PC_TD_CAL_PARAM_T *pParam);
SP_EXPORT int       SP_API SP_tdSaveToFlash(SP_HANDLE hDiagPhone, unsigned long ulTimeOut);


// Data Endian Convert
SP_EXPORT int       SP_API SP_DataEndianConv16(SP_HANDLE hDiagPhone, unsigned char *pData, unsigned long dataLen);
SP_EXPORT int       SP_API SP_DataEndianConv32(SP_HANDLE hDiagPhone, unsigned char *pData, unsigned long dataLen);


// BlueTooth
SP_EXPORT int       SP_API SP_btEnterEUTMode(SP_HANDLE hDiagPhone, bool bEnterOrLeave);
SP_EXPORT int       SP_API SP_btLoadAddr(SP_HANDLE hDiagPhone, unsigned char addr[MAX_BT_ADDR_NV_LENGTH]);
SP_EXPORT int       SP_API SP_btSaveAddr(SP_HANDLE hDiagPhone, unsigned char addr[MAX_BT_ADDR_NV_LENGTH]);

// WIFI
SP_EXPORT int       SP_API SP_wifiEnterEutMode (SP_HANDLE hDiagPhone, bool bEnable);
SP_EXPORT int       SP_API SP_wifiLoadAddr     (SP_HANDLE hDiagPhone, unsigned char addr[MAX_WIFI_ADDR_NV_LENGTH]);
SP_EXPORT int       SP_API SP_wifiSaveAddr     (SP_HANDLE hDiagPhone, unsigned char addr[MAX_WIFI_ADDR_NV_LENGTH]);
SP_EXPORT int       SP_API SP_wifiSetChannel   (SP_HANDLE hDiagPhone, unsigned int nChannel);
SP_EXPORT int       SP_API SP_wifiSetDataRate  (SP_HANDLE hDiagPhone, WIFI_DATA_RATE_ENUM eDataRate);
SP_EXPORT int       SP_API SP_wifiSetMode      (SP_HANDLE hDiagPhone, WIFI_MODE_ENUM eMode);
SP_EXPORT int       SP_API SP_wifiSetPCL       (SP_HANDLE hDiagPhone, unsigned int nPCL);
SP_EXPORT int       SP_API SP_wifiStartTx      (SP_HANDLE hDiagPhone, bool bStart, unsigned int nTxModeCtrl);
SP_EXPORT int       SP_API SP_wifiStartRx      (SP_HANDLE hDiagPhone, unsigned int nRxCtrl);
SP_EXPORT int       SP_API SP_wifiGetRxPackets (SP_HANDLE hDiagPhone, unsigned int *pnTotalPackets, unsigned int *pnErrorPackets);
SP_EXPORT int       SP_API SP_wifiFactorOper   (SP_HANDLE hDiagPhone, WIFI_FACTOR_ACTION eOperAction,  WIFI_PWR_FACTOR_SET_T *pFactorSet);
SP_EXPORT int       SP_API SP_wifiSetCrystalCap(SP_HANDLE hDiagPhone, unsigned char indac[2]);

// CMMB
SP_EXPORT int       SP_API SP_SendCmmbCommand(SP_HANDLE hDiagPhone, TOOL_TV_MSG_ID_E eCommand, const pTV_PARAM_T pInput, pTV_PARAM_T pOutput, DWORD ulTimeOut);


// DSP Debug
SP_EXPORT int       SP_API SP_dspDbgResetRFParam(SP_HANDLE hDiagPhone);
SP_EXPORT int       SP_API SP_dspDbgReadMemo    (SP_HANDLE hDiagPhone, bool bIsRegister, DSPDBG_MEMO_ACCESS_MODE eAccMode, unsigned long addr, void *lpData, unsigned long count);
SP_EXPORT int       SP_API SP_dspDbgWriteMemo   (SP_HANDLE hDiagPhone, bool bIsRegister, DSPDBG_MEMO_ACCESS_MODE eAccMode, unsigned long addr, unsigned long data);
SP_EXPORT int       SP_API SP_dspDbgPlotFrame   (SP_HANDLE hDiagPhone, FRAME_TYPE_NULLSTATE_E eFrameType, bool bIsOn, unsigned char nBandIndex, unsigned short uArfcn, unsigned short gain);   

// Change Mode
SP_EXPORT int       SP_API SP_ChangeCalMode(SP_HANDLE hDiagPhone, RM_MODE_ENUM eCalMode, unsigned long ulElaspedTime);


// Customer [GIONEER] Requirements: [[[
SP_EXPORT int       SP_API SP_CustomerPhoneStateReset(SP_HANDLE hDiagPhone, CUSTOMER_PHONE_STATE_OPER eOperType);
// Customer [GIONEER] Requirements: ]]]


SP_EXPORT int       SP_API  SP_SendDiagPacket(SP_HANDLE      hDiagPhone, 
                                              unsigned char  type, 
                                              unsigned char  subtype,  
                                              const void    *lpData, 
                                              unsigned int   ulLen);
SP_EXPORT int       SP_API  SP_RecvDiagPacket(SP_HANDLE      hDiagPhone, 
                                              unsigned char  type, 
                                              unsigned char  subtype,  
                                              void          *lpData, 
                                              unsigned long  ulLen, 
                                              unsigned long *pnRecvLen,
                                              unsigned long  ulTimeOut);

//  [11/8/2011 Xiaoping.Jing]  Send Condition command & receive Condition response
SP_EXPORT int       SP_API  SP_SendCondDiagPacket(SP_HANDLE      hDiagPhone, 
                                                  unsigned char  type, 
                                                  unsigned char  subtype,  
                                                  const void    *lpData, 
                                                  unsigned int   ulLen,
                                                  int           *pCondition);
SP_EXPORT int       SP_API  SP_RecvCondDiagPacket(SP_HANDLE      hDiagPhone, 
                                                  int            nCondition, 
                                                  void          *lpData, 
                                                  unsigned long  ulLen, 
                                                  unsigned long *pnRecvLen,
                                                  unsigned long  ulTimeOut);
//  [11/8/2011 Xiaoping.Jing] Send Condition command & receive Condition response
SP_EXPORT int     SP_API  SP_AutoBBTest(SP_HANDLE hDiagPhone,
										DEVICE_AUTOTEST_ID_E eCmd,
										const char*lpData, unsigned long 
										ulBytesToWrite,
										void *lpBuff, 
										unsigned long ulBytesToRead);


// LTE
SP_EXPORT int       SP_API SP_LTESpiRw (SP_HANDLE hDiagPhone,unsigned short rw_mode, unsigned short &nRegVal);
SP_EXPORT int       SP_API SP_LTEGpioSet(SP_HANDLE hDiagPhone,unsigned short nGpioRx,unsigned short nGpioTx);
SP_EXPORT int       SP_API SP_LTEActive( SP_HANDLE hDiagPhone, BOOL bActive);
SP_EXPORT int       SP_API SP_LTEAfcStart( SP_HANDLE hDiagPhone, BOOL bAfcStart,pPC_LTE_AFC_PARAM pAfcParam);
SP_EXPORT int       SP_API SP_LTEApcStart( SP_HANDLE hDiagPhone, BOOL bApcStart,pPC_LTE_APC_PARAM pApcParam);
SP_EXPORT int       SP_API SP_LTEAgcStart( SP_HANDLE hDiagPhone, BOOL bAgcStart,pPC_LTE_AGC_PARAM pAgcparam,  pPC_LTE_AGC_RET pAgcRet);
// R/W LTE calibration parameters
SP_EXPORT int       SP_API SP_LTEWriteCalibrationParam(SP_HANDLE hDiagPhone, const LTE_NV_CALDATA_WRITE_T *pParam);
SP_EXPORT int       SP_API SP_LTEReadCalibrationParam(SP_HANDLE hDiagPhone, LPPC_LTE_CAL_PARAM_T pParam);
SP_EXPORT int       SP_API SP_LTESaveToFlash(SP_HANDLE hDiagPhone, unsigned long ulTimeOut);


// HuaWei API 
SP_EXPORT int       SP_API SP_GetMsVersion_HuaWei(SP_HANDLE hDiagPhone, void *lpBuff, unsigned long ulBuffLen);
SP_EXPORT int       SP_API SP_GetSupportBand_HuaWei(SP_HANDLE hDiagPhone, void *lpBuff, unsigned long ulBuffLen);
SP_EXPORT int       SP_API SP_GetTargetCalPower_HuaWei(SP_HANDLE hDiagPhone, void *lpBuff, unsigned long ulBuffLen);
SP_EXPORT int       SP_API SP_GetBatteryTempCheckStatus_HuaWei(SP_HANDLE hDiagPhone, void *lpBuff, unsigned long ulBuffLen);

// ZTE
SP_EXPORT int       SP_API SP_ZTEWriteAdjust(SP_HANDLE hDiagPhone,const BYTE *pProductBuff, int nBuffsize);           
SP_EXPORT int       SP_API SP_ZTEReadAdjust(SP_HANDLE hDiagPhone,BYTE *pProductBuff, int nBuffsize, int *pnReadLen);
SP_EXPORT int       SP_API SP_ZTEWriteBoardNum(SP_HANDLE hDiagPhone,const BYTE *pProductBuff, int nBuffsize);
SP_EXPORT int       SP_API SP_ZTEReadBoardNum(SP_HANDLE hDiagPhone,BYTE *pProductBuff, int nBuffsize, int *pnReadLen);



#endif /* __SS__ */

#endif /* _PHONECOMMAND_H_BY_XPJ_20100928_33F51A91_858A_4779_BCC6_4AD48583B6BD_ */
