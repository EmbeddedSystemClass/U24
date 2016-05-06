#include "stdafx.h"
#include "ToolLoader.h"

using namespace ECIErrorCode::ToolLoader;

CToolLoader::CToolLoader(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC), m_hDLL(NULL)
{
}

CToolLoader::~CToolLoader()
{
}

size_t CToolLoader::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	ret = this->FreeToolDLL();
	
	if(NOERROR == ret)
		ret = this->LoadToolDLL();

	return ret;
}

size_t CToolLoader::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CToolLoader::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CToolLoader::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CToolLoader::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;

	ret = this->FreeToolDLL();

	return ret;
}

size_t CToolLoader::FreeToolDLL()
{
	if(m_hDLL && m_pITI)
	{
		p_fn_deleteToolInterface DelToolInterface = (p_fn_deleteToolInterface) GetProcAddress (m_hDLL, DLL_DEL_TOOLINTERFACE_NOTIFY);

		if ( !DelToolInterface )
		{
			return FREE_DLL_FAIL;
		}

		DelToolInterface(m_pITI);
		FreeLibrary(m_hDLL);

		m_pITI = NULL;
		m_hDLL = NULL;
	}

	return NOERROR;
}

size_t CToolLoader::LoadToolDLL()
{
	int nError = 0;
	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_BBTEST ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDEM ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDWRITE ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDWIFI_1 ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDWIFI_2 ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDWRITE_SN ||
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CSDREAD_SN)
	{
		SetCurrentDirectory(this->m_Parametermap[ParameterKeyWord::WORKINGDIR]);
		m_hDLL = ::LoadLibrary(_T("FTTool.dll"));

		if (NULL == m_hDLL)
		{
			nError = GetLastError();
			return LOAD_DLL_FAIL;
		}
	}

	else if( this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2GPTEST || 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_3GPTEST ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_4GPTEST ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_BTWLAN ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CURRENT ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_ONLINE_WLS ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_ONLINE_WLS2 ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == "SPRD" ||
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == "LTE")
	{
		SetCurrentDirectory(this->m_Parametermap[ParameterKeyWord::WORKINGDIR]);
		m_hDLL = ::LoadLibrary(_T("RFTool.dll"));

		if (NULL == m_hDLL)
		{
			nError = GetLastError();
			return LOAD_DLL_FAIL;
		}
	}

	else if( this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_CAMERA || 
		     this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_ALS_AUDIO || 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_AUDIO || 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_ALS|| 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_IMEI || 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_FCHECK|| 
			 this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_MMI_BB
			 ) 
	{
		CString strDll =  this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + _T("MMIBBTool.dll");
		m_hDLL = ::LoadLibrary(strDll);

		if (NULL == m_hDLL)
		{
			nError = GetLastError();
			return LOAD_DLL_FAIL;
		}
	}
/*
	else if(this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_OS_DL || 
		    this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_REDL ||
			this->m_Parametermap[_T("DLMODE")] == _T("ReDL"))
	{
		CString strDll =  this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + _T("DLTool.dll");

		m_hDLL = ::LoadLibrary(strDll);

		if (NULL == m_hDLL)
		{
			nError = GetLastError();
			return LOAD_DLL_FAIL;
		}
	}
*/
	p_fn_newToolInterfaceNotify NewToolInterface = (p_fn_newToolInterfaceNotify) GetProcAddress (m_hDLL, DLL_NEW_TOOLINTERFACE_NOTIFY);
	p_fn_deleteToolInterfaceNotify DelToolInterface = (p_fn_deleteToolInterfaceNotify) GetProcAddress (m_hDLL, DLL_DEL_TOOLINTERFACE_NOTIFY);

	if ( !(NewToolInterface &&  DelToolInterface ))
	{
		return LOAD_DLL_FAIL;
	}

	if (  !NewToolInterface (&m_pITI) )
	{
		return LOAD_DLL_FAIL;		
	}

	//Set log dir to tool dll here
	this->m_pITI->SetParameterValue(ParameterKeyWord::LOGDIR, this->m_Parametermap[ParameterKeyWord::LOGDIR].GetString());

	return NOERROR;
}

size_t CToolLoader::Enhance_SetResopnseToShowDlg(char* sz_showReslut)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_SetResopnseToShowDlg(sz_showReslut);
	if(NOERROR != ret)
		return ret;

	return ret;
}