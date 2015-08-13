
#include "StdAfx.h"
#include "Download8X25.h"

CCriticalSection CDownload8X25::m_obj_loadcheckSumCritSection;
CCriticalSection CDownload8X25::m_obj_loadImageVerCritSection;

CDownload8X25::CDownload8X25(const TSTRING& str_deviceName, const std::string& str_platform)
:CDLFormatCommand(str_deviceName, str_platform), m_b_reOpenAfterReset(false), m_b_resetMode(true), m_b_overridePartition(false), m_p_adbDevice(NULL)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	m_i_rework = 0;
	m_i_reboot = 0;
	m_i_Downgrade = 1;
	m_i_DDRCheck = 0;
	m_i_checkSum = 0;
	m_i_backupNV = 0;
	m_i_SupportQDownload = 0;
	m_i_SupportADBToBootloader = 0;
	m_i_eraseModem_st1 = 0;
	m_i_eraseModem_st2 = 0;
	m_i_eraseUserFat = 0;
	m_i_DisplayScreenColor = 0;
	m_map_COMPicasso.clear();
	m_map_imageNameCheckSum.clear();

	m_i_CheckHWVersion = 0;
}

CDownload8X25::CDownload8X25(int i_COMPort, const std::string& str_platform)
:CDLFormatCommand(COMDeviceName(i_COMPort), str_platform), m_b_reOpenAfterReset(false), m_b_resetMode(true), m_b_overridePartition(false), m_p_adbDevice(NULL)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	m_i_rework = 0;
	m_i_reboot = 0;
	m_i_Downgrade = 1;
	m_i_DDRCheck = 0;
	m_i_checkSum = 0;
	m_i_backupNV = 0;
	m_i_SupportQDownload = 0;
	m_i_SupportADBToBootloader = 0;
	m_i_eraseModem_st1 = 0;
	m_i_eraseModem_st2 = 0;
	m_i_eraseUserFat = 0;
	m_i_DisplayScreenColor = 0;
	m_map_COMPicasso.clear();
	m_map_imageNameCheckSum.clear();

	m_i_CheckHWVersion = 0;
}

CDownload8X25::~CDownload8X25(void)
{
	if (m_p_adbDevice) 
	{
		delete m_p_adbDevice;
		m_p_adbDevice = NULL;
	}
}

/*****************************************************************************
* Function name: MultiStartDownload      
* Summary      : Execute download (Hex DL + fastboot DL)
* Parameters   : b_speedUp = false; nDLPROTOCOL = QTT_FASTDOWNLOAD;
* Return       : true : success.
*                false: failure.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19            First version             
*****************************************************************************/
bool CDownload8X25::MultiDownload(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL) 
{
	bool b_result = false;

	if (m_str_DLMode != DOWNLOAD_MODE_PREDL && m_str_DLMode != DOWNLOAD_MODE_REDL)
	{
		AddMsg("DL mode is not PreDL or ReDL.", DownloadStep::None, 100);
		return false;
	}

	if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	{
		/* Hex Download and fastboot image download ,change to fastboot mode */
		b_result = Download(b_speedUp, b_reOpenAfterReset, nDLPROTOCOL);
		if (!b_result) 
		{
			return false;
		}
	}

	/* Fastboot Download */

	if (!GetADB()) 
	{
		return false;
	}

	if (m_i_SupportADBToBootloader)
	{
		CString str_devicename = m_p_adbDevice->GetDeviceName().c_str();
		if(!str_devicename.GetLength())
			return false;
		str_devicename.MakeUpper();
		if(str_devicename.Find("PID_3032",0)==-1 && str_devicename.Find("PID_D00D",0)==-1)
		{
			AddMsg("Start Reboot to BootLoader...", DownloadStep::None, 100);
			// Marked by Lennon 2013.05.08
			//*******************************************************************************
			//if(m_p_adbDevice->AdbReboot(1))
			//if use m_p_adbDevice to reboot from adb to fb,Read Response must use ADB way.
			//Lennon has confirmed they are different via usb monitor. (may endpoint etc...)
			//*******************************************************************************
			m_p_adbDevice->~ADBDevice();
			if (RebootToBootLoader())
			{
				m_p_adbDevice = NULL;
				KillADB( );
				Sleep(5000);
				if (!GetADB()) 
				{
					m_str_errorCode = CommErr_Get_ADB_Fail;
					return false;
				}
				AddMsg("Reboot to BootLoader Successfully!", DownloadStep::None, 100);

			}
			else
			{
				AddMsg("Reboot to BootLoader Failure!", DownloadStep::None, 100);
				return false;
			}
		}

	}

	if (m_i_DisplayScreenColor == 1)
	{
		//Start& During Fasboot Download, show Green Screen
		if (!SetDislayColor("display-blue"))
		{
			//display-blue
			return false;
		}
	}

	b_result = FastbootDownload();
	if(b_result)
	{
		if ( m_i_DisplayScreenColor == 1 )
		{
			//Download Success, show Green Screen
			if (!SetDislayColor("display-green"))
			{
				//display-green
				return false;
			}
		}

		if (m_i_reboot == 1)
		{
			if (!Reboot()) 
			{
				m_str_errorCode = CommErr_Reset_Phone_Fail;
				return false;
			}
		} 

	}
	else
	{
		if (m_i_DisplayScreenColor == 1)
		{
			//Download Failure, show Red Screen
			if (!SetDislayColor("display-red"))
			{
				//display-red
				return false;
			}
		}
	}
	return b_result;
}

/*****************************************************************************
* Function name: Download     
* Summary      : DMSSDL + StreamingDL
* Parameters   : 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::Download(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL) 
{
	AddMsg("Hex Download", None, 100);
	
	m_b_reOpenAfterReset = b_reOpenAfterReset;

	/* DMSS DL + Streaming DL */
	if (DMSSDL() && StreamingDL(b_speedUp, nDLPROTOCOL))
	{
	} 
	else
	{
		if(m_b_resetMode) 
		{
			/* Init PotocolType to NONE mode */
			CInterfaceBase::ResetMode();
		}
		AddMsg("Hex Download Fail.", None, 100);
		return false;
	}

	/* DDR Check */
	if (m_i_DDRCheck == 1)
	{
		bool b_reConnect = false;
		CReOpenComport obj_ReOpenComport(this, true);
		b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterReset, false, 15, 1000);
		if (b_reConnect == false) 
		{
			m_str_errorCode = DLErr_Setting_Open_Fail;
			return false;
		}

		WORD dw_address = 0;
		if(!DMSSOpenAndWriteHexFile(m_str_hex, dw_address)) 
		{
			m_str_errorCode = DLErr_DL_Hex_Fail;
			return false;
		}

		if(!DMSSGo(dw_address, 0)) 
		{
			m_str_errorCode = DLErr_DL_Hex_Fail;
			return false;
		}

		b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterGo, false, 15, 1000);
		AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
		if (b_reConnect == false) 
		{
			m_str_errorCode = DLErr_Setting_Open_Fail;
			return false;
		}

		if (!StreamingHello(b_speedUp, g_i_8X25HelloRetryTimes, g_i_8X25HelloTimeout, nDLPROTOCOL)) 
		{
			m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
			return false;
		}

		if (!StreamingSecurity("", b_speedUp)) 
		{
			m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
			return false;
		}

		/* Get DDR check result */
		if (!ReadDDRTestReslult())
		{
			m_str_errorCode = FunErr_DDR_Size_Fail;
			return false;
		}

		if (!StreamingReset(b_speedUp)) 
		{
			m_str_errorCode = DLErr_DL_Streaming_Reset_Fail;
			return false;
		}
	}

	AddMsg("Hex Download Success.", None, 100);
	return true;
}
/*****************************************************************************
* Function name: DMSSDL     
* Summary      : DMSSDL
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
//bool CDownload8X25::DMSSDL_ReDL(void)
//{
//	using namespace PLATFORM_TOOL;
//	
//	if (!DMSSNop(g_i_8X25NopRetryTimes, g_i_8X25NopTimeout)) 
//	{
//		m_str_errorCode = DLErr_DL_Dmss_Nop_Fail;
//		return false;
//	}
//
//	if (!DMSSParameter()) 
//	{
//		m_str_errorCode = DLErr_DL_Dmss_Nop_Fail;
//		return false;
//	}
//
//	/* PreDL */
//	//Remark by Lennon for Dubai Test 2013.01.09
//	//if (!SetFastBootFlag())
//	//{
//	//	m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
//	//	return false;
//	//}
//	
//	//string str_hex = m_str_HexImagePath + _T("\\MPRG8660.hex");
//	string str_hex = m_str_hex;
//
//	WORD dw_address = 0;
//	if(!DMSSOpenAndWriteHexFile(str_hex, dw_address)) 
//	{
//		m_str_errorCode = DLErr_DL_Hex_Fail;
//		return false;
//	}
//
//	if(!DMSSGo(dw_address, 0)) 
//	{
//		m_str_errorCode = DLErr_DL_Hex_Fail;
//		return false;
//	}
//
//	bool b_speedUp = false;
//	bool b_reConnect = false;
//	CReOpenComport obj_ReOpenComport(this, true);
//	b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterGo, false, 15, 1000);
//	AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
//
//	if (b_reConnect == false) 
//	{
//		m_str_errorCode = DLErr_Setting_Open_Fail;
//		return false;
//	}
//
//	if (!StreamingHello(b_speedUp, g_i_8X25HelloRetryTimes, g_i_8X25HelloTimeout, QTT_FASTDOWNLOAD)) 
//	{
//		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
//		return false;
//	}
//
//	if (!StreamingSecurity("", b_speedUp)) 
//	{
//		m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
//		return false;
//	}
//	///* PreDL */
//	//if (!SetFastBootFlag())
//	//{
//	//	m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
//	//	return false;
//	//}
//	
//	if (!SendOEM_CMD())
//	{
//		m_str_errorCode = DLErr_DL_Send_OEMCMD_Fail;
//		return false;
//	}	
//
//	if (!SendReset_CMD())
//	{
//		m_str_errorCode = DLErr_DL_Send_Reset_Fail;
//		return false;
//	}	
//
//	/*if (!StreamingReset(b_speedUp)) 
//	{
//		m_str_errorCode = DLErr_DL_Streaming_Reset_Fail;
//		return false;
//	}*/
//	
//	if (m_b_reOpenAfterReset) 
//	{
//		bool b_res = false;
//		CReOpenComport obj_ReOpenComport(this);
//		b_res = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterReset, false, 15, 1000);
//		AddMsg(b_res, "ReOpenComPortAfterReset", ReOpenComport);
//		if (b_res == false)
//		{
//			return false;
//		} 
//	}	
//
//	return true;
//}

/*****************************************************************************
* Function name: DMSSDL     
* Summary      : DMSSDL
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::DMSSDL(void)
{
	using namespace PLATFORM_TOOL;

	if (!DMSSNop(g_i_8X25NopRetryTimes, g_i_8X25NopTimeout)) 
	{
		m_str_errorCode = DLErr_DL_Dmss_Nop_Fail;
		return false;
	}

	if (!DMSSParameter()) 
	{
		m_str_errorCode = DLErr_DL_Dmss_Nop_Fail;
		return false;
	}

	/* PreDL */
	// Remark by Lennon for Dubai Test 2013.01.09
	//if (!SetFastBootFlag())
	//{
	//	m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
	//	return false;
	//}

	WORD dw_address = 0;
	if(!DMSSOpenAndWriteHexFile(m_str_hex, dw_address)) 
	{
		m_str_errorCode = DLErr_DL_Hex_Fail;
		return false;
	}
	
	// update by Lennon 2013.01.15 for dubai short solution
	WORD nwoffset = 0;
	if (dw_address == 0x8001) // update by Lennon 2013.01.15 for dubai short solution
		nwoffset = 0x3000;
	if(!DMSSGo(dw_address, nwoffset)) 
	{
		m_str_errorCode = DLErr_DL_Hex_Fail;
		return false;
	}
	return true;
}

/*****************************************************************************
* Function name: StreamingDL     
* Summary      : StreamingDL
* Parameters   : default
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::StreamingDL(bool b_speedUp, DownloadProtocol nDLPROTOCOL)
{
	using namespace PLATFORM_TOOL;

	bool b_res = false;
	bool b_reConnect = false;

	CReOpenComport obj_ReOpenComport(this, true);
	b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterGo, false, 15, 1000);
	AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
	if (b_reConnect == false) 
	{
		m_str_errorCode = DLErr_Setting_Open_Fail;
		return false;
	}

	if (!StreamingHello(b_speedUp, g_i_8X25HelloRetryTimes, g_i_8X25HelloTimeout, nDLPROTOCOL)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
		return false;
	}

	if (!StreamingSecurity("", b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
		return false;
	}

	/* DDR Check */
	if (m_i_DDRCheck == 1)
	{
		if (!DDRTest())
		{
			m_str_errorCode = FunErr_DDR_Size_Fail;
			return false;
		}
	}

	if (!StreamingDLMultiImage(OPEN_MULTI_MODE_EMMC_USER_8X25,  m_str_msimage,  b_speedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_i_8X25CloseImageTimeout)) 
	{
		m_str_errorCode = DLErr_DL_MsImage_Fail;
		return false;
	}

	if (!StreamingReset(b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Reset_Fail;
		return false;
	}

	if (m_b_reOpenAfterReset) 
	{
		CReOpenComport obj_ReOpenComport(this);
		b_res = obj_ReOpenComport.ReOepnComport(g_i_8X25ReOpenComPortAfterReset, false, 15, 1000);
		AddMsg(b_res, "ReOpenComPortAfterReset", ReOpenComport);
		if (b_res == false)
		{
			return false;
		} 
	}
	else 
	{
		b_res = true;
	}

	return b_res;
}

/*****************************************************************************
* Function name: SetFastBootFlag      
* Summary      : Transfer COM port to device.(fastboot download)
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::SetFastBootFlag(void)
{
	using namespace PLATFORM_TOOL;

	bool b_res = false;
	bool b_success = false;

	/* Get Device COM port */
	CReOpenComport obj_ReOpenComport(this);
	int i_comport = obj_ReOpenComport.GetComport();

	AddMsg("SetFastBootFlag", None, 0);

	CRequestPtr ptRequest = new CSetFastBootFlagQCSRequestFor8X25(b_success, i_comport);
	b_res = Execute(ptRequest) && b_success;
	if (b_res) 
	{
		AddMsg("SetFastBootFlag Success", None, 100);
	} 
	else 
	{
		AddMsg("SetFastBootFlag Failure", None, 100);
	}

	return b_res;
}
/*****************************************************************************
* Function name: SendOEM_CMD      
* Summary      : Send OEM CMD.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Macl.Ma     2012/07/19          First version             
*****************************************************************************/
bool CDownload8X25::SendOEM_CMD(void)
{
	using namespace PLATFORM_TOOL;

	AddMsg("Send OEM CMD.", None, 0);

	bool b_res = false;
	CRequestPtr ptRequest = new CSendOEMCMD();
	CBufferPtr ptBuffer = new CBuffer;
	b_res = Execute(ptRequest) && ptRequest->GetReceiveData(ptBuffer);
	if (!b_res) 
	{
		AddMsg("Send OEM CMD Fail.", None, 100);
		return false;
	} 

	AddMsg("Send OEM CMD Success.", None, 100);
	return true;
}
/*****************************************************************************
* Function name: SendReset_CMD      
* Summary      : Send Reset CMD.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Macl.Ma     2012/07/19          First version             
*****************************************************************************/
bool CDownload8X25::SendReset_CMD(void)
{
	using namespace PLATFORM_TOOL;

	AddMsg("Send Reset CMD.", None, 0);

	bool b_res = false;
	CRequestPtr ptRequest = new CSendResetCMD();
	CBufferPtr ptBuffer = new CBuffer;
	b_res = Execute(ptRequest) && ptRequest->GetReceiveData(ptBuffer);
	if (!b_res) 
	{
		AddMsg("Send Reset CMD Fail.", None, 100);
		return false;
	} 

	AddMsg("Send Reset CMD Success.", None, 100);
	return true;
}
/*****************************************************************************
* Function name: DDRTest      
* Summary      : DDR Check.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/28          First version             
*****************************************************************************/
bool CDownload8X25::DDRTest(void)
{
	using namespace PLATFORM_TOOL;

	AddMsg("DDR Test.", None, 0);

	bool b_res = false;
	CRequestPtr ptRequest = new CDDRTest();
	CBufferPtr ptBuffer = new CBuffer;
	b_res = Execute(ptRequest) && ptRequest->GetReceiveData(ptBuffer);
	if (!b_res) 
	{
		AddMsg("DDR Test Fail.", None, 100);
		return false;
	} 
	
	AddMsg("DDR Test Success.", None, 100);
	return true;
}

/*****************************************************************************
* Function name: ReadDDRTestReslult      
* Summary      : Get DDR check result.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/28          First version             
*****************************************************************************/
bool CDownload8X25::ReadDDRTestReslult(void)
{
	using namespace PLATFORM_TOOL;

	AddMsg("Get DDR Test Result.", None, 0);

	bool b_res = false;
	CRequestPtr ptRequest = new CGetDDRTestResult();
	CBufferPtr ptBuffer = new CBuffer;
	b_res = Execute(ptRequest) && ptRequest->GetReceiveData(ptBuffer);
	if (!b_res) 
	{
		AddMsg("Get DDR Test Result Fail.", None, 100);
		return false;
	}

	/* Check Result */
	BYTE* lpData = (BYTE*)(ptBuffer->GetData());
	if(lpData)
	{
		if (lpData[1] == 0x00)
		{
		}
		else if (lpData[1] == 0x01)
		{
			AddMsg("No DDR attached error.", None, 100);
			return false;
		}
		else if (lpData[1] == 0x02)
		{
			AddMsg("Memory map error.", None, 100);
			return false;
		}
		else if (lpData[1] == 0x03)
		{
			AddMsg("Data line error.", None, 100);
			return false;
		}
		else if (lpData[1] == 0x04)
		{
			AddMsg("Address line error.", None, 100);
			return false;
		}
		else
		{
			AddMsg("other error.", None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Get Data Fail.", None, 100);
		return false;
	}
		
	AddMsg("Get DDR Test Result Success.", None, 100);
	return true;
}

/*****************************************************************************
* Function name: GetErrorCode     
* Summary      : Return error code.
* Parameters   : 
*                
* Return       : 
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19          First version             
*****************************************************************************/
CString CDownload8X25::GetErrorCode(void) 
{
	return m_str_errorCode;
}
/*****************************************************************************
* Function name: GetComport      
* Summary      : Get device COM port.
* Parameters   : void
*                
* Return       : COM port
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
int CDownload8X25::GetComport(void)
{
	CReOpenComport ReopenAfterReset(this);
	return ReopenAfterReset.GetComport();
}

/*****************************************************************************
* Function name: SetIsOveridePartition      
* Summary      : b_overridePartition==true means overside the partiton.
* Parameters   : b_overridePartition : bool
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
void CDownload8X25::SetIsOveridePartition(bool b_overridePartition)
{
	if (b_overridePartition) 
	{
		AddMsg("Set Override Partition ON", None, 100);
	}
	m_b_overridePartition = b_overridePartition;

	return;
}

/*****************************************************************************
* Function name: SetResetMode      
* Summary      : b_reset==true means will reset mode if flashing fails.
* Parameters   : b_reset : bool
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
void CDownload8X25::SetResetMode(bool b_reset)
{
	m_b_resetMode = b_reset;
}

/*****************************************************************************
* Function name: SetImageFilePath      
* Summary      : Set image file path to member variable.
* Parameters   : i_imageType : image type
*                str_filePath: image file path
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19            First version             
*****************************************************************************/
bool CDownload8X25::SetImageFilePath(const int i_imageType, const std::string& str_filePath)
{
	bool b_res = false;

	/* Check Input */
	if(str_filePath.c_str() == _T("")) 
	{
		return false;
	}

	switch(i_imageType)
	{
	case IMAGE_TYPE_8X25::HEX_8X25:
		m_str_hex = str_filePath;
		break;

	case IMAGE_TYPE_8X25::MSIMAGE_8X25:
		m_str_msimage = str_filePath;
		break;
	case IMAGE_TYPE_8X25::PARTITION_8X25:
		m_str_partition = str_filePath;
		break;

	case IMAGE_TYPE_8X25::CFG_DATA_8X25:
		m_str_cfgdata = str_filePath;
		break;

	case IMAGE_TYPE_8X25::QCSBL_8X25:
		m_str_qcsbl = str_filePath;
		break;

	case IMAGE_TYPE_8X25::OEMSBL_8X25:
		m_str_oemsbl = str_filePath;
		break;

	case IMAGE_TYPE_8X25::APPBOOT_8X25:
		m_str_appboot = str_filePath;
		break;

	case IMAGE_TYPE_8X25::BOOT_8X25:
		m_str_boot = str_filePath;
		break;

	case IMAGE_TYPE_8X25::RECOVERY_8X25:
		m_str_recovery = str_filePath;
		break;

	case IMAGE_TYPE_8X25::MODEM_8X25:
		m_str_modem = str_filePath;
		break;

	case IMAGE_TYPE_8X25::SYSTEM_8X25:
		m_str_system = str_filePath;
		break;

	case IMAGE_TYPE_8X25::USERDATA_8X25:
		m_str_userdata = str_filePath;
		break;

	case IMAGE_TYPE_8X25::CACHE_8X25:
		m_str_cache = str_filePath;
		break;

	case IMAGE_TYPE_8X25::PERSIST_8X25:
		m_str_persist = str_filePath;
		break;

	case IMAGE_TYPE_8X25::QGLOG_8X25:
		m_str_qglog = str_filePath;
		break;

	case IMAGE_TYPE_8X25::LOGO_8X25:
		m_str_logo = str_filePath;
		break;

	case IMAGE_TYPE_8X25::MODEM_ST1_8X25:
		m_str_modem_st1 = str_filePath;
		break;

	case IMAGE_TYPE_8X25::MODEM_ST2_8X25:
		m_str_modem_st2 = str_filePath;
		break;

	default:
		return false;
	}

	/* Check image file if exist */
	if(::_taccess(str_filePath.c_str(), 0) == 0) 
	{
		b_res = true;
	} 
	else 
	{
		b_res = false;
	}

	return b_res;
}

/*****************************************************************************
* Function name: FastbootDownload      
* Summary      : fastboot DL
* Parameters   : 
* Return       : true : success.
*                false: failure.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::FastbootDownload(void)
{
	AddMsg("Fastboot Download.", DownloadStep::None, 100);

	/* Get ADB */
	if(m_i_SupportADBToBootloader && !KillADB())
	{
		return false; 
	}
	if (!GetADB()) 
	{
		m_str_errorCode = CommErr_Get_ADB_Fail;
		return false;
	}

	if (m_i_Downgrade == 0)
	{
		if(!GetImageSWVariant())
			return false;
		if(!GetPhoneSWVariant())
			return false;
		if(!CompareVersion())
			return false;
	}

	if (m_i_checkSum == 1)
	{
		/* Read CheckSum file */
		if (!ReadCheckSumFile())
		{
			m_str_errorCode = DLErr_DL_Read_Checksum_File_Fail;
		return false;
		}
		//SetCheckHWVersion(1);
		if(m_i_CheckHWVersion == 1) // Check HW Version
		{
			if(!CheckHWVariant())
			{
				m_str_errorCode = CommErr_Check_HW_ID_Fail;
				return false;
			}
		}
	}


	/* Backup NV partition */
	if (m_i_backupNV == 1)
	{
		if (( (m_str_DLMode == DOWNLOAD_MODE_PREDL) && (m_i_rework == 1) ) || (m_str_DLMode == DOWNLOAD_MODE_REDL))
		{
			if (!BackupNVPartition())
			{
				return false;
			}
		}
	}

	/*============= ADB download all images =========================*/
	int n_MaxRetryCount = 5;

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Partition_Table_Fail;
			return false;
		}
		if (FastbootFlashCmd("partition"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_RPM_Fail;
			return false;
		}
		if (FastbootFlashCmd("cfg_data"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_SBL1_Fail;
			return false;
		}
		if (FastbootFlashCmd("qcsbl"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_SBL2_Fail;
			return false;
		}
		if (FastbootFlashCmd("oemsbl"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Emmc_Appboot_Fail;
			return false;
		}
		if (FastbootFlashCmd("aboot"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Boot_Fail;
			return false;
		}
		if (FastbootFlashCmd("boot"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Recovery_Fail;
			return false;
		}
		if (FastbootFlashCmd("recovery"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Nonhlos_Fail;
			return false;
		}
		if (FastbootFlashCmd("modem"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_System_Fail;
			return false;
		}
		if (FastbootFlashCmd("system"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Userdata_Fail;
			return false;
		}
		if (FastbootFlashCmd("userdata"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Cache_Fail;
			return false;
		}
		if (FastbootFlashCmd("cache"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Persist_Fail;
			return false;
		}
		if (FastbootFlashCmd("persist"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Glog_Fail;
			return false;
		}
		if (FastbootFlashCmd("qglog"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Logo_Fail;
			return false;
		}
		if (FastbootFlashCmd("logo"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Logo_Fail;
			return false;
		}
		if (FastbootFlashCmd("modem_st1"))
			break;
	}

	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			m_str_errorCode = DLErr_DL_Logo_Fail;
			return false;
		}
		if (FastbootFlashCmd("modem_st2"))
			break;
	}

	if ( m_str_modem_st1 == _T("") && m_i_eraseModem_st1 == 1)
	{
		if (!EraseModem_st1())
		{
			m_str_errorCode = DLErr_Erase_User_Fat_Fail;
			return false;
		}
	}

	if ( m_str_modem_st2 == _T("") && m_i_eraseModem_st2 == 1)
	{
		if (!EraseModem_st2())
		{
			m_str_errorCode = DLErr_Erase_User_Fat_Fail;
			return false;
		}
	}

	if (m_i_eraseUserFat == 1)
	{
		if (!EraseUserFat())
		{
			m_str_errorCode = DLErr_Erase_User_Fat_Fail;
			return false;
		}
	}

	if (m_i_backupNV == 1)
	{
		/* Backup NV partition */
		if (!BackupNVPartition())
		{
			return false;
		}
	}

	AddMsg("Fastboot Download Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: FastbootFlashCmd      
* Summary      : Call ADBFlash()
* Parameters   : str_parameter : image partition name
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::FastbootFlashCmd(const std::string& str_parameter)
{
	/* Check input parameter */
	if (str_parameter == _T("")) 
	{
		return false;
	}

	/* Get current image file name */
	std::string str_imageFilePath;
	if (str_parameter == _T("partition"))
	{
		str_imageFilePath = m_str_partition;
	}
	else if (str_parameter == _T("cfg_data")) 
	{
		str_imageFilePath = m_str_cfgdata;
	}
	else if ( str_parameter == _T("qcsbl"))
	{
		str_imageFilePath = m_str_qcsbl;
	}
	else if ( str_parameter == _T("oemsbl")) 
	{
		str_imageFilePath = m_str_oemsbl;
	}
	else if (str_parameter == _T("aboot")) 
	{
		str_imageFilePath = m_str_appboot;
	}
	else if (str_parameter == _T("boot")) 
	{
		str_imageFilePath = m_str_boot;
	}
	else if (str_parameter == _T("recovery")) 
	{
		str_imageFilePath = m_str_recovery;
	}
	else if (str_parameter == _T("modem"))
	{
		str_imageFilePath = m_str_modem;
	}
	else if (str_parameter == _T("system")) 
	{
		str_imageFilePath = m_str_system;
	}
	else if (str_parameter == _T("userdata")) 
	{
		str_imageFilePath = m_str_userdata;
	}
	else if (str_parameter == _T("cache"))
	{
		str_imageFilePath = m_str_cache;
	}
	else if (str_parameter == _T("persist")) 
	{
		str_imageFilePath = m_str_persist;
	}
	else if (str_parameter == _T("qglog")) 
	{
		str_imageFilePath = m_str_qglog;
	}
	else if (str_parameter == _T("logo")) 
	{
		str_imageFilePath = m_str_logo;
	}
	else if (str_parameter == _T("modem_st1")) 
	{
		str_imageFilePath = m_str_modem_st1;
	}
	else if (str_parameter == _T("modem_st2")) 
	{
		str_imageFilePath = m_str_modem_st2;
	}

	/* Check if download this image ( =0 ) */
	if (str_imageFilePath == _T("")) 
	{
		CString str_message;
		str_message.Format("Skip Fastboot download:%s", str_parameter.c_str());
		AddMsg(str_message, DownloadStep::None, 100);
		return true;
	}

	CString str_message;
	str_message.Format("Fastboot download:%s", str_imageFilePath.c_str());
	AddMsg(str_message, DownloadStep::None, 100);

	/* Check image file path name exist */
	if (::_taccess(str_imageFilePath.c_str(), 0) != 0) 
	{
		CString str_message;
		str_message.Format("Image is not exist:%s", str_imageFilePath.c_str());
		AddMsg(str_message, DownloadStep::None, 100);
		return false;
	}

	if (m_i_checkSum == 1)
	{
		/* Write CheckSum */
		if (!WriteCheckSum((LPCTSTR)str_imageFilePath.c_str()))
		{
			return false;
		}
	}

	/*================================ Resolve malloc problem Start =======================================*/
	///* Load image file */
	//unsigned long long i_fileSize = 0;
	//void *p_data = LoadFile((LPCTSTR)str_imageFilePath.c_str(), &i_fileSize);
	//if (!p_data)
	//{
	//	CString str_message;
	//	str_message.Format("Load image file Fail:%s", str_imageFilePath.c_str());
	//	AddMsg(str_message, DownloadStep::None, 100);
	//	return false;
	//}
    //
	///* Flash to Mobile */
	//if (!ADBFlash(QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), p_data, i_fileSize))
	//{
	//	free(p_data);
	//	return false;
	//}
	//free(p_data);

	/* ADB flash */
	if (!ADBFlashAll(QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), (LPCTSTR)str_imageFilePath.c_str()))
	{
		return false;
	}
	/*================================ Resolve malloc problem End =======================================*/

	str_message.Format("Fastboot download Success:%s", str_imageFilePath.c_str());
	AddMsg(str_message, DownloadStep::None, 100);
	return true;
}


/*****************************************************************************
* Function name: ADBFlash      
* Summary      : Execute to fastboot download.
* Parameters   : sz_imageName : image file path name.
*                p_data       : image file content.
*                l_fileLen    : image file content size length.
* Return       : true : success.
*                false: failure.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8X25::ADBFlash(const char* sz_parmeter, const void* p_data, unsigned long long l_fileLen)
{
	/* Check input parameter */
	if ((sz_parmeter == NULL) || (p_data == NULL) || (l_fileLen <= 0)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	bool b_result = false;
	unsigned long long l_totalLen = l_fileLen;

	/* Show UI info */
	char sz_imgae[64] = {0};
	sprintf(sz_imgae, "Fastboot download:%s", sz_parmeter);

	/* download protocol --- "download:%08x"  example:"download:00162c00" */
	char sz_command[64] = {0};
	sprintf(sz_command, "download:%08x", l_fileLen);

	/* Write download protocol */
	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		/* ADB Read --- 如果返回DATA，则可以发送数据  */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (nRet == RESPONSE::DATA) 
		{
			char* c_p_data = (char*) p_data;
			unsigned long long l_count = 0;
			bool b_writenSuccess = true;
			int i_progress = 0;

			/* Max write byte:4096 */
			while (l_fileLen > 0 && b_writenSuccess) 
			{
				i_progress = (l_count * 100) / l_totalLen;
				AddMsg(sz_imgae, DownloadStep::ADBProtocol, i_progress);
				int xfer = (l_fileLen > 4096) ? 4096 : l_fileLen;

				/* ADB Write */
				b_writenSuccess = m_p_adbDevice->Write(c_p_data, xfer, 60000);
				if (!b_writenSuccess)
				{
					break;
				}
				l_count   += xfer;
				l_fileLen -= xfer;
				c_p_data  += xfer;
			};

			if (!b_writenSuccess)
			{
				AddMsg("Write download bytes to partition Failure", DownloadStep::None, 100);
			}

			char szOutput[2048] = {0};
			int nRet = m_p_adbDevice->ReadResponse_Msg(120000, szOutput);
			AddMsg(szOutput, DownloadStep::None, 100);
			if (b_writenSuccess && (nRet == RESPONSE::OKAY)) 
			{
				/* flash command --- ""flash:%s""  example:"flash boot" */
				char sz_command[64] = {0};
				sprintf(sz_command, "flash:%s", sz_parmeter);

				/* Write flash command */
				if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 300000)) 
				{
					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(300000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					b_result = (nRet == RESPONSE::OKAY);
					if (!b_result) 
					{
						AddMsg("Read flash command response Failure", DownloadStep::None, 100);
					} 
					else 
					{
						/* Success */
						AddMsg(sz_imgae, DownloadStep::ADBProtocol, 100);
					}
				} 
				else 
				{
					AddMsg("Write flash command Failure", DownloadStep::None, 100);
				}
			}
		}
		else 
		{
			AddMsg("Read download command response Failure", DownloadStep::None, 100);
		}			
	}
	else 
	{
		AddMsg("Write download command Failure", DownloadStep::None, 100);
	}

	return b_result;
}


/*****************************************************************************
* Function name: LoadFile(文件大小，最大2G)    
* Summary      : Read image.
* Parameters   : sz_fileName   : image file name.   [Input]
*                i_fileSize    : file byte size.    [Output]
* Return       : image content.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
void* CDownload8X25::LoadFile(const char* sz_fileName, unsigned long long* i_fileSize)
{
	/* 线程同步处理 */
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	AddMsg("LoadFile.", DownloadStep::None, 100);

	/* Check Input*/
	if (sz_fileName == NULL)
	{
		*i_fileSize = 0;
		AddMsg("File name is empty.", DownloadStep::None, 100);
		return NULL;
	}

	HANDLE h_file  = NULL;
	char* c_p_data = NULL;
	DWORD dw_fileSize = 0;

	/* 打开文件 */
	h_file = CreateFile(sz_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(h_file == INVALID_HANDLE_VALUE) 
	{
		*i_fileSize = 0;
		AddMsg("CreateFile Failure.", DownloadStep::None, 100);
		return NULL;
	}
	else 
	{
		/* 获取文件大小（字节数） --- GetFileSize支持的最大文件大小为2G */
		dw_fileSize = GetFileSize(h_file, NULL);
		if (dw_fileSize == INVALID_FILE_SIZE)
		{
			/* 文件大小超过2G */
			CloseHandle(h_file);
			*i_fileSize = 0;
			AddMsg("GetFileSize is more than 2G.", DownloadStep::None, 100);
			return NULL;
		}
		if(dw_fileSize > 0) 
		{
			c_p_data = (char*) malloc(dw_fileSize);
			if(c_p_data == NULL) 
			{
				CloseHandle(h_file);
				*i_fileSize = 0;
				AddMsg("malloc Failure.", DownloadStep::None, 100);
				return NULL;
			}
			else 
			{
				/* 读文件 --- ReadFile最大读64MB大小的文件 */
				DWORD dw_ReadBytes = 0;
				if(!ReadFile(h_file, c_p_data, dw_fileSize, &dw_ReadBytes, NULL) ||	dw_ReadBytes != dw_fileSize) 
				{
					int i_retry = 0;

					/* 内存不足 */
					if(GetLastError() == ERROR_NO_SYSTEM_RESOURCES) 
					{
						/* 每次最大只读10MB */
						DWORD dw_bytesToRead = dw_fileSize;
						DWORD dw_bytesReaded = 0;
						DWORD dw_blockSize   = 10*1024*1024;

						/* 从文件开始位置，重读 */
						SetFilePointer(h_file, 0, NULL, FILE_BEGIN);

						while(dw_bytesToRead > 0) 
						{
							/* 检查是否超过10MB */
							if(dw_blockSize > dw_bytesToRead) 
							{
								dw_blockSize = dw_bytesToRead;
							}

							/* 读文件 */
							if(!ReadFile(h_file, c_p_data + dw_bytesReaded, dw_blockSize, &dw_ReadBytes, NULL) || dw_ReadBytes != dw_blockSize)
							{
								i_retry = 1;
								break;
							}
							dw_bytesReaded += dw_blockSize;
							dw_bytesToRead -= dw_blockSize;
						}
					}
					else 
					{
						i_retry = 1;
					}

					if(i_retry == 1) 
					{
						free(c_p_data);
						c_p_data = NULL;
						CloseHandle(h_file);
						*i_fileSize = 0;
						AddMsg("ReadFile Failure.", DownloadStep::None, 100);
						return NULL;
					}
				}
			}
		} 
		else 
		{
			/* 文件大小为0 */
			CloseHandle(h_file);
			*i_fileSize = 0;
			AddMsg("File size is 0.", DownloadStep::None, 100);
			return NULL;
		}
		CloseHandle(h_file);
		*i_fileSize = (unsigned long long)dw_fileSize;
	}

	AddMsg("LoadFile Success.", DownloadStep::None, 100);

	return c_p_data;
}

/*****************************************************************************
* Function name: GetADBDevice      
* Summary      : Get device.
* Parameters   : sz_comPort   : COM Port.
* Return       : Device.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
ADBDevice* CDownload8X25::GetADBDevice(const char* sz_comPort)
{
	/* Get ADB by the COM port */
	if (m_p_adbDevice == NULL)
	{
		m_p_adbDevice = new ADBDevice(adb_class_id, 0, 0, DEVICE_INTERFACE_ID, sz_comPort, 40);
		if(m_p_adbDevice) 
		{
			if(m_p_adbDevice->IsActive() == false) 
			{
				delete m_p_adbDevice;
				m_p_adbDevice = NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	return m_p_adbDevice;
}


/*****************************************************************************
* Function name: GetADB      
* Summary      : Get ADB.
* Parameters   : 
* Return       : Device.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::GetADB(void)
{
//	AddMsg("Get ADB ...", DownloadStep::None, 100);

	/* Get ADB port */
	char sz_comPort[10];
	memset(sz_comPort, 0, sizeof(sz_comPort));
	int i_COMPort = 0;
	i_COMPort = GetComport();

	//if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	//{
	//	sprintf(sz_comPort, "#%02x#{", i_COMPort);
	//}
	//else if (m_str_DLMode == DOWNLOAD_MODE_REDL)
	//{
	//	sprintf(sz_comPort, "&%d#{", i_COMPort);
	//}
	//else
	//{
	//	AddMsg("Is not PreDL mode or ReDL mode.", DownloadStep::None, 100);
	//	return false;
	//}

	if (m_i_SupportQDownload == 1)
	{
		strcpy(sz_comPort, "#{");
	}
	else
	{
		sprintf(sz_comPort, "&%d#{", i_COMPort);
	}

//	AddMsg(sz_comPort, DownloadStep::None, 100);

	/* Find ADB */
	if (!GetADBDevice(sz_comPort)) 
	{
		m_str_errorCode = CommErr_Get_ADB_Fail;
		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Get ADB Success.", DownloadStep::None, 100);
	return true;
}


/*****************************************************************************
* Function name: RebootSetMode      
* Summary      : Set FTD Mode flag.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/08/23             First version             
*****************************************************************************/
bool CDownload8X25::SetFTDMode(void)
{
	AddMsg("Fastboot Set FTD Mode.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "oem set-boot-mode 3" */
	if (m_p_adbDevice->Write("oem set-boot-mode 3", 19, 60000)) 
	{
		/* Read response command "oem set-boot-mode 3" */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (nRet == RESPONSE::OKAY) 
		{
		}
		else 
		{
			AddMsg("Read Set FTD Mode command response Fail.", DownloadStep::None, 100);
			return false;
		}
	} 
	else
	{
		AddMsg("Write Set FTD Mode command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot SetFTDMode Success.", DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::SetDislayColor(const std::string& str_parameter)
{
	if (str_parameter != _T("display-green")
		&& str_parameter != _T("display-blue")
		&& str_parameter != _T("display-red"))
	{
		AddMsg("CDownload8X25::SetDislayColor() fastboot oem Parameter Error.", DownloadStep::None, 100);
		return false;
	}

	std::string str_fastboot_cmd = "oem " + str_parameter;
	std::string str_output_msg = "";
	str_output_msg = str_output_msg + "fastboot " + str_fastboot_cmd.c_str() + "...";
	AddMsg(str_output_msg.c_str(), DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write fastboot command */
	char sz_command[1024] = {0};
	strcpy(sz_command,str_fastboot_cmd.c_str());
	int n_strlen_cmd = strlen(sz_command);
	if (m_p_adbDevice->Write(sz_command, n_strlen_cmd, 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read fastboot command response Fail.", DownloadStep::None, 100);
			m_str_errorCode = DLErr_DL_Send_OEMCMD_Fail;
			return false;
		}
	}
	else 
	{
		AddMsg("Send fastboot command Fail.", DownloadStep::None, 100);
		m_str_errorCode = DLErr_DL_Send_OEMCMD_Fail;
		return false;
	}

	str_output_msg = "";
	str_output_msg = str_output_msg + "fastboot " + str_fastboot_cmd.c_str() + "...Success";
	AddMsg(str_output_msg.c_str(), DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: Reboot      
* Summary      : Reboot mobile.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/19             First version             
*****************************************************************************/
bool CDownload8X25::Reboot(void)
{
	AddMsg("Fastboot reboot.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "reboot" */
	if (m_p_adbDevice->Write("reboot", 6, 60000)) 
	{
		/* Read response command "reboot" */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read reboot command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write reboot command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot reboot Success.", DownloadStep::None, 100);
	return true;
}


/*****************************************************************************
* Function name: EraseSBL1      
* Summary      : Enter emergence DL mode by erase SBL1.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/19             First version             
*****************************************************************************/
bool CDownload8X25::EraseSBL1(void)
{
	AddMsg("Fastboot erase sbl1.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "erase sbl1" */
	if (m_p_adbDevice->Write("erase:sbl1", 10, 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read erase command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write erase command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot erase sbl1 Success.", DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::EraseModem_st1(void)
{
	AddMsg("Fastboot modem_st1.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "erase sbl1" */
	if (m_p_adbDevice->Write("erase:modem_st1", 15, 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read erase command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write erase command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot erase modem_st1 Success.", DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::EraseModem_st2(void)
{
	AddMsg("Fastboot modem_st2.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "erase sbl1" */
	if (m_p_adbDevice->Write("erase:modem_st2", 15, 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read erase command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write erase command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot erase modem_st2 Success.", DownloadStep::None, 100);
	return true;
}
bool CDownload8X25::EraseUserFat(void)
{
	AddMsg("Fastboot erase user fat.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Write command "erase mdm"; mdm-> Mass Storage */
	if (m_p_adbDevice->Write("erase:mdm", 9, 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read erase command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write erase command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot erase user fat Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: WriteCheckSum      
* Summary      : Write checksum to mobile.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::WriteCheckSum(CString str_imageFilePathName)
{
	AddMsg("Write CheckSum.", DownloadStep::None, 100);

	/* Check input parameter */
	if (str_imageFilePathName == _T("")) 
	{
		AddMsg("Image file name is empty.", DownloadStep::None, 100);
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	/* Get checksum */
	CString str_checkSum;
	str_checkSum = m_map_imageNameCheckSum[str_imageFilePathName];
	char sz_command[1024] = {0};
	sprintf_s(sz_command, sizeof(sz_command), "oem cksum %s", str_checkSum);
	if (str_checkSum == _T(""))
	{
		AddMsg("CheckSum is not exist.", DownloadStep::None, 100);
		return false;
	}

	/* Write command */
	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		/* Read response command */
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY) 
		{
		} 
		else 
		{
			AddMsg("Read CheckSum command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write CheckSum command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Write CheckSum Success.", DownloadStep::None, 100);
	return true;
}


/*****************************************************************************
* Function name: ReadCheckSumFile      
* Summary      : Read checksum file.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::ReadCheckSumFile(void)
{
	CSingleLock singleLock(&m_obj_loadcheckSumCritSection, TRUE);

	AddMsg("Read CheckSum file.", DownloadStep::None, 100);
	m_str_ImageHWVariant = _T("");
	CString str_ModeHWVariant = _T("");
	CString str_AndroidHWVariant = _T("");

	AddMsg("Read CheckSum file CHECKSUM_MODEM_FILE_NAME.", DownloadStep::None, 100);
	/* Check file exist */
	CString str_checkModemSumFileName;
	str_checkModemSumFileName.Format("%s\\%s", m_str_imagePath, CHECKSUM_MODEM_FILE_NAME);
	if (::_taccess(str_checkModemSumFileName, 0) != 0) 
	{
		AddMsg("Checksum file is not exist.", DownloadStep::None, 100);
		return false;
	}

	/* Open checksum file */
	CStdioFile obj_file;
	if (!obj_file.Open(str_checkModemSumFileName, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
	{
		AddMsg("Open Checksum file Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Read checksum file */
	try 
	{
		CString str_string;
		while (obj_file.ReadString(str_string)) 
		{
			/* Check_Sum Size Image_Filename */
			CString str_imageName;
			CString str_checkSum;
			CStringArray stra_value;
			if(str_string.Find(_T(":")) != -1) //add hw variant infor in checksum file from Cario
			{
				str_ModeHWVariant = str_string.Left(4);
				AddMsg("Modem HW: "+str_ModeHWVariant, DownloadStep::None, 100);
			}
			else
			{
			if (SpiltString(str_string, " ", stra_value) != true)
			{
				AddMsg("Split Checksum file Fail.", DownloadStep::None, 100);
				return false;
			}
			if (stra_value.GetSize() != 3)
			{
				AddMsg("Checksum file format is error.", DownloadStep::None, 100);
				return false;
			}
			str_checkSum = stra_value.GetAt(0);
			str_imageName = stra_value.GetAt(2);

			CString str_imagePathName;
			str_imagePathName = m_str_imagePath + _T("\\") + str_imageName;

			m_map_imageNameCheckSum[str_imagePathName] = str_checkSum;
			}

			str_string = _T("");
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
		obj_file.Close();
		AddMsg("Read Checksum file Exception Fail.", DownloadStep::None, 100);
		return false;
	}
	obj_file.Close();
	AddMsg("Read CheckSum file CHECKSUM_MODEM_FILE_NAME Success.", DownloadStep::None, 100);

	AddMsg("Read CheckSum file CHECKSUM_ANDROID_FILE_NAME.", DownloadStep::None, 100);
	/* Check file exist */
	CString str_checkAndroidSumFileName;
	str_checkAndroidSumFileName.Format("%s\\%s", m_str_imagePath, CHECKSUM_ANDROID_FILE_NAME);
	if (::_taccess(str_checkAndroidSumFileName, 0) != 0) 
	{
		AddMsg("Checksum file is not exist.", DownloadStep::None, 100);
		return false;
	}

	/* Open checksum file */
	CStdioFile obj_file2;
	if (!obj_file2.Open(str_checkAndroidSumFileName, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
	{
		AddMsg("Open Checksum file Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Read checksum file */
	try 
	{
		CString str_string;
		while (obj_file2.ReadString(str_string)) 
		{
			/* Check_Sum Size Image_Filename */
			CString str_imageName;
			CString str_checkSum;
			CStringArray stra_value;
			if(str_string.Find(_T(":")) != -1) //add hw variant infor in checksum file from Cario
			{
				str_AndroidHWVariant = str_string.Left(4);
				AddMsg("Android HW: "+str_AndroidHWVariant, DownloadStep::None, 100);
			}
			else
			{
			if (SpiltString(str_string, " ", stra_value) != true)
			{
				AddMsg("Split Checksum file Fail.", DownloadStep::None, 100);
				return false;
			}
			if (stra_value.GetSize() != 3)
			{
				AddMsg("Checksum file format is error.", DownloadStep::None, 100);
				return false;
			}
			str_checkSum = stra_value.GetAt(0);
			str_imageName = stra_value.GetAt(2);

			CString str_imagePathName;
			str_imagePathName = m_str_imagePath + _T("\\") + str_imageName;

			m_map_imageNameCheckSum[str_imagePathName] = str_checkSum;
			}

			str_string = _T("");
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
		obj_file2.Close();
		AddMsg("Read Checksum file Exception Fail.", DownloadStep::None, 100);
		return false;
	}
	obj_file2.Close();
	AddMsg("Read CheckSum file CHECKSUM_ANDROID_FILE_NAME Success.", DownloadStep::None, 100);

	if(str_AndroidHWVariant != str_ModeHWVariant)
	{
		AddMsg("Modem HW Variant not equal Android HW Variant.", DownloadStep::None, 100);
		return false;
	}
	else
		m_str_ImageHWVariant = str_AndroidHWVariant;

	AddMsg("Read CheckSum file Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: ReadFA      
* Summary      : Read FA.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::ReadFA(char* sz_FAData)
{
	AddMsg("Read FA.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	int i_length = FALENGTH;
	//bool b_result = false;
	char sz_buffer[512] = {0};

	/* dump protocol --- "dump:%s"  example:"dump:otpfa" */
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s", "otpfa");

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		RESPONSE enum_readRosponse = UNKNOW;

		char szOutput[2048] = {0};
		enum_readRosponse = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (enum_readRosponse == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", i_length);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if ( nRet == RESPONSE::DATA ) 
				{
					unsigned long l_readLen = 0;
					if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 60000) && (l_readLen == FALENGTH)) 
					{
						memcpy(sz_FAData, sz_buffer, FALENGTH);
					}
					else
					{
						AddMsg("Read upload data Fail.", DownloadStep::None, 100);
						return false;
					}
					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					//b_result = ();
					if ( nRet != RESPONSE::OKAY )
					{
						AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
						return false;
					}
				}
				else
				{
					AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
					return false;
				}
			} 
			else 
			{
				AddMsg("Write upload command Fail.", DownloadStep::None, 100);
				return false;
			}
		}
		/* FA is empty */
		else if (enum_readRosponse == RESPONSE::FAILFAEMPTY)
		{
			char sz_buffer[512] = {0};
			memcpy(sz_FAData, sz_buffer, FALENGTH);
		} 
		else 
		{
			AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Write dump command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Read FA Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: WriteFA      
* Summary      : Write FA.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::WriteFA(char* sz_FAData)
{
	AddMsg("Write FA.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	int i_length = FALENGTH;
	//bool b_result = false;

	/* download protocol --- "download:%08x"  example:"download:00162c00" */
	char sz_command[64] = {0};
	sprintf(sz_command, "download:%08x", i_length);
	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000))
	{
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::DATA)
		{
			char* c_p_data = (char*) sz_FAData;
			int i_count = 0;
			bool b_writeSuccess = true;

			// Max write byte:4096
			while (i_length > 0 && b_writeSuccess) 
			{
				int xfer = (i_length > 4096) ? 4096 : i_length;

				// ADB Write
				b_writeSuccess = m_p_adbDevice->Write(c_p_data, xfer);
				i_count   += xfer;
				i_length -= xfer;
				c_p_data  += xfer;
			};

			if (!b_writeSuccess)
			{
				AddMsg("Write download data Fail.", DownloadStep::None, 100);
				return false;
			}

			char szOutput[2048] = {0};
			int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
			AddMsg(szOutput, DownloadStep::None, 100);
			if ( nRet == RESPONSE::OKAY)
			{
				char sz_command[64] = {0};
				sprintf(sz_command, "flash:%s", "otpfa");

				if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 120000))
				{
					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					if (nRet != RESPONSE::OKAY) 
					{
						AddMsg("Read flash response Fail.", DownloadStep::None, 100);
						return false;
					}
				} 
				else 
				{
					AddMsg("Write flash command Fail.", DownloadStep::None, 100);
					return false;
				}
			}
			else 
			{
				AddMsg("Read download data response Fail.", DownloadStep::None, 100);
				return false;
			}
		} 
		else
		{
			AddMsg("Read download response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write download command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Write FA Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: ReadFASector      
* Summary      : Read FA --- New FA structure.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/11/23          First version             
*****************************************************************************/
bool CDownload8X25::ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	AddMsg("Starting read FA...CDownload8X25::ReadFASector( ) .", DownloadStep::None, 100);	
	
	//if (m_str_HexDLMode == DOWNLOAD_MODE_REDL) 
	//{
	//	if(!DMSSDL_ReDL())
	//	{
	//		return false;
	//	}
	//}

	/* Get ADB */
	if (!GetADB()) 
	{
		/* Hex Download and fastboot image download ,change to fastboot mode */
		/* Add by Lennon 2013.10.14 for T-Line RE_DL process */
		/* Below Code is marked and reserved for later use*/
		//bool b_result = false;
		//b_result = Download(false, false, QTT_FASTDOWNLOAD);
		//if (!b_result) 
		//{
		//	return false;
		//}
		//if (!GetADB()) 
		return false;
	}

	//bool b_result = false;
	int  i_length = i_sectorSize;
	char sz_buffer[512] = {0};

	/* dump protocol --- "dump:%s"  example:"dump:otpfa0x0+512" */
	char sz_sectorSize[32] = {0};
	sprintf(sz_sectorSize, "%d+%d", i_sectorNum * 512, i_sectorSize);
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s%s", "otpfa", sz_sectorSize);

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		RESPONSE enum_readRosponse = UNKNOW;

		char szOutput[2048] = {0};
		enum_readRosponse = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (enum_readRosponse == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", i_length);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{

				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if (nRet == RESPONSE::DATA) 
				{
					unsigned long l_readLen = 0;
					unsigned long Total_Read = 0;
					//USB 2.0 Low Speed  8 bytes/Packet, max count =  FALENGTH/LowSpeed = 512/8 = 64
					for (int i = 0; i < 64;  i++ )
					{
						l_readLen = 0;
						m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 60000);
						memcpy(sz_sectorData + Total_Read , sz_buffer, l_readLen);
						Total_Read = Total_Read + l_readLen;

						char sz_temp[256];
						sprintf(sz_temp,"Read %d time(s), data Length is %d",i + 1,l_readLen);
						AddMsg(sz_temp, DownloadStep::None, 100);

						if (Total_Read < FALENGTH)
							continue;
						else
							break;
					}
					if (Total_Read == FALENGTH)
					{
					}
					else
					{
						AddMsg("Read upload data Fail.", DownloadStep::None, 100);
						return false;
					}

					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					if ( nRet != RESPONSE::OKAY )
					{
						AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
						return false;
					}
				}
				else
				{
					AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
					return false;
				}
			} 
			else 
			{
				AddMsg("Write upload command Fail.", DownloadStep::None, 100);
				return false;
			}
		}
		/* FA is empty */
		else if (enum_readRosponse == RESPONSE::FAILFAEMPTY)
		{
			char sz_buffer[512] = {0};
			memcpy(sz_sectorData, sz_buffer, i_sectorSize);
		} 
		else 
		{
			AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Write dump command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Read FA Success... CDownload8X25::ReadFASector( )", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: WriteFASector      
* Summary      : Write FA --- New FA structure.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/11/23          First version             
*****************************************************************************/
bool CDownload8X25::WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	AddMsg("Starting write FA... CDownload8X25::WriteFASector( ) ", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	int i_length = i_sectorSize;
	//bool b_result = false;

	int n_MaxRetryCount = 5;
	for (int i = 0; i <= n_MaxRetryCount; ++ i)
	{
		if ( i >= n_MaxRetryCount) 
		{
			AddMsg("Write FA Fail... CDownload8X25::WriteFASector( ) ", DownloadStep::None, 100);
			return false;
		}
		if ( i > 1) 
			Sleep(500);
		/* download protocol --- "download:%08x"  example:"download:00162c00" */
		char sz_command[64] = {0};
		sprintf(sz_command, "download:%08x", i_length);
		if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000))
		{
			char szOutput[2048] = {0};
			int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
			AddMsg(szOutput, DownloadStep::None, 100);
			if ( nRet == RESPONSE::DATA)
			{
				char* c_p_data = (char*) sz_sectorData;
				int i_count = 0;
				bool b_writeSuccess = true;

				// Max write byte:4096
				while (i_length > 0 && b_writeSuccess) 
				{
					int xfer = (i_length > 4096) ? 4096 : i_length;

					// ADB Write
					b_writeSuccess = m_p_adbDevice->Write(c_p_data, xfer);
					i_count   += xfer;
					i_length -= xfer;
					c_p_data  += xfer;
				};

				if (!b_writeSuccess)
				{
					AddMsg("Write download data Fail.", DownloadStep::None, 100);
					continue;
				}

				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if ( nRet == RESPONSE::OKAY)
				{
					char sz_sectorSize[32] = {0};
					sprintf(sz_sectorSize, "%d+%d", i_sectorNum * 512, i_sectorSize);
					char sz_command[64] = {0};
					sprintf(sz_command, "flash:%s%s", "otpfa", sz_sectorSize);

					if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 120000))
					{
						char szOutput[2048] = {0};
						int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
						AddMsg(szOutput, DownloadStep::None, 100);
						if ( nRet != RESPONSE::OKAY) 
						{
							AddMsg("Read flash response Fail.", DownloadStep::None, 100);
							continue;
						}
						else
						{
							AddMsg("Write FA Success... CDownload8X25::WriteFASector( )", DownloadStep::None, 100);
							return true;
						}

					} 
					else 
					{
						AddMsg("Write flash command Fail.", DownloadStep::None, 100);
						continue;
					}
				}
				else 
				{
					AddMsg("Read download data response Fail.", DownloadStep::None, 100);
					continue;
				}
			} 
			else
			{
				AddMsg("Read download response Fail.", DownloadStep::None, 100);
				continue;
			}
		}
	}
	// Default return false for debug code error;
	AddMsg("Write FA Fail... CDownload8X25::WriteFASector( ) ", DownloadStep::None, 100);
	return false;
}

/*****************************************************************************
* Function name: BackupNVPartition      
* Summary      : Backup NV partition.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8X25::BackupNVPartition(void)
{
	AddMsg("Backup NV Partition.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}
	
	AddMsg("Backup NV Partition 0xa0000_1152.", DownloadStep::None, 100);
	/* Backup 0xa0000_1152 */
	long l_length1 = NVPARTITION_1_LENGTH;
	char sz_buffer1[NVPARTITION_1_LENGTH] = {0};
	//bool b_result = false;
	/* dump protocol --- "dump:%s"  example:"dump:0xa0000_1280" */
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s", "0xa0000_1152");

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if (nRet == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", l_length1);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if (nRet == RESPONSE::DATA) 
				{
					unsigned long l_readLen = 0;
					if (m_p_adbDevice->Read(sz_buffer1, sizeof(sz_buffer1), l_readLen, 120000) && (l_readLen == NVPARTITION_1_LENGTH)) 
					{
					}
					else
					{
						AddMsg("Read upload data Fail.", DownloadStep::None, 100);
						return false;
					}

					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					if (nRet != RESPONSE::OKAY)
					{
						AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
						return false;
					}
				}
				else
				{
					AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
					return false;
				}
			} 
			else 
			{
				AddMsg("Write upload command Fail.", DownloadStep::None, 100);
				return false;
			}
		}
		else 
		{
			AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Write dump command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Backup NV Partition 0xb0000_128.", DownloadStep::None, 100);
	/* Backup 0xb0000_128 */
	long l_length2 = NVPARTITION_2_LENGTH;
	char sz_buffer2[NVPARTITION_2_LENGTH] = {0};
	//b_result = false;
	/* dump protocol --- "dump:%s"  example:"dump:0xa0000_1280" */
	char sz_command2[64] = {0};
	sprintf(sz_command2, "dump:%s", "0xb0000_128");

	if (m_p_adbDevice->Write(sz_command2, strlen(sz_command2), 60000)) 
	{
		char szOutput[2048] = {0};
		int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
		AddMsg(szOutput, DownloadStep::None, 100);
		if ( nRet == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", l_length2);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if ( nRet == RESPONSE::DATA) 
				{
					unsigned long l_readLen = 0;
					if (m_p_adbDevice->Read(sz_buffer2, sizeof(sz_buffer2), l_readLen, 120000) && (l_readLen == NVPARTITION_2_LENGTH)) 
					{
					}
					else
					{
						AddMsg("Read upload data Fail.", DownloadStep::None, 100);
						return false;
					}

					char szOutput[2048] = {0};
					int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
					AddMsg(szOutput, DownloadStep::None, 100);
					if (nRet != RESPONSE::OKAY)
					{
						AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
						return false;
					}
				}
				else
				{
					AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
					return false;
				}
			} 
			else 
			{
				AddMsg("Write upload command Fail.", DownloadStep::None, 100);
				return false;
			}
		}
		else 
		{
			AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Write dump command Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Backup to file */
	CString str_directory;
	WIN32_FIND_DATA fd_fileInformation;
	HANDLE h_searchFind = NULL;
	h_searchFind = ::FindFirstFile(".\\LOG", &fd_fileInformation);
	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		::FindClose(h_searchFind); 
	} 
	else 
	{
		::FindClose(h_searchFind); 
		if(::CreateDirectory(".\\LOG", NULL) == 0) 
		{
			AddMsg(".\\LOG is not exist.", DownloadStep::None, 100);
			return false;	
		} 
	}

	h_searchFind = NULL;
	h_searchFind = ::FindFirstFile(".\\LOG\\BackupNV", &fd_fileInformation);
	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		::FindClose(h_searchFind); 
	}
	else
	{
		::FindClose(h_searchFind); 
		if(::CreateDirectory(".\\LOG\\BackupNV", NULL) == 0) 
		{
			AddMsg(".\\LOG\\BackupNV is not exist.", DownloadStep::None, 100);
			return false;
		} 
	}
	str_directory = _T(".\\LOG\\BackupNV");

	int i_COMPort = 0;
	i_COMPort = GetComport();
	CString picasso = m_map_COMPicasso[i_COMPort];
	if (picasso == _T(""))
	{
		AddMsg("Picasso is not exist.", DownloadStep::None, 100);
		return false;
	}

	CString str_fileName;
	str_fileName.Format(_T("%s\\%s.txt"), str_directory, picasso);
	CStdioFile obj_file;
	if (!obj_file.Open(str_fileName, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
	{
		AddMsg("Open backup NV file Fail.", DownloadStep::None, 100);
		return false;
	}

	CString str_title;
	SYSTEMTIME st_systemTime;
	GetLocalTime(&st_systemTime);
	CString str_time;
	str_time.Format(_T("Backup NV partition Time: %4d/%02d/%02d %02d:%02d:%02d\r\n"), st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay, st_systemTime.wHour, st_systemTime.wMinute, st_systemTime.wSecond);
	str_title = _T("\r\n===========================================================================================\r\n");
	str_title += str_time;
	str_title += _T("===========================================================================================\r\n");

	try 
	{
		/* Write title */
		obj_file.SeekToEnd();
		obj_file.Write(str_title, str_title.GetLength());
		obj_file.Flush();

		/* Write NV Partition 0xa0000_1152 */
		CString str_NV1Title;
		str_NV1Title = "Backup NV Partition 0xa0000_1152\r\n";
		obj_file.SeekToEnd();
		obj_file.Write(str_NV1Title, str_NV1Title.GetLength());
		obj_file.Flush();

		obj_file.SeekToEnd();
		obj_file.Write(sz_buffer1, sizeof(sz_buffer1));
		obj_file.Flush();

		/* Write NV Partition 0xb0000_128 */
		CString str_NV2Title;
		str_NV2Title = "\r\nBackup NV Partition 0xb0000_128\r\n";
		obj_file.SeekToEnd();
		obj_file.Write(str_NV2Title, str_NV2Title.GetLength());
		obj_file.Flush();

		obj_file.SeekToEnd();
		obj_file.Write(sz_buffer2, sizeof(sz_buffer2));
		obj_file.Flush();
	} 
	catch (CFileException *e)
	{
		e->Delete();
		obj_file.Close();
		AddMsg("Write backup NV file Fail.", DownloadStep::None, 100);
		return false;
	}
	obj_file.Close();

	AddMsg("Backup NV Partition Success.", DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg)
{
	/* Check Input */
	if (str_sourceString == _T(""))
	{
		return false;
	}

	stra_strArg.RemoveAll();

	CString str_temp;
	int i_position = 0;
	int i_strLength = 0;

	/* Delimiter is space case */
	if (str_delimiter == _T(" "))
	{
		str_sourceString.TrimLeft();
		str_sourceString.TrimRight();
	}

	/* Find the first delimiter */
	i_position = str_sourceString.FindOneOf(str_delimiter);
	if (i_position == -1)
	{
		/* The source string did not have delimiter */
		stra_strArg.Add(str_sourceString);		
		return true;
	}

	while(str_sourceString != _T("")) 
	{
		/* Get the first of position of delimiter in source string */
		i_position = str_sourceString.FindOneOf(str_delimiter);
		i_strLength = str_sourceString.GetLength();
		if (i_position == -1 )
		{
			/* The source string did not have delimiter */
			stra_strArg.Add(str_sourceString);
			break ;
		}
		try
		{
			/* Get the first string */
			str_temp = str_sourceString.Mid(0, i_position);
		} 
		catch(CMemoryException e)
		{
			e.Delete();
			return false;
		}

		/* Delete right space of string */
		if (str_delimiter == _T(" "))
		{
			str_temp.TrimRight();
		}

		stra_strArg.Add(str_temp);

		try
		{
			/* Get the next string */
			str_sourceString = str_sourceString.Mid(i_position + 1);
		} 
		catch(CMemoryException e) 
		{
			e.Delete();
			return false;
		}

		if (str_delimiter == _T(" ")) 
		{
			/* delete left space of string */
			str_sourceString.TrimLeft();
		}

		/* The end string */
		if (i_position == i_strLength - 1)
		{
			stra_strArg.Add(str_sourceString);
		}
	}	

	return true;
}

bool CDownload8X25::ADBFlashAll(const char* sz_parmeter, const char* sz_fileName)
{
	/* Check input parameter */
	if ((sz_parmeter == NULL) || (sz_fileName == NULL)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		return false;
	}

	HANDLE h_file  = NULL;
	DWORD dw_fileSize = 0;
	h_file = CreateFile(sz_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h_file == INVALID_HANDLE_VALUE) 
	{
		AddMsg("CreateFile Failure.", DownloadStep::None, 100);
		return false;
	}
	else 
	{
		/* 获取文件大小（字节数） --- GetFileSize支持的最大文件大小为2G */
		dw_fileSize = GetFileSize(h_file, NULL);
		if (dw_fileSize == INVALID_FILE_SIZE)
		{
			/* 文件大小超过2G */
			CloseHandle(h_file);
			AddMsg("GetFileSize is more than 2G.", DownloadStep::None, 100);
			return false;
		}
		if (dw_fileSize <= 0)
		{
			CloseHandle(h_file);
			AddMsg("File size is less than 0.", DownloadStep::None, 100);
			return false;
		}

		/* Show UI info */
		char sz_imgae[64] = {0};
		sprintf(sz_imgae, "Fastboot download:%s", sz_parmeter);

		/* download protocol --- "download:%08x"  example:"download:00162c00" */
		char sz_command[64] = {0};
		sprintf(sz_command, "download:%08x", dw_fileSize);

		/* Write download protocol */
		if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
		{
			/* ADB Read --- 如果返回DATA，则可以发送数据  */
			char szOutput[2048] = {0};
			int nRet = m_p_adbDevice->ReadResponse_Msg(60000, szOutput);
			AddMsg(szOutput, DownloadStep::None, 100);
			if ( nRet == RESPONSE::DATA) 
			{
				DWORD dw_bytesToRead = dw_fileSize;
				DWORD dw_bytesReaded = 0;
				DWORD dw_blockSize   = 4096;
				DWORD dw_ReadBytes = 0;
				char sz_byteSize[4096] = {0};
		
				int i_progress = 0;
				unsigned long long l_count = 0;

				while(dw_bytesToRead > 0) 
				{
					if(dw_blockSize > dw_bytesToRead) 
					{
						dw_blockSize = dw_bytesToRead;
					}

					/* 读文件 */
					if(!ReadFile(h_file, sz_byteSize, dw_blockSize, &dw_ReadBytes, NULL) || dw_ReadBytes != dw_blockSize)
					{
						CloseHandle(h_file);
						return false;
					}
					dw_bytesReaded += dw_blockSize;
					dw_bytesToRead -= dw_blockSize;

					i_progress = (l_count * 100) / dw_fileSize;
					AddMsg(sz_imgae, DownloadStep::ADBProtocol, i_progress);
					int xfer = (dw_blockSize > 4096) ? 4096 : dw_blockSize;
					if (!m_p_adbDevice->Write(sz_byteSize, xfer, 60000))
					{
						AddMsg("Write download bytes to partition Failure", DownloadStep::None, 100);
						CloseHandle(h_file);
						return false;
					}
					l_count += xfer;
				}

				char szOutput[2048] = {0};
				int nRet = m_p_adbDevice->ReadResponse_Msg(120000, szOutput);
				AddMsg(szOutput, DownloadStep::None, 100);
				if ( nRet == RESPONSE::OKAY) 
				{
					/* flash command --- ""flash:%s""  example:"flash boot" */
					char sz_command[64] = {0};
					sprintf(sz_command, "flash:%s", sz_parmeter);
					AddMsg(sz_command, DownloadStep::None,100);

					/* Write flash command */
					if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 300000)) 
					{
						char szOutput[2048] = {0};
						int nRet = m_p_adbDevice->ReadResponse_Msg(1200000, szOutput);
						AddMsg(szOutput, DownloadStep::None, 100);
						char szResult[4096] ={0};
						sprintf(szResult, "Result: %d",nRet);
						AddMsg(szResult, DownloadStep::None, 100);
						if(nRet != RESPONSE::OKAY)
						//if (!(m_p_adbDevice->ReadResponse(300000) == RESPONSE::OKAY)) 
						{
							AddMsg("Read flash command response Failure", DownloadStep::None, 100);
							CloseHandle(h_file);
							return false;
						} 
						AddMsg(sz_imgae, DownloadStep::ADBProtocol, 100);
					} 
					else 
					{
						AddMsg("Write flash command Failure", DownloadStep::None, 100);
						CloseHandle(h_file);
						return false;
					}
				}
			}
			else 
			{
				AddMsg("Read download command response Failure", DownloadStep::None, 100);
				CloseHandle(h_file);
				return false;
			}			
		}
		else 
		{
			AddMsg("Write download command Failure", DownloadStep::None, 100);
			CloseHandle(h_file);
			return false;
		}
	}

	CloseHandle(h_file);
	return true;
}

/*****************************************************************************
* Function name: CheckHWVariant      
* Summary      : Check HW Variant (CDMA/UMTS).
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Magan.Mei        2012/06/01          First version             
*****************************************************************************/
bool CDownload8X25::CheckHWVariant(void)
{	
	AddMsg("Fastboot Check HW Variant.", DownloadStep::None, 100);

	int i_length = FALENGTH;
	bool b_result = false;
	char sz_buffer[512] = {0};
	CString str_TargetHWVariant = _T("");

	if (!GetADB()) 
	{
		return false;
	}

	char sz_command[64] = {0};
	sprintf(sz_command, "getvar:rfid");
	AddMsg("fastboot getvar rfid.", DownloadStep::None, 100);

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 10000)) 
	{
		unsigned long l_readLen = 0;
		if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 10000) ) 
		{
			AddMsg(sz_buffer, DownloadStep::None, 100);

			CString strTemp = sz_buffer;
			CString strTemp1 = strTemp.Left(4);

			if(strTemp1 != _T("OKAY"))
			{
				AddMsg("getvar response not okay.", DownloadStep::None, 100);
				return false;
			}

			str_TargetHWVariant = strTemp.Right(4);
			AddMsg(str_TargetHWVariant.GetBuffer(4), DownloadStep::None, 100);

		}
		else
		{
			AddMsg("getvar data Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write getvar rfid command Fail.", DownloadStep::None, 100);
		return false;
	}
	
	if(str_TargetHWVariant != m_str_ImageHWVariant)
	{
		AddMsg("Check HW Variant fail." + str_TargetHWVariant + (":") + m_str_ImageHWVariant, DownloadStep::None, 100);
		return false;
	}
	AddMsg("Check HW Variant Success."+ str_TargetHWVariant +(":") + m_str_ImageHWVariant, DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::GetImageSWVariant(void)
{
	CSingleLock singleLock(&m_obj_loadImageVerCritSection, TRUE);
	m_str_ImageSWVariant = _T("");
	AddMsg("Read OSImageVersion.txt.", DownloadStep::None, 100);

	/* Check file exist */
	CString str_OSImageVersionFile;
	str_OSImageVersionFile.Format("%s\\%s", m_str_imagePath, OS_IMAGE_VERSION_FILE_NAME);
	if (::_taccess(str_OSImageVersionFile, 0) != 0) 
	{
		AddMsg("OSImageVersion file is not exist.", DownloadStep::None, 100);
		return false;
	}

	/* Open OSImageVersion file */
	CStdioFile obj_file;
	if (!obj_file.Open(str_OSImageVersionFile, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
	{
		AddMsg("Open OSImageVersion file Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Read checksum file */
	try 
	{
		CString str_string;
		while (obj_file.ReadString(str_string)) 
		{
			str_string.MakeUpper();
			if(str_string.Find(_T("BUILDVERSION")) != -1) 
			{
				m_str_ImageSWVariant = str_string.Right(str_string.GetLength() - 12);
				m_str_ImageSWVariant.TrimLeft();
				m_str_ImageSWVariant.TrimLeft('=');
				AddMsg("OSImageVersion: " + m_str_ImageSWVariant, DownloadStep::None, 100);
				break;
			}
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
		obj_file.Close();
		AddMsg("Read OSImageVersion file Exception Fail.", DownloadStep::None, 100);
		return false;
	}
	obj_file.Close();

	AddMsg("Read OSImageVersion file Success.", DownloadStep::None, 100);
	return true;
}

bool CDownload8X25::GetPhoneSWVariant(void)
{	
	AddMsg("Fastboot Get SW Variant.", DownloadStep::None, 100);

	int i_length = FALENGTH;
	bool b_result = false;
	char sz_buffer[512] = {0};
	CString str_TargetSWVariant = _T("");

	if (!GetADB()) 
	{
		return false;
	}

	char sz_command[64] = {0};
	sprintf(sz_command, "getvar:build");
	AddMsg("fastboot getvar build.", DownloadStep::None, 100);

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 10000)) 
	{
		unsigned long l_readLen = 0;
		if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 10000) ) 
		{
			int length = strlen(sz_buffer);
			for (int i = 0; i <= length; ++ i)
			{
				if ( unsigned(sz_buffer[i]) > 127)
				{
					sz_buffer[0] = 0;
					break;
				}
			}
			AddMsg(sz_buffer, DownloadStep::None, 100);

			CString strTemp = sz_buffer;
			CString strTemp1 = strTemp.Left(4);

			if(strTemp1 != _T("OKAY"))
			{
				AddMsg("getvar response not okay.", DownloadStep::None, 100);
				return false;
			}

			m_str_PhoneSWVariant = strTemp.Right(strTemp.GetLength() - 4);
			AddMsg("getvar response okay:" + m_str_PhoneSWVariant, DownloadStep::None, 100);
			return true;
		}
		else
		{
			AddMsg("getvar data Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write getvar rfid command Fail.", DownloadStep::None, 100);
		return false;
	}
}

bool CDownload8X25::CompareVersion(  )
{
	CString str_PhoneSWVariant = m_str_PhoneSWVariant;
	CString str_ImageSWVariant = m_str_ImageSWVariant;
	str_PhoneSWVariant.Replace(_T("-"),_T(""));
	str_PhoneSWVariant.Replace(_T("_"),_T(""));
	str_PhoneSWVariant.Replace(_T("."),_T(""));
	str_ImageSWVariant.Replace(_T("-"),_T(""));
	str_ImageSWVariant.Replace(_T("_"),_T(""));
	str_ImageSWVariant.Replace(_T("."),_T(""));

	int min = (str_PhoneSWVariant.GetLength() < str_ImageSWVariant.GetLength()?str_PhoneSWVariant.GetLength():str_ImageSWVariant.GetLength());
	for (int i = 0; i <= min; i++)
	{
		if (str_PhoneSWVariant.Mid(i) == str_ImageSWVariant.Mid(i))
			continue;

		if (str_PhoneSWVariant.Mid(i) < str_ImageSWVariant.Mid(i))
		{
			AddMsg("The Image version is newer than Phone FW Version.", DownloadStep::None, 100);
			return true;
		}

		if (str_PhoneSWVariant.Mid(i) > str_ImageSWVariant.Mid(i))
		{
			AddMsg("Phone FW Version is newer than the Image version.", DownloadStep::None, 100);
			AddMsg("Downgrade operation is not allowed!", DownloadStep::None, 100);
			return false;
		}
	}
	return true;
}
bool CDownload8X25::RebootToBootLoader(  )
{
	CString strCommand;
	strCommand = strCommand + _T("adb.exe reboot bootloader");
	WCHAR wsz_commandLine[2048];
	wcscpy(wsz_commandLine, CT2CW(strCommand));

	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcessW(NULL, wsz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) {
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE); 
	return true;
}

bool CDownload8X25::KillADB(  )
{
	CString strCommand;
	strCommand = strCommand + _T("taskkill /f /im adb.exe");
	WCHAR wsz_commandLine[2048];
	wcscpy(wsz_commandLine, CT2CW(strCommand));

	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcessW(NULL, wsz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) {
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE); 
	return true;
}