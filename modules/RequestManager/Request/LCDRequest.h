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


class RM_CLASS_API CWolfLCDOnOffSwitchRequest: public CDiagRequestBase
{
public:
	CWolfLCDOnOffSwitchRequest(bool& bResult, bool bOn);
	virtual ~CWolfLCDOnOffSwitchRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
	}

private:
	bool& m_Success;
	bool m_bOn;
};


} // End of namespace PLATFORM_TOOL
