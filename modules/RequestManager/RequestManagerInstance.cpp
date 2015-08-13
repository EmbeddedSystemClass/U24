#include "StdAfx.h"
#include "RequestManagerInstance.h"

namespace PLATFORM_TOOL
{
	CRequestManagerInstance::CRequestManagerInstance(const TSTRING& strDevice,CRequestManager*	pCRequestManager,const std::string& strPlatform)
		:m_strDevice(strDevice)
		,m_strPlatform(strPlatform)
		,m_pCRequestManager(pCRequestManager)
	{
		if(m_pCRequestManager)
			m_pCRequestManager->AddRef(strDevice);
	}

	CRequestManagerInstance::~CRequestManagerInstance(void)
	{
		if(m_pCRequestManager)
			m_pCRequestManager->ReleaseRef(m_strDevice);
	}
	
	bool CRequestManagerInstance::Open()
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Open(m_strDevice);
		return bRes;
	}

	bool CRequestManagerInstance::Close(bool bKeepPotocolType)
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Close(m_strDevice,bKeepPotocolType);
		return bRes;
	}

	bool CRequestManagerInstance::IsOpen() const
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->IsOpen(m_strDevice);
		return bRes;
	}

	bool CRequestManagerInstance::Execuate(CRequestPtr& prREquest,const std::string& strPlatform)
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Execuate(m_strDevice,prREquest,strPlatform);
		return bRes;
	}

	const TSTRING& CRequestManagerInstance::GetDevice()
	{
		return m_strDevice;
	}

	void CRequestManagerInstance::Reset()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->Reset(m_strDevice);
	}
	void CRequestManagerInstance::ResetMode()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->ResetMode(m_strDevice);
	}

	PotocolType CRequestManagerInstance::GetPotocolType()
	{
		PotocolType type = NONE;
		if(m_pCRequestManager)
			type = m_pCRequestManager->GetPotocolType(m_strDevice);
		return type;
	}

	void CRequestManagerInstance::PopLastLog()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->PopLastLog(m_strDevice);
	}
}