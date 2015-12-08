/***************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name   : Common Define value  
* File name     : OtherStationDlg.h             
* Summary       : UI for 2G/3G/WLS/BB/MMIBB                                                  
* Version     Author             Date                Abstract                      
* 0.1        Nancy.Yang        2011/07/07           First version  
****************************************************************************/

#pragma once

#define FA_SIZE   512
#define PICASSO_SIZE 10
#define MOBILEID_SIZE 11

#include <string>
#include <vector>
#include <algorithm>
#include <afxpriv.h>
#include "CommDef.h"

using std::find;
using std::string;
using std::vector;

#ifndef _UNICODE
#define _UNICODE
#endif

class CFAOperator
{
public:
	CFAOperator(void);
	~CFAOperator(void);

	bool SetFA(char* sz_faData);
	bool GetFA(char* sz_faData);
	void GetPicassoNumber(char* sz_picasso);
	bool SetPicassoNumber(const char* sz_picasso);
	void GetModelName(char* sz_modelName);
	bool SetModelName(CString str_modelname);
	
	void GetSWVersion(char* sz_swVersion);
	bool SetSWVersion(CString str_swVersion);
	void GetMobileID(char *sz_mobileID);
	bool SetMobileID(const char *sz_mobileID);
	void GenerateMobileID(char * sz_mobileID, int szSlot3);
	void GetMobileIDHexStr(char *szMobileID);
	bool IsValidePicasso(char *sz_picasso);
	bool IsValideMobileID(char *szMobileID);

	CString GetErrorCode();
	CString GetErrorMsg();

	bool WriteStationInfo(
		char* szStationIndex,
		char* szLineType,
		char* szStationExternLen,
		char* szTestResult,
		char* szReserve,
		char* szStationExternInfo);
	
	bool FA_Station_Setup (char* szStationAmount, char* szStationList);
	bool EraseFactoryArea();
private:
	char m_sz_faData[FA_SIZE];
	CString m_str_errCode;
	CString m_str_errMsg;
private:
	
	bool IsStationSetup(char* faData);
	bool IsInSetupStationList(char* szStationIndex, char* faData);

	bool SetFAData(char* sz_faData);
	bool GetFAData(char* sz_faData);
	int TransferIntToHex_BCD(int nInt);
	bool TransferHexToInt_BCD(const int nHex, char* sz);

};
