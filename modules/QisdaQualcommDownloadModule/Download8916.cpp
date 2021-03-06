
#include "StdAfx.h"
#include "Download8916.h"
#include "../../lib/Qualcomm/QDART4823/inc/QLib.h"
#include "../UnifyDLL_iF1_Download/SparseLib/sparse.h"
#include <SetupAPI.h>
#pragma comment(lib, "setupapi")


//#define _DebugSparse

CCriticalSection CDownload8916::m_obj_loadcheckSumCritSection;
CDownload8916* g_pFirehoseDownload = NULL;

CDownload8916::CDownload8916(const TSTRING& str_deviceName, const std::string& str_platform)
:CDLFormatCommand(str_deviceName, str_platform), m_b_reOpenAfterReset(false), m_b_resetMode(true), m_b_overridePartition(false), m_p_adbDevice(NULL)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	m_i_rework = 0;
	m_i_reboot = 0;
	m_i_DDRCheck = 0;
	m_i_checkSum = 0;
	m_i_backupNV = 0;
	m_i_SupportQDownload = 0;
	m_i_eraseUserFat = 0;
	m_map_COMPicasso.clear();
	m_map_imageNameCheckSum.clear();
	g_hResourceContext = NULL;

	g_pFirehoseDownload = this;

	m_iFileCount = 0;
	m_iFileIndex = 0;

}

CDownload8916::CDownload8916(int i_COMPort, CString str_multiDLFlag, const std::string& str_platform)
:CDLFormatCommand(COMDeviceName(i_COMPort), str_platform), m_b_reOpenAfterReset(false), m_b_resetMode(true), m_b_overridePartition(false), m_p_adbDevice(NULL)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	m_i_rework = 0;
	m_i_reboot = 0;
	m_i_DDRCheck = 0;
	m_i_DumpEmmcLog = 0;
	m_i_RebootMode = 0;
	m_i_checkSum = 0;
	m_i_backupNV = 0;
	m_i_SupportQDownload = 0;
	m_i_eraseUserFat = 0;
	m_map_COMPicasso.clear();
	m_map_imageNameCheckSum.clear();
	m_str_multiDLFlag = str_multiDLFlag;
	m_i_COMPort = i_COMPort;
}

CDownload8916::~CDownload8916(void)
{
	if (m_p_adbDevice) 
	{
		delete m_p_adbDevice;
		m_p_adbDevice = NULL;
	}

	if (m_hQMSL_MSVC10R != NULL)
	{
		FreeLibrary (m_hQMSL_MSVC10R);
		m_hQMSL_MSVC10R = NULL;
	}

	if (m_hSparselibModule != NULL)
	{
		FreeLibrary (m_hSparselibModule);
		m_hSparselibModule = NULL;
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
bool CDownload8916::MultiDownload(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL) 
{
	bool b_result = false;

	if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	{
		/* Hex Download */
		b_result = Download(b_speedUp, b_reOpenAfterReset, nDLPROTOCOL);

		/* Fastboot Download */
		if (b_result)
		{
			b_result = FastbootDownload();
		}
	}
	else if ((m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL))
	{
		/* Fastboot Download */
		b_result = FastbootDownload();
	}
	else
	{
		AddMsg("DL mode is not PreDL or ReDL.", DownloadStep::None, 100);
		return false;
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
bool CDownload8916::Download(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL) 
{
	AddMsg("Hex Download", None, 100);
	
	m_b_reOpenAfterReset = b_reOpenAfterReset;

	

	/* DMSS DL + Streaming DL */
	//if (SAHARADL() && StreamingDL(b_speedUp, nDLPROTOCOL))
	//{
	//} 
	m_hQMSL_MSVC10R = LoadLibrary("QMSL_MSVC10R.dll");
	if(m_hQMSL_MSVC10R == NULL)
	{
		AddMsg(false, "Load QMSL_MSVC10R Fail", FireHoseProtocol);
		return false;
	}


	if (FlashProgrammer() && FireHose())
	{
		FreeLibrary(m_hQMSL_MSVC10R);

		AddMsg("Sahara , FireHose PASS , reboot ", None, 20);
		return true;
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
		b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterReset, false, 15, 1000);
		if (b_reConnect == false) 
		{
			m_str_errorCode = DLErr_Reboot_Phone_Fail;
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

		b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterGo, false, 15, 1000);
		AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
		if (b_reConnect == false) 
		{
			m_str_errorCode = DLErr_Reboot_Phone_Fail;
			return false;
		}

		if (!StreamingHello(b_speedUp, g_i_8916HelloRetryTimes, g_i_8916HelloTimeout, nDLPROTOCOL)) 
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
bool CDownload8916::DMSSDL(void)
{
	using namespace PLATFORM_TOOL;

	if (!DMSSNop(g_i_8916NopNopRetryTimes, g_i_8916NopTimeout)) 
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
	//if (!SetFastBootFlag())
	//{
	//	m_str_errorCode = DLERR_SETFASTBOOTFLAG;
	//	return false;
	//}

	WORD dw_address = 0;
	if ((m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL))
	{
		CString pthToolDir;
		::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
		pthToolDir.ReleaseBuffer();
		pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

		string str_hex = pthToolDir + _T("\\MPRG8930.hex");
		m_str_hex = str_hex;

		//CString iniFilePth = _T("C:\\PE_Config\\GDETB1A\\Setting\\UIControl.ini");

		//if (!IsPathExist(iniFilePth)) 
		//{
		//	return false;
		//}
		//TCHAR szValue[MAX_PATH];
		//TCHAR* szSection;
		//memset(szValue, 0, sizeof(szValue));
		//::GetPrivateProfileString(_T("COMMON"), _T("SW_VER"), _T(""), szValue, MAX_PATH, iniFilePth);
		//string str_hex = szValue;
		//str_hex = _T("C:\\Image\\GDETB1A\\") + str_hex + _T("\\MPRG8960.hex");
		//m_str_hex = str_hex;
	}
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

	return true;
}

/*****************************************************************************
* Function name: QMSLCallBack_8916
* Summary      : CallBack function for SAHARADL
* Parameters   : 
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Erin.Chang     2013/09/18          First version             
*****************************************************************************/
void QMSLCallBack_8916(HANDLE hQMSLContext,
      char	*sMsg,
      unsigned short iMsgSize)
{
	BYTE readBuffer[256] = {0};
	memcpy(readBuffer, sMsg, iMsgSize);

	FILE *pFile = NULL;
	pFile = fopen("D:\\Log\\UI_RD\\QMSLCallBack_8916.log", "wb");
	fseek(pFile, 0, SEEK_END);
	fwrite(readBuffer, 1, sizeof(readBuffer)-1, pFile);
	fclose(pFile);
}


bool CDownload8916::SAHARADL(void)
{
	pQLIB_QPHONEMS_ConnectServer_Sahara ConnectServer_Sahara = (pQLIB_QPHONEMS_ConnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_Sahara");
	pQLIB_QPHONEMS_Sahara_FlashProgrammer Sahara_FlashProgrammer = (pQLIB_QPHONEMS_Sahara_FlashProgrammer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_Sahara_FlashProgrammer");
	pQLIB_DisconnectServer DisconnectServer = (pQLIB_DisconnectServer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_Sahara");

	unsigned int iPort = m_i_COMPort;
	HANDLE m_hPhone = NULL;
	unsigned long saharaVer = 0;
	unsigned char cret;

	unsigned long* szMsmid = 0;
	unsigned char szBGetMsmId= '0';
	int n_mode = 0;
	unsigned long n_timeout = 100;

		unsigned long version = 0;
		unsigned long msmid = 0;
		unsigned long timeout = 0;
		unsigned char bGetMsmId = 0;
		int mode = 0;

	//QLIB_QPHONEMS_SaharaConfigureCallback SWDL_QPHONEMS_CB = QMSLCallBack_8916;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_8916;

	if(ConnectServer_Sahara != NULL)
	{
		//m_hPhone = ConnectServer_Sahara(iPort, &saharaVer, szMsmid, szBGetMsmId,n_mode, n_timeout, SWDL_QPHONEMS_CB);
		m_hPhone = ConnectServer_Sahara(iPort, &version, &msmid, bGetMsmId, mode, timeout, SWDL_QPHONEMS_CB);
		
		AddMsg(true, "ConnectServer_Sahara", SaharaProtocol);
	}
	else
	{
		return false;
	}

	if(saharaVer == 0){
		//AddMsg(false, "ConnectServer_Sahara_fail", SaharaProtocol);
		//return false;
	}

	if(Sahara_FlashProgrammer != NULL)
	{
		char cHexPath[256] = {0};
		strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());
		cret = Sahara_FlashProgrammer(m_hPhone, cHexPath);

		if(cret) //1
		{
			//SetUIMessage("Flash Programmer Success");
		}
		else         //0
		{
			//SetUIMessage("Flash Programmer Fail");
			return false;
		}

		AddMsg(true, "Sahara_FlashProgrammer", SaharaProtocol);
	}
	else
	{
		return false;
	}
    Sleep(1000);

	if (m_hPhone != NULL && DisconnectServer != NULL)
	{
		DisconnectServer(m_hPhone);
		m_hPhone = NULL;
	}

	return true;
}


void QMSLCallBack_FireHose(HANDLE hQMSLContext,
				  char	*szMsg,
				  unsigned short iMsgSize)
{
	if (NULL ==g_pFirehoseDownload)
		return;

	g_pFirehoseDownload->RecordQMSLCallBack(szMsg);
}


void CDownload8916::RecordQMSLCallBack( char	*szMsg)
{
	if (NULL ==g_pFirehoseDownload)
		return;
//	TraceLog(MakeSTDstring("[QMSLCallBack]%s", szMsg));

	AddMsg("[RecordQMSLCallBack]", DownloadStep::None, 10);

	g_pFirehoseDownload->RecordQMSLCallBack(szMsg);

		//Update Progress
		m_iFileIndex++;

		if (m_iFirehoseDLMode == FIREHOSE_DL_MODE_FASTBOOT_ONLY){
			AddMsg("Get ADB Fail.", DownloadStep::None, ((int)((m_iFileIndex * 80) / m_iFileCount) + 10, 100));
		}
		else{

			AddMsg("Get ADB Fail.", DownloadStep::None,((int)((m_iFileIndex * 60) / m_iFileCount) + 10, 100));
		}


		
	//AddMsg("Get ADB Fail.", DownloadStep::None, 100);
}

void swdlCallBack(
   HANDLE hContextID,
   char * sMsg,
   unsigned short iMsgSize)
{

   printf("\n%s", sMsg);

	FILE *pFile = NULL;
	pFile = fopen("C:\\FireHoseQMSLCallBack_8916.log", "wb");
	fseek(pFile, 0, SEEK_END);
	fwrite(sMsg, 1, sizeof(sMsg)-1, pFile);
	fclose(pFile);
}



bool CDownload8916::FlashProgrammer()
{
	unsigned int iPort = m_i_COMPort;
	HANDLE hPhone = NULL;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_FireHose;

	//Connect Server
	pQLIB_QPHONEMS_ConnectServer_Sahara ConnectServer_Sahara = (pQLIB_QPHONEMS_ConnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_Sahara");
	if(ConnectServer_Sahara != NULL)
	{
		unsigned long version = 0;
		unsigned long msmid = 0;
		unsigned long timeout = 0;
		unsigned char bGetMsmId = 0;
		int mode = 0;
		hPhone = ConnectServer_Sahara(iPort, &version, &msmid, bGetMsmId, mode, timeout, SWDL_QPHONEMS_CB);

		if (hPhone == 0)
		{
			//SetUIMessage("Connect Sahara Server Fail");
			return false;
		}
		else
		{
			//SetUIMessage("Connect Sahara Server Success");
		}
	}
	else
	{
		return false;
	}

	//Configure Callback
	pQLIB_QPHONEMS_SaharaConfigureCallback Sahara_ConfigureCallback = (pQLIB_QPHONEMS_SaharaConfigureCallback)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_SaharaConfigureCallback");
	Sahara_ConfigureCallback(hPhone, SWDL_QPHONEMS_CB);

	//Flash Programmer
	pQLIB_QPHONEMS_Sahara_FlashProgrammer Sahara_FlashProgrammer = (pQLIB_QPHONEMS_Sahara_FlashProgrammer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_Sahara_FlashProgrammer");
	if(Sahara_FlashProgrammer != NULL)
	{
		char szHexPath[MAX_PATH] = {0};
		unsigned char szResult = 0;

		char cHexPath[256] = {0};
		//strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());

		sprintf_s(szHexPath, MAX_PATH, "%s", m_str_hex.c_str());
		//sprintf_s(szHexPath, MAX_PATH, "%s", m_strProgrammerFilePath.c_str());

		szResult = Sahara_FlashProgrammer(hPhone, szHexPath);
		if(szResult) //1
		{
			//SetUIMessage("Flash Programmer Success");
		}
		else         //0
		{
		//	SetUIMessage("Flash Programmer Fail");
			return false;
		}
	}
	else
	{
		return false;
	}

	//Disconnect Server
	pQLIB_DisconnectServer_Sahara DisconnectServer_Sahara = (pQLIB_DisconnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_Sahara");
	if (hPhone != NULL && DisconnectServer_Sahara != NULL)
	{
		DisconnectServer_Sahara(hPhone);
		//SetUIMessage("Disconnect Sahara Server Success");
	}
	hPhone = NULL;


	return true;
}


bool CDownload8916::FireHose(void)
{
	unsigned int iPort = m_i_COMPort;
	unsigned char szResult = 0;

	HANDLE hPhone = NULL;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_FireHose;

	std::string m_strRawProgramFile;
	std::string m_strRawProgramFilePath;
	std::string m_strPatchFile;
	std::string m_strPatchFilePath;


	Sleep(3000);//wait for 3 second form qualcomm demo code


	//pQLIB_QPHONEMS_QLIB_SetLogFlags SetLogFlags = (pQLIB_QPHONEMS_QLIB_SetLogFlags)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_SetLogFlags");
	//pQLIB_QPHONEMS_QLIB_StartLogging StartLogging  = (pQLIB_QPHONEMS_QLIB_StartLogging)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_StartLogging");
	/*loading Lib*/
	pQLIB_QPHONEMS_ConnectServer_FireHose ConnectServer_FireHose = (pQLIB_QPHONEMS_ConnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_FireHose");
	pQLIB_DisconnectServer_FireHose DisconnectServer_FireHose = (pQLIB_DisconnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_FireHose");
	pQLIB_QPHONEMS_FireHosePower FireHose_Reboot = (pQLIB_QPHONEMS_FireHosePower)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHosePower");
//	pQLIB_QPHONEMS_FireHosePower_WithDelay FireHosePower_WithDelay = (pQLIB_QPHONEMS_FireHosePower_WithDelay)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHosePower_WithDelay");

	//Configure Callback
	//pQLIB_QPHONEMS_FireHoseConfigureCallback FireHose_ConfigureCallback = (pQLIB_QPHONEMS_FireHoseConfigureCallback)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseConfigureCallback");
//	FireHose_ConfigureCallback(hPhone, SWDL_QPHONEMS_CB);

	//FireHose_ConfigureCallback(g_hResourceContext, swdlCallBack);
  //  QLIB_QPHONEMS_FireHoseConfigureCallback(g_hResourceContext, swdlCallBack);

 //   SetLogFlags( g_hResourceContext, LOG_ALL );
 //   StartLogging( g_hResourceContext,  "C:\\QlibDemo_Sahara_Firehose.txt");


	//Connect Server

	hPhone = ConnectServer_FireHose(iPort, swdlCallBack);
	if (hPhone == 0)
	{
		//SetUIMessage("Connect Firehose Server Fail");
		return false;
	}
	else
	{
		//SetUIMessage("Connect Firehose Server Success");
	}



	//Ping device
	pQLIB_QPHONEMS_FireHoseNOP FireHose_NOP = (pQLIB_QPHONEMS_FireHoseNOP)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseNOP");
	szResult = FireHose_NOP(hPhone);
	if(szResult) //1
	{
		//SetUIMessage("Ping Firehose device Success");
	}
	else         //0
	{
		//SetUIMessage("Ping Firehose device Fail");
		return false;
	}

	//Configure
	char *MaxPayloadSizeToTargetInBytes = "49152";
	char *memory_name = "eMMC";
	char *target_name = "8x26";
	char *AckPacketNumber = "100";
	unsigned char bUseAckRawDataAttr = false;
	unsigned char bUseSkipWriteAttr = false;
	unsigned char bUseAlwaysValidateAttr = false;
	unsigned char bUseVerboseAttr = false;
	unsigned char bUseAckRawDataEveryNumPackets = false;

	pQLIB_QPHONEMS_FireHoseConfigure FireHose_Configure = (pQLIB_QPHONEMS_FireHoseConfigure)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseConfigure");
	szResult = FireHose_Configure( hPhone,
		MaxPayloadSizeToTargetInBytes,
		memory_name,
		target_name,
		AckPacketNumber,
		bUseAckRawDataAttr,
		bUseSkipWriteAttr,
		bUseAlwaysValidateAttr,
		bUseVerboseAttr,
		bUseAckRawDataEveryNumPackets
		);

	if(szResult) //1
	{
		//SetUIMessage("Configure Firehose Success");
	}
	else         //0
	{
		//SetUIMessage("Configure Firehose Fail");
		return false;
	}

	//Download image
	char szRawProgramFile[MAX_PATH] = {0};
	char szPatchFile[MAX_PATH] = {0};
	float imageSizeInMB = 0.0;
	float throughput = 0.0;

		//char cHexPath[256] = {0};
		//strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());
		//cret = Sahara_FlashProgrammer(m_hPhone, cHexPath);

	//CString csStrRawProgramFilePath, csStrPatchFilePath;
	CString csImgPath =  m_str_hex.c_str();
	csImgPath = csImgPath.Left (csImgPath.ReverseFind(_T('\\'))); 
	
	//csStrRawProgramFilePath = csImgPath + _T("\\rawprogram0.xml");
	//m_strPatchFilePath = csImgPath + _T("\\patch0_fastbootimage.xml.xml");
	m_strRawProgramFilePath = csImgPath + _T("\\rawprogram0_fastbootimage.xml");
	m_strPatchFilePath = csImgPath + _T("\\patch0_fastbootimage.xml");
	sprintf_s(szRawProgramFile, MAX_PATH, "%s", m_strRawProgramFilePath.c_str());
	sprintf_s(szPatchFile, MAX_PATH, "%s", m_strPatchFilePath.c_str());

	//	GetCurrentDirectory(MAX_PATH, szNowPath);
	//CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	//SetCurrentDirectory(WorkDir);

	//SetUIProgress(10, 100);
	//SetUIMessage("Download image...");

	//orig: pass
	pQLIB_QPHONEMS_UploadEmmcImage_FireHose UploadEmmcImage_FireHose = (pQLIB_QPHONEMS_UploadEmmcImage_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_UploadEmmcImage_FireHose");
	szResult = UploadEmmcImage_FireHose(hPhone, szRawProgramFile, szPatchFile, &imageSizeInMB, &throughput);
	if(szResult) //1
	{
	//	SetUIMessage("Download image Success");
	}
	else         //0
	{
		//SetUIMessage("Download image Fail");
		return false;
	}

	//Reboot device

	szResult = FireHose_Reboot(hPhone, _T("reset"));
	if(szResult) //1
	{
		//SetUIMessage("Reboot device Success");
	}
	else         //0
	{
		//SetUIMessage("Reboot device Fail");
		return false;
	}



    printf("\n----------------QLIB_DisconnectServer_FireHose---------------------------\n");
    DisconnectServer_FireHose(hPhone);




	return true;
}
bool CDownload8916::IsPathExist(const CString& path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
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
bool CDownload8916::StreamingDL(bool b_speedUp, DownloadProtocol nDLPROTOCOL)
{
	using namespace PLATFORM_TOOL;

	bool b_res = false;
	bool b_reConnect = false;

	CReOpenComport obj_ReOpenComport(this, true);
	b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterGo, false, 15, 1000);
	AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
	if (b_reConnect == false) 
	{
		m_str_errorCode = DLErr_Reboot_Phone_Fail;
		return false;
	}

	if (!StreamingHello(b_speedUp, g_i_8916HelloRetryTimes, g_i_8916HelloTimeout, nDLPROTOCOL)) 
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

	if (!StreamingDLMultiImage(Q8K_OPEN_MULTI_MODE_EMMC_USER,  m_str_msimage,  b_speedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_i_8916CloseImageTimeout)) 
	{
		m_str_errorCode = DLErr_DL_MsImage_Fail;
		return false;
	}

	// set fastboot flag
	char * pEnd;
	int data = strtol(m_str_multiDLFlag, &pEnd, 16);
	byte byteData[1] = {0};
	memcpy(&byteData, &data, 1);

	if (!StreamingStrWrite(byteData, 1, 0x40A400, b_speedUp, g_nRetryTimes, g_nTimeout))
	{
		m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
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
		b_res = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterReset, false, 15, 1000);
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
bool CDownload8916::SetFastBootFlag(void)
{
	bool b_res = false;
	bool b_speedUp = false;
	char * pEnd;
	DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
	int data = strtol(m_str_multiDLFlag, &pEnd, 16);
	byte byteData[1] = {0};
	memcpy(&byteData, &data, 1);

	if (!StreamingHello(b_speedUp, g_i_8916HelloRetryTimes, g_i_8916HelloTimeout, nDLPROTOCOL)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
		return false;
	}

	if (!StreamingSecurity("", b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
		return false;
	}

	if (!(StreamingOpenHeaderImage(Q8K_OPEN_MULTI_MODE_EMMC_USER, "", false, g_nRetryTimes, g_nTimeout) &&
		StreamingStrWrite(byteData, 1, 0x003FFE00, b_speedUp, g_nRetryTimes, g_nTimeout) &&
		StreamingClose(false, g_nRetryTimes, g_nTimeout)))
	{
		m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
		return false;
	}

	//if (!StreamingStrWrite(pData, 1, 0x003ffe00, b_speedUp, g_nRetryTimes, g_nTimeout))
	//{
	//	m_str_errorCode = DLERR_SETFASTBOOTFLAG;
	//	return false;
	//}


	if (!StreamingReset(b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Reset_Fail;
		return false;
	}

	if (m_b_reOpenAfterReset) 
	{
		CReOpenComport obj_ReOpenComport(this);
		b_res = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterReset, false, 15, 1000);
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
* Function name: DDRTest      
* Summary      : DDR Check.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/28          First version             
*****************************************************************************/
bool CDownload8916::DDRTest(void)
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
bool CDownload8916::ReadDDRTestReslult(void)
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
* Function name: StreamingDLIRAMLog      
* Summary      : Dump IRAM log in eDL mode
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          EriN.Chang     2013/02/18          First version             
*****************************************************************************/
bool CDownload8916::StreamingDLIRAMLog(void)
{
	bool b_res = true;
	bool b_speedUp = false;
	char * pEnd;
	DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
	int data = strtol(m_str_multiDLFlag, &pEnd, 16);
	byte byteData[1] = {0};
	memcpy(&byteData, &data, 1);

	if (!StreamingHello(b_speedUp, g_i_8916HelloRetryTimes, g_i_8916HelloTimeout, nDLPROTOCOL)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
		return false;
	}

	if (!StreamingSecurity("", b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
		return false;
	}

	if (!(StreamingOpenHeaderImage(Q8K_OPEN_MULTI_MODE_EMMC_USER, "", false, g_nRetryTimes, g_nTimeout) &&
		StreamingStrWrite(byteData, 1, 0x003FFE00, b_speedUp, g_nRetryTimes, g_nTimeout) &&
		StreamingClose(false, g_nRetryTimes, g_nTimeout)))
	{
		m_str_errorCode = DLErr_Set_Fastboot_Flag_Fail;
		return false;
	}

	BYTE readBuffer[2048];
	memset(readBuffer, 0, sizeof(readBuffer));

	// Read Picasso number
	if (!StreamingStrRead(readBuffer, 0x200, (40960 * 512), false, g_nRetryTimes, g_nTimeout))
	{
		m_str_errorCode = DLErr_Read_PicassoNum_Fail;
		return false;
	}

	CString strFilePath;
	CString strFileName((LPCSTR)readBuffer, 2048);
	strFileName = strFileName.Mid(19, 10);

	if(_taccess("D:\\Log\\eMMC\\", 0) == -1)
	{
		system("mkdir D:\\Log\\eMMC\\");
	}

	// Dump SBL log
	strFilePath.Format(_T("D:\\Log\\eMMC\\%s_SBL.bin"), strFileName);
	memset(readBuffer, 0, sizeof(readBuffer));

	if (!StreamingIRAMRead(readBuffer, 0x200, 0x2F000, false, g_nRetryTimes, g_nTimeout))
	{
		m_str_errorCode = DLErr_Dump_SBL_Log_Fail;
		return false;
	}

	if(strFilePath.IsEmpty() == 0)
	{
		FILE *pFile;
		pFile = fopen (strFilePath, "wb");
		fwrite (readBuffer , 1 , sizeof(readBuffer)-1 , pFile);
		fclose (pFile);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	// Dump PBL log
	strFilePath.Format(_T("D:\\Log\\eMMC\\%s_PBL.bin"), strFileName);
	memset(readBuffer, 0, sizeof(readBuffer));

	if (!StreamingIRAMRead(readBuffer, 0x200, 0x43F18, false, g_nRetryTimes, g_nTimeout))
	{
		m_str_errorCode = DLErr_Dump_PBL_Log_Fail;
		return false;
	}

	if(strFilePath.IsEmpty() == 0)
	{
		FILE *pFile;
		pFile = fopen (strFilePath, "wb");
		fwrite (readBuffer , 1 , sizeof(readBuffer)-1 , pFile);
		fclose (pFile);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	return b_res;
}

bool CDownload8916::GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
{
	map_strstrCOMDevice.clear();

	BOOL bContinue = FALSE;

	// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	::SetupDiClassGuidsFromNameA("Ports", NULL, 0, &uiGuidSize);

	if (uiGuidSize > 0)
	{
		ptGuidBuf = new GUID[uiGuidSize];
		ASSERT(ptGuidBuf != NULL);

		if (ptGuidBuf != NULL)
		{
			bContinue = ::SetupDiClassGuidsFromNameA("Ports", ptGuidBuf, uiGuidSize, &uiGuidSize);
		}
	}

	// 2. Returns a device information set that contains all devices of a specified class
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue)
	{
		hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// 3. Start enumeration
	DWORD uiIndex = 0;

	while (bContinue)
	{
		// Returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
		{
			// Opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// Read the name of the port.
				char szDev[256];
				DWORD uiType = 0, uiDevSize = 256;
				memset(szDev, 0, 256);

				if ((::RegQueryValueExA(hKey, "PortName", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					std::string strTmp = szDev;

					memset(szDev, 0, 256);
					uiDevSize = 256;
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					else
						map_strstrCOMDevice[strTmp] = std::string("");
				}

				// Close the key.
				::RegCloseKey(hKey);
			}
		}

		++uiIndex;
	}

	// clean-up.
	if (ptGuidBuf != NULL)
		delete [] ptGuidBuf;

	if (hDevInfoSet != INVALID_HANDLE_VALUE)
		::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return true;
}

bool CDownload8916::DetectDiagPort()
{

	std::map<std::string, std::string> map_strstrCOMDeviceName;
	m_strKeyword.assign("QUALCOMM");

	if (GetCOMPortDevByWDK(map_strstrCOMDeviceName))
	{
		std::map<std::string, std::string>::iterator itr;

		if (map_strstrCOMDeviceName.size() < 1)
		{
			//CmdLog("No COM device detected");
			return false;
		}
		else
		{
			bool bFound = false;
			for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
			{
				std::string strDev = itr->first;
				std::string strDevName = itr->second;
				std::transform(strDevName.begin(), strDevName.end(), strDevName.begin(), ::toupper);

				if (strDevName.find(m_strKeyword) !=  std::string::npos)
				{
					bFound = true;
					size_t iPosition;
					if ((iPosition = strDev.find("COM")) != std::string::npos)
						m_strPROLIFICcomPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
					else
						m_strPROLIFICcomPort = m_strDeviceName = strDev;
				}
			}

			if (! bFound)
			{

				AddMsg(_T("Fail to find QUALCOMM Comport"), DownloadStep::None, 100);
				return false;
			}

		}
	}
	else
	{
		//CmdLog("Fail to detect COM device with WDK");
		return false;
	}


	return true;
}



bool CDownload8916::StreamingDLeMMCLog(int nSector)
{
	if ( m_i_DumpEmmcLog != 1 ) {//0:dumpLog, else:do nothing
		
		return true;
	}

	bool b_res = true;
	bool b_speedUp = false;
	char * pEnd;
	DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
	int data = strtol(m_str_multiDLFlag, &pEnd, 16);
	bool b_reConnect = false;
	
	AddMsg(_T("Starting StreamingDLeMMCLog"), DownloadStep::None, 100);
	if(DetectDiagPort() == false){
		AddMsg(_T("DetectDiagPort Fail"), DownloadStep::None, 100);
		return false;
	}

	b_res = SAHARADL();
	if ( b_res == false ){
		AddMsg(_T("run SAHARADL Fail"), DownloadStep::None, 100);
		return false;
	}

	CReOpenComport obj_ReOpenComport(this, true);
	b_reConnect = obj_ReOpenComport.ReOepnComport(g_i_8916ReOpenComPortAfterGo, false, 15, 1000);
	AddMsg(b_reConnect, "ReOpenComPortAfterGo", StreamingProtocol);
	if (b_reConnect == false) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
		return false;
	}

	if (!StreamingHello(b_speedUp, g_i_8916HelloRetryTimes, g_i_8916HelloTimeout, nDLPROTOCOL)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Hello_Fail;
		return false;
	}

	if (!StreamingSecurity("", b_speedUp)) 
	{
		m_str_errorCode = DLErr_DL_Streaming_Security_Fail;
		return false;
	}


	if (!(StreamingOpenHeaderImage(Q8K_OPEN_MULTI_MODE_EMMC_USER, "", false, g_nRetryTimes, g_nTimeout) ))
	{
		m_str_errorCode = DLErr_Setting_Open_Fail;
		return false;
	}


	BYTE readBuffer[2048];
	memset(readBuffer, 0, sizeof(readBuffer));

	// Read Picasso number
	if (!StreamingStrRead(readBuffer, 0x200, (16384 * 512), false, g_nRetryTimes, g_nTimeout))//40960 * 512
	{
		m_str_errorCode = DLErr_Read_PicassoNum_Fail;
		return false;
	}

	CString strFilePath;
	CString strFileName((LPCSTR)readBuffer, 2048);
	strFileName = strFileName.Mid(19, 10);

	if(_taccess("D:\\Log\\eMMC\\", 0) == -1)
	{
		system("mkdir D:\\Log\\eMMC\\");
	}

	// Dump SBL log
	strFilePath.Format(_T("D:\\Log\\eMMC\\%s_eMMC.bin"), strFileName);
	memset(readBuffer, 0, sizeof(readBuffer));

	if (!StreamingDLeMMCRead(readBuffer, 0x200, 8194 * 512, false, g_nRetryTimes, g_nTimeout))//0x200, 8194 * 512,
	{
		m_str_errorCode = DLErr_Dump_SBL_Log_Fail;
		return false;
	}

	if(strFilePath.IsEmpty() == 0)
	{
		FILE *pFile;
		pFile = fopen (strFilePath, "wb");
		fwrite (readBuffer , 1 , sizeof(readBuffer)-1 , pFile);
		fclose (pFile);
	}


	return b_res;
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
CString CDownload8916::GetErrorCode(void) 
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
int CDownload8916::GetComport(void)
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
void CDownload8916::SetIsOveridePartition(bool b_overridePartition)
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
void CDownload8916::SetResetMode(bool b_reset)
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
bool CDownload8916::SetImageFilePath(const int i_imageType, const std::string& str_filePath)
{
	bool b_res = false;

	/* Check Input */
	if(str_filePath.c_str() == _T("")) 
	{
		return false;
	}

	switch(i_imageType)
	{
	case IMAGE_TYPE_8916::Q8K_HEX:
		m_str_hex = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_MSIMAGE:
		m_str_msimage = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PARTITIONBIN:
		m_str_partitionBin = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SBL1:
		m_str_sbl1 = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_HYP:
		m_str_hyp = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_HYPBAK:
		m_str_hypbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RPM:
		m_str_rpm = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_TZ:
		m_str_tz = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_APPBOOT:
		m_str_appboot = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SBL1BAK:
		m_str_sbl1bak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RPMBAK:
		m_str_rpmbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_TZBAK:
		m_str_tzbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_ABOOTBAK:
		m_str_abootbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_BOOT:
		m_str_boot = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RECOVERY:
		m_str_recovery = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SYSTEM:
		m_str_system = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_CACHE:
		m_str_cache = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PERSIST:
		m_str_persist = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_QGLOGFILTER:
		m_str_qlogfilter = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_QGLOG:
		m_str_qglog = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_NONHLOS:
		m_str_nonhlos = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_OEM:
		m_str_oem = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SPLASH:
		m_str_splash = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_USERDATA:
		m_str_userdata = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_DRM:
		m_str_drm = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PASSPORT:
		m_str_passport = str_filePath;
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
bool CDownload8916::FastbootDownload(void)
{
	AddMsg("Fastboot Download.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Backup NV partition */
	if (m_i_backupNV == 1)
	{
		if (( (m_str_DLMode == DOWNLOAD_MODE_PREDL) && (m_i_rework == 1) ) || (m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL))
		{
			if (!BackupNVPartition())
			{
				return false;
			}
		}
	}

	if (m_i_checkSum == 1)
	{
		/* Read CheckSum file */
		if (!ReadCheckSumFile())
		{
			m_str_errorCode = DLErr_Md5_Check_Open_Fail;
			return false;
		}
	}

	if (!ExecuteFastboot(_T("oem cal_md5 0")))
	{
		m_str_errorCode = DLErr_Md5_Check_Open_Fail;
		return false;
	}

	/*============= ADB download all images =========================*/
	if (!FastbootFlashCmd("passport")) 
	{
		m_str_errorCode = DLErr_DL_passport_Fail;
		return false;
	}

	if (!FastbootFlashCmd("partition")) 
	{
		m_str_errorCode = DLErr_DL_Partition_Table_Fail;
		return false;
	}

	//if (!ExecuteFastboot(_T("erase:sbl1")))
	//{
	//	m_str_errorCode = DLErr_DL_SBL1_Fail;
	//	return false;
	//}

	if (!FastbootFlashCmd("sbl1")) 
	{
		m_str_errorCode = DLErr_DL_SBL1_Fail;
		return false;
	}

	if (!FastbootFlashCmd("rpm")) 
	{
		m_str_errorCode = DLErr_DL_RPM_Fail;
		return false;
	}

	if (!FastbootFlashCmd("tz")) 
	{
		m_str_errorCode = DLErr_DL_TZ_Fail;
		return false;
	}

	if (!FastbootFlashCmd("hyp")) 
	{
		m_str_errorCode = DLErr_DL_SDI_Fail;
		return false;
	}

	if (!FastbootFlashCmd("hypbak")) 
	{
		m_str_errorCode = DLErr_DL_SDI_Fail;
		return false;
	}

	if (!FastbootFlashCmd("aboot")) 
	{
		m_str_errorCode = DLErr_DL_Emmc_Appboot_Fail;
		return false;
	}

	if (!FastbootFlashCmd("sbl1bak")) {
		m_str_errorCode = DLErr_DL_SBL1BAK_Fail;
		return false;
	}

	if (!FastbootFlashCmd("rpmbak")) {
		m_str_errorCode = DLErr_DL_RPMBAK_Fail;
		return false;
	}

	if (!FastbootFlashCmd("tzbak")) {
		m_str_errorCode = DLErr_DL_TZBAK_Fail;
		return false;
	}

	if (!FastbootFlashCmd("abootbak")) {
		m_str_errorCode = DLErr_DL_ABOOTBAK_Fail;
		return false;
	}

	if (!FastbootFlashCmd("boot")) {
		m_str_errorCode = DLErr_DL_Boot_Fail;
		return false;
	}

	if (!FastbootFlashCmd("recovery")) 
	{
		m_str_errorCode = DLErr_DL_Recovery_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:system")))
	{
		m_str_errorCode = DLErr_DL_System_Fail;
		return false;
	}

	if (!FastbootFlashCmd("system")) 
	{
		m_str_errorCode = DLErr_DL_System_Fail;
		return false;
	}

	if (!FastbootFlashCmd("persist")) 
	{
		m_str_errorCode = DLErr_DL_Persist_Fail;
		return false;
	}

	if (!FastbootFlashCmd("Qlogfilter")) 
	{
		m_str_errorCode = DLErr_DL_Qlogfilter_Fail;
		return false;
	}

	if (!FastbootFlashCmd("modem")) 
	{
		m_str_errorCode = DLErr_DL_Nonhlos_Fail;
		return false;
	}


	if (!FastbootFlashCmd("splash")) 
	{
		m_str_errorCode = DLErr_DL_Splash_Fail;
		return false;
	}

	if (!FastbootFlashCmd("oem")) 
	{
		m_str_errorCode = DLErr_DL_flex_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:emgdload")))//lion
	{
		m_str_errorCode = DLErr_Erase_Qdlog_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:userdata")))
	{
		m_str_errorCode = DLErr_Format_Userdata_Fail;
		return false;
	}

	if (!FastbootFormatCmd(_T("userdata")))
	{
		m_str_errorCode = DLErr_Format_Userdata_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:cache")))
	{
		m_str_errorCode = DLErr_DL_Cache_Fail;
		return false;
	}

	if (!FastbootFormatCmd("cache")) 
	{
		m_str_errorCode = DLErr_DL_Cache_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:Qglog")))
	{
		m_str_errorCode = DLErr_Erase_Qdlog_Fail;
		return false;
	}

	if (!FastbootFormatCmd("Qglog")) 
	{
		m_str_errorCode = DLErr_Format_Qglog_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:drm")))
	{
		m_str_errorCode = DLErr_Format_Drm_Fail;
		return false;
	}

	if (!FastbootFormatCmd(_T("drm")))
	{
		m_str_errorCode = DLErr_Format_Drm_Fail;
		return false;
	}	

	if (!ExecuteFastboot(_T("erase:Qcust")))
	{
		m_str_errorCode = DLErr_Format_Qcust_Fail;
		return false;
	}

	if (!FastbootFormatCmd(_T("Qcust")))
	{
		m_str_errorCode = DLErr_Format_Qcust_Fail;
		return false;
	}	

	//if (!ExecuteFastboot(_T("oem write Qdlog 0 4194304")))
	//{
	//	m_str_errorCode = DLErr_Erase_Qdlog_Fail;
	//	return false;
	//}
	//**** erase root status end ****//

	if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	{
		//set all variables off
		if (!ExecuteFastboot(_T("oem all_var Qoff")))
		{
			m_str_errorCode = DLErr_Oem_All_Qon;
			return false;
		}
		// set all variables on
		//if (!ExecuteFastboot(_T("oem all_var Qon")))
		//{
		//	m_str_errorCode = DLErr_Oem_All_Qon;
		//	return false;
		//}

		 // L1 request erase fsg on PreDL 20140711, Lion

		if (!ExecuteFastboot(_T("oem set_clear_wp fsg 0")))
		{
			m_str_errorCode = DLErr_Erase_Fsg_Fail;
			return false;
		}


		if (!ExecuteFastboot(_T("erase:fsg")))
		{
			m_str_errorCode = DLErr_Erase_Fsg_Fail;
			return false;
		}
	}

	if ((m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL)) 
	{
		if (!ExecuteFastboot(_T("oem emmc_rst_n_enable")))
		{
			m_str_errorCode = DLErr_Oem_Emmc_Rst_N_Enable_Fail;
			return false;
		}

		//set all variables off
		if (!ExecuteFastboot(_T("oem all_var Qoff")))
		{
			m_str_errorCode = DLErr_Oem_All_Qon;
			return false;
		}


	}

	/*os_dl only for factory to ship image , pline*/
	if ( m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL)
	{
		if (!ExecuteFastboot(_T("erase:persistent")))
		{
			m_str_errorCode = DLErr_DL_Persist_Fail;
			return false;
		}
	}

	if (!ExecuteFastboot(_T("erase:modemst1")))
	{
		m_str_errorCode = DLErr_Erase_Modemst1_Fail;
		return false;
	}

	if (!ExecuteFastboot(_T("erase:modemst2")))
	{
		m_str_errorCode = DLErr_Erase_Modemst2_Fail;
		return false;
	}


	if (m_i_eraseUserFat == 1)
	{
		if (!EraseUserFat())
		{
			m_str_errorCode = DLErr_Erase_User_Fat_Fail;
			return false;
		}
	}

	if (m_i_reboot == 1)
	{
		/* Reboot to online mode */
		if (!Reboot()) 
		{
			m_str_errorCode = DLErr_Reboot_Phone_Fail;
			return false;
		}
	} 
	else
	{
		/* Set FTD mode flag */
		//if (!SetFTDMode())
		//{
		//	m_str_errorCode = DLERR_REBOOTSETMODE;
		//	return false;
		//}
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
* Function name: GetOemMainVersion      
* Summary      : Call ExecuteFastbootOut()
* Parameters   : str_parameter : image version
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0         Evonne.Hsieh    2012/08/01          First version             
*****************************************************************************/
bool CDownload8916::GetOemMainVersion(CString& str_parameter)
{
	AddMsg("Get factory version.", DownloadStep::None, 100);
	if (m_str_DLMode == DOWNLOAD_MODE_REDL)
	{
		CString outString;
		if (!ExecuteFastbootOut(_T("getvar:OEM_MAIN_VERSION"), outString))
		{
			m_str_errorCode = CommErr_Get_Software_Version_Fail;
			return false;
		}
		str_parameter = outString;
	}

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
bool CDownload8916::FastbootFlashCmd(const std::string& str_parameter)
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
		str_imageFilePath = m_str_partitionBin;
	}
	else if (str_parameter == _T("sbl1"))
	{
		str_imageFilePath = m_str_sbl1;
	}
	else if (str_parameter == _T("rpm")) 
	{
		str_imageFilePath = m_str_rpm;
	}
	else if (str_parameter == _T("tz"))
	{
		str_imageFilePath = m_str_tz;
	}
	else if (str_parameter == _T("hyp")) 
	{
		str_imageFilePath = m_str_hyp;
	}
	else if (str_parameter == _T("hypbak")) 
	{
		str_imageFilePath = m_str_hypbak;
	}
	else if (str_parameter == _T("aboot")) 
	{
		str_imageFilePath = m_str_appboot;
	}
	else if (str_parameter == _T("sbl1bak")) 
	{
		str_imageFilePath = m_str_sbl1bak;
	}
	else if (str_parameter == _T("rpmbak")) 
	{
		str_imageFilePath = m_str_rpmbak;
	}
	else if (str_parameter == _T("tzbak")) 
	{
		str_imageFilePath = m_str_tzbak;
	}
	else if (str_parameter == _T("abootbak")) 
	{
		str_imageFilePath = m_str_abootbak;
	}
	else if (str_parameter == _T("boot")) 
	{
		str_imageFilePath = m_str_boot;
	}
	else if (str_parameter == _T("recovery")) 
	{
		str_imageFilePath = m_str_recovery;
	}
	else if (str_parameter == _T("system")) 
	{
		str_imageFilePath = m_str_system;
	}
	else if (str_parameter == _T("persist")) 
	{
		str_imageFilePath = m_str_persist;
	}
	else if (str_parameter == _T("Qlogfilter")) 
	{
		str_imageFilePath = m_str_qlogfilter;
	}
	else if (str_parameter == _T("modem"))
	{
		str_imageFilePath = m_str_nonhlos;
	}
	else if (str_parameter == _T("splash"))
	{
		str_imageFilePath = m_str_splash;
	}
	else if (str_parameter == _T("oem"))
	{
		str_imageFilePath = m_str_oem;
	}
	else if (str_parameter == _T("passport")) 
	{
		str_imageFilePath = m_str_passport;
	}

	//else if (str_parameter == _T("userdata")) 
	//{
	//	str_imageFilePath = m_str_userdata;
	//}
	//else if (str_parameter == _T("cache"))
	//{
	//	str_imageFilePath = m_str_cache;
	//}
	//else if (str_parameter == _T("Qglog")) 
	//{
	//	str_imageFilePath = m_str_qglog;
	//}
	//else if (str_parameter == _T("drm")) 
	//{
	//	str_imageFilePath = m_str_drm;
	//}
	//else if (str_parameter == _T("Qcust")) 
	//{
	//	str_imageFilePath = m_str_qcust;
	//}


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

		//CString csFileName = sz_fileName;
	if (strncmp (QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), "system", 6) == 0){
		/* ADB flash */
		if (!ADBFlashAllLargeSize(QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), (LPCTSTR)str_imageFilePath.c_str()))
		{
			return false;
		}
	}else
	{
	/* ADB flash */
	if (!ADBFlashAll(QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), (LPCTSTR)str_imageFilePath.c_str()))
	{
		return false;
	}
	}
	

	str_message.Format("Fastboot download Success:%s", str_imageFilePath.c_str());
	AddMsg(str_message, DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: FastbootFormatCmd      
* Summary      : Call ADBFlash()
* Parameters   : str_parameter : image partition name
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8916::FastbootFormatCmd(const std::string& str_parameter)
{
	/* Check input parameter */
	if (str_parameter == _T("")) 
	{
		return false;
	}

	CString str_message;
	CString str_command;

	// getvar:partition-type
	str_command.Format(_T("getvar:partition-type:%s"), str_parameter.c_str());
	
	CString outType;
	if (!ExecuteFastbootOut((char*)str_command.GetString(), outType))
	{
		str_message.Format("getvar:partition-type:%s :: FAIL", str_parameter.c_str());
		AddMsg(str_message, DownloadStep::None, 100);
		return false;
	}	
	str_message.Format("getvar:partition-type:%s :: %s", str_parameter.c_str(), outType.GetString());
	AddMsg(str_message, DownloadStep::None, 100);

	// getvar:partition-size
	str_command.Format(_T("getvar:partition-size:%s"), str_parameter.c_str());

	CString outSize;
	if (!ExecuteFastbootOut((char*)str_command.GetString(), outSize))
	{
		str_message.Format("getvar:partition-size:%s :: FAIL", str_parameter.c_str());
		AddMsg(str_message, DownloadStep::None, 100);
		return false;
	}	
	str_message.Format("getvar:partition-size:%s :: %s", str_parameter.c_str(), outSize.GetString());
	AddMsg(str_message, DownloadStep::None, 100);


	/* Set tmp image file name */
	CString str_TempPathBuffer = m_str_partitionBin.c_str(); 
	str_TempPathBuffer = str_TempPathBuffer.Left(str_TempPathBuffer.ReverseFind('\\'));

	CString str_Tmp;
	std::string str_imageFilePath;
	if (str_parameter == _T("userdata")) 
	{
		str_Tmp.Format("%s\\userdata_0%s.img", str_TempPathBuffer, m_str_multiDLFlag);
	}
	else if (str_parameter == _T("drm")) 
	{
		str_Tmp.Format("%s\\drm_0%s.img", str_TempPathBuffer, m_str_multiDLFlag);
	}
	else if (str_parameter == _T("Qcust")) 
	{
		str_Tmp.Format("%s\\Qcust_0%s.img", str_TempPathBuffer, m_str_multiDLFlag);
	}
	else if (str_parameter == _T("Qglog")) 
	{
		str_Tmp.Format("%s\\Qglog_0%s.img", str_TempPathBuffer, m_str_multiDLFlag);
	}
	else if (str_parameter == _T("cache")) 
	{
		str_Tmp.Format("%s\\cache_0%s.img", str_TempPathBuffer, m_str_multiDLFlag);
	}
	str_imageFilePath = str_Tmp.GetString();

	/* Check if download this image ( =0 ) */
	if (str_imageFilePath == _T("")) 
	{
		//CString str_message;
		str_message.Format("Skip Fastboot download:%s", str_parameter.c_str());
		AddMsg(str_message, DownloadStep::None, 100);
		return true;
	}

	CString currentDir;
	GetModuleFileName(NULL, currentDir.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
	currentDir.ReleaseBuffer();
	currentDir = currentDir.Left(currentDir.ReverseFind('\\'));

	// Convert HEX(CStrint) to DEC(unsigned long long)
	int nStringLen = outSize.GetLength()+1;
	TCHAR *strHex = new TCHAR[nStringLen];
	memset(strHex, 0, nStringLen);
	_tcscpy(strHex, outSize.GetString());

	int i = 0, ch;
	unsigned long long ullSize = 0;

	while(0 != (nStringLen-1))
	{
		ch = *(strHex + i);
		if(('0' <= ch && ch <= '9'))
		{
			ullSize = ullSize * 16;
			ullSize = ullSize + (ch - '0');
		}
		else if('a' <= ch && ch <= 'f')
		{
			ullSize = ullSize * 16;
			ullSize = ullSize + (ch - 'W');
		}
		else if('A' <= ch && ch <= 'F')
		{
			ullSize = ullSize * 16;
			ullSize = ullSize + (ch - '7');
		}

		nStringLen--;
		i++;
	}
	delete [] strHex;
	strHex = NULL;
	///////////////////////////////////////////////////////////////////

	if (outType.Compare("ext4") == 0)
	{
		/*
			int nStringLen = outSize.GetLength()+1;
			TCHAR *strHex = new TCHAR[nStringLen];
			memset(strHex, 0, nStringLen);
			_tcscpy(strHex, outSize.GetString());
			unsigned long ulSize = _tcstoul(strHex, NULL, 16);
			delete [] strHex;
			strHex = NULL;
		*/
		
		//str_command.Format(_T("/C \"%s\\make_ext4fs_0%s.exe\" -l %u -s %s"), currentDir, m_str_multiDLFlag, ulSize, str_imageFilePath.c_str());
		str_command.Format(_T("/C \"%s\\make_ext4fs_0%s.exe\" -l %llu -s %s"), currentDir, m_str_multiDLFlag, ullSize, str_imageFilePath.c_str());
			ShellExecute(0, "open", "cmd", str_command, 0, SW_HIDE);
			AddMsg(str_command, DownloadStep::None, 100);
			str_message.Format("Create image file : %s", str_imageFilePath.c_str());
			AddMsg(str_message, DownloadStep::None, 100);
		}
	else if (outType.Compare("fat") == 0)
	{
	}
	else{
		str_message.Format("can't find partition-type : %s", outType);
		AddMsg(str_message, DownloadStep::None, 100);
		return false;
	}
	Sleep(2000);

	//CString str_message;
	str_message.Format("Fastboot download:%s", str_imageFilePath.c_str());
	AddMsg(str_message, DownloadStep::None, 100);

	// Check image file path name exist
	if (::_taccess(str_imageFilePath.c_str(), 0) != 0) 
	{
		Sleep(2000);
		if (::_taccess(str_imageFilePath.c_str(), 0) != 0) 
		{
			str_message.Format("Image is not exist:%s", str_imageFilePath.c_str());
			AddMsg(str_message, DownloadStep::None, 100);
			return false;
		}
	}

	if (m_i_checkSum == 1)
	{
		// Write CheckSum
		if (!WriteCheckSum((LPCTSTR)str_imageFilePath.c_str()))
		{
			return false;
		}
	}

	// ADB flash
	if (!ADBFlashAll(QSYNC_LIB::GetAnsi((LPCTSTR)str_parameter.c_str()).c_str(), (LPCTSTR)str_imageFilePath.c_str()))
	{
		DeleteFile((LPCTSTR)str_imageFilePath.c_str());
		return false;
	}
	DeleteFile((LPCTSTR)str_imageFilePath.c_str());

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
bool CDownload8916::ADBFlash(const char* sz_parmeter, const void* p_data, unsigned long long l_fileLen)
{
	/* Check input parameter */
	if ((sz_parmeter == NULL) || (p_data == NULL) || (l_fileLen <= 0)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{

		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
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
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA) 
		{
			char* c_p_data = (char*) p_data;
			unsigned long long l_count = 0;
			bool b_writenSuccess = true;
			unsigned long long i_progress = 0;

			/* Max write byte:4096 */
			while (l_fileLen > 0 && b_writenSuccess) 
			{
				i_progress = (l_count * 100) / l_totalLen;
				AddMsg(sz_imgae, DownloadStep::ADBProtocol, static_cast<int>(i_progress));
				int xfer = (l_fileLen > 4096) ? 4096 : static_cast<int>(l_fileLen);

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

			if (b_writenSuccess && m_p_adbDevice->ReadResponse(120000) == RESPONSE::OKAY) 
			{
				/* flash command --- ""flash:%s""  example:"flash boot" */
				char sz_command[64] = {0};
				sprintf(sz_command, "flash:%s", sz_parmeter);

				/* Write flash command */
				if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 600000)) 
				{
					b_result = (m_p_adbDevice->ReadResponse(600000) == RESPONSE::OKAY);
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
		AddMsg("Write download command Failure (in ADBFlash)", DownloadStep::None, 100);
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
void* CDownload8916::LoadFile(const char* sz_fileName, unsigned long long* i_fileSize)
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
ADBDevice* CDownload8916::GetADBDevice(const char* sz_comPort)
{
	AddMsg("CDownload8916 GetADBDevice Start ...", DownloadStep::None, 100);

	try
	{
		// Do something to throw an exception.

		/* Get ADB by the COM port */
		if (m_p_adbDevice == NULL)
		{
			m_p_adbDevice = new ADBDevice(adb_class_id, 0, 0, DEVICE_INTERFACE_ID, sz_comPort, 40);
			if(m_p_adbDevice) 
			{
				m_p_adbDevice->m_usb_buf_ADBDevice[USB_BUF_SIZE_ADBDevice] = 0;/*lion add*/
				m_p_adbDevice->m_usb_buf_len_ADBDevice = 0;
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

	}
	catch(CException* e)
	{
		AfxMessageBox("GetADBDevice Error!!");
		e->Delete();
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
bool CDownload8916::GetADB(void)
{
	AddMsg("Get ADB ...", DownloadStep::None, 100);

	/* Get ADB port */
	char sz_comPort[64];
	memset(sz_comPort, 0, sizeof(sz_comPort));

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
		//strcpy(sz_comPort, "#{");
		if (m_str_multiDLFlag.Find(_T("1")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa20"));
		}
		else if (m_str_multiDLFlag.Find(_T("2")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa21"));
		}
		else if (m_str_multiDLFlag.Find(_T("3")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa22"));
		}
		else if (m_str_multiDLFlag.Find(_T("4")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa23"));
		}
		else if (m_str_multiDLFlag.Find(_T("5")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa24"));
		}
		else if (m_str_multiDLFlag.Find(_T("6")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa25"));
		}
		else if (m_str_multiDLFlag.Find(_T("7")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa26"));
		}
		else if (m_str_multiDLFlag.Find(_T("8")) != -1) {
			sprintf(sz_comPort, "%s", _T("0x7766aa27"));
		}
	}
	else
	{
		int i_COMPort = 0;
		i_COMPort = GetComport();
		sprintf(sz_comPort, "&%d#{", i_COMPort);
	}

	/* Find ADB */
	if (!GetADBDevice(sz_comPort)) 
	{
		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		m_str_errorCode = CommErr_Get_ADB_Fail;
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
bool CDownload8916::SetFTDMode(void)
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
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
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
bool CDownload8916::Reboot(void)
{
	AddMsg("Fastboot reboot.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{

		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Write command "reboot" */
	if (m_p_adbDevice->Write("reboot", 6, 60000)) 
	{
		/* Read response command "reboot" */
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
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
bool CDownload8916::EraseSBL1(void)
{
	AddMsg("Fastboot erase sbl1.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Write command "erase sbl1" */
	if (m_p_adbDevice->Write("erase:sbl1", 10, 60000)) 
	{
		/* Read response command */
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
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


bool CDownload8916::EraseUserFat(void)
{
	AddMsg("Fastboot erase user fat.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	/* Write command "erase mdm" */
	if (m_p_adbDevice->Write("erase:mdm", 9, 60000)) 
	{
		/* Read response command */
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
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
bool CDownload8916::WriteCheckSum(CString str_imageFilePathName)
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
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
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
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
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
bool CDownload8916::ReadCheckSumFile(void)
{
	CSingleLock singleLock(&m_obj_loadcheckSumCritSection, TRUE);

	AddMsg("Read CheckSum file.", DownloadStep::None, 100);

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
bool CDownload8916::ReadFA(char* sz_FAData)
{
	AddMsg("Read FA.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	int i_length = FALENGTH;
	bool b_result = false;
	char sz_buffer[512] = {0};

	/* dump protocol --- "dump:%s"  example:"dump:otpfa" */
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s", "otpfa");

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		RESPONSE enum_readRosponse = UNKNOW;
		enum_readRosponse = m_p_adbDevice->ReadResponse(60000);
		if (enum_readRosponse == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", i_length);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA);
				if (b_result) 
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

					b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
					if (!b_result)
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
bool CDownload8916::WriteFA(char* sz_FAData)
{
	AddMsg("Write FA.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}
	
	int i_length = FALENGTH;
	bool b_result = false;

	/* download protocol --- "download:%08x"  example:"download:00162c00" */
	char sz_command[64] = {0};
	sprintf(sz_command, "download:%08x", i_length);
	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000))
	{
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA)
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

			if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
			{
				char sz_command[64] = {0};
				sprintf(sz_command, "flash:%s", "otpfa");

				if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 120000))
				{
					b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
					if (!b_result) 
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
bool CDownload8916::ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	bool b_speedUp = false;
	bool b_reOpenAfterReset = false;
	bool b_res = true;
	DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
		
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}
	AddMsg("Read FA.", DownloadStep::None, 100);

	bool b_result = false;
	int  i_length = i_sectorSize;
	char sz_buffer[512] = {0};

	/* dump protocol --- "dump:%s"  example:"dump:otpfa0x0+512" */
	char sz_sectorSize[32] = {0};
	sprintf(sz_sectorSize, "%d+%d", i_sectorNum * 512, i_sectorSize);
	char sz_command[1024] = {0};
	sprintf(sz_command, "dump:%s%s", "Qfa@", sz_sectorSize);

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		RESPONSE enum_readRosponse = UNKNOW;
		enum_readRosponse = m_p_adbDevice->ReadResponse(60000);
		if (enum_readRosponse == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", i_length);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
				if (b_result) 
				{
					unsigned long l_readLen = 0;
					if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 60000) && (l_readLen == FALENGTH)) 
					{
						memcpy(sz_sectorData, sz_buffer, i_sectorSize);
					}
					else
					{
						AddMsg("Read upload data Fail.", DownloadStep::None, 100);
						return false;
					}

					b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
					if (!b_result)
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

	AddMsg("Read FA Success.", DownloadStep::None, 100);
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
bool CDownload8916::WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	AddMsg("Write FA.", DownloadStep::None, 100);

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	int i_length = i_sectorSize;
	bool b_result = false;

	/* fastboot oem write Qfa offset length value */
	char sz_command[1024] = {0};
	int offset = i_sectorNum * 512;

	if(i_sectorNum == 0){
		sprintf(sz_command, "oem write Qfa %d %d %s", offset, 255, sz_sectorData);
		b_result = runWriteFASector(sz_command);
		if (b_result){
			sprintf(sz_command, "oem write Qfa %d %d %s", offset+256, 512-1, sz_sectorData+256);
			b_result = runWriteFASector(sz_command);
		}
	}
	else{
		sprintf(sz_command, "oem write Qfa %d %d %s", offset, offset+i_sectorSize-1, sz_sectorData);
		b_result = runWriteFASector(sz_command);
	}
	
	return b_result;
}


bool CDownload8916::runWriteFASector(char *sz_sectorData){
	if (m_p_adbDevice->Write(sz_sectorData, strlen(sz_sectorData), 60000))
	{
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
		{
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
* Function name: BackupNVPartition      
* Summary      : Backup NV partition.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8916::BackupNVPartition(void)
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
	bool b_result = false;
	/* dump protocol --- "dump:%s"  example:"dump:0xa0000_1280" */
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s", "0xa0000_1152");

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	{
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", l_length1);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA);
				if (b_result) 
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

					b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
					if (!b_result)
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
	b_result = false;
	/* dump protocol --- "dump:%s"  example:"dump:0xa0000_1280" */
	char sz_command2[64] = {0};
	sprintf(sz_command2, "dump:%s", "0xb0000_128");

	if (m_p_adbDevice->Write(sz_command2, strlen(sz_command2), 60000)) 
	{
		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
		{
			/* upload command --- "upload:%08x"  example:"upload:0007d000" */
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", l_length2);
			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
			{
				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA);
				if (b_result) 
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

					b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
					if (!b_result)
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
	return b_result;
}

bool CDownload8916::SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg)
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

bool CDownload8916::ADBFlashAll(const char* sz_parmeter, const char* sz_fileName)
{
	/* Check input parameter */
	if ((sz_parmeter == NULL) || (sz_fileName == NULL)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	CString str_message;
	HANDLE h_file  = NULL;
	DWORD dw_fileSize = 0;
	h_file = CreateFile(sz_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h_file == INVALID_HANDLE_VALUE)
	{
		Sleep(3000);
		h_file = CreateFile(sz_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (h_file == INVALID_HANDLE_VALUE)
		{
			str_message.Format("CreateFile Failure with GetLastError = %d.", GetLastError());
			AddMsg(str_message, DownloadStep::None, 100);		
			return false;
		}
	}
	//else 
	if (h_file != INVALID_HANDLE_VALUE)
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
			if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA) 
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

				if (m_p_adbDevice->ReadResponse(120000) == RESPONSE::OKAY) 
				{
					/* flash command --- ""flash:%s""  example:"flash boot" */
					char sz_command[64] = {0};
					sprintf(sz_command, "flash:%s", sz_parmeter);

					/* Write flash command */
					if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 600000)) 
					{
						if (!(m_p_adbDevice->ReadResponse(600000) == RESPONSE::OKAY)) 
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
			AddMsg("Write download command Failure (in ADBFlashAll)", DownloadStep::None, 100);
			CloseHandle(h_file);
			return false;
		}
	}

	if (h_file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(h_file);
	}
	return true;
}

static void saveLog_Sparse(CString message)
{
	CString pthLog;
	pthLog = _T("D:\\Log\\UI_RD");
	
	if (::GetFileAttributes(pthLog) != INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\Sparse.txt");

	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;

	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}


static void saveLog_Value(CString message)
{
	CString pthLog;
	pthLog = _T("D:\\Log\\UI_RD");
	
	if (::GetFileAttributes(pthLog) != INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\system.CSV");

	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;

	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}


/*OK 2013 1222*/

static int fb_download_data_sparse_write(void *priv, const void *data, int len){
	ADBDevice*  m_p_adbDevice = (ADBDevice*)priv;
	int to_write;
	const char *ptr = (const char*)data;
	char usb_buf[USB_BUF_SIZE] = {0};

#ifdef _DebugSparse			
	CString csTmp;
	csTmp.Format("original data[0] = %d ,len = %d , data[len-1] = %d , usb_buf_len = %d'\n", ptr[0], len,ptr[len-1],m_p_adbDevice->m_usb_buf_len_ADBDevice );
	saveLog_Sparse(csTmp);
#endif

	if (m_p_adbDevice->m_usb_buf_len_ADBDevice) {

		to_write = min(USB_BUF_SIZE - m_p_adbDevice->m_usb_buf_len_ADBDevice, len);
		//memcpy(usb_buf + m_p_adbDevice->m_usb_buf_len_ADBDevice, ptr, to_write);
		memcpy(m_p_adbDevice->m_usb_buf_ADBDevice + m_p_adbDevice->m_usb_buf_len_ADBDevice, ptr, to_write);
		m_p_adbDevice->m_usb_buf_len_ADBDevice += to_write;
		//usb_buf_len += to_write;
		ptr += to_write;
		len -= to_write;
	}

	if (m_p_adbDevice->m_usb_buf_len_ADBDevice == USB_BUF_SIZE) {

#ifdef _DebugSparse		
		csTmp.Format("len=buf write usb_buf[0] = %d , ptr[USB_BUF_SIZE-1] = %d , usb_buf_len = %d'\n", m_p_adbDevice->m_usb_buf_ADBDevice[0], m_p_adbDevice->m_usb_buf_ADBDevice[USB_BUF_SIZE-1], m_p_adbDevice->m_usb_buf_len_ADBDevice); 
		saveLog_Sparse(csTmp);
#endif	
		
		if (!m_p_adbDevice->Write(m_p_adbDevice->m_usb_buf_ADBDevice, USB_BUF_SIZE, 60000))
		{
			return -1;
		}
//		usb_buf_len = 0;
		m_p_adbDevice->m_usb_buf_len_ADBDevice = 0;
	}

	if (len > USB_BUF_SIZE) {
		if (m_p_adbDevice->m_usb_buf_len_ADBDevice > 0) {
			sprintf(ERROR, "internal error: usb_buf not empty\n");
			return -1;
		}
		to_write = round_down(len, USB_BUF_SIZE);

#ifdef _DebugSparse
		csTmp.Format("len>buf write ptr[0] = %d ,to_write = %d , ptr[to_write-1] = %d , usb_buf_len = %d'\n", ptr[0], to_write,ptr[to_write-1],m_p_adbDevice->m_usb_buf_len_ADBDevice );
		saveLog_Sparse(csTmp);
#endif	
		char* c_p_data = (char*) ptr;
		unsigned long long l_count = 0;
		bool b_writenSuccess = true;
		 int l_fileLen = to_write; 
		 int iMaxSize = 1024 * 1024;

		while (l_fileLen > 0 && b_writenSuccess) 
		{
			int xfer = (l_fileLen > iMaxSize) ? iMaxSize : static_cast<int>(l_fileLen);
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
			return -1;

#ifdef _DebugSparse		
			csTmp = "[#]m_p_adbDevice->Write fail\n";
			saveLog_Sparse(csTmp);
#endif
		}

		ptr += to_write;
		len -= to_write;
	}

	if (len > 0) {
		if (len > USB_BUF_SIZE) {
			sprintf(ERROR, "internal error: too much left for usb_buf\n");
			return -1;
		}
		//memcpy(usb_buf, ptr, len);
		memcpy(m_p_adbDevice->m_usb_buf_ADBDevice, ptr, len);
		m_p_adbDevice->m_usb_buf_len_ADBDevice = len;
	}

	return 0;
}



/*OK 20131222 */
int CDownload8916::fb_download_data_sparse_flush( ADBDevice*  m_p_adbDevice )
{
   // if (usb_buf_len > 0) {
	if (  m_p_adbDevice->m_usb_buf_len_ADBDevice > 0) {
		
#ifdef 	saveLog_Sparse	
		CString csTmp;
		csTmp.Format("flush len=buf write usb_buf[0] = %d , ptr[USB_BUF_SIZE-1] = %d , usb_buf_len = %d'\n", m_p_adbDevice->m_usb_buf_ADBDevice[0],m_p_adbDevice->m_usb_buf_ADBDevice[USB_BUF_SIZE-1],m_p_adbDevice->m_usb_buf_len_ADBDevice );
		saveLog_Sparse(csTmp);
#endif

		if (!m_p_adbDevice->Write(m_p_adbDevice->m_usb_buf_ADBDevice,  m_p_adbDevice->m_usb_buf_len_ADBDevice, 60000))
		{
			return -1;
		}
        //usb_buf_len = 0;
		 m_p_adbDevice->m_usb_buf_len_ADBDevice = 0 ;
    }

    return 0;
}

bool CDownload8916::ADBFlashAllLargeSize(const char* sz_parmeter, const char* sz_fileName)
{
	char sz_imgae[64] = {0};
	CString str_message;
	int64_t sz64;
	int64_t limit;
	int nFileDivision = 0;
	int nSparseDLLHandle = 0;
	CString csPortNum;

	/* Check input parameter */
	if ((sz_parmeter == NULL) || (sz_fileName == NULL)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);
		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}


	if  (LoadSparseLibDLL() == false ){
		return false;
	}else
	{
		str_message.Format("LoadSparseLibDLL Successful");
		AddMsg(str_message, DownloadStep::None, 10);
	}

	CString str_command;
	str_command = ("getvar:max-download-size");

	CString outSize;
	if (!ExecuteFastbootOut((char*)str_command.GetString(), outSize))
	{
		str_message.Format("getvar:max-download-size:%s :: FAIL");
		AddMsg(str_message, DownloadStep::None, 100);
		return false;
	}	
	str_message.Format("getvar:max-download-size:: %s", outSize.GetString());
	AddMsg(str_message, DownloadStep::None, 100);

	limit = strtoul(outSize, NULL, 0);
	//usb_buf_len = 0 ;

	/*check access file*/
	if (::_taccess(sz_fileName, 0) != 0) 
	{
		AddMsg("can't access image", DownloadStep::None, 100);
		return false;
	}

	/**/
	if (limit) {
		struct sparse_file **s = CF_load_sparse_files(sz_fileName, limit);
		str_message.Format("load_sparse_files s = %d,%d",s ,*s );
		AddMsg(str_message , DownloadStep::None, 100);

		if (s == NULL) {
			AddMsg( "cannot sparse load" , DownloadStep::None, 100);
			return false;
		}else{

			while (*s) {
				//str_message.Format("while (*s) :: %d,%d\n",s, *s );
				//str_message.Format("FileDivision[%d] :: %d \n", nSparseDLLHandle, nFileDivision++);
				///AddMsg(str_message , DownloadStep::None, 100);
				//saveLog_Value(str_message);

				sz64 = CF_sparse_file_len_dll(*s, true, false);
				
				str_message.Format("sparse_file_len[%d]:: %d \n", nFileDivision++, ( sz64 / 1024) );
				AddMsg(str_message , DownloadStep::None, 100);

#ifdef _DebugSparse
		saveLog_Sparse(str_message);
#endif
				sprintf(sz_imgae, "Fastboot download:%s", sz_parmeter);

				/* download protocol --- "download:%08x"  example:"download:00162c00" */
				char sz_command[64] = {0};
				sprintf(sz_command, "download:%08x", sz64);

				/* Write download protocol */
				if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
				{
					/* ADB Read --- 如果返回DATA，则可以发送数据  */
					if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA)
					{
						int r;
						
						r = CF_sparse_file_callback(*s, true, false, fb_download_data_sparse_write, (void*)m_p_adbDevice);
						
						if (r < 0) {
							return false;
						}
						fb_download_data_sparse_flush(m_p_adbDevice);

						Sleep(5000);
						if (m_p_adbDevice->ReadResponse(30000) == RESPONSE::OKAY)
						{
							/* download protocol --- "download:%08x"  example:"download:00162c00" */
							char sz_command[64] = {0};
							sprintf(sz_command, "flash:system");

							/* Write download protocol */
							if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 600000)) 
							{
								if (m_p_adbDevice->ReadResponse(600000) == RESPONSE::OKAY)
								{
								}else{
									AddMsg("Write download command Failure (in ADBFlashAllLargeSize 1)", DownloadStep::None, 100);
									return false;
								}
							}else{
								AddMsg("Write download command Failure (in ADBFlashAllLargeSize 2)", DownloadStep::None, 100);
								return false;
							}
						}else{
							AddMsg("Write download command Failure (in ADBFlashAllLargeSize 3)", DownloadStep::None, 100);
							return false;
						}
					}
				}
				*s++;
			}// while loop
		}// end if 
		/*************************************************/
	}else{
		AddMsg( "FAIL , limit == NULL " , DownloadStep::None, 100);
		return false;
	}

	return true;
}


bool CDownload8916::ExecuteFastboot(char* sz_command)
{
	/* Check input parameter */
	if ((sz_command == NULL)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	CString strMsg;
	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 6000)) 
	{
		CString sResponse;
		while (1)
		{
			int ret = m_p_adbDevice->ReadResponse7K(sResponse, 6000);
			if (ret == RESPONSE::OKAY || ret == RESPONSE::FAIL) 
			{
				break;
			}
		}
		
		if (sResponse.Find(_T("FAIL")) != -1)
		{
			strMsg.Format(_T("Fastboot command \"%s\" response Fail."), sz_command);
			AddMsg(strMsg, DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		strMsg.Format(_T("Write fastboot command \"%s\" response Fail."), sz_command);
		AddMsg(strMsg, DownloadStep::None, 100);
		return false;
	}

	strMsg.Format(_T("Fastboot command \"%s\" Success."), sz_command);
	AddMsg(strMsg, DownloadStep::None, 100);
	return true;
}

bool CDownload8916::ExecuteFastbootOut(char* sz_command, CString& out)
{
	/* Check input parameter */
	if ((sz_command == NULL)) 
	{
		return false;
	}

	/* Get ADB */
	if (!GetADB()) 
	{
		StreamingDLeMMCLog(8194);

		AddMsg("Get ADB Fail.", DownloadStep::None, 100);
		return false;
	}

	if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 6000)) 
	{
		CString sResponse;
		while (1)
		{
			int ret = m_p_adbDevice->ReadResponse7K(sResponse, 6000);
			if (ret == RESPONSE::OKAY || ret == RESPONSE::FAIL) 
			{
				if (ret == RESPONSE::OKAY) 
				{
					sResponse.Replace("OKAY", "");
					out = sResponse;
				}
				break;
			}
		}
		
		if (sResponse.Find(_T("FAIL")) != -1)
		{
			AddMsg("Fastboot command response Fail.", DownloadStep::None, 100);
			return false;
		}
	}
	else 
	{
		AddMsg("Write fastboot command Fail.", DownloadStep::None, 100);
		return false;
	}

	AddMsg("Fastboot command Success.", DownloadStep::None, 100);
	return true;
}
bool CDownload8916::LoadSparseLibDLL(){

	m_hSparselibModule = LoadLibrary(_T("sparselib.dll"));


	if(m_hSparselibModule == NULL)
	{
		AddMsg("Load LoadSparseLibDLL DLL fail.", DownloadStep::None, 100);
		return false;
	}


	CF_load_sparse_files = (PF_load_sparse_files)GetProcAddress(m_hSparselibModule , "load_sparse_files_dll");
	if(CF_load_sparse_files == NULL)
	{
		AddMsg("Load CF_load_sparse_files fail.", DownloadStep::None, 100);
		return false;
	}

	CF_sparse_file_callback = (PF_sparse_file_callback)GetProcAddress(m_hSparselibModule , "sparse_file_callback_dll");
	if(CF_sparse_file_callback == NULL)
	{
		AddMsg("Load CF_sparse_file_callback  fail.", DownloadStep::None, 100);
		return false;
	}

	CF_sparse_file_len_dll = (PF_sparse_file_len_dll)GetProcAddress(m_hSparselibModule , "sparse_file_len_dll");
	if(CF_sparse_file_len_dll == NULL)
	{
		AddMsg("Load CF_sparse_file_len_dll fail.", DownloadStep::None, 100);
		return false;
	}

	return true;

}

