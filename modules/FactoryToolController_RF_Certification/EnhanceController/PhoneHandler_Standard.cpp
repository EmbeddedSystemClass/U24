#include "stdafx.h"
#include "PhoneHandler_Standard.h"

using namespace ECIErrorCode::PhoneHdr;

CPhoneHdrStd::CPhoneHdrStd(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC)
{
}

CPhoneHdrStd::~CPhoneHdrStd()
{
}

size_t CPhoneHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPhoneHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	ret = this->CheckTestItemXMLExist();
	if(NOERROR != ret)
		return ret;

	if(this->m_Parametermap[_T("DLMODE")] == _T("ReDL"))
	{
		return ret;
	}

	ret = this->CheckConfigXMLExist();
	if(NOERROR != ret)
		return ret;

	this->m_mapPhone.clear();

	ret = this->LoadPhoneInTestItem();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadPhoneInConfig();
	if(NOERROR != ret)
		return ret;

	ret = this->SetToolPhone();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPhoneHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPhoneHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPhoneHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	m_mapPhone.clear();

	return ret;
}

size_t CPhoneHdrStd::CheckTestItemXMLExist()
{
	CString cstrTestItemPath;
	
	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == _T("LTE"))
	{
		cstrTestItemPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem_CMW.xml");
	}
	else
	{
		cstrTestItemPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem.xml");
	}
	size_t ret = NOERROR;

	if (::_taccess(cstrTestItemPath, 0) == 0)
	{
		if (m_TestItemXML.Load(cstrTestItemPath) != ERROR_SUCCESS)
		{
			ret = LOAD_PHONE_SETTING_FAIL;
		}
	}
	else
	{
		ret = LOAD_PHONE_SETTING_FAIL;
	}

	return ret;
}

size_t CPhoneHdrStd::CheckConfigXMLExist()
{
	CString cstrConfigPath =	this->m_Parametermap[ParameterKeyWord::CONFIGDIR] + 
											this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
											_T("_") +
											this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
											_T("_Config.xml");
	size_t ret = NOERROR;

	if (::_taccess(cstrConfigPath, 0) == 0)
	{
		if (m_ConfigXML.Load(cstrConfigPath) != ERROR_SUCCESS)
		{
			ret = LOAD_PHONE_SETTING_FROM_CONFIG_FAIL;
		}
	}
	else
	{
		ret = LOAD_PHONE_SETTING_FROM_CONFIG_FAIL;
	}

	return ret;
}

size_t CPhoneHdrStd::LoadPhoneInTestItem()
{
	size_t ret = NOERROR;
	XMLNodeList phoneList;

	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == _T("LTE"))
	{
		phoneList = m_TestItemXML.GetChildNodes(_T("//Configuration//Phone"));
	}
	else
	{
		phoneList = m_TestItemXML.GetChildNodes(_T("//Configuration//TestProcessor//Phone"));
	}
	
	if (phoneList == NULL)
	{
		return LOAD_PHONE_SETTING_FAIL;
	}

	for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_TestItemXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_TestItemXML.GetNodeName(phoneParams);
		CString nodevalue = m_TestItemXML.GetNodeText(phoneParams);

		this->m_mapPhone[nodename] = nodevalue;
	}

	return ret;
}

size_t CPhoneHdrStd::LoadPhoneInConfig()
{
	size_t ret = NOERROR;

	XMLNodeList phoneList = m_ConfigXML.GetChildNodes(_T("//Configuration//Phone"));

	if (phoneList == NULL)
	{
		return LOAD_PHONE_SETTING_FROM_CONFIG_FAIL;
	}

	for (UINT i = 0; i < m_ConfigXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_ConfigXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_ConfigXML.GetNodeName(phoneParams);
		CString nodevalue = m_ConfigXML.GetNodeText(phoneParams);

		this->m_mapPhone[nodename] = nodevalue;
	}

	return ret;
}

size_t CPhoneHdrStd::SetToolPhone()
{
	size_t ret = NOERROR;

	for(	std::map<CString, CString>::iterator iter = this->m_mapPhone.begin();
			iter != this->m_mapPhone.end();
			iter++)
	{
		if(!this->m_pITI->SetTestPart(IToolInterface::PHONE, L"", iter->first.GetString(), iter->second.GetString()))
		{
			ret = TOOL_SET_PHONE_FAIL;
			break;
		}
	}

	return ret;
}