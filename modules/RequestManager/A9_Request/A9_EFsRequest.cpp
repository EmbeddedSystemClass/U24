#include "stdafx.h"
#include "A9_EFsRequest.h"


namespace PLATFORM_TOOL
{

//-------Start CA9EFsBackupRequest
CA9EFsBackupRequest::CA9EFsBackupRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9EFsBackupRequest"), 60000)
	, m_bSuccess(bSuccess)
{

	m_bSuccess = false;
	BYTE szCommand[] = {0xD1, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9EFsBackupRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
	} A9_EFsBackup_Response;
#pragma pack (pop)

	A9_EFsBackup_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD1 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//-------CA9EFsBackupRequest End-------//


//-------Start CA9EFsRestoreRequest
CA9EFsRestoreRequest::CA9EFsRestoreRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9EFsRestoreRequest"), 60000)
	, m_bSuccess(bSuccess)
{

	m_bSuccess = false;
	BYTE szCommand[] = {0xD1, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9EFsRestoreRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
	} A9_EFsRestore_Response;
#pragma pack (pop)

	A9_EFsRestore_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD1 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//-------CA9EFsRestoreRequest End-------//


//-------Start CA9EFsClearRequest
CA9EFsClearRequest::CA9EFsClearRequest(bool& bSuccess)
	: CDiagRequestBase(_T("CA9EFsClearRequest"), 60000)
	, m_bSuccess(bSuccess)
{

	m_bSuccess = false;
	BYTE szCommand[] = {0xD1, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9EFsClearRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;	//Success or fail
	} A9_EFsClear_Response;
#pragma pack (pop)

	A9_EFsClear_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD1 && response.status_type != 0x04)
	{
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//-------CA9EFsClearRequest End-------//

//-------Start CA9EFsVerifyBackupTimesRequest
CA9EFsVerifyBackupTimesRequest::CA9EFsVerifyBackupTimesRequest(char* szBackupTimes, bool& bSuccess)
	: CDiagRequestBase(_T("CA9EFsVerifyBackupTimesRequest"))
	, m_szBackupTimes(szBackupTimes)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szBackupTimes, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD1, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9EFsVerifyBackupTimesRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//Success or fail
		BYTE response_data[4];	// Response data
	} A9_EFsBackupTimes_Response;
#pragma pack (pop)

	A9_EFsBackupTimes_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0xD1 && response.status_type != 0x04)
	{

		std::string strTmp;
		strTmp.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.response_data); ++i)
		{
			sprintf(szTmp, "%X", response.response_data[i]);
			strTmp += szTmp;
		}
		m_bSuccess = true;
		sprintf(m_szBackupTimes, "%s", strTmp.c_str());


	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//-------CA9EFsVerifyBackupTimesRequest End-------//


} // End of namespace PLATFORM_TOOL
