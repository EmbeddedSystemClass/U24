#include "stdafx.h"
#include "NFCReaderTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CNFCReaderTest)

CNFCReaderTest::CNFCReaderTest()
	: m_WriteCPKLog(false)
{
}

bool CNFCReaderTest::Run()
{
	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "NfcCPK";
	gCpkRecord.setFileName (cpkfile.c_str());
	gCpkRecord.ResetAll();

	int count = m_CPKHeaderList.size();
	for (int i=0; i < count; ++i)
	{
		gCpkRecord.setHeader (i, m_CPKHeaderList[i].c_str());
	}

	bool bRes = CheckNFCReaderID();

	tmp_strMeasured.Replace(',', '-');

	if (bRes)
	{
		FactoryLog(true, "NFC_Reader_Test", "Pass", "", "", "", "", m_strMeasured, "", "NFC Reader Test PASS");
	}
	else
	{
		FactoryLog(false, "NFC_Reader_Test", FunErr_NFC_HCI_TypeA_Reader_Fail, "", "", "", "", m_strMeasured, "", m_strMessage);
	}
	gCpkRecord.assign ("NFC_TYPEA_READER_ID", tmp_strMeasured.GetBuffer(0)); //write NFC Reader ID to CPK
	tmp_strMeasured.ReleaseBuffer();
	SetPICSData(_T("NFC_TYPEA_READER"), m_strMeasured);

	// write result to CPK log
	if (m_WriteCPKLog)
	{
		gCpkRecord.assign ("SN", g_strPicasso);
		gCpkRecord.assign ("Result", ( bRes ? "PASS" : "FAIL"));

		SYSTEMTIME currTime;
		::GetLocalTime(&currTime);
		std::string strTime = MakeSTDstring ("%02d:%02d:%02d", currTime.wHour, currTime.wMinute, currTime.wSecond);
		gCpkRecord.assign ("Time", strTime);

		gCpkRecord.WriteRecordToFile();
	}

	return bRes;
}

bool CNFCReaderTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CNFCReaderTest::CheckNFCReaderID()
{
	char sz_Input[FTD_BUF_SIZE] = {0};
	char sz_Output[FTD_BUF_SIZE] = {0};

	if (!m_pIPhone->NFCHCITypeAReader(sz_Input, sz_Output))
	{
		m_strMessage = "Read NFC Type A Reader ID from mobile error";
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	/* Compare ID */
	m_strMeasured = sz_Output;
	tmp_strMeasured = m_strMeasured.c_str();

	if (strcmp (tmp_strMeasured.Trim(), "len=0,uid=0 0 0 0 0 0 0 0") != 0)
	{
		m_strMessage = "Check NFC Type A Reader ID success!";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strMessage = "NFC Type A Reader ID is not matched.";
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	return true;
}

bool CNFCReaderTest::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//NfcTestObjects//NfcCPKcolumn"));
	if (processObjectSet == NULL)
		return false;

	XMLNodeList processObjectsCategoryList = processObjectSet->GetchildNodes();
	if (processObjectsCategoryList == NULL)
		return false;

	int listsize = m_TestItemXML.GetNodeListSize(processObjectsCategoryList);
	for (int j = 0; j < listsize; ++j)
	{
		XMLNode processObject = m_TestItemXML.GetNodeListItem(processObjectsCategoryList, j);
		if (processObject == NULL)
			continue;
			
		// Parse the process object
		CString cstrName = m_TestItemXML.GetNodeAttrValue(processObject, _T("Name"));
		if (cstrName.IsEmpty())
			continue;
		CT2A szName(cstrName);
		std::string strName(szName);

		m_CPKHeaderList.push_back(strName);
	}

	return true;
}