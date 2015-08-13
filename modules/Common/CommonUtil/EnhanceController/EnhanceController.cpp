#include "stdafx.h"
#include "EnhanceController.h"


p_fn_ECICallback CEnhanceController::m_ECICallback				= NULL;
IToolInterface_Notify* CEnhanceController::m_pITI					= NULL;
std::vector<CString> CEnhanceController::m_vTestItemList;
std::map<CString, CString> CEnhanceController::m_Parametermap;

CEnhanceController::CEnhanceController() : m_pNextCEC(NULL)
{
}

CEnhanceController::CEnhanceController(CEnhanceController* pNextCEC)
{
	this->SetNextEC(pNextCEC);
}

CEnhanceController::~CEnhanceController()
{
	if(this->m_pNextCEC)
		delete this->m_pNextCEC;
}

size_t CEnhanceController::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	if(this->m_pNextCEC)
	{
		ret = this->m_pNextCEC->Enhance_OnAPPLaunch();
		if(NOERROR != ret)
			return ret;
	}

	return ret;
}

size_t CEnhanceController::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	if(this->m_pNextCEC)
	{
		ret = this->m_pNextCEC->Enhance_OnRunInit(i_slot);
		if(NOERROR != ret)
			return ret;
	}

	return ret;
}

size_t CEnhanceController::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	if(this->m_pNextCEC)
	{
		ret = this->m_pNextCEC->Enhance_RunTest(i_slot);
		if(NOERROR != ret)
			return ret;
	}

	return ret;
}

size_t CEnhanceController::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	if(this->m_pNextCEC)
	{
		ret = this->m_pNextCEC->Enhance_OnRunFinish(i_slot);
		if(NOERROR != ret)
			return ret;
	}

	return ret;
}

size_t CEnhanceController::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	if(this->m_pNextCEC)
	{
		ret = this->m_pNextCEC->Enhance_OnAPPEnd();
		if(NOERROR != ret)
			return ret;
	}

	return ret;
}

void CEnhanceController::SetNextEC(CEnhanceController *pCEC) 
{
	this->m_pNextCEC = pCEC;
}

void CEnhanceController::FreeNextEC() 
{
	if(this->m_pNextCEC)
	{
		delete this->m_pNextCEC;
		this->m_pNextCEC = NULL;
	}
}

void CEnhanceController::SendMsg(const TCHAR *MsgType, LPARAM Msg)
{
	if(this->m_ECICallback)
		this->m_ECICallback(MsgType, Msg);
}

void CEnhanceController::SetECICallback(p_fn_ECICallback cb)
{
	if(cb)
		this->m_ECICallback = cb;
}