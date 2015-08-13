#include "stdafx.h"
#include "BTRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{
	CWolf_BTModuleSwitchRequest::CWolf_BTModuleSwitchRequest(bool bOn, bool& bResult)
		: CDiagRequestBase(_T("CWolf_BTModuleSwitchRequest"), 10000, true)
		, m_bResult(bResult)
	{
		m_bResult = false;

		BYTE req[5] = {75, 250, 55, 0, 0};

		if (bOn)
			req[4] = 3;
		else
			req[4] = 4;

		CBufferPtr ptBufferSend;
		Encode(req, 5, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolf_BTModuleSwitchRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 55, 0, 0};

		Safememcopy(&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		/*
		if (resp[4] == 1)
			m_bResult = true;
		else
			m_bResult = false;
		*/

		// Currently there's no check bit for this DIAG command
		m_bResult = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopard_BT_FM_Request::CLeopard_BT_FM_Request (int commandCode, struct_32byte& buffer, bool& bResult)
		:CDiagRequestBase(_T("CLeopard_BT_FM_Request"))
		,m_InOutBuffer (buffer)
		,m_bResult(bResult)
	{
		m_wCmdCode = commandCode;
		m_bResult = false;

		CBufferPtr ptBufferSend;
		
		//cmdCode |  cmdLength | responseLen |  struct_32byte usage                                        comment
		//--------+------------+-------------+-------------------------------------------------------+-------------------------
		//50        6 byte         23 byte      2  [IN ] : cmd[5-6]                                        BT/FM module ID read          75 250 50 0 0/1 0
		//                                      16 [OUT]: 16 byte BT/FM Module ID
		//									  other   : reserved
		//51        4              6            --                                                         FM Start
		//52        4              6            --                                                         FM Stop
		//53        8              6            4  [IN ]: frequency                                        FM tune
		//54        4              6            2  [OUT]:                                                  FM read RSSI
		//55        5              5            1  [IN ]: Power level  (0-high, 1-middle, 2-low)           BT TXPower,                  


		int cmdlen = 0;
		BYTE BT_FM_cmd[10] = {75, 250, (BYTE)m_wCmdCode,0,0,0,0,0,0,0};

		switch (m_wCmdCode)
		{
			case 50: //BT/FM module ID read
				cmdlen = 6;
				memcpy (&BT_FM_cmd[4], &m_InOutBuffer.Reserved, 2);
				break;

			case 51: // FM start
			case 52: // FM stop
			case 54: // FM read RSSI
				cmdlen = 4;
				break;


			case 53: // FM tune
				cmdlen = 8;
				memcpy (&BT_FM_cmd[4], &m_InOutBuffer.Reserved, 4);
				break;

			case 55: // BT TXPower
				cmdlen = 5;
				BT_FM_cmd[4] = m_InOutBuffer.Reserved[0];
				break;

			default: // FMstart
				return;
		}

		Encode(BT_FM_cmd, cmdlen, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopard_BT_FM_Request::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE szResponseCmd[24] = {75, 250, 0, 0, 0, 0, 0, 0, 0, 0};
		*((WORD*)&szResponseCmd[2]) = m_wCmdCode;

		const char *pResp = ptBufferReceived->GetData();

		DWORD resplen = 0;
		switch (m_wCmdCode)
		{
			case 50: // BT / FM module ID read
				resplen = 23;
				if (ptBufferReceived->GetLength() >= resplen && 
					0 == ::memcmp(pResp, szResponseCmd, 6) &&
					pResp[22] == 0)
				{
					memcpy (&m_InOutBuffer.Reserved[2], &pResp[6], 16);
					m_bResult = true;
				}
		
				break;

			case 51: // FM start
			case 52: // FM stop
			case 53: // FM tune
				resplen = 6;
				if (ptBufferReceived->GetLength() >= resplen && 
					0 == ::memcmp(pResp, szResponseCmd, 4) &&
					pResp[4] == 1)
				{
					m_bResult = true;
				}
				break;

			case 54: // Read RSSI
				resplen = 6;
				if (ptBufferReceived->GetLength() >= resplen && 
					0 == ::memcmp(pResp, szResponseCmd, 4))
				{
					memcpy (&m_InOutBuffer.Reserved, &pResp[4], 2);
					if (1 < *((WORD*)m_InOutBuffer.Reserved))
						m_bResult = true;
				}
				break;

			case 55:
				resplen = 5;
				if (ptBufferReceived->GetLength() >= resplen && 
					0 == ::memcmp(pResp, szResponseCmd, 4) &&
					pResp[4] == 1)
				{
					m_bResult = true;
				}
				break;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopardBTPowerOffRequest::CLeopardBTPowerOffRequest(bool& bResult)
		: CDiagRequestBase (_T("CLeopardBTPowerOffRequest"))
		, m_bResult (bResult)
	{
		m_bResult = false;

		BYTE req[4] = {75, 250, 57, 0};

		CBufferPtr ptBufferSend;
		Encode(req, 4, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardBTPowerOffRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 57, 0, 0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);
		//assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CBTTestModeRequest::CBTTestModeRequest(bool bEnetr,bool& bResult)
		:CDiagRequestBase(_T("CBTTestModeRequest"))
		,m_bEnetr(bEnetr)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;



		/*
		000159: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be7b4 (Endpoint Address: 0x4)
		Send 0x8 bytes to the device:
		4B 03 0D 00 DD 63 9E 7E
		000160: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be794 (Endpoint Address: 0x84)
		Get 0x7 bytes from the device:
		4B 03 0D 00 60 75 7E

		BYTE szEnterTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDD};
		BYTE szLeaveTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDE};
		*/

		// command for Leopard / Wolf
		BYTE szEnterTestCmd[] = {75, 250, 56, 0};
		BYTE szLeaveTestCmd[] = {75, 250, 57, 0}; //same as BT TxPowerOff

		if(bEnetr)
			Encode(szEnterTestCmd,5,ptBufferSend);
		else 
			Encode(szLeaveTestCmd,5,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTTestModeRequest::~CBTTestModeRequest()
	{
	}

	bool CBTTestModeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CBTTestModeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		//if(ptBufferReceived->GetLength() >= 0)
		//	m_bResult = true;

		BYTE resp[5];
		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);	
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

}