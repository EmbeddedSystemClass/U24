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

	#pragma pack(push, 1)
	typedef struct
	{
		BYTE Reserved[32];
	}struct_32byte;
	#pragma pack(pop)

	class RM_CLASS_API CLeopard_BT_FM_Request  : public CDiagRequestBase
	{
	public:
		CLeopard_BT_FM_Request (int commandCode, struct_32byte& buffer, bool& bResult);
		virtual ~CLeopard_BT_FM_Request(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:

		int  m_wCmdCode;
		bool& m_bResult;
		struct_32byte& m_InOutBuffer;
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


    class RM_CLASS_API CLeopardBTPowerOffRequest: public CDiagRequestBase
	{
	public:
		CLeopardBTPowerOffRequest (bool& bResult);
		virtual ~CLeopardBTPowerOffRequest(){}
	protected:
		void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	private:
		bool& m_bResult;
	};












	class RM_CLASS_API CWriteBTHeadsetAddress : public CDiagRequestBase 
	{
    public:
        CWriteBTHeadsetAddress(const char* BTAddress, int length,bool& bIsCmdSuccess);
        virtual ~CWriteBTHeadsetAddress();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        int HexToInt(char c);
        bool CheckCmdResult();
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }
	protected:
        bool& m_bIsCmdSuccess;
    private:
        char* m_pBTAddress;
        int m_nBTAddressLength;
    };
	
    class RM_CLASS_API CBTBound : public CWriteBTHeadsetAddress 
	{
    public:
        CBTBound(bool bBound,const char* szBTAddress,const char* szBTPass,bool& bIsCmdSuccess);
        virtual ~CBTBound();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) 
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    };

    class RM_CLASS_API CBTRequest : public CWriteBTHeadsetAddress 
	{
    public:
        CBTRequest(UINT nCommand,const char* szBTAddress,bool& bIsCmdSuccess);
        virtual ~CBTRequest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) 
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    };

	/*
	1. GAP command
	Commands used to test basic Bluetooth functionality.
	Request packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 01 00 for GAP command
	BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
	Response packet
	No response packet
	*/
	class RM_CLASS_API CEnableGAPRequest  : public CDiagRequestBase
	{
	public:
		CEnableGAPRequest(bool bEnableGap,bool& bResult);
		virtual ~CEnableGAPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		bool m_bEnableGap;
	};
	
	/*
	13. TXPOWER command
	Commands used to test TX power.
	Request packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0E 00 for TXPOWER command
	BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
	Response packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0E 00 for TXPOWER command
	err	1	Error code
	*/
	class RM_CLASS_API CBTTxPowerRequest  : public CDiagRequestBase
	{
	public:
		CBTTxPowerRequest(int nDegree,bool& bResult);
		virtual ~CBTTxPowerRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nDegree;
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
	

	/*
	16. TEST RESULT command
	Commands used to query the result of commands performed during factory testing. The TEST RESULT command is only for Lavernock.
	Request packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 18 00 for TEST RESULT command
	BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
	253: TEST RESULT command
	Response packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 18 00 for TEST RESULT command
	result	1	The response of this command, used to obtain the result of commands used during factory testing.
	0x0 represents no error occurs.
	0x1 represents bonding fails.
	0x2 represents HFG connection fails.
	0x4 represents HFG audio connection fails.
	This value resets to 0x0 when a bond command is issued.
	*/
	class RM_CLASS_API CBTQueryTestResultRequest  : public CDiagRequestBase
	{
	public:
		CBTQueryTestResultRequest(int& nResult,bool& bSuccess);
		virtual ~CBTQueryTestResultRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nResult;
		bool& m_bSuccess;
	};
	
	/*
	FM test mode
	*/
	class RM_CLASS_API CFMRequest  : public CDiagRequestBase
	{
	public:
		CFMRequest(WORD nTestCase,WORD nPayload1,WORD nPayload2,WORD& nRSSI,bool& bResult);
		virtual ~CFMRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		WORD m_nTestCase;
		WORD m_nPayload1;
		WORD m_nPayload2;
		WORD& m_nRSSI;
	};

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





}
