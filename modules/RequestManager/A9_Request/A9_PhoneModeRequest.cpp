#include "stdafx.h"
#include "A9_PhoneModeRequest.h"


namespace PLATFORM_TOOL
{

//---------CA9PhoneModeOfflineModeRequest
	CA9PhoneModeOfflineModeRequest::CA9PhoneModeOfflineModeRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeOfflineModeRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x29, 0x01, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeOfflineModeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_offline_mode_cmd[2];
		} A9_PhoneModeOfflineMode_Response;
	#pragma pack (pop)

	A9_PhoneModeOfflineMode_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x29)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeOfflineModeRequest-------------//


//---------CA9PhoneModeFTMModeRequest
	CA9PhoneModeFTMModeRequest::CA9PhoneModeFTMModeRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeFTMModeRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x29, 0x03, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeFTMModeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_offline_mode_cmd[2];
		} A9_PhoneModeFTMMode_Response;
	#pragma pack (pop)

	A9_PhoneModeFTMMode_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x29)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeFTMModeRequest-------------//

//---------CA9PhoneModeOnlineModeRequest
	CA9PhoneModeOnlineModeRequest::CA9PhoneModeOnlineModeRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeOnlineModeRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x29, 0x04, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeOnlineModeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_offline_mode_cmd[2];
		} A9_PhoneModeOnlineMode_Response;
	#pragma pack (pop)

	A9_PhoneModeOnlineMode_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x29)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeOnlineModeRequest-------------//

//---------CA9PhoneModeLPMModeRequest
	CA9PhoneModeLPMModeRequest::CA9PhoneModeLPMModeRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeLPMModeRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x29, 0x05, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeLPMModeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_offline_mode_cmd[2];
		} A9_PhoneModeLPMMode_Response;
	#pragma pack (pop)

	A9_PhoneModeLPMMode_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x29)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeLPMModeRequest-------------//

//---------CA9PhoneModeOfflineModeResetRequest
	CA9PhoneModeOfflineModeResetRequest::CA9PhoneModeOfflineModeResetRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeOfflineModeResetRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x29, 0x02, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeOfflineModeResetRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_offline_mode_cmd[2];
		} A9_PhoneModeOfflineModeReset_Response;
	#pragma pack (pop)

	A9_PhoneModeOfflineModeReset_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x29)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeOfflineModeResetRequest-------------//

//---------CA9PhoneModeReadModeTypeRequest
	CA9PhoneModeReadModeTypeRequest::CA9PhoneModeReadModeTypeRequest(char* szModeType, bool& bSuccess)
		: CDiagRequestBase(_T("CA9PhoneModeReadModeTypeRequest"), 10000)
		, m_szModeType(szModeType)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szModeType, "");
		m_bSuccess = false;

		BYTE szCommand[] = {75, 15, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneModeReadModeTypeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE cmd_code;			// Diagnostic type or error code
			BYTE subsys_id;
			BYTE subsys_cmd_code[2];
			BYTE overall_call_state[4];
			BYTE operational_mode[4];

		} A9_PhoneModeReadModeType_Response;
	#pragma pack (pop)

	A9_PhoneModeReadModeType_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.cmd_code == 0x4B)
	{
		
		//BYTE *type = response.operational_mode;
		sprintf(m_szModeType, "%u", *response.operational_mode);
		//memcpy(m_szModeType, response.operational_mode, 4);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9PhoneModeReadModeTypeRequest-------------//

} // End of namespace PLATFORM_TOOL
