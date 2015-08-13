#ifndef __OS_REQUEST_H__
#define __OS_REQUEST_H__


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


class RM_CLASS_API CA9SimLockGetControlKeyRequest : public CDiagRequestBase
{
public:
	CA9SimLockGetControlKeyRequest(bool& bSuccess);
	virtual ~CA9SimLockGetControlKeyRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9SimLockGetUnblockKeyRequest : public CDiagRequestBase
{
public:
	CA9SimLockGetUnblockKeyRequest(bool& bSuccess);
	virtual ~CA9SimLockGetUnblockKeyRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9SimLockWriteSimDataRequest : public CDiagRequestBase
{
public:
	CA9SimLockWriteSimDataRequest(bool& bSuccess, BYTE* szSimDataBuf, int szSize);
	virtual ~CA9SimLockWriteSimDataRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;

};


class RM_CLASS_API CA9SimLockDeactivateRequest : public CDiagRequestBase
{
public:
	CA9SimLockDeactivateRequest(bool& bSuccess, char* szControlKey);
	virtual ~CA9SimLockDeactivateRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9SimLockActivateRequest : public CDiagRequestBase
{
public:
	CA9SimLockActivateRequest(bool& bSuccess, char* szControlKey);
	virtual ~CA9SimLockActivateRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9SimLockGetSimDataRequest : public CDiagRequestBase
{
public:
	CA9SimLockGetSimDataRequest(bool& bSuccess);
	virtual ~CA9SimLockGetSimDataRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9SimLockEFsOpenRequest : public CDiagRequestBase
{
public:
	CA9SimLockEFsOpenRequest(bool& bSuccess, char* szFileName);
	virtual ~CA9SimLockEFsOpenRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9SimLockEFsReadRequest : public CDiagRequestBase
{
public:
	CA9SimLockEFsReadRequest(bool& bSuccess, char* szContent);
	virtual ~CA9SimLockEFsReadRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szContent;

};

class RM_CLASS_API CA9SimLockEFsCloseRequest : public CDiagRequestBase
{
public:
	CA9SimLockEFsCloseRequest(bool& bSuccess);
	virtual ~CA9SimLockEFsCloseRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;

};

class RM_CLASS_API CA9SimLockEFsDeleteRequest : public CDiagRequestBase
{
public:
	CA9SimLockEFsDeleteRequest(bool& bSuccess, char* szFileName);
	virtual ~CA9SimLockEFsDeleteRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9SimLockSimCardStatusRequest : public CDiagRequestBase
{
public:
	CA9SimLockSimCardStatusRequest(bool& bSuccess, char* szResponse);
	virtual ~CA9SimLockSimCardStatusRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResponse;
};

class RM_CLASS_API CA9_SimLock_DeleteFolder_mmgsdiRequest : public CDiagRequestBase
{
public:
	CA9_SimLock_DeleteFolder_mmgsdiRequest(bool& bSuccess);
	virtual ~CA9_SimLock_DeleteFolder_mmgsdiRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9_SimLock_GeneratePersonEngineRequest : public CDiagRequestBase
{
public:
	CA9_SimLock_GeneratePersonEngineRequest(bool& bSuccess);
	virtual ~CA9_SimLock_GeneratePersonEngineRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};
} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __OS_REQUEST_H__
