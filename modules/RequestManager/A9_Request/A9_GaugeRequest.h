#ifndef __Gauge_REQUEST_H__
#define __Gauge_REQUEST_H__


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


	class RM_CLASS_API CA9GaugeReadIDRequest : public CDiagRequestBase
	{
	public:
		CA9GaugeReadIDRequest(bool& bSuccess, char *szID);
		virtual ~CA9GaugeReadIDRequest() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
		char *m_szID;
	};

	class RM_CLASS_API CA9GaugeupdateFWRequest : public CDiagRequestBase
	{
	public:
		CA9GaugeupdateFWRequest(bool& bSuccess, char *szRequireVer);
		virtual ~CA9GaugeupdateFWRequest() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
		int m_RequireVer;
	};

	class RM_CLASS_API CA9GaugeCheckThermalRequest : public CDiagRequestBase
	{
	public:
		CA9GaugeCheckThermalRequest(bool& bSuccess);
		virtual ~CA9GaugeCheckThermalRequest() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
	};
} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Gauge_REQUEST_H__
