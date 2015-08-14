#include "stdafx.h"
#include "ValidateNTC.h"
#include <math.h>


using namespace InspectionUtility;
using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CValidateNTC)


bool CValidateNTC::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "NTC status", "", "", "", "", "", "", "", "Success to validate NTC status");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Check_Battery_Temperature_Fail;//<---Need to update error code!!

		FactoryLog(false, "NTC status", m_strErrCode, "", "", "", "", "", "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CValidateNTC::MainFunction()
{
	char szInputCmd[4096] = {0};
	memset(szInputCmd, 0, sizeof szInputCmd);

	char chReadNTC[4096] = {0};
	memset(chReadNTC, 0, sizeof chReadNTC);

	double dTemperature = 0.0;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	if (m_byteEnable & 0x01) // For NTC 1
	{
		sprintf_s(szInputCmd, 4096, "%s", "2;6;0;0;1");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadNTC, 4096))
		{
			m_strMsg = "Fail to get NTC 1 from FTD";
			TraceLog(m_strMsg);
			return false;
		}
		m_strNTC_1 = chReadNTC;

		m_dNTC1 = atof(m_strNTC_1.c_str());
		if (! InRange(m_dNTC_1_Lower, m_dNTC_1_Upper, m_dNTC1))
		{
			m_strMsg = MakeSTDstring("NTC 1 status(%.3f) not in range(%.3f,%.3f)", m_dNTC1, m_dNTC_1_Lower, m_dNTC_1_Upper);
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMsg = MakeSTDstring("NTC 1 status is %.2f", m_dNTC1);
			TraceLog(m_strMsg);
		}
	}
	

	if (m_byteEnable & 0x02) // For NTC 2
	{
		sprintf_s(szInputCmd, 4096, "%s", "2;6;0;0;2");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadNTC, 4096))
		{
			m_strMsg = "Fail to get NTC 2 from FTD";
			TraceLog(m_strMsg);
			return false;
		}
		m_strNTC_2 = chReadNTC;

		m_dNTC2 = atof(m_strNTC_2.c_str());
		if (! InRange(m_dNTC_2_Lower, m_dNTC_2_Upper, m_dNTC2))
		{
			m_strMsg = MakeSTDstring("NTC 2 status(%.3f) not in range(%.3f,%.3f)", m_dNTC2, m_dNTC_2_Lower, m_dNTC_2_Upper);
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMsg = MakeSTDstring("NTC 2 status is %.2f", m_dNTC2);
			TraceLog(m_strMsg);
		}
	}
	

	if (m_byteEnable & 0x04) // For NTC 3
	{
		sprintf_s(szInputCmd, 4096, "%s", "2;6;0;0;3");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadNTC, 4096))
		{
			m_strMsg = "Fail to get NTC 3 from FTD";
			TraceLog(m_strMsg);
			return false;
		}
		m_strNTC_3 = chReadNTC;

		m_dNTC3 = atof(m_strNTC_3.c_str());
		if (! InRange(m_dNTC_3_Lower, m_dNTC_3_Upper, m_dNTC3))
		{
			m_strMsg = MakeSTDstring("NTC 3 status(%.3f) not in range(%.3f,%.3f)", m_dNTC3, m_dNTC_3_Lower, m_dNTC_3_Upper);
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMsg = MakeSTDstring("NTC 3 status is %.2f", m_dNTC3);
			TraceLog(m_strMsg);
		}
	}
	

	if (m_byteEnable & 0x08) // For NTC 4
	{
		sprintf_s(szInputCmd, 4096, "%s", "2;6;0;0;4");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadNTC, 4096))
		{
			m_strMsg = "Fail to get NTC 4 from FTD";
			TraceLog(m_strMsg);
			return false;
		}
		m_strNTC_4 = chReadNTC;

		m_dNTC4 = atof(m_strNTC_4.c_str());
		if (! InRange(m_dNTC_4_Lower, m_dNTC_4_Upper, m_dNTC4))
		{
			m_strMsg = MakeSTDstring("NTC 4 status(%.3f) not in range(%.3f,%.3f)", m_dNTC4, m_dNTC_4_Lower, m_dNTC_4_Upper);
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMsg = MakeSTDstring("NTC 4 status is %.2f", m_dNTC4);
			TraceLog(m_strMsg);
		}
	}
	

	if (m_byteEnable & 0x10) // For NTC 5
	{
		sprintf_s(szInputCmd, 4096, "%s", "2;6;0;0;5");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadNTC, 4096))
		{
			m_strMsg = "Fail to get NTC 5 from FTD";
			TraceLog(m_strMsg);
			return false;
		}
		m_strNTC_5 = chReadNTC;

		m_dNTC5 = atof(m_strNTC_5.c_str());
		if (! InRange(m_dNTC_5_Lower, m_dNTC_5_Upper, m_dNTC5))
		{
			m_strMsg = MakeSTDstring("NTC 5 status(%.3f) not in range(%.3f,%.3f)", m_dNTC5, m_dNTC_5_Lower, m_dNTC_5_Upper);
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMsg = MakeSTDstring("NTC 5 status is %.2f", m_dNTC5);
			TraceLog(m_strMsg);
		}
	}

	return true;
}

bool CValidateNTC::InitData(std::map<std::string, std::string>& paramMap)
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
	std::string strTemp;
	std::vector<std::string> vTmp;
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	strTemp =  myParm_node.child("NTCEnableList").text().as_string(ERROR_STRING);
	CHECK_PARM("NTCEnableList", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	int iSum = 0;
	for (unsigned int icount = 0; icount < vTmp.size(); icount++)
	{
		if (atoi(vTmp[icount].c_str()) == 1)
			iSum += (int)pow(2.0, (int)icount);
	}
	m_byteEnable = (BYTE)iSum;

	// Get NTC_1_Range
	strTemp =  myParm_node.child("NTC_1_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("NTC_1_Range", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter <NTC_1_Range> for CValidateNTC");
		return false;
	}
	m_dNTC_1_Lower = atof(vTmp[0].c_str());
	m_dNTC_1_Upper = atof(vTmp[1].c_str());

	// Get NTC_2_Range
	strTemp =  myParm_node.child("NTC_2_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("NTC_2_Range", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter <NTC_2_Range> for CValidateNTC");
		return false;
	}
	m_dNTC_2_Lower = atof(vTmp[0].c_str());
	m_dNTC_2_Upper = atof(vTmp[1].c_str());

	// Get NTC_3_Range
	strTemp =  myParm_node.child("NTC_3_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("NTC_3_Range", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter <NTC_3_Range> for CValidateNTC");
		return false;
	}
	m_dNTC_3_Lower = atof(vTmp[0].c_str());
	m_dNTC_3_Upper = atof(vTmp[1].c_str());

	// Get NTC_4_Range
	strTemp =  myParm_node.child("NTC_4_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("NTC_4_Range", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter <NTC_4_Range> for CValidateNTC");
		return false;
	}
	m_dNTC_4_Lower = atof(vTmp[0].c_str());
	m_dNTC_4_Upper = atof(vTmp[1].c_str());

	// Get NTC_5_Range
	strTemp =  myParm_node.child("NTC_5_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("NTC_5_Range", strTemp, ERROR_STRING);
	STDStringTokenizer(strTemp, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter <NTC_5_Range> for CValidateNTC");
		return false;
	}
	m_dNTC_5_Lower = atof(vTmp[0].c_str());
	m_dNTC_5_Upper = atof(vTmp[1].c_str());

	return true ;
}
