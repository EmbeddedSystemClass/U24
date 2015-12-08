#include "StdAfx.h"
#include "RelayBoard.h"
#include "Shlwapi.h"
#include "stdio.h"

CRelayBoard::CRelayBoard(void)
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

CRelayBoard::~CRelayBoard(void)
{
	if (m_hRBDLL)
	{
		FreeLibrary(m_hRBDLL);
	}
}


bool CRelayBoard::InitialRB()
{
	if (! m_hRBDLL)
	{
		m_hRBDLL = LoadLibrary(_T("super4.dll"));
		if (! m_hRBDLL)
		{
			return false;
		}

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
		{
			return false;
		}
	}

	return true;
}

int CRelayBoard::GetRBCount()
{
	return m_iRBCount;
}

int CRelayBoard::GetDLLVersion()
{
	return m_pFnDLLVersion();
}


bool CRelayBoard::SetRB(int iRBNumber, int iSlot, bool bOn)
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

bool CRelayBoard::IsInit()
{
	if (m_hRBDLL == NULL)
	{
		return false;
	}

	return true;
}

bool CRelayBoard::SetRBAll( int iRBNumber, bool bOn )
{
	for (int i = 1; i< 5; i++)
	{
		if (!SetRB(iRBNumber,i,bOn))
		{
			return false;
		}
	}
	return true;
}
