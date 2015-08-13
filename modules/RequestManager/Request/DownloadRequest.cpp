#include "stdafx.h"
#include "DownloadRequest.h"
#include "..\codec\BQPacketLib.h"
#include "..\codec\QualcommCRCChecker.h"


namespace PLATFORM_TOOL
{
	bool ByteToChar(BYTE *pucHexBcdBuf, int iHexBcdBufSize, CStringA& strBuffer)
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

	CDownloadRequestBase::CDownloadRequestBase(const TSTRING& strName,bool bHadReturnvalue)
		:CRequestBase(strName,DL, 5000, bHadReturnvalue), 
		m_nErrorCode(DL_ERROR_SUCCESS), m_bRunSuccess(false),
		m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDownloadRequestBase::CDownloadRequestBase(const TSTRING& strName,PotocolType nPotocolType,int	nTimeout ,bool bHadReturnvalue)
		:CRequestBase(strName, nPotocolType, nTimeout, bHadReturnvalue), 
		m_nErrorCode(DL_ERROR_SUCCESS), m_bRunSuccess(false),
		m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDownloadRequestBase::~CDownloadRequestBase(){}

	bool CDownloadRequestBase::Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend, bool bSpeedup)
	{
		bool bRes = false;

		char chStart = START_CHAR;
		char chEnd = END_CHAR;	   //0x7E;
		void *pBuffer = NULL;
		int nPackedLen = 0;
		
		if(bSpeedup)
			nPackedLen = CBQPacketLib::SpeedUpPackBuffer( szCommand, nCommandLen, chStart, chEnd, &pBuffer);
		else
			nPackedLen =  CBQPacketLib::PackBuffer( szCommand, nCommandLen, chStart, chEnd, &pBuffer);
		
		ptBufferSend = new CBuffer(nPackedLen,(const char*)pBuffer);		
		delete[] pBuffer;

		bRes = nPackedLen > 0;
		assert(bRes);
		return bRes;
	}

	bool CDownloadRequestBase::Tx(const char* szCommand,int nCommandLen, bool bSpeedUp)
	{
		bool bRes = false;
		CBufferPtr ptBufferSend = new CBuffer;
		if(Encode(szCommand,nCommandLen,ptBufferSend,bSpeedUp))
		{		
			SetSendData(ptBufferSend);
			bRes = true;
		}
		else
			m_nErrorCode = DL_ERROR_TX_ERROR;
		return bRes;
	}


	bool CDownloadRequestBase::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		m_ptTempBufferReceived->Append(ptBufferReceived);

		char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());		
		QualcommCRCChecker CRCChecker(true);
		for ( DWORD j = 0 ; j < m_ptTempBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				bRes = true;
				ptBufferReceived = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
				m_ptTempBufferReceived->SetEmpty();
				break;
			}
		}
		return bRes;
	}
	bool CDownloadRequestBase::GetReceiveData(CBufferPtr& ptBufferReceived)
	{
		CRequestBase::GetReceiveData(ptBufferReceived);
		return GetProcessResult();
	}
	
	int CDownloadRequestBase::GetLastError()
	{
		return m_nErrorCode;
	}
	void CDownloadRequestBase::SetLastError(int nError)
	{
		m_nErrorCode = nError;
	}
	bool CDownloadRequestBase::GetProcessResult()
	{
		return m_bRunSuccess;
	}
	void CDownloadRequestBase::SetProcessResult(bool bResult)
	{
		m_bRunSuccess = bResult;
	}

	void CDownloadRequestBase::Safememcopy(void* pBuffer, int nBufferSize,void* pData,int nDataSize)
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
				//Síª¬p, glog
			}
			else if (GetName() == _T("CReadSWVersionRequest")
				&& ptSendData 
				&& ptSendData->GetData()
				&& 0 == ::memcmp(ptSendData->GetData(), btReadSWCmd, 4) // Get SW version
				&& nDataSize == 65)
			{
				//Síª¬p, glog
			}
			else
			{
				//PostUSBLog(ptSendData, pData, nBufferSize, nDataSize, _T("BufferSizeCheck.txt"));
			}
		}
	}
//*******************************************************************//		

	//CDLRAWRequest Request
	CDLRAWRequest::CDLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
		:CDownloadRequestBase(_T("CDLRAWRequest"),false)
	{
		if (bAddCRC)
		{
			CBufferPtr ptBufferSend = new CBuffer;
			Encode(szCmdInput, nCmdLength,ptBufferSend);
			SetSendData(ptBufferSend);
		}
		else
		{
			CBufferPtr ptBufferSend = new CBuffer(nCmdLength, szCmdInput);
			SetSendData(ptBufferSend);
		}
	}
	CDLRAWRequest::~CDLRAWRequest()
	{
	}

	//*******************************************************************//		

	//Send dl raw request with returned value
	CDLRawWithReturnRequest::CDLRawWithReturnRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
		:CDownloadRequestBase(_T("CDLRawWithReturnRequest"))
	{
		if (bAddCRC)
		{
			CBufferPtr ptBufferSend = new CBuffer;
			Encode(szCmdInput, nCmdLength,ptBufferSend);
			SetSendData(ptBufferSend);
		}
		else
		{
			CBufferPtr ptBufferSend = new CBuffer(nCmdLength, szCmdInput);
			SetSendData(ptBufferSend);
		}
	}
	CDLRawWithReturnRequest::~CDLRawWithReturnRequest()
	{
	}
	void CDLRawWithReturnRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}


	//*******************************************************************//	
	CDMSSNop::CDMSSNop(): CDownloadRequestBase(_T("CDMSSNop"), DL)
	{
		BYTE nDloadNopCmd = DMSS_CMD_NOP;
		CDownloadRequestBase::Tx((const char*) &nDloadNopCmd, sizeof(nDloadNopCmd));
	}
	CDMSSNop::~CDMSSNop(){}
	void CDMSSNop::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//


	CDMSSReset::CDMSSReset(): CDownloadRequestBase(_T("CDMSSReset"), DL)
	{
		BYTE nDloadNopCmd = DMSS_CMD_RESET;
		CDownloadRequestBase::Tx((const char*) &nDloadNopCmd, sizeof(nDloadNopCmd));
	}
	CDMSSReset::~CDMSSReset(){}
	void CDMSSReset::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDMSSParameter::CDMSSParameter(): CDownloadRequestBase(_T("CDMSSParameter"), DL)
	{
		DmssParaReq req = {0};
		req.nDloadCmd = DMSS_CMD_PREQ;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CDMSSParameter::~CDMSSParameter(){}
	void CDMSSParameter::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssPraResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == DMSS_CMD_PARAMS);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDMSSWrite32Bit::CDMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress): CDownloadRequestBase(_T("CDMSSWrite32Bit"), DL)
	{
		DmssWrite32bitAddressReq req = {0};
		req.nDloadCmd = DMSS_CMD_WRITE32BITADDRESS;
		DWORD dwtemp=dwAddress;
		for (int i=0;i<4;i++)
		{
			req.nsAddress[3-i] = dwtemp%256;
			dwtemp=dwtemp/256;
		}
		WORD wtemp=dwWLength;
		for (int i=0;i<2;i++)
		{
			req.nsLength[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		memcpy(req.nsData,lpWData,dwWLength);
		CDownloadRequestBase::Tx((const char*) &req, dwWLength+7);
	}
	CDMSSWrite32Bit::~CDMSSWrite32Bit(){}
	void CDMSSWrite32Bit::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CDMSSGo::CDMSSGo(WORD nwSegment,WORD nwoffset): CDownloadRequestBase(_T("CDMSSNop"), DL)
	{
		DmssGOReq req = {0};
		req.nDloadCmd = DMSS_CMD_GO;
		WORD wtemp = nwSegment;
		for (int i=0;i<2;i++)
		{
			req.nsSegment[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		wtemp=nwoffset;
		for (int i=0;i<2;i++)
		{
			req.nsOffset[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CDMSSGo::~CDMSSGo(){}
	void CDMSSGo::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLHello::CStreamingDLHello(int nVersion,int nCVersion,int nFeaturebit,bool bSpeedUp,DownloadProtocol nDownloadProtocol) 
		:CDownloadRequestBase(_T("CStreamingDLHello"), DL)
		,m_bSpeedUp(bSpeedUp)
		,m_nDownloadProtocol(nDownloadProtocol)
	{
		StreamingHelloParmReq req = {0};
		if(m_nDownloadProtocol == QTT_FASTDOWNLOAD)
		{
			req.nCmdCode = m_bSpeedUp ? Streaming_CMD_SpeedUp_HELLO : Streaming_CMD_HELLO;
			memcpy(req.nMagicNumber,"QCOM fast download protocol host",32);
		}
		else if(m_nDownloadProtocol == QTH_FASTDOWNLOAD)
		{
			req.nCmdCode = Streaming_CMD_HELLO;
			memcpy(req.nMagicNumber,"QCOM fast download protocol host",32);
		}
		req.nVersion = nVersion;
		req.nCVersion = nCVersion;
		req.nFeaturebit = nFeaturebit;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CStreamingDLHello::~CStreamingDLHello(){}
	bool CStreamingDLHello::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingHelloParmResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}	
			if(resp.nCmdCode == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLHello::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingHelloParmResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(m_bSpeedUp)
		{
			if(m_nDownloadProtocol == QTT_FASTDOWNLOAD)
				SetProcessResult(resp.nCmdCode == Streaming_CMD_SpeedUp_HELLORESP);
			else if(m_nDownloadProtocol == QTH_FASTDOWNLOAD)
				SetProcessResult(resp.nCmdCode == Streaming_CMD_HELLORESP);
		}
		else
			SetProcessResult(resp.nCmdCode == Streaming_CMD_HELLORESP);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//
	CStreamingDLSecurity::CStreamingDLSecurity(BYTE nMode, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLSecurity"), DL)
	{
		StreamingSecurityReq req = {0};
		req.nDloadCmd = Streaming_CMD_SECMODE;
		req.nMode = nMode;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLSecurity::~CStreamingDLSecurity(){}
	bool CStreamingDLSecurity::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingSecurityResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}	
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLSecurity::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		StreamingSecurityResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_SECMODER);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//

	CStreamingDLPartition::CStreamingDLPartition(BYTE* lpbyData, WORD wnlength, bool bOverride, BYTE& nStatus, bool bSpeedUp) 
		: CDownloadRequestBase(_T("CStreamingDLPartition"), DL), m_nRespStatus(nStatus)
	{
		StreamingPartitionReq req = {0};
		req.nDloadCmd = Streaming_CMD_PATTTIONT;
		req.nOverrive = bOverride ? 0x1 : 0x0;
		memcpy(req.nPartition,lpbyData,wnlength);
		CDownloadRequestBase::Tx((const char*) &req, wnlength+2, bSpeedUp);
	}
	CStreamingDLPartition::~CStreamingDLPartition(){}
	bool CStreamingDLPartition::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingPartitionResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLPartition::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingPartitionResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_PATTTIONTR);
		m_nRespStatus = resp.nStatus;/*0 = Partition table accepted*/
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLOpenMultiImage::CStreamingDLOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLOpenMultiImage"), DL)
	{
		StreamingOpenmimageReq req = {0};
		req.nDloadCmd = Streaming_CMD_OPENMIMAGE;
		req.nType = nType;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+2, bSpeedUp);
	}
	CStreamingDLOpenMultiImage::~CStreamingDLOpenMultiImage(){}
	bool CStreamingDLOpenMultiImage::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingOpenmimageResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLOpenMultiImage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingOpenmimageResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_OPENEDMIMAGE);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLOpenMultiImage7K::CStreamingDLOpenMultiImage7K(BYTE nType, BYTE* lpData, WORD wnLength, CString Md5FromFile, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLOpenMultiImage"), DL)
	{
		m_Md5FromFile = Md5FromFile;
		StreamingOpenmimageReq req = {0};
		req.nDloadCmd = Streaming_CMD_OPENMIMAGE;
		req.nType = nType;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+2, bSpeedUp);
	}
	CStreamingDLOpenMultiImage7K::~CStreamingDLOpenMultiImage7K(){}
	bool CStreamingDLOpenMultiImage7K::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingOpenmimageResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLOpenMultiImage7K::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingOpenmimageResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		bool b_isMd5Pass = true;
		if (resp.dummy[0] != 0 && resp.dummy[1] != 0 && resp.dummy[2] != 0 && resp.dummy[3] != 0 &&
			resp.dummy[4] != 0 && resp.dummy[5] != 0 && resp.dummy[6] != 0 && resp.dummy[7] != 0 &&
			resp.dummy[8] != 0 && resp.dummy[9] != 0 && resp.dummy[10] != 0 && resp.dummy[11] != 0 &&
			resp.dummy[12] != 0 && resp.dummy[13] != 0 && resp.dummy[14] != 0 && resp.dummy[15] != 0) {
			CString Md5FromTarget;
			for (int i = 0; i < 16; i ++)
			{
				CString tmp;
				tmp.Format("%02x", resp.dummy[i]);
				Md5FromTarget += tmp;
			}

			b_isMd5Pass = m_Md5FromFile.Compare(Md5FromTarget) == 0;
		}

		SetProcessResult(resp.nDloadCmd == Streaming_CMD_OPENEDMIMAGE && b_isMd5Pass);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLIRAMRead::CStreamingDLIRAMRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLIRAMRead"))
		,m_dwAddress(dwAddress)
	{
		StreamingStrReadReq req = {0};
		req.nDloadCmd = 0x26;
		
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nLength,&wnLength,sizeof(wnLength));
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}

	CStreamingDLIRAMRead::~CStreamingDLIRAMRead(){}
	bool CStreamingDLIRAMRead::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrReadResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLIRAMRead::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrReadResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_READDATA && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CStreamingDLeMMCRead::~CStreamingDLeMMCRead(){}
	bool CStreamingDLeMMCRead::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrReadResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLeMMCRead::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrReadResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_READDATA && memcmp(resp.nAddress,&m_dwSector,sizeof(m_dwSector)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	CStreamingDLeMMCRead::CStreamingDLeMMCRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLeMMCRead"))
		,m_dwSector(dwAddress)
	{
		StreamingStrReadReq req = {0};
		req.nDloadCmd = 0x3;
		
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwSector = dwAddress;
		memcpy(req.nLength,&wnLength,sizeof(wnLength));
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}


	CStreamingDLStrRead::CStreamingDLStrRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLStrRead"))
		,m_dwAddress(dwAddress)
	{
		StreamingStrReadReq req = {0};
		req.nDloadCmd = Streaming_CMD_READ;
		
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nLength,&wnLength,sizeof(wnLength));
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLStrRead::~CStreamingDLStrRead(){}
	bool CStreamingDLStrRead::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrReadResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrRead::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrReadResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_READDATA && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLStrWrite::CStreamingDLStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLStrWrite"))
		,m_dwAddress(dwAddress)
	{
		StreamingStrWriteReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRWRITE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+5, bSpeedUp);
	}
	CStreamingDLStrWrite::~CStreamingDLStrWrite(){}
	bool CStreamingDLStrWrite::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrWrite::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLStrBuffer7K::CStreamingDLStrBuffer7K(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLStrBuffer7K"))
		,m_dwAddress(dwAddress)
	{
		StreamingStrWriteReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRBUFFER_7K;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+5, bSpeedUp);
	}
	CStreamingDLStrBuffer7K::~CStreamingDLStrBuffer7K(){}
	bool CStreamingDLStrBuffer7K::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrBuffer7K::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_STRBUFFER_ACK_7K && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLStrBufferWrite7K::CStreamingDLStrBufferWrite7K(int nImageSize, CString MD5, bool bSpeedUp) 
		:CDownloadRequestBase(_T("CStreamingDLStrBuffer7K"))
	{
		StreamingStrBufferWriteReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRBUFFERWRITE_7K;
		memcpy(req.imageSize, &nImageSize, 4);
		m_dwAddress = nImageSize;
		byte Md5Tmp[16] = {0};
		for (int i = 0; i < MD5.GetLength(); i+=2)
		{
			CString tmp;
			char * pEnd;
			tmp = MD5.Mid(i, 2);
			int nMd5ToByte = strtol(tmp, &pEnd, 16);
			Md5Tmp[i/2] = nMd5ToByte;
		}
		memcpy(req.MD5, Md5Tmp, 16);
		CDownloadRequestBase::Tx((const char*) &req, 16+5, bSpeedUp);
	}
	CStreamingDLStrBufferWrite7K::~CStreamingDLStrBufferWrite7K(){}
	bool CStreamingDLStrBufferWrite7K::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrBufferWrite7K::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_STRBUFFERWRITE_ACK_7K && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLCloseImage::CStreamingDLCloseImage(bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLCloseImage"), DL)
	{
		StreamingCloseReq req = {0};
		req.nDloadCmd = Streaming_CMD_CLOSE;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLCloseImage::~CStreamingDLCloseImage(){}
	bool CStreamingDLCloseImage::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingCloseResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLCloseImage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingCloseResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_CLOSED);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLCloseImage7K::CStreamingDLCloseImage7K(CString Md5FromFile, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLCloseImage"), DL)
	{
		m_Md5FromFile = Md5FromFile;
		StreamingCloseReq req = {0};
		req.nDloadCmd = Streaming_CMD_CLOSE;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLCloseImage7K::~CStreamingDLCloseImage7K(){}
	bool CStreamingDLCloseImage7K::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingCloseResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLCloseImage7K::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingCloseResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		bool b_isMd5Pass = true;
		if (resp.dummy[0] != 0 && resp.dummy[1] != 0 && resp.dummy[2] != 0 && resp.dummy[3] != 0 &&
			resp.dummy[4] != 0 && resp.dummy[5] != 0 && resp.dummy[6] != 0 && resp.dummy[7] != 0 &&
			resp.dummy[8] != 0 && resp.dummy[9] != 0 && resp.dummy[10] != 0 && resp.dummy[11] != 0 &&
			resp.dummy[12] != 0 && resp.dummy[13] != 0 && resp.dummy[14] != 0 && resp.dummy[15] != 0) {
			CString Md5FromTarget;
			for (int i = 0; i < 16; i ++)
			{
				CString tmp;
				tmp.Format("%02x", resp.dummy[i]);
				Md5FromTarget += tmp;
			}

			b_isMd5Pass = m_Md5FromFile.Compare(Md5FromTarget) == 0;
		}

		SetProcessResult(resp.nDloadCmd == Streaming_CMD_CLOSED && b_isMd5Pass);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLReset::CStreamingDLReset(bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLReset"), DL)
	{
		StreamingResetReq req = {0};
		req.nDloadCmd = Streaming_CMD_RESET;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLReset::~CStreamingDLReset(){}
	bool CStreamingDLReset::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingResetResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLReset::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingResetResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_RESETACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/* Alex.Chen Add Start 2011/06/12 */
	CSetFastBootFlagQCSRequestFor8260::CSetFastBootFlagQCSRequestFor8260(bool& bSuccess,byte comport)
		:CDownloadRequestBase(_T("CSetFastBootFlagQCSRequest"), true)
		,m_bSuccess(bSuccess)
	{
		typedef struct
		{
			byte cmd_code;
			byte address[4];
			byte length[2];
			byte data[4];
		} CMD_UPLOAD_HEX_REQ_TYPE_t;

		CMD_UPLOAD_HEX_REQ_TYPE_t Req = {0};
		Req.cmd_code = 0x0F;

		Req.address[0] = 0x2A;
		Req.address[1] = 0x05;
		Req.address[2] = 0xF6;
		Req.address[3] = 0x5C;

		Req.length[1] = 4;

		Req.data[0] = 0x00;
		Req.data[1] = 0x55;
		Req.data[2] = 0x66;
		Req.data[3] = comport;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(CMD_UPLOAD_HEX_REQ_TYPE_t),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSetFastBootFlagQCSRequestFor8260::~CSetFastBootFlagQCSRequestFor8260()
	{

	}

	bool CSetFastBootFlagQCSRequestFor8260::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}

	void CSetFastBootFlagQCSRequestFor8260::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			byte rsp_code;
			byte buf[2048];
		} CMD_UPLOAD_HEX_RSP_TYPE_t;
		CMD_UPLOAD_HEX_RSP_TYPE_t resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.rsp_code == 2;
		assert(m_bSuccess);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Alex.Chen Add End 2011/06/12 */

	/* Evonne.Hsieh Add Start 2012/03/30 */
	CSetFastBootFlagQCSRequestFor8960::CSetFastBootFlagQCSRequestFor8960(bool& bSuccess,byte comport)
		:CDownloadRequestBase(_T("CSetFastBootFlagQCSRequest"), true)
		,m_bSuccess(bSuccess)
	{
		typedef struct
		{
			byte cmd_code;
			byte address[4];
			byte length[2];
			byte data[4];
		} CMD_UPLOAD_HEX_REQ_TYPE_t;

		CMD_UPLOAD_HEX_REQ_TYPE_t Req = {0};
		Req.cmd_code = 0x0F;

		Req.address[0] = 0x2A;
		Req.address[1] = 0x05;
		Req.address[2] = 0xF6;
		Req.address[3] = 0x5C;

		Req.length[1] = 4;

		Req.data[0] = 0x00;
		Req.data[1] = 0x55;
		Req.data[2] = 0x66;
		//Req.data[3] = comport;
		Req.data[3] = 0x77;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(CMD_UPLOAD_HEX_REQ_TYPE_t),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSetFastBootFlagQCSRequestFor8960::~CSetFastBootFlagQCSRequestFor8960()
	{

	}

	bool CSetFastBootFlagQCSRequestFor8960::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}

	void CSetFastBootFlagQCSRequestFor8960::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			byte rsp_code;
			byte buf[2048];
		} CMD_UPLOAD_HEX_RSP_TYPE_t;
		CMD_UPLOAD_HEX_RSP_TYPE_t resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.rsp_code == 2;
		assert(m_bSuccess);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Evonne.Hsieh Add End 2012/03/30 */

	/* Evonne.Hsieh Add Start 2012/02/17 */
	CSetFastBootFlagQCSRequestFor7227A::CSetFastBootFlagQCSRequestFor7227A(CString str_multiDLFlag, bool& bSuccess, byte comport)
		:CDownloadRequestBase(_T("CSetFastBootFlagQCSRequest"), true)
		,m_bSuccess(bSuccess)
	{
		typedef struct
		{
			byte code;
			byte repartition;
			byte table[4096];
		} CMD_UPLOAD_HEX_REQ_TYPE_t;

		CMD_UPLOAD_HEX_REQ_TYPE_t Req = {0};
		Req.code = 0x2A;
		Req.repartition = 2;
		
		UINT nFileSize = 16;
		byte* pData = new byte[nFileSize];
		int add1 = 0xFFFEF808;
		int data1 = 0xA4BC385F;
		int add2 = 0xFFFEF80C;
		char * pEnd;
		int data2 = strtol(str_multiDLFlag, &pEnd, 16);

		memcpy(&pData[0], &add1, 4);
		memcpy(&pData[4], &data1, 4);
		memcpy(&pData[8], &add2, 4);
		memcpy(&pData[12], &data2, 4);
		memcpy(Req.table, pData, nFileSize);

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(CMD_UPLOAD_HEX_REQ_TYPE_t),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSetFastBootFlagQCSRequestFor7227A::~CSetFastBootFlagQCSRequestFor7227A()
	{

	}

	bool CSetFastBootFlagQCSRequestFor7227A::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}

	void CSetFastBootFlagQCSRequestFor7227A::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			byte rsp_code[2];
			byte buf[2048];
		} CMD_UPLOAD_HEX_RSP_TYPE_t;
		CMD_UPLOAD_HEX_RSP_TYPE_t resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.rsp_code[0] == 50 && resp.rsp_code[1] == 0;
		assert(m_bSuccess);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Evonne.Hsieh Add Start 2012/02/17 */
	/* Macl.Ma Add Start 2012/07/19 */
	CSendOEMCMD::CSendOEMCMD() : CDownloadRequestBase(_T("CSendOEMCMD"), true)
	{
		typedef struct
		{
			BYTE nCmd[2];
		} OEMCMD;

		OEMCMD Req = {0};
		Req.nCmd[0] = 0x11;
		Req.nCmd[1] = 0x01;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(OEMCMD), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CSendOEMCMD::~CSendOEMCMD()
	{

	}
	bool CSendOEMCMD::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}
	void CSendOEMCMD::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			BYTE nCmd[2];
		} OEMCMD_Res;
		OEMCMD_Res resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nCmd[0] == 0x12 && resp.nCmd[1] == 0x01);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/* Lennon.Zhang Add Start 2013/01/25 */
	CSetFastBootFlagQCSRequestFor8X25::CSetFastBootFlagQCSRequestFor8X25(bool& bSuccess,byte comport)
		:CDownloadRequestBase(_T("CSetFastBootFlagQCSRequest"), true)
		,m_bSuccess(bSuccess)
	{
		typedef struct
		{
			byte cmd_code;
			byte address[4];
			byte length[2];
			byte data[4];
		} CMD_UPLOAD_HEX_REQ_TYPE_t;

		CMD_UPLOAD_HEX_REQ_TYPE_t Req = {0};
		Req.cmd_code = 0x0F;

		Req.address[0] = 0x2A;
		Req.address[1] = 0x05;
		Req.address[2] = 0xF6;
		Req.address[3] = 0x5C;

		Req.length[1] = 4;

		Req.data[0] = 0x00;
		Req.data[1] = 0x55;
		Req.data[2] = 0x66;
		Req.data[3] = comport;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(CMD_UPLOAD_HEX_REQ_TYPE_t),ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CSetFastBootFlagQCSRequestFor8X25::~CSetFastBootFlagQCSRequestFor8X25()
	{

	}

	bool CSetFastBootFlagQCSRequestFor8X25::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}

	void CSetFastBootFlagQCSRequestFor8X25::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			byte rsp_code;
			byte buf[2048];
		} CMD_UPLOAD_HEX_RSP_TYPE_t;
		CMD_UPLOAD_HEX_RSP_TYPE_t resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.rsp_code == 2;
		assert(m_bSuccess);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/* Lennon.Zhang Add End 2013/01/25 */

	CSendResetCMD::CSendResetCMD() : CDownloadRequestBase(_T("CSendResetCMD"), true)
	{
		typedef struct
		{
			BYTE nCmd;
		} SendResetCMD;

		SendResetCMD Req = {0};
		Req.nCmd = 0x0b;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(SendResetCMD), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CSendResetCMD::~CSendResetCMD()
	{

	}
	bool CSendResetCMD::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}
	void CSendResetCMD::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			BYTE nCmd;
		} SendResetCMD_Res;
		SendResetCMD_Res resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nCmd == 0x0c);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Alex.Chen Add Start 2011/09/28 */
	CDDRTest::CDDRTest() : CDownloadRequestBase(_T("CDDRTest"), true)
	{
		typedef struct
		{
			BYTE nCmd;
		} DDRTestReq;

		DDRTestReq Req = {0};
		Req.nCmd = 0x1D;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(DDRTestReq), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CDDRTest::~CDDRTest()
	{

	}
	bool CDDRTest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}
	void CDDRTest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			BYTE nCmd;
		} DDRTest_Res;
		DDRTest_Res resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nCmd == 0x1E);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CGetDDRTestResult::CGetDDRTestResult() : CDownloadRequestBase(_T("CGetDDRTestResult"), true)
	{
		typedef struct
		{
			BYTE nCmd;
		} DDRTestResultReq;

		DDRTestResultReq Req = {0};
		Req.nCmd = 0x1F;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(DDRTestResultReq), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CGetDDRTestResult::~CGetDDRTestResult()
	{

	}
	bool CGetDDRTestResult::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}
	void CGetDDRTestResult::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		typedef struct
		{
			BYTE nCmd;
		} DDRTestResult_Res;
		DDRTestResult_Res resp = {0};

		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nCmd == 0x20);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Alex.Chen Add End 2011/09/28 */

	/* Alex.Chen Add Start 2011/10/13 */
	CStreamingRead::CStreamingRead() : CDownloadRequestBase(_T("CDMSSRead"), true)
	{
		StreamingReadReq Req = {0};
		Req.nDloadCmd = Streaming_CMD_READ;

		Req.nAddress[0] = 0x2A;
		Req.nAddress[1] = 0x05;
		Req.nAddress[2] = 0xF6;
		Req.nAddress[3] = 0x5C;

		Req.nLength[1] = 4;

		CBufferPtr ptBufferSend = new CBuffer;
		Encode((const char *)&Req, sizeof(StreamingReadReq), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CStreamingRead::~CStreamingRead()
	{

	}
	bool CStreamingRead::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}
	void CStreamingRead::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingReadResp resp = {0};
		Safememcopy(&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_READDATA);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	/* Alex.Chen Add End 2011/10/13 */

}
