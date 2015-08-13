#ifndef __MHL_REQUEST_H__
#define __MHL_REQUEST_H__


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


class RM_CLASS_API CReadMHLIDRequest : public CDiagRequestBase
{
public:
	CReadMHLIDRequest(char* szMHLID, bool& bSuccess);
	virtual ~CReadMHLIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szMHLID;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __MHL_REQUEST_H__
