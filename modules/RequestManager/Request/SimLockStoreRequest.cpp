#include "stdafx.h"
#include "SimLockStoreRequest.h"
#include <assert.h>

namespace PLATFORM_TOOL
{
	CSimLockStoreRequest::CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ& simReq, DWORD &cmdStatus, DWORD &status)
		:CDiagRequestBase(60000), m_SimLockReq(simReq), m_CmdStatus(cmdStatus), m_Status(status)
	{
		m_SimLockReq.command_code = 128;
		m_SimLockReq.subsys_id = 33;
		m_SimLockReq.subsys_cmd_code = 72;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_SimLockReq,sizeof(m_SimLockReq),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStoreRequest::CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ_576& simReq, DWORD &cmdStatus, DWORD &status)
		:CDiagRequestBase(60000), m_SimLockReq_576(simReq), m_CmdStatus(cmdStatus), m_Status(status)
	{
		m_SimLockReq_576.command_code = 128;
		m_SimLockReq_576.subsys_id = 33;
		m_SimLockReq_576.subsys_cmd_code = 72;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_SimLockReq_576,sizeof(m_SimLockReq_576),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStoreRequest::CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ_1024& simReq, DWORD &cmdStatus, DWORD &status)
		:CDiagRequestBase(60000), m_SimLockReq_1024(simReq), m_CmdStatus(cmdStatus), m_Status(status)
	{
		m_SimLockReq_1024.command_code = 128;
		m_SimLockReq_1024.subsys_id = 33;
		m_SimLockReq_1024.subsys_cmd_code = 72;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_SimLockReq_1024, sizeof(m_SimLockReq_1024), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStoreRequest::CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ_2048& simReq, DWORD &cmdStatus, DWORD &status)
		:CDiagRequestBase(60000), m_SimLockReq_2048(simReq), m_CmdStatus(cmdStatus), m_Status(status)
	{
		m_SimLockReq_2048.command_code = 128;
		m_SimLockReq_2048.subsys_id = 33;
		m_SimLockReq_2048.subsys_cmd_code = 72;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_SimLockReq_2048, sizeof(m_SimLockReq_2048), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStoreRequest::~CSimLockStoreRequest(void)
	{
	}

	void CSimLockStoreRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{	
		GSIDDIAG_SIM_SECURE_STORE_RESP simLockResp;
		::memcpy(&simLockResp, ptBufferReceived->GetData(), sizeof(GSIDDIAG_SIM_SECURE_STORE_RESP));

		/*
		**********Protocol defined CmdStatus:**********
		#define GSDIDIAG_SUCCESS		   0
		#define GSDIDIAG_BAD_PARAM         1
		#define GSDIDIAG_ACCESS_DENIED     2
		#define GSDIDIAG_NOT_SUPPORTED     9

		**********Protocol defined status:**********
		typedef enum {
		GSDI_SUCCESS = 0,
		GSDI_ERROR,
		GSDI_ACCESS_DENIED,
		GSDI_NOT_FOUND,
		¡K
		#ifdef FEATURE_QISDA_SIMLOCK
		
		* Qlock event shall not exceed 0xff because ATCOP cannot handle GSDI event
		* that is greater than 0xff.
		 
		GSDI_QLOCK_EXTENSION_START  = 200,
		GSDI_QLOCK_MEMORY_ERROR_HEAP_EXHAUSTED = GSDI_QLOCK_EXTENSION_START = 200,
		GSDI_QLOCK_ERROR_READ_PERSO_ERROR,
		GSDI_QLOCK_ERROR_WRITE_PERSO_ERROR,
		GSDI_QLOCK_ERROR_READ_KEY_ERROR,
		GSDI_QLOCK_ERROR_WRITE_KEY_ERROR,
		GSDI_QLOCK_ERROR_NEED_UNLOCK_FIRST,
		GSDI_QLOCK_ERROR_KEY_LENGTH_ERROR,
		GSDI_QLOCK_ERROR_WRITE_DCK_ERROR,
		GSDI_QLOCK_ERROR_GET_DATA_ERROR,
		GSDI_QLOCK_ERROR_NEXT_AVAIL_TIME_ERROR,
		GSDI_QLOCK_ERROR_RETRY_INFO_ERROR,
		GSDI_QLOCK_ERROR_RANDOM_ALREADY_GENERATED,
		GSDI_QLOCK_ERROR_NO_RANDOM,
		GSDI_QLOCK_EMPTY,
		GSDI_QLOCK_NO_KEY,
		GSDI_QLOCK_PARSER_ERROR_WRONG_LEN,
		GSDI_QLOCK_PARSER_ERROR_INSUFFICIENT_DATA,
		GSDI_QLOCK_PARSER_ERROR_UNKNOWN_TAG,
		GSDI_QLOCK_PARSER_ERROR_UNEXPECTED_TAG,
		GSDI_QLOCK_PARSER_ERROR_MISSING_TLV,
		GSDI_QLOCK_PARSER_ERROR_REDUNDANT_TLV,
		GSDI_QLOCK_PARSER_ERROR_WRONG_VALUE,
		GSDI_QLOCK_PARSER_ERROR_SERIAL_ERROR,
		GSDI_QLOCK_PARSER_ERROR_READ_IMEI_ERROR,
		GSDI_QLOCK_PARSER_ERROR_IMEI_ERROR,
		GSDI_QLOCK_PARSER_ERROR_READ_FSN_ERROR,
		GSDI_QLOCK_PARSER_ERROR_FSN_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_NEW_HANDLE_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_SET_PARAM_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_GET_PADDED_LEN_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_DIGEST_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_KEY_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_AUTH_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_AES_ERROR,
		GSDI_QLOCK_CRYPTO_ERROR_RANDOM_ERROR,
		GSDI_QLOCK_AUTH_FAIL,
		#endif /* FEATURE_QISDA_SIMLOCK 
		*/

		m_CmdStatus = simLockResp.command_status;
		m_Status = simLockResp.status;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimLockStoreRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		if (bRes){
			BYTE *szBuffer = (BYTE*)(ptBufferReceived->GetData());
			if (szBuffer[0] == 0x80 &&
				szBuffer[1] == 0x21 &&
				szBuffer[2] == 0x48 &&
				szBuffer[3] == 0x00)
				return true;
		}
		return false;
	}

	// -----------------------------------------------------------------
	CSecurityActivateRequest::CSecurityActivateRequest(const GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ &secureReq, DWORD &status)
		:m_SecureActivateReq(secureReq), m_Status(status)
	{
		m_SecureActivateReq.command_code = 128;
		m_SecureActivateReq.subsys_id = 33;
		m_SecureActivateReq.subsys_cmd_code = 72;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_SecureActivateReq, sizeof(m_SecureActivateReq), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSecurityActivateRequest::~CSecurityActivateRequest(void)
	{
	}

	void CSecurityActivateRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{	
		GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP secureResp = {0};		
		::memcpy(&secureResp, ptBufferReceived->GetData(), sizeof(GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP));

		m_Status = secureResp.status;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSecurityActivateRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------
	CSimLockStatusRequest::CSimLockStatusRequest(diagpkt_subsidy_lock_resp &lockResp)
		:CDiagRequestBase(60000), m_Resp(lockResp)
	{
		memset(&m_Req, 0, sizeof(m_Req));

		m_Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		m_Req.subsys_id = DIAG_SUBSYS_GSDI;
		m_Req.subsys_cmd_code = 0x42;  // GSDI_GET_FEATURE_INDICATION_CMD

		m_Req.status = 0;
		m_Req.delayed_rsp_id = 0;
		m_Req.rsp_cnt = 0;		
		m_Req.sim_slot_number = 1;     // GSDI_SLOT1

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_Req, sizeof(m_Req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStatusRequest::~CSimLockStatusRequest(void)
	{
	}

	void CSimLockStatusRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(diagpkt_subsidy_lock_resp));

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimLockStatusRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);

		if (bRes){
			diagpkt_subsidy_lock_resp* szBuffer = (diagpkt_subsidy_lock_resp*)(ptBufferReceived->GetData());
			if (szBuffer->cmd_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0x42)
				return true;
		}
		return false;		
	}

	// -----------------------------------------------------------------
	CSimLockStatusRequestEx::CSimLockStatusRequestEx(diagpkt_subsidy_lock_ex_resp &lockResp)
		:CDiagRequestBase(60000), m_Resp(lockResp)
	{
		memset(&m_Req, 0, sizeof(m_Req));

		m_Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		m_Req.subsys_id = DIAG_SUBSYS_GSDI;
		m_Req.subsys_cmd_code = 0x48;
		m_Req.aps_as_transport = 0x00000001;
		m_Req.subcommand = 0x00000042;
		m_Req.slot_id = 0x01;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_Req, sizeof(m_Req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimLockStatusRequestEx::~CSimLockStatusRequestEx(void)
	{
	}

	void CSimLockStatusRequestEx::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(diagpkt_subsidy_lock_ex_resp));

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimLockStatusRequestEx::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		if (CDiagRequestBase::IsCompletePacket(ptBufferReceived)) {
			diagpkt_subsidy_lock_ex_resp* szBuffer = (diagpkt_subsidy_lock_ex_resp*)(ptBufferReceived->GetData());
			if (szBuffer->cmd_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0x48)
				return true;
		}
		return false;		
	}

	// -----------------------------------------------------------------
	CSimUnLockRequest::CSimUnLockRequest(int nLockType, char *sCode, diagpkt_subsidy_unlock_resp &unlockResp)
		:m_Resp(unlockResp)
	{
		memset(&m_Req, 0, sizeof(m_Req));

		m_Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		m_Req.subsys_id = DIAG_SUBSYS_GSDI;
		m_Req.subsys_cmd_code = 0x41; 
		m_Req.status = 0;
		m_Req.delayed_rsp_id = 0;
		m_Req.rsp_cnt = 0;		
		m_Req.sim_slot_number = 1;     // GSDI_SLOT1
		m_Req.lock_type = nLockType;

		strcpy(m_Req.szLock, sCode);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_Req, sizeof(m_Req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimUnLockRequest::~CSimUnLockRequest(void)
	{
	}

	void CSimUnLockRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(diagpkt_subsidy_unlock_resp));
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimUnLockRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);

		if (bRes){
			diagpkt_subsidy_unlock_req* szBuffer = (diagpkt_subsidy_unlock_req*)(ptBufferReceived->GetData());
			if (szBuffer->cmd_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0x41)
				return true;
		}
		return false;		
	}

	// -----------------------------------------------------------------
	CSimUnLockRequestEx::CSimUnLockRequestEx(int nLockType, char *sCode, diagpkt_subsidy_unlock_ex_resp &unlockResp)
		:m_Resp(unlockResp)
	{
		memset(&m_Req, 0, sizeof(m_Req));

		m_Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		m_Req.subsys_id = DIAG_SUBSYS_GSDI;
		m_Req.subsys_cmd_code = 0x48; 
		m_Req.aps_as_transport = 0x00000001;
		m_Req.subcommand = 0x00000041;
		m_Req.slot_id = 0x01;
		m_Req.sim_app_id_len = 0;
		m_Req.perso_feature = nLockType;

		strcpy(m_Req.perso_feature_key, sCode);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_Req, sizeof(m_Req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimUnLockRequestEx::~CSimUnLockRequestEx(void)
	{
	}

	void CSimUnLockRequestEx::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(diagpkt_subsidy_unlock_ex_resp));
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimUnLockRequestEx::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		if (CDiagRequestBase::IsCompletePacket(ptBufferReceived)) {
			diagpkt_subsidy_unlock_ex_req* szBuffer = (diagpkt_subsidy_unlock_ex_req*)(ptBufferReceived->GetData());
			if (szBuffer->cmd_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0x48)
				return true;
		}
		return false;		
	}

	// -----------------------------------------------------------------
	CSimTempUnLockRequest::CSimTempUnLockRequest(int nLockType, char *sCode, diagpkt_subsidy_unlock_resp &unlockResp)
		:m_Resp(unlockResp)
	{
		memset(&m_Req, 0, sizeof(m_Req));

		m_Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		m_Req.subsys_id = DIAG_SUBSYS_GSDI;
		m_Req.subsys_cmd_code = 0xF0; 
		m_Req.status = 0;
		m_Req.delayed_rsp_id = 0;
		m_Req.rsp_cnt = 0;		
		m_Req.sim_slot_number = 1;     // GSDI_SLOT1
		m_Req.lock_type = nLockType;

		strcpy(m_Req.szLock, sCode);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&m_Req, sizeof(m_Req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSimTempUnLockRequest::~CSimTempUnLockRequest(void)
	{
	}

	void CSimTempUnLockRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(diagpkt_subsidy_unlock_resp));
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimTempUnLockRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);

		if (bRes){
			diagpkt_subsidy_unlock_req* szBuffer = (diagpkt_subsidy_unlock_req*)(ptBufferReceived->GetData());
			if (szBuffer->cmd_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0xF0)
				return true;
		}
		return false;		
	}

	// -----------------------------------------------------------------
	CSimUnLockPhoneMasterCodeRequest::CSimUnLockPhoneMasterCodeRequest(char *sCode, gsdidiag_subsidy_unlock_PhoneCode_resp &unlockResp)
		:m_Resp(unlockResp)
	{
		assert (sCode != NULL);
		assert (strlen(sCode) == 8 || strlen(sCode) == 16);

		memset(&m_Req8, 0, sizeof(m_Req8));
		memset(&m_Req16, 0, sizeof(m_Req16));

		m_Req8.cmd_code			= m_Req16.cmd_code			= DIAG_SUBSYS_CMD_VER_2_F;
		m_Req8.subsys_id		= m_Req16.subsys_id			= DIAG_SUBSYS_GSDI;
		m_Req8.subsys_cmd_code	= m_Req16.subsys_cmd_code	= 0x48;
		m_Req8.aps_as_transport = m_Req16.aps_as_transport	= 0;
		m_Req8.subcommand		= m_Req16.subcommand		= 93;
		m_Req8.new_phone_code_len = m_Req16.new_phone_code_len = 4;
		memcpy(&m_Req8.new_phone_code, "1234", 4);
		memcpy(&m_Req16.new_phone_code, "1234", 4);

		CBufferPtr ptBufferSend;

		if (strlen(sCode) == 8){
			m_Req8.master_key_len = 8;
			memcpy(&m_Req8.master_key, sCode, 8);
			Encode((BYTE*)&m_Req8, sizeof(m_Req8), ptBufferSend);
		}
		else if (strlen(sCode) == 16){
			m_Req16.master_key_len = 16;
			memcpy(&m_Req16.master_key, sCode, 16);
			Encode((BYTE*)&m_Req16, sizeof(m_Req16), ptBufferSend);
		}
		else
			assert(0);

		SetSendData(ptBufferSend);
	}

	CSimUnLockPhoneMasterCodeRequest::~CSimUnLockPhoneMasterCodeRequest(void)
	{
	}

	void CSimUnLockPhoneMasterCodeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{			
		::memcpy(&m_Resp, ptBufferReceived->GetData(), sizeof(gsdidiag_subsidy_unlock_PhoneCode_resp));
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSimUnLockPhoneMasterCodeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);

		if (bRes){
			gsdidiag_subsidy_unlock_PhoneCode_resp* szBuffer = (gsdidiag_subsidy_unlock_PhoneCode_resp*)(ptBufferReceived->GetData());
			if (szBuffer->command_code == DIAG_SUBSYS_CMD_VER_2_F &&
				szBuffer->subsys_id == DIAG_SUBSYS_GSDI &&
				szBuffer->subsys_cmd_code == 0x48)
				return true;
		}
		return false;		
	}

	
	CCheckSIMDataRequest::CCheckSIMDataRequest(std::string& strSIMLockName,int& nStatus)
		:CDiagRequestBase(_TC("CCheckSIMDataRequest"))
		,m_strSIMLockName(strSIMLockName)
		,m_nStatus(nStatus)
		,m_bWaitForDelayResponse(false)
	{
		diagpkt_subsys_hdr_type_v2 req =  {0};

		req.command_code     = DIAG_SUBSYS_CMD_VER_2_F ;
		req.subsys_id        = DIAG_SUBSYS_GSDI ;//              = 0x21;
		req.subsys_cmd_code  /*= GSDIDIAG_GET_SIMLOCK_NAME_CMD */ = 0xF0;
		req.status           = 0;
		req.delayed_rsp_id          = 0;
		req.rsp_cnt                 = 0;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req,sizeof(diagpkt_subsys_hdr_type_v2),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCheckSIMDataRequest::~CCheckSIMDataRequest()
	{
	}
	
	bool CCheckSIMDataRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		/*
		000041: Bulk or Interrupt Transfer (UP), 10.02.2009 11:13:30.9771618 +0.0
		Pipe Handle: 0x89e3a8dc (Endpoint Address: 0x1)
		Send 0xf bytes to the device:
		80 21 F0 00 00 00 00 00 00 00 00 00 6F F4 7E
		000042: Bulk or Interrupt Transfer (UP), 10.02.2009 11:13:30.9771618 +0.0
		Pipe Handle: 0x89e3a8bc (Endpoint Address: 0x81)
		Get 0xf bytes from the device:
		80 21 F0 00 00 00 00 00 03 00 00 00 A2 D1 7E
		000043: Bulk or Interrupt Transfer (UP), 10.02.2009 11:13:30.9927867 +0.0156249
		Pipe Handle: 0x89e3a8bc (Endpoint Address: 0x81)
		Get 0x4f bytes from the device:
		80 21 F0 00 00 00 00 00 03 00 01 00 47 4D 45 4C
		42 31 41 5F 41 46 5F 54 65 6C 65 66 6F 6E 69 63
		61 5F 4C 6F 63 30 00 00 00 00 00 00 00 00 00 00 
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
		00 00 00 00 00 00 00 00 00 00 00 00 B2 28 7E
		*/
		
		bool bFinish = false;		
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);

		while(bFinish == false && bRes == true)
		{
			bool bHasFail = false;
			if(bRes)
			{
				if(m_bWaitForDelayResponse == false)
				{
					diagpkt_subsys_hdr_type_v2 resp =  {0};
					memcpy(&resp,ptBufferReceived->GetData(),sizeof(diagpkt_subsys_hdr_type_v2));
					m_nStatus = resp.status;
					m_bWaitForDelayResponse = true;
					bHasFail = m_nStatus != 0;
				}
				else
				{
					m_bWaitForDelayResponse = false;
				}
			}

			if(bHasFail)
				bFinish = true;
			else
				bFinish = m_bWaitForDelayResponse == false && bRes;

			if(bFinish == false)
			{
				ptBufferReceived = new CBuffer;
				bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);
			}
		};

		return bFinish;
	}

	
	void CCheckSIMDataRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		gsdidiag_get_simlock_name_delayed_rsp_type resp =  {0};
		memcpy(&resp,ptBufferReceived->GetData(),sizeof(gsdidiag_get_simlock_name_delayed_rsp_type));
		m_nStatus = resp.header.status;
		if(m_nStatus == 0)
		{
			char szBuffer[65] = {0};
			::memcpy_s(szBuffer,65,resp.name,64);
			m_strSIMLockName.assign(szBuffer);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//

	CQuerySIMLockNameRequest::CQuerySIMLockNameRequest (char *szSIMLockName, bool& nStatus)
		:CDiagRequestBase(_TC("CQuerySIMLockNameRequest"))
		,m_szSIMLockName(szSIMLockName)
		,m_bExecuteResult(nStatus)
	{
		m_bExecuteResult = false;

		BYTE request[] = {128, 33, 241, 0};
		CBufferPtr ptBufferSend;
		Encode(request,sizeof(request),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CQuerySIMLockNameRequest::~CQuerySIMLockNameRequest()
	{
	}

	bool CQuerySIMLockNameRequest::IsCompletePacket (CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CQuerySIMLockNameRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct 
		{
			BYTE cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
			BYTE subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
			WORD subsys_cmd_code;	// 0x48
			DWORD returncode1;		// must = 0
			DWORD reserved;			// 
			DWORD returncode2;		// must = 0
			BYTE  filename[64];			// 0x01
		} diagpkt_lock_name_resp;
		#pragma pack(pop)

		diagpkt_lock_name_resp resp = {0};
		memcpy (&resp, ptBufferReceived->GetData(), sizeof(resp));
		if (0==resp.returncode1 && 0==resp.returncode2)
		{
			m_bExecuteResult = true;
			char szBuffer[65] = {0};
			memcpy (szBuffer, resp.filename, 64);
			strcpy(m_szSIMLockName, szBuffer);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	
	//*******************************************************************//

	CQuerySIMLockNameNewRequest::CQuerySIMLockNameNewRequest (char *szSIMLockName, bool& nStatus)
		:CDiagRequestBase(_TC("CQuerySIMLockNameRequest"))
		,m_szSIMLockName(szSIMLockName)
		,m_bExecuteResult(nStatus)
	{
		m_bExecuteResult = false;

		BYTE request[] = {128, 33, 241, 0};
		CBufferPtr ptBufferSend;
		Encode(request,sizeof(request),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CQuerySIMLockNameNewRequest::~CQuerySIMLockNameNewRequest()
	{
	}

	bool CQuerySIMLockNameNewRequest::IsCompletePacket (CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CQuerySIMLockNameNewRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct 
		{
			BYTE cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
			BYTE subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
			WORD subsys_cmd_code;	// 0x48
			DWORD returncode1;		// must = 0
			DWORD reserved;			// 
			DWORD returncode2;		// must = 0
			DWORD fileLen;
			BYTE  filename[64];			// 0x01
		} diagpkt_lock_name_resp;
		#pragma pack(pop)

		diagpkt_lock_name_resp resp = {0};
		memcpy (&resp, ptBufferReceived->GetData(), sizeof(resp));
		if (0==resp.returncode1 && 0==resp.returncode2)
		{
			m_bExecuteResult = true;
			char szBuffer[65] = {0};
			memcpy (szBuffer, resp.filename, 64);
			strcpy(m_szSIMLockName, szBuffer);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	//*******************************************************************//



	//*******************************************************************//
	CQueryLockStatusRequest::CQueryLockStatusRequest(diagpkt_subsidy_lock_resp& resp, bool& bExecuteResult)
		:m_resp(resp), m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;

		diagpkt_subsidy_lock_req req = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subsys_cmd_code = 0x42;  // GSDI_GET_FEATURE_INDICATION_CMD
		req.status = 0;
		req.delayed_rsp_id = 0;
		req.rsp_cnt = 0;		
		req.sim_slot_number = 1;     // GSDI_SLOT1
		
		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CQueryLockStatusRequest::~CQueryLockStatusRequest(){}
	bool CQueryLockStatusRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{		
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CQueryLockStatusRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		memcpy(&m_resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		if(m_resp.cmd_code ==DIAG_SUBSYS_CMD_VER_2_F && 
		   m_resp.subsys_id == DIAG_SUBSYS_GSDI &&
		   m_resp.subsys_cmd_code == 0x42)
		{
			m_bExecuteResult = m_resp.status == 0;// 0 means query success
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//

	CWriteSimDataRequest::CWriteSimDataRequest(BYTE *pSimDataBuf, int nSimDataBufLen, bool& bExecuteResult)
	:m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;
		GSIDDIAG_SIM_SECURE_STORE_REQ_576 req = {0};
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subcommand = 87;
		req.aps_as_transport =  1;
		req.subsys_cmd_code = 72; //GSDIDIAG_SIM_SECURE_STORE_CMD
		req.slot_id = 0x01;
		req.sim_app_in_len = 16;
		req.simsecure_data_len = nSimDataBufLen;
		memcpy(req.simsecure_data, pSimDataBuf, nSimDataBufLen);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteSimDataRequest::~CWriteSimDataRequest(){}
	bool CWriteSimDataRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CWriteSimDataRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		GSIDDIAG_SIM_SECURE_STORE_RESP resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		if(resp.command_code == DIAG_SUBSYS_CMD_VER_2_F && 
			resp.subsys_id == DIAG_SUBSYS_GSDI &&
			resp.subcommand == 87 &&
			resp.command_status == 0 &&
			resp.status == 0)
		{
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	//*******************************************************************//

	CWriteSimDataNewRequest::CWriteSimDataNewRequest(BYTE *pSimDataBuf, int nSimDataBufLen, bool& bExecuteResult, DWORD biSlotId)
	:m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;
		GSIDDIAG_SIM_SECURE_STORE_REQ_NEW req = {0};
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subsys_cmd_code = 249; //GSDIDIAG_SIM_SECURE_STORE_CMD
		req.slot_id = biSlotId;



		req.simsecure_data_len = nSimDataBufLen;
		memcpy(req.simsecure_data, pSimDataBuf, nSimDataBufLen);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteSimDataNewRequest::~CWriteSimDataNewRequest(){}
	bool CWriteSimDataNewRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CWriteSimDataNewRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		GSIDDIAG_SIM_SECURE_STORE_RESP_NEW resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		if(resp.command_code == DIAG_SUBSYS_CMD_VER_2_F && 
			resp.subsys_id == DIAG_SUBSYS_GSDI &&
			resp.subsys_cmd_code == 249 &&
			resp.command_status == 0 &&
			resp.status == 0)
		{
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CUnlockNWSCPRequest::CUnlockNWSCPRequest(BYTE nLockType, char* szLock8, bool& bUnlockOK, bool& bExecuteResult)
		: m_bExecuteResult(bExecuteResult), m_bUnlockOK(bUnlockOK)
	{
		m_bExecuteResult = false; 
		m_bUnlockOK = false;
		diagpkt_subsidy_unlock_req req = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subsys_cmd_code = 0x41;
		req.status = 0;			
		req.delayed_rsp_id = 0;	
		req.rsp_cnt = 0;		
		req.sim_slot_number = 1;
		req.lock_type = nLockType; 
		strcpy(req.szLock, szLock8);

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CUnlockNWSCPRequest::~CUnlockNWSCPRequest(){}
	bool CUnlockNWSCPRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CUnlockNWSCPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		diagpkt_subsidy_unlock_resp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		if (resp.cmd_code == DIAG_SUBSYS_CMD_VER_2_F && resp.subsys_id == DIAG_SUBSYS_GSDI &&
			resp.subsys_cmd_code == 0x41)
		{
			m_bExecuteResult = true;
			if(resp.status == 0)
				m_bUnlockOK = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CUnlockPhoneCodeRequest::CUnlockPhoneCodeRequest(char* szLock/*MasterCode*/, bool& bUnlockOK, bool& bExecuteResult)
		: m_bExecuteResult(bExecuteResult), m_bUnlockOK(bUnlockOK)
	{
		m_bExecuteResult = false;
		m_bUnlockOK = false;

		CBufferPtr ptBufferSend;
		if(szLock && strlen(szLock) == 8)
		{
			gsdidiag_subsidy_unlock_PhoneCode_req req = {0};
			req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
			req.subsys_id = DIAG_SUBSYS_GSDI;
			req.subsys_cmd_code = 0x48;
			req.aps_as_transport = 0;
			req.subcommand = 93;
			req.master_key_len = strlen(szLock);
			memcpy(req.master_key, szLock, strlen(szLock));
			req.new_phone_code_len = 4;
			req.new_phone_code[0] = '1';
			req.new_phone_code[1] = '2';
			req.new_phone_code[2] = '3';
			req.new_phone_code[3] = '4';
			Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		}
		else if(szLock && strlen(szLock) == 16)
		{
			gsdidiag_subsidy_unlock_PhoneCode16_req req = {0};
			req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
			req.subsys_id = DIAG_SUBSYS_GSDI;
			req.subsys_cmd_code = 0x48;
			req.aps_as_transport = 0;
			req.subcommand = 93;
			req.master_key_len = strlen(szLock);
			memcpy(req.master_key, szLock, strlen(szLock));
			req.new_phone_code_len = 4;
			req.new_phone_code[0] = '1';
			req.new_phone_code[1] = '2';
			req.new_phone_code[2] = '3';
			req.new_phone_code[3] = '4';
			Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		}
		SetSendData(ptBufferSend);
	}
	CUnlockPhoneCodeRequest::~CUnlockPhoneCodeRequest(){}
	bool CUnlockPhoneCodeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CUnlockPhoneCodeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		gsdidiag_subsidy_unlock_PhoneCode_resp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		if (resp.command_code == DIAG_SUBSYS_CMD_VER_2_F &&
			resp.subsys_id == DIAG_SUBSYS_GSDI &&
			resp.subsys_cmd_code == 0x48)
		{
			m_bExecuteResult = true;
			if(resp.status == 0 && resp.unknown == 0)
				m_bUnlockOK = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

}	