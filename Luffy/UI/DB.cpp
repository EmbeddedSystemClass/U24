
// DB.cpp : implementation file
//

#include "stdafx.h"
#include "DB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection CDB::m_obj_DBLock;

CDB::CDB(void)
{
	m_bDBReady = false;
}

void CDB::SetidParameter(st_UIIDparameter st_idParameter)
{
	m_st_idParameter = st_idParameter;
	m_strDummyDB.Format(_T("%s\\%s\\Setting\\%s"), CONFIG_PATH, m_st_idParameter.str_modelName, DUMMY_DB);
}

void CDB::SetslotPICSData(std::map<int, std::map<CString, CString>> map_slotPICSData)
{
	m_map_slotPICSData = map_slotPICSData;
}

bool CDB::ReadRecordFromDB(int &iTotal, int &iPass, int &iFail)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);

	bool bResult = false;
	if(ConnectDataBase())
	{
		ReadRecord(iTotal, iPass, iFail);

		bResult = true;
	}

	return bResult;
}

bool CDB::WriteRecordToDB(int iTotal, int iPass, int iFail)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);

	bool bResult = false;
	if(ConnectDataBase())
	{
		WriteRecord(iTotal, iPass, iFail);

		bResult = true;
	}

	return bResult;
}

bool CDB::InsertPICSDataToDB(int iSlot)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);

	if(ConnectDataBase())
	{
		return InsertPICSData(iSlot);
	}

	return false;
}

bool CDB::ConnectDataBase()
{
	if (!m_bDBReady)
	{
		TCHAR szDummyDB[512] = {0};
		CFileFind file_findCtrl;

		wsprintf(szDummyDB, _T("%s\\%s\\Setting\\%s"), CONFIG_PATH, m_st_idParameter.str_modelName, DUMMY_DB);

		if (!file_findCtrl.FindFile(m_strDummyDB))
		{
			CStdioFile m_obj_file;        // File handle
			if (!m_obj_file.Open(m_strDummyDB, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
			{
				//m_str_errorCode = ;
				m_str_errorMsg.Format(_T("Can not find %s file!"), m_strDummyDB);

				return false;
			}
			m_obj_file.Close();
		}

		m_bDBReady = true;
	}

	return true;
}

void CDB::ReadRecord(int &iTotal, int &iPass, int &iFail)
{
	TCHAR szTemp[128] = {0};
	CString strRecord(_T("RECORD"));

	::GetPrivateProfileString(strRecord, _T("Total"), _T("0"), szTemp, sizeof(szTemp), m_strDummyDB);
	iTotal = StrToInt(szTemp);

	if (iTotal == 0)
	{
		WriteRecord(0, 0, 0);
		iPass = 0;
		iFail = 0;
	}
	else
	{
		memset(szTemp, 0, sizeof(szTemp));
		::GetPrivateProfileString(strRecord, _T("Pass"), _T("0"), szTemp, sizeof(szTemp), m_strDummyDB);
		iPass = StrToInt(szTemp);

		memset(szTemp, 0, sizeof(szTemp));
		::GetPrivateProfileString(strRecord, _T("Fail"), _T("0"), szTemp, sizeof(szTemp), m_strDummyDB);
		iFail = StrToInt(szTemp);
	}
}

void CDB::WriteRecord(int iTotal, int iPass, int iFail)
{
	CString strRecord(_T("RECORD"));

	CString strTotal, strPass, strFail;
	strTotal.Format(_T("%d"), iTotal);
	strPass.Format(_T("%d"), iPass);
	strFail.Format(_T("%d"), iFail);

	::WritePrivateProfileString(strRecord, _T("Total"), strTotal, m_strDummyDB);
	::WritePrivateProfileString(strRecord, _T("Pass"), strPass, m_strDummyDB);
	::WritePrivateProfileString(strRecord, _T("Fail"), strFail, m_strDummyDB);
}

bool CDB::InsertPICSData(int iSlot)
{
	CString strPicasso(m_map_slotPICSData[iSlot][_T("PICASSO")]);

	if (strPicasso.IsEmpty())
		return false;

	for (std::map<CString, CString>::iterator itr = m_map_slotPICSData[iSlot].begin();
		itr != m_map_slotPICSData[iSlot].end();
		++itr)
	{
		CString strTemp(itr->first);
		if (strTemp.Find(_T("PICASSO")) == -1)
		{
			::WritePrivateProfileString(strPicasso, itr->first, itr->second, m_strDummyDB);
		}
	}

	return true;
}