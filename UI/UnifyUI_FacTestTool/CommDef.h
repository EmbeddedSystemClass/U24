/***************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name   : Common Define value  
* File name     : CommDef.h             
* Summary       : Common Define for UI use.                                                  
* Version     Author             Date                 Abstract                      
* 1.0         Gilpin.Xi          2011/07/04           First version  
****************************************************************************/

#pragma once
#include "afxcmn.h"
#include "PageInterface.h"
#include "afxmt.h"
#include "Errorcode.h"
//#include "../../Modules/Common/ErrorDefine/ErrorDefine.h"



/* CIM Default Data */
#define DEFAULT_SO						_T("123456")
#define DEFAULT_SO_TYPE					_T("R")
#define DEFAULT_EMPLOYEE				_T("1007237")
#define DEFAULT_PC_NAME					_T("T02D01") //STATION_TRIGGER
#define OLDFA                           0
#define NEWFA                           1
#define UNDEFINED						_T("Undefined")
#define FACTORYLOG						_T("F")
#define SERVICELOG						_T("S")
#define OTHERLOG						_T("O")
#define PASSRESULT						_T("Y")
#define FAILRESULT						_T("N")

/* Message between main UI and Pages */
#define WM_USER_SETRESULT   WM_USER + 201
#define WM_USER_SETFOCUS    WM_USER + 202

/* Main UI Background color */
#define BKGCOLOUR_TS			       RGB(204, 153, 255)
#define BKGCOLOUR_RD				   RGB(192, 192, 192)//RGB(153,204,255)淡蓝//
#define BKGCOLOUR_FACTORY		       RGB(172, 213, 121)//益眼绿//RGB(215, 240, 198)原//RGB(204, 255, 204)浅绿//RGB(204, 232, 207)豆沙绿// 

/* Entrance Tool parameter to Main process */
#define NUM_TRANSFER_PARAMETER         5

/*==================== Station ============================*/
/* Station Number */
#define STATION_COUNT			      20

/* T-Line */
#define STATION_DL					  _T("DL")
#define STATION_TRIGGER			      _T("TRIGGER")
#define STATION_BOOT				  _T("BOOT")
#define STATION_2GPTEST			      _T("2GPTEST")
#define STATION_3GPTEST			      _T("3GPTEST")
#define STATION_4GPTEST			      _T("4GPTEST")
#define STATION_BBTEST 			      _T("BBTEST")
#define STATION_BTWLAN 			      _T("BTWLAN")
#define STATION_2G3GTEST              _T("2G3GTEST")
#define STATION_REDL				  _T("Re-DL")
#define STATION_LTE     		      _T("LTE")

/* A-Line */
#define STATION_ONLINE_WLS		      _T("ONLINE_WLS")
#define STATION_ONLINE_WLS2		      _T("ONLINE_WLS2")
#define STATION_ONLINE_WLS3		      _T("ONLINE_WLS3")
#define STATION_MMI_BB			      _T("MMI_BB")
#define STATION_ALS_AUDIO			  _T("ALS_AUDIO")
#define STATION_TOUCH				  _T("TOUCH")
#define STATION_CAMERA				  _T("CAMERA")
#define STATION_CAMERA_1              _T("CAMERA1")
#define STATION_CAMERA_2              _T("CAMERA2")
#define STATION_MMI_EM			      _T("MMI_EM")
#define STATION_FALINK			      _T("FALINK")
#define STATION_VI				      _T("VI")
#define STATION_RUN_IN				  _T("RUN_IN")

/* P-Line */
#define STATION_IMEI			      _T("IMEI")
#define STATION_OS_DL			      _T("OS_DL")
#define STATION_FCHECK			      _T("FCHECK")
#define STATION_PPIDLINK		      _T("PPIDLINK")
#define STATION_GB_PRINTING		      _T("GB_PRINTING")
#define STATION_FFU				      _T("FFU")
#define STATION_MMI_TEST			  _T("MMI_TEST")
#define STATION_ALS					  _T("ALS")
#define STATION_AUDIO				  _T("AUDIO")
#define STATION_CURRENT				  _T("CURRENT")
#define STATION_BIST				  _T("BIST")
#define STATION_NAL_SCAN              _T("NAL_SCAN")
#define STATION_PREMMIBB			  _T("PRE_MMI")
#define STATION_CSDPREMMIBB			  _T("CSD_PRE_MMI")

/* OOB */
#define STATION_OOB				      _T("OOB")


#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif
/*==================== Station ============================*/

/*================================= Station DLL Name ==================================*/
#define    DOWNLOAD_DLLNAME       _T("DL.dll")
#define    FT_DLLNAME             _T("FT.dll")
#define    RF_DLLNAME			  _T("RF.dll")
#define    RUN_IN_DLLNAME		  _T("RUN_IN.dll")

#define    MMIBB_DLLNAME          _T("MMIBB.dll")

#define    IMEI_DLLNAME           _T("IMEI.dll")
#define    FinalCheck_DLLNAME     _T("FinalCheck.dll")



#define PART_NUMBER                "PART_NUMBER"
#define PICASSO                    "PICASSO"
#define SW_VERSION                 "SW_VERSION"
#define FACTORY_VERSION			   "FACTORY_VERSION"
#define HW_VERSION                 "HW_VERSION"
#define TOOL_VERSION               "TOOL_VERSION"
#define IMEI		               "IMEI"
#define IMAGEPATH                  "IMAGEPATH"
#define PARAMETER_LOGSAVEPATH      "LOGSAVEPATH"
#define PARAMETER_LOGFILENAME      "LOGFILENAME"
#define MODEL_NAME				   "MODEL_NAME"
#define SO							"SO"
#define STATION_NAME				"STATION"
#define LOGPATH						"UI_LOG_PATH"
#define TOOLMODE					"TOOL_MODE"
#define DLMODE						"DLMODE"
#define RESERVE1                     "RESERVE1"
#define RESERVE2                     "RESERVE2"
#define SLOT_AVAILABLE_LIST         "SLOT_AVAILABLE_LIST"
#define DEVICE1						"DEVICE1"
#define DEVICE2						"DEVICE2"
#define DEVICE3						"DEVICE3"
#define DEVICE4						"DEVICE4"
#define COM_PORT					"COM_PORT"
#define MULTI_MODE                  "MULTI_MODE"

#define PICASSO_LENGTH  10
#define NAL_LENGTH		15
#define IMEI_LENGTH     15
#define Acer22_LENGTH  22

#define POWERON   0x01
#define POWEROFF  0x02
#define DEVIN     0x01
#define DEVOUT    0x02

#define CONFIG_PATH                _T("C:\\PE_Config")
#define PS_NUMBER				2
#define ALLSLOT					15
/*==================== Structure ==========================*/
/* Main UI display ID for all station use */
#pragma pack (1)
struct st_UIIDparameter
{
	int     i_parameterNum;          // Parameter num (NUM_TRANSFER_PARAMETER)
	bool    b_tsFlag;	             // TS_Flag
	CString str_firstProcess;		 // First process(station)
	CString str_station;             // Station (DL,FT...)
	CString str_modelName;           // Model Name
	CString str_line;                // Line (T-Line,A-Line...)
	CString str_pcba;
	CString str_so;
	CString str_partNumber;
	CString str_toolVer;
	CString str_employee;
	CString str_factoryVer;
	CString str_userLoadVer;
	CString str_imagePath;
	CString str_soType;
	CString str_segment;
	CString str_CRType;
	CString str_Reserve1;
	CString str_Reserve2;
	CString str_idtype;
	int		i_idLength;
};
#pragma pack()

/* For IMEI/FC station use */
#define SD_FILE_MAX_NUMBER     10

#pragma pack (1)
struct st_SDFILE
{
	CString str_SDFileName;
	CString str_SDFileSize;
};
#pragma pack()

#pragma pack (1)
struct st_pageReturn
{
	int i_slot;
	CString str_errorcode;
	CString str_errorMsg;
};
#pragma pack()

#pragma pack (1)
struct st_UIControl
{
	int i_PowerSupply[PS_NUMBER];
	int i_PowerOnDelay[PS_NUMBER];
	int i_PSAddr[PS_NUMBER];
	int i_PSInterface[PS_NUMBER];
	double lf_Voltage[PS_NUMBER];
	double lf_Current[PS_NUMBER];
	double lf_ShutVol[PS_NUMBER];

	int i_DevControl;
	CString str_DevPort;
	CString str_DevInCommand;
	CString str_DevOutCommand;
	int i_DeviceInDelay;

	bool b_RelayBoard;
	CString str_Device1;
	CString str_Device2;
	CString str_Device3;
	CString str_Device4;
	bool b_Multi;

	bool b_ScanPicasso;
	bool b_PreScan;
	bool b_AutoRunPreScan;
	bool b_AutoRun;
	bool b_NAL;
	bool b_ACER22;
	bool b_SQN;
	int  b_VKEnter;
	bool b_ReadFA;
	bool b_WriteFA;
	int  i_FAArea;
	CString str_NAL6;
};
#pragma pack()
/*==================== Structure ==========================*/

/*==================== Type Define ========================*/
typedef CArray<CPageInterface*, CPageInterface*> PropertyPageAry;
/*==================== Type Define ========================*/

//commstructs or functions
void GetStationVsDllMap(std::map<CString,CString> &map_StationVsDll);

bool Check_Picasso_Rule_All(CString str_picasso);
CString ConvertString(const char* sz_buffer);
bool IsStringAlphaNumber(CString &str_text);
bool SpiltString(CString str_srcString, CString str_delimiter, CStringArray& stra_strArg);
void CStringToChar(CString str_In, char *sz_Out);
/*void LogMsg(CString strMsg);*/
CString ConvertToVisibleString(char *sz_buf, int i_len);

extern CCriticalSection m_obj_FileLog;