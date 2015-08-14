#include "stdafx.h"
#include "GetCurrent.h"

using namespace StringUtility;
using namespace InspectionUtility;

GP_IMPLEMENT_DYNCREATE(CGetCurrent)

CGetCurrent::CGetCurrent(void)
:m_iWaitTimeForFetch(0)
,m_dLower(0.0)
,m_dUpper(0.0)
,m_pCGProc(NULL)
,m_pIPS(NULL)
,m_iAverageTimes(1)
{
}

CGetCurrent::~CGetCurrent(void)
{
}

bool CGetCurrent::Run(int iItem)
{
	if (m_iPowerSupplyNo == 1)
		m_pIPS = m_pCGProc->m_pIPS;
	else if(m_iPowerSupplyNo == 2)
		m_pIPS = m_pCGProc->m_pIPS2;
	else if(m_iPowerSupplyNo == 3)
		m_pIPS = m_pCGProc->m_pIPS3;
	else if(m_iPowerSupplyNo == 4)
		m_pIPS = m_pCGProc->m_pIPS4;

	m_strMeasured = "";
	string strUnit;
	if (m_strUnit == "mA")
		strUnit="mA";
	else
		strUnit="A";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if (m_pIPS == NULL)
	{
		if (m_strErrCode.empty())
			m_strErrCode = CommErr_PowerSupply_Control_Fail;

		FactoryLog(false, "", m_strErrCode, "", "", szLower, szUpper, m_strMeasured, strUnit, "Power supply not detected");
		return false;
	}

	bool bRes = MainFunction();

	SetPICSData(m_strPICSName, m_strMeasured);
	if (bRes)
	{
		SetUIMessage("Get current PASS");
		SetUIMessage(m_strMsg);
		//FactoryLog(true, m_NickName, "", "", "", szLower, szUpper, m_strMeasured, strUnit, "Current test PASS");
		
	}
	else
	{
		if (m_strErrCode.empty())
			m_strErrCode = FunErr_Leakage_Current_Test_Fail;

		FactoryLog(false, m_NickName, m_strErrCode, "", "", szLower, szUpper, m_strMeasured, strUnit, m_strMsg);
	}

	if (m_strUnit == "mA")
		m_pIPS->SetCurrRangeMax();

	return bRes;
}

bool CGetCurrent::MainFunction()
{
	double dCurrent;

	if (m_strUnit == "mA")
	{
		if (! m_pIPS->SetCurrRangeMin())
		{
			m_strErrCode = CommErr_PowerSupply_Control_Fail;
			m_strMsg = "Fail to set power supply current range to minimum";
			return false;
		}
	}
	Sleep(m_iWaitTimeForFetch);
	double dCurrentSum = 0.0;
	int iCalcTimes = 0;

	for (int i=0; i<m_iAverageTimes; ++i)
	{
		if (! m_pIPS->GetCurrent(dCurrent))
		{
			m_strErrCode = CommErr_PowerSupply_Control_Fail;
			m_strMsg = "Fail to get current value from power supply";
			return false;
		}
		if (m_strUnit == "mA")
			dCurrent *= 1000.0;

		if (m_bNoNegative && dCurrent < 0)
			continue;

		++iCalcTimes;
		
		dCurrentSum += dCurrent;
		Sleep(200);
	}

	
	double dCurrentResult = dCurrentSum/iCalcTimes;

	char szMeasured[256];
	memset(szMeasured, 0, 256);
	sprintf_s(szMeasured, 255, "%.5f", dCurrentResult);
	m_strMeasured = szMeasured;

	if (! InRange(m_dLower, m_dUpper, dCurrentResult))
	{
		m_strMsg = "current not in range";
		m_strErrCode = FunErr_Leakage_Current_Test_Fail;
		return false;
	}

	m_strMsg = MakeSTDstring("Lower: %d, Upper: %d, MeasureResult: %s mA, current PASS", m_dLower, m_dUpper, m_strMeasured.c_str());

	return true;
}


bool CGetCurrent::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);

	std::vector<std::string> vTmp;
	std::string szString;

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get Parameter

	m_iPowerSupplyNo =  myParm_node.child("PS_Index").text().as_int(ERROR_INT);
	CHECK_PARM("PS_Index", m_iPowerSupplyNo, ERROR_INT);


	m_iWaitTimeForFetch =  myParm_node.child("WaitTimeForFetch").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForFetch", m_iWaitTimeForFetch, ERROR_INT);

	szString =  myParm_node.child("Range").text().as_string(ERROR_STRING);
	CHECK_PARM("Range", szString, ERROR_STRING);

	STDStringTokenizer(szString, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter Range for CGetCurrent");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (myParm_node.child("PICSName").text().as_string(ERROR_STRING) != ERROR_STRING)
		m_strPICSName = myParm_node.child("PICSName").text().as_string();
	else
		m_strPICSName = m_NickName;

	if (myParm_node.child("Unit").text().as_string(ERROR_STRING) != ERROR_STRING)
		m_strUnit = myParm_node.child("Unit").text().as_string();
	else
		m_strUnit = "A";

	if (myParm_node.child("AverageTimes").text().as_int(ERROR_INT) != ERROR_INT)
		m_iAverageTimes = myParm_node.child("AverageTimes").text().as_int();
	else
		m_iAverageTimes = 1;


	if (myParm_node.child("NoNegative").text().as_bool(ERROR_BOOL) != ERROR_BOOL)
		m_bNoNegative = myParm_node.child("NoNegative").text().as_bool();
	else
		m_bNoNegative = false;

	return true;
}
