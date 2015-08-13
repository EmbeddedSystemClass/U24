#ifndef __DL_INTERFACE__H__
#define __DL_INTERFACE__H__
#pragma once

#include "NotifyBase.h"
#include <vector>
#include <string>

const char UI_Progress[] = "UIProgress";
const char UI_Message[] = "UIMessage";
const char UI_Info[] = "UIInfo";
const char UI_Log[] = "UILog";
//UIInfo KeyName
const char szUI_Info_Result[] = "Result";
const char szUI_Info_Connection[] = "IsConnected";
const char szUI_Info_Connected[] = "Connected";
const char szUI_Info_Disconnected[] = "Disconnected";




struct sUIProgress
{
	int  nSlot;
	int  nMainProgress;
	int  nSubProgress;
};

struct sUIMessage
{
	int nSlot;
	char strMessage[512];
};

struct sUIInfo
{
	int nSlot;
	char strKey[256];
	char strInfo[256];
};

struct sUILog
{
	int nSlot;
	int nSeverity;
	char strMessage[512];
};


interface IDLInterface : public QSYNC_LIB::INotifyBase_Char
{
	virtual ~IDLInterface(){}

	// Device management and run
	virtual bool IsConnected(int nSlot) =0;
	virtual bool PreRun(int nSlot) =0;
	virtual bool Run(int nSlot) =0;
	virtual bool PostRun(int nSlot) =0;
	virtual bool Stop(int nSlot) =0;
	
	//get Donwload mode (download subset of SW image)
	virtual int GetDLModeCount()=0;
	virtual bool GetDLModeNameByIndex(int nIndex, char* szName , int nSize)=0;
	virtual bool SetImagePath(const char* szImagePath)=0 ;	

	//get image count 
	virtual int GetSWImageCount(const char* szDLMode ) =0;
	virtual bool GetSWImageByIndex(int nIndex, char* szName, int nSizeName, char* szImageType, int nSizeType ) =0;

	//get device name
	virtual bool GetDeviceNameBySlot(int nSlot, char* szName, int nSizeName ) =0;


	virtual bool SetAttribute(const char* szKey, const char* szValue, int nValueSize) =0;
	virtual bool GetAttribute(const char* szKey, char** szValue, int& nValueSize) =0;
	virtual int  GetLastError(int nIndex , char* szErrorMsg, int nSize) =0;


};

typedef bool (*pfnD_NewDLInterface)(IDLInterface**); //bool D_NewDLInterface(IDLInterface**);
typedef bool (*pfnD_DeleteDLInterface)(IDLInterface*); //bool D_DeleteDLInterface(IDLInterface);


#endif // __DL_INTERFACE__H__