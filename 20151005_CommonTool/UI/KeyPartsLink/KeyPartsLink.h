/**********************************************************
*   Copyright (C) 2011 Qisda Corporation
*   All Rights Reserved.
*   Class name : CPdaRec                                                     
*   Summary    : Record management for FA station and QA team
*   Version     Author            Date                Abstract  
*   1.0         Nancy.Yang        2011/05/12          First version
************************************************************
*/

// KeyPartsLink.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef UNICODE
#define argv __wargv
#else
#define argv __argv
#endif 
/* Entrance Tool parameter to Main process */
#define NUM_TRANSFER_PARAMETER         5
#define RUN_MODE_OFFLINE               false

#include "resource.h"		// main symbols


// CKeyPartsLinkApp:
// See KeyPartsLink.cpp for the implementation of this class
//

class CKeyPartsLinkApp : public CWinAppEx
{
public:
	CKeyPartsLinkApp();
	virtual ~CKeyPartsLinkApp();

	//This Struct is copied from \platformtool\UI\UnifyUI_FacTestTool\CommDef.h
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
	};

// Overrides
	public:
	virtual BOOL InitInstance();
	bool InitialDLL(void);
	typedef bool (*QueryEmployeeID_Right_V2)(char* sz_employId/*input*/, char * sz_Level/*output*/,char * sz_status/*output*/, char * sz_wrkstation/*output*/, char* sz_ErrCode/*output*/, char* sz_ErrMsg/*output*/);
	typedef bool (*UpdateEmployeeID_Status)(char* sz_employId/*input*/, char * sz_status/*input*/, char * sz_wrkstation/*input*/, char* sz_ErrCode/*output*/, char* sz_ErrMsg/*output*/);
	//typedef bool (*QuerySO_Info)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QuerySO_Info_All_V4)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_Segment,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*CheckSO)(char * sz_So/*input*/,char * sz_soCRtype/*input*/,char * sz_picasso/*input*/,char * sz_model/*input*/,char * sz_process/*input*/,char * sz_segment/*input*/, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryStation_Info)(char* sz_pcName, char* sz_line, char* sz_process, char* sz_segment, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QueryKeyparts_Info)(char * sz_PN, char * sz_Model,char * sz_Process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryPNinBACH_BySO)(char * sz_PN, char * sz_SO, char * sz_qty,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*CheckKeyparts_duplicate)(char* sz_Picasso,char * sz_kpid/*, char * sz_kpname,char * sz_model*//*input*/,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QuerySegmentProcess)(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
	typedef bool (*GetPicasso)(char * sz_InputID, char * sz_Model,char * sz_Picasso,  char* sz_partno, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryIMEI_ByPicasso)(char * sz_picassoid ,char * sz_IMEI,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QuerySOCount)(char* sz_Picasso, char* sz_SO, char* sz_Process, char* sz_SN, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*CheckRouting)(char* sz_id, char* sz_model, char* sz_process, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*CheckRouting_New)(char* sz_id, char* sz_model, char* sz_process, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertFAIL_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_inputErrorCode, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertPASS_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertKeypartsInfo)(char * sz_id, char * sz_process,char * sz_kpid, char * sz_kpname,char * sz_empolyee,char * sz_pn,char * sz_ErrCode,char * sz_ErrMsg);
	typedef bool (*InsertOOB_data)(char * sz_result, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertCRHoldInfo)(char * sz_picasso/*input*/,char * sz_IMEI/*input*/,char * sz_so/*input*/,char * sz_soCRtype/*input*/, char * sz_model/*input*/,char * sz_process/*input*/, char * sz_segment/*input*/,char * sz_employID/*input*/,char * sz_PN/*input*/,char * sz_ErrCode, char * sz_ErrMsg); 
	typedef bool (*QueryholdInfo)(char * sz_id ,char * sz_model,char * sz_status,char * sz_process,char * sz_holdreas,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryCRHoldInfo)(char * sz_id/*input*/,char * sz_status/*output*/, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*CheckProcessTestDate)(char * sz_Picasso,char * sz_model, char * sz_segment,char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryKeypartsByID)(char * sz_ID/*input*/, char * sz_process/*input*/,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*Query_PCBA_SerialNO)(char * sz_picasso/*input*/,char * sz_AcerMB/*input*/, char * sz_YWW/*input*/, char * sz_Employee/*input*/,char * sz_Serial/*output*/, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*Query_PCBA_NO)(char * sz_picasso/*input*/, char * sz_process/*input*/, char * sz_PCBAno/*output*/,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryNAL)(char * sz_IMEI/*input*/, char * sz_NAL, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryIMEIbyNAL)(char * sz_NAL/*input*/, char * sz_IMEI, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*CheckNAL_Validity)(char * sz_pn /*input*/,char * sz_NAL/*input*/,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*CheckBadge_Validity)(char * sz_Badge/*input*/, char * sz_pn/*input*/, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*SetValue)(const char * i_column, const char* sz_value);
	typedef bool (*ConnectDB)(void);
	typedef void (*CloseDB)(void);
	typedef bool (*GetIMEI_Info)(char * sz_InputID, char * sz_model,char * sz_Picasso, char * sz_partno,char * sz_SO, char * sz_sw,char * sz_ErrCode, char * sz_ErrMsg);

	//****For Wolf key part link	Dave.Wu 20111007
	typedef bool (*QueryRuninID)(char * sz_model, char* sz_ID, char * sz_column,char* sz_errorCode, char* sz_errorMsg);
    typedef bool (*UnlinkKeyparts)(char * sz_id, char * sz_model,char * sz_ErrCode, char * sz_ErrMsg);
    typedef bool (*UnlinkKeyparts_FA)(char * sz_id, char * sz_model,char * sz_process/*input*/,char * sz_ErrCode, char * sz_ErrMsg);
    typedef bool (*QueryIMEIStatus)(char * sz_InputID,char * sz_status,char * sz_cartonID, char * sz_ErrCode,char * sz_ErrMsg);

	QueryRuninID pQueryRuninID;
	//****Dave.Wu 20111007

	QueryEmployeeID_Right_V2 pQueryEmployeeID_Right_V2;
	UpdateEmployeeID_Status pUpdateEmployeeID_Status;
	//QuerySO_Info pQuerySO_Info;
	QuerySO_Info_All_V4 pQuerySO_Info_All_V4;
	CheckSO pCheckSO;
	QueryStation_Info pQueryStation_Info;
	QueryKeyparts_Info pQueryKeyparts_Info;
	QueryPNinBACH_BySO pQueryPNinBACH_BySO;
	CheckKeyparts_duplicate pCheckKeyparts_duplicate;
	QuerySegmentProcess pQuerySegmentProcess;
	GetPicasso pGetPicasso;
	QueryIMEI_ByPicasso pQueryIMEI_ByPicasso;
	QuerySOCount pQuerySOCount;
	CheckRouting pCheckRouting;
	CheckRouting_New pCheckRouting_New;
	InsertFAIL_data pInsertFAIL_data;
	InsertPASS_data pInsertPASS_data;
	InsertKeypartsInfo pInsertKeypartsInfo;
	InsertOOB_data pInsertOOB_data;
	InsertCRHoldInfo pInsertCRHoldInfo;
	QueryholdInfo pQueryholdInfo;
	QueryCRHoldInfo pQueryCRHoldInfo;
	CheckProcessTestDate pCheckProcessTestDate;
	QueryKeypartsByID pQueryKeypartsByID;
	SetValue pSetValue;
	ConnectDB pConnectDB;
	CloseDB pCloseDB;
	GetIMEI_Info pGetIMEI_Info;
    UnlinkKeyparts pUnlinkKeyparts;
	UnlinkKeyparts_FA pUnlinkKeyparts_FA;
	QueryIMEIStatus pQueryIMEIStatus;
	Query_PCBA_SerialNO pQuery_PCBA_SerialNO;
	Query_PCBA_NO pQuery_PCBA_NO;
	QueryNAL pQueryNAL;
	QueryIMEIbyNAL pQueryIMEIbyNAL;
	CheckNAL_Validity pCheckNAL_Validity;
	CheckBadge_Validity pCheckBadge_Validity;

	HINSTANCE m_hLib;
// Implementation

	int MultiByteToWideCharEnhanced(char * sz_src , CString & str_dst)
	{
		WCHAR WTemp[8192]={0};
		int nLen = MultiByteToWideChar(CP_ACP, 0, sz_src, -1, NULL, NULL); 
		MultiByteToWideChar(CP_ACP,NULL,sz_src,-1,WTemp,nLen);
		str_dst = WTemp;
		return nLen;
	}


	void SaveLog(CString strLogFileName,CString str_log)
	{
		/* Check Input valid */
		if ( str_log == _T("")) 
		{
			return;
		}


		/* Get current time */
		SYSTEMTIME time;
		::GetLocalTime(&time);

		/* CheckD:\\LOG exist and create */
		CString str_logDir;
		str_logDir = _T("D:\\LOG\\KeyPartsLink");

		WIN32_FIND_DATA fd_fileInformation;
		HANDLE h_searchFind;
		h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
		if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			FindClose(h_searchFind); 
		} 
		else 
		{
			FindClose(h_searchFind ); 
			if (CreateDirectory(str_logDir, NULL) == 0) 
			{
				return;
			} 
		} 

		CStdioFile obj_file;
		CString str_logFile;
		if (strLogFileName == _T(""))
			str_logFile.Format(_T("%s\\%04d_%02d_%02d.log"),str_logDir, time.wYear, time.wMonth, time.wDay);
		else
			str_logFile.Format(_T("%s\\%s"),str_logDir, strLogFileName);

		CString str_temp;
		str_temp.Format(_T("[%02d:%02d:%02d:%03d]:  %s "),time.wHour, time.wMinute, time.wSecond,time.wMilliseconds,str_log);

		/* Open log file and Save Log*/
		if (obj_file.Open(str_logFile, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL)) 
		{
			obj_file.SeekToEnd();
			obj_file.WriteString(str_temp);
			obj_file.WriteString(_T("\r\n"));
			obj_file.Close();
		}
		return;
	}

	DECLARE_MESSAGE_MAP()
};

extern CKeyPartsLinkApp theApp;