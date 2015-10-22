/******************************************************************************
* Copyright (C) : QTH/WR24                                      
* Module name	: DLL
* File name     : QC1.h             
* Summary       : Download DLL for QC1 (8960).                                                        
* Version       Author             Date                 Abstract                      
*   1.0         Evonne.Hsieh       2012/02/09           First version
*****************************************************************************/

#pragma once
#include <set>
#include "BaseProject.h"
#include "..\..\..\Modules\QisdaQualcommDownloadModule\Download8994.h"
#include "..\UnifyDLL_Download.h"

class CU2416 : public CBaseProject
{
public:
	CU2416(int i_COMPort, CString str_multiDLFlag); 
	virtual ~CU2416(void);
public:
	virtual bool Run();
	virtual bool PostRun(int i_slot);
	virtual void Event(const std::string& str_Event, long nParam);

public:
	bool ReadFA(char* sz_FAData);
	bool WriteFA(char* sz_FAData);
	bool ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	bool WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	bool LogUpload(char *str_modelname);
	bool LogUploadByBAT(char* str_modelname);
	bool DFSCheckFile(char *str_LocalPath, char *str_RemotePath, char *str_TracePath);
	bool DFSDeleteFile(char *str_LocalPath, char *str_TracePath);
	
	/* MD5 verify lion */

	
	set < CString> mapNewMd5;
	set < CString> mapExistMd5;

	bool genMD5(CString folderPath);

	bool check_IMAGE_MD5();
	bool readMD5(set < CString> *readMapMd5, CString csPath);
	bool compareMD5(CString csIMGName);
	bool RunMD5Check();

private:
	bool PreDLRun(void);          // COM download + Fastboot download
	bool ReDLRun(void);           // Fastboot download
	bool Download(void);
	bool DLchipset(void);
	bool ReadIniFile(void);

private:
	bool GetDLMode(void);
	bool GetSupportQDownload(void);

private:
	int      m_i_COMPort;              // COM port
	CString  m_str_multiDLFlag;        // multi-download flag
	CString  m_str_errorCode;          // ErrorCode
	CString  m_str_DLMode;             // DL Mode
	int      m_i_rework;               // Rework flag
	int      m_i_reboot;               // Reboot flag
	int      m_i_DDRCheck;             // DDR check flag
	int      m_i_DumpEmmcLog;            
	int      m_i_RebootMode;
	int      m_i_checkSum;             // CheckSum flag
	int      m_i_BackupNVPartition;    // Backup NV flag
	int      m_i_SupportQDownload;     // Support for QDownload
	int      m_i_eraseUserFat;         // Erase user fat
	CString  m_str_dllIniFile;
	static CCriticalSection  m_obj_CritSection;
	static CCriticalSection  m_obj_MD5_CritSection;


	bool m_b_LogUpload;
	std::string m_str_UploadServerIP;
};
