#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL{

	class RM_CLASS_API CDiagUpdateTouchFW: public CDiagRequestBase
	{
	public:
		CDiagUpdateTouchFW();
		virtual ~CDiagUpdateTouchFW();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CDiagCalibrateTouch: public CDiagRequestBase
	{
	public:
		CDiagCalibrateTouch(bool &bSuccess);
		virtual ~CDiagCalibrateTouch();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CDiagGetTouchStream: public CDiagRequestBase
	{
	public:
		CDiagGetTouchStream(char *szTouchStream);
		virtual ~CDiagGetTouchStream(){};
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		char *m_TouchStream;
	};

	class RM_CLASS_API CDiagGetTouchDelta: public CDiagRequestBase
	{
	public:
		CDiagGetTouchDelta(char *szTouchDelta);
		virtual ~CDiagGetTouchDelta(){};
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		char *m_TouchDelta;
	};

	class RM_CLASS_API CDiagGetTouchIDAC: public CDiagRequestBase
	{
	public:
		CDiagGetTouchIDAC(char *szTouchIDAC);
		virtual ~CDiagGetTouchIDAC(){};
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		char *m_TouchIDAC;
	};

	class RM_CLASS_API CDiagGetTouchBaseLine: public CDiagRequestBase
	{
	public:
		CDiagGetTouchBaseLine(char *szTouchBaseLine);
		virtual ~CDiagGetTouchBaseLine(){};
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		char *m_TouchBaseLine;
	};

	class RM_CLASS_API CDiagTouchEnableSensor: public CDiagRequestBase
	{
	public:
		CDiagTouchEnableSensor(bool &bSuccess);
		virtual ~CDiagTouchEnableSensor(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CDiagTouchDisableSensor: public CDiagRequestBase
	{
	public:
		CDiagTouchDisableSensor(bool &bSuccess);
		virtual ~CDiagTouchDisableSensor(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CDiagTouchEnterTestPage: public CDiagRequestBase
	{
	public:
		CDiagTouchEnterTestPage();
		virtual ~CDiagTouchEnterTestPage(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CDiagTouchCheckVersion: public CDiagRequestBase
	{
	public:
		CDiagTouchCheckVersion(char &TouchVersion, bool &bSuccess);
		virtual ~CDiagTouchCheckVersion(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		char &m_TouchVersion;
		bool  &m_Success;
	};

	class RM_CLASS_API CDiagTouchClearStreamBuffer: public CDiagRequestBase
	{
	public:
		CDiagTouchClearStreamBuffer(bool &bSuccess);
		virtual ~CDiagTouchClearStreamBuffer(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		bool  &m_Success;
	};
}