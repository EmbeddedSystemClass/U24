#include "stdafx.h"
#include "WIFIRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{
	CWolf_WiFiModuleSwitchRequest::CWolf_WiFiModuleSwitchRequest(bool bOn, bool& bResult)
		: CDiagRequestBase(_T("CWolf_WiFiModuleSwitchRequest"))
		, m_bOn(bOn)
		, m_bResult(bResult)
	{
		m_bResult = false;

		BYTE req[6] = {75, 250, 49, 0, 1, 0};

		if (bOn)
			req[5] = 1;
		else
			req[5] = 0;

		CBufferPtr ptBufferSend;
		Encode(req, 6, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolf_WiFiModuleSwitchRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE resp[6] = {75, 250, 49, 0, 0, 0};

		Safememcopy(&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (resp[5] == 1)
		{
			if (m_bOn)
			{
				if (resp[4] == 1)
					m_bResult = true;
			}
			else
			{
				if (resp[4] == 0)
					m_bResult = true;
			}
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolf_WiFiModuleCheckRequest::CWolf_WiFiModuleCheckRequest(bool& bOn, bool& bResult)
		: CDiagRequestBase(_T("CWolf_WiFiModuleCheckRequest"))
		, m_bOn(bOn)
		, m_bResult(bResult)
	{
		m_bResult = false;

		//Get WiFi PM State:
		//sendrawrequest 75 250 49 0 0 0
		//Return: 75 250 49 0 [State] [Result]  // State: 0: off, 1: on
												// Result: 0: Failed, 1: OK

		BYTE req[6] = {75, 250, 49, 0, 0, 0};

		if (bOn)
			req[5] = 1;
		else
			req[5] = 0;

		CBufferPtr ptBufferSend;
		Encode(req, 6, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolf_WiFiModuleCheckRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE resp[6] = {75, 250, 49, 0, 0, 0};

		Safememcopy(&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bResult = resp[5] != 0;
		m_bOn = resp[4] == 1;
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

		// Send: 75 250 43 0 11 6 16 (75 250 43 0 rate - channel - power) -------------- 75 250 43 0 0 0 0 to turn off
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


	CLeopardTurnOnTXPower2ChNRequest::CLeopardTurnOnTXPower2ChNRequest (BYTE Rate, BYTE nChannel, BYTE Power, bool &bResult)
		: CDiagRequestBase (_T("CLeopardTurnOnTXPower2ChNRequest"))
		, m_bResult (bResult)
	{
		m_bResult = false;

		// Send: 75 250 47 0 [index] [channel] [power] ------- 75 250 47 0 0 0 0 to turn off
		// Resp: 75 250 47 0 0/1

		BYTE req[7] = {75, 250, 47, 0, Rate, nChannel, Power};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardTurnOnTXPower2ChNRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[5] = {75, 250, 47, 0, 0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bResult = (1 == resp[4]);
		//m_bResult = true;
		assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CLeopardTurnOnRXPower2Request::CLeopardTurnOnRXPower2Request (BYTE Rate, BYTE nChannel, int &data1, int &data2, int &data3)
		: CDiagRequestBase (_T("CLeopardTurnOnRXPower2Request"))
		, m_data1 (data1)
		, m_data2 (data2)
		, m_data3 (data3)
	{
		// Send: 75 250 44 0 [rate] [channel] -------------- 75 250 44 0 0 0 0 to turn off
		// Resp: 75 250 44 0 data1 data2 data3 --> data is a WORD

		BYTE req[7] = {75, 250, 44, 0, Rate, nChannel};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardTurnOnRXPower2Request::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;
			BYTE sybsys_id;
			short Mode_ID;
			WORD data1;
			WORD data2;
			WORD data3;
		} diag_RX_resp;
		#pragma pack (pop)

		diag_RX_resp resp;

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_data1 = resp.data1;
		m_data2 = resp.data2;
		m_data3 = resp.data3;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CLeopardTurnOnRXPower2ChNRequest::CLeopardTurnOnRXPower2ChNRequest (BYTE Rate, BYTE nChannel, int &data1, int &data2, int &data3)
		: CDiagRequestBase (_T("CLeopardTurnOnRXPower2ChNRequest"))
		, m_data1 (data1)
		, m_data2 (data2)
		, m_data3 (data3)
	{
		// Send: 75 250 48 0 [index] [channel] ------- 75 250 48 0 0 0 0 to turn off
		// Resp: 75 250 48 0 [x x] [x x] [x x] --- data1 data2 data3

		BYTE req[7] = {75, 250, 48, 0, Rate, nChannel};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardTurnOnRXPower2ChNRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;
			BYTE sybsys_id;
			short Mode_ID;
			WORD data1;
			WORD data2;
			WORD data3;
		} diag_RX_resp;
		#pragma pack (pop)

		diag_RX_resp resp;

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_data1 = resp.data1;
		m_data2 = resp.data2;
		m_data3 = resp.data3;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}















	CWolfReadMACRequest::CWolfReadMACRequest (char* address, bool& bResult)
		: CDiagRequestBase (_T("CWolfReadMACRequest"))
		, m_bResult (bResult)
		, m_MACaddress (address)
	{
		m_bResult = false;
		memset (m_MACaddress, 0, 12);

		// sendrawrequest 75 250 40 0
		// resp: 0x4B FA 28 00 25 25 [30 30 39 30 34 43 43 35 31 32 33 38] 00 00 00 00 01 00

		BYTE req[] = {75, 250, 40, 0};

		CBufferPtr ptBufferSend;
		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolfReadMACRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[24] = {0};

		Safememcopy (resp, sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		memcpy (m_MACaddress, &resp[6], 12);
		m_bResult = resp[22]==1;
		//assert (m_bResult);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}













}