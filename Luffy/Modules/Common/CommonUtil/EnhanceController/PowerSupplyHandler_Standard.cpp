#include "stdafx.h"
#include "PowerSupplyHandler_Standard.h"

using namespace ECIErrorCode::PowerSupplyHdr;

CPowerSupplyHdrStd::CPowerSupplyHdrStd(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC)
{
}

CPowerSupplyHdrStd::~CPowerSupplyHdrStd()
{
}

size_t CPowerSupplyHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPowerSupplyHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	ret = this->CheckPSXMLExist();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadPS();
	if(NOERROR != ret)
		return ret;

	ret = this->SetToolPS();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPowerSupplyHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPowerSupplyHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CPowerSupplyHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	m_mapPS.clear();
	m_mapPSList.clear();

	return ret;
}

size_t CPowerSupplyHdrStd::CheckPSXMLExist()
{
	size_t ret = NOERROR;

	CString cstrTestBoxPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
											_T("Qisda\\") + 
											this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
											_T("_") + 
											this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
											_T("_TestItem.xml");

	if (::_taccess(cstrTestBoxPath, 0) == 0)
	{
		if (m_PSXML.Load(cstrTestBoxPath) != ERROR_SUCCESS)
		{
			ret = LOAD_PS_SETTING_FAIL;
		}
	}
	else
	{
		ret = LOAD_PS_SETTING_FAIL;
	}

	return ret;
}

size_t CPowerSupplyHdrStd::LoadPS()
{
	m_mapPS.clear();
	m_mapPSList.clear();

	size_t ret = NOERROR;
	
	ret = this->LoadPSCount();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadPSList();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadPSItem();
	if(NOERROR != ret)
		return ret;

	return ret;
}


size_t CPowerSupplyHdrStd::SetToolPS()
{
	size_t ret = NOERROR;

	for(	std::map<CString, CString>::iterator iter = this->m_mapPS.begin();
			iter != this->m_mapPS.end();
			iter++)
	{
		if(!this->m_pITI->SetTestPart(IToolInterfaceV2::POWERSUPPLY, L"", iter->first.GetString(), iter->second.GetString()))
		{
			ret = TOOL_SET_PS_FAIL;
			break;
		}
	}

	for(	std::vector<CString>::iterator iter = this->m_mapPSList.begin();
			iter != this->m_mapPSList.end();
			iter++)
	{
		if(!this->m_pITI->SetTestPart(IToolInterfaceV2::POWERSUPPLY, L"", _T("PowerSupplyList"), (*iter).GetString()))
		{
			ret = TOOL_SET_PS_FAIL;
			break;
		}
	}

	return ret;
}


size_t CPowerSupplyHdrStd::LoadPSCount()
{
	XMLNode powerSupplyCount = m_PSXML.SearchNode(_T("//Configuration//PowerSupply//PowerSupplyCount"));
	if (powerSupplyCount != NULL)
	{
		CString cstrPowerSupplyCount = m_PSXML.GetNodeText(powerSupplyCount);

		this->m_mapPS[_T("PowerSupplyCount")] = cstrPowerSupplyCount;

		return NOERROR;
	}

	return LOAD_PS_SETTING_FAIL;
}

size_t CPowerSupplyHdrStd::LoadPSList()
{
	XMLNode powerSupplyList = m_PSXML.SearchNode(_T("//Configuration//PowerSupply//PowerSupplyList"));
	if (powerSupplyList != NULL)
	{
		XMLNodeList powerSupplyOrderList = m_PSXML.GetChildNodes(powerSupplyList);
		if (powerSupplyOrderList != NULL)
		{
			for (UINT i = 0; i < m_PSXML.GetNodeListSize(powerSupplyOrderList); ++i)
			{
				XMLNode powerSupplyOrder = m_PSXML.GetNodeListItem(powerSupplyOrderList, i);
				if (powerSupplyOrder != NULL)
				{
					CString cstrPSAddr = m_PSXML.GetNodeText(powerSupplyOrder);
					/*
					if (!cstrPSAddr.IsEmpty())
					{
						this->m_mapPS[_T("PowerSupplyList")] = cstrPSAddr;
					}
					*/
					if (!cstrPSAddr.IsEmpty())
					{
						this->m_mapPSList.push_back(cstrPSAddr);
					}
				}
			}

			return NOERROR;
		}

		return LOAD_PS_SETTING_FAIL;
	}

	return LOAD_PS_SETTING_FAIL;
}

size_t CPowerSupplyHdrStd::LoadPSItem()
{
	XMLNode powerSupply = m_PSXML.SearchNode(_T("//Configuration//PowerSupply"));
	if (powerSupply != NULL)
	{
		XMLNodeList powerSupplyChildList = m_PSXML.GetChildNodes(powerSupply);
		if (powerSupplyChildList != NULL)
		{
			for (UINT i = 0; i < m_PSXML.GetNodeListSize(powerSupplyChildList); ++i)
			{
				XMLNode powerSupplyItem = m_PSXML.GetNodeListItem(powerSupplyChildList, i);
				if (powerSupplyItem != NULL)
				{
					if (m_PSXML.GetNodeName(powerSupplyItem).CompareNoCase(_T("Item")) != 0)
						continue;

					CString cstrName = m_PSXML.GetNodeAttrValue(powerSupplyItem, _T("Name"));
					CString cstrClassName = m_PSXML.GetNodeAttrValue(powerSupplyItem, _T("ClassName"));
					if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
						continue;

					this->m_mapPS[cstrName] = cstrClassName;
				}
			}

			return NOERROR;
		}

		return LOAD_PS_SETTING_FAIL;
	}

	return LOAD_PS_SETTING_FAIL;
}
