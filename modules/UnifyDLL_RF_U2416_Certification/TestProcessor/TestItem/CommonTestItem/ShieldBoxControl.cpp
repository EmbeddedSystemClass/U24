#include "stdafx.h"
#include "ShieldBoxControl.h"


RF_IMPLEMENT_DYNCREATE(CShieldBoxControl)


bool CShieldBoxControl::Run()
{
	bool bRes = MainFunction();

	if (bRes)
		FactoryLog(true, "", "", "", "", "", "", "", "", "Shield Box Control PASS");
	else
		FactoryLog(false, "Shield Box Control", "", "", "", "", "", "", "", m_strMsg);

	return bRes;
}

bool CShieldBoxControl::MainFunction()
{
	char szErrMsg[256];
	memset(szErrMsg, 0, 256);

	return COMDeviceControl(const_cast<char*>(m_strDeviceName.c_str()), m_iBaudRate,
		const_cast<char*>(m_strCommand.c_str()), szErrMsg);
}

bool CShieldBoxControl::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("DeviceName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DeviceName for CShieldBoxControl");
		return false;
	}
	m_strDeviceName = paramMap["DeviceName"];

	if (paramMap.find("Command") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Command for CShieldBoxControl");
		return false;
	}
	m_strCommand = paramMap["Command"];

	if (paramMap.find("BaudRate") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter BaudRate for CShieldBoxControl");
		return false;
	}
	m_iBaudRate = atoi(paramMap["BaudRate"].c_str());

	return true;
}
