#include "stdafx.h"
#include "A9_PowerRequest.h"


namespace PLATFORM_TOOL
{

//-------Class CA9ReadBatteryIDRequest
	CA9ReadBatteryIDRequest::CA9ReadBatteryIDRequest(char* szPowerID, bool& bSuccess)
		: CDiagRequestBase(_T("CA9ReadBatteryIDRequest"))
		, m_szPowerID(szPowerID)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szPowerID, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ReadBatteryIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
			DWORD response_id;	// Response data
		} A9_PowerID_Response;
	#pragma pack (pop)

		A9_PowerID_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;

			sprintf (m_szPowerID, "%u", response.response_id);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ReadBatteryIDRequest---------//

//-------Class CA9ChargerEnableRequest
	CA9ChargerEnableRequest::CA9ChargerEnableRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9ChargerEnableRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ChargerEnableRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_ChargerEnable_Response;
	#pragma pack (pop)

		A9_ChargerEnable_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ChargerEnableRequest--------//

//-------Class CA9ChargerDisableRequest
	CA9ChargerDisableRequest::CA9ChargerDisableRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9ChargerDisableRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ChargerDisableRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_ChargerDisable_Response;
	#pragma pack (pop)

		A9_ChargerDisable_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ChargerDisableRequest--------//

//--------CA9ReadBatteryVoltageRequest
CA9ReadBatteryVoltageRequest::CA9ReadBatteryVoltageRequest(char* szBatteryVoltage, bool& bSuccess)
	: CDiagRequestBase(_T("CA9ReadBatteryVoltageRequest"))
	, m_szBatteryVoltage(szBatteryVoltage)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szBatteryVoltage, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9ReadBatteryVoltageRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//Success or fail
		BYTE response_data[12];	// Response data
	} A9_BatteryVoltage_Response;
#pragma pack (pop)

	A9_BatteryVoltage_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type != 0x04)
	{
		m_bSuccess = true;
		memcpy(m_szBatteryVoltage, response.response_data, 12);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//--------End of CA9ReadBatteryVoltageRequest--------//



//--------CA9SendPowerKeyEventRequest
CA9SendPowerKeyEventRequest::CA9SendPowerKeyEventRequest(bool& bSuccess, BYTE szDelayTime)
	: CDiagRequestBase(_T("CA9SendPowerKeyEventRequest"))
	, m_bSuccess(bSuccess)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0xD2, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	szCommand[4] = szDelayTime; //send key event after how many seconds
	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SendPowerKeyEventRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//Success or fail
		BYTE response_data[12];	// Response data
	} A9_PowerKeyEvent_Response;
#pragma pack (pop)

	A9_PowerKeyEvent_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD2 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//--------End of CA9SendPowerKeyEventRequest--------//


//-------Class CA9WakeLockRequest
	CA9WakeLockRequest::CA9WakeLockRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9WakeLockRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9WakeLockRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_WakeLock_Response;
	#pragma pack (pop)

		A9_WakeLock_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9WakeLockRequest--------//


//-------Class CA9WakeUnlockRequest
	CA9WakeUnlockRequest::CA9WakeUnlockRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9WakeUnlockRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9WakeUnlockRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_WakeUnlock_Response;
	#pragma pack (pop)

		A9_WakeUnlock_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9WakeUnlockRequest--------//



} // End of namespace PLATFORM_TOOL

