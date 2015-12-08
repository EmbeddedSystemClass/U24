#pragma once

#define  INIFILEPASH           _T(".\\INI\\setup.ini")
#define  DIRECTORYPATH         _T("C:\\Image\\")
#define  TOOLPATH              _T("C:\\TOOL\\")
#define  TOOLPATHCONFG         _T(".\\INI\\Tool_Path.ini")
#define  ERRORNUMBER           512

#include "..\..\Modules\DBAccess\DBAccess.h"
#include "..\..\Modules\VMS_DBAccess\VMS_DBAccess.h"

#include "MD5_Check.h"
#include "afx.h" 
#include "Filename_resolution.h"
#include "DirectoryDispose.h"

class CToolFunction
{
public:
	CToolFunction(void);
	~CToolFunction(void);
	bool GetSOInfoFromDB(char* chSOID, char* szErrorCode, char* szErrorMsg);

private:
	string m_strSO;
	string m_strToolVersion;
	string m_strFImageVersion;
	string m_strOImageVersion;
	string m_strEntranceVersion;
	string m_strFImageFixTag;
	string m_strEntranceFixTag;
	string m_strSOModel;
	string m_strSOPartNo;
	string m_strSOType;
	string m_strSegment;
	string m_strFImageList;

	string m_strProject;
	string m_strProcess;
public:
	bool CheckModel(char* chModel , char* szErrorCode , char* szErrorMsg);
	bool CheckVRCMStatus(char* szErrorCode, char* szErrorMsg);
	bool CheckVesionStatus(char* szErrorCode, char* szErrorMsg);
};
