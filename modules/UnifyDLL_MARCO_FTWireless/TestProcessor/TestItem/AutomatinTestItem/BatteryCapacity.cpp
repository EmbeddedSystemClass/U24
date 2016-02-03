/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CBatteryCapacity object.
 * FileName   : BatteryCapacityDetect.cpp
 * Version    : $Revision:   1.0  $
 * Author     :  Lion.Wu
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12    Lion.Wu         Create.
 ********************************************************************************/

#include "stdafx.h"
#include "BatteryCapacity.h"


RF_IMPLEMENT_DYNCREATE(CBatteryCapacity)

bool CBatteryCapacity::PreRun()
{
	m_strItemCode = CStr::IntToStr(BatteryCapacity_BaseItemCode);
	m_strErrorCode = FunErr_Check_Battery_Capacity_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CBatteryCapacity::Run()
{
	bool bRes = false;
	CString cs_BatteryName = m_str_BatteryName.c_str();
	if (cs_BatteryName.Compare("BatteryDetectFull") == 0)
	{
		bRes = BatteryCapacityDetectFull();
	}
	else if (cs_BatteryName.Compare("BatteryDetectEmpty") == 0)
	{
		bRes = BatteryCapacityDetectEmpty();
	}
	return bRes;
}

bool CBatteryCapacity::PostRun()
{
	return true;
}

bool CBatteryCapacity::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_BatteryName = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter m_str_BatteryLimit for CBatteryCapacity"));
		return false;
	}

	if (paramMap.find(std::string("BatteryLimit")) != paramMap.end())
	{
		m_str_BatteryLimit = paramMap[std::string("BatteryLimit")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter m_str_BatteryLimit for CBatteryCapacity"));
		return false;
	}
	return true;
}

bool CBatteryCapacity::BatteryCapacityDetectEmpty()
{
	bool b_res = false;
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	std::string str_msg;
	if (!(b_res = m_pIPhone->FTD_PWRbattCapacity(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData)))
	{
		str_msg = "PWRbattCapacity Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = sz_outData;
	int n_BatteryCapacity = atoi(sz_outData);
	if (b_res)
	{
		if (n_BatteryCapacity < atoi(m_str_BatteryLimit.c_str()))
		{
			b_res = false;
			showWarningMsgEmpty();
		}
	}

	/* test result */
	if (b_res)
	{
		str_msg = "PASS, BatteryCapacityDetectEmpty = " + m_strMeasured;
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "BatteryCapacityDetectEmpty pass!";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "FAIL, BatteryCapacityDetectEmpty = " + m_strMeasured;
		m_strResult = "FAIL";
		m_strMessage = "BatteryCapacityDetectEmpty fail";
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return true;//always pass
}

bool CBatteryCapacity::BatteryCapacityDetectFull()
{
	bool b_res = false;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutput[FTD_BUF_SIZE] = {0};
	std::string str_msg;
	if (!(b_res = m_pIPhone->FTD_PWRbattCapacity(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput)))
	{
		str_msg = "PWRbattCapacity Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strMeasured = szOutput;
	int n_BatteryCapacity = atoi(szOutput);
	if (b_res && n_BatteryCapacity > atoi(m_str_BatteryLimit.c_str()))
	{
		b_res = false;
		//showWarningMsgFull();
	}
	SetPICSData(_T("BatteryCapacityFull"), m_strMeasured);
	if (b_res)
	{
		str_msg = "PASS, BatteryCapacityDetectFull = " + m_strMeasured;
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "BatteryCapacityDetectFull pass!";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "FAIL, BatteryCapacityDetectFull = " + m_strMeasured;
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Battery_Voltage_Too_High;
		m_strMessage = "BatteryCapacityDetectFull fail";
		TraceLog(MSG_INFO, str_msg);
	}
	FactoryLog();
	return b_res;// always pass
}

bool CBatteryCapacity::showWarningMsgEmpty()
{
	ShowDialog(CW2A(L"电量过低，请更换电池!\n Please Change Battery\n"));
	return true;
}

bool CBatteryCapacity::showWarningMsgFull()
{
	ShowDialog(CW2A(L"电量过高，请更换电池!\n Please Change Battery\n"));
	return true;
}
