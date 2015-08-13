#pragma once
#include "IRequestManager.h"
#include "RequestManager.h"

namespace PLATFORM_TOOL
{
	class CRequestManagerInstance : public IRequestManager
	{
	public:
		CRequestManagerInstance(const TSTRING& strDevice,CRequestManager*	pCRequestManager,const std::string& strPlatform);
		virtual ~CRequestManagerInstance(void);
		
		virtual bool Open();
		virtual bool Close(bool bKeepPotocolType = false);
		virtual bool IsOpen() const;
		virtual const TSTRING& GetDevice();
		virtual bool Execuate(CRequestPtr& prREquest,const std::string& strPlatform);
		virtual void Reset();
		virtual void ResetMode();
		virtual PotocolType GetPotocolType();
		virtual void PopLastLog();

	private:
		TSTRING m_strDevice;
		std::string m_strPlatform;
		CRequestManager*	m_pCRequestManager;
	};
}