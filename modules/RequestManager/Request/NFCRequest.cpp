#include "stdafx.h"
#include "NFCRequest.h"

namespace PLATFORM_TOOL
{
	/*  JY , get sd status
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID VQisda EM = 250
		Mode ID		2	Mode ID V set to 112  to check NFC connect
		*/
	CLeopardNFCIsConnectedRequest::CLeopardNFCIsConnectedRequest (bool& bResult)
		: CDiagRequestBase (_T("CLeopardNFCIsConnectedRequest"))
		, m_Success (bResult)
	{

		m_Success = false;

		BYTE szRAWCommand[] = {75, 250, 112, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardNFCIsConnectedRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code;		//75
			BYTE  subsys_id;	//250
			short mobile_id;	//112
			BYTE  Result;		//3=success, 0=fail
			BYTE  para1;		//dont know what it is

		} diag_leopard_IsConnected_response;
#pragma pack(pop)

		diag_leopard_IsConnected_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.Result == 3;
		
		/*if (m_Success)
		{
			m_Size = resp.size;
		}*/

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

} //PLATFORM_TOOL