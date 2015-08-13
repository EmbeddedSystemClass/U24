#ifndef __GPS_REQUEST_H__
#define __GPS_REQUEST_H__


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


class RM_CLASS_API CA9GPSCMDModeOnRequest : public CDiagRequestBase
{
public:
	CA9GPSCMDModeOnRequest(bool& bSuccess, bool bOn);
	virtual ~CA9GPSCMDModeOnRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9GPSCMDModeOnRequest_V2 : public CDiagRequestBase
{
public:
	CA9GPSCMDModeOnRequest_V2(bool& bSuccess);
	virtual ~CA9GPSCMDModeOnRequest_V2() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};


class RM_CLASS_API CA9GNSSRFMultichannelTrackingRequest : public CDiagRequestBase
{
public:
	CA9GNSSRFMultichannelTrackingRequest(bool& bSuccess);
	virtual ~CA9GNSSRFMultichannelTrackingRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9GNSSRFMultichannelTrackingRequest_V2 : public CDiagRequestBase
{
public:
	CA9GNSSRFMultichannelTrackingRequest_V2(bool& bSuccess);
	virtual ~CA9GNSSRFMultichannelTrackingRequest_V2() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
};

class RM_CLASS_API CA9GetGPSCNoRequest : public CDiagRequestBase
{
public:
	CA9GetGPSCNoRequest(char* szGPSCNo, bool& bSuccess);
	virtual ~CA9GetGPSCNoRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szGPSCNo;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __GPS_REQUEST_H__
