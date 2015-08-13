#include "stdafx.h"
#include "MHLRequest.h"


namespace PLATFORM_TOOL
{


CReadMHLIDRequest::CReadMHLIDRequest(char* szMHLID, bool& bSuccess)
	: CDiagRequestBase(_T("CReadMHLIDRequest"))
	, m_szMHLID(szMHLID)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szMHLID, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x6F, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CReadMHLIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE response_data[12];	// Response data
	} A9_MHLID_Response;
#pragma pack (pop)

	A9_MHLID_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2)
	{
		m_bSuccess = true;
		memcpy(m_szMHLID, response.response_data, 12);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


} // End of namespace PLATFORM_TOOL
