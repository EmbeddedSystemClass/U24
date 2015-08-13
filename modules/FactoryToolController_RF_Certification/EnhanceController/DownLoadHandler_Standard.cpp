#include "stdafx.h"
#include "DownLoadHandler_Standard.h"

using namespace ECIErrorCode::DownloadHdr;
using namespace ECIErrorCode::TestItemHdr;

CDownLoadHdrStd::CDownLoadHdrStd(CEnhanceController* pNextCEC) :	CEnhanceController(pNextCEC)
{
}

CDownLoadHdrStd::~CDownLoadHdrStd()
{
}

size_t CDownLoadHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

#ifdef DL_STATION

	m_Parametermap[ParameterKeyWord::STATIONNAME] = "OSDL" ;

#endif

#ifdef REDL_STATION
	
	m_Parametermap[ParameterKeyWord::STATIONNAME] = "REDL" ;

#endif

	ret = this->CheckTestItemXMLExist();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadImage();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CDownLoadHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	if(!this->m_pITI->OnRunInit(i_slot))
	{
		return TOOL_ONRUNINIT_FAIL;
	}

	return ret;
}

size_t CDownLoadHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	if(!this->m_pITI->RunTestItem(0, i_slot))
	{
		return TOOL_ONRUN_FAIL;
	}

	return ret; 
}

size_t CDownLoadHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	if(!this->m_pITI->OnRunFinish(i_slot))
	{
		return TOOL_ONRUNFINISH_FAIL;
	}

	return ret;
}

size_t CDownLoadHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	m_vTestItemList.clear();

	return ret;
}



size_t CDownLoadHdrStd::LoadImage()
{
	
	m_vTestItemList.clear();

	XMLNodeList TestItemNodeList = NULL;
	TestItemNodeList = m_TestItemXML.GetChildNodes(_T("//Configuration//ImageList"));

	if (TestItemNodeList == NULL)
	{
		return LOAD_IMAGEFILE_FAIL;
	}

	for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(TestItemNodeList); ++i)
	{
		XMLNode testItemNode = m_TestItemXML.GetNodeListItem(TestItemNodeList, i);

		CString cstrEnabled = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Enabled"));
		if (cstrEnabled.CompareNoCase(_T("TRUE")) != 0)
			continue;

		CString cstrTestItemName = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Name"));
		if (cstrTestItemName.IsEmpty() != true)
		{
			m_vTestItemList.push_back(cstrTestItemName);
		}

	}

	return NOERROR;


}

size_t CDownLoadHdrStd::CheckTestItemXMLExist()
{
	CString cstrTestItemPath =	this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem.xml");
	size_t ret = NOERROR;

	if (::_taccess(cstrTestItemPath, 0) == 0)
	{
		if (m_TestItemXML.Load(cstrTestItemPath) != ERROR_SUCCESS)
		{
			ret = LOAD_IMAGEFILE_FAIL;
		}
	}
	else
	{
		ret = LOAD_IMAGEFILE_FAIL;
	}

	return ret;
}

