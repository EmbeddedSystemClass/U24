#include "stdafx.h"
#include "IPhone.h"
//#include "QLib_Defines.h"

GP_IMPLEMENT_DYNAMIC(IPhone)


IPhone::IPhone()
{
	m_iSlot=-1;

	m_strDeviceName="";
	m_strQisdaCommandModuleName="";
	m_strWorkingDir="";

	//Log
	m_pLog=NULL;
}


void IPhone::SetLog(CLogger* pLog)
{
	m_pLog = pLog;
}

void IPhone::CmdLog(std::string strMsg)
{
	std::string strHeader("PHONE_CMD   : ");
	std::string strLog = strHeader + strMsg;

	if( m_pLog )
	{
		m_pLog->CmdLog(strLog.c_str(), m_iSlot);
	}
}
