#ifndef __DOWNLOAD_INTERFACE__
#define __DOWNLOAD_INTERFACE__

#include "../../CommonUtil/NotifyBase.h"
#include "../../Modules/QisdaNVModule\NVItem.h"
#include "Download_Export.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"

using namespace std;
using namespace QSYNC_LIB;
const TSTRING DOWNLOAD_INI_FILENAME = _T("Download.ini");

//Notify
const TSTRING EVENT_TOTALLY_CALLBACK        = _T("TOTALLY_DOWNLOAD_CALL_BACK");
const TSTRING EVENT_TOTALLY_PROGRESS        = _T("TOTALLY_DOWNLOAD_PROGRESS");
const TSTRING EVENT_TOTALLY_STEP_LIGHT      = _T("TOTALLY_DOWNLOAD_STEP_LIGHT");
const TSTRING EVENT_TOTALLY_GET_VARIANT_STR = _T("TOTALLY_DOWNLOAD_VARIANT_STRING");
const TSTRING EVENT_TOTALLY_SET_ROOT_STR = _T("TOTALLY_DOWNLOAD_GET_ROOT_STRING");
const TSTRING EVENT_MULTIDL_PROCESS        = _T("MULTI_DOWNLOAD_PROCESS");

typedef enum _Platform {
    NPRG6240,
    NPRG6245,
    NPRG6260,
    NPRG6270,
	NPRG6280,
    NPRG8650,
	NPRG7X27,
    UNKNOWN
} Platform;

class DownloadInterface : public CNotifyBase, public INotify_ANSI 
{

public:
    DownloadInterface(int nPort)
		:m_nSelectCom(nPort)
		,m_pIConfigure(QSYNC_LIB::CreateIConfigure(_T("QDownload")))
		,m_bDifferentPartition(false) 
		,m_bUARTDownload(false)
	{
		m_pIConfigure->Load();

        DefineNotify(EVENT_TOTALLY_CALLBACK);
        DefineNotify(EVENT_TOTALLY_PROGRESS);
        DefineNotify(EVENT_TOTALLY_STEP_LIGHT);
        DefineNotify(EVENT_TOTALLY_GET_VARIANT_STR);
		DefineNotify(EVENT_TOTALLY_SET_ROOT_STR);
		DefineNotify(EVENT_MULTIDL_PROCESS);

        //////////////////////////////////////////////////////////////////////////
        // Get the Download.ini file path
		m_strIni = GetINI();
        ASSERT(_taccess(m_strIni.GetString(), 0) == 0);
    }

    virtual ~DownloadInterface() {}

    bool Register(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Register(pNotify, strEvent);
    }
    bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Register(pEventFunc, strEvent);
    }
    bool Unregister(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pNotify, strEvent);
    }
    bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pEventFunc, strEvent);
    }
    void DefineNotify(const TSTRING& strNewEvent) {
        CNotifyBase::DefineNotify(strNewEvent);
    }
    void Fire(const TSTRING& strEvent,long nParam) {
        CNotifyBase::Fire(strEvent, nParam);
    }

    void Event(const std::string& strEvent,long nParam) 
	{
        if(strEvent == EVENT_DL_PROGRESS) 
		{
			//Get time stamp
			CTime t = CTime::GetCurrentTime();
			CStringA strTime;
			strTime.Format("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond());

            int nPrecentage = ((DL_PROGRESS*)nParam)->nProgress;
            CStringA strPercent;
            strPercent.Format("%s %s %d",strTime,((DL_PROGRESS*)nParam)->szMsg, nPrecentage);
            DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
            wchar_t *pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
            int nStep = GetStepLight(strPercent);
            if(nStep != 0)
                Fire(EVENT_TOTALLY_STEP_LIGHT, nStep);  // transfer step light
            Fire(EVENT_TOTALLY_CALLBACK, (long)pwText); // transfer message to UI
            Fire(EVENT_TOTALLY_PROGRESS, nPrecentage);  // transfer percentage
			Fire(EVENT_MULTIDL_PROCESS,nParam);
            delete[] pwText;
        } 
		else if(strEvent == EVENT_DL_PARTITION_DIF) 
		{
            m_bDifferentPartition = true;
        }
    }

public:
	const CString& GetRoot()
	{
		if(m_strRootPath.IsEmpty())
		{
			//1. Find INI [path] root=Ask or "" or [Absolut Path]
			TCHAR szRootPath[MAX_PATH] = {0};
			GetPrivateProfileString(_T("path"), _T("root"), _T(""), szRootPath, MAX_PATH, m_strIni.GetString());
			m_strRootPath = szRootPath;

			//2. Ask from UI
			if(m_strRootPath == _T("Ask"))
			{
				//First, try to get path
				m_pIConfigure->Load();
				m_strRootPath = m_pIConfigure->GetCfgVar(_T("LoadPath"),_T("")).c_str();

				if(m_strRootPath.IsEmpty())
				{//Second, get the path from user selected
					TCHAR szAskRootPath[MAX_PATH] = {0};
					m_strRootPath.Empty();
					Fire(EVENT_TOTALLY_SET_ROOT_STR, (long)(TCHAR*)szAskRootPath);
					m_strRootPath = szAskRootPath;
				}
				
				//Check again
				if(_taccess(m_strRootPath,0) == -1)
					m_strRootPath.Empty();
			}
			else if(!m_strRootPath.IsEmpty())
			{
				TCHAR szAskRootPath[MAX_PATH] = {0};
				_tcscpy(szAskRootPath, m_strRootPath.GetBuffer());
				Fire(EVENT_TOTALLY_SET_ROOT_STR, (long)(TCHAR*)szAskRootPath);
				if(_taccess(szAskRootPath, 0) == -1)
				{
					m_strRootPath.Empty();
					WritePrivateProfileString(_T("path"),_T("root"),_T("Ask"), m_strIni.GetString());
				}
				else
				{
					m_strRootPath = szAskRootPath;
					WritePrivateProfileString(_T("path"),_T("root"),m_strRootPath, m_strIni.GetString());
				}	
			}

			//3. Use current folder
			if(m_strRootPath.IsEmpty())
			{
				TCHAR szCurrentFolder[MAX_PATH] = {0};
				::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
				m_strRootPath = szCurrentFolder;
			}

			_tcscpy(szRootPath,m_strRootPath);
			::PathAddBackslash(szRootPath);
			m_strRootPath = szRootPath;
		}

		return m_strRootPath;
	}

protected:
    bool OpenImage(IMAGE & imgLoad, const CString& strLoadName) 
	{
        TCHAR szLoadPath[MAX_PATH] = {0};
        GetPrivateProfileString(_T("path"), strLoadName.GetString(), _T(""), szLoadPath, MAX_PATH, m_strIni.GetString());

		bool bLoadExist = false;
		CString strLoadFileName;
		if(_tcslen(szLoadPath) > 0)
		{
			strLoadFileName = GetRoot() + CString(szLoadPath);
			bLoadExist = (_taccess(strLoadFileName.GetString(), 0) == 0);
		}

		if(bLoadExist)
		{
			CFile fLoad;
			BOOL bOpened = fLoad.Open(strLoadFileName.GetString(), CFile::modeRead);
			if(bOpened != FALSE) 
			{
				UINT nLength = (UINT)fLoad.GetLength();
				if(nLength > 0)
				{
					BYTE* pData = new BYTE[nLength+2];
					::ZeroMemory(pData,nLength+2);
					UINT nReadSize = fLoad.Read(pData, nLength);
					ASSERT(nReadSize == nLength);				
					bLoadExist = nReadSize == nLength;
					if(bLoadExist)
						imgLoad.copyFrom(pData,nLength);
					delete [] pData;					
				}
			}
		}
        return bLoadExist;
    }

public:
	static CString GetINI()
	{
		TCHAR szCurrentFolder[MAX_PATH] = {0};
		::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
		::PathAddBackslash(szCurrentFolder);
		return CString(szCurrentFolder) + DOWNLOAD_INI_FILENAME.c_str();
	}

    static Platform GetPlatform(const CString& strHex = _T("")) 
	{
		CString strTempHex = strHex;
		if(strTempHex.IsEmpty())
		{
			TCHAR szHEXPath[MAX_PATH] = {0};
			GetPrivateProfileString(_T("path"), _T("hex"), _T(""), szHEXPath, MAX_PATH, GetINI().GetString());
			strTempHex = ::PathFindFileName(szHEXPath);
		}

        Platform platform = UNKNOWN;
        if(strTempHex.CompareNoCase(_T("NPRG6240.hex")) == 0) {
            platform = NPRG6240;
        } else if(strTempHex.CompareNoCase(_T("NPRG6245.hex")) == 0) {
            platform = NPRG6245;
        } else if(strTempHex.CompareNoCase(_T("NPRG6260.hex")) == 0) {
            platform = NPRG6260;
        } else if(strTempHex.CompareNoCase(_T("NPRG6270.hex")) == 0) {
            platform = NPRG6270;
        } else if(strTempHex.CompareNoCase(_T("NPRG8650.hex")) == 0) {
            platform = NPRG8650;
        } else if(strTempHex.CompareNoCase(_T("NPRG6280.hex")) == 0) {
            platform = NPRG6280;
		} else if(strTempHex.CompareNoCase(_T("NPRG7627.hex")) == 0) {
			platform = NPRG7X27;
		} else {
            ASSERT(0);
        }
        return platform;
    }

protected:
	void BackupBRT()
	{
		::DeleteFile(_T("output.brt"));

		//Backup BRT
		TCHAR szBuf[16] = {0};
		DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("BackupBRT"), _T("0"), szBuf, 16, m_strIni.GetString());
		if(_ttoi(szBuf) == 1)
		{
			char szCurrentFolderA[MAX_PATH] = {0};
			::GetCurrentDirectoryA(MAX_PATH, szCurrentFolderA);
			TCHAR szCurrentFolder[MAX_PATH] = {0};
			::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
			CStringA strIni = CStringA(szCurrentFolderA) + "\\Download.ini";
			char szCSV[MAX_PATH] = {0};
			DWORD dwResult = ::GetPrivateProfileStringA("path", "CSV", "", szCSV, MAX_PATH, strIni.GetString());

			bool bLoadExist = false;
			CString strLoadFileName;
			if(strlen(szCSV) > 0)
			{
				strLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(szCSV);
				bLoadExist = (_taccess(strLoadFileName.GetString(), 0) == 0);
			}

			if(bLoadExist)
			{
#ifdef _DEBUG
				HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_ShareMD.dll"));
#else
				HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_ShareM.dll"));
#endif    
				if(MobileToolKitDll)
				{
					typedef bool (*pFnBackupNV2BRT)(long nComport,const std::string& strOutputBRTFileName,const std::string& strInputCSVFile);
					typedef bool (*pFnRestoreBRT2NV)(long nComport,const std::string& strInBRTFileName);
					pFnBackupNV2BRT BackupNV2BRT = (pFnBackupNV2BRT)GetProcAddress(MobileToolKitDll, "BackupNV2BRT");
					pFnRestoreBRT2NV RestoreBRT2NV = (pFnRestoreBRT2NV)GetProcAddress(MobileToolKitDll, "RestoreBRT2NV");
					BackupNV2BRT(m_nSelectCom,"output.brt",(const char*)szCSV);
				}
			}
		}
	}

	
	void RestoreBRT()
	{
		//Restore BRT
		TCHAR szBuf[16] = {0};
		DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("RestoreBRT"), _T("0"), szBuf, 16, m_strIni.GetString());
		if(_ttoi(szBuf) == 1)
		{
			if(access("output.brt",0) != -1)
			{
#ifdef _DEBUG
				HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_ShareMD.dll"));
#else
				HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_ShareM.dll"));
#endif    
				if(MobileToolKitDll)
				{
					typedef bool (*pFnBackupNV2BRT)(long nComport,const std::string& strOutputBRTFileName,const std::string& strInputCSVFile);
					typedef bool (*pFnRestoreBRT2NV)(long nComport,const std::string& strInBRTFileName);
					pFnBackupNV2BRT BackupNV2BRT = (pFnBackupNV2BRT)GetProcAddress(MobileToolKitDll, "BackupNV2BRT");
					pFnRestoreBRT2NV RestoreBRT2NV = (pFnRestoreBRT2NV)GetProcAddress(MobileToolKitDll, "RestoreBRT2NV");
					RestoreBRT2NV(m_nSelectCom,"output.brt");
				}
			}
		}
	}
protected:
    bool m_bDifferentPartition;
    CString m_strIni;
	CString m_strRootPath;
	IConfigure* m_pIConfigure;

private:
	int m_nSelectCom;

public:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // These functions should be implemented
    virtual bool PreDownload() = 0;                 ///< doing something before downloading
    virtual bool PostDownload() = 0;                ///< doing something after downloading
    virtual bool StartDownload() = 0;               ///< implement by sub-class
	virtual bool StartUARTDownload() = 0;               ///< implement by sub-class
    virtual bool SetImageParameters() = 0;          ///< implement by sub-class
    virtual int GetStepLight(const CStringA& strMsg) = 0; ///< return the step index
    virtual void SetDLLoadPath7x27(const tagDLSetting7x27& LoadData){}
	virtual bool MultiStartDownload(){ return true;}
    ///////////////////////////////////////////////////////////////////////////////////////////////
	bool		m_bUARTDownload;

	private:
		bool PowerOff(int nComport) 
		{
			CNVItem nv(nComport);
			return nv.PowerOff();
		}
		bool OnLineMode(int nComport) 
		{
			CNVItem nv(nComport);
			return nv.OnLineMode();
		}
		bool ChangeOnline(int nComport) 
		{
			CNVItem nv(nComport);
			return nv.ChangeOnline();
		}
		bool CheckFTM(int nComport, int& nFTMValue) 
		{
			CNVItem nv(nComport);
			return nv.CheckFTM(nFTMValue);
		}

	protected:
		void FinalReboot() 
		{
			TCHAR szBuf[16] = {0};
			DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("Reboot"), _T("0"), szBuf, 16, m_strIni.GetString());
			if(*szBuf == _T('0')) 
			{
				PowerOff(m_nSelectCom);
			} 
			else 
			{
				while(true) 
				{
					//Qualcomm's change mode to online mode
					OnLineMode(m_nSelectCom);
					OnLineMode(m_nSelectCom);
					//Write NV453 to 0
					ChangeOnline(m_nSelectCom);
					ChangeOnline(m_nSelectCom);
					int nFTMValue = 0;
					//Read NV453
					CheckFTM(m_nSelectCom, nFTMValue);
					if(nFTMValue == 0)
						break;
				}
				PowerOff(m_nSelectCom);
			}
		}
};

#endif