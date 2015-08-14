/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: Modules\Luffy\Unify_GP\Phone\AndroidPhone.cpp $
   
when       who			what, where, why
--------   ----		   ------------------------------------------------------------- 
==============================================================================*/

#include "stdafx.h"
#include "AndroidPhone.h"


GP_IMPLEMENT_DYNCREATE(CAndroidPhone)

using namespace ComDeviceUtility;
using namespace StringUtility;
using namespace ExecCmdUtil;
//using namespace System.IO::File;



#pragma warning (push)

CAndroidPhone	*m_pMyPhone = NULL;
CMutex CAndroidPhone::m_Mutex_Command;

void GetResponse(string str)
{
	m_pMyPhone->CmdLog(str);
}

CAndroidPhone::CAndroidPhone()
{
	m_pMyPhone = this;

	m_hQisdaModule = NULL;
	m_bNeedToGetFA = true;

	memset(m_szWorkingDir, 0, 512);
	memset(m_szFaData, 0, FA_SIZE);	
}

CAndroidPhone::~CAndroidPhone()
{
	if (m_hQisdaModule != NULL)
	{
		FreeLibrary(m_hQisdaModule);
		m_hQisdaModule = NULL;
	}

	if (m_pMyPhone != NULL)
		m_pMyPhone = NULL;
}

bool CAndroidPhone::InitData(pugi::xml_document *pXMLTestItem, pugi::xml_document *pXMLConfig)
{
	m_strKeyword = pXMLConfig->child("Configuration").child("Phone").child("Keyword").child_value();
 	m_strQisdaCommandModuleName = pXMLTestItem->child("Configuration").child("Phone").child("QisdaCmdModule").child_value();
	
	m_szBbTestToolDir = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("BBTestToolDir").child_value();
	if(m_szBbTestToolDir.empty())
	{	return false;
	}
	m_szBbTestToolName = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("BBTestToolName").child_value();
	if(m_szBbTestToolName.empty())
	{	return false;
	}
	m_szAdbToolDir = pXMLTestItem->child("Configuration").child("Phone").child("AdbToolDir").child_value();
	if(m_szAdbToolDir.empty())
	{	return false;
	}
	m_szPADProductToolDir = pXMLTestItem->child("Configuration").child("Phone").child("PADProductToolDir").child_value();
	if(m_szPADProductToolDir.empty())
	{	return false;
	}
	m_szFlashToolName = pXMLTestItem->child("Configuration").child("Phone").child("FlashToolName").child_value();
	if(m_szFlashToolName.empty())
	{	return false;
	}
	m_szFactoryImageDir = pXMLTestItem->child("Configuration").child("Phone").child("FactoryImageInfo").child("ImageDir").child_value();
	if(m_szFactoryImageDir.empty())
	{	return false;
	}
	m_szFactoryFwFile = pXMLTestItem->child("Configuration").child("Phone").child("FactoryImageInfo").child("fw").child_value();
	if(m_szFactoryFwFile.empty())
	{	return false;
	}
	m_szFactoryMiscFile = pXMLTestItem->child("Configuration").child("Phone").child("FactoryImageInfo").child("MISC").child_value();
	m_szFactoryRecoveryFile = pXMLTestItem->child("Configuration").child("Phone").child("FactoryImageInfo").child("RECOVERY").child_value();
	m_szFactorySystemFile = pXMLTestItem->child("Configuration").child("Phone").child("FactoryImageInfo").child("SYSTEM").child_value();
	m_szUserImageDir = pXMLTestItem->child("Configuration").child("Phone").child("UserImageInfo").child("ImageDir").child_value();
	if(m_szUserImageDir.empty())
	{	return false;
	}
	m_szUserFwFile = pXMLTestItem->child("Configuration").child("Phone").child("UserImageInfo").child("fw").child_value();
	if(m_szUserFwFile.empty())
	{	return false;
	}
	m_szUserMiscFile = pXMLTestItem->child("Configuration").child("Phone").child("UserImageInfo").child("MISC").child_value();
	m_szUserRecoveryFile = pXMLTestItem->child("Configuration").child("Phone").child("UserImageInfo").child("RECOVERY").child_value();
	m_szUserSystemFile  = pXMLTestItem->child("Configuration").child("Phone").child("UserImageInfo").child("SYSTEM").child_value();


	m_szPMIC = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("PMU").child("ITEMCMD").child_value();

	m_szNAND = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("Nand").child("ITEMCMD").child_value();
	m_szNandSize = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("Nand").child("NandSize").child_value();
	//m_dNandSize = atof(szString.c_str());

	m_szDDR = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("DDR").child("ITEMCMD").child_value();
	m_szDDRSize = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("DDR").child("DDRSize").child_value();
	//m_dDDRSize = atof(szString.c_str());

	m_szHDMI = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("HDMI").child("ITEMCMD").child_value();

	m_szUart = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("Uart").child("ITEMCMD").child_value();

	m_szEthernet = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("Ethernet").child("ITEMCMD").child_value();

	m_szSDWriteProtected = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDWriteProtected").child("ITEMCMD").child_value();
	m_szSDProtected = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDWriteProtected").child("Protected").child_value();

	m_szSDPlugin = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDPlugin").child("ITEMCMD").child_value();
	m_szSDType = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDPlugin").child("SDType").child_value();

	m_szSDWriteRead = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDWriteRead").child("ITEMCMD").child_value();

	m_szSDeMMC = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDeMMCSize").child("ITEMCMD").child_value();
	m_szSDeMMCSize =  pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDeMMCSize").child("eMMCSize").child_value();

	m_szSDClockRate = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("SDClockRate").child("ITEMCMD").child_value();

	m_szRecoverKey = pXMLTestItem->child("Configuration").child("Phone").child("BBTestToolInfo").child("RecoverKeyTest").child("ITEMCMD").child_value();

	//m_szCoincellVolCmd = myParm_node.child("ITEMCMD").text().as_string();
	m_szCoincellVolCmd = pXMLTestItem->child("Configuration").child("Phone").child("CoincellVoltageTest").child("ITEMCMD").child_value();
	if(m_szCoincellVolCmd.empty())
	{	return false;
	}
	//std::string szTemp = pXMLTestItem->child("Configuration").child("Phone").child("CoincellVoltageTest").child("VoltageSize").text().as_string(); 
	//if (szTemp.empty())
	//{
	//	//TraceLog("FAIL to initdata for CCoincellVol");
	//	//sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to initdata for CCoincellVol");
	//	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//	return false;
	//}
	m_iCoincellVolSize = pXMLTestItem->child("Configuration").child("Phone").child("CoincellVoltageTest").child("VoltageSize").text().as_int(); 
	if (0 == m_iCoincellVolSize)
	{	return false;
	}
	//m_iCoincellVolSize = (int)strtol(szTemp.c_str(),0,0);

	m_szIdProductCmd = pXMLTestItem->child("Configuration").child("Phone").child("USBTest").child("idProductCMD").child_value();
	m_szIdVendorCmd = pXMLTestItem->child("Configuration").child("Phone").child("USBTest").child("idVendorCMD").child_value();
	if (m_szIdProductCmd.empty() || m_szIdVendorCmd.empty())
	{	return false;
	}

	m_szIdProductSize = pXMLTestItem->child("Configuration").child("Phone").child("USBTest").child("idProductSize").text().as_string();
	m_szIdVendorSize =  pXMLTestItem->child("Configuration").child("Phone").child("USBTest").child("idVendorSize").text().as_string();
	if (m_szIdProductSize.empty() || m_szIdVendorSize.empty())
	{	return false;
	}

	m_szAudioCmd = pXMLTestItem->child("Configuration").child("Phone").child("AudioTestCmd").child_value();
	if(m_szAudioCmd.empty())
	{	return false;
	}

    return true;
}

// ------------------ Initialize functions -----------------

bool CAndroidPhone::DetectDevice()
{
	
    if (m_strCOMport == "0")
    {
        std::map<std::string, std::string> map_strstrCOMDeviceName;

        if (GetCOMPortDevByUsb(map_strstrCOMDeviceName))
        {
            std::map<std::string, std::string>::iterator itr;

            if (map_strstrCOMDeviceName.empty())
            {
                CmdLog("No COM device detected");
                return false;
            }
            else
            {
                if (m_strKeyword.empty())
                {
                    if (map_strstrCOMDeviceName.size() == 1)
                    {
                        itr = map_strstrCOMDeviceName.begin();
                        std::string strDev = itr->first;

                        if (strDev.find("COM") != std::string::npos)
                            m_strCOMport = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
                        else
                            m_strCOMport = m_strDeviceName = strDev;
                    }
                    else
                    {
                        CmdLog("Fail to determine COM port number, Keyword for COM device is empty while detect multiple device");
                        return false;
                    }
                }
                else
                {
                    bool bFound = false;
                    for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
                    {
                        std::string strDev = itr->first;
                        std::string strDevName = itr->second;

                        if (strDevName.find(m_strKeyword) !=  std::string::npos)
                        {
                            bFound = true;

							if (strDev.find("HS") != std::string::npos)
                                m_strCOMport = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
                            else
                                m_strCOMport = m_strDeviceName = strDev;
                        }
                    }

                    if (! bFound)
                    {
                        CmdLog("Fail to find device by Keyword");
                        return false;
                    }
                }
            }
        }
        else
        {
            CmdLog("Fail to detect COM device with WDK");
            return false;
        }
    }
    return true;
}

bool CAndroidPhone::Initial()
{
	if (m_hQisdaModule == NULL)
	{
		//m_hQisdaModule = ::LoadLibraryA(m_strQisdaCommandModuleName.c_str());
		m_hQisdaModule =::LoadLibraryA("CmdModuleExport.dll");

		if (NULL == m_hQisdaModule)
		{
			CmdLog("m_hQisdaModule == NULL");
			CmdLog(m_strQisdaCommandModuleName);
			return false;
		}
	}

	CmdLog("Initial m_hQisdaModule Success");

    return true;
}

void CAndroidPhone::SetWorkingDir(const char* cszWorkingDir)
{
	strcpy_s(m_szWorkingDir, 512, cszWorkingDir);
}


// --------------------- FA functions ----------------------
bool CAndroidPhone::SetFAData(char* szValue)
{
	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	char szMaxWaitTime[32];
	char szRetryPeriod[32];
	memset(szMaxWaitTime, 0, 32);
	sprintf_s(szMaxWaitTime, 32, "%d", 10);
	memset(szRetryPeriod, 0, 32);
	sprintf_s(szRetryPeriod, 32, "%d", 500);

    pFA_Write FA_Write = (pFA_Write)GetProcAddress(m_hQisdaModule, "FA_Write_SPU");


    if (FA_Write)
		return FA_Write(m_strCOMport.c_str(), "0", szMaxWaitTime, szRetryPeriod, szValue);

    return false;
}

bool CAndroidPhone::GetSerialNumber(char* szValue, int iSize)
{
	if (m_bNeedToGetFA)
	{
		if (!GetFAData())
			return false;
	}

	char sz_picasso[11] = {0};
	for (int i=0; i< 10; i++)
	{
		sz_picasso[i] = m_szFaData[i+32];
	}
	sz_picasso[10] = '\0';

	memcpy_s(szValue, iSize, sz_picasso, iSize > 11 ? 11 : iSize);

	return true;
}

bool CAndroidPhone::GetTimeStamp(char* szValue, int iSize)
{
	if (m_bNeedToGetFA)
	{
		if (!GetFAData())
			return false;
	}

	char sz_TimeStamp[12] = {0};
	for (int i=0; i < 11; i++)
	{
		sz_TimeStamp[i] = m_szFaData[i+9];
	}
	sz_TimeStamp[11] = '\0';

	memcpy_s(szValue, iSize, sz_TimeStamp, iSize > 12 ? 12 : iSize);

	return true;
}

bool CAndroidPhone::GetFAData()
{
	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	char szFaData[FA_SIZE];
	memset(szFaData, 0, FA_SIZE);

	// Run command
	pFA_Read FA_Read = (pFA_Read)GetProcAddress(m_hQisdaModule, "FA_Read_SPU");

	char szMaxWaitTime[32];
	char szRetryPeriod[32];
	memset(szMaxWaitTime, 0, 32);
	sprintf_s(szMaxWaitTime, 32, "%d", 10);
	memset(szRetryPeriod, 0, 32);
	sprintf_s(szRetryPeriod, 32, "%d", 500);

	if (FA_Read)
	{
		if (FA_Read(m_strCOMport.c_str(), "0", szMaxWaitTime, szRetryPeriod, szFaData))
		{
			//Set result
			for (int i=0; i<FA_SIZE; i++)
			{
				m_szFaData[i] = ((szFaData[i]=='|')? 0:szFaData[i]);
				m_bNeedToGetFA = false;
			}

			return true;
		}
	}

	return false;
}

bool CAndroidPhone::SetIMEI(char *szvalue, int iSize)
{
	m_szIMEI = szvalue;
	return true;
}

bool CAndroidPhone::SetSN(char *szvalue, int iSize)
{	
	m_szSN = szvalue;
	return true;
}

// ------------- Qisda module phone functions --------------

bool CAndroidPhone::IsConnected()
{
	char *Buf = NULL;
	char szWorkingDir[108]={0};
	this->CmdLog("*** Call IsConnected() ***");

	//std::string chReturn = "";
	//Buf = (char*)malloc(512 * sizeof(char));
	//memset(Buf,'\0',512);
	//ExecuteCmdBlocking("adb.exe", "get-state", m_szWorkingDir, &chReturn);
	sprintf_s(szWorkingDir, sizeof(szWorkingDir)/sizeof(char), m_szAdbToolDir.c_str());
	ExecuteCmdBlockingWithTimeout("adb.exe", "get-state", szWorkingDir, &Buf, 4000);

	//CmdLog(MakeSTDstring("Call adb get-state, return: %s", chReturn));
	//CmdLog(MakeSTDstring("Call adb get-state, return: %s", **Buf));
	//if (chReturn.find("device") !=std::string::npos)
	//	return true;
	if (strstr(Buf,"device") != 0)
		return true;

	return false;
}

bool CAndroidPhone::Disconnect()
{
	ShellExecuteA(NULL, "open", "taskkill", "/F /IM adb.exe", this->m_strWorkingDir.c_str(), SW_HIDE);

	return true;
}

bool CAndroidPhone::Reboot()
{
	CmdLog("ADB_Reboot");

	ExecuteCmdBlockingWithTimeout("adb.exe", "kill-server", m_szWorkingDir, NULL);

	return ERROR_SUCCESS == ExecuteCmdBlockingWithTimeout("adb.exe", "reboot", m_szWorkingDir, NULL);
}


//正确时回传true，szWIFIMAC回转WIFI的MAC
//错误时回传false，szWIFIMAC回转错误的信息
bool CAndroidPhone::ReadWifiAddress(std::string szWorkingExe, std::string szWorkingDir, std::string *szWIFIMAC)
{		
	bool bReturn = false;
	char szCmdWorkingDir[512] = "";
	char szWorkingCmd[128] = "";

	sprintf_s(szCmdWorkingDir,512,"%s",szWorkingDir.c_str());
	sprintf_s(szWorkingCmd,128,"%s",szWorkingExe.c_str());

	char *BUF = (char*) malloc( 512 * sizeof(char));
	memset(BUF,'\0',512);
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szWorkingCmd," -r WIFIMAC",szCmdWorkingDir,&BUF,4000) == ERROR_SUCCESS)
	{
		*szWIFIMAC = BUF;
		
		int b1 = strlen(BUF);
		int a1 = szWIFIMAC->length();
		std::string::iterator new_end = remove_if(szWIFIMAC->begin(), szWIFIMAC->end(), bind2nd(equal_to <char>(), ' '));

		szWIFIMAC->erase(new_end, szWIFIMAC->end());

		int a2 = szWIFIMAC->length();
		//if(szWIFIMAC.length() == 25)
		//{	
		std::string::size_type i = szWIFIMAC->find("WIFIMAC=",0);
		if (i == 0)
		{	szWIFIMAC->erase(i,i+8);
			bReturn = true;
		}
		//}
		//else
		//{	
		//}
	}
	else
	{
		*szWIFIMAC = BUF;
	}
	return bReturn;
}

bool CAndroidPhone::WriteSn(std::string szWorkingExe, std::string szWorkingDir)
{
	bool bReturn = false;
	char szCmdWorkingDir[512] = "";
	char szWorkingCmd[128] = "";
	char szCmdLine[128]="";

	sprintf_s(szCmdWorkingDir,512,"%s",szWorkingDir.c_str());
	sprintf_s(szWorkingCmd,128,"%s",szWorkingExe.c_str());
	sprintf_s(szCmdLine,128," -w SN=%s",m_szSN.c_str());


	char *BUF = (char*) malloc( 512 * sizeof(char));
	memset(BUF,'\0',512);
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szWorkingCmd, szCmdLine, szCmdWorkingDir,&BUF,4000) == ERROR_SUCCESS)
	{
		std::string szBuf = BUF;
		
		std::string::size_type i = szBuf.find("Write successfully",0);
		if (i == 0)
		{bReturn = true;
		}
	}
	else
	{
	}
	return bReturn;
}

bool CAndroidPhone::ReadSn(std::string szWorkingExe, std::string szWorkingDir, std::string *szSN)
{
	bool bReturn = false;
	char szCmdWorkingDir[512] = "";
	char szWorkingCmd[128] = "";
	char szCmdLine[128]="";

	sprintf_s(szCmdWorkingDir,512,"%s",szWorkingDir.c_str());
	sprintf_s(szWorkingCmd,128,"%s",szWorkingExe.c_str());
	sprintf_s(szCmdLine,128," -r SN");

	char *BUF = (char*) malloc( 512 * sizeof(char));
	memset(BUF,'\0',512);
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szWorkingCmd, szCmdLine, szCmdWorkingDir, &BUF, 4000) == ERROR_SUCCESS)
	{
		*szSN = BUF;

		std::string::size_type i = szSN->find("SN=",0);
		if (i == 0)
		{	szSN->erase(i,i+3);
			i = szSN->find(m_szSN,0);
			if(i != std::string::npos )
			{	bReturn = true;
			}
		}
		//}
		//else
		//{	
		//}
	}
	else
	{
		*szSN = BUF;
	}
	return bReturn;
}

bool CAndroidPhone::SetWifiEnabled(bool bWifiOn)
{
	CmdLog("ADB_Reboot");

	char szCmdLine[128] = "";
	if(bWifiOn)
	{	sprintf_s(szCmdLine,128," shell svc wifi enable");
	}
	else
	{	sprintf_s(szCmdLine,128," shell svc wifi disable");
	}

	char *BUF = (char*) malloc( 512 * sizeof(char));
	memset(BUF,'\0',512);
	return ERROR_SUCCESS == ExecuteCmdBlockingWithTimeout("adb.exe", szCmdLine, m_szWorkingDir, &BUF,4000);
}

bool CAndroidPhone::ReadIMEI(char* szIMEI, int iSize)
{
	/* Only for test */
	/*CSingleLock singleLock(&m_Mutex_Command, TRUE);*/

	bool ret;

	pRead_IMEI ReadIMEI = (pRead_IMEI)GetProcAddress(m_hQisdaModule, "ReadIMEI_SPU");
	if (! ReadIMEI)
		return false;

	char szOutput[32];
	memset(szOutput, 0, 32);

	ret = ReadIMEI(m_strCOMport.c_str(), "0", "1", "500", szOutput);

	if(!ret)
	{
		return false;
	}
	sprintf_s(szIMEI, iSize > 32 ? 32 : iSize, "%s", szOutput);

	return true;
}

bool CAndroidPhone::CheckLabelPartNo(void)
{
	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef bool (_stdcall *lpGetLabelPartNo)(const unsigned char* InSn,      unsigned short SnLen,
											            unsigned char* OutPartNo, unsigned short PartNoLen);
		lpGetLabelPartNo iGetLabelPartNo;
		iGetLabelPartNo = (lpGetLabelPartNo)::GetProcAddress(hDll,"GetLabelPartNo");
		if (iGetLabelPartNo != NULL)
		{	unsigned char szPartNo[30] = "";
			unsigned char szSn[30] = "";
			std::string szLabelPartNo = "";
			sprintf_s((char*)szSn, 30,"%s",m_szSN.c_str());
			bool bTemp = iGetLabelPartNo(szSn,30,szPartNo,30);
			FreeLibrary(hDll);
			if(bTemp)
			{	szLabelPartNo = (char*)szPartNo;
				transform(szLabelPartNo.begin(),szLabelPartNo.end(),szLabelPartNo.begin(),toupper);
				if(szLabelPartNo == PartNo.GetPartNo())
				{	return true;
				}
				else
				{	return false;
				}
			}
			else
			{	return false;
			}
		}
		else
		{	
			FreeLibrary(hDll);
			return false;
		}
	}
	else
	{	
		return false;
	}
}

bool CAndroidPhone::IfRepeated(unsigned short IdType)
{
	unsigned char Sn[25] = "";
	if (IdType == 1)
	{	sprintf_s((char*)Sn,25,"%s",Id.GetId().c_str());
	}
	else if (IdType == 2)
	{	sprintf_s((char*)Sn,25,"%s",m_szSN.c_str());
	}
	else
	{	return true;   //默认重复
	}

	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef bool (_stdcall *lpIfRepeatedId)(const unsigned short IdType, const unsigned char* InSn,      unsigned short SnLen);
		lpIfRepeatedId iIfRepeatedId = (lpIfRepeatedId)::GetProcAddress(hDll,"IfRepeatedId");
		if (iIfRepeatedId != NULL)
		{	if(iIfRepeatedId(IdType,Sn,25))
			{	FreeLibrary(hDll);
				return true;
			}
			else
			{	FreeLibrary(hDll);
				return false;
			}
		}
		else
		{	
			FreeLibrary(hDll);
			return true;
		}
	}
	else
	{	
		return true;
	}
}

bool CAndroidPhone::GetAndReturnId(char *szvalue, int iSize)
{
	memset(szvalue,'\0',iSize);
	if(Id.ReadId())
	{	Id.GetId();
		std::string szId = Id.GetId();
		if(szId.empty() || szId.length() != 11)
		{	std::string ErrMsg = "Fail to read ID, ID = ";
			ErrMsg.append(szId);
			CmdLog(ErrMsg);	
			return false;
		}
		else
		{	std::string ErrMsg = "Pass to read ID, ID = ";
			ErrMsg.append(szId);
			CmdLog(ErrMsg);	
			sprintf_s(szvalue,iSize,"%s",szId.c_str());
			return true;
		}
	}
	else
	{	std::string ErrMsg = "Read ID fail.";
		CmdLog(ErrMsg);	
		return false;
	}

}

bool CAndroidPhone::LoadPhoneInfo(void)
{
	std::string ErrMsg = "";
	//Load 9J
	Runcard.GetRunCardById(Id.GetId());
	if(!Runcard.GetRuncard().empty())
	{	ErrMsg = "Pass to get runcard, runcard = ";
		ErrMsg.append(Runcard.GetRuncard());
		CmdLog(ErrMsg);	
	}
	else
	{	ErrMsg = "Fail to get runcard, runcard = ";
		ErrMsg.append(Runcard.GetRuncard());
		CmdLog(ErrMsg);	
		return false;
	}

	So.GetSoByRuncard(Runcard.GetRuncard());
	if( !So.GetSo().empty())
	{	ErrMsg = "Pass to get so, so = ";
		ErrMsg.append(So.GetSo());
		CmdLog(ErrMsg);
	}
	else
	{	ErrMsg = "Fail to get so, so = ";
		ErrMsg.append(So.GetSo());
		CmdLog(ErrMsg);
		return false;
	}

	PartNo.SearchPartNo(So.GetSo());
	if( !PartNo.GetPartNo().empty())
	{	ErrMsg = "Pass to get partno, partno = ";
		ErrMsg.append(PartNo.GetPartNo());
		CmdLog(ErrMsg);
	}
	else
	{	ErrMsg = "Fail to get partno, partno = ";
		ErrMsg.append(PartNo.GetPartNo());
		CmdLog(ErrMsg);
		return false;
	}

	WbcFileName.SearchWbcFileName(PartNo.GetPartNo());
	if( !WbcFileName.GetWbcFileName().empty())
	{	ErrMsg = "Pass to get partno, Wbcfilename = ";
		ErrMsg.append(WbcFileName.GetWbcFileName());
		CmdLog(ErrMsg);
	}
	else
	{	ErrMsg = "Fail to get partno, Wbcfilename = ";
		ErrMsg.append(WbcFileName.GetWbcFileName());
		CmdLog(ErrMsg);
		return false;
	}

	return true;
}

bool CAndroidPhone::IfRepeatedLabel(void)
{
	return IfRepeated(2);
}

bool CAndroidPhone::IfRepeatedId(void)
{
	return IfRepeated(1);
}

bool CAndroidPhone::PushDownFileToSDCard(std::string szPcFilePath)
{
	this->CmdLog("*** Call PushDownFileToSDCard() ***");

	//std::string chReturn = "";

	//ExecuteCmdBlocking("adb.exe", "push otainfo.txt /storage/sdcard1/", m_szWorkingDir, &chReturn);

	//CmdLog(MakeSTDstring("Call adb push otainfo.txt /storage/sdcard1/, return: %s", chReturn));

	//if (chReturn.find("KB/s") !=std::string::npos)
	//	return true;

	//return false;
	char chPcFilePath[256]={0};
	sprintf_s(chPcFilePath, 256, "push %sotainfo.txt /storage/sdcard1/otainfo.txt",szPcFilePath.c_str());
	char *Buf;
	if (ExecuteCmdBlockingWithTimeout("adb.exe",chPcFilePath,m_szWorkingDir,&Buf,4000) == ERROR_SUCCESS)
	{	std::string szBuf = Buf;
		std::string::size_type i = szBuf.find("KB/s",0);
		if (i != std::string::npos)
		{	return true;
		}
	}
	return false;

}

bool CAndroidPhone::PlayMusic(void)
{
	this->CmdLog("*** Call PlayMusic() ***");

	std::string chReturn = "";
	char *szCmd="shell am start -n \"com.android.music/.AudioPreview\" -t \"audio/ogg\" -d \"file:///system/media/audio/ringtones/Orion.ogg\"";
	ExecuteCmdBlocking("adb.exe",szCmd,m_szWorkingDir,&chReturn);
	CmdLog(MakeSTDstring("Call adb play a music, return: %s", chReturn));

	return true;
	//if (chReturn.find() != std::string::npos )
	//	return true;
	//else
	//	return false;
}

bool CAndroidPhone::pullRunInLog(std::string szLocalDir)
{
	this->CmdLog("*** Call pullRunInLog() ***");
	std::string chReturn = "";
	char szCmd[512] = "";
	sprintf_s(szCmd, 512, "pull data/data/com.oem.runin/runin_debug.log %s", szLocalDir.c_str());
	//ExecuteCmdBlocking("adb.exe",szCmd,m_szWorkingDir,&chReturn);
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, m_szWorkingDir, &Return, 4000) == ERROR_SUCCESS)
	{	std::string chReturn = Return;
		CmdLog(MakeSTDstring("Call adb pull runin_debug.log, return: %s", chReturn.c_str()));
		if (chReturn.find("KB/s") != std::string::npos)
			return true;
		else
			return false;
	}
	else
	{	CmdLog(MakeSTDstring("Call adb pull runin_debug.log fail"));
		return false;
	}

}

bool CAndroidPhone::GetRunInResult(void)
{
	this->CmdLog("*** Call GetRunInResult() ***");
	std::string chReturn = "";
	char szCmd[512] = "";
	sprintf_s(szCmd, 512, "shell sqlite3 /data/data/com.android.providers.settings/databases/settings.db \"select value from system where name='runin_result'\"");
	//ExecuteCmdBlocking("adb.exe",szCmd,m_szWorkingDir,&chReturn);
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, m_szWorkingDir, &Return, 50000) == ERROR_SUCCESS)
	{	if(Return != NULL)
		{	std::string chReturn = Return;
			CmdLog(MakeSTDstring("Call adb %s, return: %s",szCmd, chReturn.c_str()));
			if (chReturn.find("1") != std::string::npos)
			{	return true;
			}
			else
			{	return false;
			}
		}
		else
		{	CmdLog(MakeSTDstring("Call adb %s,return NULL ",szCmd));
			return false;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call adb %s fail",szCmd));
		return false;
	}
}

bool CAndroidPhone::pullCPULoadLog(char *chLocalPath)
{
	this->CmdLog("*** Call pullCPULoadLog() ***");
	char szCmd[512] = "";
	sprintf_s(szCmd, 512, "pull /data/data/com.oem.runin/CPULoadLog.txt %s", chLocalPath);
	//ExecuteCmdBlocking("adb.exe",szCmd,m_szWorkingDir,&chReturn);
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, m_szWorkingDir, &Return, 8000) == ERROR_SUCCESS)
	{	std::string chReturn = Return;
		CmdLog(MakeSTDstring("Call adb pull CPULoadLog.txt, return: %s", chReturn.c_str()));
		if (chReturn.find("KB/s") != std::string::npos)
			return true;
		else
			return false;
	}
	else
	{	CmdLog(MakeSTDstring("Call adb pull CPULoadLog.txt fail"));
		return false;
	}
}

bool CAndroidPhone::WriteMacAddress(std::string szWorkingExe, std::string szWorkingDir)
{
	char szCmdLine[64]={0};
	sprintf_s(szCmdLine, 64, " -w ETHMAC=%s", m_szMACAddr.c_str());

	char szTestExe[32]={0} ;
	sprintf_s(szTestExe, 32, "%s", szWorkingExe.c_str());

	char chWorkingDir[108]={0};
	sprintf_s(chWorkingDir, 108, "%s", szWorkingDir.c_str());

	char *BUF = NULL;
	ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szTestExe,szCmdLine,chWorkingDir,&BUF,4000);

	if(strstr(BUF,"Write successfully") != 0)
	{	//找到"Write successfully",则认为烧录成功
		CmdLog(MakeSTDstring("Call command write mac address, return: %s", BUF));
		CmdLog(MakeSTDstring("Call command write mac address pass"));
		return true;
	}
	else   // == 0表示找不到
	{	//若找不到"Write successfully",则认为失败
		CmdLog(MakeSTDstring("Call command write mac address, return: %s", BUF));
		CmdLog(MakeSTDstring("Call command write mac address fail"));
		return false;
	}
}

bool CAndroidPhone::SetPhoneMacAddress(std::string szMacAdd)
{
	m_szMACAddr = szMacAdd;
	return true;
}

std::string CAndroidPhone::GetRecoverKeyStatus(std::string szWorkingExe, std::string szWorkingDir)
{
	std::string szReturn = "";

	char chCmdLine[64]={0};
	sprintf_s(chCmdLine, 64, "%s RecoverKey_test",  szWorkingExe.c_str());
	
	char chWorkingDir[256]={0};
	sprintf_s(chWorkingDir, 256, "%s", szWorkingDir.c_str());
	
	char *BUF = NULL;
	if ( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("Command.exe",chCmdLine,chWorkingDir, &BUF,5000))
	{	if ( NULL == BUF )
		{	szReturn = "NULL";
		}
		else
		{	szReturn = BUF;
		}
	}
	else
	{	szReturn = "ERROR";
	}

	return szReturn;
}

std::string CAndroidPhone::GetRecoverKeyStatus()
{
	this->CmdLog("*** Call GetRecoverKeyStatus() ***");
	std::string szReturn = "FAIL";
	char chCmdLine[64]={0};
	//sprintf_s(chCmdLine, 64, "%s RecoverKey_test",  m_szBbTestToolName.c_str());
	sprintf_s(chCmdLine, 64, "%s %s",  m_szBbTestToolName.c_str(),m_szRecoverKey.c_str());

	char chWorkingDir[256]={0};
	sprintf_s(chWorkingDir, 256, "%s", m_szBbTestToolDir.c_str());

	CmdLog(MakeSTDstring("Call ADFU command get recoverKey %s", chWorkingDir));
	CmdLog(MakeSTDstring("Call ADFU command get recoverKey %s", chCmdLine));

	char *BUF = NULL;
	if ( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("Command.exe",chCmdLine,chWorkingDir, &BUF,5000))
	{	if ( NULL == BUF )
		{	CmdLog(MakeSTDstring("Call ADFU command get recoverKey return NULL"));
			szReturn = "NULL";
		}
		else
		{	CmdLog(MakeSTDstring("Call ADFU command get recoverKey return %s", BUF));
			szReturn = BUF;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call ADFU command get recoverKey return %s", BUF));
		szReturn = "ERROR";
	}

	return szReturn;
}

bool CAndroidPhone::PlayMovie(std::string szWorkingCmd)
{
	this->CmdLog("*** Call PlayMovie() ***");
	char szCmd[512] = "";
	sprintf_s(szCmd, 512, "%s", szWorkingCmd.c_str());
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, m_szWorkingDir, &Return, 4000))
	{	if(NULL == Return)
		{	CmdLog(MakeSTDstring("Call adb play Movie return null."));
			return false;
		}
		else
		{	std::string chReturn = Return;
			CmdLog(MakeSTDstring("Call adb play Movie, return: %s", chReturn.c_str()));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Fail to call adb PlayMovie"));
		return false;
	}
}

bool CAndroidPhone::PlayMovie()
{
	this->CmdLog("*** Call PlayMovie() ***");
	char szCmd[512] = {0};
	char szWorkingDir[512] = {0};
	sprintf_s(szCmd, 512, "%s", m_szAudioCmd.c_str());
	sprintf_s(szWorkingDir, 512, "%s", m_szAdbToolDir.c_str());
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, szWorkingDir, &Return, 4000))
	{	if(NULL == Return)
		{	CmdLog(MakeSTDstring("Call adb play Movie return null."));
			return false;
		}
		else
		{	std::string chReturn = Return;
			CmdLog(MakeSTDstring("Call adb play Movie, return: %s", chReturn.c_str()));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Fail to call adb PlayMovie"));
		return false;
	}
}

bool CAndroidPhone::LoadOs(std::string szFacImgName, std::string szFacImgDir, std::string szWorkingDir)
{
	//Set buffer to get result of CMD
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[1024]={0};

	sprintf_s(chWorkingDir, 512, szWorkingDir.c_str());
	sprintf_s(chCmdLine,1024,
		      "-fw=\"%s%s\" device=3 flash_erase=1 MISC=\"%smisc.img\" RECOVERY=\"%srecovery.img\" SYSTEM=\"%ssystem.img\" BOOT_MSG=FMT DATA=FMT CACHE=FMT",
		      szFacImgDir.c_str(),szFacImgName.c_str(),szFacImgDir.c_str(),szFacImgDir.c_str(),szFacImgDir.c_str());

	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{	CmdLog(MakeSTDstring("Call command load os fail"));
			return false;
		}
		else
		{	CmdLog(MakeSTDstring("Call command load os pass"));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call command load os fail,return NULL"));
		return false;
	}
}

bool CAndroidPhone::EraseOs(std::string szFacImgName, std::string szFacImgDir, std::string szWorkingDir)
{
	//Set buffer to get result of CMD
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[512]={0};

	sprintf_s(chWorkingDir, 512, szWorkingDir.c_str());
	sprintf_s(chCmdLine, 512,
		      "-fw=\"%s%s\" -dev=3 -mode=1 -flag=2",
		      szFacImgDir.c_str(), szFacImgName.c_str());

	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool CAndroidPhone::EraseFactoryOs()
{
	//Set buffer to get result of CMD
	this->CmdLog("*** Call EraseFactoryOs() ***");
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[512]={0};

	sprintf_s(chWorkingDir, 512, m_szPADProductToolDir.c_str());
	sprintf_s(chCmdLine, 512,
		      "-fw=\"%s%s\" -dev=3 -mode=1 -flag=2",
		      m_szFactoryImageDir.c_str(), m_szFactoryFwFile.c_str());
	CmdLog(MakeSTDstring("Call adb %s", chCmdLine));
	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{	CmdLog(MakeSTDstring("Call command load os fail"));
			return false;
		}
		else
		{	CmdLog(MakeSTDstring("Call command load os pass"));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call command load os fail,return NULL"));
		return false;
	}
}

bool CAndroidPhone::EraseUserOs()
{
	//Set buffer to get result of CMD
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[512]={0};

	sprintf_s(chWorkingDir, 512, m_szPADProductToolDir.c_str());
	sprintf_s(chCmdLine, 512,
		      "-fw=\"%s%s\" -dev=3 -mode=1 -flag=2",
		      m_szUserImageDir.c_str(), m_szUserFwFile.c_str());
	CmdLog(MakeSTDstring("Call adb %s", chCmdLine));
	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool CAndroidPhone::LoadFactoryOs()
{
	//Set buffer to get result of CMD
	this->CmdLog("*** Call LoadFactoryOs() ***");
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[1024]={0};

	sprintf_s(chWorkingDir, 512, m_szPADProductToolDir.c_str());
	sprintf_s(chCmdLine,1024,
		      "-fw=\"%s%s\" device=3 flash_erase=1 MISC=\"%smisc.img\" RECOVERY=\"%srecovery.img\" SYSTEM=\"%ssystem.img\" BOOT_MSG=FMT DATA=FMT CACHE=FMT",
		      m_szFactoryImageDir.c_str(), m_szFactoryFwFile.c_str(), m_szFactoryImageDir.c_str(), m_szFactoryImageDir.c_str(), m_szFactoryImageDir.c_str());
	CmdLog(MakeSTDstring("Call adb %s", chCmdLine));
	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{	CmdLog(MakeSTDstring("Call command load os fail"));
			return false;
		}
		else
		{	CmdLog(MakeSTDstring("Call command load os pass"));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call command load os fail,return NULL"));
		return false;
	}
}

bool CAndroidPhone::LoadUserOs()
{
	//Set buffer to get result of CMD
	this->CmdLog("*** Call LoadUserOs() ***");
	std::string Buf="";

	char chWorkingDir[512]={0};
	char chCmdLine[1024]={0};

	sprintf_s(chWorkingDir, 512, m_szPADProductToolDir.c_str());
	sprintf_s(chCmdLine,1024,
		      "-fw=\"%s%s\" device=3 flash_erase=1 MISC=\"%smisc.img\" RECOVERY=\"%srecovery.img\" SYSTEM=\"%ssystem.img\" BOOT_MSG=FMT DATA=FMT CACHE=FMT",
		      m_szUserImageDir.c_str(), m_szUserFwFile.c_str(), m_szUserImageDir.c_str(), m_szUserImageDir.c_str(), m_szUserImageDir.c_str());

	ExecCmdUtil::ExecuteCmdBlocking("Command.exe", chCmdLine, chWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{	CmdLog(MakeSTDstring("Call command load os fail"));
			return false;
		}
		else
		{	CmdLog(MakeSTDstring("Call command load os pass"));
			return true;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call command load os fail,return NULL"));
		return false;
	}
}

bool CAndroidPhone::BbTest(char* szCmdLine, char* szWorkDir, std::string *buf)
{

	if (ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlocking("Command.exe",szCmdLine,szWorkDir, buf))
	{	return true;
	}
	else
	{	return false;
	}
}

std::string CAndroidPhone::BbTest(bool enabledPMIC, bool enabledNAND, bool enabledDDRSize, bool enabledHDMI, bool enabledUart,
		                   bool enabledEthernet, bool enabledSDWriteProtected, bool enabledSDPlugin, bool enabledSDWriteRead,
				           bool enabledSDeMMCSize, bool enabledSDClockRate, bool enabledRecoverKey)
{
	this->CmdLog("*** Call BbTest() ***");
	std::string szReturn="BBTest FAIL";
	std::string Buf="";
	char szCmdLine[512] = {0};
	char szWorkDir[512] = {0};
	char *szSpace = " ";
	sprintf_s(szCmdLine,"%s ",m_szBbTestToolName.c_str());

	if (true == enabledPMIC)
	{	strcat_s(szCmdLine, 512, m_szPMIC.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledNAND)
	{	strcat_s(szCmdLine, 512, m_szNAND.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledDDRSize)
	{	strcat_s(szCmdLine, 512, m_szDDR.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledHDMI)
	{	strcat_s(szCmdLine, 512, m_szHDMI.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledUart)
	{	strcat_s(szCmdLine, 512, m_szUart.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledEthernet)
	{	strcat_s(szCmdLine, 512, m_szEthernet.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledSDWriteProtected)
	{	strcat_s(szCmdLine, 512, m_szSDWriteProtected.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledSDPlugin)
	{	strcat_s(szCmdLine, 512, m_szSDPlugin.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledSDWriteRead)
	{	strcat_s(szCmdLine, 512, m_szSDWriteRead.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledSDeMMCSize)
	{	strcat_s(szCmdLine, 512, m_szSDeMMC.c_str());
		strcat_s(szCmdLine, 512, szSpace);
	}
	if (true == enabledSDClockRate)
	{	strcat_s(szCmdLine, 512, m_szSDClockRate.c_str());
	}
	if (true == enabledRecoverKey)
	{	strcat_s(szCmdLine, 512, m_szRecoverKey.c_str());
	}

	sprintf_s(szWorkDir, 512, m_szBbTestToolDir.c_str());
	CmdLog(MakeSTDstring("Call ADFU command %s", szWorkDir));
	CmdLog(MakeSTDstring("Call ADFU command %s", szCmdLine));
	if (ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlocking("Command.exe", szCmdLine, szWorkDir, &Buf))
	{	if (!Buf.empty())
		{	std::string::size_type i = 0;
			//找到第一个"[",并将其删除掉
			std::string::size_type j = Buf.find("[",i);
			if ( j != 0 )
			{	CmdLog(MakeSTDstring("Call ADFU command return %s", Buf.c_str()));
				return szReturn;
			}
			Buf.erase(j,j + 1);
			std::string Temp = "";
			if (enabledPMIC)
			{	//找到m_szPMIC
				i = Buf.find(m_szPMIC);
				if (i!=string::npos )
				{	//找到m_szPMIC之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "PMU Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledNAND)
			{	//找到m_szNAND
				i = Buf.find(m_szNAND);
				if (i!=string::npos )
				{	//找到m_szNAND之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "Nand Fail";
							return szReturn;
						}
						else
						{	i = Temp.find(m_szNandSize);
							if( i == string::npos )
							{	szReturn = "Nand size Fail";
								return szReturn;
							}
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledDDRSize)
			{	//找到m_szNAND
				i = Buf.find(m_szDDR);
				if (i!=string::npos )
				{	//找到m_szNAND之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "DDR Fail";
							return szReturn;
						}
						else
						{	i = Temp.find(m_szDDRSize);
							if(i == string::npos )
							{	szReturn = "DDR size Fail";
								return szReturn;
							}
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledHDMI)
			{	//找到m_szHDMI
				i = Buf.find(m_szHDMI);
				if (i!=string::npos )
				{	//找到m_szHDMI之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "HDMI Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledUart)
			{
				//找到m_szUart
				i = Buf.find(m_szUart);
				if (i!=string::npos )
				{
					//找到m_szUart之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{
						Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "Uart Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledEthernet)
			{	//找到m_szEthernet
				i = Buf.find(m_szEthernet);
				if (i!=string::npos )
				{	//找到m_szEthernet之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "Ethernet Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if(enabledSDWriteProtected)
			{
				//找到m_szSDWriteProtected
				i = Buf.find(m_szSDWriteProtected);
				if (i!=string::npos )
				{	//找到m_szSDWriteProtected之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "SD Write Fail";
							return szReturn;
						}
						else
						{	i = Temp.find(m_szSDProtected);
							if(i == string::npos )
							{	szReturn = "SD Write protected Fail";
								return szReturn;
							}
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if (enabledSDPlugin)
			{	//找到m_szSDPlugin
				i = Buf.find(m_szSDPlugin);
				if ( i!=string::npos )
				{	//找到m_szSDPlugin之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{
						Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "SD plugin Fail";
							return szReturn;
						}
						else
						{	i = Temp.find(m_szSDType);
							if(i == string::npos )
							{	szReturn = "SD plugin type Fail";
								return szReturn;
							}
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if(enabledSDWriteRead)
			{
				//找到m_szSDWriteRead
				i = Buf.find(m_szSDWriteRead);
				if ( i!=string::npos )
				{	//找到m_szSDWriteRead之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{
						Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "SD WriteRead Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if(enabledSDeMMCSize)
			{	//找到m_szSDeMMC
				i = Buf.find(m_szSDeMMC);
				if ( i!=string::npos )
				{	//找到m_szSDeMMC之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "SD eMMC Fail";
							return szReturn;
						}
						else
						{	i = Temp.find(m_szSDeMMCSize);
							if(i == string::npos )
							{	szReturn = "SD eMMC size Fail";
								return szReturn;
							}
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}

			if(enabledSDClockRate)
			{	//找到m_szSDClockRate
				i = Buf.find(m_szSDClockRate);
				if ( i!=string::npos )
				{	//找到m_szSDClockRate之后的最近的一个"["
					j = Buf.find("[",i);
					if(j == string::npos )
						j=Buf.length();
					//提取区间的字符串
					if (j >= i)
					{	Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
						Buf.erase(i,j);    //删除掉已取出的字符串
						i = Temp.find("successful");
						if(i == string::npos)    //找不到"successful"，认为测试失败
						{	szReturn = "SD clockrate Fail";
							return szReturn;
						}
					}
					else
					{	return szReturn;
					}
				}
				else
				{	return szReturn;
				}
			}
			CmdLog(MakeSTDstring("Call ADFU command return pass"));
			szReturn = "PASS";
			return szReturn;
		}
		else
		{	CmdLog(MakeSTDstring("Call ADFU command return %s", Buf.c_str()));
			CmdLog(MakeSTDstring("Call ADFU command return null"));
			return szReturn;
		}
	}
	else
	{	CmdLog(MakeSTDstring("Call ADFU command return %s", Buf.c_str()));
		CmdLog(MakeSTDstring("Call ADFU command fail"));
		return szReturn;
	}
}

std::string CAndroidPhone::CheckIdProduct()
{
	std::string chReturn = "";
	this->CmdLog("*** Call CheckIdProduct() ***");
	char szCmd[512] = {0};
	char szWorkingDir[512] = {0};
	sprintf_s(szCmd, 512, "%s", m_szIdProductCmd.c_str());
	sprintf_s(szWorkingDir, 512, "%s", m_szAdbToolDir.c_str());
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, szWorkingDir, &Return, 4000))
	{	if(NULL == Return)
		{	CmdLog(MakeSTDstring("Call adb get IdProduct return null."));
			chReturn = "NULL";
			return chReturn;
		}
		else
		{	chReturn = Return;
			std::string::size_type i = chReturn.find(m_szIdProductSize);
			if(  std::string::npos == i )
			{	CmdLog(MakeSTDstring("Phone IdProduct error, return: %s", chReturn.c_str()));
				return chReturn;
			}
			else
			{	CmdLog(MakeSTDstring("Phone IdProduct pass, return: %s", chReturn.c_str()));
				chReturn = "PASS";
				return chReturn;
			}
		}
	}
	else
	{	CmdLog(MakeSTDstring("Fail to call adb get IdProduct"));
		chReturn = "ERROR";
		return chReturn;
	}
}

std::string CAndroidPhone::CheckIdVendor()
{
	std::string chReturn = "";
	this->CmdLog("*** Call CheckIdVendor() ***");
	char szCmd[512] = {0};
	char szWorkingDir[512] = {0};
	sprintf_s(szCmd, 512, "%s", m_szIdVendorCmd.c_str());
	sprintf_s(szWorkingDir, 512, "%s", m_szAdbToolDir.c_str());
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
	if( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, szWorkingDir, &Return, 4000))
	{	if(NULL == Return)
		{	CmdLog(MakeSTDstring("Call adb get IdVendor return null."));
			chReturn = "NULL";
			return chReturn;
		}
		else
		{	chReturn = Return;
			std::string::size_type i = chReturn.find(m_szIdVendorSize);
			if(  std::string::npos == i )
			{	CmdLog(MakeSTDstring("Phone IdVendor error, return: %s", chReturn.c_str()));
				return chReturn;
			}
			else
			{	CmdLog(MakeSTDstring("Phone IdVendor pass, return: %s", chReturn.c_str()));
				chReturn = "PASS";
				return chReturn;
			}
		}
	}
	else
	{	CmdLog(MakeSTDstring("Fail to call adb get IdVendor"));
		chReturn = "ERROR";
		return chReturn;
	}
}

std::string CAndroidPhone::CheckCoincellVol()
{
	std::string chReturn = "";
	this->CmdLog("*** Call CheckCoincellVol() ***");
	char szWorkingDir[512]={0};
	char szCmd[512] = {0};
	sprintf_s(szWorkingDir, 512, "%s", m_szAdbToolDir.c_str());
	//sprintf_s(szCmd, 512, "shell cat /sys/class/hwmon/hwmon1/device/backupbat_voltage");
	sprintf_s(szCmd, 512, "%s", m_szCoincellVolCmd.c_str());
	CmdLog(MakeSTDstring("Call adb %s", szCmd));
	char *Return = NULL;
    if( ERROR_SUCCESS == ExecCmdUtil::ExecuteCmdBlockingWithTimeout("adb.exe", szCmd, szWorkingDir, &Return, 4000))
	{	if (NULL == Return)
		{	CmdLog(MakeSTDstring("Call adb get coincell voltage return null."));
			chReturn = "NULL";
			return chReturn;
		}
		else
		{	chReturn = Return;
			int iVol = (int)strtol(chReturn.c_str(),0,0);
			char szVol[50]="";
			char szLower[50] = "";
			sprintf_s(szVol, 50, "%.2f", iVol * 5.86);
			sprintf_s(szLower, 50, "%.2f", m_iCoincellVolSize * 5.86);
			if ( iVol >= m_iCoincellVolSize )
			{	CmdLog(MakeSTDstring("Lower: %s mV,CoincellVoltage MeasureResult: %s mV, CoincellVoltage PASS", szLower, szVol));
				chReturn = "PASS";
				return chReturn;
			}
			else
			{	CmdLog(MakeSTDstring("Lower: %s mV,CoincellVoltage MeasureResult: %s mV, CoincellVoltage Fail", szLower, szVol));
				return chReturn;
			}
		}
	}
	else
	{	CmdLog(MakeSTDstring("Fail to call adb get IdVendor"));
		chReturn = "ERROR";
        return chReturn;
	}
}
//bool CAndroidPhone::ReadId(char* szId,int iIdLen)
//{
//	char szBuf[20]="";
//	memset(szId,'\0',iIdLen);
//	HINSTANCE hDll;
//	hDll = LoadLibrary(_T("I2C.dll"));
//	if (hDll != NULL )
//	{
//		typedef void(_stdcall *lpReadId)(char*,int);
//		lpReadId ReadId;
//		ReadId =(lpReadId)::GetProcAddress(hDll,"ReadId");
//		if( ReadId != NULL )
//		{
//			ReadId(szBuf,20);
//			FreeLibrary(hDll);
//		}
//		else
//		{
//		}
//	}
//	if(iIdLen >(int)strlen(szBuf))
//	{	strcpy_s(szId,iIdLen,szBuf);}
//	return true;
//}

//bool CAndroidPhone::GetRunCardById(unsigned char *szId, unsigned short IdLen, unsigned char *szRuncard, unsigned short RuncardLen)
//{
//	unsigned char szBuf[30] = "";
//	memset(szRuncard,'\0',RuncardLen);
//	HINSTANCE hDll;
//	hDll = LoadLibrary(_T("f32server2.dll"));
//	if(hDll != NULL)
//	{	typedef void(_stdcall *lpGetRuncardById)(unsigned char*,unsigned short,unsigned char*,unsigned short);
//		lpGetRuncardById GetRuncardById;
//		GetRuncardById = (lpGetRuncardById)::GetProcAddress(hDll,"GetRunCardById");
//		if (GetRuncardById != NULL)
//		{	GetRuncardById(szId,IdLen,szBuf,30);
//			if(RuncardLen >strlen((char*)szBuf))
//			{	strcpy_s((char*)szRuncard,RuncardLen,(char*)szBuf);
//				return true;
//			}
//		}
//		else
//			return false;
//	}
//	else
//		return false;
//
//}

#pragma warning (pop)