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
	class RM_CLASS_API CGetePINStatusRequest: public CDiagRequestBase
	{
	public:
		CGetePINStatusRequest (bool &bSuccess);
		virtual ~CGetePINStatusRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool  &m_Success;
	};

	class RM_CLASS_API CA9PhoneInfoWriteDefaultIMEIRequest: public CDiagRequestBase
	{
	public:
		CA9PhoneInfoWriteDefaultIMEIRequest (bool &bSuccess, char* szIMEI);
		virtual ~CA9PhoneInfoWriteDefaultIMEIRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool& m_bSuccess;
		char* m_szIMEI;
	};


}