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


class RM_CLASS_API CA9OSResetToFastbootRequest : public CDiagRequestBase
{
public:
	CA9OSResetToFastbootRequest(bool& bSuccess);
	virtual ~CA9OSResetToFastbootRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9OSRebootRequest : public CDiagRequestBase
{
public:
	CA9OSRebootRequest(bool& bSuccess);
	virtual ~CA9OSRebootRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9ReadCodecIDRequest : public CDiagRequestBase
{
public:
	CA9ReadCodecIDRequest(char* szCodecID, bool& bSuccess);
	virtual ~CA9ReadCodecIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szCodecID;
};


class RM_CLASS_API CA9ReadModemVersionRequest : public CDiagRequestBase
{
public:
	CA9ReadModemVersionRequest(char* szModemVersion, bool& bSuccess);
	virtual ~CA9ReadModemVersionRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szModemVersion;
};

class RM_CLASS_API CA9ReadHWVersionRequest : public CDiagRequestBase
{
public:
	CA9ReadHWVersionRequest(char* szHWVersion, bool& bSuccess);
	virtual ~CA9ReadHWVersionRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szHWVersion;
};

class RM_CLASS_API CA9OSWriteMassProductionRequest : public CDiagRequestBase
{
public:
	CA9OSWriteMassProductionRequest(bool& bSuccess, char* szIsMP);
	virtual ~CA9OSWriteMassProductionRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9OSReadMassProductionRequest : public CDiagRequestBase
{
public:
	CA9OSReadMassProductionRequest(bool& bSuccess, char* szIsMP);
	virtual ~CA9OSReadMassProductionRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szIsMP;

};

} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __OS_REQUEST_H__
