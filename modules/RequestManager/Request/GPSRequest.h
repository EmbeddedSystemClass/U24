#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif


typedef struct
{
	int Lobound;
	int Upbound;
	int packetCount;
	WORD Ivalue; //out
	WORD Qvalue; //out
} packet_0x1372_structure;




namespace PLATFORM_TOOL
{

	/*****************************************************/
	/*	GPS   CLeopardReadIQValueRequest                  */
    /******************************************************/
	class RM_CLASS_API CLeopardReadIQValueRequest: public CDiagRequestBase
	{
	public:
		CLeopardReadIQValueRequest (bool& bSuccess, WORD& Ivalue, WORD& Qvalue);
		virtual ~CLeopardReadIQValueRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		WORD &m_wIvalue;
		WORD &m_wQvalue;
	};

	/*****************************************************/
	/*	GPS   CLeopardGetCNOvalueRequest                  */
    /******************************************************/
	class RM_CLASS_API CLeopardGetCNOvalueRequest: public CDiagRequestBase
	{
	public:
		CLeopardGetCNOvalueRequest (WORD &CNO, bool& bSuccess);
		virtual ~CLeopardGetCNOvalueRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		WORD& m_CNO;
	};



	/******************************************************/
	/*	GPS   CGPS_SA_RF_VERIF_MODE_SWITCHRequest         */
    /******************************************************/
	class RM_CLASS_API CGPS_SA_RF_VERIF_MODE_SWITCHRequest: public CDiagRequestBase
	{
	public:
		CGPS_SA_RF_VERIF_MODE_SWITCHRequest (bool bEnter, bool &bSuccess);
		virtual ~CGPS_SA_RF_VERIF_MODE_SWITCHRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)	{return CDiagRequestBase::IsCompletePacket(ptBufferReceived);}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;

	};

	/******************************************************/
	/*	GPS   CReadIQValueRequest                         */
    /******************************************************/
	class RM_CLASS_API CReadIQValueRequest: public CDiagRequestBase
	{
	public:
		CReadIQValueRequest(int& nIValue,int& nQValue,bool& bSuccess);
		virtual ~CReadIQValueRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nIValue;
		int& m_nQValue;
		bool& m_bSuccess;
	};

	/*****************************************************/
	/*	GPS   CGPS_SV_TRACKRequest                        */
    /******************************************************/
	class RM_CLASS_API CGPS_SV_TRACKRequest: public CDiagRequestBase
	{
	public:
		CGPS_SV_TRACKRequest (BYTE svNumber, BYTE cmd, bool &bSuccess);
		virtual ~CGPS_SV_TRACKRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)	{return CDiagRequestBase::IsCompletePacket(ptBufferReceived);}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;

	};
	class RM_CLASS_API CEnableGPSLog: public CDiagRequestBase
	{
	public:
		CEnableGPSLog(bool bEnable, DWORD packetNo, bool& bSuccess);
		virtual ~CEnableGPSLog(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;

	};

	class RM_CLASS_API CGPSEnterStandaloneRFRequest: public CDiagRequestBase
	{
	public:
		CGPSEnterStandaloneRFRequest(BYTE ByteEnterMode, DWORD packetNo, bool& bSuccess, int& status, void *pOutput=NULL, DWORD dwBufferSize=0, bool bWaitLog=true);
		virtual ~CGPSEnterStandaloneRFRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:

		bool& m_bSuccess;
		bool m_bWaitLog;
		int& m_status; //0=IO error, 1=success, 2=value error.
		int m_counter;
		int m_CmdType;
		//BYTE Reserved[3];
		DWORD m_packetNo;
		bool m_bFirstCommandResponseSuccess;
		CBufferPtr m_ptTempBufferReceived;

		packet_0x1372_structure *m_packet1372; //packet 0x1372 data
	};



	class RM_CLASS_API CGPSSVTrack_LogRequest : public CDiagRequestBase
	{
	public:
		CGPSSVTrack_LogRequest(BYTE ByteSV, int CmdType, bool& bSuccess, WORD wThreshold, WORD &plastCNO, bool bWaitLog);
		virtual ~CGPSSVTrack_LogRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		bool m_bWaitLog;
		int  m_CmdType;
		WORD m_wThreshold;
		WORD &m_lastCNO;

		CBufferPtr m_ptTempBufferReceived;
	};

	class RM_CLASS_API CGPSTrackingSwitchRequest : public CDiagRequestBase
	{
	public:
		CGPSTrackingSwitchRequest(bool& bSuccess, bool bOn);
		virtual ~CGPSTrackingSwitchRequest() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CGPSPollingTTFFCN0Request : public CDiagRequestBase
	{
	public:
		CGPSPollingTTFFCN0Request(bool& bSuccess, int& iTTFF, int& iCN0);
		virtual ~CGPSPollingTTFFCN0Request() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
		int& m_iTTFF;
		int& m_iCN0;
	};



	class RM_CLASS_API CGPS_RF_Dev_Multichannel_TrackingRequest : public CDiagRequestBase
	{
	public:
		CGPS_RF_Dev_Multichannel_TrackingRequest(bool& bSuccess, int nCommand);
		virtual ~CGPS_RF_Dev_Multichannel_TrackingRequest() {}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool& m_bSuccess;
	};








} //PLATFORM_TOOL