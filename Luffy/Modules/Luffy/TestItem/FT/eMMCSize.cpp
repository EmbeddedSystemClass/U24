#include "stdafx.h"
#include "eMMCSize.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CEMMCSize)


bool CEMMCSize::Run(int iItem)
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "eMMCSize", "", "", "", "", "", m_strMeasured, "", "eMMC size test PASS");
		SetPICSData("eMMCSizeTest", "PASS");
		SetPICSData("eMMCSizeTestValue", m_strMeasured);
	}
	else
	{
		FactoryLog(false, "eMMCSize", FunErr_Check_eMMC_Fail, "", "", "", "", m_strMeasured, "Sector", m_strMsg);
		SetPICSData("eMMCSizeTest", "FAIL");
		SetPICSData("eMMCSizeTestValue", m_strMeasured);
	}

	return bRes;
}

bool CEMMCSize::MainFunction()
{
	int iSize;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->eMMCSize(&iSize))
	{
		m_strMsg = "Fail to read eMMC size with Qisda module";
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
		m_strMsg = MakeSTDstring("eMMC size: %s is correct.", m_strMeasured.c_str());
	else
		m_strMsg = MakeSTDstring("eMMC size: %s is incorrect.", m_strMeasured.c_str());

	TraceLog(m_strMsg);

	return bFound;
}


bool CEMMCSize::InitData(std::map<std::string, std::string>& paramMap)
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
