#include "stdafx.h"
#include "SDRequest.h"

namespace PLATFORM_TOOL
{



	/*  JY , get sd status
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID VQisda EM = 250
		Mode ID		2	Mode ID V set to 102  to get SD size
		function	4	01 00 00 00
		Result		2	Success: 1, Fail: 0
		*/
	CLeopardGetSDcardSizeRequest::CLeopardGetSDcardSizeRequest (DWORD &dwSize, bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGetSDcardSizeRequest"))
		, m_Size (dwSize)
		, m_Success (bSuccess)
	{

		bSuccess = false;
		m_Size = 0;
		BYTE szRAWCommand[] = {75, 250, 102, 0, 1, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardGetSDcardSizeRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//250
			short mobile_id;//102
			DWORD size; // (unit in Maga)
			WORD  Result; //1=success, 0=fail

		} diag_leopard_SDsize_response;
		#pragma pack(pop)

		diag_leopard_SDsize_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.Result == 1;
		if (m_Success)
		{
			m_Size = resp.size;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/*  JY , get sd status
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID VQisda EM = 250
		Mode ID		2	Mode ID V set to 103  to get SD status
		function	4	02 00 00 00
		Result		2	Success: 1, Fail: 0
		*/
	CLeopardGetSDcardStatusRequest::CLeopardGetSDcardStatusRequest (DWORD &dwSize, bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGetSDcardStatusRequest"))
		, m_Status (dwSize)
		, m_Success (bSuccess)
	{
		bSuccess = false;
		m_Status = 0;
		BYTE szRAWCommand[] = {75, 250, 103, 0, 2, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}


	void CLeopardGetSDcardStatusRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//250
			short mobile_id;//103
			DWORD status; // 
			WORD  Result; //1=success, 0=fail

		} diag_leopard_SDstatus_response;
		#pragma pack(pop)

		diag_leopard_SDstatus_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		
		m_Success= 1==resp.Result;
		m_Status= resp.status;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



/*  JY , get sd status
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID VQisda EM = 250
		Mode ID		2	Mode ID ¡V qisda_em_system_sd_size = 103
		function	4	01 00 00 00
		Result		2	Success: 1, Fail: 0
		*/
	CLeopardGetSDcardSizenStatusRequest_Sys::CLeopardGetSDcardSizenStatusRequest_Sys (DWORD &dwSize, DWORD &dwStatus, bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGetSDcardSizenStatusRequest_Sys"))
		, m_Size (dwSize)
		, m_Status (dwStatus)
		, m_Success (bSuccess)
	{

		bSuccess = false;
		m_Size = 0;
		BYTE szRAWCommand[] = {75, 250, 102, 0, 2, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardGetSDcardSizenStatusRequest_Sys::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//250
			short mobile_id;//102
			DWORD size; // (unit in Maga)
			WORD  Result; //1=success, 0=fail

		} diag_leopard_SDsize_response;
		#pragma pack(pop)

		diag_leopard_SDsize_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.Result == 1;
		if (m_Success)
		{
			m_Size = resp.size;
			m_Status = 1;
		}
		else
		{
			m_Size = 0;
			m_Status = 0;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

} //PLATFORM_TOOL