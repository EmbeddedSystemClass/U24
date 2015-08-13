#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\EFsRequest.h"
#include "..\QisdaEFsModule\EFs.h"
// add for orange [7/26/2011 Kevin.Dong]
bool CQcomDiagModule::ReadHWID(char* szHWID)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;

	CRequestPtr ptRequest = new CReadHWIDRequestForOrange(szHWID, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::ReadFA(char* szOffset,char* szFA)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;

	CRequestPtr ptRequest = new CReadFARequestForOrange(szOffset,szFA, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}

bool CQcomDiagModule::ReadFA_Sector(char* szOffset,char* szFA,char* szSectorSize)
{
	using namespace PLATFORM_TOOL;
	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CReadFASectorRequestForOrange(szOffset,szFA,szSectorSize,bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}
	return bSuccess;
}
bool CQcomDiagModule::WriteFA(char* szOffset,char* szData)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CWriteFARequestForOrange(szOffset,szData, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}
	return bSuccess;
}
bool CQcomDiagModule::WriteFA_Sector(char* szOffset,char* szFA,char* szSectorSize)
{
	using namespace PLATFORM_TOOL;
	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CWriteFASectorRequestForOrange(szOffset,szFA,szSectorSize,bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}

bool CQcomDiagModule::CheckSWVersion(char* szData)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CCheckSWVersionForOrange(szData, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}

bool CQcomDiagModule::CoinCharge(bool bFlag)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CCoinChargeForOrange(bFlag, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}
bool CQcomDiagModule::BatteryCharge(bool bFlag)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CBatteryChargeForOrange(bFlag, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}

bool CQcomDiagModule::FileSystemBackup(char* szUserData)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CFileSystemBackupForOrange(bSuccess,szUserData);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;

}
//add read coin cell voltage for Cairo[5/31/2012 Kevin.Dong]
bool CQcomDiagModule::ReadCoinCellVoltage(char* szVoltage)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;

	CRequestPtr ptRequest = new CReadCoinCellVoltageForCairo(szVoltage, bSuccess);
	bRes = Execute (ptRequest);
	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}
/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::EFs_GoldenCopy()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	
	CRequestPtr ptRequest = new CEFsGoldenCopyRequest (bSuccess);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::EFS_SyncNV_NoWait(DWORD &token)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	
	CRequestPtr ptRequest = new CEFsSyncNVNOWaitRequest (token, bSuccess);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::EFS_Sync_Status(DWORD token)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	
	CRequestPtr ptRequest = new CEFsSyncNVGetStatusRequest (token, bSuccess);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::EFs_BackupCheck()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;

	CRequestPtr ptRequest = new CEFsBackupCheckRequest (bSuccess);
	bRes = Execute (ptRequest) && bSuccess;


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}


bool CQcomDiagModule::EFs_GoldenCopyCheck(int *status)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false; 
	bool bSuccess = false;

	CRequestPtr ptRequest = new CGoldenCopyCheckRequest (bSuccess, *status);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}



/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool EFs_GoldenCopy_SPU(char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.EFs_GoldenCopy();

	return ret;
}

QCOM_DIAG_API bool EFS_DeleteFile_SPU(char*szPort, char *sFileDst)
{
	long iErrorCode = 0;

	CEFs efs(::atoi(szPort));
	return efs.EfsDeleteFile (sFileDst, iErrorCode);
}

QCOM_DIAG_API bool EFS_SyncNV_NoWait_SPU(char*szPort, char *szToken, char *szRetryTime)
{
	CQcomDiagModule diag (atoi(szPort));
	int Rty = atoi(szRetryTime);
	DWORD dwToken(0);
	
	while(Rty-- > 0){
		if(diag.EFS_SyncNV_NoWait(dwToken)){
			sprintf_s(szToken, 256, "%d", dwToken);
			return true;
		}

		::Sleep(200);
	}

	return false;
}

QCOM_DIAG_API bool EFS_Sync_Status_SPU(char*szPort, char *szToken, char *szRetryTime)
{
	CQcomDiagModule diag (atoi(szPort));
	int Rty = atoi(szRetryTime);
	DWORD dwToken(atoi(szToken));
	
	while(Rty-- > 0){
		if(diag.EFS_Sync_Status(dwToken)){
			return true;
		}

		::Sleep(200);
	}

	return false;
}

QCOM_DIAG_API bool EFS_Sync_Combo_SPU(char *szPort, char *szRetryTime)
{
	CQcomDiagModule diag (atoi(szPort));
	int Rty = atoi(szRetryTime), Rty2 = Rty;
	DWORD dwToken(0);
	
	while(Rty-- > 0){
		if(diag.EFS_SyncNV_NoWait(dwToken)){
			::Sleep(500);
			
			while(Rty2-- > 0){
				if(diag.EFS_Sync_Status(dwToken)){
					return true;
				}

				::Sleep(500);
			}

			return false;
		}
	}
	return false;
}

// add for Orange [7/26/2011 Kevin.Dong]
QCOM_DIAG_API bool ReadHWID_SPU(char* szPort,char* szHWID)
{
  CQcomDiagModule diag (atoi(szPort));
  if(!diag.ReadHWID(szHWID))
  {
	  return false;
  }
  return true;
}

QCOM_DIAG_API bool ReadFA_SPU(char* szPort,char* szOffset,char* szFA)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.ReadFA(szOffset,szFA))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool WriteFA_SPU(char* szPort,char* szOffset,char* szData)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.WriteFA(szOffset,szData))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool CheckSWVersion_SPU(char* szPort,char* szData)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.CheckSWVersion(szData))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool CoinCharge_SPU(char* szPort,char* bFlag)
{ 
	bool bEnable= false;
	if(bFlag[0] == '1')
	{
		bEnable = true;
	}
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.CoinCharge(bEnable))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool BatteryCharge_SPU(char* szPort,char* bFlag)
{
	bool bEnable= false;
	if(bFlag[0] == '1')
	{
		bEnable = true;
	}
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.BatteryCharge(bEnable))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool FileSystemBackup_SPU(char* szPort,char* szUserData)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.FileSystemBackup(szUserData))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool ReadFA_Sector(char* szPort,char* szOffset,char* szFA,char* szSectorSize)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.ReadFA_Sector(szOffset,szFA,szSectorSize))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool WriteFA_Sector(char* szPort,char* szOffset,char* szFA,char* szSectorSize)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.WriteFA_Sector(szOffset,szFA,szSectorSize))
	{
		return false;
	}
	return true;
}

QCOM_DIAG_API bool ReadCoinCellVoltage_SPU(char* szPort,char* szVoltage)
{
	CQcomDiagModule diag (atoi(szPort));
	if(!diag.ReadCoinCellVoltage(szVoltage))
	{
		return false;
	}
	return true;
}


QCOM_DIAG_API bool EFs_CheckBackup_SPU(char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.EFs_BackupCheck();

	return ret;
}

QCOM_DIAG_API bool EFs_GoldenCopyCheck_SPU (char* szPort, char* szStatus)
{
	int nStatus;

	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.EFs_GoldenCopyCheck(&nStatus);
	wsprintf (szStatus, "%d", nStatus);

	return ret;
}
