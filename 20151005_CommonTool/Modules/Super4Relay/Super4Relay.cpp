#include "stdafx.h"
#include "Super4Relay.h"

#include "Shlwapi.h"
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
		char szPath[256];
		char szDLLPathName[256];
		memset(szPath, 0, 256);
		memset(szDLLPathName, 0, 256);

		GetModuleFileNameA(NULL, szPath, 255);
		TrancateFilePath(szPath);

		// Load DLL
		sprintf_s(szDLLPathName, 255, "%s\\super4.dll", szPath);
		m_hRBDLL = LoadLibraryA(szDLLPathName);
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

void Super4Relay::TrancateFilePath(char* szFilePath)
{
	int iLength = strlen(szFilePath);

	if (iLength <= 0)
		return;

	for (int i = iLength - 1; i >= 0; --i)
	{
		if (szFilePath[i] == '\\')
		{
			szFilePath[i] = '\0';
			break;
		}
	}
}
