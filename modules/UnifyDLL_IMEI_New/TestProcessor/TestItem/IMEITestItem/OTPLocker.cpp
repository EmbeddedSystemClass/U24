#include "stdafx.h"
#include "OTPLocker.h"


RF_IMPLEMENT_DYNCREATE(COTPLocker)

bool COTPLocker::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool COTPLocker::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool COTPLocker::PostRun()
{
	return true;
}

bool COTPLocker::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'COTPLocker' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("OTPLock")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'OTPLock' for class 'COTPLocker'"));
		return false;
	}

	if(paramMap[std::string("OTPLock")].compare("TRUE") == 0)
		m_bOTPLock = true;
	else
		m_bOTPLock = false;


	return true;
}

bool COTPLocker::MainFunction()
{
	char sz_inData[FTD_BUF_SIZE] = {0}; 
	char sz_outData[FTD_BUF_SIZE] = {0};

	if(m_bOTPLock)
		strcpy(sz_inData, "on,0x16024958");
	else
		strcpy(sz_inData, "off,0xE9FDB6A7");

	m_strMessage = m_bOTPLock? "Write OTP Enable START" : "Write OTP Disable START";
	FactoryLog();

	for(int i = 1; i <= m_iRetry; i++)
	{
		if (m_pIPhone->FTD_FAC_OTPEnable(m_nFtdPort, m_nFtdTimeOut, sz_inData, sz_outData))	break;
		else
		{
			if(i == m_iRetry)
			{
				m_strMessage = m_bOTPLock? "Write OTP Enable fail....." : "Write OTP Disable fail.....";
				m_strResult = "FAIL";
				m_strErrorCode = CommErr_Write_IMEI_Fail;
				TraceLog(MSG_ERROR, m_strMessage);
				FactoryLog();
				return false;
			}
			m_strMessage = m_bOTPLock? "Write OTP Enable fail, retry again..." : "Write OTP Disable fail, retry again...";
			FactoryLog();
		}
	}
	m_strMessage = m_bOTPLock? "Write OTP Enable success....." : "Write OTP Disable success.....";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}