#include "stdafx.h"
#include "A9_CameraRequest.h"


namespace PLATFORM_TOOL
{


CA9CameraFlashlightRequest::CA9CameraFlashlightRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9CameraFlashlightRequest"))
	, m_bSuccess(bSuccess)
{

	m_bSuccess = false;
	BYTE szCommand[] = {0xD2, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9CameraFlashlightRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
	} A9_CameraFlashlight_Response;
#pragma pack (pop)

	A9_CameraFlashlight_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

CA9CameraFlashlightOnOffRequest::CA9CameraFlashlightOnOffRequest(bool bEnable, bool& bSuccess)
: CDiagRequestBase(_T("CA9CameraFlashlightOnOffRequest"))
, m_bSuccess(bSuccess)
{

	m_bSuccess = false;

	BYTE Enable;
	bEnable ? Enable = 0x8D : Enable = 0x8E;
	BYTE szCommand[] = {0xD2, Enable, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9CameraFlashlightOnOffRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
	} A9_CameraFlashlight_Response;
#pragma pack (pop)

	A9_CameraFlashlight_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


} // End of namespace PLATFORM_TOOL
