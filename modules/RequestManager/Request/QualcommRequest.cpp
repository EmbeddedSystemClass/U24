#include "StdAfx.h"
#include "QualcommRequest.h"
#include "..\codec\BQPacketLib.h"
#include "..\codec\QualcommCRCChecker.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\Common\CommonUtil/LogPolicy.h"

#define ESCAPE_CHAR 0x7D
#define END_CHAR 0x7E
#define COMPLEMENT_CHAR 0x20
#define START_CHAR 0x7E

namespace PLATFORM_TOOL
{	
	CDiagRequestBase::CDiagRequestBase(const TSTRING& strName,int nTimeout,bool bHadReturnvalue)
		:CRequestBase(strName,DIAG,nTimeout,bHadReturnvalue)
		,m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDiagRequestBase::CDiagRequestBase(int nTimeout,bool bHadReturnvalue)
		:CRequestBase(DIAG,nTimeout,bHadReturnvalue)
		,m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDiagRequestBase::~CDiagRequestBase()
	{
	}

	void CDiagRequestBase::Safememcopy(void* pBuffer, int nBufferSize,void* pData,int nDataSize)
	{
		int nRealTransferSize = 0;
		if(nBufferSize > nDataSize)
			nRealTransferSize = nDataSize;
		else
			nRealTransferSize = nBufferSize;
		memcpy(pBuffer,pData,nRealTransferSize);

		if(nBufferSize != nDataSize && nDataSize != 0)
		{
			CBufferPtr ptSendData; 
			GetSendData(ptSendData);

			BYTE btReadSWCmd[4] = {0x7c, 0x93, 0x49, 0x7e};
			if (GetName() == _T("CNVRequest")
				&& ptSendData 
				&& ptSendData->GetData()
				&& ptSendData->GetData()[1] == 0x55 // Read SPC
				&& nDataSize == 17)
			{
				//¯S®íª¬ªp, ¤£¼glog
			}
			else if (GetName() == _T("CReadSWVersionRequest")
				&& ptSendData 
				&& ptSendData->GetData()
				&& 0 == ::memcmp(ptSendData->GetData(), btReadSWCmd, 4) // Get SW version
				&& nDataSize == 65)
			{
				//¯S®íª¬ªp, ¤£¼glog
			}
			else
			{
				PostUSBLog(ptSendData, pData, nBufferSize, nDataSize, _T("BufferSizeCheck.txt"));
			}
		}
	}

	bool TransByteToChar(BYTE *pucHexBcdBuf, int iHexBcdBufSize, CStringA& strBuffer)
	{
		bool bRes = false;
		if (pucHexBcdBuf && (iHexBcdBufSize > 0))
		{
			for(int i=0; i< iHexBcdBufSize; i++)
			{
				CStringA strTemp;
				strTemp.Format("%02X ",pucHexBcdBuf[i]);
				strBuffer+=strTemp;
				if(i!=0 && (i+1)%18 ==0 )
					strBuffer += "\r\n";
			}
			bRes = true;
		}
		return bRes;
	}

	void CDiagRequestBase::PostUSBLog(CBufferPtr ptBufferSend, void* ptBufferReceived, int nExpectiveRecSize, int nGottenRecSize, TSTRING strFileName) //Post sent data and receive data.
	{
		//const TSTRING& GetName() const {return m_strName;}
		if (GetName().length() == 0)
			LogPolicy::Log(_T("Not named request"), strFileName.c_str());
		else
			LogPolicy::Log(GetName().c_str(), strFileName.c_str());

		CStringA cstraSendData;
		TransByteToChar((BYTE*)ptBufferSend->GetData(), ptBufferSend->GetLength(), cstraSendData);
		cstraSendData = "Send data:\r\n" + cstraSendData;

		CStringA cstraReceiveDataTitle;
		cstraReceiveDataTitle.Format("Receive data expect size: %d, gotten size: %d\r\n", nExpectiveRecSize, nGottenRecSize);
		CStringA cstraReceiveData;
		TransByteToChar((BYTE*)ptBufferReceived, nGottenRecSize, cstraReceiveData);
		cstraReceiveData = cstraReceiveDataTitle + cstraReceiveData;

		CStringA straMsg = cstraSendData + "\r\n" + cstraReceiveData + "\r\n" ;
		LogPolicy::Log(straMsg, strFileName.c_str());

	}

	bool CDiagRequestBase::Encode(const BYTE* szCommand,int nCommandLen,CBufferPtr& ptBufferSend)
	{
		bool bRes = false;

		//Prepare diag command
		char chStart = PACKET_NULL_CHAR;
		char chEnd = END_CHAR;	   //0x7E;
		void *pBuffer = NULL;
		int nPackedLen = CBQPacketLib::PackBuffer( szCommand, nCommandLen, chStart, chEnd, &pBuffer);
		ptBufferSend = new CBuffer(nPackedLen,(const char*)pBuffer);		
		delete[] pBuffer;

		bRes = nPackedLen > 0;
		assert(bRes);
		return bRes;
	}

	bool CDiagRequestBase::IsCompletePacket(CBufferPtr& ptBufferReceived1)
	{		
		m_ptTempBufferReceived->Append(ptBufferReceived1);

		bool bRes = false;
		char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());
		int nTotalLen = m_ptTempBufferReceived->GetLength();		
		int nszBufferIndex = 0;
		bool bFind7E = false;
		do
		{
			bFind7E = false;
			char *szNewBuffer = szBuffer + nszBufferIndex;
			for(; nszBufferIndex < nTotalLen ; nszBufferIndex++)
			{
				if(szBuffer[nszBufferIndex] == 0x7E)
				{
					bFind7E = true;
					break;
				}
			}
			nszBufferIndex+=1;

			if(bFind7E)
			{
				if(szNewBuffer[0] == 0x60)
				{//It's Event Message, ingore it
				}
				else if(szNewBuffer[0] == 0x15)
				{//It's Hello Message, ingore it
				}
				else if(szNewBuffer[0] == 0x10)
				{//It's SW log, ignore it.
				}
				else if (szNewBuffer[0] == 0x79)
				{
					break;  //debug log, ingore it, sunlight
				}
				else if(szNewBuffer[0] == 0x13)// && szNewBuffer[1] == 0x61)
				{//It's Hello Message, ingore it
					/*
					00015: Bulk or Interrupt Transfer (UP), 12.03.2010 16:05:30.4750964 +0.0
					Pipe Handle: 0x8a67d0dc (Endpoint Address: 0x84)
					Get 0x5 bytes from the device:
					15 06 58 81 7E
					000016: Bulk or Interrupt Transfer (UP), 12.03.2010 16:05:30.4907214 +0.0156250
					Pipe Handle: 0x8a67d0dc (Endpoint Address: 0x84)
					Get 0x7 bytes from the device:
					13 61 74 0D 62 09 7E
					*/
				}
				else
				{
					QualcommCRCChecker CRCChecker(false);
					for ( DWORD j = 0 ; ((szNewBuffer - szBuffer) + j) <= m_ptTempBufferReceived->GetLength(); j ++)
					{
						int nRxLen = CRCChecker.Push(szNewBuffer[j]);
						if ( nRxLen > 0)
						{
							bRes = true;
							ptBufferReceived1 = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
							m_ptTempBufferReceived->PopFront(j + 1);
							break;
						}
					};
					break;
				}
			}
		}
		while(bFind7E);

		return bRes;
	}

	CEventRequest::CEventRequest(bool bEnable)
	{
		BYTE szBuf[2]={0x60,0x00};
		if(bEnable)
			szBuf[1] = 0x01;
		else
			szBuf[1] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szBuf,2,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEventRequest::~CEventRequest()
	{
	}

	bool CEventRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		char *szBuffer = const_cast<char *>(ptBufferReceived->GetData());	
		QualcommCRCChecker CRCChecker(false);
		for ( DWORD j = 0 ; j < ptBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				bRes = true;
				ptBufferReceived = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
				break;
			}
		}
		return bRes;
	}

	CAskDLDIAGModeRequest::CAskDLDIAGModeRequest(bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode)
		:CRequestBase(NONE,10000,true)
		,m_bIsDiagMode(bIsDiagMode)
		,m_bIsDLMode(bIsDLMode)
		,m_bIsATMode(bIsATMode)
		,ptBufferReceived(new CBuffer)
	{
		//3.4.96 Obsolete (CMD_CODE 97 / 0x61)
		//This is no longer supported.
		/*
		000029: Bulk or Interrupt Transfer (UP), 08.05.2008 09:41:09.0789610 +2.5463534
		Pipe Handle: 0x89e781e4 (Endpoint Address: 0x4)
		Send 0xa bytes to the device:
		61 74 0D 1C D5 7E 06 4E 95 7E
		000030: Bulk or Interrupt Transfer (UP), 08.05.2008 09:41:09.0789610 +0.0
		Pipe Handle: 0x89e781c4 (Endpoint Address: 0x84)
		Get 0xc bytes from the device:
		13 61 74 0D 62 09 7E 15 06 58 81 7E
		*/
		//unsigned char szBuf[10]={0x61,0x74,0x0D,0x1C,0xD5,0x7E,0x06,0x4E,0x95,0x7E};
		//unsigned char szBuf[6]={0x61,0x74,0x0D,0x1C,0xD5,0x7E};
		//0x7E, 0x06, 0x4E, 0x95, 0x7E	-->	DMSS NOOP command
		//0x4B, 0x0F, 0x00, 0x00, 0xBB, 0x60, 0x7E	-->	ask mode command
		unsigned char szBuf[]={0x4B, 0x0F, 0x00, 0x00, 0xBB, 0x60, 0x7E, 0x7E, 0x06, 0x4E, 0x95, 0x7E};
		CBufferPtr ptBufferSend = new CBuffer(sizeof(szBuf),(const char*)szBuf);		
		SetTimeout (150);
		SetSendData(ptBufferSend);
	}

	CAskDLDIAGModeRequest::~CAskDLDIAGModeRequest()
	{
	}

	void CAskDLDIAGModeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		m_bIsDiagMode = false;
		m_bIsDLMode = false;
		m_bIsATMode = false;
		/*
		#define	DETECT_DIAG_RSP_6250		("\x13\x61\x74\x0D")
		#define	DETECT_DIAG_RSP				("\x15\x06\x58\x81\x7e")		
		#define	DETECT_DL					("\x7E\x02\x6A\xD3\x7E")
		#define DETECT_DL2					("\x7E\x03\x00\x06\x9E\x4C\x7E")
		//SecureOSBL 20110818 Dave.Wu
		#define DETECT_DL3					("\x7E\x0E\x49\x6E\x76\x61\x6C\x69\x64\x20\x43\x6F\x6D\x6D\x61\x6E\x64\x0A\x58\xBD\x7E")
		#define DETECT_DL4					("\x7E\x0D\x05\x00\x00\x00\x49\x6E\x76\x61\x6C\x69\x64\x20\x43\x6F\x6D\x6D\x61\x6E\x64\xBF\xC3\x7E")
		//SecureOSBL 20110818 Dave.Wu
		#define DETECT_AT					("\x0D\x0A\x4F\x4B\x0D\x0A")
		const unsigned char szModemReplyBuf[9]={0x61 ,0x74 ,0x0d ,0x0d ,0x0a ,0x4f ,0x4b ,0x0d ,0x0a};
		*/
		#define	DETECT_DL					("\x7E\x02\x6A\xD3\x7E")			//DMSS ACK
		#define	DETECT_DIAG_RSP				("\x4B\x0F\x00\x00\x00\x00\x00\x00")//First 8 bytes of mode req. response

		unsigned char szbu[512]={0};
		memcpy((void*)szbu, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		if(::memcmp(DETECT_DL, ptBufferReceived->GetData(), 5) == 0)
			m_bIsDLMode = true;
		else if(::memcmp(DETECT_DIAG_RSP, ptBufferReceived->GetData(), 8) == 0)
			m_bIsDiagMode = true;
		else assert(0);
		/*
		if(::memcmp(DETECT_DIAG_RSP_6250,ptBufferReceived->GetData(),4) == 0)
			m_bIsDiagMode = true;
		else if(::memcmp(DETECT_DIAG_RSP,ptBufferReceived->GetData(),5) == 0)
			m_bIsDiagMode = true;
		else if(::memcmp(DETECT_DL,ptBufferReceived->GetData(),5) == 0)
			m_bIsDLMode = true;
		else if(::memcmp(DETECT_DL2, ptBufferReceived->GetData(),7) == 0)
			m_bIsDLMode = true;
		//SecureOSBL 20110818 Dave.Wu
		else if(::memcmp(DETECT_DL3, ptBufferReceived->GetData(),21) == 0)
			m_bIsDLMode = true;
		else if(::memcmp(DETECT_DL4, ptBufferReceived->GetData(),24) == 0)
			m_bIsDLMode = true;
		//SecureOSBL 20110818 Dave.Wu
		else if(::memcmp(DETECT_AT, ptBufferReceived->GetData(),6) == 0)
			m_bIsATMode = true;
		else if(::memcmp(szModemReplyBuf, ptBufferReceived->GetData(),9) == 0)
			m_bIsATMode = true;
		else assert(0);
		*/
		CRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CAskDLDIAGModeRequest::IsCompletePacket(CBufferPtr& ptTempBufferReceived)
	{		
		const unsigned char szModemReplyBuf[9]={0x61 ,0x74 ,0x0d ,0x0d ,0x0a ,0x4f ,0x4b ,0x0d ,0x0a};
		
		ptBufferReceived->Append(ptTempBufferReceived);
		bool bRes = false;
		char *szBuffer = const_cast<char *>(ptBufferReceived->GetData());		
		if(ptBufferReceived->GetLength() > 0 && szBuffer[0] == 0x60)
		{//It's Event Message, ingore it
			bRes  = false;
		}
		else if(ptBufferReceived->GetLength() > 2 && szBuffer[0] == 0x15 && szBuffer[0] == 0x06 && szBuffer[0] == 0x58)
		{
			bRes  = false;
		}
		else if(ptBufferReceived->GetLength() >= 6 && szBuffer[0] == 0x0D && szBuffer[1] == 0x0A)
		{//AT command
			bRes  = true;
		}
		else if(ptBufferReceived->GetLength() == 9 && memcmp(szBuffer,szModemReplyBuf,9) == 0)
		{//AT command
			bRes  = true;
		}
		else
		{
			QualcommCRCChecker CRCChecker(szBuffer[0] == 0x7E);
			for ( DWORD j = 0 ; j <= ptBufferReceived->GetLength(); j ++)
			{
				int nRxLen = CRCChecker.Push(szBuffer[j]);
				if ( nRxLen > 0)
				{
					ptTempBufferReceived = ptBufferReceived;
					bRes = true;
					break;
				}
			};
		}
		return bRes;
	}

	CDS2DIAGRequest::CDS2DIAGRequest():CRequestBase(NONE)
	{
		char szBuf[10]={"at$qcdmg\r"};

		CBufferPtr ptBufferSend = new CBuffer(9,szBuf);		
		SetSendData(ptBufferSend);
	}

	CDS2DIAGRequest::~CDS2DIAGRequest()
	{
	}

	bool CDS2DIAGRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		assert(0);//Need to check
		return false;
	}
	
	CDIAG2DL6280Request::CDIAG2DL6280Request():CRequestBase(NONE)
	{
		unsigned char szBuf[4]= {0x3A,0xA1,0x6E,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(4,(const char*)szBuf);		
		SetSendData(ptBufferSend);
	}

	CDIAG2DL6280Request::~CDIAG2DL6280Request()
	{
	}

	bool CDIAG2DL6280Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		unsigned char szBuf[4]= {0x3A,0xA1,0x6E,0x7E};
		int nRes = ::memcmp(ptBufferReceived->GetData(),szBuf,4);
		bRes = nRes == 0;
		return bRes;
	}

	////////////////////////////////////////////////////////////////////////////////////	
		
	CCloseLog1Request::CCloseLog1Request():CRequestBase(NONE)
	{
		//InternalWrite("\x1f\xff\x00\x5e\xf6\x7e", sizeof("\x1f\xff\x00\x5e\xf6\x7e")-1);
		
		unsigned char szBuf[6]= {0x1F,0xFF,0x00,0x5E,0xF6,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(6,(const char*)szBuf);		
		SetSendData(ptBufferSend);
	}

	CCloseLog1Request::~CCloseLog1Request()
	{
	}

	bool CCloseLog1Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}	

	CCloseLog2Request::CCloseLog2Request():CRequestBase(NONE)
	{
		//InternalWrite( "\x60\x00\x12\x6A\x7E", sizeof("\x60\x00\x12\x6A\x7E")-1);
		char szBuf[5]= {0x60,0x00,0x12,0x6A,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(5,szBuf);		
		SetSendData(ptBufferSend);
	}

	CCloseLog2Request::~CCloseLog2Request()
	{
	}

	bool CCloseLog2Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}	

	CCloseLog3Request::CCloseLog3Request():CRequestBase(NONE)
	{
		//InternalWrite("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e", sizeof("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e")-1);
		unsigned char szBuf[11]= {0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDA,0x81,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(11,(const char*)szBuf);
		SetSendData(ptBufferSend);
	}

	CCloseLog3Request::~CCloseLog3Request()
	{
	}

	bool CCloseLog3Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}
	////////////////////////////////////////////////////////////////////////////////////

	CUnknown1Request::CUnknown1Request():CRequestBase(NONE)
	{
		unsigned char szBuf[10]= {0x41,0x30,0x30,0x30,0x30,0x30,0x30,0xDF,0x8A,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(10,(const char*)szBuf);		
		SetSendData(ptBufferSend);
	}

	CUnknown1Request::~CUnknown1Request()
	{
	}

	bool CUnknown1Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}	

	CUnknown2Request::CUnknown2Request():CRequestBase(NONE)
	{
		
		char szBuf[6]= {0x29,0x01,0x00,0x31,0x40,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(6,szBuf);		
		SetSendData(ptBufferSend);
	}

	CUnknown2Request::~CUnknown2Request()
	{
	}

	bool CUnknown2Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}	

	CUnknown3Request::CUnknown3Request():CRequestBase(NONE,INFINITE)
	{
		unsigned char szBuf[4]= {0x0C,0x14,0x3A,0x7E};

		CBufferPtr ptBufferSend = new CBuffer(4,(const char*)szBuf);
		SetSendData(ptBufferSend);
	}

	CUnknown3Request::~CUnknown3Request()
	{
	}

	bool CUnknown3Request::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}
	////////////////////////////////////////////////////////////////////////////////////
	CDiagAskStateRequest::CDiagAskStateRequest(bool& bCanBeChanged2DL)
		:CDiagRequestBase(1000, true), m_bCanBeChanged2DL(bCanBeChanged2DL)
	{
		m_bCanBeChanged2DL = false;
		DIAG_SUBSYS_WCDMA_STATUS_F_req_type req = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_WCDMA;
		req.subsys_cmd_code = 0x0E;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((BYTE*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDiagAskStateRequest::~CDiagAskStateRequest()
	{
	}

	bool CDiagAskStateRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{ 
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CDiagAskStateRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		DIAG_SUBSYS_WCDMA_STATUS_F_rsp_type resp = {0};
		if(ptBufferReceived->GetLength() > sizeof(resp))
			memcpy(&resp, ptBufferReceived->GetData(),sizeof(resp));
		else
			memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		
		if(resp.cmd_code == DIAG_SUBSYS_CMD_F && resp.subsys_id == DIAG_SUBSYS_WCDMA)
		{
			m_bCanBeChanged2DL = resp.tmc_state == 0;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	////////////////////////////////////////////////////////////////////////////////////
	CDIAGGetSWVersionRequest::CDIAGGetSWVersionRequest()
		:CDiagRequestBase(_T("CDIAGGetSWVersionRequest"))
	{
		BYTE szBuf[2] = {0x7C, 0x0};

		CBufferPtr ptBufferSend;
		Encode(szBuf, 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDIAGGetSWVersionRequest::~CDIAGGetSWVersionRequest()
	{
	}

    bool CDIAGGetSWVersionRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
    {
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

	void CDIAGGetSWVersionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
}