#include "stdafx.h"
#include "FMRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{
	CFMRequest::CFMRequest(WORD nTestCase,WORD nPayload1,WORD nPayload2,WORD& nRSSI,bool& bResult)
		:CDiagRequestBase(_TC("CFMRequest"))
		,m_nTestCase(nTestCase)
		,m_nPayload1(nPayload1)
		,m_nPayload2(nPayload2)
		,m_nRSSI(nRSSI)
		,m_bResult(bResult)
	{		
	#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			WORD testCase;
			WORD payload1;
			WORD payload2;
		} diag_FM_req;
	#pragma pack(pop)

		//diag_FM_req req = {75,69,1};
		diag_FM_req req = {75,249,1};//For qualcomm's change codbase, 2009/2/17 alan tu
		req.testCase = m_nTestCase;
		req.payload1 = m_nPayload1;
		req.payload2 = m_nPayload2;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req,10,ptBufferSend);
		SetSendData(ptBufferSend);

		m_bResult = false;
	}

	CFMRequest::~CFMRequest()
	{
	}

	bool CFMRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CFMRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
	#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			WORD result;
		} diag_FM_resp;
	#pragma pack(pop)

		diag_FM_resp resp = {0};
		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());		
		if(m_nTestCase == 20)
		{
			m_nRSSI = resp.result;
			m_bResult = true;
		}
		else
			m_bResult = resp.result == 0; //0: success

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}	
}