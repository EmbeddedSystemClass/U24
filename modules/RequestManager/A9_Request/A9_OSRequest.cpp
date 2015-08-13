#include "stdafx.h"
#include "A9_OSRequest.h"


namespace PLATFORM_TOOL
{

//--------CA9OSResetToFastbootRequest
	CA9OSResetToFastbootRequest::CA9OSResetToFastbootRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9OSResetToFastbootRequest"))
		, m_bSuccess(bSuccess)
	{

		BYTE szCommand[] = {0xD2, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9OSResetToFastbootRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_OSToFastboot_Response;
	#pragma pack (pop)

		A9_OSToFastboot_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9OSResetToFastbootRequest----------//

//---------CA9OSRebootRequest
	CA9OSRebootRequest::CA9OSRebootRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9OSRebootRequest"))
		, m_bSuccess(bSuccess)
	{

		BYTE szCommand[] = {0xD2, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9OSRebootRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
		} A9_OSReboot_Response;
	#pragma pack (pop)

		A9_OSReboot_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//---------END of CA9OSRebootRequest----------//


//-------Class CA9ReadCodecIDRequest
	CA9ReadCodecIDRequest::CA9ReadCodecIDRequest(char* szCodecID, bool& bSuccess)
		: CDiagRequestBase(_T("CA9ReadCodecIDRequest"))
		, m_szCodecID(szCodecID)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szCodecID, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ReadCodecIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
			BYTE response_id;	// Response data
		} A9_CodecID_Response;
	#pragma pack (pop)

		A9_CodecID_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2/* && response.status_type != 0x04*/)
		{
			m_bSuccess = true;
			sprintf (m_szCodecID, "%u", response.response_id);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ReadCodecIDRequest---------//


//-------Class CA9ReadModemVersionRequest
	CA9ReadModemVersionRequest::CA9ReadModemVersionRequest(char* szModemVersion, bool& bSuccess)
		: CDiagRequestBase(_T("CA9ReadModemVersionRequest"))
		, m_szModemVersion(szModemVersion)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szModemVersion, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD1, 0x0B, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ReadModemVersionRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
			BYTE response_version[80];	// Response data
		} A9_ModemVersion_Response;
	#pragma pack (pop)

		A9_ModemVersion_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD1 && response.status_type != 0x04)
		{
			m_bSuccess = true;
			memcpy(m_szModemVersion, response.response_version, 80);
			//sprintf (m_szCodecID, "%d", response.response_id);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ReadModemVersionRequest---------//


	//-------Class CA9ReadHWVersionRequest
	CA9ReadHWVersionRequest::CA9ReadHWVersionRequest(char* szHWVersion, bool& bSuccess)
		: CDiagRequestBase(_T("CA9ReadHWVersionRequest"))
		, m_szHWVersion(szHWVersion)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szHWVersion, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD1, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9ReadHWVersionRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;	//Success or fail
			BYTE rf_version;	// Response data
			BYTE hw_version;
			BYTE memory_version;
			BYTE emmc_version;
		} A9_HWVersion_Response;
#pragma pack (pop)

		A9_HWVersion_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD1 && response.status_type != 0x04)
		{
			m_bSuccess = true;
			sprintf(m_szHWVersion, "%d%d%d%d", response.rf_version, response.hw_version, response.memory_version, response.emmc_version);
			//sprintf (m_szCodecID, "%d", response.response_id);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//--------End of CA9ReadSWVersionRequest---------//

//---------CA9OSWriteMassProductionRequest
	CA9OSWriteMassProductionRequest::CA9OSWriteMassProductionRequest(bool& bSuccess, char* szIsMP)
		: CDiagRequestBase(_T("CA9OSWriteMassProductionRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE bIsMP = atoi(szIsMP);

		BYTE szCommand[133] = {0x27, 0x89, 0x03};
		szCommand[3] = bIsMP;

		for(int i=4; i<133; i++)
		{
			szCommand[i]=0x00;
		}

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9OSWriteMassProductionRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE dump[128];
			BYTE status_type[2];	//Success or fail
		} A9_OSReboot_Response;
	#pragma pack (pop)

		A9_OSReboot_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0x27 && response.status_type[0] == 0x00 && response.status_type[1] == 0x00)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//---------END of CA9OSWriteMassProductionRequest----------//

//---------CA9OSReadMassProductionRequest
	CA9OSReadMassProductionRequest::CA9OSReadMassProductionRequest(bool& bSuccess, char* szIsMP)
		: CDiagRequestBase(_T("CA9OSReadMassProductionRequest"))
		, m_bSuccess(bSuccess)
		, m_szIsMP(szIsMP)
	{
		BYTE bIsMP = atoi(szIsMP);

		BYTE szCommand[133] = {0x26, 0x89, 0x03, 0x00};

		for(int i=4; i<133; i++)
		{
			szCommand[i]=0x00;
		}

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9OSReadMassProductionRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE mp;
			BYTE dump[127];
			BYTE status_type[2];	//Success or fail
		} A9_OSReboot_Response;
	#pragma pack (pop)

		A9_OSReboot_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0x26 && response.status_type[0] == 0x00 && response.status_type[1] == 0x00)
		{
			sprintf(m_szIsMP, "%d", response.mp);
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//---------END of CA9OSReadMassProductionRequest----------//

} // End of namespace PLATFORM_TOOL

