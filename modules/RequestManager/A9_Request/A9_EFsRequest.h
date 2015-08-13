#ifndef __EFs_REQUEST_H__
#define __EFs_REQUEST_H__


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


class RM_CLASS_API CA9EFsBackupRequest : public CDiagRequestBase
{
public:
	CA9EFsBackupRequest(bool& bSuccess);
	virtual ~CA9EFsBackupRequest() {}
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9EFsRestoreRequest : public CDiagRequestBase
{
public:
	CA9EFsRestoreRequest(bool& bSuccess);
	virtual ~CA9EFsRestoreRequest() {}
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9EFsClearRequest : public CDiagRequestBase
{
public:
	CA9EFsClearRequest(bool& bSuccess);
	virtual ~CA9EFsClearRequest() {}
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9EFsVerifyBackupTimesRequest : public CDiagRequestBase
{
public:
	CA9EFsVerifyBackupTimesRequest(char* szBackupTimes, bool& bSuccess);
	virtual ~CA9EFsVerifyBackupTimesRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szBackupTimes;
};



} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __EFs_REQUEST_H__
