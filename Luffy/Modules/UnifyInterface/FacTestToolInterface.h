/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Unify Interface  
* File name     : FacTestToolInterface.h             
* Summary       : Interface for all station tools.                                                  
* Version		Author             Date                Abstract                      
* 0.1          Gilpin.Xi        2011/07/04           First version  
****************************************************************************************/

#pragma once


#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif


//#ifndef MI_MODE
//#define MI_MODE  //MI_DODE
//#endif

#ifndef FA_MODE
#define FA_MODE   //FA_MODE
#endif

//#ifndef CSD_MODE
//#define CSD_MODE  //CSD_DODE
//#endif

#define   MAX_SLOT				1
#define   DEFAULT_SLOT			0

#define IICDLL          TEXT("IIC.dll") //read id Yu.LEE
#define F32SERVERDB     TEXT("f32server2.dll") // query check pass/fail, 9j ¹¤†Î Yu.LEE
#define PICS_S3_FACTORY TEXT("Pics_S3_Factory.dll")//3s mac , bt address, update state booking , used SKY.GAN
#define VMS_DBACCESS    TEXT("VMS_DBAccess.dll")

/*===================================== InterfaceName =================================*/
#define     DLL_NEW_INTERFACE       "NewInterface"
#define     DLL_DEL_INTERFACE       "DeleteInterface"
/*===================================== InterfaceName =================================*/

/*============================================= Const =================================*/
const char UI_PROGRESS[]   = "UIProgress";
const char UI_MESSAGE[]    = "UIMessage";
const char UI_PATTERN[]    = "UIPattern";
const char UI_ITEM[]       = "UIITEM";
const char UI_RESULT[]     = "UIResult";
const char UI_PICSDATA[]   = "PICSData";
const char UI_JIGCONTROL[] = "JigControl";
const char UI_SHOWDIALOG[] = "ShowDialog";
const char UI_RELAYBOARD[] = "RelayBoard";
const char UI_POWERSUPPLY[] = "PowerSupply";
const char UI_CONNECT[]     = "UIConnect";
const char UI_NEXTITEM[]    = "UINextItem";
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

#pragma pack (1)
typedef struct
{
	int i_slot;                           // slot: [1, 8]  Default: 1
	int i_Connect;					      // 0: connect 1: disconnect
	bool CheckFlag;                     
}st_Connect;
#pragma pack()
/*============================================= Structure =============================*/

typedef void (*p_fn_FacTestToolInterface_callback)(const char* szEvent, long nParam);

/* UI interface */
interface IFacTestToolInterface
{
	virtual ~IFacTestToolInterface() {}

	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value, int i_slot = -1) = 0;
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size, int i_slot = -1) = 0;

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value) = 0;
	virtual bool GetSerialNumber(int i_slot, char* sz_value, int i_size) = 0;
	virtual bool GetTimeStamp(int i_slot, char* sz_value, int i_size) = 0;
	virtual bool CreateAndInitPhone(const int i_slot) = 0;
	virtual bool SetIMEI(int i_slot,char* sz_value,int i_size) = 0;
	virtual bool SetSN(int i_slot,char* sz_value,int i_size) = 0;
	virtual bool ReadId(int i_slot, char* sz_value, int i_size) = 0;
	virtual bool GetFailItem(int i_slot, char* sz_value, int i_size) = 0;

	/* Get test items */
	virtual int GetItemCount() = 0;
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size) = 0;

	virtual int GetRecoverItemCount() = 0;
	virtual bool GetRecoverItemByIndex(int i_index, char* sz_name, int i_size) = 0;

	/* Test sequence */
	virtual bool Begin    (int i_slot = 1) = 0;
	virtual bool PreRun   (int i_slot = 1) = 0;
	virtual bool Run      (int i_slot = 1) = 0;
	virtual bool PostRun  (int i_slot = 1) = 0;
	virtual bool Stop     (int i_slot = 1) = 0;
	virtual bool End      (int i_slot = 1) = 0;
	virtual bool LoadPhoneInfo(int i_slot = 1) = 0;

	/* Call back */
	virtual bool RegCallBack(p_fn_FacTestToolInterface_callback pCallback) = 0;
};

/* Global function pointer */
typedef bool (__cdecl *p_fn_newFacTestToolInterface)   (IFacTestToolInterface**);
typedef bool (__cdecl *p_fn_deleteFacTestToolInterface)(IFacTestToolInterface*);

