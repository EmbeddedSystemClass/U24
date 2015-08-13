#ifndef __UIINFOHANDLER_H__
#define __UIINFOHANDLER_H__

#include <string>
#include "IniAccess.h"
#include "CGLog.h"

struct PICSinfo
{
	PICSinfo():
		 cstrSONumber(""),
		 cstrModelName(""),
		 cstrPicasso(""),
		 cstrMobileID(""),
		 cstrStation(""),
		 cstrGDND(""),
		 cstrErrCode(""),
		 cstrEmployeeID(""),
		 cstrPCBA(""),
		 cstrSWver(""),
		 cstrToolver(""),
		 cstrCFGver(""),
		 cstrDTver(""),
		 nStationNum(-1)
	{};

	CStringA cstrSONumber;
	CStringA cstrModelName;
	CStringA cstrPicasso;
	CStringA cstrMobileID;
	CStringA cstrStation;
	CStringA cstrGDND;
	CStringA cstrErrCode;
	CStringA cstrEmployeeID;
	CStringA cstrPCBA;
	CStringA cstrSWver;
	CStringA cstrToolver;
	CStringA cstrCFGver;
	CStringA cstrDTver;

	int nStationNum;
};

struct FAinfo
{
	FAinfo():
		cstrFAver(""),
		cstrMEver(""),
		cstrModelName(""),
		cstrPicasso(""),
		cstrPCBA(""),
		cstrSWver(""),
		cstrSlot("")
	{};
	
	CStringA cstrFAver;
	CStringA cstrMEver;
	CStringA cstrModelName;
	CStringA cstrPicasso;
	CStringA cstrPCBA;
	CStringA cstrSWver;
	CStringA cstrSlot;
};



class CPICS
{
public:
	bool WritePICS(PICSinfo *Info, char *szErrCode, char *szErrMsg);
	bool SetPICSValue(const char* key, const char* value);
	bool GenPICSFile(int szStation, char *PICSPath, char *szErrorCode, char *szErrorString);
};

class CFA
{
public:
	CFA();
	bool CheckPrevStation(char *szPort, const char *szIni, char *szErrCode, char *szErrMsg);

	bool FAHandler(char *szPort, char *szResult, char *szErrCode, char *szErrMsg, FAinfo *pFA = NULL);

	bool ReadFactoryArea(char* szPort, 
				char* szFAData, 
				char* szErrCode,
				char* szErrMsg);

	bool WriteFactoryArea(char* szPort, 
				 char* szFAData, 
				 char* szErrCode,
				 char* szErrMsg);

	bool EraseFactoryArea(char* szPort, 
				 char* szErrCode,
				 char* szErrMsg);

	bool EraseTargetStationInfo(char* szPort,
				       char* index,
				       char* szErrCode,
				       char* szErrMsg);

	bool WriteMobileID  (char* szPort, 
				char* szSlot,
				char* szErrCode,
				char* szErrMsg);

	bool ReadMobileID (char* szPort, 
			      char* szMobileID,
			      char* szErrCode,
			      char* szErrMsg);

	bool ReadModelName (char* szPort, 
			       char* szModelname,
			       char* szErrCode,
			       char* szErrMsg);

	bool WriteModelName (char* szPort, 
				char* szModelName,
				char* szErrCode,
				char* szErrMsg);

	bool WriteFAVersion (char* szPort, 
				char* szFAVersion,
				char* szErrCode,
				char* szErrMsg);

	bool ReadFAVersion (char* szPort, 
			       char* szFAVersion,
			       char* szErrCode,
			       char* szErrMsg);

	bool WriteMEVersion (char* szPort, 
				char* szMEVersion,
				char* szErrCode,
				char* szErrMsg);

	bool ReadMEVersion (char* szPort, 
			       char* szMEVersion,
			       char* szErrCode,
			       char* szErrMsg);

	bool WriteSWVersion (char* szPort, 
				char* szSWVersion,
				char* szErrCode,
				char* szErrMsg);

	bool ReadSWVersion (char* szPort, 
			       char* szSWVersion,
			       char* szErrCode,
			       char* szErrMsg);

	bool WritePCBAVersion (char* szPort, 
				  char* szPCBAVersion,
				  char* szErrCode,
				  char* szErrMsg);

	bool ReadPCBAVersion (char* szPort, 
				 char* szPCBAVersion,
				 char* szErrCode,
				 char* szErrMsg);

	bool WritePicassoNumber (char* szPort, 
			            char* szPicasso,
				    char* szErrCode,
				    char* szErrMsg);

	bool ReadPicassoNumber (char* szPort, 
				   char* szPicasso,
				   char* szErrCode,
				   char* szErrMsg);

	bool WriteTranceiverNumber (char* szPort, 
				       char* szXCVR,
				       char* szErrCode,
				       char* szErrMsg);

	bool ReadTranceiverNumber  (char* szPort, 
				       char* szXCVR,
				       char* szErrCode,
				       char* szErrMsg);

	bool WriteAllVersion   (char* szPort, 
				   char* szModelName,
				   char* szSWVersion,
				   char* szPCBAVersion,
				   char* szFAVersion,
				   char* szErrCode,
				   char* szErrMsg);
	bool ReadAllVersion (char* szPort, 
				char* szModelName,
				char* szMobileID,
				char* szSWVersion,
				char* szPCBAVersion,
				char* szPicasso,
				char* szFAVersion,
				char* szErrCode,
				char* szErrMsg);

	bool ReadStationInfo(char* szPort, 
				char* szStationIndex,
				char* szLineType,
				char* szTestResult,
				char* szTestCount,
				char* szTestDateTime,
				char* szTestLineStation,
				char* szReserve,
				char* szErrCode,
				char* szErrMsg);

	bool WriteStationInfo(char* szPort, 
				 char* szStationIndex,
				 char* szLineType,
				 char* szStationExternLen,
				 char* szTestResult,
				 char* szReserve,
				 char* szStationExternInfo,
				 char* szErrCode,
				 char* szErrMsg);

	bool CheckStationInfo(char* szPort, 
				 char* szStationIndex,	
				 char* szErrCode,
				 char* szErrMsg);

	bool CheckMobileID     (char* szPort, 
				   char* szMobileID,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckUISWVersion  (char* szPort, 
				   char* szUISW,
				   char* szFASW,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckUIPCBAVersion (char* szPort, 
	                            char* szUIPCBA,
				    char* szFAPCBA,
				    char* szErrCode,
				    char* szErrMsg);

	bool CheckUIMEVersion  (char* szPort, 
				   char* szUIME,
				   char* szFAME,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckUIPicasso    (char* szPort, 
				   char* szUIPicasso,
				   char* szFAPicasso,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckUITranceiver (char* szPort, 
				   char* szUIXCVR,
				   char* szFAXCVR,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckUIModelName  (char* szPort, 
				   char* szUIModelName,
				   char* szFAModelName,
				   char* szErrCode,
				   char* szErrMsg);

	bool CheckPicassoRule  (char* szPort, 
				   char* szPicasso,
				   char* szModelID,
				   char* szErrCode,
				   char* szErrMsg);
private:
	bool InitializeFAStationInfo(char *szPort, char *szErrCode, char *szErrMsg);
	bool InitializeFAPhoneInfo(char *szPort, char *szErrCode, char *szErrMsg, FAinfo *pFA);
	bool WriteFAStationInfo(char *szPort, char *szResult, char *szErrCode, char *szErrMsg);
	bool GetIniHandle(const char *szIni, char *szErrCode, char *szErrMsg);

	std::string m_strStationInfoPath;
	int m_nCurrentStation;
	CIniAccess m_cStationInitInfoini;
	FAinfo *m_pFAinfo;
};

class CPCEnv
{
public:
    bool TestPCEqiupment(const char *szIni, char *szErrMsg) ;
    int GetPCCPUSpeed() ;
    int GetPCRAMSize () ;
};

class CUIInfoHandler
{
public:
	//GLog
	CGLog GLog;

	//QLog

	//PICS
	CPICS PICS;
	
	//FA
	CFA FA;

    //PCEnv
    CPCEnv PCEnv ;
};
#endif