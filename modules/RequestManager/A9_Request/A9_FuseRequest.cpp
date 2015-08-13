#include "stdafx.h"
#include "A9_FuseRequest.h"


namespace PLATFORM_TOOL
{
CA9FuseReadStatusRequest::CA9FuseReadStatusRequest(char* szFuseType, bool& bSuccess)
	: CDiagRequestBase(_T("CA9FuseReadStatusRequest"))
	, m_szFuseStatus(szFuseType)
	, m_bSuccess(bSuccess)
{
	sprintf(m_szFuseStatus, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD1, 0x70, 0x26, 0x00, 0x00,						//BYTE: 1, 1, 1, 2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//BYTE: 77
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00};														//BYTE: 2

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9FuseReadStatusRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type[4];		// Diagnostic type or error code
		BYTE fuse_type[2];		// Fuse Status
		BYTE cmd_code[77];		// Command code
		BYTE status_type[2];	// Success or fail
		BYTE response_crc[2];	// CRC
	} A9_FuseSize_Response;
#pragma pack (pop)

	A9_FuseSize_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	unsigned short unStatus = *(unsigned short*)response.status_type;

	if (unStatus == 0)
	{
		memcpy(m_szFuseStatus, response.fuse_type, 2);

		if( m_szFuseStatus != NULL )
			m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


CA9QFuseBlowRequest::CA9QFuseBlowRequest(char* szFuseType, bool& bSuccess)
: CDiagRequestBase(_T("CA9QFuseBlowRequest"))
, m_szFuseStatus(szFuseType)
, m_bSuccess(bSuccess)
{
	sprintf(m_szFuseStatus, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0xD1, 0x71, 0x26, 0x00, 0x00,						//BYTE: 1, 1, 1, 2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//BYTE: 77
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00};														//BYTE: 2

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9QFuseBlowRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type[4];		// Diagnostic type or error code
		BYTE fuse_type[1];		// Fuse Status
		BYTE cmd_code[77];		// Command code
		BYTE status_type[2];	// Success or fail
		BYTE response_crc[2];	// CRC
	} A9_FuseSize_Response;
#pragma pack (pop)

	A9_FuseSize_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	unsigned short unStatus = *(unsigned short*)response.status_type;

	if (unStatus == 0)
	{
		m_bSuccess = true;
		memcpy(m_szFuseStatus, response.fuse_type, 2);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}

} // End of namespace PLATFORM_TOOL
