#include "stdafx.h"
#include "ODMRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{


	CDiagRAWRequest::CDiagRAWRequest(BYTE* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn)
		:CDiagRequestBase(5000, bHasReturn)
	{
		if (bAddCRC)
		{
			CBufferPtr ptBufferSend = new CBuffer;
			Encode(szCmdInput, nCmdLength,ptBufferSend);
			SetSendData(ptBufferSend);
		}
		else
		{
			CBufferPtr ptBufferSend = new CBuffer(nCmdLength, szCmdInput);
			SetSendData(ptBufferSend);
		}
	}

	CDiagRAWRequest::~CDiagRAWRequest(){}




	#pragma pack(push, 1)

	//CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
	//SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 250 for Qisda EM Diag
	//Command code	2	90 WM Power State Set
	//Power State	2	0: Power OFF
	//1: Reset
	//Result	2	Reserved for the setting device status

	typedef struct
	{
		BYTE command_code; 
		BYTE subsys_id;
		short Mode_ID;
		short State;
		short Result;
	} diag_MS_power_req;
	#pragma pack(pop)

	CMSPowerRequest::CMSPowerRequest(int nPowerState)
		: CDiagRequestBase (_T("CMSPowerRequest"), 30000, false)
	{
		CBufferPtr ptBufferSend;
		diag_MS_power_req req = {75, 250, 90,nPowerState,2};
		Encode ((const BYTE*)&req, sizeof(diag_MS_power_req), ptBufferSend);
		SetSendData (ptBufferSend);
	}
	
	//DFI Temp location
	CDiagUpdateDFIFW::CDiagUpdateDFIFW(bool& bSuccess, BYTE& iErrCode)
		: CDiagRequestBase()
		, m_bSuccess(bSuccess)
		, m_iErrCode(iErrCode)
	{
		m_bSuccess = false;

		BYTE szRAWCommand[] = {75, 36, 19, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagUpdateDFIFW::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE pResponse[6];
		memset(pResponse, 0, 6);

		Safememcopy(pResponse, sizeof(pResponse), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (pResponse[4] == 1)
			m_bSuccess = true;
		else
			m_bSuccess = false;

		m_iErrCode = pResponse[5];

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



	// Wolf QFuse --> should be in FTM mode.

	CWolfReadQFuse::CWolfReadQFuse(bool& bSuccess, BYTE& iErrCode, BYTE* pRegister, long regsize)
		: CDiagRequestBase()
		, m_bSuccess(bSuccess)
		, m_ConfigRegister (pRegister)
		, m_RegisterSize (regsize)
	{
		m_bSuccess = false;

		//#pragma pack(push, 1)
		//typedef struct
		//{
		//	BYTE command_code;		// set to 75 for this message
		//	BYTE subsys_id;			// Set to 11 to select FTM subsystem
		//	short CmdCode;			// set to 24 to select FTM_QFUSE_C
		//	short FTM_CDM_ID;		// set to 0 to select FTM_QFUSE_READ
		//	short FTM_REQ_DATALEN;	// set to 2;
		//	short FTM_RSP_PKT_SIZE;	// set to 32; indicates total length of response packet.
		//	short chain_select;		// 0 -- configuration chain
		//} diag_wolf_readQFuse_req;
		//#pragma pack(pop)

		//Example packet: 0x4B 0x0B 0x18 0x00 0x00 0x00 0x02 0x00 0x20 0x00 0x00 0x00
		BYTE szRAWCommand[] = {75, 11, 24, 0, 0, 0, 2, 0, 32, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode (szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolfReadQFuse::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;		// set to 75 for this message
			BYTE subsys_id;			// Set to 11 to select FTM subsystem
			short CmdCode;			// set to 24 to select FTM_QFUSE_C
			short FTM_CDM_ID;		// set to 0 to select FTM_QFUSE_READ
			short FTM_REQ_DATALEN;	// set to 2;
			short FTM_RSP_PKT_SIZE;	// set to 32; indicates total length of response packet.
			short read_status;		// 0 : success, 1: read fail, 2: write fail, 3: operation on selected chain fail
			//BYTE  config_register[24];  // array of six registers (0-5), size of each register is 4 bytes.
		} diag_wolf_readQFuse_rsp;
		#pragma pack(pop)


		long bufsize = sizeof(diag_wolf_readQFuse_rsp) + m_RegisterSize;
		BYTE *pRespBuffer = new BYTE [bufsize];
		memset (pRespBuffer, 0, bufsize);

		diag_wolf_readQFuse_rsp *pResp = (diag_wolf_readQFuse_rsp*) pRespBuffer;

		Safememcopy (pRespBuffer, bufsize, (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bSuccess = pResp->read_status == 0;

		memcpy (m_ConfigRegister, pRespBuffer + sizeof(diag_wolf_readQFuse_rsp), m_RegisterSize);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
		delete [] pRespBuffer;
	}


	CWolfWriteQFuse::CWolfWriteQFuse(bool& bSuccess, BYTE* pRegister, long bufsize)
		: CDiagRequestBase()
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;			// set to 75 for this message
			BYTE subsys_id;				// Set to 11 to select FTM subsystem
			short CmdCode;				// set to 24 to select FTM_QFUSE_C
			short FTM_CDM_ID;			// set to 1 to select FTM_QFUSE_WRITE
			short FTM_REQ_DATALEN;		// set to 2;
			short FTM_RSP_PKT_SIZE;		// set to 12; indicates total length of response packet.
			short chain_select;			// 0 -- configuration chain
			//BYTE  config_register[24];	// array of six registers (0-5), size of each register is 4 bytes.
		} diag_wolf_readQFuse_req;
		#pragma pack(pop)

		long request_size = sizeof(diag_wolf_readQFuse_req) + bufsize;
		BYTE* pReqBuffer = new BYTE [request_size];

		diag_wolf_readQFuse_req *pReq = (diag_wolf_readQFuse_req*) pReqBuffer;
		pReq->command_code = 75;
		pReq->subsys_id = 11;
		pReq->CmdCode = 24;
		pReq->FTM_CDM_ID = 1;
		pReq->FTM_REQ_DATALEN = 2;
		pReq->FTM_RSP_PKT_SIZE = 12;
		pReq->chain_select = 0;

		BYTE* pRegPtr = pReqBuffer + sizeof(diag_wolf_readQFuse_req);
		memcpy (pRegPtr, pRegister, bufsize);


		CBufferPtr ptBufferSend;
		Encode (pReqBuffer, request_size, ptBufferSend);
		SetSendData(ptBufferSend);

		delete [] pReqBuffer;
	}

	void CWolfWriteQFuse::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;		// set to 75 for this message
			BYTE subsys_id;			// Set to 11 to select FTM subsystem
			short CmdCode;			// set to 24 to select FTM_QFUSE_C
			short FTM_CDM_ID;		// set to 1 to select FTM_QFUSE_WRITE
			short FTM_REQ_DATALEN;	// set to 26;
			short FTM_RSP_PKT_SIZE;	// set to 12; indicates total length of response packet.
			short write_status;		// 0 : success, 1: read fail, 2: write fail, 3: operation on selected chain fail
		} diag_wolf_writeQFuse_rsp;
		#pragma pack(pop)



		diag_wolf_writeQFuse_rsp WriteQFuseRsp;
		memset(&WriteQFuseRsp, 0, sizeof(diag_wolf_writeQFuse_rsp));

		Safememcopy (&WriteQFuseRsp, sizeof(diag_wolf_writeQFuse_rsp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (WriteQFuseRsp.write_status == 0)
			m_bSuccess = true;
		else
			m_bSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}





	CAustinFreezeOTPRequest::CAustinFreezeOTPRequest(int nRequestType, bool& bSuccess, int& nStatus)
		:CDiagRequestBase(_TC("CAustinFreezeOTPRequest"))
		,m_bSuccess(bSuccess)
		,m_nStatus(nStatus)
	{
		m_bSuccess = false;
		m_nStatus = 0;
		int nCommandSize = 4;

		BYTE szRAWCommand[6] ={0};
		if(nRequestType == 1) //Freeze OTP
		{	
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 12;
			szRAWCommand[3] = 0;
		}
		else if(nRequestType == 2) //Erase OTP
		{
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 5;
			szRAWCommand[3] = 0;

		}
		else if(nRequestType == 3) //Erase EFS
		{
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 2;
			szRAWCommand[3] = 0;
			szRAWCommand[4] = 2;
			szRAWCommand[5] = 0;
			nCommandSize = 6;

		}
		else
			assert(0);
		//		BYTE szRAWCommand[] = {	75, 0, 12,0};
		//		BYTE szRAWCommand[] = {	75, 0, 5,0};
		//				BYTE szRAWCommand[] = {	75,00,02,00,2,00};

		CBufferPtr ptBufferSend;
		//Encode((const char*)szRAWCommand,nCommandSize,ptBufferSend);
		Encode(szRAWCommand,nCommandSize,ptBufferSend);

		SetSendData(ptBufferSend);
	}

	CAustinFreezeOTPRequest::~CAustinFreezeOTPRequest()
	{
	}

	void CAustinFreezeOTPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		austin_otp_freeze_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(resp.status == 0 || resp.status == -3 || resp.status == -4)
			m_bSuccess = true;
		else
			m_bSuccess = false;
			m_nStatus = resp.status;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CC7FreezeOTPRequest::CC7FreezeOTPRequest(int nRequestType, bool& bSuccess, int& nStatus)
		:CDiagRequestBase(_TC("CC7FreezeOTPRequest"))
		,m_bSuccess(bSuccess)
		,m_nStatus(nStatus)
	{
		m_bSuccess = false;
		m_nStatus = 0;
		int nCommandSize = 4;

		BYTE szRAWCommand[6] ={0};
		if(nRequestType == 1) //Freeze OTP
		{	
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 12;
			szRAWCommand[3] = 0;
		}
		else if(nRequestType == 2) //Erase OTP
		{
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 5;
			szRAWCommand[3] = 0;

		}
		else if(nRequestType == 3) //Erase EFS
		{
			szRAWCommand[0] = 75;
			szRAWCommand[1] = 0;
			szRAWCommand[2] = 2;
			szRAWCommand[3] = 0;
			szRAWCommand[4] = 2;
			szRAWCommand[5] = 0;
			nCommandSize = 6;

		}
		else
			assert(0);
		//		BYTE szRAWCommand[] = {	75, 0, 12,0};
		//		BYTE szRAWCommand[] = {	75, 0, 5,0};
		//				BYTE szRAWCommand[] = {	75,00,02,00,2,00};

		CBufferPtr ptBufferSend;
		//Encode((const char*)szRAWCommand,nCommandSize,ptBufferSend);
		Encode(szRAWCommand,nCommandSize,ptBufferSend);

		SetSendData(ptBufferSend);
	}

	CC7FreezeOTPRequest::~CC7FreezeOTPRequest()
	{
	}

	void CC7FreezeOTPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		C7_otp_freeze_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(resp.status == 0 || resp.status == -3 || resp.status == -4)
			m_bSuccess = true;
		else
			m_bSuccess = false;
			m_nStatus = resp.status;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
} // End of namespace PLATFORM_TOOL





//CWolfReadQFuse