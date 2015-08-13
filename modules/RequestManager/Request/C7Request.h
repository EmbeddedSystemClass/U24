#ifndef __EFSREQUEST_H__
#define __EFSREQUEST_H__

#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "stdio.h"


#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
    class RM_CLASS_API CC7NVBackupRequest  : public CDiagRequestBase
    {
    public:
        CC7NVBackupRequest(bool &bSuccess);
        virtual ~CC7NVBackupRequest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    private:
        bool  &m_Success;
    };

	class RM_CLASS_API CC7NVRestoreRequest  : public CDiagRequestBase
	{
	public:
		CC7NVRestoreRequest(bool &bSuccess);
		virtual ~CC7NVRestoreRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool  &m_Success;
	};

	  class RM_CLASS_API CC7DRMBackupRequest  : public CDiagRequestBase
    {
    public:
        CC7DRMBackupRequest(bool &bSuccess);
        virtual ~CC7DRMBackupRequest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    private:
        bool  &m_Success;
    };
}





#endif