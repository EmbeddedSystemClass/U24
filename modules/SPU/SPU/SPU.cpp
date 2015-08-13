// SPU.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SPU.h"
#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include "..\..\Common\CommonUtil\UnicodeUtility.h"
#include "..\..\Common\CommonUtil\Win32ThreadUtility.h"
#include "..\..\Common\CommonUtil\LogPolicy.h"
#include "..\..\Common\CommonUtil\QPerformanceTimer.h"
#include "Functor.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    // initialize MFC and print and error on failure
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        // TODO: change error code to suit your needs
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    }
    else
    {
        // TODO: code your application's behavior here.
    }

    return nRetCode;
}

using namespace QSYNC_LIB;

CSPU::CSPU(LPISPU_CALLBACK pISPU_CALLBACK)
:m_pISPU_CALLBACK(pISPU_CALLBACK)
,m_strScriptName(_T("SPU_SCRIPT"))
,m_hThread(NULL)
,m_pThreadCallBack(NULL)
{
}

CSPU::CSPU(LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptName)
:m_pISPU_CALLBACK(pISPU_CALLBACK)
,m_strScriptName(strScriptName)
{
}

CSPU::~CSPU(void)
{
}

void CSPU::Init()
{
	//Registers
	int nIP = 0;
	int nLoopIP = 0;
	bool bCompare = false;
	int nLoopCount = 0;
}

void CSPU::GetToken(const TSTRING& strOriginal,TSTRING& strToken1,TSTRING& strToken2,const TSTRING& strSeperator)
{
	//Avoid " " in the strToken1
	TSTRING::size_type nStartFindIndex = strOriginal.find(_T("\""));
	if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
	{
		TSTRING::size_type nFindEndIndex = strOriginal.find(_T("\""),1);

		TSTRING::size_type nFindIndex = strOriginal.find(strSeperator,nFindEndIndex);
		if(nFindIndex != TSTRING::npos)
		{
			strToken1 = strOriginal.substr(1,nFindEndIndex-1);
			strToken2 = strOriginal.substr(nFindIndex+strSeperator.size());
		}
		else
		{
			strToken1 = strOriginal.substr(1,nFindEndIndex-1);;
			strToken2.clear();
		}
	}
	else
		QSYNC_LIB::GetToken(strOriginal,strToken1,strToken2,strSeperator);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2009/2/12 By alan.Tu
	//Please don't remove " " for strToken2
	//Avoid " " in the strToken2
	/*
	nStartFindIndex = strToken2.find(_T("\""));
	if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
	{
		TSTRING::size_type nFindEndIndex = strToken2.find(_T("\""),1);
		strToken2 = strToken2.substr(1,nFindEndIndex-1);
	}
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	QSYNC_LIB::TrimLeft(strToken1,_T(": \r\n\t"));
	QSYNC_LIB::TrimRight(strToken1,_T(": \r\n\t"));
	QSYNC_LIB::TrimLeft(strToken2,_T(": \r\n\t"));
	QSYNC_LIB::TrimRight(strToken2,_T(": \r\n\t"));
}

SPU_RESULT CSPU::Run(vector<TSTRING>& strMemory,bool bEnableTRACE_F11,const TSTRING& strScriptName_F11)
{
	SPU_RESULT res = SPU_SUCCESS;
	
	TSTRING strScriptName = strScriptName_F11;

	//Debug info
	bool bEnableTRACE = bEnableTRACE_F11;
	bool bHasBREAK = false;

	//Registers
	int nIP = 0;
	int nLoopIP = 0;
	bool bCompare = false;
	int nLoopCount = 0;	
	vector<TSTRING> strVars;
	TSTRING strEmpty;
	for(int i=0;i<REGISTER_COUNT;i++)
		strVars.push_back(strEmpty);

	TCHAR szCurrentPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH,szCurrentPath);

	TCHAR szName[MAX_PATH] = {0};
	_tcscpy(szName,PathFindFileName(strScriptName.c_str()));
	TCHAR szPath[MAX_PATH] = {0}; 

	BOOL bAppend = ::PathAppend(szCurrentPath,strScriptName.c_str());

	_tcscpy(szPath,szCurrentPath);
	::PathRemoveFileSpec(szPath);
	::PathAppend(szPath,_T("Log"));
	::CreateDirectory(szPath,NULL);
	
	::PathAddBackslash(szPath);
	::PathAppend(szPath,szName);
	
	CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	CString strTemp;
	strTemp.Format(_T("%s_%s.%06d"),szPath,strTime,clock());
	TSTRING strLogScriptName = strTemp;

	while(SPU_SUCCESS == res && nIP < static_cast<int>(strMemory.size()))
	{
		TSTRING strADDR,strParameter;
		GetToken(strMemory[nIP++],strADDR,strParameter,_T(" "));
		
		//define 9XXX to be break
		bHasBREAK = (strADDR[0] == '9');

		TSTRING strInstruction;
		GetToken(strParameter,strInstruction,strParameter,_T(" "));
		MakeLower(strInstruction);

		//Handle debug command
		TCHAR szTemp[8192] = {0};
		
		TSTRING strDebugInfo = _T("Execute: ") + strScriptName + _T("\n\n");

		strDebugInfo += _T("---->\t")  + strMemory[nIP-1];
		for(int nMaxLine = 0;nMaxLine < 5;nMaxLine++)
		{
			strDebugInfo += '\n';
			if((unsigned int)(nIP + nMaxLine) < strMemory.size())
				strDebugInfo += _T("\t") + strMemory[nIP + nMaxLine];			
		}

		::_stprintf(szTemp,_T("\n\n\nIP\t\t=%04d\nLoopIP\t\t=%04d\nLoopCount\t=%04d\nCompare\t=%01d"),nIP,nLoopIP,nLoopCount,bCompare);
		strDebugInfo += szTemp;
		for(vector<TSTRING>::iterator Iter = strVars.begin();Iter != strVars.end();Iter++)
		{
			if (4096 >= 9 + Iter->length() +1)
				::_stprintf(szTemp,_T("\nVAR%02d\t\t=%s"),(Iter - strVars.begin()),Iter->c_str());
			strDebugInfo += szTemp;
		}		

		DebugCommand nDebugCommand = WaitDebugCommand(bHasBREAK || bEnableTRACE,strDebugInfo);		
		//Reset
		bool bEnableTRACE_F11 = false;
		bHasBREAK = false;
		bEnableTRACE = false;
		if(F10 == nDebugCommand)
			bEnableTRACE = true;
		else if(F11 == nDebugCommand)
		{
			bEnableTRACE = true;
			bEnableTRACE_F11 = true;
		}
		else if(CTRL_BREAK == nDebugCommand)
		{
			res = SPU_BREAK;
			break;
		}

		res = Execute(strInstruction,strParameter,nIP,nLoopIP,bCompare,nLoopCount,strVars,bEnableTRACE_F11,strLogScriptName);
	};	
	if(res == SPU_SUCCESS)
		res = bCompare ? SPU_SUCCESS : SPU_FAIL;
	return res;
}

DebugCommand CSPU::WaitDebugCommand(bool bEnable,const TSTRING& strDebugInfo)
{
	DebugCommand DC = F5;
	if(bEnable)
	{
		//Wait for input
		if(m_pISPU_CALLBACK)
		{
			TCHAR szBuffer[2048] = {0};
			_tcscpy_s(szBuffer,2048,strDebugInfo.c_str());
			if(m_pISPU_CALLBACK->WaitInput(_T("DEBUGCOMMAND"),szBuffer,2048))
			{
				if(_tcscmp(szBuffer,_T("F5")) == 0)
					DC = F5;
				else if(_tcscmp(szBuffer,_T("F10")) == 0)
					DC = F10;
				else if(_tcscmp(szBuffer,_T("F11")) == 0)
					DC = F11;
				else if(_tcscmp(szBuffer,_T("CTRL_BREAK")) == 0)
					DC = CTRL_BREAK;
			}
		}
	}
	return DC;
}

bool  CSPU::SetValue(const TSTRING& strParameter,const TSTRING& strValue,vector<TSTRING>& strVars)
{
	bool bRes = false;
	if(strParameter.find(_T("%var")) == 0)
	{
		TSTRING strInteger = strParameter.substr(4);
		int nIndex = toInt(strInteger);
		if(nIndex < REGISTER_COUNT)
		{
			strVars[nIndex] = strValue;
			bRes = true;
		}
	}
	return bRes;
}

TSTRING  CSPU::GetValue(const TSTRING& strParameter,vector<TSTRING>& strVars)
{
	TSTRING strTemp;
	if(strParameter.find(_T("%var")) == 0)
	{
		TSTRING strInteger = strParameter.substr(4);
		int nIndex = toInt(strInteger);
		if(nIndex < REGISTER_COUNT)
			strTemp = strVars[nIndex];
	}
	else
	{	
		//Avoid " " in the strParameter
		TSTRING::size_type nStartFindIndex = strParameter.find(_T("\""));
		if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
		{
			TSTRING::size_type nFindEndIndex = strParameter.find(_T("\""),1);
			strTemp = strParameter.substr(1,nFindEndIndex-1);
		}
		else
			strTemp = strParameter;
	}

	return strTemp;
}

SPU_RESULT CSPU::Execute(const TSTRING& strInstruction,const TSTRING& strParameter
	,int& nIP
	,int& nLoopIP
	,bool& bCompare
	,int& nLoopCount
	,vector<TSTRING>& strVars	
	,bool bEnableTRACE_F11
	,const TSTRING& strLogScriptName)
{
	SPU_RESULT res = SPU_FAIL;
	DWORD nStartTime = GetTickCount();

	CString strLog;
	strLog.Format(_T("%04d %s %s"),nIP-1, strInstruction.c_str(),strParameter.c_str());
	Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);

	if(strInstruction == _T("sleep"))
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : sleep 5000
		//Sample2 : sleep [var1]
		///////////////////////////////////////////////////////////////////////////////////////
		::Sleep(toInt(GetValue(strParameter,strVars)));
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("assign"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : assign %var1 123456		
		//Sample2 : assign %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strVariable,strValue;
		GetToken(strParameter,strVariable,strValue,_T(" "));
		res = SetValue(strVariable,GetValue(strValue,strVars),strVars) ? SPU_SUCCESS : SPU_FAIL;
	}
	else if(strInstruction == _T("compare"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = GetValue(strValue1,strVars) ==  GetValue(strValue2,strVars);

		CString strLog;
		strLog.Format(_T("\"%s\" == \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("comparenocase"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		transform(strValue1.begin(), strValue1.end(), strValue1.begin(), toupper);
		transform(strValue2.begin(), strValue2.end(), strValue2.begin(), toupper);
		bCompare = GetValue(strValue1,strVars) ==  GetValue(strValue2,strVars);

		CString strLog;
		strLog.Format(_T("\"%s\" == \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);

		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("bigger"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = ::_ttoi(GetValue(strValue1,strVars).c_str()) > ::_ttoi(GetValue(strValue2,strVars).c_str());

		CString strLog;
		strLog.Format(_T("\"%s\" > \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("smaller"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = ::_ttoi(GetValue(strValue1,strVars).c_str()) < ::_ttoi(GetValue(strValue2,strVars).c_str());

		CString strLog;
		strLog.Format(_T("\"%s\" < \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("add"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : add %var1 1		
		//Sample2 : add %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1, strValue2;
		GetToken (strParameter, strValue1, strValue2, _T(" "));
		int val1 = _ttoi (GetValue(strValue1, strVars).c_str());
		int val2 = _ttoi (GetValue(strValue2, strVars).c_str());
		TCHAR result[16];
		_stprintf (result, _T("%d"), val1+val2);
		res = SetValue (strValue1, result, strVars) ? SPU_SUCCESS : SPU_FAIL;

	}
	else if(strInstruction == _T("addstring"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : add %var1 1		
		//Sample2 : add %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1, strValue2 ;
		GetToken (strParameter, strValue1, strValue2, _T(" "));
		TSTRING strVal1 = GetValue(strValue1, strVars).c_str() ;
		TSTRING strVal2 = GetValue(strValue2, strVars).c_str() ;
		TCHAR result[MAX_PATH];
		_stprintf_s (result, MAX_PATH, _T("%s%s"), strVal1.c_str(), strVal2.c_str());
		res = SetValue (strValue1, result, strVars) ? SPU_SUCCESS : SPU_FAIL;

	}
	else if(strInstruction == _T("print"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : print 123456 NAME
		//Sample2 : print %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////		
		TSTRING strTarget,strName;
		GetToken(strParameter,strTarget,strName,_T(" "));
		if(m_pISPU_CALLBACK)
		{
			if(m_pISPU_CALLBACK->Print(GetValue(strName,strVars),GetValue(strTarget,strVars)))
				res = SPU_SUCCESS;
		}
	}
    else if(strInstruction == _T("writelog"))
    {
        ///////////////////////////////////////////////////////////////////////////////////////		
        //Sample1 : writelog item result
        //Sample2 : writelog %var2 %var1
        ///////////////////////////////////////////////////////////////////////////////////////		
        TSTRING strItem,strValue;
        GetToken(strParameter, strItem, strValue, _T(" "));
        if(m_pISPU_CALLBACK)
        {
			if(m_pISPU_CALLBACK->Print(_T("DBInformation"),GetValue(strItem, strVars) + _T(":") + GetValue(strValue, strVars)))
                res = SPU_SUCCESS;
        }
    }
	else if(strInstruction == _T("loop"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : loop -1
		//Sample2 : loop 2
		//Sample3 : loop 
		///////////////////////////////////////////////////////////////////////////////////////
		if(strParameter.empty())
		{
			if(nLoopCount != -1)
			{
				nLoopCount--;
				if(nLoopCount > 0)
					nIP = nLoopIP;
			}
			else nIP = nLoopIP;			
		}
		else
		{
			nLoopIP = nIP;
			nLoopCount = toInt(GetValue(strParameter,strVars));
		}
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("waitinput"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : waitinput %var1 Name
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strTarget,strName;
		GetToken(strParameter,strTarget,strName,_T(" "));

		if(m_pISPU_CALLBACK)
		{
			TCHAR szBuffer[SPU_BUFFER_SIZE] = {0};
			assert(GetValue(strTarget,strVars).size() < SPU_BUFFER_SIZE);
			_tcscpy_s(szBuffer,SPU_BUFFER_SIZE,GetValue(strTarget,strVars).c_str());
			if(m_pISPU_CALLBACK->WaitInput(GetValue(strName,strVars),szBuffer,SPU_BUFFER_SIZE))
			{
				SetValue(strTarget,szBuffer,strVars);
				res = SPU_SUCCESS;
			}
		}
	}
	else if(strInstruction == _T("checkbreak"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : checkbreak
		///////////////////////////////////////////////////////////////////////////////////////
		if(m_pISPU_CALLBACK)
			res = m_pISPU_CALLBACK->CheckBreak() ? SPU_BREAK : SPU_SUCCESS;
		else
			res = SPU_SUCCESS;//Without any check
	}
	else if(strInstruction == _T("comparetrue"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : comparetrue 0005
		///////////////////////////////////////////////////////////////////////////////////////
		if(bCompare == true)
			nIP = toInt(strParameter);
		if(nIP == 9999)
			res = SPU_FAIL;
		else
			res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("comparefalse"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : comparefalse 0005
		///////////////////////////////////////////////////////////////////////////////////////
		if(bCompare == false)
			nIP = toInt(strParameter);
		if(nIP == 9999)
			res = SPU_FAIL;
		else
			res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("run"))
	{
		TSTRING strAPIDLL;
		TSTRING strFunction;

		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : run API.dll Function1 %var1 ABC %var3
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strFunctionAndParameter;
		GetToken(strParameter,strAPIDLL,strFunctionAndParameter,_T(" "));

#ifdef _DEBUG
		// DEBUG mode, try to use xxx_xD.dll
		TSTRING strDebugDLL = strAPIDLL;
		strDebugDLL.insert (strDebugDLL.size()-4, _T("D"));

		// check if file exists
		HMODULE hHandle = ::LoadLibrary (strDebugDLL.c_str ( ));
		if (hHandle && hHandle != INVALID_HANDLE_VALUE)
		{
			::FreeLibrary (hHandle);
			strAPIDLL = strDebugDLL;
		}
#endif

#ifdef _DEBUG  //easier to debug, no need to edit all script files.
		TSTRING::size_type nIndex = strAPIDLL.find(_T("StaticM.dll"));
		if (nIndex != string::npos)
			strAPIDLL.replace(nIndex, 11, _T("StaticMD.dll"));

		nIndex = strAPIDLL.find(_T("StaticU.dll"));
		if (nIndex != string::npos)
			strAPIDLL.replace(nIndex, 11, _T("StaticUD.dll"));

		nIndex = strAPIDLL.find(_T("ShareM.dll"));
		if (nIndex != string::npos)
			strAPIDLL.replace(nIndex, 11, _T("ShareMD.dll"));

		nIndex = strAPIDLL.find(_T("ShareU.dll"));
		if (nIndex != string::npos)
			strAPIDLL.replace(nIndex, 11, _T("ShareUD.dll"));
#endif

		TSTRING strFunctionParameter;
		GetToken(strFunctionAndParameter,strFunction,strFunctionParameter,_T(" "));

		vector<CFunctorParameter> vectParameter;
		TSTRING strFunctionParameter1,strFunctionParameter2 = strFunctionParameter;
		do
		{			
			GetToken(strFunctionParameter2,strFunctionParameter1,strFunctionParameter2,_T(" "));
			if(!strFunctionParameter1.empty())
			{
				TSTRING strValue,strType;
				GetToken(strFunctionParameter1,strValue,strType,_T("::"));
				CFunctorParameter parameter(GetValue(strValue,strVars),strType,strValue,strVars);
				vectParameter.push_back(parameter);
			}
		}while(!strFunctionParameter2.empty());

		CFunctor command(strAPIDLL,strFunction,vectParameter);
		bCompare = command();
		res = SPU_SUCCESS;

		strLog.Format(_T("Compare flag = %d"),bCompare);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	}
	else if(strInstruction == _T("runscript"))
	{
		TSTRING strScript = strParameter;
		res = ExecuteScript(strScript,m_pISPU_CALLBACK,bEnableTRACE_F11);
		bCompare = SPU_SUCCESS == res;
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("runshell"))
	{
		TSTRING strAction;
		TSTRING strFile;

		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : runshell open API.exe %var1 %var2 %var3
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strFileAndParameter;
		GetToken(strParameter,strAction,strFileAndParameter,_T(" "));
		GetToken(strFileAndParameter,strFile,strFileAndParameter,_T(" "));

		vector<CFunctorParameter> vectParameter;
		TSTRING strFunctionParameter1,strFunctionParameter2 = strFileAndParameter;
		do
		{			
			GetToken(strFunctionParameter2,strFunctionParameter1,strFunctionParameter2,_T(" "));
			if(!strFunctionParameter1.empty())
			{
				TSTRING strValue,strType;
				GetToken(strFunctionParameter1,strValue,strType,_T("::"));
				CFunctorParameter parameter(GetValue(strValue,strVars),strType,strValue,strVars);
				vectParameter.push_back(parameter);
			}
		}while(!strFunctionParameter2.empty());

		TSTRING strShellParameter;
		for(vector<CFunctorParameter>::iterator Iter = vectParameter.begin();Iter != vectParameter.end();Iter++)
		{
			using namespace QSYNC_LIB;			
			strShellParameter += _T(" ");
			string strTemp = *Iter;
			strShellParameter += GetTSTRING(strTemp);
		}

		res = (HINSTANCE)32 >= ShellExecute(NULL,GetValue(strAction,strVars).c_str(),GetValue(strFile,strVars).c_str(),strShellParameter.c_str(),NULL,SW_SHOWNORMAL) ? SPU_SUCCESS : SPU_FAIL;
		bCompare = SPU_SUCCESS == res;
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("runthread"))
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : runthread API.dll Function1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strAPIDLL;
		TSTRING strFunction;

		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : run API.dll Function1 %var1 ABC %var3
		///////////////////////////////////////////////////////////////////////////////////////
		GetToken(strParameter,strAPIDLL,strFunction,_T(" "));

		bCompare = SPU_SUCCESS == ExecuteThread(strAPIDLL,strFunction,m_pISPU_CALLBACK);		
		res = SPU_SUCCESS;
	}
	else
	{
		assert(0);
		CString strLog;
		strLog.Format(_T("There is a unknown instruction on your script. \"%s\""),strInstruction);
		Log((LPCTSTR)strLog,strLogScriptName,_T("9999"),_T("-1"),_T("1"),strVars);
	}

	// Save log
	//CString cstrParameter = strParameter.c_str();
	//cstrParameter.Replace(_T("\""), _T("\"\""));

	//CString strLog_RunTime;
	//DWORD nRunTime = GetTickCount() - nStartTime;
	//strLog_RunTime.Format(_T("%s,\"%s\",%d"), strInstruction.c_str(), cstrParameter, nRunTime);
	//switch(res)
	//{
	//case SPU_SUCCESS:
	//	strLog_RunTime += _T(",SPU_SUCCESS");
	//	break;
	//case SPU_FAIL:
	//	strLog_RunTime += _T(",SPU_FAIL");
	//	break;
	//case SPU_BREAK:
	//	strLog_RunTime += _T(",SPU_BREAK");
	//	break;
	//default:
	//	strLog_RunTime += _T(",Unknown response");
	//	break;
	//}

	//LogPolicy::Log(strLog_RunTime, _T("SPU_RunTime.csv"), false);
	return res;
}

void CSPU::Log(const TSTRING& strLog,const TSTRING& strName,const TSTRING& strErrorCode,const TSTRING& strMinorErrorCode,const TSTRING& strPriority,vector<TSTRING>& strVars)
{
	vector<CFunctorParameter> vectLogParameter;
	TSTRING strValue,strType;
	strValue = strName;
	CFunctorParameter parameter1(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter1);
	strValue = strErrorCode;
	CFunctorParameter parameter2(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter2);
	strValue = strMinorErrorCode;
	CFunctorParameter parameter3(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter3);
	strValue = strPriority;
	CFunctorParameter parameter4(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter4);
	strValue = strLog;
	CFunctorParameter parameter5(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter5);
#ifdef MFCShare

#ifdef _DEBUG
#ifdef _UNICODE
	CFunctor Logcommand(_T("ReportCenter_ShareUD.dll"),_T("Log_ForSPU"),vectLogParameter);
#else
	CFunctor Logcommand(_T("ReportCenter_ShareMD.dll"),_T("Log_ForSPU"),vectLogParameter);
#endif
#else
#ifdef _UNICODE
	CFunctor Logcommand(_T("ReportCenter_ShareU.dll"),_T("Log_ForSPU"),vectLogParameter);
#else
	CFunctor Logcommand(_T("ReportCenter_ShareM.dll"),_T("Log_ForSPU"),vectLogParameter);
#endif
#endif

#else //#ifdef MFCShare

#ifdef _DEBUG
#ifdef _UNICODE
	CFunctor Logcommand(_T("ReportCenter_StaticUD.dll"),_T("Log_ForSPU"),vectLogParameter);
#else
	CFunctor Logcommand(_T("ReportCenter_StaticMD.dll"),_T("Log_ForSPU"),vectLogParameter);
#endif
#else
#ifdef _UNICODE
	CFunctor Logcommand(_T("ReportCenter_StaticU.dll"),_T("Log_ForSPU"),vectLogParameter);
#else
	CFunctor Logcommand(_T("ReportCenter_StaticM.dll"),_T("Log_ForSPU"),vectLogParameter);
#endif
#endif

#endif //#ifdef MFCShare
	Logcommand();
}

DWORD WINAPI CSPU::ThreadProc( LPVOID lpParam ) 
{
	CSPU *pThis = (CSPU *)lpParam;

	vector<CFunctorParameter> vectParameter;
	CFunctor* pCommand = new CFunctor(pThis->m_strThreadAPIDLL,pThis->m_strThreadFunction,vectParameter,pThis->m_pThreadCallBack); 	
	pThis->m_nThreadRet = pCommand->run() ? SPU_SUCCESS : SPU_FAIL;
	delete pCommand;
	return 0;
}

SPU_RESULT CSPU::ExecuteThread(const TSTRING& strAPIDLL,const TSTRING& strFunction,LPISPU_CALLBACK pCallBack)
{
	DWORD nThreadID = 0;
	m_strThreadAPIDLL= strAPIDLL;
	m_strThreadFunction= strFunction;
	m_pThreadCallBack= pCallBack;
	m_hThread = ::CreateThread(NULL,0,ThreadProc,(LPVOID)this,0,&nThreadID);
	using namespace QSYNC_LIB;
	MessageLoop(&m_hThread,1);
	return m_nThreadRet;
}

SPU_RESULT CSPU::ExecuteScript(const TSTRING& lpszScript,LPISPU_CALLBACK pCallBack,bool bEnableTRACE_F11)
{
	SPU_RESULT nRet = SPU_SUCCESS;
	vector<TSTRING> strMemory;

	if(_taccess(lpszScript.c_str(), 0) == -1) //file not exist
	{
		if(pCallBack)
			pCallBack->Print(_T(""),_T("Script file path error!\n"));
		nRet = SPU_FAIL;
	}
	else
	{
		CStdioFileEx cFile;
		if(cFile.Open(lpszScript.c_str(), CStdioFile::modeRead))
		{
			nRet = LoadCommand(cFile,strMemory) ? SPU_SUCCESS : SPU_FAIL;
			if(nRet == SPU_FAIL && pCallBack)
				pCallBack->Print(_T(""),_T("Load Scription Failed!\n"));
			cFile.Close();
		}
		else
		{
			if(pCallBack)
				pCallBack->Print(_T(""),_T("Open Script file error!\n"));
			nRet = SPU_FAIL;
		}
	}

	if(nRet == SPU_SUCCESS)
	{		
		nRet = Run(strMemory,bEnableTRACE_F11,lpszScript);
	}

	return nRet;
}

bool CSPU::LoadCommand(CStdioFileEx& cFile,vector<TSTRING>& strMemory)
{
	bool bCodeFound = false, bCodeFinished = false;	

	while(!bCodeFound)
	{
		CString	instring;
		cFile.ReadString(instring);
		if(instring == _T("{Code"))
			bCodeFound = true;
	}

	while( bCodeFound && !bCodeFinished)
	{
		CString	instring;
		cFile.ReadString(instring);
		int nIndex = instring.Find(_T("//"), 0);

		TSTRING	outstring = _T("......................................................................................................................................................................");
		if(nIndex != -1)
			outstring = instring.Left(nIndex);
		else
			outstring = instring;

		if(outstring == _T("}"))
			bCodeFinished = true;
		else if(outstring == _T(""))
			continue;
		else
			strMemory.push_back(outstring);
	}

	return true;
}