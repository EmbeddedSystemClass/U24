#include "stdafx.h"
#include "Super4Relay.h"

#include "stdio.h"


Super4Relay::Super4Relay()
{
	m_hRBDLL = NULL;
	m_pFnListRelays = NULL;
	m_pFnDLLVersion = NULL;
	m_pFnSetRelays = NULL;

	for (int i = 0; i < RB_COUNT; ++i)
	{
		memset(m_szRBName[i], 0, 20);
	}

	m_iRBCount = 0;
}

Super4Relay::~Super4Relay()
{
	if (m_hRBDLL)
		FreeLibrary(m_hRBDLL);
}


bool Super4Relay::InitialRB()
{
	if (! m_hRBDLL)
	{
		// Load DLL
		m_hRBDLL = LoadLibraryA(_T("super4.dll"));
		if (! m_hRBDLL)
			return false;

		// Get API
		m_pFnListRelays = (pFnListRelays)GetProcAddress(m_hRBDLL, "ListRelays");
		m_pFnSetRelays = (pFnSetRelays)GetProcAddress(m_hRBDLL, "SetRelays");
		m_pFnDLLVersion = (pFnDLLVersion)GetProcAddress(m_hRBDLL, "dllversion");

		if (m_pFnListRelays == NULL || m_pFnSetRelays  == NULL || m_pFnDLLVersion == NULL)
		{
			FreeLibrary(m_hRBDLL);
			m_hRBDLL = NULL;
			return false;
		}

		m_iRBCount = m_pFnListRelays(m_szRBName);
		if (m_iRBCount <= 0)
			return false;
	}

	return true;
}

int Super4Relay::GetRBCount()
{
	return m_iRBCount;
}

int Super4Relay::GetDLLVersion()
{
	return m_pFnDLLVersion();
}

bool Super4Relay::SetRB(int iRBNumber, int iSlot, bool bOn)
{
	char szRBName[20];
	memset(szRBName, 0, 20);
	unsigned char mask = 0;
	unsigned char set = 0;

	if (iRBNumber > RB_COUNT || iRBNumber > m_iRBCount || m_iRBCount <= 0)
		return false;

	if (strcmp(m_szRBName[iRBNumber], "") == 0)
		return false;
	else
		sprintf_s(szRBName, 19, "%s", m_szRBName[iRBNumber]);

	switch (iSlot)
	{
	case 1:
		mask = 1;
		break;
	case 2:
		mask = 2;
		break;
	case 3:
		mask = 4;
		break;
	case 4:
		mask = 8;
		break;
	default:
		return false;
	}

	if (bOn)
		set = 1;

	if (m_pFnSetRelays(szRBName, mask, set) < 0)
		return false;

	return true;
}

bool Super4Relay::InitSetting()
{
	if (!InitialRB())
	{
		return false;
	}
	if (m_bInitOnOff)
	{
		for (int i=1; i< 5; i++)
		{
			SetRB(m_iRBNumber,i,m_b_onoff_slot[i]);
		}	
	}
	return true;
}

bool Super4Relay::InitData( std::map<std::string, std::string>& paramMap )
{
	std::string strTmp;

	/* RelayBoard_Control */
	StrVtr vtrstr;
	if (paramMap.find(std::string("RelayBoard_Control")) != paramMap.end())
	{
		strTmp = paramMap[std::string("RelayBoard_Control")];	
		vtrstr.clear();
		CStr::ParseString(strTmp, _T(","), vtrstr);
		if (vtrstr.size() != 5)
		{
			return false;	
		}

		m_iRBNumber = CStr::StrToInt(vtrstr[0]);
		m_b_onoff_slot[0] = 0;
		m_b_onoff_slot[1] = CStr::StrToBool(vtrstr[1]);
		m_b_onoff_slot[2] = CStr::StrToBool(vtrstr[2]);
		m_b_onoff_slot[3] = CStr::StrToBool(vtrstr[3]);
		m_b_onoff_slot[4] = CStr::StrToBool(vtrstr[4]);
	}
	else
	{
		return false;
	}

	// initial On off
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		strTmp = paramMap[std::string("OnOff")];
		if (strTmp.compare("TRUE") == 0)
		{
			m_bInitOnOff = true;
		}
		else
		{
			m_bInitOnOff = false;
		}	
	}

	return true;
}




