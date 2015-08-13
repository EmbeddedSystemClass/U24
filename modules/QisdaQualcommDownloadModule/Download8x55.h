#ifndef __DL8x55__H__
#define __DL8x55__H__
#pragma once

//#include "..\..\Lib\RequestUtility\InterfaceBase.h"
//#include "..\..\Lib\RequestUtility\DownloadCommand.h"
//#include "..\..\RequestManager\Request\DownloadRequest.h"
//#include "..\..\CommonUtil\RefObject.h"
#include <vector>
//#include "..\..\CommonUtil\NotifyBase.h"
//#include "..\..\CommonUtil\ErrorCode.h"
#include "Download.h"

#pragma warning(push)
#pragma warning(disable:4251)

/************************************************************************/
/* 8x55 fine tune data                                                  */
/************************************************************************/
const int g_n8x55NopRetryTimes		   		= 30;
const int g_n8x55NopTimeout			   		= 1000;
const int g_n8x55HelloRetryTimes	   		= 200;
const int g_n8x55HelloTimeout		   		= 700;
const int g_n8x55ReOpenAfterGoSleepTime		= 35000;
const int g_n8x55ReOpenAfterResetSleepTime	= 40000;
const int g_n8x55ReOpenAfterResetSleepTime_Path = 30000;
const int g_n8x55CloseImageTimeout			= 300000;
/************************************************************************/

//for 8x55
class QISDA_DOWNLOAD_CLASS_API CDownload8x55 : public CDLFormatCommand, public CRefObject
{
public:
	CDownload8x55(const TSTRING& strDevice, const std::string& strPlatform = "Qualcomm8x55", bool bReopenAfterReset = false);
	CDownload8x55(int nCOMPort, const std::string& strPlatform = "Qualcomm8x55", bool bReopenAfterReset = false);
	virtual ~CDownload8x55(void);
public: //Setting
	void SetIsOveridePartition(bool bOverridePartition);
	void SetResetMode(bool bReset);
	// Set File Path
	bool SetHex(const std::string& strFilePath);
	bool SetPartition(const std::string& strFilePath);	
//jy
	bool Set_EMMC_USER(const std::string& strFilePath);	
//jy
	//SecureOSBL 20110817 Dave.Wu
	bool Set_SecureOSBL(const std::string& strFilePath);	

public:
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	virtual bool DMSSDownload(const std::string& strHexFilePath = "" /*"" means to use m_strHEX*/);
	virtual bool StreamingDL(bool bSpeedUp = true,DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
	//20110817 Dave.Wu
	virtual bool SecureOSBLDL(bool bSpeedUp = true, DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD);
protected:
	std::string m_strHEX;
	std::string m_strPartition;
//jy
	std::string m_str_EMMC_USER;
//jy
	//SecureOSBL 20110817 Dave.Wu
	std::string m_str_SecureOSBL;

	bool	m_bReopenAfterReset; // For QDownload
	bool    m_bOverridePartition;
	bool    m_bResetMode;
public:
	//dave
	bool SwitchToDL();
	bool ReopenAfterReset(int nRetryTimes = 10, int nTimeout=g_nTimeout);
	bool ReopenAfterResetByStreaming(int nRetryTimes = 10, int nTimeout=g_nTimeout);
	//dave
};

typedef CRefObjectPtr<CDownload8x55> CDownload8x55Ptr;

#pragma warning(pop)
#endif // __DL8x55__H__