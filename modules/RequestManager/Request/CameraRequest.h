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


// Camera function for wolf
#define CAMERA_KEY_EVENT                   1
#define CAMERA_TOUCH_EVENT                 2
#define CAMERA_FOCUS_EVENT                 3
#define CAMERA_BACKKEY_EVENT               4
#define CAMERA_COPY_FROM_WMFS_TO_EFS       5
#define CHECK_SNAPSHOT_STATUS              6
#define CAMERA_CHECK_COPY_STATUS		   12
// reserve 7-10
#define CHECK_PREVIEW_STATUS               11
#define CAMERA_COPY_FROM_WMFS_TO_EFS_STATUS 12
#define CAMERA_TOUCH_CENTER_EVENT			15





namespace PLATFORM_TOOL
{
	class RM_CLASS_API CWolfCameraRequest  : public CDiagRequestBase
	{
	public:
		CWolfCameraRequest (bool &bSuccess, BYTE funcIndex);
		virtual ~CWolfCameraRequest(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CWolfCameraAFStatusRequest  : public CDiagRequestBase
	{
	public:
		CWolfCameraAFStatusRequest (bool &bSuccess);
		virtual ~CWolfCameraAFStatusRequest(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool &m_bSuccess;
	};

}
