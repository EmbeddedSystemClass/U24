#pragma once

#include "ErrCodeGenerator_API.h"
#include "WriteLog.h"

class CQueryErrorCode
{
public:
	CQueryErrorCode(void);
	~CQueryErrorCode(void);
public:
	
	bool IsInitDLL();
	bool LoadErrCodeGeneratorDLL();
	bool UnLoadErrCodeGeneratorDLLL();
	bool QueryErrCode(char* sz_errInput, char* sz_genErrcode, char* sz_genErrMsg, char* sz_errMessage,bool &b_inCIM);
	bool MapCSVFile(char* sz_errMessage);

	CString GetErrorMsg();

	pQueryErrorCode QueryErrorCode;
	pMapCSV MapCSV;
private:
	HMODULE m_h_ErrModule;
	CString m_str_errorMsg;
};
