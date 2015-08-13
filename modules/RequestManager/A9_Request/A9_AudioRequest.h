#ifndef __Camera_REQUEST_H__
#define __Camera_REQUEST_H__


#include "..\Request\QualcommRequest.h"


#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif


namespace PLATFORM_TOOL
{


class RM_CLASS_API CA9AudioDetectEarphoneRequest : public CDiagRequestBase
{
public:
	CA9AudioDetectEarphoneRequest(bool& bSuccess);
	virtual ~CA9AudioDetectEarphoneRequest() {}
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

private:
	bool& m_bSuccess;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Camera_REQUEST_H__
