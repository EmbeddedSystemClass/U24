#ifndef __Fuse_REQUEST_H__
#define __Fuse_REQUEST_H__


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
	class RM_CLASS_API CA9FuseReadStatusRequest : public CDiagRequestBase
	{
	public:
		CA9FuseReadStatusRequest(char* unFuseType, bool& bSuccess);
		virtual ~CA9FuseReadStatusRequest() {}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool& m_bSuccess;
		char* m_szFuseStatus;
	};




	class RM_CLASS_API CA9QFuseBlowRequest : public CDiagRequestBase
	{
	public:
		CA9QFuseBlowRequest(char* szFuseType, bool& bSuccess);
		virtual ~CA9QFuseBlowRequest() {}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

	private:
		bool& m_bSuccess;
		char* m_szFuseStatus;
	};


} // End of namespace PLATFORM_TOOL


#endif // End of #ifndef __Fuse_REQUEST_H__
