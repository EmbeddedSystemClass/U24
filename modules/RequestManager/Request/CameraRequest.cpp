#include "stdafx.h"
#include "CameraRequest.h"

namespace PLATFORM_TOOL
{
	CWolfCameraRequest::CWolfCameraRequest(bool &bSuccess, BYTE funcIndex)
		: CDiagRequestBase(_T("CWolfCameraRequest"), 20000)
		, m_bSuccess(bSuccess)
	{
		// Index:
		// 01 : send camera key event
		// 02 : send touch event to allow position information for preview
		// 03 : Send focus key event to execute auto-focus
		// 04 : Send back key event to exit camera application
		// 05 : Copy picture form WM file system to EFS
		// 06 : Check snapshot status per 2 sec
		// 07 : --
		// 11 : Check preview status per 2 sec
		// 15



		//Send camera key event: sendrawrequest 75 250 71 00 00 00 01
		//Response (success): 0x4B FA 47 00 00 00 01 00 01 00
		//Response (fail):    0x4B FA 47 00 00 00 01 00 00 00

		BYTE szRAWCommand[] = {75, 250, 71, 0, 0, 0, 0};
		szRAWCommand[6] = funcIndex;
		m_bSuccess = false;

		CBufferPtr ptBufferSend;
		Encode (szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfCameraRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[10];
		Safememcopy((void*)resp, sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bSuccess = (1 == resp[8]);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfCameraAFStatusRequest::CWolfCameraAFStatusRequest(bool &bSuccess)
		: CDiagRequestBase(_T("CWolfCameraAFStatusRequest"), 20000)
		, m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 70, 0};
		m_bSuccess = false;

		CBufferPtr ptBufferSend;
		Encode (szRAWCommand, sizeof(szRAWCommand), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfCameraAFStatusRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[10];
		Safememcopy((void*)resp, sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bSuccess = (1 == resp[8]);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


}
