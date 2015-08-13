#include "stdafx.h"
#include "A9_NFCRequest.h"


namespace PLATFORM_TOOL
{


	CA9NFCIDRequest::CA9NFCIDRequest(char* szNFCID, bool& bSuccess)
		: CDiagRequestBase(_T("CA9NFCIDRequest"))
		, m_szNFCID(szNFCID)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szNFCID, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9NFCIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
			typedef struct
			{
				BYTE diag_type;			// Diagnostic type or error code
				BYTE cmd_code[2];		// Command code
				BYTE status_type;	//Success or fail
				BYTE response_id[6];	// Response data
			} A9_NFCID_Response;
		#pragma pack (pop)

		A9_NFCID_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			std::string strTmp;
			strTmp.clear();
			char szTmp[4];
			memset(szTmp, 0, 4);

			for (int i=0; i<sizeof(response.response_id); ++i)
			{
				sprintf(szTmp, "%02X", response.response_id[i]);
				strTmp += szTmp;
			}

			m_bSuccess = true;
			sprintf(m_szNFCID, "%s", strTmp.c_str());
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}




	CA9NFCCardIDRequest::CA9NFCCardIDRequest(char* szNFCCardID, bool& bSuccess)
		: CDiagRequestBase(_T("CA9NFCCardIDRequest"))
		, m_szNFCCardID(szNFCCardID)
		, m_bSuccess(bSuccess)
	{
		sprintf(m_szNFCCardID, "");
		m_bSuccess = false;

		BYTE szCommand[] = {0xD2, 0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9NFCCardIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
			typedef struct
			{
				BYTE diag_type;			// Diagnostic type or error code
				BYTE cmd_code[2];		// Command code
				BYTE status_type;	//Success or fail
				BYTE response_id[4];	// Response data
			} A9_NFCCardID_Response;
		#pragma pack (pop)

		A9_NFCCardID_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD2 && response.status_type != 0x04)
		{
			std::string strTmp;
			strTmp.clear();
			char szTmp[4];
			memset(szTmp, 0, 4);

			for (int i=0; i<sizeof(response.response_id); ++i)
			{
				sprintf(szTmp, "%02X", response.response_id[i]);
				strTmp += szTmp;
			}

			m_bSuccess = true;
			sprintf(m_szNFCCardID, "%s", strTmp.c_str());
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


} // End of namespace PLATFORM_TOOL
