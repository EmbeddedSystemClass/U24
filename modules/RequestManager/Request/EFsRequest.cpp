#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "EFsRequest.h"



namespace PLATFORM_TOOL
{
	CEFsRequest::CEFsRequest()
	{
	}

	CEFsRequest::~CEFsRequest()
	{
	}

	CEFsOpenFileRequest::CEFsOpenFileRequest(const TCHAR* sFileName, const long iFlag, const long iMode)
	{	
		BYTE szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_OPEN & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_OPEN >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFlag & 0xFF);
		szBuf[5] = (BYTE)((iFlag >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFlag >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFlag >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iMode) & 0xFF);
		szBuf[9] = (BYTE)((iMode >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iMode >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iMode >> 24) & 0xFF);

		size_t iNameSize = _tcslen(sFileName);
		memcpy(&szBuf[12], sFileName, iNameSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iNameSize + 12 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsOpenFileRequest::~CEFsOpenFileRequest()
	{
	}

	CEFsCloseFileRequest::CEFsCloseFileRequest(const DWORD iFd)
	{	
		BYTE szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_CLOSE & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_CLOSE >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFd & 0xFF);
		szBuf[5] = (BYTE)((iFd >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFd >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFd >> 24) & 0xFf);

		CBufferPtr ptBufferSend;
		Encode(szBuf,8, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCloseFileRequest::~CEFsCloseFileRequest()
	{
	}

	CEFsReadFileRequest::CEFsReadFileRequest(const DWORD iFb, const DWORD iBytes, const DWORD iOffset)
	{
		BYTE szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_READ & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_READ >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFb & 0xFF);
		szBuf[5] = (BYTE)((iFb >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFb >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFb >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iBytes) & 0xFF);
		szBuf[9] = (BYTE)((iBytes >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iBytes >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iBytes >> 24) & 0xFF);
		szBuf[12] = (BYTE)((iOffset) & 0xFF);
		szBuf[13] = (BYTE)((iOffset >> 8) & 0xFF);
		szBuf[14] = (BYTE)((iOffset >> 16) & 0xFF);
		szBuf[15] = (BYTE)((iOffset >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadFileRequest::~CEFsReadFileRequest()
	{
	}

	CEFsWriteFileRequest::CEFsWriteFileRequest(const DWORD iFd, const DWORD iOffset, const TCHAR* sData, const DWORD iSize)
	{
		BYTE szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_WRITE & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_WRITE >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFd & 0xFF);
		szBuf[5] = (BYTE)((iFd >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFd >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFd >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iOffset) & 0xFF);
		szBuf[9] = (BYTE)((iOffset >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iOffset >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iOffset >> 24) & 0xFF);

		memcpy(&szBuf[12], sData, iSize);

		CBufferPtr ptBufferSend;
		Encode(szBuf,iSize+12, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsWriteFileRequest::~CEFsWriteFileRequest()
	{
	}

	CEFsDeleteFileRequest::CEFsDeleteFileRequest(const TCHAR *sFilename)
	{
		BYTE szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_UNLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_UNLINK >> 8) & 0xFF);

		size_t iNameSize = _tcslen(sFilename);
		memcpy(&szBuf[4], sFilename, iNameSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iNameSize + 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsDeleteFileRequest::~CEFsDeleteFileRequest()
	{
	}

	CEFsMakeDirRequest::CEFsMakeDirRequest(const WORD iMode, const TCHAR* sFolderPath)
	{
		BYTE szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_MKDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_MKDIR >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iMode & 0xFF);
		szBuf[5] = (BYTE)((iMode >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[6], sFolderPath, iPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize + 6 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsMakeDirRequest:: ~CEFsMakeDirRequest()
	{
	}

	CEFsCreateLinkRequest::CEFsCreateLinkRequest(const TCHAR* sFilePath, const TCHAR *sPointedFilePath)
	{
		BYTE szBuf[1000] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_SYMLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_SYMLINK >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		size_t iPointedPathSize = strlen(sPointedFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize +1);
		memcpy(&szBuf[4]+iPathSize+1, sPointedFilePath, iPointedPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize+ (int)iPointedPathSize+4+2, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCreateLinkRequest::~CEFsCreateLinkRequest()
	{
	}

	CEFsReadLinkRequest::CEFsReadLinkRequest(const TCHAR* sFilePath)
	{
		BYTE szBuf[4096] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_READLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_READLINK >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize+4+1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadLinkRequest::~CEFsReadLinkRequest()
	{
	}

	CEFsOpenDirRequest::CEFsOpenDirRequest(const TCHAR* sFolderPath)
	{
		BYTE szBuf[4096] = {0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_OPENDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_OPENDIR >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[4], sFolderPath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize+4+1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsOpenDirRequest:: ~CEFsOpenDirRequest()
	{
	}
	
	CEFsCloseDirRequest::CEFsCloseDirRequest(const DWORD iDirp)
	{

		BYTE szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_CLOSEDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_CLOSEDIR >>8 ) & 0xFF);
		szBuf[4] = (BYTE)((iDirp) & 0xFF);
		szBuf[5] = (BYTE)((iDirp >> 8) & 0xFF);
		szBuf[6] = (BYTE)((iDirp >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iDirp >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 8, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCloseDirRequest::~CEFsCloseDirRequest()
	{
	}

	CEFsFileSystemStatusRequest::CEFsFileSystemStatusRequest(const TCHAR * sFilePath)
	{
		BYTE szBuf[4096] = {0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_STATFS & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_STATFS >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize +1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 4 +1, ptBufferSend);
		SetSendData(ptBufferSend);

	}
	CEFsFileSystemStatusRequest::~CEFsFileSystemStatusRequest()
	{
	}

	CEFsFlashDeviceInfoRequest::CEFsFlashDeviceInfoRequest(void)
	{
		BYTE szBuf[10] = {0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_NAND_DEV_INFO & 0xFF);
		szBuf[3]=(BYTE)((_EFS2_DIAG_NAND_DEV_INFO >> 8) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 4, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsFlashDeviceInfoRequest::~CEFsFlashDeviceInfoRequest()
	{
	}

	CEFsCheckAccessAttrRequest::CEFsCheckAccessAttrRequest(const WORD iPermissionBits, const TCHAR* sFilePath)
	{
		BYTE szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_ACCESS & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_ACCESS >> 8) & 0xFF);
		szBuf[4]=(BYTE)(iPermissionBits & 0xFF);
		szBuf[5]=(BYTE)((iPermissionBits >>8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[6], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 6 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsCheckAccessAttrRequest::~CEFsCheckAccessAttrRequest()
	{
	}

	CEFsChomdRequest::CEFsChomdRequest(const DWORD iMode, const TCHAR* sFilePath)
	{
		BYTE szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_CHMOD & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_CHMOD >> 8) & 0xFF);
		szBuf[4]=(BYTE)(iMode &0xFF);
		szBuf[5]=(BYTE)((iMode >> 8) & 0xFF);
		szBuf[6]=(BYTE)((iMode >>16) & 0xFF);
		szBuf[7]=(BYTE)((iMode >>24) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[8], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 8 +1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsChomdRequest::~CEFsChomdRequest()
	{
	}

	CEFsQueryFileStatusRequest::CEFsQueryFileStatusRequest(const TCHAR* sFilePath)
	{
		BYTE szBuf[4096]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_STAT & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_STAT >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize+ 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsQueryFileStatusRequest::~CEFsQueryFileStatusRequest()
	{
	}

	CEFsRenameRequest::CEFsRenameRequest(const TCHAR* sOldPathName,const TCHAR* sNewPathName)
	{
		BYTE szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_RENAME & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_RENAME >> 8 )& 0xFF);

		size_t iOldPathSize = _tcslen(sOldPathName);
		size_t iNewPathSize = _tcslen(sNewPathName);
		memcpy(&szBuf[4], sOldPathName, iOldPathSize +1);
		memcpy(&szBuf[4]+iOldPathSize+1, sNewPathName, iNewPathSize);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iOldPathSize+(int)iNewPathSize + 4 + 2, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsRenameRequest::~CEFsRenameRequest()
	{
	}

	CEFsRemoveFolderRequest::CEFsRemoveFolderRequest(const TCHAR* sFolderPath)
	{
		BYTE szBuf[4096]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_RMDIR & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_RMDIR >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[4], sFolderPath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsRemoveFolderRequest::~CEFsRemoveFolderRequest()
	{
	}

	CEFsReadDirRequest::CEFsReadDirRequest(const DWORD dirp, const int iseqno)
	{
		BYTE szBuf[20]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_READDIR & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_READDIR >> 8) & 0xFF);

		szBuf[4] = (BYTE)(dirp & 0xFF);
		szBuf[5] = (BYTE)((dirp>>8) & 0xFF);
		szBuf[6] = (BYTE)((dirp>>16)&0xFF);
		szBuf[7] = (BYTE)((dirp>>24)&0xFF);
		szBuf[8] = (BYTE)(iseqno&0xFF);
		szBuf[9] = (BYTE)((iseqno>>8)&0xFF);
		szBuf[10] = (BYTE)((iseqno >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iseqno >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf,12, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadDirRequest::~CEFsReadDirRequest()
	{
	}
	
	CGetMicroSDInfoRequest::CGetMicroSDInfoRequest(int& nFilesystemType,int& nBlockSize,int& nTotalBlockCount,int& nFreeBlockCount,
		int& nAvailableBlockCount,int& nTotalFileNumber,int& nFreeFileNumber,int& nFileSystemID
		,int& nMaximumFilename,BYTE& bStatus)
		:CDiagRequestBase("CGetMicroSDInfoRequest")
		,m_nFilesystemType(nFilesystemType)
		,m_nBlockSize(nBlockSize)
		,m_nTotalBlockCount(nTotalBlockCount)
		,m_nFreeBlockCount(nFreeBlockCount)
		,m_nAvailableBlockCount(nAvailableBlockCount)
		,m_nTotalFileNumber(nTotalFileNumber)
		,m_nFreeFileNumber(nFreeFileNumber)
		,m_nFileSystemID(nFileSystemID)
		,m_nMaximumFilename(nMaximumFilename)
		,m_bStatus(bStatus)
	{
		//SendRawRequest 75 19 19 0 109 109 99 49 0      //mmc1
		//BYTE szRAWCommand[] = {75, 255,42,  0};
		BYTE szRAWCommand[] = {75, 19, 19, 0, 109, 109, 99, 49, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGetMicroSDInfoRequest::~CGetMicroSDInfoRequest()
	{
	}

	bool CGetMicroSDInfoRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CGetMicroSDInfoRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{	
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID – CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID – SUBSYS_ID is set to 19 for File System
		Command_Code	2	Command Code – set to 19 for reading SD card size
		Diag Error No	4	diag_errno
		F_fsid	4	File system ID
		f_type	8	Type of filesystem
		F_bsize	4	Filesystem blocksize
		f_blocks	4	Total data blocks in filesystem
		f_bavail	4	Free blocks available to user use
		f_ffree	4	Free file nodes in FS
		max_file_size	4	max_file_size
		Nfiles	4	Current number of files in this filesystem
		max_nfiles	4	Maximum number of files in this filesystem
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short mobile_id;
			int dwError;//Diag Error No	4	diag_errno
			int nFilesystemType1;//f_type	8	Type of filesystem
			int nFilesystemType2;//f_type	8	Type of filesystem
			int nBlockSize;//F_bsize	4	Filesystem blocksize
			int nTotalBlockCount;//f_blocks	4	Total data blocks in filesystem
			int nFreeBlockCount;//f_bavail	4	Free blocks available to user use
			int nFreeFileNotes;//f_ffree	4	Free file nodes in FS
			int nMaximumFileSize;//max_file_size	4	max_file_size
			int nFilesNumber;//Nfiles	4	Current number of files in this filesystem
			int nMaximumFilename;//max_nfiles	4	Maximum number of files in this filesystem
		} diag_get_miscro_SD_rsp_type;
		/*
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short mobile_id;
			int dwAvail;
			int nFilesystemType;
			int nBlockSize;
			int nTotalBlockCount;
			int nFreeBlockCount;
			int nAvailableBlockCount;
			int nTotalFileNumber;
			int nFreeFileNumber;
			int nFileSystemID;
			int nMaximumFilename;
			BYTE nStatus;
		} diag_get_miscro_SD_rsp_type;
		*/
#pragma pack(pop)
		diag_get_miscro_SD_rsp_type resp = {0};
		int nSize = sizeof(diag_get_miscro_SD_rsp_type);
		assert(sizeof(diag_get_miscro_SD_rsp_type) <= ptBufferReceived->GetLength());
		memcpy(&resp,ptBufferReceived->GetData(),nSize);

		//m_nFilesystemType = resp.nFilesystemType;
		m_nBlockSize = resp.nBlockSize;
		m_nTotalBlockCount = resp.nTotalBlockCount;
		m_nFreeBlockCount = resp.nFreeBlockCount;
		//m_nAvailableBlockCount = resp.nAvailableBlockCount;
		//m_nTotalFileNumber = resp.nTotalFileNumber;
		//m_nFreeFileNumber = resp.nFreeFileNumber;
		//m_nFileSystemID = resp.nFileSystemID;
		m_nMaximumFilename = resp.nMaximumFilename;
		//m_bStatus = resp.nStatus;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CEFsGoldenCopyRequest::CEFsGoldenCopyRequest (bool &bSuccess)
		: CDiagRequestBase (_T("CEFsGoldenCopyRequest"), 5000, false)
		, m_Success (bSuccess)
	{

		bSuccess = false;
        // {0x4B, 0x13, 0x35, 0x00, 0x01, 0x00, 0x2F, 0x6E, 0x76, 0x6D, 0x00} for "/NVm"
        // {0x4B, 0x13, 0x35, 0x00, 0x01, 0x00, 0x2F, 0x00} for "/"
		BYTE szRAWCommand[] = {0x4B, 0x13, 0x35, 0x00, 0x01, 0x00, 0x2F, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsGoldenCopyRequest::~CEFsGoldenCopyRequest()
	{
	}

	void CEFsGoldenCopyRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		//by Ember:
		//Success => no response and auto restart for EFs backup
		//Fail    => has response(but unknown)
		//this->m_Success = true;

		//CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CEFsSyncNVNOWaitRequest::CEFsSyncNVNOWaitRequest (DWORD &dwToken, bool &bSuccess)
		: CDiagRequestBase (_T("CEFsSyncNVNOWaitRequest"))
		, m_Token (dwToken)
		, m_Success (bSuccess)
	{

		bSuccess = false;
		BYTE szRAWCommand[] = {0x4B, 0x13, 0x30, 0x00, 0x01, 0x00, 0x2F, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsSyncNVNOWaitRequest::~CEFsSyncNVNOWaitRequest()
	{
	}

	void CEFsSyncNVNOWaitRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//19
			short mobile_id;//48
			short seq_num; // 1
			DWORD token;
			DWORD errnum;

		} diag_syncnv_nowait_response;
		#pragma pack(pop)

		diag_syncnv_nowait_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		
		
		m_Success = 0 == resp.errnum;
		m_Token = resp.token;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CEFsSyncNVGetStatusRequest::CEFsSyncNVGetStatusRequest (DWORD token, bool &bSuccess)
		: CDiagRequestBase (_T("CEFsSyncNVGetStatusRequest"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
		BYTE *pTokens = (BYTE*)&token;
		BYTE szRAWCommand[] = {0x4B, 0x13, 0x31, 0x00, 0x01, 0x00, *pTokens, *(pTokens + 1), *(pTokens + 2), *(pTokens + 3), 0x2F, 0x6E, 0x76, 0x6D, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsSyncNVGetStatusRequest::~CEFsSyncNVGetStatusRequest()
	{
	}

	void CEFsSyncNVGetStatusRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//19
			short mobile_id;//48
			short seq_num; // 1
			BYTE status; //0:pending, 1:sync complete
			DWORD errcode;

		} diag_syncnv_getstatus_response;
		#pragma pack(pop)

		diag_syncnv_getstatus_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.status == 1;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	
	// add for orange [7/26/2011 Kevin.Dong] 
	//HWID
	CReadHWIDRequestForOrange::CReadHWIDRequestForOrange (char* szHWID, bool &bSuccess)
		: CDiagRequestBase (_T("CReadHWIDRequestForOrange"))
		, m_Success (bSuccess)
		,szHWID_Get(szHWID)
	{

		bSuccess = false;
		BYTE szRAWCommand[] = {0x4b,0xfa,0x01,0x00,0x00,0x00,0x00,0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CReadHWIDRequestForOrange::~CReadHWIDRequestForOrange()
	{
	}

	void CReadHWIDRequestForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[100] = {0};
		int i = ptBufferReceived->GetLength();
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		memcpy(szHWID_Get,szbuffer + 7,80);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//Read FA
	CReadFARequestForOrange::CReadFARequestForOrange (char* szOffset,char* szFA, bool &bSuccess)
		: CDiagRequestBase (_T("CReadFARequestForOrange"))
		, m_Success (bSuccess)
		,szFA_Get(szFA)
	{

		bSuccess = false;
		BYTE szRAWCommand[] = {0x4b,0xfa,0x02,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CReadFARequestForOrange::~CReadFARequestForOrange()
	{
	}

	void CReadFARequestForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[1024] = {0};

		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		memcpy(szFA_Get,szbuffer + 7,512);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//Write FA
	CWriteFARequestForOrange::CWriteFARequestForOrange (char* szOffset,char* szData, bool &bSuccess)
		: CDiagRequestBase (_T("CWriteFARequestForOrange"))
		, m_Success (bSuccess)
	{

		//Currently,one must write 512 bytes once,unreasonable!
		//assert(sizeof(szData) == 512);
		char* pData = szData;
		//head+1,flag+1,command+2,comID+2,payload+2,offset+2,datalen+2＃＃data  
		BYTE szRAWCommand[528] = {0x4b,0xfa,0x03,0x00,0x00,0x00,0x10,0x02,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		for (int i = 16; i<528; i++)
		{
			szRAWCommand[i] = (byte)(*pData);
			pData++;
		}
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend); 
		
	}

	CWriteFARequestForOrange::~CWriteFARequestForOrange()
	{
	}

	void CWriteFARequestForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//CheckSWVersionForOrange
	CCheckSWVersionForOrange::CCheckSWVersionForOrange (char* szData, bool &bSuccess)
		: CDiagRequestBase (_T("CCheckSWVersionForOrange"))
		, m_Success (bSuccess)
		,szData_Get(szData)
	{

		bSuccess = false;
		BYTE szRAWCommand[] = {0x4b,0xfa,0x04,0x00,0x00,0x00,0x00,0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCheckSWVersionForOrange::~CCheckSWVersionForOrange()
	{

	}

	void CCheckSWVersionForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		memcpy(szData_Get,szbuffer + 7,200);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//Coin Charge
	CCoinChargeForOrange::CCoinChargeForOrange (bool bFlag, bool &bSuccess)
		: CDiagRequestBase (_T("CCoinChargeForOrange"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
		BYTE szRAWCommand[8] = {0x4b,0xfa,0x05,0x00,0x00,0x00,0x00,0x00};
		if(bFlag)
		{
            szRAWCommand[4] = 0x01;
		}


		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCoinChargeForOrange::~CCoinChargeForOrange()
	{
	}

	void CCoinChargeForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//CBatteryChargeForOrange
	CBatteryChargeForOrange::CBatteryChargeForOrange (bool bFlag, bool &bSuccess)
		: CDiagRequestBase (_T("CBatteryChargeForOrange"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
		BYTE szRAWCommand[8] = {0x4b,0xfa,0x06,0x00,0x00,0x00,0x00,0x00};
		if(bFlag)
		{
			szRAWCommand[4] = 0x01;
		}

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBatteryChargeForOrange::~CBatteryChargeForOrange()
	{
	}

	void CBatteryChargeForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
   
	//CFileSystemBackupForOrange
	CFileSystemBackupForOrange::CFileSystemBackupForOrange (bool &bSuccess,char* szUserData)
		: CDiagRequestBase (_T("CFileSystemBackupForOrange"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
		BYTE szRAWCommand[84] = {0};
		szRAWCommand[0] = 0x4b;
		szRAWCommand[1] = 0xfa;
		szRAWCommand[2] = 0x07;
		szRAWCommand[3] = 0x00;

		char* pData = szUserData;
		for (int i = 4; i< 44; i++)
		{
			szRAWCommand[i] = (byte)(*pData);
			pData++;
		}
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CFileSystemBackupForOrange::~CFileSystemBackupForOrange()
	{
	}

	void CFileSystemBackupForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//Read FA Sector 
	CReadFASectorRequestForOrange::CReadFASectorRequestForOrange (char* szOffset,char* szFA,char* szSectorSize, bool &bSuccess)
		: CDiagRequestBase (_T("CReadFASectorRequestForOrange"))
		, m_Success (bSuccess)
		,szFA_Get(szFA)
	{

		bSuccess = false;
		BYTE szRAWCommand[16] = {0x4b,0xfa,0x02,0x00,0x00,0x00,0x08,0x00}; 
		//4 bytes offset
		unsigned int i_Offset = atoi(szOffset) * 512;
		unsigned char szOffset_Temp[4] = {0};
		szOffset_Temp[0] = (unsigned char)i_Offset;
		szOffset_Temp[1] = (unsigned char)(i_Offset >> 8);
		szOffset_Temp[2] = (unsigned char)(i_Offset >> 16);
		szOffset_Temp[3] = (unsigned char)(i_Offset >> 24);

		memcpy(szRAWCommand+8,szOffset_Temp,4);

		//the length(512 bytes) read fa 

        szRAWCommand[12] = 0x00;
		szRAWCommand[13] = 0x02;
		szRAWCommand[14] = 0x00;
		szRAWCommand[15] = 0x00;


		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CReadFASectorRequestForOrange::~CReadFASectorRequestForOrange()
	{
	}

	void CReadFASectorRequestForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[1024] = {0};

		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		memcpy(szFA_Get,szbuffer + 7,512);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//Write FA Sector
	CWriteFASectorRequestForOrange::CWriteFASectorRequestForOrange (char* szOffset,char* szData,char* szSectorSize, bool &bSuccess)
		: CDiagRequestBase (_T("CWriteFARequestForOrange"))
		, m_Success (bSuccess)
	{

		//Currently,one must write 512 bytes once,unreasonable!
		char* pData = szData;
		//head+1,flag+1,command+2,comID+2,payload+2,offset+2,datalen+2＃＃data  
		BYTE szRAWCommand[528] = {0x4b,0xfa,0x03,0x00,0x00,0x00,0x10,0x02}; 
		//4 bytes offset
		unsigned int i_Offset = atoi(szOffset) * 512;
		unsigned char szOffset_Temp[4] = {0};
		szOffset_Temp[0] = (unsigned char)i_Offset;
		szOffset_Temp[1] = (unsigned char)(i_Offset >> 8);
		szOffset_Temp[2] = (unsigned char)(i_Offset >> 16);
		szOffset_Temp[3] = (unsigned char)(i_Offset >> 24);

		memcpy(szRAWCommand+8,szOffset_Temp,4);
        //the length(512 bytes) Write fa 
		szRAWCommand[12] = 0x00;
		szRAWCommand[13] = 0x02;
		szRAWCommand[14] = 0x00;
		szRAWCommand[15] = 0x00;

		memcpy(szRAWCommand + 16,pData,512);

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend); 

	}

	CWriteFASectorRequestForOrange::~CWriteFASectorRequestForOrange()
	{
	}

	void CWriteFASectorRequestForOrange::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[255] = {0};
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	// Read Coin Cell Voltage For Cairo [5/31/2012 Kevin.Dong]
	CReadCoinCellVoltageForCairo::CReadCoinCellVoltageForCairo (char* szVoltage, bool &bSuccess)
		: CDiagRequestBase (_T("CReadCoinCellVoltageForCairo"))
		, m_Success (bSuccess)
		,szVoltage_Get(szVoltage)
	{

		bSuccess = false;
		BYTE szRAWCommand[] = {0x4b,0xfa,0x0a,0x00,0x00,0x00,0x00,0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CReadCoinCellVoltageForCairo::~CReadCoinCellVoltageForCairo()
	{
	}

	void CReadCoinCellVoltageForCairo::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szbuffer[100] = {0};
		int i = ptBufferReceived->GetLength();
		memcpy(szbuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_Success = szbuffer[4] == 0;

		memcpy(szVoltage_Get,szbuffer + 7,80);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}





































































































































































	CEFsBackupCheckRequest::CEFsBackupCheckRequest (bool &bIsExit)
		: CDiagRequestBase (_T("CEFsBackupCheckRequest"), 5000, false)
		, m_IsExit (bIsExit)
	{

		bIsExit = false;
		
		BYTE szRAWCommand[] = {0x4B, 0xFA, 0x83, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsBackupCheckRequest::~CEFsBackupCheckRequest()
	{
	}

	void CEFsBackupCheckRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szBuffer[5] = {0};
		memcpy(szBuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		//m_IsExit = szBuffer[4] == 0;
		if(szBuffer[4] == 1)
			m_IsExit = true;
		else
			m_IsExit = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CGoldenCopyCheckRequest::CGoldenCopyCheckRequest (bool &bSuccess, int &nStatus)
		: CDiagRequestBase (_T("CGoldenCopyCheckRequest"))
		, m_Status (nStatus)
		, m_Success(bSuccess)
	{
		m_Success = false;

		//sendrawrequest 0x4B 0xFA 0x84 0x00 0x00 0x80 0x06 0x00 0x00 0x00 0x00 0x00
		BYTE szRAWCommand[] = {0x4B, 0xFA, 0x84, 0x00, 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGoldenCopyCheckRequest::~CGoldenCopyCheckRequest()
	{
	}

	void CGoldenCopyCheckRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szBuffer[12] = {0};
		memcpy(szBuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		//正確回傳:
		//0x4B FA 84 00 00 80 06 00 00
		//錯誤回傳:
		//0x4B FA 84 00 00 80 06 00 03
		m_Status = szBuffer[8];
		m_Success = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}




}

