#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{

	class RM_CLASS_API CLeopardGSensorRequest: public CDiagRequestBase
	{
	public:
		CLeopardGSensorRequest(bool &bSuccess);
		virtual ~CLeopardGSensorRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};


	class RM_CLASS_API CLeopardGSensorCalibrationRequest: public CDiagRequestBase
	{
	public:
		CLeopardGSensorCalibrationRequest(bool &bSuccess);
		virtual ~CLeopardGSensorCalibrationRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CWolfGSensorIsCalibratedRequest: public CDiagRequestBase
	{
	public:
		CWolfGSensorIsCalibratedRequest(bool &bSuccess, BYTE& nByteParameter1, BYTE& nByteParameter2, BYTE& nByteParameter3);
		virtual ~CWolfGSensorIsCalibratedRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
		BYTE& m_nByteParameter1;
		BYTE& m_nByteParameter2;
		BYTE& m_nByteParameter3;
	};

	class RM_CLASS_API CWolfGSensorSelfTestRequest: public CDiagRequestBase
	{
	public:
		CWolfGSensorSelfTestRequest(bool &bSuccess);
		virtual ~CWolfGSensorSelfTestRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CLeopardECompassRequest: public CDiagRequestBase
	{
	public:
		CLeopardECompassRequest(bool &bSuccess);
		virtual ~CLeopardECompassRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};


	class RM_CLASS_API CLeopardIOControlRequest: public CDiagRequestBase
	{
	public:
		CLeopardIOControlRequest(int nFunction,BYTE& nByteParameter1,BYTE& nByteParameter2,BYTE& nByteParameter3,BYTE& nByteParameter4,int& nValue,bool& bSuccess);
		virtual ~CLeopardIOControlRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nValue;
		int m_nFunction;
		BYTE& m_nByteParameter1;
		BYTE& m_nByteParameter2;
		BYTE& m_nByteParameter3;
		BYTE& m_nByteParameter4;
	};

	class RM_CLASS_API CGSensorRequest  : public CDiagRequestBase
	{
	public:
		CGSensorRequest(int nFunction,int& nTemp, bool& bSuccess);
		CGSensorRequest(int& m_nX,int& m_nY,int& m_nZ,  bool& bSuccess);
		CGSensorRequest(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ);
		CGSensorRequest(bool& bIsGSensorWork,bool& bIsUpWork,int& nTemp);
		virtual ~CGSensorRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nVersion;
		bool &m_bSuccess;
		int& m_nX;
		int& m_nY;
		int& m_nZ;
		bool& m_bIsGSensorWork;
		bool& m_bIsUpWork;
	};

	class RM_CLASS_API CGetRFComponentIDRequest: public CDiagRequestBase
	{
	public:
		CGetRFComponentIDRequest(int nID,int nSubID,std::string& strID,bool& bSuccess);
		virtual ~CGetRFComponentIDRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nID;
		int m_nSubID;
		std::string& m_strID;
		bool& m_bSuccess;
	};

	class RM_CLASS_API CGetComponentIDRequest: public CDiagRequestBase
	{
	public:
		CGetComponentIDRequest(int nID,int nSubID,std::string& strID,bool& bSuccess);
		virtual ~CGetComponentIDRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nID;
		int m_nSubID;
		std::string& m_strID;
		bool& m_bSuccess;
	};




}