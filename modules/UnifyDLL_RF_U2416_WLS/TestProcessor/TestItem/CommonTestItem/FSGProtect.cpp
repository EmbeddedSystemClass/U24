#include "stdafx.h"
#include "FSGProtect.h"
//#include "../../Phone/Qisda_API.h"


RF_IMPLEMENT_DYNCREATE(CFSGProtect)


bool CFSGProtect::Run()
{
	char szOutput[FTD_BUF_SIZE] = {0};
	bool b_res = m_pIPhone->FTD_FAC_FSGProtect(m_nFtdPort, m_nFtdTimeOut, m_szCommand, szOutput);
	FactoryLog(b_res, "", "", "", "", "", "", "", "", b_res ? "FSG Protect command PASS" : "FSG Protect command FAIL");
	SetPICSData(m_strPICSName, b_res ? "PASS" : "FAIL");
	return b_res;
}

bool CFSGProtect::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("Command") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Command for CFSGProtect");
		return false;
	}
	strcpy(m_szCommand, paramMap["Command"].c_str());

	if (paramMap.find("PICSName") != paramMap.end())
	{
		m_strPICSName = paramMap["PICSName"];
	}
	else
	{
		m_strPICSName.clear();
	}
	return true;
}
