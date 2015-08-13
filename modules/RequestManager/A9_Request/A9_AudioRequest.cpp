#include "stdafx.h"
#include "A9_AudioRequest.h"


namespace PLATFORM_TOOL
{


CA9AudioDetectEarphoneRequest::CA9AudioDetectEarphoneRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9AudioDetectEarphoneRequest"))
	, m_bSuccess(bSuccess)
{

	m_bSuccess = false;
	BYTE szCommand[] = {0xD2, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9AudioDetectEarphoneRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
		BYTE result;
	} A9_CameraFlashlight_Response;
#pragma pack (pop)

	A9_CameraFlashlight_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.result != 0x00)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


} // End of namespace PLATFORM_TOOL
