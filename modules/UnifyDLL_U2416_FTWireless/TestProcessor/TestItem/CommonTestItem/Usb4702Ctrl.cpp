#include "stdafx.h"
#include "Usb4702Ctrl.h"


RF_IMPLEMENT_DYNCREATE(CUsb4702Ctrl)

bool CUsb4702Ctrl::InitData(std::map<std::string, std::string>& paramMap)
{


	//The 4702 action
	std::string str_Action;
	if (paramMap.find("USB4702Action") == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Get USB4702Action Fail."));
		return false;
	}
	str_Action = paramMap[std::string("USB4702Action")];
	m_i_Action = CStr::StrToInt(str_Action);


	//If not initial, 4702 needs bitlist.
	char c_ListTmp[20];
	char *p_ctoken;
	if(m_i_Action != 0)
	{
		if (paramMap.find("BitList") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter BitList for USB4702");
			return false;
		}
		
		strcpy(c_ListTmp, paramMap["BitList"].c_str());
		
		p_ctoken = strtok(c_ListTmp, ",");
		m_i_list_length = 0;
		while (p_ctoken != NULL)
		{
			m_i_list[m_i_list_length] = atoi(p_ctoken);
			p_ctoken = strtok(NULL, ",");
			m_i_list_length++;
		}
	}


	return true;
}

bool CUsb4702Ctrl::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CUsb4702Ctrl::Run()
{
	bool b_res = false;

	switch(m_i_Action)
	{
		case 0:
			b_res = Init();
			break;
		case 1:
			b_res = SetDOBit();
			break;
		case 2:
			b_res = ClearDOBit();
			break;
		default:
			break;
	}
	return b_res;
}

bool CUsb4702Ctrl::PostRun()
{
	return true;
}

bool CUsb4702Ctrl::Init()
{

	if (! m_pIPhone->USB4702_SetAOVol(0, 0, "0.0"))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetAOVol0,0.0");
		return false;
	}

	if (! m_pIPhone->USB4702_SetAOVol(0, 1, "5.0"))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetAOVol1,5.0");
		return false;
	}

	if (! m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 3))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit3");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 6))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit6");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 7))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit7");
		return false;
	}

	m_strMessage = _T("Init clear sucess");
	FactoryLog();
	return true;
}

bool CUsb4702Ctrl::SetDOBit()
{
	int i;
	std::string strTmp;
	char cTmp[10];

	for(i= 0; i < m_i_list_length; i++)
	{
		if (! m_pIPhone->USB4702_SetDOBit(1, m_i_list[i]))
		{
			itoa(m_i_list[i], cTmp, 10);
			strTmp.assign(cTmp);
			m_strMessage = _T("Fail to execute m_pIPhone->USB4702_SetDOBit") + strTmp;
			TraceLog(MSG_INFO, m_strMessage);
			return false;
		}
	}
	m_strMessage = _T("Set4702 DOBit sucess");
	FactoryLog();
	return true;
}

bool CUsb4702Ctrl::ClearDOBit()
{
	int i;
	std::string strTmp;
	char cTmp[10];

	for(i= 0; i < m_i_list_length; i++)
	{
		if (! m_pIPhone->USB4702_ClearDOBit(1, m_i_list[i]))
		{
			itoa(m_i_list[i], cTmp, 10);
			strTmp.assign(cTmp);
			m_strMessage = _T("Fail to execute m_pIPhone->USB4702_ClearDOBit") + strTmp;
			TraceLog(MSG_INFO, m_strMessage);
			return false;
		}
	}
	m_strMessage = _T("Clear4702 DOBit sucess");
	FactoryLog();
	return true;
}