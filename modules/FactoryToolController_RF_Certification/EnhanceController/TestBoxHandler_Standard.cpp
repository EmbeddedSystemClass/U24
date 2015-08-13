#include "stdafx.h"
#include "TestBoxHandler_Standard.h"

using namespace ECIErrorCode::TestBoxHdr;

CTestBoxHdrStd::CTestBoxHdrStd(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC)
{
}

CTestBoxHdrStd::~CTestBoxHdrStd()
{
}

size_t CTestBoxHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;
	
	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CTestBoxHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	ret = this->CheckTestBoxXMLExist();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadTestBox();
	if(NOERROR != ret)
		return ret;

	ret = this->SetToolTestBox();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CTestBoxHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CTestBoxHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CTestBoxHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	m_mapTesteBox.clear();

	return ret;
}

size_t CTestBoxHdrStd::CheckTestBoxXMLExist()
{
	size_t ret = NOERROR;
	CString cstrTestBoxPath;

	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_BBTEST)
	{
		cstrTestBoxPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") +
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_DeviceItem.xml");
	}
	else if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST ||
			this->m_Parametermap[ParameterKeyWord::STATIONNAME] == _T("LTE"))
	{
		cstrTestBoxPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem_CMW.xml");
	}
	else
	{
		cstrTestBoxPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem.xml");
	}

	if (::_taccess(cstrTestBoxPath, 0) == 0)
	{
		if (m_TestBoxXML.Load(cstrTestBoxPath) != ERROR_SUCCESS)
		{
			ret = LOAD_TESTBOX_SETTING_FAIL;
		}
	}
	else
	{
		ret = LOAD_TESTBOX_SETTING_FAIL;
	}

	return ret;
}

size_t CTestBoxHdrStd::LoadTestBox()
{
	m_mapTesteBox.clear();
	XMLNode testerDevice;

	// Tester device parameters
	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == _T("LTE"))
	{
		testerDevice = m_TestBoxXML.SearchNode(_T("//Configuration//TesterDevice"));
	}
	else
	{
		testerDevice = m_TestBoxXML.SearchNode(_T("//Configuration//TestProcessor//TesterDevice"));
	}
	
	if (testerDevice != NULL)
	{
		XMLNodeList testerDeviceList = m_TestBoxXML.GetChildNodes(testerDevice);
		if (testerDeviceList != NULL)
		{
			for (UINT i = 0; i < m_TestBoxXML.GetNodeListSize(testerDeviceList); ++i)
			{
				XMLNode testerDeviceItem = m_TestBoxXML.GetNodeListItem(testerDeviceList, i);
				if (testerDeviceItem != NULL)
				{
					CString cstrName = m_TestBoxXML.GetNodeAttrValue(testerDeviceItem, _T("Name"));
					CString cstrClassName = m_TestBoxXML.GetNodeAttrValue(testerDeviceItem, _T("ClassName"));
					if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
						continue;

					// Insert <Name, ClassName> to map
					m_mapTesteBox[cstrName] = cstrClassName;
				}
			}
		}
	}

	else
		return LOAD_TESTBOX_SETTING_FAIL;

	return NOERROR;
}

size_t CTestBoxHdrStd::SetToolTestBox()
{
	size_t ret = NOERROR;

	for(	std::map<CString, CString>::iterator iter = this->m_mapTesteBox.begin();
			iter != this->m_mapTesteBox.end();
			iter++)
	{
		if(!this->m_pITI->SetTestPart(IToolInterface::TEST_BOX, L"", iter->first.GetString(), iter->second.GetString()))
		{
			ret = TOOL_SET_TESTBOX_FAIL;
			break;
		}
	}

	return ret;
}