#ifndef __Power_REQUEST_H__
#define __Power_REQUEST_H__


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


class RM_CLASS_API CA9ReadBatteryIDRequest : public CDiagRequestBase
{
public:
	CA9ReadBatteryIDRequest(char* szPowerID, bool& bSuccess);
	virtual ~CA9ReadBatteryIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szPowerID;
};


class RM_CLASS_API CA9ChargerEnableRequest : public CDiagRequestBase
{
public:
	CA9ChargerEnableRequest(bool& bSuccess);
	virtual ~CA9ChargerEnableRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9ChargerDisableRequest : public CDiagRequestBase
{
public:
	CA9ChargerDisableRequest(bool& bSuccess);
	virtual ~CA9ChargerDisableRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9ReadBatteryVoltageRequest : public CDiagRequestBase
{
public:
	CA9ReadBatteryVoltageRequest(char* szPowerID, bool& bSuccess);
	virtual ~CA9ReadBatteryVoltageRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szBatteryVoltage;
};


class RM_CLASS_API CA9SendPowerKeyEventRequest : public CDiagRequestBase
{
public:
	CA9SendPowerKeyEventRequest(bool& bSuccess, BYTE szDelayTime);
	virtual ~CA9SendPowerKeyEventRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9WakeLockRequest : public CDiagRequestBase
{
public:
	CA9WakeLockRequest(bool& bSuccess);
	virtual ~CA9WakeLockRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9WakeUnlockRequest : public CDiagRequestBase
{
public:
	CA9WakeUnlockRequest(bool& bSuccess);
	virtual ~CA9WakeUnlockRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Power_REQUEST_H__
