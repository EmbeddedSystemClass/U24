/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CHeadsetInfo object.
 * FileName   : HeadsetInfo.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/01/16    Evonne.Hsieh          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "HeadsetInfo.h"


RF_IMPLEMENT_DYNCREATE(CHeadsetInfo)

bool CHeadsetInfo::PreRun()
{
	m_strItemCode = CStr::IntToStr(Audio_Headset_BaseItemCode);
	m_strErrorCode = FunErr_Headset_Info_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CHeadsetInfo::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CHeadsetInfo::Run()
{
	bool bRes = false;
	bRes = HeadsetInfo();

	return bRes;
}

bool CHeadsetInfo::PostRun()
{
	return true;
}

bool CHeadsetInfo::HeadsetInfo()
{
	bool b_res = false;
	std::string str_msg;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_AUheadsetInfo(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = "Headset Info Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Headset Info pass!";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Headset Info fail";
	}
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return b_res;
}
