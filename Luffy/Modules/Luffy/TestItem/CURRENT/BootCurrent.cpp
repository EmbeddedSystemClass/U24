#include "stdafx.h"
#include "BootCurrent.h"

using namespace StringUtility;
using namespace InspectionUtility;

GP_IMPLEMENT_DYNCREATE(CBootCurrent)

CBootCurrent::CBootCurrent(void)
: m_iWaitTimeForShutdown(0)
 ,m_iWaitTimeForBoot(0)
 ,m_iMeasureCount(0)
 ,m_iWaitTimeAfterBoot(0)
 ,m_dLower(0.0)
 ,m_dUpper(0.0)
 ,m_pCGProc(NULL)
 ,m_pIPS(NULL)
 ,m_pIPS2(NULL)
{
}

CBootCurrent::~CBootCurrent(void)
{
}


bool CBootCurrent::Run(int iItem)
{
	m_pIPS = m_pCGProc->m_pIPS;

	m_pIPS2 = m_pCGProc->m_pIPS2;

	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if (m_pIPS == NULL)
	{
		FactoryLog(false, "", CommErr_PowerSupply_Control_Fail, "", "", szLower, szUpper, m_strMeasured, "mA", "Power supply not detected");
		SetPICSData("BootCurrent", "FAIL");
		SetPICSData("BootCurrentValue", "0.000");

		return false;
	}

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", szLower, szUpper, m_strMeasured, "mA", "Boot current PASS");
		SetPICSData("BootCurrent", "PASS");
		SetPICSData("BootCurrentValue", m_strMeasured);
	}
	else
	{
		FactoryLog(false, "", FunErr_Boot_Current_Test_Fail, "", "", szLower, szUpper, m_strMeasured, "mA", m_strMsg);
		SetPICSData("BootCurrent", "FAIL");
		SetPICSData("BootCurrentValue", m_strMeasured);
	}

	m_pIPS->SetCurrRangeMax();
	m_pIPS->SetPSOnOff(true);
	m_pIPS2->SetPSOnOff(true);

	return bRes;
}

bool CBootCurrent::MainFunction()
{
	if (! m_pIPS->SetPSOnOff(false))
	{
		m_strMsg = "Fail to set power supply off";
		TraceLog(m_strMsg);
		return false;
	}
	
	if (! m_pIPS2->SetPSOnOff(false))
	{
		m_strMsg = "Fail to set power supply2 off";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeForShutdown);

	if (! m_pIPS->SetPSOnOff(true))
	{
		m_strMsg = "Fail to set power supply on";
		TraceLog(m_strMsg);
		return false;
	}
	
	if (! m_pIPS2->SetPSOnOff(true))
	{
		m_strMsg = "Fail to set power supply2 on";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeForBoot);

	if (! m_pIPS2->SetPSOnOff(false))
	{
		m_strMsg = "Fail to set power supply2 off";
		TraceLog(m_strMsg);
		return false;
	}

	double dSum = 0.0;
	int iCounter = 0;
	do
	{
		double dCurrent = 0.0;

		if (! m_pIPS->GetCurrent(dCurrent))
		{
			m_strMsg = "Fail to get current value form power supply";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			dCurrent *= 1000.0;

			char szTmp[256];
			sprintf_s(szTmp, sizeof(szTmp), "%03d --> %.3f mA", iCounter, dCurrent);
			TraceLog(szTmp);

			dSum += dCurrent;
		}

		Sleep(1000);
		++iCounter;
	} while (iCounter < m_iMeasureCount);

	dSum /= m_iMeasureCount;

	char szMeasured[256];
	memset(szMeasured, 0, 256);
	sprintf_s(szMeasured, 255, "%.3f", dSum);
	m_strMeasured = szMeasured;

	if (! InRange(m_dLower, m_dUpper, dSum))
	{
		m_strMsg = "Boot current not in range";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeAfterBoot);

	return true;
}


bool CBootCurrent::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);

	std::vector<std::string> vTmp;
	std::string szString;


	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get Parameter

	m_iWaitTimeForShutdown =  myParm_node.child("WaitTimeForShutdown").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForShutdown", m_iWaitTimeForShutdown, ERROR_INT);

	m_iWaitTimeForBoot =  myParm_node.child("WaitTimeForBoot").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForBoot", m_iWaitTimeForBoot, ERROR_INT);

	m_iMeasureCount =  myParm_node.child("MeasureCount").text().as_int(ERROR_INT);
	CHECK_PARM("MeasureCount", m_iMeasureCount, ERROR_INT);

	m_iWaitTimeAfterBoot =  myParm_node.child("WaitTimeAfterBoot").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeAfterBoot", m_iWaitTimeAfterBoot, ERROR_INT);

	szString =  myParm_node.child("Range").text().as_string(ERROR_STRING);
	CHECK_PARM("Range", szString, ERROR_STRING);

	STDStringTokenizer(szString, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter Range for CBootCurrent");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
