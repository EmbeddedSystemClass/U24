#include "stdafx.h"
#include "PhoneInfoRequest.h"

namespace PLATFORM_TOOL
{
	CGetePINStatusRequest::CGetePINStatusRequest (bool &bSuccess)
		: CDiagRequestBase (_T("CGetePINStatusRequest"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
		//m_Size = 0;
		BYTE szRAWCommand[] = {75, 33, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGetePINStatusRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE response[29] = {0};
		Safememcopy((void*)&response, 29,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if( response[5] == 0)
			m_Success = true;
		else
			m_Success = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CA9PhoneInfoWriteDefaultIMEIRequest::CA9PhoneInfoWriteDefaultIMEIRequest(bool& bSuccess, char* szIMEI)
		: CDiagRequestBase(_T("CA9PhoneInfoWriteDefaultIMEIRequest"), 60000)
		, m_szIMEI(szIMEI)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;
		sprintf(m_szIMEI, "");

		BYTE szCommand[] = {0xD1, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9PhoneInfoWriteDefaultIMEIRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{ 
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE cmd_code[2];		// Command code
			BYTE status_type;		//Success or fail
			BYTE imei[9];
		// Response data
		} A9_PhoneInfoWriteDefaultIMEI_Response;
	#pragma pack (pop)

		A9_PhoneInfoWriteDefaultIMEI_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.diag_type == 0xD1 && response.status_type == 0x00)
		{
			std::string strTmp;
			strTmp.clear();
			char szTmp[3];
			memset(szTmp, 0, 3);

			for(int j = 0; j <9; j++)
			{
				sprintf(szTmp, "%02X", (response.imei[j]));
				strTmp += szTmp;
			}
			sprintf(m_szIMEI, "%s", strTmp.c_str());
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

}