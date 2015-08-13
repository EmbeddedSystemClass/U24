#pragma once

#include <vector>
#include <string>
#include "ISPU_CALLBACK.h"
#include "SPUExport.h"
#include "StdioFileEx.h"

using namespace std;

class SPU_CLASS_API CSPU : public IValue
{

public:
	CSPU(LPISPU_CALLBACK pISPU_CALLBACK);
	CSPU(LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptName);
	virtual ~CSPU(void);
	SPU_RESULT Run(vector<TSTRING>& strMemory,bool bEnableTRACE_F11,const TSTRING& strScriptName_F11);
	bool LoadCommand(CStdioFileEx& cFile,vector<TSTRING>& strMemory);
protected:
	SPU_RESULT Execute(const TSTRING& strInstruction,const TSTRING& strParameter
	,int& nIP
	,int& nLoopIP
	,bool& bCompare
	,int& nLoopCount
	,vector<TSTRING>& strVars
	,bool bEnableTRACE_F11
	,const TSTRING& strLogScriptName);
	TSTRING  GetValue(const TSTRING& strParameter,vector<TSTRING>& strVars);
	virtual bool  SetValue(const TSTRING& strParameter,const TSTRING& strValue,vector<TSTRING>& strVars);
	void Init();
	void GetToken(const TSTRING& strOriginal,TSTRING& strToken1,TSTRING& strToken2,const TSTRING& strSeperator);
	void Log(const TSTRING& strLog,const TSTRING& strName,const TSTRING& strErrorCode,const TSTRING& strMinorErrorCode,const TSTRING& strPriority,vector<TSTRING>& strVars);
	DebugCommand WaitDebugCommand(bool bEnable,const TSTRING& strDebugInfo);

	//Internal
	static DWORD WINAPI ThreadProc( LPVOID lpParam ) ;
	SPU_RESULT ExecuteThread(const TSTRING& strAPIDLL,const TSTRING& strFunction,LPISPU_CALLBACK pCallBack);
	SPU_RESULT ExecuteScript(const TSTRING& lpszScript,LPISPU_CALLBACK pCallBack,bool bEnableTRACE_F11);
	
private:
	LPISPU_CALLBACK m_pISPU_CALLBACK;

	//Log
	TSTRING m_strScriptName;

	//Thread
	HANDLE m_hThread;
	TSTRING m_strThreadAPIDLL;
	TSTRING m_strThreadFunction;
	LPISPU_CALLBACK m_pThreadCallBack;
	SPU_RESULT m_nThreadRet;
};
