#include "stdafx.h"
#include "CheckDeviceOptions.h"


RF_IMPLEMENT_DYNCREATE(CCheckDeviceOptions)


bool CCheckDeviceOptions::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Check Device Options PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = ERR_GSMTEST_STARTCALL ;

		FactoryLog(false, "", m_strErrCode, "", "", "", "", "", "", m_strMsg);
	}

	return bRes;
}

bool CCheckDeviceOptions::MainFunction()
{
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to Initial Tester Device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strOptions ;
	if (! m_pITesterDevice->GetDeviceOptions(strOptions) )
	{
		m_strMsg = "Fail to Fetch Device Options" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	std::vector<std::string> vDeviceOptionsList;
	if (! STDStringTokenizer(strOptions, ",", vDeviceOptionsList))
	{ 
		m_strMsg = "Fail to execute function STDStringTokenizer for Device Options Data" ;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	bool bSuccess;

	for (unsigned int i = 0; i < m_vec_strDeviceOptions.size(); i++)
	{
		bSuccess = false;

		for (unsigned int j = 0; j < vDeviceOptionsList.size(); j++)
		{
			if (strstr(vDeviceOptionsList[j].c_str(),m_vec_strDeviceOptions[i].c_str()) != NULL)
				bSuccess = true;
		}

		if (! bSuccess)
		{
			m_strMsg = "The Device does not have option - " + m_vec_strDeviceOptions[i] ;
			TraceLog(MSG_ERROR, m_strMsg);

			return bSuccess;
		}
	}

	return true;
}


bool CCheckDeviceOptions::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("DeviceOptions")) == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DeviceOptions for CheckDeviceOptions");
		return false;
	}
	STDStringTokenizer(paramMap["DeviceOptions"], ";", m_vec_strDeviceOptions);
	if (m_vec_strDeviceOptions.size() == 0)
		m_vec_strDeviceOptions[0] = "0";

	return true;
}
