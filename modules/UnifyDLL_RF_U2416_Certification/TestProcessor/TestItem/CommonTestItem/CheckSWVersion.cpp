#include "stdafx.h"
#include "CheckSWVersion.h"


RF_IMPLEMENT_DYNCREATE(CCheckSWVersion)


bool CCheckSWVersion::Run()
{
	bool bRes = MainFunction();

	if (bRes)
		FactoryLog(true, "CheckSWVersion", "", "", "", "", m_strSWVerList, m_strSWversion, "", "Check SW version PASS");
	else
		FactoryLog(false, "CheckSWVersion", CommErr_Check_Software_Version_Fail, "", "", "", m_strSWVerList, m_strSWversion, "", m_strMsg);

	return bRes;
}

bool CCheckSWVersion::MainFunction()
{
	char szVersion[512]={0};
	char szTmp[256]={0};

	// Read version from mobile
	memset(szVersion, 0, 512);
	if (! m_pIPhone->ExecAdbCommand("shell getprop ro.build.oemversion.main", szVersion, szTmp))
	{
		m_strMsg = "Fail to read SW version from phone";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Filter "\r", "\n" in tail
	for(int i = 0; i < 512; i++)
	{
		if((szVersion[i] == 0x0A) || (szVersion[i] == 0x0D))
			szVersion[i] = 0x00;
	}
	m_strSWversion = std::string(szVersion);
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "Fetched SW version : %s", szVersion);
	TraceLog(MSG_INFO, szTmp);

	// SetPicsData and get current version for comparison
	char SWver[512];
	if(m_nType == 0) {
		SetPICSDate("FactoryVersion", m_strSWversion);
		GetFactoryVersion(SWver);
	}else {
		SetPICSDate("SWVersion", m_strSWversion);
		GetSWVersion(SWver);
	}

	// Compare multi-version
	m_strSWVerList = std::string(SWver);
	std::vector<std::string> vToken;
	STDStringTokenizer(m_strSWVerList, ",", vToken);
	bool b_match = false;
	for(int i = 0; i < (signed)vToken.size(); i++)
	{
		if(!(strstr(szVersion, vToken[i].c_str()) == NULL))
		{
			b_match = true;
			break;
		}
	}

	if(!b_match)
	{
		m_strMsg = "SW version mismatch:" + m_strSWVerList;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;

	}

	return true;
}


bool CCheckSWVersion::InitData(std::map<std::string, std::string>& paramMap)
{
	// Type
	if (paramMap.find(std::string("RequiredType")) != paramMap.end())
	{
		m_nType = atoi(paramMap[std::string("RequiredType")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'RequiredType' for class 'CCheckSWVersion'"));
		return false;
	}
	
	return true;
}
