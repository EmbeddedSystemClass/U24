#include "stdafx.h"
#include "PowerSupplyControl.h"
#include "FacTestToolInterface.h"
using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CPowerSupplyControl)

bool CPowerSupplyControl::Run(int iItem)
{
	bool bRes = MainFunction() ;

	if (bRes)
	{
		Sleep(m_iDelay) ;

		SetUIMessage("Power supply control PASS");
		SetUIMessage(m_strMsg);
		//sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Power supply control");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi("PASS to Power supply control");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "PS_Control", CommErr_PowerSupply_Control_Fail, "", "", "", "", "", "", m_strMsg);

		//sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Power supply control");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi("FAIL to Power supply control");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CPowerSupplyControl::MainFunction()
{
	IPowerSupply* pIPS = NULL;

	if (m_iPS_Index == 1)
		pIPS = m_pCGProc->m_pIPS;
	else if (m_iPS_Index == 2)
		pIPS = m_pCGProc->m_pIPS2;
	else if (m_iPS_Index == 3)
		pIPS = m_pCGProc->m_pIPS3;
	else
	{
		m_strMsg = "Fail to parse PS index, no such PS exist";
		TraceLog(m_strMsg);
		return false;
	}

	if (pIPS == NULL)
	{
		m_strMsg = "Fail to control PS, PS not detected";
		TraceLog(m_strMsg);
		return false;
	}

	/*
	if (! pIPS->IsAvaliable())
	{
		m_strMsg = "Fail to control PS, PS not available";
		TraceLog(m_strMsg);
		return false;
	}

	if (! pIPS->SetVoltage(m_dStepdVoltage))
	{
		m_strMsg = "Fail to set Step voltage to PS";
		TraceLog(m_strMsg);
		return false;
	}

	if (! pIPS->SetCurrentLimit(m_dCurrent))
	{
		m_strMsg = "Fail to set current to PS";
		TraceLog(m_strMsg);
		return false;
	}

	if (! pIPS->SetCurrRangeMax())
	{
		m_strMsg = "Fail to set current range MAXIMUM to PS";
		TraceLog(m_strMsg);
		return false;
	}

	if (! pIPS->SetPSOnOff(m_bPS_OnOff))
	{
		m_strMsg = "Fail to control PS on/off";
		TraceLog(m_strMsg);
		return false;
	}

	if(m_bPS_OnOff)
	{
		Sleep(500) ;
		if (! pIPS->SetVoltage(m_dFinaldVoltage))
		{
			m_strMsg = "Fail to set Final voltage to PS";
			TraceLog(m_strMsg);
			return false;
		}
		TraceLog("PowerSupply on.");
		sprintf_s(m_st_UIMessage.sz_message,4096,"PowerSupply on.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{
		TraceLog("PowerSupply off.");
		sprintf_s(m_st_UIMessage.sz_message,4096,"PowerSupply off.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	*/
	if(m_bPS_OnOff)
	{
		//Sleep(500) ;
		if (! pIPS->SetPowerSupplyOn())
		{
			m_strMsg = "Fail to set PS on";
			TraceLog(m_strMsg);
			return false;
		}
		TraceLog("PowerSupply on.");
		//sprintf_s(m_st_UIMessage.sz_message,4096,"PowerSupply on.");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi("PowerSupply on.");
	}
	else
	{
		if (! pIPS->SetPowerSupplyOff())
		{
			m_strMsg = "Fail to set PS off";
			TraceLog(m_strMsg);
			return false;
		}
		TraceLog("PowerSupply off.");
		//sprintf_s(m_st_UIMessage.sz_message,4096,"PowerSupply off.");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi("PowerSupply off.");
	}
	return true;
}


bool CPowerSupplyControl::InitData(std::map<std::string, std::string>& paramMap)
{
	//std::vector<std::string> vec_strTmp;
	std::string strTmp;
	pugi::xml_node myParm_node;

	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));

	//cast interface to a real object
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);

	// CGSMTxCal Para
	myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//PS_Index
	m_iPS_Index = myParm_node.child("PS_Index").text().as_int(ERROR_INT);
	if (m_iPS_Index == ERROR_INT)
	{
		TraceLog("Fail to find parameter Voltage for CPowerSupplyControl, default value = 1");
		m_iPS_Index = 1;
	}

	/*
	//Voltage_Step
	m_dStepdVoltage = myParm_node.child("Voltage_Step").text().as_double(ERROR_DOUBLE);
	CHECK_PARM("Voltage_Step", m_dStepdVoltage , ERROR_DOUBLE);

	//Voltage_Target
	m_dFinaldVoltage = myParm_node.child("Voltage_Target").text().as_double(ERROR_DOUBLE);
	CHECK_PARM("Voltage_Target", m_dFinaldVoltage , ERROR_DOUBLE);

	//Current
	m_dCurrent = myParm_node.child("Current").text().as_double(ERROR_DOUBLE);
	CHECK_PARM("Current", m_dCurrent , ERROR_DOUBLE);
	*/

	//PS_OnOff
	strTmp = myParm_node.child("PS_OnOff").text().as_string(ERROR_STRING);
	CHECK_PARM("PS_OnOff", strTmp , ERROR_STRING);
	if ((strTmp == "TRUE") || (strTmp == "true"))// || (strTmp != "0"))
		m_bPS_OnOff = true;
	else
		m_bPS_OnOff = false;

	//WaitAfterOnOff
	m_iDelay = myParm_node.child("WaitAfterOnOff").text().as_int(ERROR_INT);
	CHECK_PARM("WaitAfterOnOff", m_iDelay , ERROR_INT);

	//PICSName
	m_strPICSName = myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	if(m_strPICSName == ERROR_STRING)
		m_strPICSName.clear();

	TraceLog("PASS to initdata for CPowerSupplyControl");
	//sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CPowerSupplyControl");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	m_pCGProc->SendMsgToUi("Pass to initdata for CPowerSupplyControl");
	return true;
}
