#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\PWPRequest.h"
using namespace QSYNC_LIB;


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
//	PWP is short for "Permanent Write Protection"
//	a.The phone should be switched to online mode.
//	b.Diag port should be enumerated.
//
//	Caution:
//	There are 4 chances to enable write protection with the new PWP data.
//
//	Notice:
//	AMSS is not able to access the storage anymore.
//	Only WM can access eMMC directly.
//
//
// Procedure:
// 1.Initial PWP(Wolf_Ini_PWPData with Op = 0)
// 2.Write Device Serial number(Wolf_WriteSerialNumber)
// 3.Write public key(Wolf_WritePublicKey with public key file)
// 4.Close PWP(Wolf_Ini_PWPData with Op = 1)
// 5.Verify PWP(Wolf_Ini_PWPData with Op = 2)
// 6.Read Device Serial number(Wolf_ReadSerialNumber)
// 7.Read public key(Wolf_ReadPublicKey)
/*****************************************************************************/
bool CQcomDiagModule::Wolf_Ini_PWPData(BYTE Op)
{

	using namespace PLATFORM_TOOL ;

	bool bRes = false ;

	CRequestPtr ptRequest = new CIniPWPRequest(bRes, Op) ;

	bRes = Execute(ptRequest) && bRes ;

	if (! bRes)
		PopLastLog() ;

	return bRes ;
}
bool CQcomDiagModule::Wolf_WriteSerialNumber(char* szDevNum)
{
	using namespace PLATFORM_TOOL ;

	bool bRes = false ;

	BYTE *DevNum = NULL;
	size_t dLen = 0;

	HexStringToByteArray(szDevNum, DevNum, dLen);

	CRequestPtr ptRequest = new CWolfWriteDevNumRequest(bRes, DevNum) ;

	bRes = Execute(ptRequest) && bRes ;

	if (! bRes)
		PopLastLog() ;

	delete [] DevNum;

	return bRes;
}

bool CQcomDiagModule::Wolf_ReadSerialNumber(char* szDevNumber)
{
	using namespace PLATFORM_TOOL ;

	bool bResult, bRes = false ;

	CRequestPtr ptRequest = new CWolfReadDevNumRequest(szDevNumber, bResult) ;

	if (Execute (ptRequest))
	{
		bRes = bResult ;
	}

	if (! bRes)
		PopLastLog() ;

	return bRes ;
}

bool CQcomDiagModule::Wolf_WritePublicKey(const char* szFileSrc)
{
	using namespace PLATFORM_TOOL ;

	/*	20110825 Mark, Dave.Wu. Now get key data from Sim data provided by 3S
	ifstream inFile ;
	inFile.open(szFileSrc) ;

	std::string s, szStr ;
	getline(inFile,s) ;

	int xx = s.rfind("<Modulus>") ;

	xx+=9;
	int yy=s.rfind("</Modulus>") ;
	szStr=s.substr(xx,(yy-xx)) ;

	BYTE PublicKey[128] ;
	int DesLen = 128 ;
	::Base64Decode(szStr.c_str(),szStr.length(),PublicKey,&DesLen) ;
	*/

	//Decode whole sim data
	bool bRes = false ;

	BYTE* SimDataBuf = NULL;
	size_t nDecLen = 0;
	bRes = HexStringToByteArray(szFileSrc, SimDataBuf, nDecLen);

	if(nDecLen < 128 || NULL == SimDataBuf)
	{
		bRes = false;
	}

	if(bRes)
	{
		//Get first 128 bytes data, i.e. public key
		BYTE PublicKey[128] = {0};
		for(int i = 0;i < 128;i++)
		{
			PublicKey[i] = SimDataBuf[i];
		}

	CRequestPtr ptRequest = new CWolfWritePublicKeyRequest(bRes, PublicKey) ;

	bRes = Execute(ptRequest) && bRes ;

	if (! bRes)
		PopLastLog() ;
	}

	if(NULL != SimDataBuf)
		delete [] SimDataBuf;

	return bRes;
}

bool CQcomDiagModule::Wolf_ReadPublicKey(char* szPubKey)
{
	using namespace PLATFORM_TOOL ;

	bool bResult, bRes = false ;

	CRequestPtr ptRequest = new CWolfReadPublicKeyRequest (szPubKey, bResult) ;

	if (Execute (ptRequest))
	{
		bRes = bResult ;
	}

	if (! bRes)
		PopLastLog() ;

	return bRes ;
}

/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool IniPWP_SPU(const char* szPort, char* szOperation)
{
	CQcomDiagModule diag (atoi(szPort)) ;
	return diag.Wolf_Ini_PWPData (atoi(szOperation)) ;
}

QCOM_DIAG_API bool Write_Dev_Num_SPU(const char* szPort, char* szDevNum)
{
	CQcomDiagModule diag (atoi(szPort)) ;
	return diag.Wolf_WriteSerialNumber(szDevNum) ;
}

QCOM_DIAG_API bool Read_Dev_Num_SPU(const char* szPort, char* szDevNum)
{
	CQcomDiagModule diag (atoi(szPort)) ;
	return diag.Wolf_ReadSerialNumber(szDevNum);
}

QCOM_DIAG_API bool Write_Public_Key_SPU(const char* szPort, const char* szFileSrc)
{
	CQcomDiagModule diag (atoi(szPort)) ;
	return diag.Wolf_WritePublicKey(szFileSrc) ;
}

QCOM_DIAG_API bool Read_Public_Key_SPU(const char* szPort, char* szPubKey)
{
	CQcomDiagModule diag (atoi(szPort)) ;
	return diag.Wolf_ReadPublicKey(szPubKey) ;
}