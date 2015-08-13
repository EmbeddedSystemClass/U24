#include "stdafx.h"
#include "LEDRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{
	CNLEDRequest::CNLEDRequest(bool &result)
		: CDiagRequestBase(_TC("CNLEDRequest"))
		, m_bResult(result)
	{		
		//sendrawrequest 75 250 21 00

		BYTE req[] = {75, 250, 21, 0};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req),ptBufferSend);
		SetSendData(ptBufferSend);

		m_bResult = false;
	}

	CNLEDRequest::~CNLEDRequest()
	{
	}

	bool CNLEDRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CNLEDRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		// response: 4B FA 15 00 FF 01
		// 傳回來的值倒數第二個為FF,最後一個為01表示有正確讀到nled的register

		BYTE resp[6];
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());		
		m_bResult = (resp[4] == 0xFF) && (resp[5]==0x01);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}	
}