#include "stdafx.h"
#include "AudioRequest.h"

namespace PLATFORM_TOOL
{
	CSpeakerAmpRequest::CSpeakerAmpRequest(bool &bSuccess)
		: CDiagRequestBase(_T("CSpeakerAmpRequest"))
		, m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 113, 0};
		m_bSuccess = false;

		CBufferPtr ptBufferSend;
		Encode (szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CSpeakerAmpRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5];
		Safememcopy((void*)resp, sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bSuccess = (1 == resp[4]);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



    // Detect Is Handset Connected
    CIsHandsetPlugged::CIsHandsetPlugged(bool& bIsPlugged)
        : CDiagRequestBase(_T("CIsHandsetPlugged"))
        , m_bIsPlugged(bIsPlugged)
    {
        BYTE szRAWCommand[] = {75, 250, 115, 0};

        CBufferPtr ptBufferSend;
        Encode (szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
        SetSendData (ptBufferSend);
    }

    void CIsHandsetPlugged::OnDataReceiver(CBufferPtr &ptBufferReceived)
    {
#pragma pack (push, 1)
        typedef struct
        {
            BYTE  cmd_code;
            BYTE  subsys_id;
            short mobile_id;
            BYTE  Result;
        } diag_DetectHandset_response;
#pragma pack (pop)

        diag_DetectHandset_response resp = {0};
        Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

        m_bIsPlugged= (resp.Result == (BYTE)1);

        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }



    // Vibrator
    CVibratorOnOff::CVibratorOnOff(bool &bSuccess, bool bOn)
        : CDiagRequestBase(_T("CVibrator"))
        , m_bSuccess(bSuccess)
        , m_bOn(bOn)
    {
        BYTE szRAWCommand[] = {75, 250, 11, 00, 01, 00} ;
        
        if (!bOn) {
            szRAWCommand[4] = (BYTE)00 ;
        }

        CBufferPtr ptBufferSend;
        Encode(szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
        SetSendData(ptBufferSend);
    }

    void CVibratorOnOff::OnDataReceiver(CBufferPtr &ptBufferReceived)
    {
#pragma pack (push, 1)
        typedef struct
        {
            BYTE  cmd_code;
            BYTE  subsys_id;
            short mobile_id;
            BYTE  Result;
        } diag_VibratorOnOff_response;
#pragma pack (pop)

        diag_VibratorOnOff_response resp = {0};
        Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

        m_bSuccess = (resp.Result == (BYTE)1);

        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
}
