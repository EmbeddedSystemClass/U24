#include "stdafx.h"
#include "EnableFSG.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)

RF_IMPLEMENT_DYNCREATE(CEnableFSG)


bool CEnableFSG::Run()
{
	bool bRes = runEnableFSG();

	if (bRes)
	{
		m_strMsg = "runEnableFSG PASS";
		FactoryLog(true, "runEnableFSG", "--", "--", "--", "--", "--", m_strMeasured, "--", m_strMsg.c_str());
	}
	else
	{
		m_strMsg = "runEnableFSG Fail";
		FactoryLog(false, "--", FunErr_FSG_Enable_Protect_Fail, "--", "--", "--", "--", m_strMeasured, "--", m_strMsg.c_str());
	}
	
	return bRes;
}

bool CEnableFSG::runEnableFSG()
{
	bool b_Res = false;
	char sz_Input[FTD_BUF_SIZE] = {0};
	char sz_Output[FTD_BUF_SIZE] = {0};	
	
	strncpy(sz_Input, m_strEnable.c_str(), m_strEnable.length());

	if (! (b_Res = m_pIPhone->FSG_Enable(sz_Input, sz_Output)))
	{
		m_strMsg = "Execute EnableFSG Fail";
		TraceLog(MSG_ERROR, m_strMsg);
		//continue;
	}

		/* Compare FMRrssi with config file */
	m_strMeasured = sz_Output;

	return b_Res;
}



bool CEnableFSG::InitData(std::map<std::string, std::string>& paramMap)
{
	//if (paramMap.find ("RequiredMode") == paramMap.end())
	//{
	//	TraceLog(MSG_ERROR, "Fail to find parameter RequiredMode for CChangeBootMode");
	//	return false;
	//}
	//m_iRequiredMode = atoi(paramMap["Enable"].c_str());

	if (paramMap.find("Enable") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Enable for CEnableFSG");
		return false;
	}
	m_strEnable = paramMap["Enable"];

	
	return true;
}
