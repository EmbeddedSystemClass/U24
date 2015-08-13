#include "StdAfx.h"
#include "BTWLAN_2G4_TxTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CBTWLAN_2G4_Test)

CBTWLAN_2G4_Test::CBTWLAN_2G4_Test(void)
{
}

CBTWLAN_2G4_Test::~CBTWLAN_2G4_Test(void)
{
}

bool CBTWLAN_2G4_Test::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("Input") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Input for CBTWLAN_2G4_Test");
		return false;
	}
	m_strInput = paramMap["Input"].c_str();

	return true;
}

bool CBTWLAN_2G4_Test::Run(void)
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	return bRes;
}

bool CBTWLAN_2G4_Test::MainFunction(void)
{
	if (!m_pIPhone->Nordic2G4_Consumer("1"))
	{
		m_strMsg = "Fail to set 2G4 consumer on";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->Nordic2G4_PowerMode("1"))
	{
		m_strMsg = "Fail to set 2G4 power up";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	char szInput[128] = {0};
	strcpy(szInput, m_strInput.c_str());
	if (!m_pIPhone->Nordic2G4_PowerOn(szInput))
	{
		m_strMsg = "Fail to start 2G4 power";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	int ret = MessageBox(NULL, _T("Do you want to stop 2G4 Tx?"), _T("Info."), 
				MB_ICONQUESTION | MB_OK);
	if (ret == 1)
	{
		// Stop 2G4 power
		if (!m_pIPhone->Nordic2G4_PowerMode("0"))
		{
			m_strMsg = "Fail to set 2G4 power down";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (!m_pIPhone->Nordic2G4_Consumer("0"))
		{
			m_strMsg = "Fail to set 2G4 consumer off";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	return true;
}