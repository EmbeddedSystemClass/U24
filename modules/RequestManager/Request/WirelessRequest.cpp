#include "stdafx.h"
#include "WirelessRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\BQPacketLib.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{



	CLeopardTurnOnTXPowerRequest::CLeopardTurnOnTXPowerRequest (BYTE nChannel, bool &bResult)
		: CDiagRequestBase (_T("CLeopardTurnOnTXPowerRequest"))
		, m_bResult (bResult)
	{
		m_bResult = false;
		//if (nChannel > 7)
		//	nChannel = 0;
		

		// Send: 75 250 [41 0] x
		// Resp: 75 250 [41 0] 0/1

		// if channel == -1 means turn off WLAN
		// cmd: 75 250 43 0 0 0 0

		BYTE req[8] = {75, 250, 41, 0, 0};
		int cmdLen;

		if (nChannel == 255)
		{
			req[2] = 43;
			cmdLen = 7;
		}
		else 
		{
			req[4] = nChannel;
			cmdLen = 5;
		}

		CBufferPtr ptBufferSend;
		Encode(req, cmdLen, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardTurnOnTXPowerRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 41, 0, 0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);
		assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopardTurnOnTXPower2Request::CLeopardTurnOnTXPower2Request (BYTE Rate, BYTE nChannel, BYTE Power, bool &bResult)
		: CDiagRequestBase (_T("CLeopardTurnOnTXPower2Request"))
		, m_bResult (bResult)
	{
		m_bResult = false;

		// Send: 75 250 43 0 11 6 16 (75 250 43 0 rate - channel - power)
		// Resp: 75 250 43 0 0/1

		BYTE req[7] = {75, 250, 43, 0, Rate, nChannel, Power};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardTurnOnTXPower2Request::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 43, 0, 0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);
		//m_bResult = true;
		assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



	CLeopardChangeWifiFWtoNativeRequest::CLeopardChangeWifiFWtoNativeRequest (bool &bResult)
		: CDiagRequestBase (_T("CLeopardChangeWifiFWtoNativeRequest"))
		, m_bResult (bResult)
	{
		m_bResult = false;

		// Send: 75 250 [42 0]
		// Resp: 75 250 [42 0] 0/1
		BYTE req[4] = {75, 250, 42, 0};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardChangeWifiFWtoNativeRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 42, 0, 0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);
		assert (m_bResult);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopard_BT_FM_Request::CLeopard_BT_FM_Request (int commandCode, struct_32byte& buffer, bool& bResult)
		:CDiagRequestBase(_TC("CLeopard_BT_FM_Request"))
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
	
	CLeopardWifiTestRequest::CLeopardWifiTestRequest (BYTE rate, BYTE channel, BYTE power, bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardWifiTestRequest"))
		, m_bResult (bSuccess)
	{
		m_bResult = false;

		// Send: 75 250 46 0 [rate] [channel] [power]
		// Resp:  4B FA 2E 00 0x 00 , sucess: 1, fail: 0

		BYTE req[7] = {75, 250, 46, 0, rate, channel, power};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardWifiTestRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE resp[6] = {75, 250, 46, 0, 0, 0};

		Safememcopy (&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);

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
		assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}








	







	CEnableGAPRequest::CEnableGAPRequest(bool bEnableGap,bool& bResult)
		:CDiagRequestBase(_TC("CEnableGAPRequest"),10000)
		,m_bEnableGap(bEnableGap)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		//Send: 4B 03 01 00 00 A8 36 7E
		//Get:4B 03 01 00 01 21 27 7E

		//Send: 4B 03 01 00 01 21 27 7E
		//Get: 4B 03 01 00 02 BA 15 7E

		BYTE szEnableGapCmd[] = {0x4B, 0x03, 0x01, 0x00, 0x00};
		BYTE szDisableGapCmd[] = {0x4B, 0x03, 0x01, 0x00, 0x01};
		if(m_bEnableGap)
			Encode(szEnableGapCmd,5,ptBufferSend);
		else			
			Encode(szDisableGapCmd,5,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEnableGAPRequest::~CEnableGAPRequest()
	{
	}

	bool CEnableGAPRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);;
	}

	void CEnableGAPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if(m_bEnableGap)
		{
			BYTE szResponseCmd[] = {0x4B, 0x03, 0x01, 0x00,0x01};
			if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
				m_bResult = true;
		}
		else
		{
			BYTE szResponseCmd[] = {0x4B, 0x03, 0x01, 0x00,0x02};
			if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
				m_bResult = true;
		}
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	
	CBTTxPowerRequest::CBTTxPowerRequest(int nDegree,bool& bResult)
		:CDiagRequestBase(_TC("CBTTxPowerRequest"))
		,m_nDegree(nDegree)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szHighPowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF0};
		BYTE szMiddlePowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF1};
		BYTE szLowPowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF2};

		/*
		2. TX power - high
		Send: 4B 03 0E 00 F0 E0 8B 7E
		Get: 4B 03 0E 00 00 6F 7C 7E

		3. TX power - middle
		Send: 4B 03 0E 00 F1 69 9A 7E
		Get: 4B 03 0E 00 00 6F 7C 7E

		4. TX power - low
		Send: 4B 03 0E 00 F2 F2 A8 7E
		Get: 4B 03 0E 00 00 6F 7C 7E
		*/

		if(nDegree == 0xF0)
			Encode(szHighPowerCmd,5,ptBufferSend);
		else if(nDegree == 0xF1)
			Encode(szMiddlePowerCmd,5,ptBufferSend);
		else if(nDegree == 0xF2)
			Encode(szLowPowerCmd,5,ptBufferSend);
		else
			assert(0);//Not define
		SetSendData(ptBufferSend);
	}

	CBTTxPowerRequest::~CBTTxPowerRequest()
	{
	}

	bool CBTTxPowerRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CBTTxPowerRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE szResponseCmd[] = {0x4B, 0x03, 0x0E,0x00, 0x00};
		if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
			m_bResult = true;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	
	CBTTestModeRequest::CBTTestModeRequest(bool bEnetr,bool& bResult)
		:CDiagRequestBase(_TC("CBTTestModeRequest"))
		,m_bEnetr(bEnetr)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szEnterTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDD};
		/*
		000159: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be7b4 (Endpoint Address: 0x4)
		Send 0x8 bytes to the device:
		4B 03 0D 00 DD 63 9E 7E
		000160: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be794 (Endpoint Address: 0x84)
		Get 0x7 bytes from the device:
		4B 03 0D 00 60 75 7E
		*/
		BYTE szLeaveTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDE};

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
		if(ptBufferReceived->GetLength() >= 0)
			m_bResult = true;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	
	CBTBound::CBTBound(bool bBound,const char* szBTAddress,const char* szBTPass,bool& bIsCmdSuccess)
		:CWriteBTHeadsetAddress(szBTAddress,(int)strlen(szBTAddress),bIsCmdSuccess)		
	{
		/*
		Bond device	"SendRawRequest 75 3 3 0 42 169 22 83 00 214 0 0 0 8 0 0 0 48 48 48 48 0 0 0 0 0 0 0 0 0 0 0 0 4 0
		RequestItem ""Bluetooth Req SC"" 42 5445289 214 8 ""0000"" 4 0"
		Debond device	"SendRawRequest 75 3 3 0 43 169 22 83 00 214 0 0 0 8 0 0 0 48 48 48 48 0 0 0 0 0 0 0 0 0 0 0 0 4 0
		RequestItem ""Bluetooth Req SC"" 43 5445289 214 8 ""0000"" 4 0"

		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
		LOCAL_DEVICE_NAME	51	The local device name
		BD_ADDR_LAP	4	Bluetooth device address ¡V LAP part
		BD_ADDR_UAP	4	Bluetooth device address ¡V UAP part
		BD_ADDR_NAP	4	Bluetooth device address ¡V NAP part
		SECOND_TO_SEARCH	2	Second to search
		MAX_RESPONSE	1	Max number of devices to search
		CLASS_OF_DEVICE	4	Class of device
		DISCOVERABILITY	1	Setting of discoverability mode
		*/
		const char* c = szBTAddress;
		int n = (int)strlen(szBTAddress);
		BYTE sendBuf[ 5 + 12 + 16 + 2] = 
		{
			0x4b
			,0x03
			,0x03
			,0x00
			,42
			,HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	
			,0
			,0
		};

		if(bBound)
			sendBuf[4] = 42;//Bound
		else
			sendBuf[4] = 43;//Debound

		int nPassLen = (int)strlen(szBTPass);
		if(nPassLen <= 16)
		{
			sendBuf[33] = nPassLen;
			for(int nIndex = 0;nIndex < nPassLen;nIndex++)
			{
				sendBuf[17 + nIndex] = szBTPass[nIndex];
			}
		}

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 5 + 12 + 16 + 2, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTBound::~CBTBound()
	{
	}
	
	void CBTBound::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		*/		
		const char* data = ptBufferReceived->GetData();
		if(ptBufferReceived->GetLength() >= 4 && data[2] == 0x03 && data[3] == 0x00)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CBTRequest::CBTRequest(UINT nCommand,const char* szBTAddress,bool& bIsCmdSuccess)
		:CWriteBTHeadsetAddress(szBTAddress,szBTAddress ? (int)strlen(szBTAddress) : 0,bIsCmdSuccess)		
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0B 00 for HFG command
		BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
		BD_ADDR_LAP	4	Bluetooth device address ¡V LAP part
		BD_ADDR_UAP	4	Bluetooth device address ¡V UAP part
		BD_ADDR_NAP	4	Bluetooth device address ¡V NAP part
		CONNECTION_ID	1	Connection ID
		SPEAKER_GAIN	1	Speaker gain
		MIC_GAIN	1	Microphone gain
		LOOP	1	Loop
		SUPPORTED_FEATURE	2	Supported features

		SendRawRequest 75 3 11 0 180 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 37 0
		RequestItem "Bluetooth Req HFG" 180 0 0 0 0 0 0 0 37
		SendRawRequest 75 3 11 0 181 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		RequestItem "Bluetooth Req HFG" 181 0 0 0 0 0 0 0 0
		*/
		const char* c = szBTAddress;
		int n = szBTAddress ? (int)strlen(szBTAddress) :  0;		
		BYTE sendBuf[ 5 + 12 + 6] = 
		{
			0x4b
			,0x03
			,0x0B
			,0x00
			,nCommand
			,n == 0 ? 0 : HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,n == 0 ? 0 : HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,n == 0 ? 0 : HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,n == 0 ? 0 : HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,n == 0 ? 0 : HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,n == 0 ? 0 : HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
			,0
			,0
			,0
			,0
		};

		if(nCommand == 180)
		{
			sendBuf[21] = 37;
		}
		else if(nCommand == 181)
		{
			sendBuf[21] = 0;
		}
		else if(nCommand == 190)
		{
			sendBuf[20] = (szBTAddress == NULL) ? 1 : 0;
		}

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 5 + 12 + 6, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTRequest::~CBTRequest()
	{
	}
	
	void CBTRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		*/		
		const char* data = ptBufferReceived->GetData();
		if(ptBufferReceived->GetLength() >= 4 && data[0] == 0x4B && data[1] == 0x03 && data[2] == 0x0B)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	// This class is used for write BT address to mobile
	//////////////////////////////////////////////////////////////////////////
	CWriteBTHeadsetAddress::CWriteBTHeadsetAddress(const char* BTAddress, int length,bool& bIsCmdSuccess) 
		:CDiagRequestBase(_T("CWriteBTHeadsetAddress"))
		,m_pBTAddress(NULL)
		,m_nBTAddressLength(length)
		,m_bIsCmdSuccess(bIsCmdSuccess)
	{
		m_pBTAddress = new char[length];
		memset(m_pBTAddress, 0, sizeof(char)*length);
		memcpy(m_pBTAddress, BTAddress, sizeof(char)*length);

		char* c = m_pBTAddress;
		int n = m_pBTAddress ? (int)strlen(m_pBTAddress) :  0;	
		BYTE sendBuf[17] = 
		{
			0x4b
			,0x03
			,0x0f
			,0x00
			,0xfa
			,n == 0 ? 0 : HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,n == 0 ? 0 : HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,n == 0 ? 0 : HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,n == 0 ? 0 : HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,n == 0 ? 0 : HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,n == 0 ? 0 : HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
		};

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 17, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteBTHeadsetAddress::~CWriteBTHeadsetAddress() 
	{
		if(m_pBTAddress != NULL)
			delete [] m_pBTAddress;
		m_pBTAddress = NULL;
	}
	void CWriteBTHeadsetAddress::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		const char* data = ptBufferReceived->GetData();
		if(data[4] == 1)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CWriteBTHeadsetAddress::CheckCmdResult() 
	{
		return m_bIsCmdSuccess;
	}

	int CWriteBTHeadsetAddress::HexToInt(char c) 
	{
		if(c >= '0' && c <= '9') return c - '0';
		if(c >= 'A' && c <= 'F') return c - 'A' + 10;
		if(c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0;
	}
	/*
	WLAN test mode
	*/
	CWLANDownLoadTFirmRequest::CWLANDownLoadTFirmRequest(bool& bResult)
		:CDiagRequestBase(_TC("CWLANDownloadTFirmRequest"))
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szDownloadTFirm[] = {0x4B, 41, 0, 0};

		Encode(szDownloadTFirm,4,ptBufferSend);
		
		
		SetSendData(ptBufferSend);
	}
	
	CWLANDownLoadTFirmRequest::~CWLANDownLoadTFirmRequest()
	{

	}
	bool CWLANDownLoadTFirmRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CWLANDownLoadTFirmRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
			//if(ptBufferReceived->GetLength() >= 0)
			m_bResult = true;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWLANPowerDownRequest::CWLANPowerDownRequest(bool& bResult)
		:CDiagRequestBase(_TC("CWLANPowerDownRequest"))
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szPowerDown[] = {0x4B, 41, 3, 0};

		Encode(szPowerDown,4,ptBufferSend);
		
		
		SetSendData(ptBufferSend);
	}
	
	CWLANPowerDownRequest::~CWLANPowerDownRequest()
	{

	}
	bool CWLANPowerDownRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CWLANPowerDownRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
			if(ptBufferReceived->GetLength() >= 0)
			m_bResult = true;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWLANStartPowerRequest::CWLANStartPowerRequest(bool bTX,int nPower,int nChannel,int iReserved,int iReserved2,bool& bResult)
		:CDiagRequestBase(_TC("CWLANPowerDownRequest"))
		,m_bResult(bResult)
		,m_iPower(nPower)
		,m_nChannel(nChannel)
		,m_iReserved(iReserved)
		,m_iReserved2(iReserved2)
		,m_bTX(bTX)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szStartPower[6]={0x4B, 41, 1, 0, 1,13};
		szStartPower[4]=nChannel;   //Channel
		szStartPower[5]=nPower;      //Output power   Unit: dBm
		BYTE szStopPower[4] = {0x4B, 41,2,0};

		if(bTX)
			Encode(szStartPower,6,ptBufferSend);
		else 
			Encode(szStopPower,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}	
	
	CWLANStartPowerRequest::~CWLANStartPowerRequest()
	{

	}
	bool CWLANStartPowerRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CWLANStartPowerRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
			if(ptBufferReceived->GetLength() >= 0)
			m_bResult = true;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/*
	FM test mode
	*/

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

	CBTQueryTestResultRequest::CBTQueryTestResultRequest(int& nResult,bool& bSuccess)
		:CDiagRequestBase(_TC("CBTQueryTestResultRequest"))
		,m_nResult(nResult)
		,m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			BYTE result;
		} diag_BT_req;
#pragma pack(pop)

		diag_BT_req req = {75,3,18,253};

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&req,10,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTQueryTestResultRequest::~CBTQueryTestResultRequest()
	{
	}

	bool CBTQueryTestResultRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	
	void CBTQueryTestResultRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			BYTE result;
		} diag_BT_resp;
#pragma pack(pop)

		diag_BT_resp resp = {0};
		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if(resp.command_code == 0x4B)
		{
			m_nResult = resp.result;
			m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}




//

}
