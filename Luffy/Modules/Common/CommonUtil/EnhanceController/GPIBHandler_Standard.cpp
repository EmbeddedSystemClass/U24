#include "stdafx.h"
#include "GPIBHandler_Standard.h"

using namespace ECIErrorCode::GPIBHdr;

CGPIBHdrStd::CGPIBHdrStd(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC)
{
}

CGPIBHdrStd::~CGPIBHdrStd()
{
}

size_t CGPIBHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CGPIBHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	ret = this->CheckGPIBXMLExist();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadGPIB();
	if(NOERROR != ret)
		return ret;

	ret = this->SetToolGPIB();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CGPIBHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CGPIBHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CGPIBHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	m_mapGPIB.clear();

	return ret;
}

size_t CGPIBHdrStd::CheckGPIBXMLExist()
{
	CString cstrTestBoxPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
											_T("Qisda\\") + 
											this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
											_T("_") + 
											this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
											_T("_TestItem.xml");
	size_t ret = NOERROR;

	if (::_taccess(cstrTestBoxPath, 0) == 0)
	{
		if (m_GPIBXML.Load(cstrTestBoxPath) != ERROR_SUCCESS)
		{
			ret = LOAD_GPIB_SETTING_FAIL;
		}
	}
	else
	{
		ret = LOAD_GPIB_SETTING_FAIL;
	}

	return ret;
}

size_t CGPIBHdrStd::LoadGPIB()
{
	m_mapGPIB.clear();

	size_t ret = NOERROR;

	XMLNode UsingGPIB = m_GPIBXML.SearchNode(_T("//Configuration//GPIB//UsingGPIB"));
	if (UsingGPIB != NULL)
	{
		CString cstrUsingGPIB = m_GPIBXML.GetNodeText(UsingGPIB);
		if (! cstrUsingGPIB.IsEmpty())
		{
			m_mapGPIB[_T("UsingGPIB")] = cstrUsingGPIB;
		}
	}
	else
	{
		ret = LOAD_GPIB_SETTING_FAIL;
	}

	XMLNode GPIBInterfaceID = m_GPIBXML.SearchNode(_T("//Configuration//GPIB//GPIBInterfaceID"));
	if (GPIBInterfaceID != NULL)
	{
		CString cstrGPIBInterfaceID = m_GPIBXML.GetNodeText(GPIBInterfaceID);
		if (! cstrGPIBInterfaceID.IsEmpty())
			m_mapGPIB[_T("GPIBInterfaceID")] = cstrGPIBInterfaceID;
	}
	else
	{
		if (0 == m_mapGPIB[_T("UsingGPIB")].CompareNoCase(_T("TRUE")))
		{
			ret = LOAD_GPIB_SETTING_FAIL;
		}
	}

	XMLNode GPIBCardAddress = m_GPIBXML.SearchNode(_T("//Configuration//GPIB//GPIBCardAddress"));
	if (GPIBCardAddress != NULL)
	{
		CString cstrGPIBCardAddress = m_GPIBXML.GetNodeText(GPIBCardAddress);
		if (! cstrGPIBCardAddress.IsEmpty())
			m_mapGPIB[_T("GPIBCardAddress")] = cstrGPIBCardAddress;
	}
	else
	{
		if (0 == m_mapGPIB[_T("UsingGPIB")].CompareNoCase(_T("TRUE")))
		{
			ret = LOAD_GPIB_SETTING_FAIL;
		}
	}

	return ret;
}


size_t CGPIBHdrStd::SetToolGPIB()
{
	size_t ret = NOERROR;

	for(	std::map<CString, CString>::iterator iter = this->m_mapGPIB.begin();
		iter != this->m_mapGPIB.end();
		iter++)
	{
		if(!this->m_pITI->SetTestPart(IToolInterfaceV2::COMMON, L"", iter->first.GetString(), iter->second.GetString()))
		{
			ret = TOOL_SET_GPIB_FAIL;
			break;
		}
	}

	return ret;
}
