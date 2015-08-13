#ifndef __BTREQUEST__H__
#define __BTREQUEST__H__

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
	#pragma pack(push, 1)
	typedef struct
	{
		BYTE Reserved[32];
	}struct_32byte;
	#pragma pack(pop)

	class RM_CLASS_API CWolf_BTModuleSwitchRequest : public CDiagRequestBase
	{
	public:
		CWolf_BTModuleSwitchRequest(bool bOn, bool& bResult);
		virtual ~CWolf_BTModuleSwitchRequest() {}

	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CLeopard_BT_FM_Request  : public CDiagRequestBase
	{
	public:
		CLeopard_BT_FM_Request (int commandCode, struct_32byte& buffer, bool& bResult);
		virtual ~CLeopard_BT_FM_Request(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived){
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		int  m_wCmdCode;
		bool& m_bResult;
		struct_32byte& m_InOutBuffer;
	};

	class RM_CLASS_API CLeopardBTPowerOffRequest: public CDiagRequestBase
	{
	public:
		CLeopardBTPowerOffRequest (bool& bResult);
		virtual ~CLeopardBTPowerOffRequest(){}
	protected:
		void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived){
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};

	/*
	Bluetooth test mode
	*/
	class RM_CLASS_API CBTTestModeRequest  : public CDiagRequestBase
	{
	public:
		CBTTestModeRequest(bool bEnetr,bool& bResult);
		virtual ~CBTTestModeRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		bool m_bEnetr;
	};
}

#endif