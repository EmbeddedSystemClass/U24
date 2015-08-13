#ifndef __eMMC_REQUEST_H__
#define __eMMC_REQUEST_H__


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


class RM_CLASS_API CA9GeteMMCSizeRequest : public CDiagRequestBase
{
public:
	CA9GeteMMCSizeRequest(char* szeMMCSize, bool& bSuccess);
	virtual ~CA9GeteMMCSizeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szeMMCSize;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __eMMC_REQUEST_H__
