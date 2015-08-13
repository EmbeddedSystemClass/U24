#include "stdafx.h"
#include "A9_eMMCRequest.h"


namespace PLATFORM_TOOL
{


CA9GeteMMCSizeRequest::CA9GeteMMCSizeRequest(char* szeMMCSize, bool& bSuccess)
	: CDiagRequestBase(_T("CA9GeteMMCSizeRequest"))
	, m_szeMMCSize(szeMMCSize)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szeMMCSize, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9GeteMMCSizeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//Success or fail
		BYTE response_data[12];	// Response data
	} A9_eMMCSize_Response;
#pragma pack (pop)

	A9_eMMCSize_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type != 0x04)
	{
		m_bSuccess = true;
		memcpy(m_szeMMCSize, response.response_data, 12);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


} // End of namespace PLATFORM_TOOL
