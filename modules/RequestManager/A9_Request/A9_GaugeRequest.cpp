#include "stdafx.h"
#include "A9_GaugeRequest.h"


namespace PLATFORM_TOOL
{
	CA9GaugeReadIDRequest::CA9GaugeReadIDRequest(bool& bSuccess, char*szID)
		: CDiagRequestBase(_T("CA9GaugeReadIDRequest"))
		, m_bSuccess(bSuccess)
		, m_szID(szID)
	{

		m_bSuccess = false;
		BYTE szCommand[] = {0xD2, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GaugeReadIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE cmd_code[4];
			BYTE unknown1;
			BYTE unknown2;
		} A9_GaugeID_Response;
#pragma pack (pop)
		
		A9_GaugeID_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.unknown1 == 0x20 && response.unknown2 == 0x05)
		{
			m_bSuccess = true;
			strcpy(m_szID, "2005");
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CA9GaugeupdateFWRequest::CA9GaugeupdateFWRequest(bool& bSuccess, char *szRequireVer)
		: CDiagRequestBase(_T("CA9GaugeupdateFWRequest"), 20000)
		, m_bSuccess(bSuccess)
		, m_RequireVer(atoi(szRequireVer))
	{

		m_bSuccess = false;
		BYTE szCommand[] = {0xD2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GaugeupdateFWRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE cmd_code[4];
			BYTE version;
		} A9_GaugeupdateFW_Response;
#pragma pack (pop)

		A9_GaugeupdateFW_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.version == this->m_RequireVer)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CA9GaugeCheckThermalRequest::CA9GaugeCheckThermalRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9GaugeCheckThermalRequest"), 20000)
		, m_bSuccess(bSuccess)
	{

		m_bSuccess = false;
		BYTE szCommand[] = {0xD2, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GaugeCheckThermalRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE cmd_code[4];
			BYTE status;
		} A9_GaugeCheckThermal_Response;
#pragma pack (pop)

		A9_GaugeCheckThermal_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_bSuccess = response.status == 0x01;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
} // End of namespace PLATFORM_TOOL
