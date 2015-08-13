#ifndef __LEDREQUEST__H__
#define __LEDREQUEST__H__

#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	class RM_CLASS_API CNLEDRequest  : public CDiagRequestBase
	{
	public:
		CNLEDRequest(bool &result);
		virtual ~CNLEDRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
	};
}
#endif