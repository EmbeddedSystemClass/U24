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


	class RM_CLASS_API CDiagRAWRequest: public CDiagRequestBase
	{
	public:
		CDiagRAWRequest(BYTE* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn);
		virtual ~CDiagRAWRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};


	
	class RM_CLASS_API CMSPowerRequest: public CDiagRequestBase
	{
	public:
		CMSPowerRequest(int nPowerState);
		virtual ~CMSPowerRequest(){}		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
	};
	
	//DFI Temp location
	class RM_CLASS_API CDiagUpdateDFIFW: public CDiagRequestBase
	{
	public:
		CDiagUpdateDFIFW(bool& bSuccess, BYTE& iErrCode);
		virtual ~CDiagUpdateDFIFW() {}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool& m_bSuccess;
		BYTE& m_iErrCode;
	};

	// Wolf QFuse
	class RM_CLASS_API CWolfReadQFuse: public CDiagRequestBase
	{
	public:
		CWolfReadQFuse(bool& bSuccess, BYTE& iErrCode, BYTE* pRegister, long regsize);
		virtual ~CWolfReadQFuse() {}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool& m_bSuccess;
		BYTE* m_ConfigRegister;
		long  m_RegisterSize;
	};

	class RM_CLASS_API CWolfWriteQFuse: public CDiagRequestBase
	{
	public:
		CWolfWriteQFuse(bool& bSuccess, BYTE* pRegister, long bufsize);
		virtual ~CWolfWriteQFuse(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool& m_bSuccess;
	};
	class RM_CLASS_API CAustinFreezeOTPRequest  : public CDiagRequestBase
	{
	public:
		CAustinFreezeOTPRequest(int nRequestType, bool& bSuccess, int& nStatus);
		virtual ~CAustinFreezeOTPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nStatus;

	};

	class RM_CLASS_API CC7FreezeOTPRequest  : public CDiagRequestBase
	{
	public:
		CC7FreezeOTPRequest(int nRequestType, bool& bSuccess, int& nStatus);
		virtual ~CC7FreezeOTPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nStatus;

	};


}
