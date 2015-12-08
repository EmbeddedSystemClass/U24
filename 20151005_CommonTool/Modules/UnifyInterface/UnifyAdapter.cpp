#include "stdafx.h"
#include "UnifyAdapter.h"
#include <Shlwapi.h>

CUnifyAdapter::CUnifyAdapter() : cGlog(NULL)
{
	//Get module full path
	::GetModuleFileNameA(NULL, this->m_cstrModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	this->m_cstrModulePath.ReleaseBuffer();
	::PathRemoveFileSpecA(this->m_cstrModulePath.GetBuffer(MAX_PATH));
	this->m_cstrModulePath.ReleaseBuffer();
	
	//Create Glog instance
	this->cGlog = new CGLog;
	this->cGlog->InitializeGLog("D:\\Log\\DLL\\");

	this->m_vecLogFileName.resize(DevLast, "");

	//DefineNotifyEvent
	this->DefineUINotify();
}
CUnifyAdapter::~CUnifyAdapter()
{
	//Delete Glog instance
	if(NULL != this->cGlog)
	{
		this->cGlog->ShutdownGLog();
		delete this->cGlog;
		this->cGlog = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          INotifyBase_Char
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnifyAdapter::Register(INotify_Char* pNotify,const char* strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent);
}
bool CUnifyAdapter::Register(EventFunc_Char* pEventFunc,const char* strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent);
}

bool CUnifyAdapter::Unregister(INotify_Char* pNotify,const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent);
}
bool CUnifyAdapter::Unregister(EventFunc_Char* pEventFunc,const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent);
}

void CUnifyAdapter::DefineNotify(const char* strNewEvent)
{
	return CNotifyBase_Char::DefineNotify(strNewEvent);
}
void CUnifyAdapter::Fire(const char* strEvent,long nParam)
{
	return CNotifyBase_Char::Fire(strEvent, nParam);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          UnifyInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnifyAdapter::SetParameterValue(char* sz_keyword, char* sz_value)
{
	return true;
}

bool CUnifyAdapter::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	return true;
}

bool CUnifyAdapter::SetFAData(int i_slot, char* sz_value)
{
	return true;
}

bool CUnifyAdapter::GetFAData(int i_slot, char* sz_value, int i_size)
{
	return true;
}

bool CUnifyAdapter::GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	return true;
}

bool CUnifyAdapter::SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	return true;
}



int CUnifyAdapter::GetItemCount()
{
	return true;
}

bool CUnifyAdapter::GetItemByIndex(int i_index, char* sz_name, int i_size)
{
	return true;
}

bool CUnifyAdapter::Begin(int i_slot)
{
	return true;
}

bool CUnifyAdapter::End(int i_slot)
{
	return true;
}

bool CUnifyAdapter::PreRun(int i_slot)
{
	return true;
}

bool CUnifyAdapter::Run(int i_slot)
{
	return true;
}

bool CUnifyAdapter::PostRun(int i_slot)
{
	return true;
}

bool CUnifyAdapter::Stop(int i_slot)
{
	return true;
}

bool CUnifyAdapter::ResopnseToJig(bool b_wait)
{
	return true;
}

bool CUnifyAdapter::ResopnseToShowDlg(char* sz_showReslut)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnifyAdapter::Log(const char *szLog, const int Severity, const int i_slot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_Log),TRUE);

	if("" != this->m_vecLogFileName[i_slot - 1])
	{	
		if(NULL != szLog)
		{
			this->SetLogFullName(this->m_vecLogFileName[i_slot - 1].GetString());
			this->cGlog->Log(Severity, szLog);

			return true;
		}

		else
		{
			return false;
		}
	}

	else
	{
		return false;
	}	
}

bool CUnifyAdapter::SetLogFullName(const char *szLogName)
{
	CStringA cstrINFO(szLogName), cstrWARN(szLogName), cstrERRO(szLogName);

	cstrINFO.Append("_INFO");
	cstrWARN.Append("_WARN");
	cstrERRO.Append("_ERRO");

	this->cGlog->SetGLogDestination_Info(cstrINFO.GetString());
	this->cGlog->SetGLogDestination_Warning(cstrWARN.GetString());
	this->cGlog->SetGLogDestination_Error(cstrERRO.GetString());

	return true;
}

bool CUnifyAdapter::SetLogName(const char *szLogName, const int i_slot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_SetLogName),TRUE);
	
	this->m_vecLogFileName[i_slot - 1].Format("%s", szLogName);

	return true;
}

void CUnifyAdapter::DefineUINotify()
{
	this->DefineNotify(UI_PROGRESS);
	this->DefineNotify(UI_MESSAGE);
	this->DefineNotify(UI_PATTERN);
	this->DefineNotify(UI_ITEM);
	this->DefineNotify(UI_RESULT);
	this->DefineNotify(UI_PICSDATA);
	this->DefineNotify(UI_JIGCONTROL);
	this->DefineNotify(UI_SHOWDIALOG);
	this->DefineNotify(UI_RELAYBOARD);
}