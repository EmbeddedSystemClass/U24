#include "stdafx.h"
#include "eMMCRequest.h"

namespace PLATFORM_TOOL
{
	CGeteMMCSizeRequest::CGeteMMCSizeRequest (DWORD &dwSize, bool &bSuccess)
		: CDiagRequestBase (_T("CGeteMMCSizeRequest"))
		, m_Size (dwSize)
		, m_Success (bSuccess)
	{

		bSuccess = false;
		m_Size = 0;
		BYTE szRAWCommand[] = {75, 250, 104, 0, 3, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGeteMMCSizeRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//250
			short mobile_id;//102
			DWORD size; // (unit in Giga)
			WORD  Result; //1=success, 0=fail

		} diag_eMMCsize_response;
		#pragma pack(pop)

		diag_eMMCsize_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.Result == 1;
		if (m_Success)
		{
			m_Size = resp.size;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
}