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



	class RM_CLASS_API CLeopardGetSDcardSizeRequest: public CDiagRequestBase
	{
	public:
		CLeopardGetSDcardSizeRequest (DWORD &dwSize, bool &bSuccess);
		virtual ~CLeopardGetSDcardSizeRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		DWORD &m_Size;
		bool  &m_Success;
	};

	class RM_CLASS_API CLeopardGetSDcardStatusRequest: public CDiagRequestBase
	{
	public:
		CLeopardGetSDcardStatusRequest (DWORD &dwSize, bool &bSuccess);
		virtual ~CLeopardGetSDcardStatusRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD &m_Status;
		bool  &m_Success;
	};

	class RM_CLASS_API CLeopardGetSDcardSizenStatusRequest_Sys: public CDiagRequestBase
	{
	public:
		CLeopardGetSDcardSizenStatusRequest_Sys (DWORD &dwSize,DWORD &dwStatus, bool &bSuccess);
		virtual ~CLeopardGetSDcardSizenStatusRequest_Sys(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		DWORD &m_Size;
		DWORD &m_Status;
		bool  &m_Success;
	};



} //PLATFORM_TOOL