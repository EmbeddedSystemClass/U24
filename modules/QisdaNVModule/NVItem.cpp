#include "StdAfx.h"
#include "NVItem.h"
#include "..\RequestManager\Request\NVRequest.h"
#include "..\Common\CommonUtil\PIMStringUtility.h"
#include "..\Common\CommonUtil\LogPolicy.h"
#include "..\RequestManager\Request\ODMRequest.h"
#include "..\RequestManager\Request\PowerMgrRequest.h"

using namespace PLATFORM_TOOL;


CNVItem::CNVItem(const TSTRING& strDevice)
:CInterfaceBase(strDevice)
,CLog(strDevice)
{
}

CNVItem::CNVItem(int nCOMPort)
:CInterfaceBase(COMDeviceName(nCOMPort))
,CLog(COMDeviceName(nCOMPort))
{
}

CNVItem::~CNVItem()
{
}

bool CNVItem::WriteSIMLockFileNameToNV(const std::string& strSIMLockFileName, int nTimeout)
{
	bool bRes = false;

	DIAG_NVITEM_PACKET_F_type ReadResp;
	::ZeroMemory(&ReadResp,sizeof(DIAG_NVITEM_PACKET_F_type));
	assert(strSIMLockFileName.size() <= 64);
	if(strSIMLockFileName.size() <= 64)
		bRes = WriteNVItem(6105,ReadResp,(const uint8 *)strSIMLockFileName.c_str(),strSIMLockFileName.size());

	return bRes;
}

bool CNVItem::ReadSIMLockFileNameFromNV(std::string& strSIMLockFileName, int nTimeout)
{
	bool bRes = false;

	DIAG_NVITEM_PACKET_F_type ReadResp;
	::ZeroMemory(&ReadResp,sizeof(DIAG_NVITEM_PACKET_F_type));
	bRes = ReadNVItem(NV_QISDA_LOC_FILENAME, ReadResp);
	if(bRes)
	{
		char szBuffer[128] = {0};
		memcpy(szBuffer,ReadResp.item_data,128);
		strSIMLockFileName = szBuffer;
		bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::UnlockSPC(const char* szLock, bool& bUnlockOK)
{
	bUnlockOK = false;

	bool status = false;
	CRequestPtr spcReq = new CSPCRequest(szLock,status);
	if (Execute(spcReq))
	{
		bUnlockOK = status;
		return true;
	}

	return false;
}

/* *****************************************************************
*  The method of locking spc is to query spc by giving an incorrect
*  spc code.
*  So, it means that to unlock spc is to query spc by correctly giving
*  the spc code.
***************************************************************** */
bool CNVItem::LockSPC()
{
	DIAG_NVITEM_PACKET_F_type ReadResp;
	CRequestPtr ptRequest = new CNVRequest(NV_SEC_CODE_I, DIAG_NV_READ_F, ReadResp);
	bool bRes = false;

	if (Execute(ptRequest))
	{
		if (ReadResp.item_data[0] != '1')
			ReadResp.item_data[0] = '1';
		else
			ReadResp.item_data[0] = '2';

		char szSPC[20] = {0};

		memcpy((char*)szSPC, (char*)(ReadResp.item_data), sizeof(char)*6);
		if(szSPC[0] != '1')
			szSPC[0] = '1';
		else
			szSPC[0] = '2';

		bool bUnlock = true;
		if(UnlockSPC(szSPC, bUnlock))
		{
			bRes = !bUnlock ? true : false;
		}
	}
	return bRes;
}

bool TransferHexToInt_BCD(const int nHex/*16進制*/, char* sz/*10進制*/)
{// Warning: This function changes 0x12 to "12", not "18"  !!

	bool bRes = ( (nHex >> 4) <= 9)
		&& ((nHex & 0x0F) <= 9);
	if (bRes)
	{
	*sz = (nHex >> 4) + '0'; //十位數
	*(sz+1) = (nHex & 0x0F) + '0';  //個位數
	*(sz+2) = 0;
	}
	return bRes;
}

bool CNVItem::ReadMobileID(char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3)
{
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	bool bRes = Execute(ptRequest);
	if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
	{
		*(szLine7) = 'T';
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45], szLine7+1);
		*(szLine7+3) = 'D';
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+1], szLine7+4);

		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+3], szAdapter3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+4], szSlot3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+5], szDay3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+6], szMonth3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+7], szYear3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+8], szHour3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+9], szMinute3);
		bRes &= TransferHexToInt_BCD(ReadResp.item_data[45+10], szSecond3);
	}
	
	if(bRes == false)
		PopLastLog();

	return bRes;
}

int TransferIntToHex_BCD(int nInt/*10進制*/)
{// Warning: This function changes 16 to 0x16, not 0x10  !!
	int nHex = ((nInt/10) << 4) + (nInt % 10);
	return nHex;
}

bool CNVItem::WriteMobileID(char* szSlot3)
{	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	Execute(ptRequest);

	char szComputer[1024] = {0};
	DWORD dwSize = 1024;
	GetComputerNameA(szComputer,&dwSize);
	if(strlen(szComputer) == 6 && szComputer[0] == 'T' && szComputer[3] == 'D')
	{
		szComputer[3] = 0;
		szComputer[6] = 0;
		char* szT = szComputer + 1;
		char* szD = szComputer + 4;
		ReadResp.item_data[45+0] = TransferIntToHex_BCD(atoi(szT));
		ReadResp.item_data[45+1] = TransferIntToHex_BCD(atoi(szD));
		ReadResp.item_data[45+2] = 0;
	}
	else
	{//Fake data T01D01
		ReadResp.item_data[45+0] = 1;
		ReadResp.item_data[45+1] = 1;
		ReadResp.item_data[45+2] = 0;
	}

	ReadResp.item_data[45+3] = 0;//Adapter
	ReadResp.item_data[45+4] = TransferIntToHex_BCD(atoi(szSlot3));//Slot

	CTime tCurrent = CTime::GetCurrentTime();
	ReadResp.item_data[45+5] = TransferIntToHex_BCD(tCurrent.GetDay());
	ReadResp.item_data[45+6] = TransferIntToHex_BCD(tCurrent.GetMonth());
	ReadResp.item_data[45+7] = TransferIntToHex_BCD(tCurrent.GetYear() % 100);
	ReadResp.item_data[45+8] = TransferIntToHex_BCD(tCurrent.GetHour());
	ReadResp.item_data[45+9] = TransferIntToHex_BCD(tCurrent.GetMinute());
	ReadResp.item_data[45+10] = TransferIntToHex_BCD(tCurrent.GetSecond());

	DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
	ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
	bool bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;
	
	if(bRes == false)
		PopLastLog();

    return bRes;
}

bool CNVItem::ReadFactoryHWSWMW(char* szHW5,char* szSW9,char* szMW2)
{
	bool bRes = false;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	bRes = Execute(ptRequest);
	if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
	{
		//strncpy_s(szMW2,2,(char*)(ReadResp.item_data + 84),1);
		memcpy (szMW2, (char*)(ReadResp.item_data + 84), 2);
		for(int i=0,j=0;i<4;i++)
		{
			BYTE byteTest = ReadResp.item_data[85 + i];
			BYTE byteTest1 = ReadResp.item_data[85 + i] >> 4;
			BYTE byteTest2 = ReadResp.item_data[85 + i] & 0x0F;
			szSW9[j++] = '0' + byteTest1;
			szSW9[j++] = '0' + byteTest2;
		}
		//strncpy_s(szHW5,5,(char*)(ReadResp.item_data + 89),_TRUNCATE);
		memcpy (szHW5, (char*)(ReadResp.item_data+89), 5);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteFactoryHWSWMW(char* szHW5,char* szSW9,char* szMW2)
{
	bool bRes = false;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	Execute(ptRequest);

	memcpy_s((char*)(ReadResp.item_data + 84),1,szMW2,1);
	for(int i=0,j=0;i<4;i++)
	{//e,g 0010 means 0.010
		ReadResp.item_data[85+i]=(unsigned char)((szSW9[j]-'0')<<4)+(unsigned char)(szSW9[j + 1]-'0');
		j+=2;
	}
	memcpy_s((char*)(ReadResp.item_data + 89),5,szHW5,5);//e,g 101A1

	DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
	ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
	bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ReadProcessInfo(char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation)
{
	bool bRes = false;
	
	int nProcessIndex = ::atoi(szProcessIndex);
	if(nProcessIndex <= 10 && nProcessIndex >= 1)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
		bRes = Execute(ptRequest);
		if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
		{
			bool bPass = false;
			BYTE byteMask;
			switch(nProcessIndex)
			{
			case 1:case 2:case 3:case 4:case 5: case 6:
				byteMask = 1 << (nProcessIndex-1);
				bPass = (*(ReadResp.item_data + 3) & byteMask) != 0;
				break;
			case 7:
				byteMask = 1 << 7;
				bPass = (*(ReadResp.item_data + 3) & byteMask) != 0;
				break;
			default:
				break;
			};

			if(bPass)
			{
				BYTE nDay = *(ReadResp.item_data + 5 + (nProcessIndex-1)*4);
				::sprintf_s(szDay,5,"%02d",nDay);
				BYTE nMonth = *(ReadResp.item_data + 6 + (nProcessIndex-1)*4);
				::sprintf_s(szMonth,5,"%02d",nMonth);
				BYTE nYear = *(ReadResp.item_data + 7 + (nProcessIndex-1)*4);
				::sprintf_s(szYear,5,"%02d",nYear);
				BYTE nLine = *(ReadResp.item_data + 8 + (nProcessIndex-1)*4);
				nLine = nLine & 0x0F;
				::sprintf_s(szLine,5,"%02d",nLine);
				BYTE nStation = *(ReadResp.item_data + 8 + (nProcessIndex-1)*4);
				nStation >>= 4;
				::sprintf_s(szStation,5,"%02d",nStation);
			}
			else
			{//If test byte is not be set, szYear will be empty
			}
			bRes = true;
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteProcessInfo(char* szProcessIndex)
{
	/*
	Bit	meaning
	1 (Process 1 )	DL
	2 (Process 2 )	2G cal&Pretest
	3 (Process 3 )	3G cal& Pretest
	4 (Process 4 )	Leakage&Charger, CIT/MMI
	5 (Process 5 )	FWT,WIRELESS
	6 (Process 6 )X	PPF --> SubSystem in Leopard
	7 (Process ? )X	ACM
	8 (Process 7 )	IMEI
	*/
	bool bRes = false;
	int nProcessIndex = ::atoi(szProcessIndex);
	if(nProcessIndex <= 10 && nProcessIndex >= 1)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
		Execute(ptRequest);

		BYTE byteMask;

		switch(nProcessIndex)
		{
		case 1:case 2:case 3:case 4:case 5: case 6:
			byteMask = 1 << (nProcessIndex-1);
			byteMask = *(ReadResp.item_data + 3) | byteMask;//For debug
			*(ReadResp.item_data + 3) = *(ReadResp.item_data + 3) | byteMask;
			break;
		case 7:
			byteMask = 1 << 7;
			*(ReadResp.item_data + 3) = *(ReadResp.item_data + 3) | byteMask;
			break;
		default:
			break;
		};

		CTime tCurrent = CTime::GetCurrentTime();
		*(ReadResp.item_data + 5 + (nProcessIndex-1)*4) = tCurrent.GetDay();
		*(ReadResp.item_data + 6 + (nProcessIndex-1)*4) = tCurrent.GetMonth();
		*(ReadResp.item_data + 7 + (nProcessIndex-1)*4) = tCurrent.GetYear() % 100;

		char szComputer[1024] = {0};
		DWORD dwSize = 1024;
		GetComputerNameA(szComputer,&dwSize);
		if(strlen(szComputer) == 6 && szComputer[0] == 'T' && szComputer[3] == 'D')
		{
			szComputer[3] = 0;
			szComputer[6] = 0;
			char* szT = szComputer + 1;
			char* szD = szComputer + 4;
			BYTE nTByte = atoi(szT);
			nTByte <<= 4;
			BYTE nDByte = atoi(szD);
			*(ReadResp.item_data + 8 + (nProcessIndex-1)*4) = nTByte | nDByte;
		}
		else
		{//Fake data T01D01
			*(ReadResp.item_data + 8 + (nProcessIndex-1)*4) = 0x11;
		}

		DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
		ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
		bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;
	}
	
	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ResetProcessInfo (char* szProcessIndex)
{
	bool bRes = false;
	int nProcessIndex = ::atoi(szProcessIndex);

	if(nProcessIndex <= 10 && nProcessIndex >= 1)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
		Execute(ptRequest);

		BYTE byteMask;

		switch(nProcessIndex)
		{
		case 1:case 2:case 3:case 4:case 5:case 6:
			byteMask = 1 << (nProcessIndex-1);
			*(ReadResp.item_data + 3) &= ~byteMask;
			break;
		case 7:
			byteMask = 1 << 7;
			*(ReadResp.item_data + 3) &= ~byteMask;
			break;
		default:
			break;
		};

		*(ReadResp.item_data + 5 + (nProcessIndex-1)*4) = 0; //Date
		*(ReadResp.item_data + 6 + (nProcessIndex-1)*4) = 0;
		*(ReadResp.item_data + 7 + (nProcessIndex-1)*4) = 0;

		*(ReadResp.item_data + 8 + (nProcessIndex-1)*4) = 0; //Computer Name

		DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
		ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
		bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;
	}


	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteTrackID(char* szTrackID,int nVer)
{
	return WritePicassoNumber(szTrackID);
}

bool CNVItem::ReadTrackID(char* szTrackID, int& nBufferSize,int nVer)
{
	return ReadPicassoNumber(szTrackID,nBufferSize);
}

bool CNVItem::ReadSUGNumber(char* szSUGNumber, int& nBufferSize,int nVer)
{
	bool bRes = false;
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	bRes = Execute(ptRequest);
	if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
	{
		::memcpy_s(szSUGNumber,nBufferSize,ReadResp.item_data + 104,10);
		if(nBufferSize > 10)
			szSUGNumber[10] = 0;
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteSUGNumber(char* szSUGNumber,int nVer)
{
	bool bRes = (strlen(szSUGNumber) == 10);
	
	if(bRes)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
		Execute(ptRequest);
		::memcpy_s(ReadResp.item_data + 104,10,szSUGNumber,10);
		DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
		ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
		bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ReadPicassoNumber(char* szPICASSO, int& nBufferSize)
{
	bool bRes = false;
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	bRes = Execute(ptRequest);
	if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
	{
		::memcpy_s(szPICASSO,nBufferSize,ReadResp.item_data + 94,10);
		if(nBufferSize > 10)
			szPICASSO[10] = 0;
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WritePicassoNumber(char* szPICASSO)
{
	bool bRes = false;
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_READ_F, ReadResp);
	Execute(ptRequest);
	::memcpy_s(ReadResp.item_data + 94,10,szPICASSO,10);
	DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
	ptRequest = new CNVRequest(NV_FACTORY_DATA_1_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
	bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CNVItem::ReadMMIFlag(char* szTestItem,char* sznResult)
{
	bool bRes = false;
	
	int nMMIItem = ::atoi(szTestItem);
	if(nMMIItem >= 384 || nMMIItem <= 431)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_4_I, DIAG_NV_READ_F, ReadResp);
		bRes = Execute(ptRequest);
		if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && ReadResp.nv_stat == 0)
		{
			if(ReadResp.item_data[nMMIItem - 384] == 1)
				::strcpy_s(sznResult,2,"1");
			else
				::strcpy_s(sznResult,2,"0");
			bRes = true;
		}
		else
		{
			bRes = false;
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteMMIFlag(char* szTestItem,char* sznResult)
{
	bool bRes = false;
	
	int nMMIItem = ::atoi(szTestItem);
	if(nMMIItem >= 384 || nMMIItem <= 431)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		CRequestPtr ptRequest = new CNVRequest(NV_FACTORY_DATA_4_I, DIAG_NV_READ_F, ReadResp);
		Execute(ptRequest);
		ReadResp.item_data[nMMIItem - 384] = ::atoi(sznResult);
		DIAG_NVITEM_PACKET_F_type ReadRespWrite = {0};
		ptRequest = new CNVRequest(NV_FACTORY_DATA_4_I, DIAG_NV_WRITE_F, ReadRespWrite, ReadResp.item_data, 128);
		bRes = Execute(ptRequest) && ReadRespWrite.nv_stat == 0;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp)
{
	bool bRes = false;
	bool bSuccess = false;
	CRequestPtr ptRequest = new CNVRequest(NVItem, DIAG_NV_READ_F, ReadResp,bSuccess);
	bRes = Execute(ptRequest);
	if (bRes && ReadResp.nDIAGcmd == DIAG_NV_READ_F && bSuccess)
	{
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, const BYTE *WriteData, const size_t nWriteData) 
{
	bool bRes = false;
	bool bSuccess = false;
	CRequestPtr ptRequest = new CNVRequest(NVItem, DIAG_NV_WRITE_F, ReadResp,bSuccess, WriteData, nWriteData);
	bRes = Execute(ptRequest) && bSuccess;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

TSTRING CNVItem::GetBTAddr()
{
	char szBTADDR100[12+1]  = {0};
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bool bRes = ReadNVItem(NV_BD_ADDR_I,ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		char sDigit[6*2+1] = {0};
		HexBcdToDigit(ReadResp.item_data, 6, sDigit, 6*2+1);		
		for(int i=0; i<12; i++)
			szBTADDR100[i] = sDigit[11-i];
	}
	return QSYNC_LIB::GetTSTRING(szBTADDR100);
}

bool CNVItem::WriteBTaddr(const string& strBTAddr)
{
	bool bRes = false;

	if(strBTAddr.size() >= 12)
	{
		char szReverseBTAddr[13] = {0};
		for(int i=0; i<12; i++)
		{
			szReverseBTAddr[i] = strBTAddr.at(i);
		}
		BYTE byteBTAddr[6]= {0};
		DigitToHexBcd(szReverseBTAddr, 13,byteBTAddr, 6);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_BD_ADDR_I,ReadResp,byteBTAddr,6))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}
TSTRING CNVItem::GetBTAddr_C7()
{
	char szBTADDR100[12+1]  = {0};
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bool bRes = ReadNVItem(NV_BD_ADDR_C7_I,ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
//		char sDigit[6*2+1] = {0};
//		HexBcdToDigit(ReadResp.item_data, 6, sDigit, 6*2+1);		
		for(int i=0; i<12; i++)
			szBTADDR100[i] = ReadResp.item_data[i];
	}
	return QSYNC_LIB::GetTSTRING(szBTADDR100);
}

/*20141022 */
bool CNVItem::GetNv_T47(char *szNV30002, char *szNV30003, char *szNV30004)
{
	char szNVItem[2]  = {0};
 
	int nNV30002[2] ;
	int nNV30003[2] ;
	int nNV30004[2] ;

	int nNV30002Index = 30002;
	int nNV30003Index = 30003;
	int nNV30004Index = 30004;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	bool bRes ;
	bRes = ReadNVItem(nNV30002Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30002, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30002[0] = ReadResp.item_data[0];
		nNV30002[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30003Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30003, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30003[0] = ReadResp.item_data[0];
		nNV30003[1] = ReadResp.item_data[1]; 
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30004Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30004, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30004[0] = ReadResp.item_data[0];
		nNV30004[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	// NV check mechanism: nv[30002] != nv[30003] == nv[30004]
	if ((nNV30002[0] != nNV30003[0]) || (nNV30002[1] != nNV30003[1])){
		if (! ((nNV30003[0] == nNV30004[0])&&(nNV30003[1] == nNV30004[1])) ){
			return false;
		}
	}else{
		return false;
	}

	return true;
}

/*20120703 */
bool CNVItem::GetNv_Detroit(char *szNV30015, char *szNV30016, char *szNV30017)
{
	char szNVItem[2]  = {0};
 
	int nNV30015[2] ;
	int nNV30016[2] ;
	int nNV30017[2] ;

	int nNV30015Index = 30015;
	int nNV30016Index = 30016;
	int nNV30017Index = 30017;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	bool bRes ;
	bRes = ReadNVItem(nNV30015Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30015, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30015[0] = ReadResp.item_data[0];
		nNV30015[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30016Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30016, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30016[0] = ReadResp.item_data[0];
		nNV30016[1] = ReadResp.item_data[1]; 
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30017Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30017, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30017[0] = ReadResp.item_data[0];
		nNV30017[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	// NV check mechanism: nv[30015] != nv[30016] == nv[30017]
	if ((nNV30015[0] != nNV30016[0]) || (nNV30015[1] != nNV30016[1])){
		if (! ((nNV30016[0] == nNV30017[0])&&(nNV30016[1] == nNV30017[1])) ){
			return false;
		}
	}else{
		return false;
	}

	return true;
}

bool CNVItem::GetNv_CheckSum(char *szNV30015, char *szNV30016, char *szNV30017)
{
	char szNVItem[2]  = {0};
 
	int nNV30015[2] ;
	int nNV30016[2] ;
	int nNV30017[2] ;

	int nNV30015Index = 30015;
	int nNV30016Index = 30016;
	int nNV30017Index = 30017;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};  

	bool bRes ;
	bRes = ReadNVItem(nNV30015Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30015, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30015[0] = ReadResp.item_data[0];
		nNV30015[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30016Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30016, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30016[0] = ReadResp.item_data[0];
		nNV30016[1] = ReadResp.item_data[1]; 
	}else{
		return false;
	}

	bRes = ReadNVItem(nNV30017Index, ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		sprintf(szNV30017, "%u,%u",  ReadResp.item_data[0], ReadResp.item_data[1]);
		nNV30017[0] = ReadResp.item_data[0];
		nNV30017[1] = ReadResp.item_data[1];
	}else{
		return false;
	}

	// NV check mechanism: nv[30015] != nv[30016]
	if ((nNV30015[0] == nNV30016[0]) && (nNV30015[1] == nNV30016[1])) return false;

	// NV check mechanism: nv[30017] > 0
	if ((nNV30017[0] <= 0) && (nNV30017[1] <= 0)) return false;

	return true;
}

bool CNVItem::WriteBTaddr_C7(const string& strBTAddr)
{
	bool bRes = false;

	if(strBTAddr.size() >= 12)
	{
		BYTE szReverseBTAddr[13] = {0};
		for(int i=0; i<12; i++)
		{
			szReverseBTAddr[i] = strBTAddr.at(i);
		}
//		BYTE byteBTAddr[6]= {0};
//		DigitToHexBcd(szReverseBTAddr, 13,byteBTAddr, 6);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_BD_ADDR_C7_I,ReadResp,szReverseBTAddr,12))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}


TSTRING CNVItem::GetWLANAddr()
{
	char szWLANADDR100[12+1]  = {0};
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bool bRes = ReadNVItem(NV_WLAN_MAC_ADDRESS_I,ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
		char sDigit[6*2+1] = {0};
		HexBcdToDigit(ReadResp.item_data, 6, sDigit, 6*2+1);		
		for(int i=0; i<12; i++)
			szWLANADDR100[i] = sDigit[11-i];
	}
	return QSYNC_LIB::GetTSTRING(szWLANADDR100);
}

bool CNVItem::WriteWLANaddr(const string& strWLANAddr)
{
	bool bRes = false;

	if(strWLANAddr.size() >= 12)
	{
		char szReverseWLANAddr[13] = {0};
		for(int i=0; i<12; i++)
		{
			szReverseWLANAddr[i] = strWLANAddr.at(i);
		}
		BYTE byteWLANAddr[6]= {0};
		DigitToHexBcd(szReverseWLANAddr, 13,byteWLANAddr, 6);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_WLAN_MAC_ADDRESS_I,ReadResp,byteWLANAddr,6))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::GetWLANAddr_C7()
{
	char szWLANADDR100[12+1]  = {0};
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bool bRes = ReadNVItem(NV_WLAN_MAC_ADDRESS_C7_I,ReadResp);	
	if(bRes && ReadResp.nv_stat==0)
	{
//		char sDigit[6*2+1] = {0};
//		HexBcdToDigit(ReadResp.item_data, 6, sDigit, 6*2+1);		
		for(int i=0; i<12; i++)
			szWLANADDR100[i] = ReadResp.item_data[i];
	}
	return QSYNC_LIB::GetTSTRING(szWLANADDR100);
}

bool CNVItem::WriteWLANaddr_C7(const string& strWLANAddr)
{
	bool bRes = false;

	if(strWLANAddr.size() >= 12)
	{
		BYTE szReverseWLANAddr[13] = {0};
		for(int i=0; i<12; i++)
		{
			szReverseWLANAddr[i] = strWLANAddr.at(i);
		}
//		BYTE byteWLANAddr[6]= {0};
//		DigitToHexBcd(szReverseWLANAddr, 13,byteWLANAddr, 6);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_WLAN_MAC_ADDRESS_C7_I,ReadResp,szReverseWLANAddr,12))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}
TSTRING CNVItem::GetIMEI()
{
	char szIMEI100[16]  = {0};
	DIAG_NVITEM_PACKET_F_type IMEIReadResp = {0};
	bool bIMEIRes = ReadNVItem(NV_UE_IMEI_I,IMEIReadResp);	
	if(bIMEIRes && IMEIReadResp.nv_stat==0)
	{
		char sDigitIMEI[128*2+1] = {0};
		HexBcdToDigit(IMEIReadResp.item_data, 128, sDigitIMEI, 128*2+1);		
		for(int i=0; i<15; i++)
			szIMEI100[i] = sDigitIMEI[i+3];
	}
	else if(bIMEIRes && IMEIReadResp.nv_stat ==5)
	{
		strcpy(szIMEI100, _T("000000011234560"));
	}
	return QSYNC_LIB::GetTSTRING(szIMEI100);
}

bool CNVItem::WriteIMEI(const string& strIMEI)
{
	bool bRes = false;

	if(strIMEI.size() >= 15)
	{
		char szReverseIMEI[19] = {0};
		szReverseIMEI[15] = 'A';
		szReverseIMEI[16] = '0';
		szReverseIMEI[17] = '8';
		for(int i=0; i<15; i++)
		{
			szReverseIMEI[14-i] = strIMEI.at(i);
		}
		BYTE byteIMEI[9]= {0};
		DigitToHexBcd(szReverseIMEI, 19,byteIMEI, 9);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_UE_IMEI_I,ReadResp,byteIMEI,9))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::GetMEID()
{
	char szMEID100[16]  = {0};
	DIAG_NVITEM_PACKET_F_type MEIDReadResp = {0};
	bool bIMEIRes = ReadNVItem(NV_MEID_I,MEIDReadResp);	
	if(bIMEIRes && MEIDReadResp.nv_stat==0)
	{
		char sDigitMEID[128*2+1] = {0};
		HexBcdToDigit(MEIDReadResp.item_data, 128, sDigitMEID, 128*2+1);		
		for(int i=0; i<14; i++)
			szMEID100[i] = sDigitMEID[13-i];
	}
	return QSYNC_LIB::GetTSTRING(szMEID100);
}

bool CNVItem::WriteMEID(const string& strMEID)
{
	bool bRes = false;

	if(strMEID.size() >= 14)
	{
		char szReverseMEID[15] = {0};
		for(int i=0; i<14; i++)
		{
			szReverseMEID[i] = strMEID.at(i);
		}
		BYTE byteMEID[7]= {0};
		DigitToHexBcd(szReverseMEID, 15,byteMEID, 7);

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_MEID_I,ReadResp,byteMEID,7))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::AustinGetServiceTag()
{
	char szTag[8]  = {0};
	DIAG_NVITEM_PACKET_F_type ServiceTagReadResp = {0};
#ifndef AUSTIN_NV_SERVICE_TAG
#define AUSTIN_NV_SERVICE_TAG   30011
#endif
	bool bIMEIRes = ReadNVItem(AUSTIN_NV_SERVICE_TAG,ServiceTagReadResp);	
	if(bIMEIRes && ServiceTagReadResp.nv_stat==0)
	{
		for(int i=0; i<7; i++)
			szTag[i] = ServiceTagReadResp.item_data[i];
	}
	return QSYNC_LIB::GetTSTRING(szTag);

}
bool CNVItem::AustinWriteServiceTag(const string& strServiceTag)
{
	bool bRes = false;

	if(strServiceTag.size() == 7)
	{
		BYTE szTag[8] = {0};
		for(int i=0; i<7; i++)
		{
			szTag[i] = strServiceTag.at(i) ;
		}
		
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
#ifndef AUSTIN_NV_SERVICE_TAG
#define AUSTIN_NV_SERVICE_TAG   30011
#endif
		if(WriteNVItem(AUSTIN_NV_SERVICE_TAG,ReadResp,szTag,7))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::AustinGetDRMKey()
{
	char szTag[401]  = {0};
	DIAG_NVITEM_PACKET_F_type DRMKeyReadResp = {0};
#ifndef AUSTIN_NV_DRM_KEY
#define AUSTIN_NV_DRM_KEY   30008
#endif
	bool bIMEIRes = ReadNVItem(AUSTIN_NV_DRM_KEY,DRMKeyReadResp);	
	if(bIMEIRes && (DRMKeyReadResp.nv_stat==0))
	{
		for(int i = 0; i < DRM_KEY_NV_SIZE; i++)
			szTag[i] = DRMKeyReadResp.item_data[i+1];

		bool bContinue = true;
		while(bContinue)
		{
			bIMEIRes = ReadNVItem(AUSTIN_NV_DRM_KEY,DRMKeyReadResp);	

			if(bIMEIRes && (DRMKeyReadResp.nv_stat==0) && (DRMKeyReadResp.item_data[0] != 0))
			{
				for(int i = 0; i < DRM_KEY_NV_SIZE; i++)
					szTag[DRMKeyReadResp.item_data[0]*40 + i] = DRMKeyReadResp.item_data[i+1];
			}
			else
				bContinue = false;
		}
	}

	return QSYNC_LIB::GetTSTRING(szTag);

}
bool CNVItem::AustinWriteDRMKey(const string& strDRMKey)
{
	bool bRes = true;
	BYTE szTag[42] = {0};

	int nSize = strDRMKey.size();
	
	if(nSize > 0 )
	{
		szTag[0] = 0;
		int i = 0, j = 1;
		for(; i< nSize && (bRes == true); i++, j++)
		{
			szTag[j] = strDRMKey.at(i);
			if( j == 40)
			{
				DIAG_NVITEM_PACKET_F_type ReadResp = {0};
#ifndef AUSTIN_NV_DRM_KEY
#define AUSTIN_NV_DRM_KEY   30008
#endif
				if(WriteNVItem(AUSTIN_NV_DRM_KEY,ReadResp,szTag,j+1))
				{
					if(ReadResp.nv_stat != NV_DONE_S)
					{
						bRes = false;
					}
				}
				j = 1;
				szTag[0]=szTag[0]+1;
			}	
		}

		if(j != 1)
		{
			DIAG_NVITEM_PACKET_F_type ReadResp = {0};
#ifndef AUSTIN_NV_DRM_KEY
#define AUSTIN_NV_DRM_KEY   30008
#endif
			if(WriteNVItem(AUSTIN_NV_DRM_KEY,ReadResp,szTag,j+1))
			{
				if(ReadResp.nv_stat != NV_DONE_S)
				{
					bRes = false;
				}
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::AustinGetBTAddr()
{
	char szTag[13]  = {0};
	DIAG_NVITEM_PACKET_F_type BTAddrReadResp = {0};
#ifndef AUSTIN_NV_BT_MAC
#define AUSTIN_NV_BT_MAC   30009
#endif
	bool bIMEIRes = ReadNVItem(AUSTIN_NV_BT_MAC,BTAddrReadResp);	
	if(bIMEIRes && BTAddrReadResp.nv_stat==0)
	{
		for(int i=0; i<12; i++)
			szTag[i] = BTAddrReadResp.item_data[i];
	}
	return QSYNC_LIB::GetTSTRING(szTag);

}
bool CNVItem::AustinWriteBTAddr(const string& strBTAddr)
{
	bool bRes = false;

	if(strBTAddr.size() == 12)
	{
		BYTE szTag[13] = {0};
		for(int i=0; i<12; i++)
		{
			szTag[i] = strBTAddr.at(i);
		}

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
#ifndef AUSTIN_NV_BT_MAC
#define AUSTIN_NV_BT_MAC   30009
#endif
		if(WriteNVItem(AUSTIN_NV_BT_MAC,ReadResp,szTag,12))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

TSTRING CNVItem::AustinGetWLANAddr()
{
	char szTag[13]  = {0};
	DIAG_NVITEM_PACKET_F_type WLANAddrReadResp = {0};
#ifndef AUSTIN_NV_WLAN_MAC
#define AUSTIN_NV_WLAN_MAC   30010
#endif
	bool bIMEIRes = ReadNVItem(AUSTIN_NV_WLAN_MAC,WLANAddrReadResp);	
	if(bIMEIRes && WLANAddrReadResp.nv_stat==0)
	{
		for(int i=0; i<12; i++)
			szTag[i] = WLANAddrReadResp.item_data[i];
	}
	return QSYNC_LIB::GetTSTRING(szTag);

}
bool CNVItem::AustinWriteWLANAddr(const string& strWLANAddr)
{
	bool bRes = false;

	if(strWLANAddr.size() == 12)
	{
		BYTE szTag[13] = {0};
		for(int i=0; i<12; i++)
		{
			szTag[i] = strWLANAddr.at(i) +'0';
		}

		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
#ifndef AUSTIN_NV_WLAN_MAC
#define AUSTIN_NV_WLAN_MAC   30010
#endif
		if(WriteNVItem(AUSTIN_NV_WLAN_MAC,ReadResp,szTag,12))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
		}
	}
	return bRes;
}

/***********************************************************************/
//nStatus == 0 means SPC lock
/***********************************************************************/
bool CNVItem::IsSPCLocked(long& nStatus)
{
	bool bRes = false;
	nStatus = 0;
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	if(ReadNVItem(NV_SEC_CODE_I,ReadResp))
	{
		if(ReadResp.nDIAGcmd == 0x26)
		{
			bool bIsDefault = true;
			for(int i=0; i<6 && bIsDefault; i++)
			{
				bIsDefault = ReadResp.item_data[i] == 0;
				if(bIsDefault == false)
					break;
			}
			if(bIsDefault)
				nStatus = 2;
		}
		else if(ReadResp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
		}	
		bRes = true;
	}
	//Surbrin add
	else
	{
		if(ReadResp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
			bRes = true;
		}
	}
	///////// /end/////////////

	//	bRes = true;//Always true, because false means SPC lock
	return bRes;
}

bool CNVItem::ProgramSPC(const char* szLock)
{
	bool bRes = false;
	if(szLock && strlen(szLock) >= 6)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		if(WriteNVItem(NV_SEC_CODE_I,ReadResp, (const uint8 *)szLock,6))
		{
			if(ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
			else assert(0);
		}
		else
			assert(0);
	}
	return bRes;
}

bool CNVItem::ReadPRL(BYTE* pPRLData, UINT& nPRLDataSize, BYTE &nStatus)
{
	CRequestPtr ptRequest = new CPRLRequest(pPRLData, nPRLDataSize, nStatus);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WritePRL(BYTE* pPRLData, UINT& nPRLDataSize, BYTE &nStatus)
{
	CRequestPtr ptRequest = new CPRLRequest(pPRLData, nPRLDataSize, nStatus, FALSE);	
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ClearActivationData(int nTimeout) 
{
	bool bSuccess = false;
	CRequestPtr ptRequest = new CClearActivationDataRequest(bSuccess,false);    
	bool bRes = Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();

	return bRes && bSuccess;
}

bool CNVItem::ClearActivationDataForLavernock(int nTimeout) 
{
	bool bSuccess = false;
	CRequestPtr ptRequest = new CClearActivationDataRequest(bSuccess,true);    
	bool bRes = Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();

	return bRes && bSuccess;
}

bool CNVItem::StandbyCurrentTest(int nTimeout) 
{
	CRequestPtr ptRequest = new CStandbyCurrentTest();
	ptRequest->SetTimeout(nTimeout);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::MasterClear(int nTimeout) 
{
	CRequestPtr ptRequest = new CMasterClear();
	ptRequest->SetTimeout(nTimeout);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::WriteQFuse(UINT chain, uint32 LSB, uint32 MSB, uint32 RegConf2, uint32 RegConf3, uint32 RegConf4, int nTimeout) 
{
	bool bRes = false;
	bool bIsFTM = false;
	CRequestPtr ptRequest = new CWriteQFuse(chain, LSB, MSB, RegConf2, RegConf3, RegConf4,bIsFTM);
	ptRequest->SetTimeout(nTimeout);
	if(Execute(ptRequest))
	{
		bRes = bIsFTM;
	}
	else		
		PopLastLog();
	return bRes;
}

bool CNVItem::ReadQFuseState(unsigned long& reg1, unsigned long& reg2, bool& bSuccess) 
{
	unsigned long dump3, dump4, dump5;
	return ReadQFuseState (&reg1, &reg2, &dump3, &dump4, &dump5, bSuccess);
}

bool CNVItem::ReadQFuseState (unsigned long* preg1, unsigned long* preg2, unsigned long* preg3, unsigned long* preg4, unsigned long* preg5, bool &bSuccess)
{
	bool bRes = false;
	bSuccess = false;

	CRequestPtr ptRequest = new CReadQFuseState (preg1, preg2, preg3, preg4, preg5, &bSuccess);
	if (Execute(ptRequest))
		bRes = bSuccess;
	else		
		PopLastLog();

	return bRes;
}

bool CNVItem::BTHeadsetChargingPower(bool IsOn, int nTimeout) 
{
	CRequestPtr ptRequest = new CBTHeadsetCharging(IsOn);
	ptRequest->SetTimeout(nTimeout);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::SearchBTDevice(int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CAutoCommand();
	ptRequest->SetTimeout(nTimeout);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::QuerySearchBTResult(int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CQueryAutoCommandResult(&bRes);
	ptRequest->SetTimeout(nTimeout);
	if(Execute(ptRequest) == false)
	{
		PopLastLog();
	}

	return bRes;
}

bool CNVItem::QueryBTPairCommand(const TSTRING& strBTAddress,int nTimeout) 
{
	bool bIsPaired = false;
	CRequestPtr ptRequest = new CQueryCommand(strBTAddress.c_str(),static_cast<int>(strBTAddress.size()),bIsPaired);
	ptRequest->SetTimeout(nTimeout);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bIsPaired;
}

bool CNVItem::CheckSIMCardExistence(int &nType, int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CCheckSIM(&nType);    
	bRes = Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CNVItem::CheckSIMCardExistence_ICE(int &nType, int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CCheckSIMICE(&nType);    
	bRes = Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();


	return bRes;
}


bool CNVItem::LineToNVItem(const CString& strLine, std::vector<CNV_ItemHolder>& vNVItem)
{
	bool bRes = false;

	if(!strLine.IsEmpty())
	{		
		int npos = 0;

		// Find the NV name using the delimiter ";"
		CString strToken = strLine.Tokenize(_T(";"),npos);
		if ( -1 != npos)
		{
			CNV_ItemHolder objCurNV;
			objCurNV.SetName(strToken);

			// Find the NV ID using the delimiter ";"
			strToken = strLine.Tokenize(_T(";"),npos);
			if ( -1 != npos)
			{
				objCurNV.SetNV_ID(strToken);
				WORD wNVid = atoi(strToken.GetBuffer());
				DIAG_NVITEM_PACKET_F_type ReadResp;
				if(ReadNVItem(wNVid,ReadResp) && ReadResp.nv_stat == 0)
				{
					objCurNV.SetData(ReadResp.item_data, 128);
					vNVItem.push_back(objCurNV);
					bRes = true;
				}

				if(bRes == false)
				{
					CString strFormat;
					strFormat.Format(_T("ReadNVItem Fail : NV ITEM == %d, State == %d"),wNVid,ReadResp.nv_stat);
					AddNVItemLog(strFormat.GetBuffer());
					strFormat.ReleaseBuffer();
					bRes = true;// Record the ID and keep going
				}
			}
		}
	}
	return bRes;
}



//************************************
// Method:    BackupBRT
// FullName:  CNVItem::BackupBRT
// Access:    public 
// Returns:   bool
// Parameter: const std::string & strOutputBRTFileName
// Parameter: const std::string & strInputCSVFile
// Purpose: Backup handset's NV to the file in BRT format on PC
//************************************
bool CNVItem::BackupBRT(const std::string& strOutputBRTFileName,const std::string& strInputCSVFile)
{
	CStdioFile csvFile;
	UINT nSuccess = 0;
	bool bRes = false;
	if( TRUE == csvFile.Open(strInputCSVFile.c_str(),CFile::shareDenyNone | CFile::modeRead) )
	{		
		CStringA strLine;
		csvFile.ReadString(strLine);

		std::vector<CNV_ItemHolder> vNVItem;
		while ( csvFile.ReadString(strLine) )
		{
			strLine.Trim();
			if(!strLine.IsEmpty() )
			{
				if(LineToNVItem(strLine, vNVItem))
					nSuccess++;                
			}
		}
		csvFile.Close();

		CFile oFile;
		if (vNVItem.size()>0 && TRUE == oFile.Open(strOutputBRTFileName.c_str(),CFile::modeCreate | CFile::modeWrite))
		{
			int nNVCount = static_cast<int>(vNVItem.size());
			oFile.Write(&nNVCount,sizeof(int));
			for (std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
			{
				int iVal = iter->GetNV_ID();
				oFile.Write(&iVal,sizeof(int)); //nv id
				iVal = iter->GetSize();
				oFile.Write(&iVal,sizeof(int)); //item size
				oFile.Write( iter->GetData(), iter->GetSize() ); //item data
			}
			oFile.Close();
			bRes = true;
		}
	}

	return bRes;
}

bool CNVItem::RestoreBRT(const std::string& strInBRTFileName)
{
    bool bRes = false;

    std::vector<CNV_ItemHolder> vNVItem;

    CFile oFile;
    if (oFile.Open(strInBRTFileName.c_str(),CFile::shareDenyNone| CFile::modeRead))
    {		
        int nvCount = 0;
        UINT nRead = oFile.Read(&nvCount,sizeof(int));
        while(nRead > 0)
        {
            CNV_ItemHolder NVHolder;

            int iVal = 0;
            nRead = oFile.Read(&iVal,sizeof(int));//nvid
            NVHolder.SetNV_ID(iVal);
            nRead = oFile.Read(&iVal,sizeof(int));
            NVHolder.SetSize(iVal);
            nRead = oFile.Read(NVHolder.GetData(),iVal);//data

            if(nRead == iVal && nRead > 0)
                vNVItem.push_back(NVHolder);
            else
                break;
        }
        bRes = true;
    }

    if(bRes)
    {
        for(std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
        {
            if (iter->GetSize() == 256)
            {
                DIAG_NVITEM_PACKET_F_type ReadResp = {0};
				ReadResp.nv_stat = 0x302F;
				unsigned char* pStart = iter->GetData();
                bRes = WriteNVItem(iter->GetNV_ID(),ReadResp,pStart,128);
				if(bRes && ReadResp.nv_stat == NV_DONE_S)
				{
					pStart += 128;
					ReadResp.nv_stat = 0x302F;
					bRes = WriteNVItem(iter->GetNV_ID(),ReadResp,pStart,128) && ReadResp.nv_stat == NV_DONE_S;
				}
            }
            else
            {
                DIAG_NVITEM_PACKET_F_type ReadResp = {0};
                if(WriteNVItem(iter->GetNV_ID(),ReadResp,iter->GetData(),iter->GetSize()))
                {
                    if(ReadResp.nv_stat == NV_DONE_S)
                        bRes = true;
                    else if(ReadResp.nv_stat == NV_BUSY_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_BADCMD_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_FULL_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_FAIL_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOTACTIVE_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_BADPARM_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_READONLY_S)
                        bRes = false;//Remember unlock SPC
                    else if(ReadResp.nv_stat == NV_BADTG_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOMEM_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOTALLOC_S)
                        bRes = false;
                    else
                        assert(0);

                    if(bRes == false)
                    {
                        CString strFormat;
                        strFormat.Format(_T("WriteNVItem Fail : NV ITEM == %d, State == %d"),iter->GetNV_ID(),ReadResp.nv_stat);
                        AddLog(strFormat.GetString());
                    }
                }
                else
                {
                    bRes = false;

                    CString strFormat;
                    strFormat.Format(_T("WriteNVItem Fail : NV ITEM == %d"),iter->GetNV_ID());
                    AddLog(strFormat.GetString());
                }
            }

            if(bRes == false)
                break;
        }
    }

    return bRes;
}

bool CNVItem::BackupNVPartition(int nTimeout) 
{
	bool bRes = false;
	bool bStatus = false;
	CRequestPtr ptRequest = new CBackupRestoreNV(1,bStatus);
	ptRequest->SetTimeout(nTimeout);
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::RestoreNVPartition(int nTimeout) 
{
	bool bRes = false;
	bool bStatus = false;	
	CRequestPtr ptRequest = new CBackupRestoreNV(2,bStatus);
	ptRequest->SetTimeout(nTimeout);
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::CheckNVPartition(int& nBackupTimes, CTime& LastBackupTime, int nTimeout) 
{
	bool bRes = false;
	bool bStatus = false;
	int nY,  nM,  nD, nH, nMin, nS;
	CRequestPtr ptRequest = new CBackupRestoreNV(3,bStatus,nBackupTimes, nY,  nM,  nD, nH, nMin, nS);
	if(Execute(ptRequest, nTimeout))
	{
		if(bStatus)
		{
			if(nBackupTimes > 0)
			{
				if(nY > 1000 && nY < 9999 && nM >= 1 && nM <= 12 && nD >= 1 && nD <= 31 && nH >= 1 && nH <= 23 && nMin >= 0 && nMin <= 59 )
				{
					CTime tempT(nY,  nM,  nD, nH, nMin, nS);
					LastBackupTime = tempT;
				}
			}
			bRes = true;
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ReadFactoryAreaNew(unsigned char* szFactoryBuf, int& nState, int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CReadFactoryArea(szFactoryBuf, &nState);
	ptRequest->SetTimeout(nTimeout);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::BTHandsetVoltageReading(long& nVoltage, int nTimeout) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CBTHandsetVoltageReading(&nVoltage);
	ptRequest->SetTimeout(nTimeout);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::OnLineMode() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new COnlineMode();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::FTMMode() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CFTMMode();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::SetOnline() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new COnlineMode();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CNVItem::SetOffline() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new COfflineMode();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CNVItem::ResetHandset() 
{
	bool bRes = SetOffline();
	if(bRes)
	{
		CRequestPtr ptRequest = new CResetHandset();
		bRes = Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CNVItem::SetFTM() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CFTMMode();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ChangeOnline() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CChangeOnline();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ChangeFTM() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CChangeFTM();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::CheckFTM(int& nFTMValue) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CReadFTMNV(&nFTMValue);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::EnableDiagEvent(bool bEnable)
{
	return CInterfaceBase::EnableDiagEvent(bEnable);
}

bool CNVItem::ReadBatteryTemperature(int& nTemperature) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CReadBatteryTemperature(nTemperature);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ReadBatteryVoltage(int& nVoltage) 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CReadBatteryVoltage(&nVoltage, bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::SendRawRequest(const BYTE* pReqData, const UINT nReqDataSize, BYTE* pRespData, UINT& nRespDataSize)
{
	bool bRes = false;
	CRequestPtr ptRequest = new CSendSync(pReqData, nReqDataSize, pRespData, nRespDataSize);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::PowerOff() 
{
	bool bRes = false;
	CRequestPtr ptRequest = new CPowerOff();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CNVItem::ProximitySensorTest() {
    bool bRes = false, bReturn = false;
    CRequestPtr pst = new CProximitySensorTestRequest(bReturn);
    bRes = Execute(pst);
    if(bRes)
        return bReturn;
    else
        return false;
}

CNVItem_API int CAD(int nPort, int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.ClearActivationData(nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int SCT(int nPort, int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.StandbyCurrentTest(nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int MC(int nPort, int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.MasterClear(nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int WQF(int nPort, unsigned int chain, unsigned long reg[5], int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.WriteQFuse(chain, reg[0], reg[1], reg[2], reg[3], reg[4], nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int BTCP(int nPort, int IsOn, int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.BTHeadsetChargingPower(IsOn != 0? true : false, nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int CSIMCE(int nPort, int& nType, int nTimeout) 
{
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.CheckSIMCardExistence(nType, nTimeout*2);
	return bRes ? 1 : 0;
}

CNVItem_API int RFA(int nPort, unsigned char* szFactoryBuf, int& nState, int nTimeout) {
	CNVItem nv(nPort);
	Sleep(500);
	bool bRes = nv.ReadFactoryAreaNew(szFactoryBuf, nState, nTimeout);
	return bRes ? 1 : 0;
}

CNVItem_API int BTHVR(int nPort, long& nVoltage, int nTimeout) 
{
	CNVItem nv(nPort);
	bool bRes = nv.BTHandsetVoltageReading(nVoltage, nTimeout);
	return bRes ? 1 : 0;
}

//
// ------------------------------------------------------------------
// 

CControlModeSet::CControlModeSet(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CControlModeSet::~CControlModeSet()
{
}

bool CControlModeSet::Restart(unsigned int nSleepTime)
{
	if (DiagControlMode(MODE_OFFLINE_D_F))
	{
		if (DiagControlMode(MODE_RESET_F))
			Sleep(nSleepTime);
		return true;
	}		
	return false;
}

bool CControlModeSet::DiagControlMode(const unsigned int mode)
{
	CRequestPtr ptRequest = new CControlMode(mode);
	return Execute(ptRequest);
}


//*******************************************************************************//
CCFCDiagCommand::CCFCDiagCommand(const TSTRING& strDevice) 
:CNVItem(strDevice)
,CLastError(strDevice)
{
}

CCFCDiagCommand::CCFCDiagCommand(int nCOMPort) 
:CNVItem(COMDeviceName(nCOMPort))
,CLastError(COMDeviceName(nCOMPort))
{

}

#ifdef UseMobileFinder
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#endif

CCFCDiagCommand::~CCFCDiagCommand()
{
#ifdef UseMobileFinder
	/*Because this project doesn't call any function of mobile finder, so the "smart static link" won't link mobile finder.
	Hence the event of comport won't be set or reset.
	So we call a function of mobile finder here.
	This is meaningless, it is just used to force link mobile finder.
	*/
	QSYNC_LIB::GetDeviceFinder();
#endif
}

bool CCFCDiagCommand::IsConnected()
{
	return CInterfaceBase::IsConnected() && IsConnectinOpen();
}
bool CCFCDiagCommand::IsConnectinOpen()
{
	bool bRes = false;
	if(!CInterfaceBase::IsConnected())
		SetCFCLastError(DIAG_ERROR_CONNECTION_FAIL);
	else
	{
		if( CInterfaceBase::GetPotocolType() != NONE)
			bRes = true;
		else
			SetCFCLastError(DIAG_ERROR_TXRX_FAIL);
	}
	
	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::IsTestMode(int nTimeOut)
{
	bool bRes = false;
	PLATFORM_TOOL::PotocolType type = CInterfaceBase::GetPotocolType();
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(ReadNVItem(NV_FTM_MODE_I, resp, nTimeOut))
	{
		if ( resp.item_data[0] == 1)
			bRes = true;
		else
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_INVALID_STATUS);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::IsEMU(int nTimeOut)
{
	bool bRes = false;
	bool bIsEMU = false;
	CRequestPtr ptRequest = new CIsEMURequest(bIsEMU);
	if(Execute(ptRequest, nTimeOut))
		bRes = bIsEMU;
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CCFCDiagCommand::IsDLMode()
{
	bool bRes = false;
	if( CInterfaceBase::GetPotocolType() == DL)
		bRes = true;
	return bRes;
}
bool CCFCDiagCommand::IsUnitOn()
{
	return CInterfaceBase::IsConnected();
}
bool CCFCDiagCommand::IsUnitOff()
{
	return !IsUnitOn();
}
bool CCFCDiagCommand::Reset(int nTimeOut, bool bWaitForRestart)
{
	bool bRes = false;
	if(Cmd_DIAG_CONTROL_F(MODE_POWER_OFF_F, nTimeOut))
	{
		if(bWaitForRestart)
		{	
			CReOpenComport ReopenAfterReset(this);
			bRes = ReopenAfterReset.ReOepnComport(20000, false);
		}
		else
			bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::Restart(int nTimeOut, bool bWaitForRestart)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type req = {0};
	req.nDIAGcmd = DIAG_NV_WRITE_F;
	req.item = NV_FTM_MODE_I;
	req.item_data[0] = 0; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
	if(WriteNVItem(req, nTimeOut))	
	{
		if(Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F, nTimeOut))
		{
			if(Cmd_DIAG_CONTROL_F(MODE_RESET_F, nTimeOut))
			{
				if(bWaitForRestart)
				{
					CReOpenComport ReopenAfterReset(this);
					bRes = ReopenAfterReset.ReOepnComport(20000, false, 80, 5000);
				}
				else
					bRes = true;
			}
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
#ifdef UseMobileFinder
bool CCFCDiagCommand::Restart_UsePNP(int nRestartWaitingTime , int nNVTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type req = {0};
	req.nDIAGcmd = DIAG_NV_WRITE_F;
	req.item = NV_FTM_MODE_I;
	req.item_data[0] = 0; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
	if(WriteNVItem(req, nNVTimeOut))	
	{
		if(Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F, nNVTimeOut))
		{
			if(Cmd_DIAG_CONTROL_F(MODE_RESET_F, nNVTimeOut))
			{
				CReOpenComport ReopenAfterReset(this);
				bRes = ReopenAfterReset.ReOepnComport(nRestartWaitingTime, true);
			}
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
#endif

bool CCFCDiagCommand::SetTestMode(int nTimeOut)
{
	bool bRes = false;
	if(!IsTestMode(nTimeOut))
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_FTM_MODE_I;
		req.item_data[0] = 1; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
		if(WriteNVItem(req, nTimeOut))	
		{
			if(Reset(nTimeOut))
			{
				bRes = true;
			}
		}
	}
	else
		bRes = true;

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::CloseConnection(int nTimeOut)
{
	bool bRes = false;
	bool bExcuteResult = false;
	CRequestPtr ptRequest = new CCloseConnectionRequest(bExcuteResult);
	if(Execute(ptRequest, nTimeOut))
	{
		bRes = bExcuteResult;
		if(bExcuteResult == false)
			SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
//SPC related
bool CCFCDiagCommand::IsSPCLocked(long& nStatus, int nTimeOut)
{
	bool bRes = false;
	nStatus = 0;
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(ReadNVItem(NV_SEC_CODE_I,resp, nTimeOut))
	{
		if(resp.nDIAGcmd == 0x26)
		{
			bool bIsDefault = true;
			for(int i=0; i<6 && bIsDefault; i++)
			{
				bIsDefault = resp.item_data[i] == 0;
			}
			if(bIsDefault)
				nStatus = 2;
		}
		bRes = true;
	}
	else
	{
		if(resp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
			bRes = true;
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadSPC(char* szLock6,long& nStatus/*0=unlocked, 1=locked, 2=default unlocked000000*/,int nTimeOut) // same as IsSPCLocked
{
	bool bRes = false;
	nStatus = 0;

	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(szLock6 && ReadNVItem(NV_SEC_CODE_I, resp, nTimeOut))
	{
		if(resp.nDIAGcmd == 0x26)
		{
			bool bIsDefault = true;
			for(int i=0; i<6; i++)
				szLock6[i] = resp.item_data[i];
			szLock6[6] = 0;
			for(int i=0; i<6 && bIsDefault; i++)
			{
				bIsDefault = resp.item_data[i] == 0;
			}
			if(bIsDefault)
				nStatus = 2;
			bRes = true; // if the unit is not locked, then we can read SPC
		}
		else if(resp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
		}
	}
	else
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::UnLockSPC(char* szLock, bool& bUnlockOK, int nTimeOut)
{
	bool bRes = false;
	if(szLock && strlen(szLock) >=6)
	{
		bool bExecuteResult = false;
		CRequestPtr ptRequest= new CUnlockSPCRequest(szLock,bUnlockOK, bExecuteResult);
		if (Execute(ptRequest, nTimeOut) )
		{
			if (bExecuteResult)
				bRes = true;
			else
				SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
		}
		else
			SetCFCLastError(DIAG_ERROR_TXRX_FAIL);
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ProgramSPC(char* szLock6, int nTimeOut)
{
	bool bRes = false;
	if(szLock6 && strlen(szLock6))
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_SEC_CODE_I;
		memcpy(req.item_data, szLock6, 6);

		if(WriteNVItem(req, nTimeOut))	
		{
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::LockSPC(int nTimeOut)
{
	bool bRes = false;
	char szLock7[7] = {0};
	long nStatus = 0;
	if(ReadSPC(szLock7, nStatus, nTimeOut) && (nStatus==0 || nStatus ==2) )
	{
		if(szLock7[0] != '0')
			szLock7[0] = '0';
		else
			szLock7[0] = '1';
		bool bUnlockOK = false;
		if(UnLockSPC(szLock7, bUnlockOK, nTimeOut))
			bRes = !bUnlockOK; // unlock should fail.
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

// sim lock related
bool CCFCDiagCommand::ReadIMEI(char* szIMEI100, int nTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(ReadNVItem(NV_UE_IMEI_I,resp, nTimeOut))
	{
		if(resp.nv_stat==0)
		{
			char sDigitIMEI[128*2+1] = {0};
			HexBcdToDigit(resp.item_data, 128, sDigitIMEI, 128*2+1);		
			for(int i=0; i<15; i++)
				szIMEI100[i] = sDigitIMEI[i+3];

			// if all numbers of IMEI are 0, return moto default IMEI 
			int nIMEI = 0;
			for(int i=0; i<15; i++)
			{
				if(szIMEI100[i] == '0')
					++nIMEI;
			}
			if(nIMEI == 15)
				sprintf_s(szIMEI100,16, "000000011234560");
			bRes =true;
		}
		else
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_INVALID_STATUS);
	}
	else
	{
		if(resp.nv_stat==NV_NOTACTIVE_S)
		{
			sprintf_s(szIMEI100,16, "000000011234560");
			bRes = true;
		}
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteIMEI(char* szIMEI, int nTimeOut)
{
	bool bRes = false;
	if(szIMEI)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_UE_IMEI_I;
		size_t nLength = strlen(szIMEI);
		char szReverseIMEI[19] = {0};
		szReverseIMEI[15] = 'A';
		szReverseIMEI[16] = '0';
		szReverseIMEI[17] = '8';
		for(int i=0; i<15; i++)
		{
			szReverseIMEI[14-i] = szIMEI[i];			
		}
		BYTE byteIMEI[9]= {0};
		DigitToHexBcd(szReverseIMEI, 19, req.item_data, 9);

		if(WriteNVItem(req, nTimeOut))	
		{
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadBTAddr(char* szAddress100, long& nLength, int nTimeOut)
{
	bool bRes = false;
	nLength = 0;
	if(szAddress100)
	{
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_BD_ADDR_I, resp, nTimeOut))
		{
			if(resp.nv_stat == NV_DONE_S)
			{
				HexBcdToDigit(resp.item_data, 6, szAddress100, 6*2+1);
				szAddress100 = _strrev(szAddress100);
				szAddress100[12] = 0;
				nLength = 12;

				bRes = true;
			}
		}
		else if(resp.nv_stat == 5)
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_NOTACTIVE);
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::WriteWLANAddr(char* szAddress, int nTimeOut)
{
	bool bRes = false;
	if(szAddress)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_WLAN_MAC_ADDRESS_I;
		size_t nLength = strlen(szAddress);
		if(nLength > 12)
		{
			szAddress[12]=0;
		}
		DigitToHexBcd(szAddress, 13, req.item_data, 6);
		if(WriteNVItem(req, nTimeOut))	
		{
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::WriteBTAddr(char* szAddress, int nTimeOut)
{
	bool bRes = false;
	if(szAddress)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_BD_ADDR_I;
		size_t nLength = strlen(szAddress);
		if(nLength > 12)
		{
			szAddress[12]=0;
		}
		DigitToHexBcd(szAddress, 13, req.item_data, 6);
		if(WriteNVItem(req, nTimeOut))	
		{
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::GenFSN(BYTE* szFSN16)
{
	bool bRes = false;
	if(szFSN16)
	{
		srand( (unsigned int)GetTickCount() );
		for(int i=0; i<8; i++)
		{
			int nFSN = rand();
			szFSN16[i*2]   =  nFSN & 0x00FF;
			szFSN16[i*2+1] = (nFSN & 0xFF00) >> 8;
		}
		bRes = true;
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadFSN(BYTE* szFSN16, int nTimeOut)
{
	bool bRes = false;
	if(szFSN16)
	{
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_DEVICE_SERIAL_NO_I, resp, nTimeOut))
		{
			memcpy(szFSN16, resp.item_data, 16);
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteFSN(BYTE* szFSN16, int nTimeOut)
{
	bool bRes = false;
	if(szFSN16)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		req.item = NV_DEVICE_SERIAL_NO_I;
		memcpy(req.item_data, szFSN16, 16);

		if(WriteNVItem(req, nTimeOut))	
		{
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

// Version related
bool CCFCDiagCommand::IsQfused(bool& bQfused, int nTimeOut)
{
	bool bRes = false;
	bQfused = true;
	unsigned int nReg1 = 0;
	unsigned int nReg2 = 0;
	if(ReadQfuse(nReg1, nReg2, nTimeOut))
	{
		if(nReg1 == 6 && nReg2 == 0)
			bQfused = false;
		bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteQfused(WORD chain_select, int config_register[5], int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/, int nTimeOut)
{
	bool bRes = false;
	bool bExecuteResult = false;
	CRequestPtr ptRequest= new CWriteQfuseRequest(chain_select, config_register, nQfusedStatus, bExecuteResult);
	if(Execute(ptRequest, nTimeOut) && bExecuteResult)
	{
		bRes = true;
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadQfuse(unsigned int& nReg1, unsigned int& nReg2, int nTimeOut)
{
	bool bRes = false;
	bool bExecuteResult = false;
	CRequestPtr ptRequest= new CReadQfuseRequest(nReg1, nReg2, bExecuteResult);
	if( Execute(ptRequest, nTimeOut) )
		bRes = bExecuteResult;
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CCFCDiagCommand::ReadINIFileName(char* szReadName1024, long& nLength, int nTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(ReadNVItem(NV_SYS_CUSTOM_INI_FILE_NAME_I, resp, nTimeOut)) 
	{
		if (resp.nv_stat == 0)
		{
			memcpy(szReadName1024, resp.item_data, 128);
			nLength = static_cast<int>(strlen(szReadName1024));
			bRes = true;
		}
		else
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_INVALID_STATUS);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadFlexVersion(char* szReadValue1024, long& nLength, int nTimeOut)
{
	bool bRes = false;
	nLength = 0;
	if(szReadValue1024)
	{
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_CFGI_PID_SYS_EFS_FILE_NAME, resp, nTimeOut)) //mobicon ini name
		{
			if(resp.nv_stat == 0)
			{
				memcpy(szReadValue1024, resp.item_data, 128);
				size_t nLen = strlen(szReadValue1024);
				if( szReadValue1024[nLen-7] == '.' && (szReadValue1024[nLen-6] == 'q' || szReadValue1024[nLen-6] == 'Q') 
					&& (szReadValue1024[nLen-5] == 'v' || szReadValue1024[nLen-5] == 'V') && (szReadValue1024[nLen-4] == 'c' || szReadValue1024[nLen-4] == 'C')
					&& (szReadValue1024[nLen-3] == 'e' || szReadValue1024[nLen-3] == 'E') && (szReadValue1024[nLen-2] == 'f' || szReadValue1024[nLen-2] == 'F')
					&& (szReadValue1024[nLen-1] == 's' || szReadValue1024[nLen-1] == 'S'))
				{
					for(int i=1; i<8; i++)
						szReadValue1024[nLen-i] = 0;
				}
				nLength = static_cast<int>(strlen(szReadValue1024));
				bRes = true;
			}
			else
				SetCFCLastError(DIAG_ERROR_READ_NVITEM_INVALID_STATUS);
		}
		else if(resp.nv_stat == 5)
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_NOTACTIVE);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadSWVersion(char* szReadValue1024, long& nLength, int nTimeOut)
{
	bool bRes = false;
	nLength = 0;
	if(szReadValue1024)
	{
		CRequestPtr ptRequest= new CReadSWVersionRequest(szReadValue1024, nLength);
		if(Execute(ptRequest, nTimeOut))
		{
			bRes = true;
		}
		else
			SetCFCLastError(DIAG_ERROR_TXRX_FAIL);
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}


// Factory Value Query
bool CCFCDiagCommand::ReadTransceiverKitNumber(char* szTKN100, int nTimeOut)
{ // 104-113 TRANCEIVER NO (10bytes)
	bool bRes = false;
	if(szTKN100)
	{
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, resp, nTimeOut))
		{
			BYTE szTKN[11] = {0};
			for(int i=104; i<114; i++)
				szTKN[i-104] = resp.item_data[i];
			strcpy_s(szTKN100,11, (char*)szTKN);
			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteTransceiverKitNumber(char* szTKN100, int nTimeOut)
{ // 104-113 TRANCEIVER NO (10bytes)
	bool bRes = false;
	if(szTKN100)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, req, nTimeOut))
		{
			size_t nLength = strlen(szTKN100);
			if(nLength == 10)
			{
				for(int i=104; i<114; i++)
					req.item_data[i] = szTKN100[i-104];

				if(WriteNVItem(req, nTimeOut))	
					bRes = true;
			}
			else
				SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
		}
		else
		{
			if(req.nv_stat==NV_NOTACTIVE_S)
			{
				memset(&req, 0, sizeof(req));
				req.item = NV_FACTORY_DATA_1_I;
				for(int i=104; i<114; i++)
					req.item_data[i] = szTKN100[i-104];

				if(WriteNVItem(req, nTimeOut))	
				{
					bRes = true;
				}
			}
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadTrackID(char* szTrackID, int nTimeOut)
{ // 94-103 PICASSO NO (10bytes)
	bool bRes = false;
	if(szTrackID)
	{
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, resp, nTimeOut))
		{
			BYTE szTID[11] = {0};
			for(int i=94; i<104; i++)
				szTID[i-94] = resp.item_data[i];
			strcpy_s(szTrackID,11, (char*)szTID);

			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteTrackID(char* szTrackID, int nTimeOut)
{ // 94-103 PICASSO NO (10bytes)
	bool bRes = false;
	if(szTrackID && strlen(szTrackID) ==10)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, req, nTimeOut))
		{	
			for(int i=94; i<104; i++)
				req.item_data[i] = szTrackID[i-94];

			if(WriteNVItem(req, nTimeOut))	
			{
				bRes = true;
			}
		}
		else
		{
			if(req.nv_stat==NV_NOTACTIVE_S)
			{
				memset(&req, 0, sizeof(req));
				req.item = NV_FACTORY_DATA_1_I;
				for(int i=94; i<104; i++)
					req.item_data[i] = szTrackID[i-94];
				if(WriteNVItem(req, nTimeOut))	
				{
					bRes = true;
				}
			}
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::ReadFactoryInforBlock(char* szBlock, long& nLength, int nTimeOut)
{// 3-4 Test-Byte
	bool bRes = false;
	if(szBlock)
	{
		memset(szBlock, 0 , sizeof(szBlock));
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, resp, nTimeOut))
		{
			BYTE szTemp = 0;
			szTemp= resp.item_data[3];
			char szBlockTmp[1*2+1] = {0};
			HexBcdToDigit(&szTemp, 1, szBlockTmp, 1*2+1);	
			szBlock[0] = szBlockTmp[1];
			szBlock[1] = szBlockTmp[0];
			szBlock[2] = '\0';
			nLength = (int)strlen(szBlock);

			bRes = true;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteFactoryInforBlock(char* szBlock, int nTimeOut)
{// 3-4 Test-Byte
	bool bRes = false;
	if(szBlock && strlen(szBlock)>=2)
	{
		DIAG_NVITEM_PACKET_F_type req = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, req, nTimeOut))
		{
			char szBlockTemp[2] = {0};
			for(int i=0; i<2; i++)
				szBlockTemp[i] = szBlock[i];
			BYTE byteTmp[2] = {0};
			DigitToHexBcd(szBlockTemp, 3, byteTmp, 1);

			req.nDIAGcmd = DIAG_NV_WRITE_F;
			req.item_data[3] = byteTmp[0];
			req.item_data[4] = 0;
			if(WriteNVItem(req, nTimeOut))	
			{
				bRes = true;
			}
		}
		else
		{
			if(req.nv_stat==NV_NOTACTIVE_S)
			{
				memset(&req, 0, sizeof(req));
				char szBlockTemp[2] = {0};
				for(int i=0; i<2; i++)
					szBlockTemp[i] = szBlock[i];
				BYTE byteTmp[2] = {0};
				DigitToHexBcd(szBlockTemp, 3, byteTmp, 1);

				req.item = NV_FACTORY_DATA_1_I;
				req.item_data[3] = byteTmp[0];
				req.item_data[4] = 0;
				if(WriteNVItem(req, nTimeOut))	
				{
					bRes = true;
				}
			}
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

// MMI setting
bool CCFCDiagCommand::TurnOnOffEM(bool bOpen, int nTimeOut)
{
	bool bRes = false;
	BYTE item_value = bOpen ? 1 : 0;
	if(/*Write1NVValue(NV_QISDA_EM__ENABLE_PHONE_ID_INFO_I,		item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_SWHW_INFO_CUSTOMER_I,  item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_SWHW_INFO_RD_I,		item_value, nTimeOut)
	   &&*/ Write1NVValue(NV_QISDA_EM__ENABLE_MFG_TEST_I,			item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_TEXT_LABEL_TEST_I,		item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_LCD_TEST_I,			item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_SLT_ALT_TEST_I,		item_value, nTimeOut)
	   //&& Write1NVValue(NV_QISDA_EM__ENABLE_LANGUAGE_SWITCH_I,		item_value, nTimeOut)
	   && Write1NVValue(NV_QISDA_EM__ENABLE_EFEM_I,				item_value, nTimeOut)
	   /*&& Write1NVValue(NV_QISDA_EM__ENABLE_OTHERS_I,				item_value, nTimeOut)*/)
	{
		bRes = true;
	}

	if (bRes)
	{
		DIAG_NVITEM_PACKET_F_type NVreq = {0};
		bRes = ReadNVItem(NV_QISDA_EM__ENABLE_MFG_TEST_I, NVreq, nTimeOut) && NVreq.item_data[0] == item_value;
	}
	if (bRes)
	{
		DIAG_NVITEM_PACKET_F_type NVreq = {0};
		bRes = ReadNVItem(NV_QISDA_EM__ENABLE_TEXT_LABEL_TEST_I, NVreq, nTimeOut) && NVreq.item_data[0] == item_value;
	}
	if (bRes)
	{
		DIAG_NVITEM_PACKET_F_type NVreq = {0};
		bRes = ReadNVItem(NV_QISDA_EM__ENABLE_LCD_TEST_I, NVreq, nTimeOut) && NVreq.item_data[0] == item_value;
	}
	if (bRes)
	{
		DIAG_NVITEM_PACKET_F_type NVreq = {0};
		bRes = ReadNVItem(NV_QISDA_EM__ENABLE_SLT_ALT_TEST_I, NVreq, nTimeOut) && NVreq.item_data[0] == item_value;
	}
	if (bRes)
	{
		DIAG_NVITEM_PACKET_F_type NVreq = {0};
		bRes = ReadNVItem(NV_QISDA_EM__ENABLE_EFEM_I, NVreq, nTimeOut) && NVreq.item_data[0] == item_value;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::GetRTC(bool& bExecuteResult,BYTE& nMonth, BYTE& nDay, WORD& nYear, BYTE& nHour, BYTE& nMin, BYTE& nSec,int nTimeOut)
{
	CRequestPtr ptRequest= new CGetPMICTimeRequest(bExecuteResult, 2/*24hr*/,nMonth,nDay,nYear,nHour,nMin,nSec);
	bool bRes = Execute(ptRequest, nTimeOut) ;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::SetRTC(const COleDateTime& time,int nTimeOut)
{
	bool bRes = false;

	bool bExecuteResult = false;
	CRequestPtr ptRequest= new CSetPMICTimeRequest(
		bExecuteResult
		, 2/*24hr*/
		,time.GetMonth()
		,time.GetDay()
		,time.GetYear()
		,time.GetHour()
		,time.GetMinute()
		,time.GetSecond()
		);
	bRes = Execute(ptRequest, nTimeOut) && bExecuteResult;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::SetRTC_For71M(int nTimeOut)
{
	bool bRes = false;
	bool bNeedRestart = false;
	if(!IsEMU(nTimeOut))
		bNeedRestart = !IsTestMode(nTimeOut);
	if(!bNeedRestart || SetTestMode(nTimeOut))
	{
		//1. Get current PC UTC time
		COleDateTime CurrentTime(COleDateTime::GetCurrentTime());
		COleDateTime time;
		QSYNC_LIB::ConvertTime(CurrentTime, time, false);

		//2. Set device time
		bool bExecuteResult = SetRTC(time,nTimeOut);
		if(bExecuteResult)
		{
			//?? for alarm ??
			if( Write1NVValue(NV_SECTIME_TIME_OFFSETS_I, 0, nTimeOut) && 
				//Write1NVValue(NV_TIME_TOD_OFFSET_I, 0, nTimeOut) &&
				Write1NVValue(NV_QISDA_SET__BLOCK_SA_I, 1, nTimeOut))
			{	
				if(bNeedRestart)
					bRes = Restart(nTimeOut);
				else
					bRes = true;
			}
		}
		else
			SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
	}	

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::MasterClear(int nTimeOut)
{
	bool bRes = false;
	if(Write1NVValue(NV_QISDA_SET_TOOL_MASTERCLEAR_ENABLED_I, 1, nTimeOut))
	{
		CReOpenComport ReopenAfterReset(this);
		bRes = ReopenAfterReset.ReOepnComport(60000, false); // need to tune sleep time
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, int nTimeOut)
{
	CRequestPtr ptRequest = new CNVRequest(NVItem, DIAG_NV_READ_F, ReadResp);
	bool bRes = Execute(ptRequest, nTimeOut);
	if (bRes)
	{
		if ( (ReadResp.nDIAGcmd != DIAG_NV_READ_F) || (ReadResp.nv_stat != NV_DONE_S) )
		{
			bRes = false;
			SetCFCLastError(DIAG_ERROR_READ_NVITEM_INVALID_STATUS);
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::WriteNVItem(DIAG_NVITEM_PACKET_F_type& WriteReq, int nTimeOut)
{
	bool bRes = false;
	WORD nNVStauts = 0; // query if writing NV is successful
	CRequestPtr ptRequest = new CWriteNVItemRequest(WriteReq, nNVStauts);
	bRes = Execute(ptRequest, nTimeOut);

	if(bRes)
	{
		switch(nNVStauts)
		{
		case NV_DONE_S :
			break;
		case NV_BUSY_S:
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED);
			break;
		case NV_BADCMD_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND);
			break;
		case NV_FULL_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY);
			break;
		case NV_FAIL_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL);
			break;
		case NV_NOTACTIVE_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE);
			break;
		case NV_BADPARM_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK);
			break;
		case NV_READONLY_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_READ_ONLY);
			break;
		case NV_BADTG_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED);
			break;
		case NV_NOMEM_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED);
			break;
		case NV_NOTALLOC_S: 
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE);
			break;
		default:	
			bRes = false;
			SetCFCLastError(DIAG_ERROR_WRITE_NVITEM_ERROR);
			break;
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::Write1NVValue(WORD item, BYTE item_value, int nTimeOut)
{
	bool bRes = false;

	DIAG_NVITEM_PACKET_F_type req = {0};
	req.nDIAGcmd = DIAG_NV_WRITE_F;
	req.item = item;
	req.item_data[0] = item_value; 
	if(WriteNVItem(req, nTimeOut))	
	{
		bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::Cmd_DIAG_CONTROL_F(int nMode, int nTimeOut)
{
	bool bRes = false;
	CRequestPtr ptRequest = new CControlMode(nMode);
	if(Execute(ptRequest, nTimeOut))
		bRes = true;
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CCFCDiagCommand::GetRestoreNVStatus(bool& bNVRestoreSuccess, bool& bCNVRestoreSuccess, int nTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type NVreq = {0};
	if(bRes = ReadNVItem(NV_RESTORE_6240_SUCCESS_I, NVreq, nTimeOut))
		bNVRestoreSuccess = (NVreq.item_data[0] == 1);
	else if(NVreq.nv_stat == NV_NOTACTIVE_S)
	{
		bRes = true;
		bNVRestoreSuccess = false;
	}
	else
		SetCFCLastError(DIAG_ERROR_GETRESTORESTATUS_FAIL);

	DIAG_NVITEM_PACKET_F_type CNVreq = {0};
	if (bRes)
	{
		if(bRes = ReadNVItem(NV_RESTORE_6240_CNV_SUCCESS_I, CNVreq, nTimeOut))
			bCNVRestoreSuccess = (CNVreq.item_data[0] == 1);
		else if(CNVreq.nv_stat == NV_NOTACTIVE_S)
		{
			bRes = true;
			bCNVRestoreSuccess = false;
		}
		else
			SetCFCLastError(DIAG_ERROR_GETRESTORESTATUS_FAIL);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCDiagCommand::GetMelosRestoreNVStatus(bool& bNVRestoreSuccess, bool& bCNVRestoreSuccess, int nTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type NVreq = {0};
	if(bRes = ReadNVItem(NV_RESTORE_6240_SUCCESS_I, NVreq, nTimeOut))
		bNVRestoreSuccess = (NVreq.item_data[0] == 1);
	else if(NVreq.nv_stat == NV_NOTACTIVE_S)
	{
		bRes = true;
		bNVRestoreSuccess = false;
	}
	else
		SetCFCLastError(DIAG_ERROR_GETRESTORESTATUS_FAIL);

	DIAG_NVITEM_PACKET_F_type CNVreq = {0};
	if (bRes)
	{
		if(bRes = ReadNVItem(NV_RESTORE_6240_CNV_SUCCESS_I, CNVreq, nTimeOut))
			bCNVRestoreSuccess = (CNVreq.item_data[0] == 1);
		else if(CNVreq.nv_stat == NV_NOTACTIVE_S)
		{
			bRes = true;
			bCNVRestoreSuccess = false;
		}
		else
			SetCFCLastError(DIAG_ERROR_GETRESTORESTATUS_FAIL);
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}




bool CCFCDiagCommand::ReopenComPort(int nWaitingTime, bool bUsePNP, int nRetryTimes, int nDelayTime)
{
	bool bRes = false;
	CReOpenComport reopen(this, false);
	bRes = reopen.ReOepnComport(nWaitingTime, bUsePNP, nRetryTimes, nDelayTime);
	return bRes;
}
bool CNVItem::C7FreezeOTP(int nRequestType, int& nStatus)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	bool bSuccess = false;
	
	CRequestPtr ptRequest = new CC7FreezeOTPRequest(nRequestType, bSuccess,nStatus);	
	
	if(Execute(ptRequest))
	{
		bRes = bSuccess;
	}
	
	return bRes;
}
