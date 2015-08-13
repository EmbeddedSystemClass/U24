#ifndef __FMREQUEST__H__
#define __FMREQUEST__H__

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
	class RM_CLASS_API CFMRequest  : public CDiagRequestBase
	{
	public:
		CFMRequest(WORD nTestCase,WORD nPayload1,WORD nPayload2,WORD& nRSSI,bool& bResult);
		virtual ~CFMRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		WORD m_nTestCase;
		WORD m_nPayload1;
		WORD m_nPayload2;
		WORD& m_nRSSI;
	};
}
#endif