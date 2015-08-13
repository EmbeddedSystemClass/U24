#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
//#include "..\..\CommonUtil\unicodeutility.h"
//#include "..\..\CommonUtil\PIMStringUtility.h"
//#include "..\..\odmtma_api\DownloadDLL\HexFile.h"
//#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "Download8x55.h"
//#include <io.h>

//////////////////////////////8x55////////////////////////////////////////////
CDownload8x55::CDownload8x55(const TSTRING& strDevice, const std::string& strPlatform, bool bReopenAfterReset)
:CDLFormatCommand(strDevice, strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(bReopenAfterReset)
{
}
CDownload8x55::CDownload8x55(int nCOMPort, const std::string& strPlatform, bool bReopenAfterReset)
:CDLFormatCommand(COMDeviceName(nCOMPort), strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(bReopenAfterReset)
{
}
CDownload8x55::~CDownload8x55(void)
{
}
bool CDownload8x55::SetHex(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strHEX = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload8x55::SetPartition(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPartition = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload8x55::Set_EMMC_USER(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_str_EMMC_USER = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload8x55::Set_SecureOSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_str_SecureOSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}

void CDownload8x55::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}
void CDownload8x55::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}
bool CDownload8x55::CFCDownload(bool bSpeedup)
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	return bRes;
}
bool CDownload8x55::Download(bool bSpeedup, bool bReopenAfterReset, DownloadProtocol nDLPROTOCOL)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode(); 
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}
bool CDownload8x55::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n8x55NopRetryTimes,g_n8x55NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}
bool CDownload8x55::StreamingDL(bool bSpeedUp,DownloadProtocol nDLPROTOCOL)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	//CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = true;
	/*
	if(GetPlatorm() == "Qualcomm8x55")
	{
		bReconnect = ReopenAfterGo.ReOepnComport(g_n8x55ReOpenAfterGoSleepTime, false);
		AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	}
	else
	{
		Sleep(10000);
	}
	ASSERT(bReconnect);
	*/
	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n8x55HelloRetryTimes, g_n8x55HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		//JY
		StreamingDLMultiImage(OPEN_MULTI_MODE_EMMC_USER_PARTITION,  m_str_EMMC_USER,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n8x55CloseImageTimeout) &&
		//JY
		StreamingReset(bSpeedUp))
	{
		bRes = true;
		
		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n8x55ReOpenAfterResetSleepTime_Path, false);
			//ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bReconnect);
		}
		else
		{
			bReconnect = true;
		}
		
	}

	return bRes && bReconnect;
}
//dave
bool CDownload8x55::SwitchToDL()
{
	return DMSSNop(g_n8x55NopRetryTimes,g_n8x55NopTimeout);// (1, g_n8x55NopTimeout);
}

bool CDownload8x55::ReopenAfterReset(int nRetryTimes, int nTimeout)
{
	bool bReconnect = true;
	
	bReconnect = DMSSReset(nRetryTimes, nTimeout);
	
	if(bReconnect)
	{
		Sleep(40000);

		CReOpenComport ReopenAfterReset(this);
		bReconnect = ReopenAfterReset.ReOepnComport(g_n8x55ReOpenAfterResetSleepTime_Path, false);
		AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
		SetLastError(DL_ERROR_ReOpenComport, bReconnect);
	}
	
	return bReconnect;
}

bool CDownload8x55::ReopenAfterResetByStreaming(int nRetryTimes, int nTimeout)
{
	bool bReconnect = true;
	
	//Modify from DMSSReset to StreamingReset, due to sucureOSBLDL don't take DMSSReset 20110819 Dave.Wu
	//Don't remove StreamingHello
	bReconnect = this->StreamingHello(false,  nRetryTimes, nTimeout);
		     this->StreamingReset(false, nRetryTimes, nTimeout);
	
	if(bReconnect)
	{
		Sleep(40000);

		CReOpenComport ReopenAfterReset(this);
		bReconnect = ReopenAfterReset.ReOepnComport(g_n8x55ReOpenAfterResetSleepTime_Path, false);
		AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
		SetLastError(DL_ERROR_ReOpenComport, bReconnect);
	}
	
	return bReconnect;
}
//dave

//SecureOSBL_DL 20110817 Dave.Wu
bool CDownload8x55::SecureOSBLDL(bool bSpeedUp, DownloadProtocol nDLPROTOCOL)
{
	using namespace PLATFORM_TOOL;

	if( StreamingHello(bSpeedUp, g_n8x55HelloRetryTimes, g_n8x55HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_EMMC_USER_PARTITION,  m_str_SecureOSBL,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n8x55CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{		
		return true;
	}

	return false;
}