#include "StdAfx.h"
#include "SIMEncoder_API.h"

#pragma warning (push)
#pragma warning (disable : 4996)

HMODULE SIMEncoder_Function::m_hEncoderModule = NULL;
void *SIMEncoder_Function::m_hSimEncHandle = NULL;

SIMEncoder_Function::SIMEncoder_Function()
{
	m_hEncoderModule = NULL;

}
SIMEncoder_Function::~SIMEncoder_Function()
{
	if (m_hEncoderModule != NULL)
	{
		FreeLibrary(m_hEncoderModule);
		m_hEncoderModule = NULL;
	}
}

bool SIMEncoder_Function::Initial_SIMEncoder(char* szErrMsg)
{
	if (m_hEncoderModule != NULL)
	{
		FreeLibrary(m_hEncoderModule);
		m_hEncoderModule = NULL;
	}

	if (m_hEncoderModule == NULL)
	{
		m_hEncoderModule = ::LoadLibrary("SimEncorder.dll");

		if (NULL == m_hEncoderModule)
		{
			strcpy(szErrMsg, "m_hEncoderModule == NULL");
			return false;
		}
	}

	return true;
}

bool SIMEncoder_Function::ISimEnc_AllocSimEncObj(int nSimTypeId, char* szErrMsg)
{
	pISimEnc_AllocSimEncObj ISimEnc_AllocSimEncObj = (pISimEnc_AllocSimEncObj)GetProcAddress(m_hEncoderModule, "ISimEnc_AllocSimEncObj");

	if (!ISimEnc_AllocSimEncObj)
	{
		strcpy(szErrMsg, "ISimEnc_AllocSimEncObj API is NULL");
		return false;
	}
	if (ISimEnc_AllocSimEncObj(nSimTypeId, m_hSimEncHandle) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Allocate SimEncObj fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_FreeSimEncObj()
{
	pISimEnc_FreeSimEncObj ISimEnc_FreeSimEncObj = (pISimEnc_FreeSimEncObj)GetProcAddress(m_hEncoderModule, "ISimEnc_FreeSimEncObj");

	if (!ISimEnc_FreeSimEncObj)
	{
		return false;
	}
	if (ISimEnc_FreeSimEncObj(m_hSimEncHandle) != 0)	return false;

	return true;
}

bool SIMEncoder_Function::ISimEnc_EnableTextLog(bool bEnableTextLog, char *szTextLogFilePath, char* szErrMsg)
{
	pISimEnc_EnableTextLog ISimEnc_EnableTextLog = (pISimEnc_EnableTextLog)GetProcAddress(m_hEncoderModule, "ISimEnc_EnableTextLog");

	if (!ISimEnc_EnableTextLog)
	{		
		strcpy(szErrMsg, "ISimEnc_EnableTextLog API is NULL");
		return false;
	}
	if (ISimEnc_EnableTextLog(m_hSimEncHandle, bEnableTextLog, szTextLogFilePath) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Enable TextLog fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_SetImei(char *szImei, int nImeiLen, char* szErrMsg)
{
	pISimEnc_SetImei ISimEnc_SetImei = (pISimEnc_SetImei)GetProcAddress(m_hEncoderModule, "ISimEnc_SetImei");

	if (!ISimEnc_SetImei)
	{		
		strcpy(szErrMsg, "ISimEnc_SetImei API is NULL");
		return false;
	}
	if (ISimEnc_SetImei(m_hSimEncHandle, szImei, nImeiLen) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Set IMEI fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_SetFlashSerialNumber(char *szFsn, int nFsnLen, char* szErrMsg)
{
	pISimEnc_SetFlashSerialNumber ISimEnc_SetFlashSerialNumber = (pISimEnc_SetFlashSerialNumber)GetProcAddress(m_hEncoderModule, "ISimEnc_SetFlashSerialNumber");

	if (!ISimEnc_SetFlashSerialNumber)
	{		
		strcpy(szErrMsg, "ISimEnc_SetFlashSerialNumber API is NULL");
		return false;
	}
	if (ISimEnc_SetFlashSerialNumber(m_hSimEncHandle, szFsn, nFsnLen) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Set Flash Serial Number fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_SetSimLockFile(char *szSimFilePath, char* szErrMsg)
{
	pISimEnc_SetSimLockFile ISimEnc_SetSimLockFile = (pISimEnc_SetSimLockFile)GetProcAddress(m_hEncoderModule, "ISimEnc_SetSimLockFile");

	if (!ISimEnc_SetSimLockFile)
	{		
		strcpy(szErrMsg, "ISimEnc_SetSimLockFile API is NULL");
		return false;
	}
	if (ISimEnc_SetSimLockFile(m_hSimEncHandle, szSimFilePath) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Set SIM_Lock File fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_SetUnlockCodes(T_SIM_UNLOCK_CODE *ptSimUnlockCode, char* szErrMsg)
{
	pISimEnc_SetUnlockCodes ISimEnc_SetUnlockCodes = (pISimEnc_SetUnlockCodes)GetProcAddress(m_hEncoderModule, "ISimEnc_SetUnlockCodes");

	if (!ISimEnc_SetUnlockCodes)
	{		
		strcpy(szErrMsg, "ISimEnc_SetUnlockCodes API is NULL");
		return false;
	}
	if (ISimEnc_SetUnlockCodes(m_hSimEncHandle, ptSimUnlockCode) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Set Unlock Codes fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_SetRSAKey(T_RSA *ptRSAKey, char* szErrMsg)
{
	pISimEnc_SetRSAKey ISimEnc_SetRSAKey = (pISimEnc_SetRSAKey)GetProcAddress(m_hEncoderModule, "_ISimEnc_SetRSAKey@8");

	if (!ISimEnc_SetRSAKey)
	{		
		strcpy(szErrMsg, "ISimEnc_SetRSAKey API is NULL");
		return false;
	}
	if (ISimEnc_SetRSAKey(m_hSimEncHandle, ptRSAKey) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Set RSA Key fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_GenSimLockData(char *pcSimLockData, int *pnSimLockDataLen, char* szErrMsg)
{
	pISimEnc_GenSimLockData ISimEnc_GenSimLockData = (pISimEnc_GenSimLockData)GetProcAddress(m_hEncoderModule, "ISimEnc_GenSimLockData");

	if (!ISimEnc_GenSimLockData)
	{		
		strcpy(szErrMsg, "ISimEnc_GenSimLockData API is NULL");
		return false;
	}
	if (ISimEnc_GenSimLockData(m_hSimEncHandle, (BYTE*)pcSimLockData, pnSimLockDataLen) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Generate SIM_Lock Data fail");
		return false;
	}
	return true;
}

bool SIMEncoder_Function::ISimEnc_CheckSimLockData(char *pcSimLockData, int nSimLockDataLen, char* szErrMsg)
{
	pISimEnc_CheckSimLockData ISimEnc_CheckSimLockData = (pISimEnc_CheckSimLockData)GetProcAddress(m_hEncoderModule, "ISimEnc_CheckSimLockData");

	if (!ISimEnc_CheckSimLockData)
	{		
		strcpy(szErrMsg, "ISimEnc_CheckSimLockData API is NULL");
		return false;
	}
	if (ISimEnc_CheckSimLockData(m_hSimEncHandle, (BYTE*)pcSimLockData, nSimLockDataLen) != 0)
	{
		strcpy(szErrMsg, "ISimEnc Check SIM_Lock Data fail");
		return false;
	}
	return true;
}

#pragma warning (pop)