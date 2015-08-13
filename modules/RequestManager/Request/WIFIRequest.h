#ifndef __WIFIREQUEST__H__
#define __WIFIREQUEST__H__

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
	class RM_CLASS_API CWolf_WiFiModuleSwitchRequest : public CDiagRequestBase
	{
	public:
		CWolf_WiFiModuleSwitchRequest(bool bOn, bool& bResult);
		virtual ~CWolf_WiFiModuleSwitchRequest() {}

	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool m_bOn;
		bool& m_bResult;
	};

	class RM_CLASS_API CWolf_WiFiModuleCheckRequest : public CDiagRequestBase
	{
	public:
		CWolf_WiFiModuleCheckRequest(bool& bOn, bool& bResult);
		virtual ~CWolf_WiFiModuleCheckRequest() {}

	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bOn;
		bool& m_bResult;
	};




	class RM_CLASS_API CLeopardChangeWifiFWtoNativeRequest: public CDiagRequestBase
	{
		public:
		CLeopardChangeWifiFWtoNativeRequest (bool& bResult);
		virtual ~CLeopardChangeWifiFWtoNativeRequest(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CLeopardWifiTestRequest  : public CDiagRequestBase
	{
	public:
		CLeopardWifiTestRequest (BYTE rate, BYTE channel, BYTE power, bool &bSuccess);
		virtual ~CLeopardWifiTestRequest(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CLeopardTurnOnTXPowerRequest: public CDiagRequestBase
	{
		public:
		CLeopardTurnOnTXPowerRequest (BYTE nChannel, bool& bResult);
		virtual ~CLeopardTurnOnTXPowerRequest(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CLeopardTurnOnTXPower2Request: public CDiagRequestBase
	{
		public:
		CLeopardTurnOnTXPower2Request (BYTE Rate, BYTE nChannel, BYTE Power, bool& bResult);
		virtual ~CLeopardTurnOnTXPower2Request(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CLeopardTurnOnTXPower2ChNRequest: public CDiagRequestBase
	{
		public:
		CLeopardTurnOnTXPower2ChNRequest (BYTE Rate, BYTE nChannel, BYTE Power, bool& bResult);
		virtual ~CLeopardTurnOnTXPower2ChNRequest(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};


	class RM_CLASS_API CLeopardTurnOnRXPower2Request: public CDiagRequestBase
	{
		public:
		CLeopardTurnOnRXPower2Request (BYTE Rate, BYTE nChannel, int &data1, int &data2, int &data3);
		virtual ~CLeopardTurnOnRXPower2Request(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		int& m_data1;
		int& m_data2;
		int& m_data3;
	};

	class RM_CLASS_API CLeopardTurnOnRXPower2ChNRequest: public CDiagRequestBase
	{
		public:
		CLeopardTurnOnRXPower2ChNRequest (BYTE Rate, BYTE nChannel, int &data1, int &data2, int &data3);
		virtual ~CLeopardTurnOnRXPower2ChNRequest(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		int& m_data1;
		int& m_data2;
		int& m_data3;
	};


	class RM_CLASS_API CWolfReadMACRequest: public CDiagRequestBase
	{
		public:
		CWolfReadMACRequest (char* address, bool& bResult);
		virtual ~CWolfReadMACRequest(){}
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
		char*  m_MACaddress;
	};





	/*
	class RM_CLASS_API CWLANDownLoadTFirmRequest: public CDiagRequestBase
	{
	public:
		CWLANDownLoadTFirmRequest(bool& bResult);
		virtual ~CWLANDownLoadTFirmRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CWLANPowerDownRequest: public CDiagRequestBase
	{
	public:
		CWLANPowerDownRequest(bool& bResult);
		virtual ~CWLANPowerDownRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CWLANStartPowerRequest: public CDiagRequestBase
	{
		public:
		CWLANStartPowerRequest(bool bTX,int nPower,int nChannel,int iReserved,int iReserved2,bool& bResult);
		virtual ~CWLANStartPowerRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		int m_iPower;
		int m_nChannel;
		int m_iReserved;
		int m_iReserved2;
		bool m_bTX;
	};
	*/
}

#endif