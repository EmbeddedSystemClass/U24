#include "stdafx.h"
#include "A9_TouchRequest.h"


namespace PLATFORM_TOOL
{

//---------Start CA9TouchCalibrationRequest
CA9TouchCalibrationRequest::CA9TouchCalibrationRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9TouchCalibrationRequest"), 60000)
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchCalibrationRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchCalibration_Response;
#pragma pack (pop)

	A9_TouchCalibration_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchCalibrationRequest


//---------Start CA9TouchIDACRequest
CA9TouchIDACRequest::CA9TouchIDACRequest(bool& bSuccess, BYTE Min, BYTE Max)
	: CDiagRequestBase(_T("CA9TouchIDACRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x7D, 0x00, 0x00, Max, Min, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchIDACRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchIDAC_Response;
#pragma pack (pop)

	A9_TouchIDAC_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchIDACRequest

//---------Start CA9TouchBaselineRequest
CA9TouchBaselineRequest::CA9TouchBaselineRequest(bool& bSuccess, BYTE Min, BYTE Max)
	: CDiagRequestBase(_T("CA9TouchBaselineRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x7E, 0x00, 0x00, Max, Min, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchBaselineRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchBaseline_Response;
#pragma pack (pop)

	A9_TouchBaseline_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchBaselineRequest


//---------Start CA9TouchDeltaRequest
CA9TouchDeltaRequest::CA9TouchDeltaRequest(bool& bSuccess, BYTE Min, BYTE Max)
	: CDiagRequestBase(_T("CA9TouchDeltaRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x7F, 0x00, 0x00, Max, Min, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchDeltaRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchDelta_Response;
#pragma pack (pop)

	A9_TouchDelta_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchDeltaRequest


//---------Start CA9TouchDisableRequest
CA9TouchDisableRequest::CA9TouchDisableRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9TouchDisableRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchDisableRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchDisable_Response;
#pragma pack (pop)

	A9_TouchDisable_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchDisableRequest


//---------Start CA9TouchEnableRequest
CA9TouchEnableRequest::CA9TouchEnableRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9TouchEnableRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchEnableRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchEnable_Response;
#pragma pack (pop)

	A9_TouchEnable_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchEnableRequest

//---------Start CA9TouchFWUpdateRequest
CA9TouchFWUpdateRequest::CA9TouchFWUpdateRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9TouchFWUpdateRequest"), 60000)
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9TouchFWUpdateRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE status_type;		//Success or fail
	// Response data
	} A9_TouchFWUpdate_Response;
#pragma pack (pop)

	A9_TouchFWUpdate_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type == 0x01)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

//----------End of CA9TouchFWUpdateRequest

//---------Start CA9ReadTouchVersionRequest
CA9ReadTouchVersionRequest::CA9ReadTouchVersionRequest(char* szVersion, bool& bSuccess)
	: CDiagRequestBase(_T("CA9ReadTouchVersionRequest"))
	, m_szVersion(szVersion)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szVersion, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9ReadTouchVersionRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[3];		// Command code
		BYTE version;		//Success or fail
	} A9_TouchVersion_Response;
#pragma pack (pop)

	A9_TouchVersion_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2)
	{
		m_bSuccess = true;
		
		sprintf (m_szVersion, "%u", response.version);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9ReadTouchVersionRequest


} // End of namespace PLATFORM_TOOL
