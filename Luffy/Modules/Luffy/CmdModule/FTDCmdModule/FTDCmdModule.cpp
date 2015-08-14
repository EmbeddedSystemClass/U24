#include "stdafx.h"
#include "FTDCmdModule.h"
#include <iostream>

#include <fstream>
#include <cstddef> // size_t
#include <ctime>

#include "UtilityForFTD.h"
#include "CmdType.h"

/****************************************************************
CFTDCmdModule
*****************************************************************/
CFTDCmdModule::CFTDCmdModule(UINT Port)
: m_Port( Port ),
  m_Timeout( 10000 ),
  m_Device( NULL ),
  m_ExtraCmd( NULL ),
  m_szErrMsg( NULL ),
  m_ExtraCmdSize(0)
{
	this->ConnectDevice();
	this->Open();	
}

void CFTDCmdModule::SetExtraCmd(char *szExtraCmd)
{
	m_ExtraCmdSize = strlen(szExtraCmd);

	if ( 0 < m_ExtraCmdSize )
	{
		this->m_ExtraCmd = new char[m_ExtraCmdSize];
		memcpy(this->m_ExtraCmd, (unsigned char *)szExtraCmd, m_ExtraCmdSize);
	}

	this->m_szErrMsg = new char[ERRMSG_SIZE];
	memset(this->m_szErrMsg, 0, ERRMSG_SIZE);
}

void CFTDCmdModule::SetExtraCmd(CString szExtraCmd)
{
	m_ExtraCmdSize = szExtraCmd.GetLength() ;

	if ( 0 < m_ExtraCmdSize )
	{
		this->m_ExtraCmd = new char[m_ExtraCmdSize];
		memcpy(this->m_ExtraCmd, szExtraCmd, m_ExtraCmdSize);
	}

	this->m_szErrMsg = new char[ERRMSG_SIZE];
	memset(this->m_szErrMsg, 0, ERRMSG_SIZE);
}

char *CFTDCmdModule::GetExtraCmd()
{
	return this->m_ExtraCmd;
}

const UINT CFTDCmdModule::GetExtraCmdSize() const
{
	return this->m_ExtraCmdSize;
}

char *CFTDCmdModule::GetErrResponse()
{
	return this->m_szErrMsg;
}

CFTDCmdModule::~CFTDCmdModule(void)
{
	this->DisConnectDevice();

	if(NULL != this->m_ExtraCmd)
	{
		delete this->m_ExtraCmd;
		this->m_ExtraCmd = NULL;
	}
}

bool CFTDCmdModule::ConnectDevice()
{
	if (m_Device == NULL)
	{
		if(m_Port == PORT_TCP) 
		{
			m_Device = new TcpDrv();
		}
	}

	return true;
}

bool CFTDCmdModule::Open()
{
	if (m_Device == NULL) 
		return false;

	int iRetryCounter = 0;
	bool bRet = false;
	while (iRetryCounter < 10)
	{
		unsigned char ucOpenStatus;
		ucOpenStatus = m_Device->open(m_Port, 115200, NOPARITY, ONESTOPBIT, 8);

		if (ucOpenStatus < 0)
		{
			Sleep(5);
			++iRetryCounter;
		}
		else
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

bool CFTDCmdModule::DisConnectDevice()
{
	if (m_Device != NULL) 
	{
		m_Device->close();
	}
	delete m_Device;
	m_Device = NULL;

	return true;
}

bool CFTDCmdModule::IsConnected()
{
	if (m_Device != NULL) 
		return true;
	else
		return false;
}

bool CFTDCmdModule::PreExe()
{
	return true;
}

bool CFTDCmdModule::ExeCommand(CCmdPtr &Cmd)
{
	if (m_Device == NULL)
		return false;

	bool ret = false;

	Cmd->SetResponseBufferSize(4096);

	ret = ExactGeneralCmdInOut(m_Port, m_Timeout, (char *)Cmd->GetCmd(), this->GetExtraCmd() , this->GetExtraCmdSize(), (char *)Cmd->GetResponse(), this->m_szErrMsg);

	return ret;

}

bool CFTDCmdModule::PostExe()
{
	return true;
}

bool CFTDCmdModule::EnterFTDMode()
{
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("shell ftd");

	bool ret = false, success = false;

	for(int i = 0;i < 10;i++)
	{
		ret = Exec(adb_path, param, 1000, true);

		if(ret) 
		{
			if(param.Find(_T("system/core/ftd")) == -1) 
			{
				success = false;
				continue;
			}
			else 
			{
				success = true;
				break;
			}
		}
	}

	return ret && success;
}

bool CFTDCmdModule::VerifyFTDMode()
{
	char *szErrMsg = new char[1024];
	char *msgBuffer = new char[1024 * 1024 * 2];

	bool ret = false, success = false;

	for(int i = 0;i < 10;i++)
	{
		ret =  ExcuteADBOut("shell ps", msgBuffer, szErrMsg);

		if(ret) 
		{
			CString outBuffer = msgBuffer;
			if(outBuffer.Find(_T("ftd")) == -1) 
			{
				//sprintf_s(szErrMsg, 1024, _T("ERROR: Not in ftd mode"));
				success = false;
				continue;
			}
			else 
			{
				success = true;
				break;
			}
		}
	}

	return ret && success;
}

bool CFTDCmdModule::ExactGeneralCmdInOut(int nPort, int nTimeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer, char* szErrMsg)
{
	if (m_Device == NULL) 
	{
		szErrMsg = "No Devices detected.";
		return false;
	}

	Sleep(15);
	//�ҥHExact_GeneralCmdIn�n�ζǶi�Ӫ�InBuffer��size�j�p

	int PayloadSize = (int)i_InLength;
	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));

	cGenPayload[0] = CtrlBuffer[0];
	cGenPayload[1] = CtrlBuffer[1];
	cGenPayload[2] = CtrlBuffer[2];
	cGenPayload[3] = CtrlBuffer[3];


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;//�ΨӨ��o��X�Ӫ�Checksum
	unsigned char cGenPayloadFinal[PAYLOAD_SIZE]; // FIXME buffer size
	memcpy(cGenPayloadFinal, cGenPayload, 4);
	memcpy(cGenPayloadFinal+4, (unsigned char *)InBuffer, PayloadSize); //FF:�p�G���Ѽƪ���copy �ݬݶǶi�Ӫ�Payloadsize�O�h��

	Sleep(20);

	SendResult = GenPacketSend(PKT_REQUEST, PayloadSize, cGenPayloadFinal, &OldChecksum, nPort, szErrMsg);
	if (SendResult < HS_OK)
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketSend Fail (Code %d)", SendResult);
		return false;
	}

	Sleep(20);
	//���BFileSize���ɤw�g�ܦ�sector�F,�S�Ψ�

	unsigned char pPackData[PAYLOAD_SIZE];
	memset(pPackData, 0, sizeof(pPackData));
	ReceiveResult = GenPacketRecv(PKT_ACK, cGenPayloadFinal, pPackData, &NewChecksum, nTimeout, szErrMsg);
	if (ReceiveResult != HS_OK) 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv ACK Fail (Code %d)", SendResult);
		return false;
	}

	//�A�ӳo�����ӭncheck�@�Uchksum�a
	//double check checksum
	//level 2: check the sender and the receiver
	//���b���ʥ]���ɭԴN�ˬd�L�@���F �̩��h
	//�{�b�h�O�ˬd �e�ʥ] �� ���ʥ] ���a�� �ҥHYT�~�|�s��Level2�a
	if (OldChecksum != NewChecksum) 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "CheckSum Fail (%x != %x)", OldChecksum, NewChecksum);
		return false;
	}

	//// 2008-02-25 Nek Wang: receive result
	memset(pPackData, 0, sizeof(pPackData));
	// Check receive buffer
	int nRetCode = HS_FAIL;

	int iRetrytime = nTimeout / 2;
	int iRetrycounter = 0;

	while (/*iRetrycounter < iRetrytime*/1)
	{
		Sleep(50);
		ReceiveResult = GenPacketRecv(PKT_RESULT, cGenPayload, pPackData, &NewChecksum, nTimeout, szErrMsg);
		if (ReceiveResult != HS_OK) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv Result Fail(Code %d)", ReceiveResult);
			break;
		}

		nRetCode = PacketResultCheck(ReceiveResult, pPackData, szErrMsg);
		if (nRetCode == HS_OK) 
		{
			memcpy(pPackData, (pPackData + 1), PAYLOAD_SIZE * sizeof(unsigned char)); // FIXME buffer size
			break;
		}
		else if (nRetCode != HS_PROGRESS) 
		{
			break;
		}

		++iRetrycounter;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�U�@�Ǧ^�Ȥ���j����...??�@�w�|��
	//�ҥH�٬O����ӰѼƷ|����n
	memset(OutBuffer, 0, sizeof(OutBuffer));
	memcpy(OutBuffer, pPackData, PAYLOAD_SIZE);
	if (nRetCode != HS_OK) 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenCommand Fail(Code %d)", nRetCode);
		return false;
	}

	return true;
}

UINT CFTDCmdModule::GenPacketSend(int iType, int PayLoadSize, unsigned char * pPackData, unsigned char * OChecksum, int nPort, char* szErrMsg)
{
	int TotalDataSize; //Total�n�Ǫ�DataSize�ۤv��
	unsigned char PacketHeader[4] = {'B','E','N','Q'};

	//�`�N�o�MP31 PROTOCOL ENDIAN���P
	//�]��command�Pdata�@�Τ@�ӰѼ�pPackData
	//�ҥH��ڪ�payload size�ٱo����4 byte(for command)
	//���b��DATA DL���ɭԦ]���Ƕi�Ӫ�PAYLOADSIZE�N�ODATA SIZE
	//�ҥH�N����
	//�H��n�O�o��n,�ثe���p�O�|�Ƕi 4(COMMAND)+PAYLOAD(DATA)
	//��DATA,���OPAYLOADSIZE�W�٫o�S�����T�ϬM�����p
	// 2006-08-23 �w�ץ�: Payloadsize = sizeof(pPackData)-4
	unsigned char FileSize[4];
	FileSize[0] = (unsigned char)(PayLoadSize >> 24);
	FileSize[1] = (unsigned char)(PayLoadSize >> 16);
	FileSize[2] = (unsigned char)(PayLoadSize >> 8);
	FileSize[3] = (unsigned char)PayLoadSize;

	unsigned char temp[9000];
	memset(&temp, 0, sizeof(temp));
	unsigned char Checksum;
	//�ˬd
	//1.packdata�O�_����temp��,ex�W�L�]�w�W��
	//2.�`�Npacketsize�O�_��packdata�j
	if (iType == PKT_REQUEST) 
	{
		memcpy(temp, PacketHeader, PKT_HEADER_SIZE); // packet header -> "BENQ"
		temp[4] = (unsigned char)iType;  // Command Type
		memcpy(temp + 5, pPackData, 4); // command or block number
		memcpy(temp + 9, FileSize, 4);  // payload size
		memcpy(temp + 13, pPackData + 4, PayLoadSize); // payload data
		TotalDataSize = PKT_HEADER_SIZE + 1 + 4 + 4 + PayLoadSize;

		//Calculate Checksum
		Checksum = temp[0];
		for (int j = 1; j < TotalDataSize; j++) 
		{
			Checksum ^= temp[j];
		}
		temp[TotalDataSize] = Checksum;
		*OChecksum = Checksum;
		TotalDataSize++; //add checksum

		int nDataSize = TotalDataSize;
		int nPktSend = 0;
		int nByteWrite = PKT_BLOCK_SIZE;
		while (nDataSize > 0) 
		{
			nByteWrite = (nDataSize < nByteWrite) ? nDataSize : PKT_BLOCK_SIZE;
			if (m_Device->write(&temp[nPktSend * PKT_BLOCK_SIZE], nByteWrite) != nByteWrite) 
			{
				//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketSend Fail");
				return HS_WRITE_DATA_FAIL;
			}
			nPktSend++;
			nDataSize -= nByteWrite;
			Sleep(300);
		}
		return HS_OK;
		
	}// if Type is PKT_REQUEST

	return HS_FAIL;
}

UINT CFTDCmdModule::GenPacketRecv(int iType, unsigned char * PayloadCommand, unsigned char * pReceiveData, unsigned char * NChecksum, int nTimeout, char* szErrMsg)
{
	//TRACER_FUNCTION_IN();
	// �~��:  while (depackage()) �p�G�^�ǬOtrue�N�@�ȸѫʥ]��
	// �@�������ɮ�,��������O,�p�Gtimeout �N��������
	// 1.����ʥ]���Ķ,�ñN���G�Q��packdata�}�C�ǥX�h
	// 1.1 function
	int nPayloadSize = 0;
	unsigned char data[4];
	data[0] = 0;
	unsigned char temp[5000];
	memset(temp, 0, sizeof(temp));
	clock_t start;
	clock_t finish;
	start = clock();
	while (1) 
	{
		//���ձ����Ĥ@��byte, �ÿ�{�O�_�����wFunction���ʥ]header
		//�]��m_pCommDrv��timeout����,�ҥH...���ݭn�ۤv�]�wtimeout
		//[B]ENQ
		if (m_Device->read(data, 1, nTimeout) != 1) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv while B, timeout");
			return HS_READ_DATA_FAIL;
		}

		if (data[0] != 'B') 
		{
			finish = clock();
			if((finish-start) > nTimeout) {
				break;
			}
			
			continue;
		}
		temp[0] = data[0];

		//B[E]NQ
		if (m_Device->read(data, 1, 300) != 1) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv while E, timeout");
			return HS_READ_DATA_FAIL;
		}

		if (data[0] != 'E') 
		{ //FF: try to read E

			/*2012.5.10. Lion Fix Double 'B' Bug -> B B E N Q*/
			int isSecondB = 0;
			if (data[0] == 'B') 
			{ // B[B]E N Q
				isSecondB = 1;
				if (m_Device->read(data, 1, 300) != 1) 
				{
					//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv while E, timeout");
					return HS_READ_DATA_FAIL;
				}
				if (data[0] != 'E') 
				{ // BB[E]NQ
					continue;
				}
			}
			if(isSecondB == 0 )
			{
				continue;
			}
			/*Lion Fix Double 'B' Bug -> B B E N Q*/
		}

		temp[1] = data[0];

		//BE[N]Q
		if (m_Device->read(data, 1, 300) != 1) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv while N, timeout");
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'N') 
		{ //FF: try to read N
			continue;
		}
		temp[2] = data[0];

		//BEN[Q]
		if (m_Device->read(data, 1, 300) != 1) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv while Q, timeout");
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'Q') 
		{ //FF: try to read Q
			continue;
		}
		temp[3] = data[0];

		if (m_Device->read(data, 1, 300) != 1) 
		{
			//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv BENQ packet data corruption: Type");
			return HS_READ_DATA_FAIL;
		}
		temp[4] = data[0];

		//�P�_�O�O�_�n����cmd�άOprogress
		//�p�G�Oprogress���ܤ]�n���U�ʥ]���e��Ǧ^�h
		bool progress_flag = false;
		if (data[0] == PKT_PROGRESS) 
		{
			progress_flag = true;
		}
		if (data[0] == iType || progress_flag) 
		{
			//�n��DataSize+1��,�]���̫��٦�1��checksum
			//FF:Get command or block number (DL or UL function)
			if (m_Device->read(data, 4, 300) != 4) 
			{
				//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv BENQ packet data corruption: Command");
				return HS_READ_DATA_FAIL;
			}
			memcpy(temp + 5, data, 4);

			//FF:get payload size
			if (m_Device->read(data, 4, 300) != 4) 
			{
				//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv BENQ packet data corruption: Payload size");
				return HS_READ_DATA_FAIL;
			}
			memcpy(temp + 9, data, 4);

			nPayloadSize = data[3] + data[2] * 16 * 16 + data[1] * 16 * 16 * 16 + data[0] * 16 * 16 * 16 * 16;
			int nRead = m_Device->read(temp + 13, nPayloadSize + 1, 5000);
			if (nRead != nPayloadSize + 1) 
			{
				//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv packet read size fail(%d != %d)", nRead, nPayloadSize);
				return HS_READ_DATA_FAIL;
			}

			unsigned char PacketChecksum = temp[0];
			//check whole packet checksum
			for (int i = 1; i < 9 + 4 + nPayloadSize; i++) 
			{
				PacketChecksum ^= temp[i];
			}
			//double check checksum�o�̬O��pc tool�o���X�Ӫ�checksum�Ӹ�DT�ǹL�Ӫ���
			//level 1: check whole packet
			if (PacketChecksum != temp[9 + 4 + nPayloadSize]) 
			{
				//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv packet checksum Fail");
				return HS_INVALID_CHECKSUM;
			}
			//�٭n�ˬdDT�Ǧ^�Ӫ�cmd�O�_�O���ePC tool�ǹL�h��command
			for (int i = 0; i < 4; i++) 
			{
				if (temp[i + 5] != PayloadCommand[i]) 
				{
					//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv check command fail(%d): %d != %d", i, temp[i + 5], PayloadCommand[i]);
					return HS_READ_DATA_FAIL;
				}
			}

			// drop out checksum
			// last packet checksum in index 9+5 if its a ack packet
			*NChecksum = temp[9 + 4 + nPayloadSize - 1];
			//copy data to temp, �u���b���araw data�ɤ~�ݭn
			memcpy(pReceiveData, &temp[13], nPayloadSize);
			if (progress_flag) 
			{
				return HS_PROGRESS;
			}
			else 
			{
				return HS_OK;
			}
		} // if (type == true) end
	} // whole pack ok?
	//FFYang 0927

	//sprintf_s(szErrMsg, ERRMSG_SIZE, "GenPacketRecv check packet header \"BENQ\" Fail");
	return HS_FAIL;
}

UINT CFTDCmdModule::PacketResultCheck(int iPacketResult, unsigned char * pPackData, char* szErrMsg)
{
	if (iPacketResult == HS_PROGRESS) 
	{
		return HS_PROGRESS;
	}
	else if (iPacketResult != HS_OK) 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "PacketResultCheck recv packet Fail");
		return iPacketResult;
	}

	if (pPackData[0] == '1') 
	{
		return HS_OK;
	}
	else if (pPackData[0] == '0') 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "PacketResultCheck recv packet fail(0)");
		return HS_FAIL;
	}
	else 
	{
		//sprintf_s(szErrMsg, ERRMSG_SIZE, "PacketResultCheck recv packet fail(Other)");
		return HS_FAIL;
	}
}

/****************************************************************
CFTDCmdContainer
*****************************************************************/
CFTDCmdContainer::CFTDCmdContainer(char *Cmd, UINT CmdSize, bool HasReturnValue, UINT TimeOut)
: m_Cmd( NULL ),
  m_Response( NULL ),
  m_TimeOut( TimeOut ),
  m_HasReturnValue( HasReturnValue ),
  m_CmdSize( CmdSize )
{
	if( 0 < this->m_CmdSize )
	{
		this->m_Cmd = new char [this->m_CmdSize];
		memcpy((void *)this->m_Cmd, (void *)Cmd, this->m_CmdSize);		
	}
}

CFTDCmdContainer::~CFTDCmdContainer()
{
	if(NULL != this->m_Cmd)
	{
		delete this->m_Cmd;
		this->m_Cmd = NULL;
	}

	if(NULL != this->m_Response)
	{
		delete this->m_Response;
		this->m_Response = NULL;
	}
}

const BYTE *CFTDCmdContainer::GetCmd() const
{
	return (BYTE*)this->m_Cmd;
}

const BYTE *CFTDCmdContainer::GetResponse() const
{
	return (BYTE*)this->m_Response;
}

const UINT CFTDCmdContainer::GetCmdSize() const
{
	return this->m_CmdSize;
}

bool CFTDCmdContainer::HasReturnValue() const
{
	return this->m_HasReturnValue;
}

UINT CFTDCmdContainer::GetTimeOut() const
{
	return this->m_TimeOut;
}

void CFTDCmdContainer::SetResponseBufferSize(UINT size)
{
	if( NULL != this->m_Response )
	{
		delete this->m_Response;
		this->m_Response = NULL;
	}

	this->m_Response = new char [size];
	::ZeroMemory((void *)this->m_Response, size);
}

UINT CFTDCmdContainer::GetResponseSize() const
{
    return sizeof this->m_Response ;
}