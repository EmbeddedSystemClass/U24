#include "stdafx.h"
#include "CheckHWVariant.h"


RF_IMPLEMENT_DYNCREATE(CCheckHWVariant)


bool CCheckHWVariant::PreRun()
{
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CCheckHWVariant::Run()
{
	if (! m_pIPhone->Initial())
	{
		SetItemError("m_pIPhone->Initial() fail");
		return false;
	}

	CString resultString;

	// compare :
	// 1. DIAG ReadHWVersion
	// 2. Config file
	// 3. NV148 and compare

	// read HW version
	char szVariant[256];
	char szMemory[256];
	char szEvt[256];
	char szEvtMin[256];
	char szPR[256];

	if (! m_pIPhone->ReadHWVersion(szVariant, szMemory, szEvt, szEvtMin, szPR))
	{
		SetItemError("Read HW Version fail");
		m_strErrorCode = READ_HWVERSION_FAIL;
		return false;
	}
	resultString.Format("%s.%s.%s.%s.%s", szVariant, szMemory, szEvt, szEvtMin, szPR);

	// Compare with config file
	std::string curVar;
	int curQCN;
	switch (atoi(szVariant))
	{
		case 1:	
			curVar = "US"; 
			curQCN = 30;	
			break;
		case 2:
			curVar = "EU"; 
			curQCN = 31;	
			break;
		default:
			return false;
	}

	if (m_pIPhone->m_strVariant != curVar)
	{
		SetItemError("Mismatch HW Version");
		return false;
	}

	// Read NV148
	int qcnValue;
	if (! m_pIPhone->ReadNVItem(148, (BYTE*)&qcnValue, sizeof(int)))
	{
		SetItemError("Read NV148 fail");
		return false;
	}
	resultString.AppendFormat("  --  %d", qcnValue);

	bool ret = curQCN == qcnValue;
	if (!ret)
	{
		m_strMessage = "QCN (NV148) and variant not match";
		SetItemError (m_strMessage.c_str());
	}
	else
	{
		m_strMessage = "CheckHWVariant success";
		TraceLog(MSG_INFO, m_strMessage);
	}

	m_strMeasured = resultString;
	return ret;

}

bool CCheckHWVariant::PostRun()
{
	TraceLog(MSG_INFO, "Check HW variant PASS");
	return true;
}

bool CCheckHWVariant::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

void CCheckHWVariant::SetItemError(const char* msg)
{
	m_strErrorCode = HWVERSION_ERROR;
	m_strMessage = msg;

	std::string strLog = "Fail to check HW variant by ";
	strLog += msg;
	TraceLog(MSG_ERROR, strLog);
}
