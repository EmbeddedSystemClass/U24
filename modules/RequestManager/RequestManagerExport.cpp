#include "StdAfx.h"
#include "RequestManagerExport.h"
#include "RequestManagerInstance.h"
#include "RequestManager.h"

namespace PLATFORM_TOOL
{
	static CRequestManager g_CRequestManager;


	RM_API bool GetIRequestManager(CIRequestManagerPtr& ptIRequestManager,const TSTRING& strDevice, const std::string& strPlatform)
	{
		ptIRequestManager = new CRequestManagerInstance(strDevice,&g_CRequestManager,strPlatform);
		return true;
	}
}