#include "stdafx.h"
#include "A9_SimLockRequest.h"


namespace PLATFORM_TOOL
{

//--------CA9SimLockGetControlKeyRequest
	CA9SimLockGetControlKeyRequest::CA9SimLockGetControlKeyRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9SimLockGetControlKeyRequest"))
		, m_bSuccess(bSuccess)
	{
		//generate file - ck
		BYTE szCommand[] = {0x4b, 0x21, 0x44, 0x00, 0x01, 0x00, 0x63, 0x6b, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockGetControlKeyRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockGetControlKey_Response;
	#pragma pack (pop)

		A9_SimLockGetControlKey_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockGetControlKeyRequest----------//


//--------CA9SimLockGetUnblockKeyRequest
	CA9SimLockGetUnblockKeyRequest::CA9SimLockGetUnblockKeyRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9SimLockGetUnblockKeyRequest"))
		, m_bSuccess(bSuccess)
	{
		// generate file - ubkey
		BYTE szCommand[] = {0x4b, 0x21, 0x44, 0x00, 0x01, 0x05, 0x55, 0x42, 0x4B, 0x45, 0x59, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockGetUnblockKeyRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockGetControlKey_Response;
	#pragma pack (pop)

		A9_SimLockGetControlKey_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockGetUnblockKeyRequest----------//
	
//--------CA9SimLockWriteSimDataRequest
	CA9SimLockWriteSimDataRequest::CA9SimLockWriteSimDataRequest(bool& bSuccess, BYTE* szSimDataBuf, int szSize)
		: CDiagRequestBase(_T("CA9SimLockWriteSimDataRequest"))
		, m_bSuccess(bSuccess)
	{
		//int iSize = szSize+6;
		BYTE szCommand[262] = {0x4b, 0x21, 0x43, 0x00, 0x01, 0x00};

	
		for(int i=0; i< szSize; i++)
		{
			 szCommand[i+6] = *(szSimDataBuf+i);
		}
		int iCmdLenght = 6+szSize+1;
		szCommand[iCmdLenght-1] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szCommand, iCmdLenght, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockWriteSimDataRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockWriteSimData_Response;
	#pragma pack (pop)

		A9_SimLockWriteSimData_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockWriteSimDataRequest----------//


//--------CA9SimLockGetSimDataRequest
	CA9SimLockGetSimDataRequest::CA9SimLockGetSimDataRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9SimLockGetSimDataRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0x4b, 0x21, 0xAE, 0x00, 0x01, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00};
 
		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockGetSimDataRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockGetSimData_Response;
	#pragma pack (pop)

		A9_SimLockGetSimData_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockGetSimDataRequest----------//

//--------CA9SimLockDeactivateRequest
	CA9SimLockDeactivateRequest::CA9SimLockDeactivateRequest(bool& bSuccess, char* szControlKey)
		: CDiagRequestBase(_T("CA9SimLockDeactivateRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0x4b, 0x21, 0x41, 0x00, 0x01, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00};
		std::string strControlKey = szControlKey;
		for(int i=0; i< strControlKey.size(); i++)
		{
			 szCommand[i+6] =(BYTE) (atoi (strControlKey.substr(i, 1).c_str()) +48);
		}


		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockDeactivateRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockDeactivate_Response;
	#pragma pack (pop)

		A9_SimLockDeactivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockDeactivateRequest----------//

//--------CA9SimLockActivateRequest
	CA9SimLockActivateRequest::CA9SimLockActivateRequest(bool& bSuccess, char* szControlKey )
		: CDiagRequestBase(_T("CA9SimLockActivateRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0x4b, 0x21, 0x40, 0x00, 0x01, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00};

		std::string strControlKey = szControlKey;
		for(int i=0; i< strControlKey.size(); i++)
		{
			 szCommand[i+6] =(BYTE) (atoi (strControlKey.substr(i, 1).c_str()) +48);
		}


		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockActivateRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockActivate_Response;
	#pragma pack (pop)

		A9_SimLockActivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockActivateRequest----------//



//--------CA9SimLockEFsOpenRequest
	CA9SimLockEFsOpenRequest::CA9SimLockEFsOpenRequest(bool& bSuccess, char* szFileName )
		: CDiagRequestBase(_T("CA9SimLockEFsOpenRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[256] = {0x4b, 0x13, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
		char szBuf[4];
		memset(szBuf, 0, 4);

  
		std::string strFileName = szFileName;
		for(int i=0; i< strFileName.size(); i++)
		{
			sprintf (szBuf,"%c", *(szFileName+i) );
			szCommand[i+12] =(BYTE) *szBuf;
		}



		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockEFsOpenRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type[3];			// Diagnostic type or error code
			BYTE cmd_code[5];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockActivate_Response;
	#pragma pack (pop)

		A9_SimLockActivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9SimLockEFsOpenRequest----------//


//--------CA9SimLockEFsReadRequest
	CA9SimLockEFsReadRequest::CA9SimLockEFsReadRequest(bool& bSuccess, char* szContent)
		: CDiagRequestBase(_T("CA9SimLockEFsReadRequest"))
		, m_bSuccess(bSuccess)
		, m_szContent(szContent)
	{
		BYTE szCommand[] = {0x4b, 0x13, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockEFsReadRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type[3];		// Diagnostic type or error code
			BYTE cmd_code[13];		// Command code
			BYTE status[4];			// Success or fail
			BYTE content[255];		// response data
		} A9_SimLockActivate_Response;
	#pragma pack (pop)

		A9_SimLockActivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			memcpy(m_szContent, response.content, 255);
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

//---------END of CA9SimLockEFsReadRequest----------//


//--------CA9SimLockEFsCloseRequest
	CA9SimLockEFsCloseRequest::CA9SimLockEFsCloseRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9SimLockEFsCloseRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0x4b, 0x13, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockEFsCloseRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type[3];		// Diagnostic type or error code
			BYTE cmd_code;		// Command code
			BYTE status[4];			// Success or fail
		} A9_SimLockActivate_Response;
	#pragma pack (pop)

		A9_SimLockActivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9SimLockEFsReadRequest----------//

//--------CA9SimLockEFsDeleteRequest
	CA9SimLockEFsDeleteRequest::CA9SimLockEFsDeleteRequest(bool& bSuccess, char* szFileName )
		: CDiagRequestBase(_T("CA9SimLockEFsDeleteRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[256] = {0x4b, 0x13, 0x08, 0x00};
		char szBuf[4];
		memset(szBuf, 0, 4);
  
		std::string strFileName = szFileName;
		for(int i=0; i< strFileName.size(); i++)
		{
			sprintf (szBuf,"%c", *(szFileName+i) );
			 szCommand[i+4] =(BYTE) *szBuf;
		}

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockEFsDeleteRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type[2];			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockActivate_Response;
	#pragma pack (pop)

		A9_SimLockActivate_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9SimLockEFsDeleteRequest----------//

//--------CA9SimLockSimCardStatusRequest
	CA9SimLockSimCardStatusRequest::CA9SimLockSimCardStatusRequest(bool& bSuccess, char* szResponse )
		: CDiagRequestBase(_T("CA9SimLockSimCardStatusRequest"))
		, m_bSuccess(bSuccess)
		, m_szResponse(szResponse)
	{
		m_bSuccess = false;
		BYTE szCommand[] = {0xD1, 0x6F, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		
		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9SimLockSimCardStatusRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status;		//Command Success or fail
			BYTE sim_status;	//SIM Status
		} A9_SimLockSimCardStatus_Response;
	#pragma pack (pop)

		A9_SimLockSimCardStatus_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD1 && response.status == 0x00)
		{
			m_bSuccess = true;
			sprintf(m_szResponse, "%X", response.sim_status);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9SimLockSimCardStatusRequest----------//


//--------CA9_SimLock_DeleteFolder_mmgsdiRequest
	CA9_SimLock_DeleteFolder_mmgsdiRequest::CA9_SimLock_DeleteFolder_mmgsdiRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9_SimLock_DeleteFolder_mmgsdiRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0x4b, 0x13, 0x25, 0x00, 0x00, 0x00, 0x2f, 0x6d, 0x6d, 0x67, 0x73, 0x64, 0x69, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9_SimLock_DeleteFolder_mmgsdiRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[3];		// Command code
			BYTE status[4];	//Success or fail
		} A9_SimLockDeleteFolder_Response;
	#pragma pack (pop)

		A9_SimLockDeleteFolder_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		std::string strTmp_status;

		strTmp_status.clear();
		char szTmp[4];
		memset(szTmp, 0, 4);

		for (int i=0; i<sizeof(response.status); ++i)
		{
			sprintf(szTmp, "%02X", response.status[i]);
			strTmp_status += szTmp;
		}

		if (response.diag_type == 0x4b && strTmp_status == "00000000")
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9_SimLock_DeleteFolder_mmgsdiRequest----------//

//--------CA9_SimLock_GeneratePersonEngineRequest
	CA9_SimLock_GeneratePersonEngineRequest::CA9_SimLock_GeneratePersonEngineRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9_SimLock_GeneratePersonEngineRequest"), 10000)
		, m_bSuccess(bSuccess)
	{
		BYTE szCommand[] = {0xD1, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9_SimLock_GeneratePersonEngineRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status;	//Success or fail
		} A9_SimLockGeneratePersonEngine_Response;
	#pragma pack (pop)

		A9_SimLockGeneratePersonEngine_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());


		if (response.diag_type == 0xD1 && response.status == 0x00)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}
//---------END of CA9_SimLock_DeleteFolder_mmgsdiRequest----------//

} // End of namespace PLATFORM_TOOL

