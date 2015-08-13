#ifndef __NFC_REQUEST_H__
#define __NFC_REQUEST_H__


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


class RM_CLASS_API CA9NFCIDRequest : public CDiagRequestBase
{
public:
	CA9NFCIDRequest(char* szNFCID, bool& bSuccess);
	virtual ~CA9NFCIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szNFCID;
};



class RM_CLASS_API CA9NFCCardIDRequest : public CDiagRequestBase
{
public:
	CA9NFCCardIDRequest(char* szNFCCardID, bool& bSuccess);
	virtual ~CA9NFCCardIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szNFCCardID;
};





} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __NFC_REQUEST_H__
