#include "stdafx.h"
#include "LCDRequest.h"


namespace PLATFORM_TOOL
{


CWolfLCDOnOffSwitchRequest::CWolfLCDOnOffSwitchRequest(bool& bResult, bool bOn)
	: CDiagRequestBase(_T("CWolfLCDOnOffSwitchRequest"))
	, m_Success(bResult)
	, m_bOn(bOn)
{
	m_Success = false;

	BYTE szRAWCommand[] = {75, 250, 116, 0};

	if (! bOn)
		szRAWCommand[2] = (BYTE)118;

	CBufferPtr ptBufferSend;
	Encode(szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CWolfLCDOnOffSwitchRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{
#pragma pack (push, 1)
	typedef struct
	{
		BYTE  cmd_code;
		BYTE  subsys_id;
		short mobile_id;
		BYTE  Result;
	} diag_Wolf_LCDOnOffSwitch_response;
#pragma pack (pop)

	diag_Wolf_LCDOnOffSwitch_response resp = {0};
	Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	m_Success = (resp.Result == (BYTE)3);

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}


} // End of namespace PLATFORM_TOOL
