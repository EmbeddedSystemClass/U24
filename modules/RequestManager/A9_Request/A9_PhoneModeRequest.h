#ifndef __PhoneMode_REQUEST_H__
#define __PhoneMode_REQUEST_H__


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


class RM_CLASS_API CA9PhoneModeOfflineModeRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeOfflineModeRequest(bool& bSuccess);
	virtual ~CA9PhoneModeOfflineModeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9PhoneModeFTMModeRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeFTMModeRequest(bool& bSuccess);
	virtual ~CA9PhoneModeFTMModeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9PhoneModeOnlineModeRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeOnlineModeRequest(bool& bSuccess);
	virtual ~CA9PhoneModeOnlineModeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9PhoneModeLPMModeRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeLPMModeRequest(bool& bSuccess);
	virtual ~CA9PhoneModeLPMModeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9PhoneModeOfflineModeResetRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeOfflineModeResetRequest(bool& bSuccess);
	virtual ~CA9PhoneModeOfflineModeResetRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9PhoneModeReadModeTypeRequest : public CDiagRequestBase
{
public:
	CA9PhoneModeReadModeTypeRequest(char* szModeType, bool& bSuccess);
	virtual ~CA9PhoneModeReadModeTypeRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
private:
	bool& m_bSuccess;
	char* m_szModeType;

};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __PhoneMode_REQUEST_H__
