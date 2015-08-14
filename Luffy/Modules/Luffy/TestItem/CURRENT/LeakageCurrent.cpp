#include "stdafx.h"
#include "LeakageCurrent.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;
using namespace InspectionUtility;

GP_IMPLEMENT_DYNCREATE(CLeakageCurrent)

CLeakageCurrent::CLeakageCurrent(void)
:m_iWaitTimeForShutdown(0)
,m_iWaitTimeForFetch(0)
,m_iFetchCount(0)
,m_dLower(0.0)
,m_dUpper(0.0)
,m_pCGProc(NULL)
,m_pIPS(NULL)
,m_pIPS2(NULL)
{
}

CLeakageCurrent::~CLeakageCurrent(void)
{
}

bool CLeakageCurrent::Run(int iItem)
{
	m_pIPS = m_pCGProc->m_pIPS;

	//m_pIPS2 = m_pCGProc->m_pIPS2;
	m_pCGProc->SendMsgToUi("Luffy������");
	m_pCGProc->SendMsgToUi("Power supply off.");
	TraceLog("Power supply off.");
	m_pIPS->SetPowerSupplyOff();

	//waiting 1s
	Sleep(1000);

	//PS ON
	m_pCGProc->SendMsgToUi("Power supply on.");
	TraceLog("Power supply on.");
	m_pIPS->SetPowerSupplyOn();

	//Waiting 5s
	Sleep(5000);

	//detect ADFU devices
	m_pCGProc->SendMsgToUi("�Զ����ADFU�豸��...");
	TraceLog("Auto detect ADFU device.");
	if(false == m_pCGProc->AutoDetectDevice())
	{	m_pCGProc->SendMsgToUi("��ⲻ��ADFU�豸,�����̨�Ƿ���ϵͳ�������ȷ");
		TraceLog("Detect device fail.");
		m_pCGProc->SetFailMsg("��ⲻ��ADFU�豸");
		return false;
	}

	m_strMeasured = "";

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

		FactoryLog(false, "", m_strErrCode, "", "", szLower, szUpper, m_strMeasured, "mA", "Power supply not detected");
		SetPICSData("LeakageCurrent", "FAIL");
		SetPICSData("LeakageCurrentValue", "0.000");

		return false;
	}

	bool bRes = MainFunction();

//	CPKLog("Current", "LeakageCurrent", "-", "-", "","",szLower, szUpper, m_strMeasured.c_str(), bRes, 1);

	if (bRes)
	{
		SetUIMessage("Leakage current PASS");
		SetUIMessage(m_strMsg);

		//FactoryLog(true, "", "", "", "", szLower, szUpper, m_strMeasured, "mA", "Leakage current PASS");
		SetPICSData("LeakageCurrent", "PASS");
		SetPICSData("LeakageCurrentValue", m_strMeasured);

		CPKLog("Current", "LeakageCurrent", "","","","", m_vLeakageCurrentRange[0].c_str(), m_vLeakageCurrentRange[1].c_str(), m_strMeasured.c_str(), bRes, 0);
	}
	else
	{
		if (m_strErrCode.empty())
			m_strErrCode = FunErr_Leakage_Current_Test_Fail;

		FactoryLog(false, "", m_strErrCode, "", "", szLower, szUpper, m_strMeasured, "mA", m_strMsg);
		SetPICSData("LeakageCurrent", "FAIL");
		SetPICSData("LeakageCurrentValue", m_strMeasured);

		CPKLog("Current", "LeakageCurrent", "","","","", m_vLeakageCurrentRange[0].c_str(), m_vLeakageCurrentRange[1].c_str(), m_strMeasured.c_str(), bRes, 0);
		m_pCGProc->SetFailMsg("©��������");
	}

	m_pIPS->SetCurrRangeMax();
	m_pIPS->SetPSOnOff(true);
	//m_pIPS2->SetPSOnOff(true);

	return bRes;
}

bool CLeakageCurrent::MainFunction()
{
	double dCurrent;

	//if (! m_pIPS->SetPSOnOff(false))
	//{
	//	m_strMsg = "Fail to set PS off";
	//	return false;
	//}

	//if (! m_pIPS2->SetPSOnOff(false))
	//{
	//	m_strMsg = "Fail to set PS2 off";
	//	return false;
	//}

	//Sleep(m_iWaitTimeForShutdown);

	sprintf_s(m_st_UIMessage.sz_message,4096,"LeakageCurrent is running......");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	if (! m_pIPS->SetCurrRangeMax())
	{
		m_strMsg = "Fail to set power supply current range to minimum";
		return false;
	}
	
	////����power supply��ʵ��ǰһ���Ѿ�������
	//if (! m_pIPS->SetPSOnOff(true))
	//{
	//	m_strMsg = "Fail to set power supply on";
	//	return false;
	//}

	//�ȴ�
	Sleep(m_iWaitTimeForFetch);

	bool bRes = false;
	int i=0;

	do
	{
		i++;
		if(i >= 30)
			break;

		if (! m_pIPS->GetCurrent(dCurrent))
		{
			m_strMsg = "Fail to get current value from power supply";
			return false;
		}

		dCurrent *= 1000.0;

		char szMeasured[256];
		memset(szMeasured, 0, 256);
		sprintf_s(szMeasured, 255, "%.3f", dCurrent);
		m_strMeasured = szMeasured;

		bRes = InRange(m_dLower, m_dUpper, dCurrent);
		if ( !bRes )         //��Falseʱ����ʱ20ms�ٲ�
			Sleep(20);
		//if (! InRange(m_dLower, m_dUpper, dCurrent))
		//{
		//	m_strMsg = "Leakage current not in range";
		//	m_strErrCode = FunErr_Leakage_Current_Test_Fail;
		//	return false;
		//}
	}
	while(!bRes);

	char szLower[256];
	char szUpper[256];
	sprintf_s(szLower, 255, "%.3f", m_dLower);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if(bRes)
	{
		m_strMsg = MakeSTDstring("Lower: %s, Upper: %s, MeasureResult: %s mA, Leakage current PASS", szLower, szUpper, m_strMeasured.c_str());
		sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return true;
	}
	else
	{
		//m_strMsg = "Leakage current not in range";
		m_strMsg = MakeSTDstring("Lower: %s, Upper: %s, MeasureResult: %s mA, Leakage current FAIL", szLower, szUpper, m_strMeasured.c_str());
		m_strErrCode = FunErr_Leakage_Current_Test_Fail;
		sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
}


bool CLeakageCurrent::InitData(std::map<std::string, std::string>& paramMap)
{

	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));

	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);

	std::vector<std::string> vTmp;
	std::string szString;

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get Parameter

	m_iWaitTimeForShutdown =  myParm_node.child("WaitTimeForShutdown").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForShutdown", m_iWaitTimeForShutdown, ERROR_INT);


	m_iWaitTimeForFetch =  myParm_node.child("WaitTimeForFetch").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForFetch", m_iWaitTimeForFetch, ERROR_INT);

	m_iFetchCount = myParm_node.child("FetchCount").text().as_int(ERROR_INT);
	CHECK_PARM("FetchCount", m_iFetchCount, ERROR_INT);

	szString =  myParm_node.child("Range").text().as_string(ERROR_STRING);
	CHECK_PARM("Range", szString, ERROR_STRING);

	STDStringTokenizer(szString, ",", vTmp);
	STDStringTokenizer(szString, ",", m_vLeakageCurrentRange);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter Range for CLeakageCurrent");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to find parameter Range for CLeakageCurrent");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	m_dLower = m_dLower * 1000;
	m_dUpper = m_dUpper * 1000;

	TraceLog("PASS to initdata for CLeakageCurrent");
	//sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CLeakageCurrent");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	m_pCGProc->SendMsgToUi("CLeakageCurrent��ʼ�����");

	return true;
}
