#include "stdafx.h"
#include "DDRSize.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CDDRSize)


bool CDDRSize::Run(int iItem)
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "DDRSize", "", "", "", "", "", m_strMeasured, "", "DDR size test PASS");
		SetPICSData("DDRSizeTest", "PASS");
		SetPICSData("DDRSizeTestValue", m_strMeasured);
	}
	else
	{
		FactoryLog(false, "DDRSize", FunErr_DDR_Size_Fail, "", "", "", "", m_strMeasured, "", m_strMsg);
		SetPICSData("DDRSizeTest", "FAIL");
		SetPICSData("DDRSizeTestValue", m_strMeasured);
	}

	return bRes;
}

bool CDDRSize::MainFunction()
{
	int iSize;

	char szInputCmd[4096] = {0};
	memset(szInputCmd, 0, sizeof szInputCmd);

	char chReadID[4096] = {0};
	memset(chReadID, 0, sizeof chReadID);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	sprintf_s(szInputCmd, 4096, "%s", "3;2");
	if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadID, 4096))
	{
		m_strMsg = "Fail to get DDR ID with FTD";
		TraceLog(m_strMsg);
		return false;
	}
	else
	{
		m_strMsg = MakeSTDstring("DDR ID is %s ", std::string(chReadID).c_str());
		TraceLog(m_strMsg);
	}

	if (! m_pIPhone->DDRSize(&iSize))
	{
		m_strMsg = "Fail to read DDR size with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	bool bFound = false;
	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", iSize);
	m_strMeasured = szTmp;

	for (int iCounter = 0; iCounter < m_iMaterialOptions; iCounter++)
	{
		if (m_strMeasured == m_vecSizeType.at(iCounter))
		{
			bFound = true;
			break;
		}
	}

	if (bFound)
		m_strMsg = MakeSTDstring("DDR size: %s is correct.", m_strMeasured.c_str());
	else
		m_strMsg = MakeSTDstring("DDR size: %s is incorrect.", m_strMeasured.c_str());

	TraceLog(m_strMsg);

	return bFound;
}


bool CDDRSize::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSpeakerTest");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	std::string strSizeType;
	strSizeType =  myParm_node.child("SizeType").text().as_string(ERROR_STRING);
	CHECK_PARM("SizeType", strSizeType, ERROR_STRING);
	STDStringTokenizer(strSizeType, ",", m_vecSizeType);
	m_iMaterialOptions = (signed)m_vecSizeType.size();
	if ( m_iMaterialOptions < 1 )
	{
		TraceLog("Fail to find parameter SizeType for CDDRSize");
		return false;
	}

	return true;
}
