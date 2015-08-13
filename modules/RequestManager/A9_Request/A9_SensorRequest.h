#ifndef __Sensor_REQUEST_H__
#define __Sensor_REQUEST_H__


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


class RM_CLASS_API CA9ReadSensorIDRequest : public CDiagRequestBase
{
public:
	CA9ReadSensorIDRequest(char* szG, char* szGyro, char* szECompas, char* szPL, bool& bSuccess);
	virtual ~CA9ReadSensorIDRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szG;
	char* m_szGyro;
	char* m_szECompas;
	char* m_szPL;
};




class RM_CLASS_API CA9SensorPSensorCalibrationRequest : public CDiagRequestBase
{
public:
	CA9SensorPSensorCalibrationRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorPSensorCalibrationRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};
class RM_CLASS_API CA9SensorPSensorTestRequest : public CDiagRequestBase
{
public:
	CA9SensorPSensorTestRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorPSensorTestRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};
class RM_CLASS_API CA9SensorPSensorFarTestRequest : public CDiagRequestBase
{
public:
	CA9SensorPSensorFarTestRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorPSensorFarTestRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};


class RM_CLASS_API CA9SensorLSensorCalibrationRequest : public CDiagRequestBase
{
public:
	CA9SensorLSensorCalibrationRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorLSensorCalibrationRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};
class RM_CLASS_API CA9SensorLSensorTestRequest : public CDiagRequestBase
{
public:
	CA9SensorLSensorTestRequest(bool& bSuccess, char* szResult, char* szRange);
	virtual ~CA9SensorLSensorTestRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};



class RM_CLASS_API CA9SensorGSensorCalibrationRequest : public CDiagRequestBase
{
public:
	CA9SensorGSensorCalibrationRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorGSensorCalibrationRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};
class RM_CLASS_API CA9SensorGSensorTestRequest : public CDiagRequestBase
{
public:
	CA9SensorGSensorTestRequest(bool& bSuccess, char* szResult);
	virtual ~CA9SensorGSensorTestRequest() {}
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

private:
	bool& m_bSuccess;
	char* m_szResult;
};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Sensor_REQUEST_H__
