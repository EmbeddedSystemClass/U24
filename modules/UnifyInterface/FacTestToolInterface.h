/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Unify Interface  
* File name     : FacTestToolInterface.h             
* Summary       : Interface for all station tools.                                                  
* Version		Author             Date                Abstract                      
* 0.1          Gilpin.Xi        2011/07/04           First version  
****************************************************************************************/

#pragma once

#include "NotifyBase.h"

/*================================= Station DLL Name ==================================*/
#define    DOWNLOAD_DLLNAME       _T("FactoryTool.dll")
#define    FT_DLLNAME             _T("FT.dll")
#define    RF_2G_DLLNAME          _T("RF.dll")
#define    RF_3G_DLLNAME          _T("RF.dll")
#define    BTWLAN_DLLNAME         _T("FT.dll")


#define    RUN_IN_DLLNAME		  _T("FactoryTool.dll")
#define    Wireless_DLLNAME       _T("FT.dll")
#define    MMIBB_DLLNAME          _T("FactoryTool.dll")
#define    FQC_DLLNAME          _T("FactoryTool.dll")
#define    PRE_MMI_DLLNAME        _T("PRE_MMI.dll")
#define    CSD_PRE_MMI_DLLNAME    _T("CSD_PRE_MMI.dll")

#define    IMEI_DLLNAME           _T("IMEI.dll")
#define    FinalCheck_DLLNAME     _T("FinalCheck.dll")

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif

/*===================================== InterfaceName =================================*/
#define     DLL_NEW_INTERFACE       "NewInterface"
#define     DLL_DEL_INTERFACE       "DeleteInterface"
/*===================================== InterfaceName =================================*/

/*============================================= Const =================================*/
const char UI_PROGRESS[]    = "UIProgress";
const char UI_MESSAGE[]     = "UIMessage";
const char UI_PATTERN[]     = "UIPattern";
const char UI_ITEM[]        = "UIITEM";
const char UI_RESULT[]      = "UIResult";
const char UI_PICSDATA[]    = "PICSData";
const char UI_JIGCONTROL[]  = "JigControl";
const char UI_SHOWDIALOG[]  = "ShowDialog";
const char UI_RELAYBOARD[]  = "RelayBoard";
const char UI_POWERSUPPLY[] = "PowerSupply";
const char UI_PICASSO[]    = "UIPicasso";
const char UI_SHOWIMGDIALOG[]	= "ShowImgDialog";
/*============================================= Const =================================*/

/*============================================= Structure =============================*/
#pragma pack (1)
typedef struct
{
	int i_slot;                           // slot: [1, 8]  Default: 1
	int i_mainProgress;                   // main progress bar: [0, 100]
	int i_subProgress;                    // sub progress bar : [0, 100]
}st_UIProgress;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                          // slot: [1, 8]  Default: 1
	char sz_message[MAX_BUFFER_SIZE];     // UI show message   (UI ListBox)
}st_UIMessage;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                           // slot: [1, 8]  Default: 1
	char sz_time	  [MAX_BUFFER_SIZE];   // current time   ( UI ListCtrl)
	char sz_result    [MAX_BUFFER_SIZE];   // time      ( UI ListCtrl)
	char sz_itemCode  [MAX_BUFFER_SIZE];   // current item   ( UI ListCtrl)
	char sz_errCode   [MAX_BUFFER_SIZE];   // errorCode    ( UI ListCtrl)
	char sz_band      [MAX_BUFFER_SIZE];   // baud    ( UI ListCtrl)
	char sz_channel   [MAX_BUFFER_SIZE];   // channel    ( UI ListCtrl)
	char sz_upper     [MAX_BUFFER_SIZE];   // upper value    ( UI ListCtrl)
	char sz_lower     [MAX_BUFFER_SIZE];   // lower value    ( UI ListCtrl)
	char sz_measured  [MAX_BUFFER_SIZE];   // measured  ( UI ListCtrl)
	char sz_unit      [MAX_BUFFER_SIZE];   // unit    ( UI ListCtrl)
	char sz_elapse    [MAX_BUFFER_SIZE];   // test time(s)  ( UI ListCtrl)
	char sz_msg       [MAX_BUFFER_SIZE];   // message  ( UI ListCtrl)

}st_UIResult;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                           // slot: [1, 8]  Default: 1
	bool b_append;						   // if this pattern need to append.(need: TRUE, not need: FALSE)
	char sz_pattern[MAX_BUFFER_SIZE];      // UI show pattern. For Touch, Camera
}st_UIPattern;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                          // slot: [1, 8]  Default: 1
	char sz_item[MAX_BUFFER_SIZE];        // Testing items
}st_UIItem;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                         // slot: [1, 8]  Default: 1
	char sz_key  [MAX_BUFFER_SIZE];      // PICS key
	char sz_value[MAX_BUFFER_SIZE];      // PICS value
}st_PICSData;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                         // slot: [1, 8]  Default: 1
	char sz_switch[MAX_BUFFER_SIZE];     // Jig Request: ON / OFF   only the two values
}st_JigRequest;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot;                        // slot: [1, 8]  Default: 1
	char sz_message[MAX_BUFFER_SIZE];   // Show dialog Request: UI Dialog showed message
}st_ShowDialog;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int  i_slot ;
	char szImgPath[MAX_BUFFER_SIZE] ;
	char szImgText[MAX_BUFFER_SIZE] ;
	char sz_buttonIDOK[MAX_BUFFER_SIZE] ;
	char sz_buttonIDCANCEL[MAX_BUFFER_SIZE] ;
}st_ShowImgDialog;
#pragma pack()


#pragma pack (1)
typedef struct
{
	int i_slot;                           // slot: [1, 8]  Default: 1
	int i_Seq;							// relayboard sequece
	bool b_OnOff;						 // open or close
}st_RelayBoard;
#pragma pack()

#pragma pack (1)
typedef struct
{
	int i_slot;                           // slot: [1, 8]  Default: 1
	int i_Seq;							  // powersupply sequece
	double dw_Voltage;						 // set voltage
	double dw_current;
}st_PowerSupply;
#pragma pack()

/*============================================= Structure =============================*/

#include "NotifyBase.h"

/* UI interface */
interface IFacTestToolInterface : public QSYNC_LIB::INotifyBase_Char
{
	virtual ~IFacTestToolInterface() {}

	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value) = 0;
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size) = 0;

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value) = 0;
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size) = 0;

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType) = 0;
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;
	virtual bool SetTag(int i_slot, char *sz_sectorData, int i_sectorSize) = 0;
	virtual bool SetSn(int i_slot, char *sz_sectorData, int i_sectorSize) = 0;

	/* Get test items */
	virtual int GetItemCount() = 0;
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size) = 0;

	/* Test sequence */
	virtual bool Begin    (int i_slot = 1) = 0;
	virtual bool PreRun   (int i_slot = 1) = 0;
	virtual bool Run      (int i_slot = 1) = 0;
	virtual bool PostRun  (int i_slot = 1) = 0;
	virtual bool Stop     (int i_slot = 1) = 0;
	virtual bool End      (int i_slot = 1) = 0;
	
	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait) = 0;
	
	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut) = 0;

};

/* Global function pointer */
typedef bool (__cdecl *p_fn_newFacTestToolInterface)   (IFacTestToolInterface**);
typedef bool (__cdecl *p_fn_deleteFacTestToolInterface)(IFacTestToolInterface*);

