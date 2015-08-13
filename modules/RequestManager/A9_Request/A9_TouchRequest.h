#ifndef __Touch_REQUEST_H__
#define __Touch_REQUEST_H__


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


class RM_CLASS_API CA9TouchCalibrationRequest : public CDiagRequestBase
{
public:
	CA9TouchCalibrationRequest(bool& bSuccess);
	virtual ~CA9TouchCalibrationRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchIDACRequest : public CDiagRequestBase
{
public:
	CA9TouchIDACRequest(bool& bSuccess, BYTE Min = 0x01, BYTE Max = 0xFE);
	virtual ~CA9TouchIDACRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchBaselineRequest : public CDiagRequestBase
{
public:
	CA9TouchBaselineRequest(bool& bSuccess, BYTE Min = 0x3C, BYTE Max = 0x8C);
	virtual ~CA9TouchBaselineRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchDeltaRequest : public CDiagRequestBase
{
public:
	CA9TouchDeltaRequest(bool& bSuccess, BYTE Min = 0x3C, BYTE Max = 0x8C);
	virtual ~CA9TouchDeltaRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchDisableRequest : public CDiagRequestBase
{
public:
	CA9TouchDisableRequest(bool& bSuccess);
	virtual ~CA9TouchDisableRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchEnableRequest : public CDiagRequestBase
{
public:
	CA9TouchEnableRequest(bool& bSuccess);
	virtual ~CA9TouchEnableRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9TouchFWUpdateRequest : public CDiagRequestBase
{
public:
	CA9TouchFWUpdateRequest(bool& bSuccess);
	virtual ~CA9TouchFWUpdateRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9ReadTouchVersionRequest : public CDiagRequestBase
{
public:
	CA9ReadTouchVersionRequest(char* szVersion, bool& bSuccess);
	virtual ~CA9ReadTouchVersionRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szVersion;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Touch_REQUEST_H__
