#include "stdafx.h"
#include "PWPRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{
	CIniPWPRequest::CIniPWPRequest( bool &bResult , BYTE funcIndex)
		: CDiagRequestBase(_TC("CIniPWPRequest"))
		, m_bResult(bResult)
	{		
		//sendrawrequest 75 250 127 00 00 00(Initialize PWP operation)
		//								  01(Close PWP operation)
		//								  02(Verify PWP operation)

		BYTE req[] = {75, 250, 127, 00, funcIndex, 00} ;

		CBufferPtr ptBufferSend ;
		Encode(req, sizeof(req),ptBufferSend) ;
		SetSendData(ptBufferSend) ;

		m_bResult = false ;
	}

	CIniPWPRequest::~CIniPWPRequest()
	{
	}

	bool CIniPWPRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CIniPWPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		// response: 4B FA 7F 00 00 00(command fail)
		//							01(Requested completed)

		BYTE resp[6];
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength()) ;		
		m_bResult = (resp[5] == 0x01) ;

		//assert (m_bResult);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived) ;
	}




	CWolfWriteDevNumRequest::CWolfWriteDevNumRequest(bool &bResult, BYTE* DevNum)
		:CDiagRequestBase(_TC("CWolfWritePWPRequest"))
		,m_bResult(bResult)
	{
		//BYTE req[136] = {75, 250, 129, 0, 8, 11, 15, 1, 2, 3, 4, 5, 6, 7, 8, 9,  10, 11, 12, 13, 14, 15} ;
		BYTE req[136] = {75, 250, 129, 0, 8, 11} ;

		/*
		for(int j = 0; j < 16; j++)
		{
			if(szDevNum[j] >= '0' && szDevNum[j] <= '9') bufHex[j] = szDevNum[j] - '0' ;
			if(szDevNum[j] >= 'A' && szDevNum[j] <= 'F') bufHex[j] = szDevNum[j] - 'A' + 10 ;
			if(szDevNum[j] >= 'a' && szDevNum[j] <= 'f') bufHex[j] = szDevNum[j] - 'a' + 10 ;
		}
		*/
		memcpy(&req[6], DevNum, 16) ;

		CBufferPtr ptBufferSend ;
		Encode(req, sizeof(req),ptBufferSend) ;
		SetSendData(ptBufferSend) ;

		m_bResult = false ;
	}

	CWolfWriteDevNumRequest::~CWolfWriteDevNumRequest()
	{}

	bool CWolfWriteDevNumRequest::IsCompletePacket(CBufferPtr &ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived) ;
	}

	void CWolfWriteDevNumRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[136] = {0} ;
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength()) ;		
		m_bResult = (resp[134] == 0x01) ;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived) ;
	}




	CWolfReadDevNumRequest::CWolfReadDevNumRequest(char *deviceNum, bool &bResult)
		:CDiagRequestBase(_TC("CWolfReadDevNumRequest"))
		,m_bResult(bResult)
		,m_DeviceNumber(deviceNum)
	{
		memset (m_DeviceNumber, 0, 32) ;

		// sendrawrequest 75 250 128 0 8 11
		// resp: 0x4B FA 80 00 08 0B [00 01 02 03 04 05 06 07 08 09 10 0A 0B 0C 0E 0F] 00 00 00 00 00 00 00 00
		//         00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
		//         00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
		//         00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
		//         01 00

		BYTE req[136] = {75, 250, 128, 0, 8, 11} ;

		CBufferPtr ptBufferSend ;
		Encode(req, sizeof(req), ptBufferSend) ;
		SetSendData(ptBufferSend) ;

		m_bResult = false ;
	}

	CWolfReadDevNumRequest::~CWolfReadDevNumRequest()
	{}

	bool CWolfReadDevNumRequest::IsCompletePacket(CBufferPtr &ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived) ;
	}

	void CWolfReadDevNumRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[136] ={0};
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength()) ;

		/*
		int i, j, k ;
		byte b ;
		for (i = 0, j = 0; i < 16; ++i) 
		{       
			b = resp[i+6] ;       
			k = b & 15 ;       
			m_DeviceNumber[j++] = (char)(k > 9 ? k + 0x37 : k + 0x30) ;  
		}
		*/
		int j = 0;
		for(int i = 0;i < 16;i++)
		{
			CStringA cstrByte;
			cstrByte.Format("%2x", resp[i + 6]);
			cstrByte.Replace(' ', '0');
			cstrByte.MakeUpper();
			m_DeviceNumber[j++] = cstrByte.GetAt(0);
			m_DeviceNumber[j++] = cstrByte.GetAt(1);
		}

		m_bResult = (resp[134] == 0x01) ;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived) ;
	}


	CWolfWritePublicKeyRequest::CWolfWritePublicKeyRequest( bool &bResult, BYTE* PublicKey)
		:CDiagRequestBase(_TC("CWolfWritePublicKeyRequest"))
		,m_bResult(bResult)
	{
		BYTE req[136] = {75, 250, 129, 0, 81, 195} ;
		memcpy(req+6, PublicKey, 128) ;

		CBufferPtr ptBufferSend ;
		Encode(req, sizeof(req), ptBufferSend) ;
		SetSendData(ptBufferSend) ;

		m_bResult = false ;
	}

	CWolfWritePublicKeyRequest::~CWolfWritePublicKeyRequest()
	{}

	bool CWolfWritePublicKeyRequest::IsCompletePacket(CBufferPtr &ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived) ;
	}

	void CWolfWritePublicKeyRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[136] = {0};
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength()) ;		
		m_bResult = (resp[134] == 0x01) ;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived) ;
	}

	CWolfReadPublicKeyRequest::CWolfReadPublicKeyRequest(char *publicKey, bool &bResult)
		:CDiagRequestBase(_TC("CWolfReadPublicKeyRequest"))
		,m_bResult(bResult)
		,m_PublicKey(publicKey)
	{
		memset(m_PublicKey, 0, 128) ;
		BYTE req[136] = {75, 250, 128, 0, 81, 195} ;

		CBufferPtr ptBufferSend ;
		Encode(req, sizeof(req), ptBufferSend) ;
		SetSendData(ptBufferSend);

		m_bResult = false ;
	}

	CWolfReadPublicKeyRequest::~CWolfReadPublicKeyRequest()
	{}

	bool CWolfReadPublicKeyRequest::IsCompletePacket(CBufferPtr &ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CWolfReadPublicKeyRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[136] = {0} ;
		Safememcopy(resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength()) ;

		int i, j, k;
		byte b;
		for (i = 0, j = 0; i < 128; ++i) 
		{       
			b = resp[i+6]; 
			k = b >> 4;       
			m_PublicKey[j++] = (char)(k > 9 ? k + 0x37 : k + 0x30) ;
			k = b & 15;       
			m_PublicKey[j++] = (char)(k > 9 ? k + 0x37 : k + 0x30) ;  
		}

		m_bResult = (resp[134] == 0x01) ;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived) ;
	}


}