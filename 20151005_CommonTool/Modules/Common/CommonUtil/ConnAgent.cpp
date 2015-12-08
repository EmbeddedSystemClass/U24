#include "StdAfx.h"
#include "ConnAgent.h"
#include "sys/stat.h"
#include "QLib.h"
#include <setupapi.h>
#include <TCHAR.h>

#pragma warning( disable : 4996 )

CConnAgent::CConnAgent()
{
}

CConnAgent::~CConnAgent()
{
}

BOOL CConnAgent::SearchQpst()
{
	BOOL bResult = TRUE;
	HKEY hKey1, hKey2;
	UINT uiDataSize = 200;
	TCHAR sSubKey[100], sData[200], sCmd[1024] = _T("");

	_tcscpy (sSubKey, _T("SOFTWARE\\Classes\\Qualcomm.ListPC.1\\CLSID\0"));
	if (bResult && (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey1) == ERROR_SUCCESS))
	{
		bResult = FALSE;

		uiDataSize = 200;
		::RegQueryValueEx(hKey1, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

		if (_tcscmp(sData, _T("")))
		{
			_stprintf (sSubKey, _T("SOFTWARE\\Classes\\CLSID\\%s\\LocalServer32\0"), sData);
			if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey2) == ERROR_SUCCESS)
			{
				uiDataSize = 200;
				::RegQueryValueEx(hKey2, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

				struct _stat tStat;
				if (-1 != _tstat(sData, &tStat))
					bResult = TRUE;

				::RegCloseKey(hKey2);
			}
		}

		::RegCloseKey(hKey1);
	}

	_tcscpy(sSubKey, _T("SOFTWARE\\Classes\\AtlasServer.ThreadTraceSnapshot\\CLSID\0"));
	if (bResult && (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey1) == ERROR_SUCCESS))
	{
		bResult = FALSE;

		uiDataSize = 200;
		::RegQueryValueEx(hKey1, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

		if (_tcscmp(sData, _T("")))
		{
			_stprintf(sSubKey, _T("SOFTWARE\\Classes\\CLSID\\%s\\LocalServer32\0"), sData);
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey2) == ERROR_SUCCESS)
			{
				uiDataSize = 200;
				::RegQueryValueEx(hKey2, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

				struct _stat tStat;
				if (-1 != _tstat(sData, &tStat))
					bResult = TRUE;

				::RegCloseKey(hKey2);
			}
		}

		::RegCloseKey(hKey1);
	}
	
	_tcscpy(sSubKey, _T("SOFTWARE\\Classes\\Qualcomm.AtlasResources\0"));
	if (bResult && (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey1) == ERROR_SUCCESS))
	{
		bResult = FALSE;

		uiDataSize = 200;
		::RegQueryValueEx(hKey1, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

		if (_tcscmp(sData, _T("Qualcomm.AtlasResources Class")) == 0)
			bResult = TRUE;

		::RegCloseKey(hKey1);
	}
	
	_tcscpy(sSubKey, _T("SOFTWARE\\QUALCOMM\\QPST\\2.0\\Server"));
	if (bResult && (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_ALL_ACCESS, &hKey1) == ERROR_SUCCESS))
    {
		bResult = FALSE;

		uiDataSize = 200;
		::RegQueryValueEx(hKey1, _T(""), NULL, NULL, (LPBYTE)sData, (LPDWORD)&uiDataSize);

		struct _stat tStat;
		if (-1 != _tstat(sData, &tStat))
			bResult = TRUE;

		::RegCloseKey(hKey1);
    }

	return bResult;
}

BOOL CConnAgent::GetCommDevAry(int iSolution, CUIntArray& auiCommDevAry, CStringArray* pacstrCommDevNameAry /*= NULL*/)
{
	switch(iSolution)
	{
	case CONN_PORT_USE_REGISTRY:
		return GetCommDevAryUseRegistry(auiCommDevAry);

	case CONN_PORT_USE_QLIB:
		return GetCommDevAryUseQLib(auiCommDevAry);

	case CONN_PORT_USE_WDK:
		return GetCommDevAryUseWdk(auiCommDevAry, pacstrCommDevNameAry);

	default:
		break;
	}

	ASSERT(FALSE);
	return FALSE;
}

BOOL CConnAgent::GetCommDevAryUseRegistry(CUIntArray& auiCommDevAry)
{
	ASSERT(auiCommDevAry.IsEmpty());

	// init output parameters.
	auiCommDevAry.RemoveAll();

	HKEY hKey = GetSerialPortHKey();
	ASSERT(hKey != NULL);

	if (hKey != NULL)
	{
		DWORD uiEntryCnt = 0;
		LONG iResult = ::RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &uiEntryCnt, NULL, NULL, NULL, NULL);

		TCHAR sEntryName[MAX_PATH], sEntryData[MAX_PATH];
		for (DWORD i = 0; (iResult == ERROR_SUCCESS) && (i < uiEntryCnt); i++)
		{
            DWORD uiType, uiNameLen = MAX_PATH, uiDataLen = MAX_PATH;
			iResult = ::RegEnumValue(hKey, i, sEntryName, &uiNameLen, NULL, &uiType, (BYTE*)sEntryData, &uiDataLen);

			// if not successfully read value, jump to next entry. more data is OK to process.
			if ((iResult != ERROR_SUCCESS) && (iResult != ERROR_MORE_DATA))
				continue;

			UINT uiIndex = CommNameToIndex(sEntryData, uiDataLen);
			if (uiIndex > 0 )
				auiCommDevAry.Add(uiIndex);
		}

		::RegCloseKey(hKey);
	}

	return !auiCommDevAry.IsEmpty();
}

HKEY CConnAgent::GetSerialPortHKey()
{
	LONG iResult = ERROR_SUCCESS;
	HKEY hKeyHardware = NULL, hKeyDeviceMap = NULL, hKeySerialComm = NULL;
 
	if (iResult == ERROR_SUCCESS)
		iResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE"), 0, KEY_QUERY_VALUE, &hKeyHardware);

	if ((iResult == ERROR_SUCCESS) && (hKeyHardware != NULL))
		iResult = ::RegOpenKeyEx(hKeyHardware, _T("DEVICEMAP"), 0, KEY_QUERY_VALUE, &hKeyDeviceMap);
 
	if ((iResult == ERROR_SUCCESS) && (hKeyDeviceMap != NULL))
		iResult = ::RegOpenKeyEx(hKeyDeviceMap, _T("SERIALCOMM"), 0, KEY_QUERY_VALUE, &hKeySerialComm);

	// clean-up.
	if (hKeyHardware != NULL) ::RegCloseKey(hKeyHardware);
	if (hKeyDeviceMap != NULL) ::RegCloseKey(hKeyDeviceMap);

	return hKeySerialComm;
}

UINT CConnAgent::CommNameToIndex(const TCHAR* sComm, const int iCommLen)
{
	ASSERT((sComm != NULL) && (iCommLen > 0));

	// do nothing if paremeters aren't correct.
	if ((sComm == NULL) || (iCommLen <= 0)) return 0;

	int iCommNum = 0;
	CString  cstrCommName, cstrCommNum;
 
	if ((sComm != NULL) && (iCommLen > 3)) // the CommName Length must > 3, like COM1.
	{
		cstrCommName = CString(sComm, iCommLen);
        		
		if (cstrCommName.Left(3) == "COM")
		{
			cstrCommNum = cstrCommName.Mid(3);
			iCommNum = _ttoi(cstrCommNum);
		}
	}

	return (iCommNum > 0) ? (UINT)iCommNum : 0;
}

BOOL CConnAgent::GetCommDevAryUseQLib(CUIntArray& auiCommDevAry)
{
	ASSERT(auiCommDevAry.IsEmpty());

	// init output parameters.
	auiCommDevAry.RemoveAll();

	USHORT nCommDevNum = 20;
	USHORT anCommDevAry[20];
	
	if (QLIB_GetAllPortList(&nCommDevNum, anCommDevAry))
	{
		for (UINT i = 0; i < nCommDevNum; i++)
			auiCommDevAry.Add(anCommDevAry[i]);
	}

	return !auiCommDevAry.IsEmpty();
}

BOOL CConnAgent::GetCommDevAryUseWdk(CUIntArray& auiCommDevAry, CStringArray* pacstrCommDevNameAry)
{
	ASSERT(auiCommDevAry.IsEmpty());

	// init output parameters.
	auiCommDevAry.RemoveAll();
	if (pacstrCommDevNameAry != NULL) pacstrCommDevNameAry->RemoveAll();

	BOOL bContinue = TRUE;

	// retrieves the GUID(s) associated with the specified class name: "Ports".
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	if (bContinue == TRUE)
	{
		::SetupDiClassGuidsFromName(_T("Ports"), NULL, 0, &uiGuidSize);

		if (uiGuidSize > 0)
		{
			ptGuidBuf = new GUID[uiGuidSize];
			ASSERT(ptGuidBuf != NULL);

			if (ptGuidBuf != NULL)
			{
				bContinue = ::SetupDiClassGuidsFromName(_T("Ports"), ptGuidBuf, uiGuidSize, &uiGuidSize);
			}
		}
	}

	// returns a device information set that contains all devices of a specified class.
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue == TRUE)
	{
		ASSERT(ptGuidBuf != NULL);
		hDevInfoSet = ::SetupDiGetClassDevs(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// start emumeration.
	DWORD uiIndex = 0;

	while (bContinue == TRUE)
	{
		ASSERT(hDevInfoSet != INVALID_HANDLE_VALUE);

		// returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)) == TRUE)
		{
			BOOL bAddDevName = FALSE;

			// opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// read in the name of the port.
				TCHAR sCommDev[MAX_PATH];
				DWORD uiType = 0, uiCommDevSize = MAX_PATH;

				if ((::RegQueryValueEx(hKey, _T("PortName"), NULL, &uiType, (LPBYTE) sCommDev, &uiCommDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					UINT uiIndex = CommNameToIndex(sCommDev, uiCommDevSize);
					auiCommDevAry.Add(uiIndex);

					bAddDevName = TRUE;
				}

				if (pacstrCommDevNameAry != NULL)
				{
					uiCommDevSize = MAX_PATH;
					if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)sCommDev, uiCommDevSize, &uiCommDevSize) && (uiType == REG_SZ))
						pacstrCommDevNameAry->Add(sCommDev);
					else
						pacstrCommDevNameAry->Add(_T(""));
				}

				// close the key.
				::RegCloseKey(hKey);
			}
		}

		uiIndex++;
	}

	// clean-up.
	if (ptGuidBuf != NULL) delete [] ptGuidBuf;
	if (hDevInfoSet != INVALID_HANDLE_VALUE) SetupDiDestroyDeviceInfoList(hDevInfoSet);

#ifdef _DEBUG

	if(pacstrCommDevNameAry != NULL)
		ASSERT(auiCommDevAry.GetSize() == pacstrCommDevNameAry->GetSize());

#endif // _DEBUG

	return !auiCommDevAry.IsEmpty();
}
