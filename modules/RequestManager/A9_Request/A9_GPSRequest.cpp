#include "stdafx.h"
#include "A9_GPSRequest.h"


namespace PLATFORM_TOOL
{

//---------CA9GPSCMDModeOnRequest
	CA9GPSCMDModeOnRequest::CA9GPSCMDModeOnRequest(bool& bSuccess, bool bOn)
		: CDiagRequestBase(_T("CA9GPSCMDModeOnRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x4B, 0x0D, 0x65, 0x00, 0x14, 0x01, 0x00};

        if (!bOn) {
            szCommand[6] = 0x01;
        }

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GPSCMDModeOnRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_subsys_gps;
			BYTE diag_gps_sub_cmd[2];
			BYTE gps_cmd_mode_switch_on;
			BYTE status;
		} A9_GPSCMDModeOn_Response;
	#pragma pack (pop)

	A9_GPSCMDModeOn_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x4B && response.status == 0x00)
	{
		m_bSuccess = true;
		//memcpy(m_szMHLID, response.response_data, 12);
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9GPSCMDModeOnRequest-------------//

//---------CA9GPSCMDModeOnRequest_V2
	CA9GPSCMDModeOnRequest_V2::CA9GPSCMDModeOnRequest_V2(bool& bSuccess)
		: CDiagRequestBase(_T("CA9GPSCMDModeOnRequest_V2"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD1, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GPSCMDModeOnRequest_V2::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE b1;			// Diagnostic type or error code
			BYTE b2;
			BYTE b3;
			BYTE b4;
		} A9_GPSCMDModeOn_V2_Response;
#pragma pack (pop)

		A9_GPSCMDModeOn_V2_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (0x04 != response.b4)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//------------End of CA9GPSCMDModeOnRequest_V2-------------//

//---------CA9GNSSRFMultichannelTrackingRequest
	CA9GNSSRFMultichannelTrackingRequest::CA9GNSSRFMultichannelTrackingRequest(bool& bSuccess)
		: CDiagRequestBase(_T("CA9GNSSRFMultichannelTrackingRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0x4B, 0x0D, 0x65, 0x00, 0x1B, 0x32, 0x04, 0x00, 0x01, 0x00, 0x00, 0x07, 0x01};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GNSSRFMultichannelTrackingRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_subsys_gps;
			BYTE diag_gps_sub_cmd[2];
			BYTE gps_cmd_mode_switch_on;
			BYTE status;
		} A9_GNSSRFMultichannelTracking_Response;
	#pragma pack (pop)

		A9_GNSSRFMultichannelTracking_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.status == 0x00)
		{
			m_bSuccess = true;
			//memcpy(m_szMHLID, response.response_data, 12);
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9GNSSRFMultichannelTrackingRequest-------------//

//---------CA9GNSSRFMultichannelTrackingRequest_V2
	CA9GNSSRFMultichannelTrackingRequest_V2::CA9GNSSRFMultichannelTrackingRequest_V2(bool& bSuccess)
		: CDiagRequestBase(_T("CA9GNSSRFMultichannelTrackingRequest_V2"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD1, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GNSSRFMultichannelTrackingRequest_V2::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
#pragma pack (push, 1)
		typedef struct
		{
			BYTE b1;			// Diagnostic type or error code
			BYTE b2;
			BYTE b3;
			BYTE b4;
		} A9_GNSSRFMultichannelTracking_Response_V2;
#pragma pack (pop)

		A9_GNSSRFMultichannelTracking_Response_V2 response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (0x04 != response.b4)
		{
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//------------End of CA9GNSSRFMultichannelTrackingRequest-------------//

//---------CA9GetGPSCNoRequest
	CA9GetGPSCNoRequest::CA9GetGPSCNoRequest(char* szGPSCNo ,bool& bSuccess)
		: CDiagRequestBase(_T("CA9GetGPSCNoRequest"))
		, m_szGPSCNo(szGPSCNo)
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		BYTE szCommand[] = {0xD1, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szCommand, sizeof(szCommand), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CA9GetGPSCNoRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
	#pragma pack (push, 1)
		typedef struct
		{
			BYTE diag_type;			// Diagnostic type or error code
			BYTE diag_cmd[2];
			BYTE status_type;	//Success or fail
			BYTE response_data[2];
		} A9_GPSCNo_Response;
	#pragma pack (pop)

		A9_GPSCNo_Response response = {0};
		Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (response.status_type != 0x04)
		{
			BYTE temp = response.response_data[0];
			response.response_data[0] = response.response_data[1];
			response.response_data[1] = temp;

			unsigned short *iCN0 = (unsigned short *)response.response_data;

			double dCN0 = *iCN0 / 10.0;

			sprintf(m_szGPSCNo, "%.1f", dCN0);

			m_bSuccess = true;
		}
		/*
		if (response.status_type != 0x04)
		{
			std::string strTmp;
			strTmp.clear();
			char szTmp[4];
			memset(szTmp, 0, 4);

			for (int i=0; i<sizeof(response.response_data); ++i)
			{
				sprintf(szTmp, "%x", response.response_data[i]);
				strTmp += szTmp;
			}

			m_bSuccess = true;
			//memcpy(m_szGPSCNo, response.response_data, 2);
			sprintf(m_szGPSCNo, "%s", strTmp.c_str());
		}
		*/
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
//------------End of CA9GetGPSCNoRequest-------------//


} // End of namespace PLATFORM_TOOL
