#include "stdafx.h"
#include "CheckNVs.h"


RF_IMPLEMENT_DYNCREATE(CCheckNVs)

bool CCheckNVs::PreRun()
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

bool CCheckNVs::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CCheckNVs::PostRun()
{
	return true;
}

bool CCheckNVs::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckNVs' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("NVLists")) == paramMap.end())
	{
		TraceLog(MSG_INFO, "Failed to find 'NVLists' for class 'CCheckNVs'");
		return false;
	}
	
	std::vector<std::string> vCheckNVs;
	STDStringTokenizer(paramMap["NVLists"], ",", vCheckNVs);
	if(vCheckNVs.size() != 3)
	{
		TraceLog(MSG_INFO, "Failed to get 'NVLists' param for class 'CCheckNVs', size != 3");
		return false;
	}
	for(int i = 0; i < 3; i++)	
		m_iNVs[i] = atoi(vCheckNVs[i].c_str());

	return true;
}

bool CCheckNVs::MainFunction()
{
	int nNVsValue[3][2];

	TraceLog(MSG_INFO, "CHECK NVs START");

	for(int NVsIdex = 0; NVsIdex < 3; NVsIdex++)
	{
		BYTE* ucData = new unsigned char[2];
		for (int i = 0; i < 2; ++i)	ucData[i] = 0;

		if (!m_pIPhone->ReadNV_QMSL(m_iNVs[NVsIdex], ucData, 2))
		{
			m_strMessage = MakeStr("Fail to read NV#%d to phone with QMSL", m_iNVs[NVsIdex]);
			FactoryLog();
			m_strErrorCode = ERR_QLIB_DIAG_NV_READ_F;
			delete ucData;
			return false;
		}
		nNVsValue[NVsIdex][0] = ucData[0];
		nNVsValue[NVsIdex][1] = ucData[1];
		delete ucData;
	}

	for(int i = 0; i < 3; i++)
	{
		m_strMessage = MakeStr("NV%d(%d) Value= %d, %d", i+1, m_iNVs[i], nNVsValue[i][0], nNVsValue[i][1]);
		FactoryLog();
	}

	// NV check mechanism: NV1 != NV2 == NV3
	if((nNVsValue[0][0] != nNVsValue[1][0]) || (nNVsValue[0][1] != nNVsValue[1][1]))
	{
		if((nNVsValue[1][0] == nNVsValue[2][0]) && (nNVsValue[1][1] == nNVsValue[2][1]))
		{
			SetPICSData("NVCheck_value", "PASS");
			m_strMessage = "Check NVs success";
			m_strResult = "PASS";
			TraceLog(MSG_INFO, m_strMessage);
			FactoryLog();
			return true;
		}
	}
	SetPICSData("NVCheck_value", "FAIL");
	m_strMessage = "Check NVs fail";
	m_strResult = "FAIL";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return false;
}
