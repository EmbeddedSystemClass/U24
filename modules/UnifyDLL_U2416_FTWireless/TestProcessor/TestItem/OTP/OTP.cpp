/*********************************************************************************
* COPYRIGHT Qisda WGC18 2011
*
* Description: .cpp file of CTouchPanelProc object.
* FileName   : CTouchPanelProc.cpp
* Version    : $Revision:   1.4  $
* Author     : Gilpin.Xi 
* Time       : 2011/04/18
* History    :
*  NO      Date        Updated by        Content of change 
*  1    2011/04/18      Gilpin.Xi             Create.
********************************************************************************/

#include "stdafx.h"
#include "OTP.h"


#define MAXTIMEOUTS		2000	// timeout(ms)
#define EVENTCHAR       0x0d	// event char
#define XON			    0x11	// DCB XonChar
#define XOFF			0x13	// DCB XoffChar

RF_IMPLEMENT_DYNCREATE(OTP)
OTP* pOTP;

bool OTP::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}


bool OTP::Run()
{
	CStdioFile	fReadCSV;
	CString csLine, strSearchTarget, csMsg;
	CString csIMEI, csBT, csWIFI;
	OTP::InitData();

	m_strIMEI = g_strIMEI.c_str();

	/*read CSV FILE*/
	bool isFound = false ;
	m_strINICSVFilename = "w5.csv";
	if (fReadCSV.Open(m_strINICSVFilename , CFile::modeRead | CFile::shareDenyNone, NULL))
	{
		while (fReadCSV.ReadString(csLine))
		{
			AfxExtractSubString(strSearchTarget, csLine, _MAPPING_ITEM_IMEI , _T(',')) ;
			strSearchTarget.Trim();

			if (0 == strSearchTarget.CompareNoCase(m_strIMEI))
			{
				isFound = true ;
				break ;
			}
		}
	}
	fReadCSV.Close() ;
	/*read CSV FILE*/

	if (!isFound)
	{
		m_strMessage  = "IEMI is Not in Mapping Table...";
		FactoryLog();
		return false ;
	}

	// Copy Data
	csMsg.Format(_T("IMEI: %s is found."), m_strIMEI) ;

	// Get BT Address
	AfxExtractSubString(m_strBTAdd, csLine, _MAPPING_ITEM_BT , _T(',')) ;
	m_strBTAdd.MakeLower();
	csMsg.Format(_T("Get BT MAC Address: %s"), m_strBTAdd) ;

	// Get WIFI Address
	AfxExtractSubString(m_strWIFIAdd, csLine, _MAPPING_ITEM_WIFI , _T(',')) ;
	m_strWIFIAdd.MakeLower();
	csMsg.Format(_T("Get WIFI MAC Address: %s"), m_strWIFIAdd) ;

	csIMEI =  _T("IMEI,") + m_strIMEI;
	csBT = _T("BT,") + m_strBTAdd;
	csWIFI = _T("Wifi,") + m_strWIFIAdd;

	char charIMEIValue[30] = {0};
	char charBTAddValue[20] = {0};
	char charWIFIAddValue[20] = {0};
	strcpy(charIMEIValue, m_strIMEI.GetString());
	strcpy(charBTAddValue, m_strBTAdd.GetString());
	strcpy(charWIFIAddValue, m_strWIFIAdd.GetString());

	char charIMEIString[30] = {0};
	char charBTAddString[20] = {0};
	char charWIFIAddString[20] = {0};
	strcpy(charIMEIString, csIMEI.GetString());
	strcpy(charBTAddString, csBT.GetString());
	strcpy(charWIFIAddString, csWIFI.GetString());

	if (!FTD_WirteIMEI(charIMEIString))return false;
	if (!FTD_ReadIMEI(_T("IMEI"), charIMEIValue)) return false;

	if (!FTD_WirteBT(charBTAddString)) return false;
	if (!FTD_ReadBT(_T("BT"), charBTAddValue)) return false;

	if (!FTD_WirteWIFI(charWIFIAddString)) return false;
	if (!FTD_ReadWIFI(_T("Wifi"), charWIFIAddValue)) return false;

	return true;
}

bool OTP::PostRun()
{
	return true;
}

bool OTP::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name_IMEI")) != paramMap.end())
	{
		m_str_Name_IMEI = paramMap[std::string("Name_IMEI")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'OTP Name_IMEI'"));
		return false;
	}

	if (paramMap.find(std::string("Name_HaierSN")) != paramMap.end())
	{
		m_str_Name_HaierSN = paramMap[std::string("Name_HaierSN")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'OTP Name_HaierSN'"));
		return false;
	}

	if (paramMap.find(std::string("Name_BT")) != paramMap.end())
	{
		m_str_Name_BT = paramMap[std::string("Name_BT")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'OTP Name_BT'"));
		return false;
	}

	if (paramMap.find(std::string("Name_WIFI")) != paramMap.end())
	{
		m_str_Name_WIFI = paramMap[std::string("Name_WIFI")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'OTP Name_WIFI'"));
		return false;
	}

	/* Value */
	if (paramMap.find(std::string("Value")) != paramMap.end())
	{
		m_str_Value = paramMap[std::string("Value")];
		m_rp_Value.SetRange(m_str_Value);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'CheckMatch' for class 'OTPProc'"));
		return false;
	}
	return true;
}

bool OTP::InitData()
{
	return true;
}

OTP::OTP(): nWaitResultTime(5000)
{
	m_bStopViewImg = false;
}

void OTP::Report(bool bRes, string strErrorCode, string strMessage)
{
	if (bRes)
	{
		m_strResult = "PASS";
		m_strErrorCode = strErrorCode;
		m_strMessage = strMessage;
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = strErrorCode;
		m_strMessage = strMessage;
		TraceLog(MSG_ERROR, m_strMessage);
	}
	FactoryLog();
}

UINT OTP::AfxViewImgDlgThread(LPVOID pParam)
{
	return true;
}

OTP::~OTP()
{
}

bool OTP::FTD_WirteIMEI(char* szParam)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Write_IMEI_Fail;
		TraceLog(MSG_ERROR, "FTD_WirteIMEI FAIL");
		m_strMessage = "FTD_WirteIMEI FAIL";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_Wirte: ";
	str_msg += sz_input;
	str_msg += " PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_WirteHaierSN(char* szParam)
{
	return true; //*not finish , need to modify Neptune.dll*/

	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Write_IMEI_Fail;
		TraceLog(MSG_ERROR, "FTD_WirteHaierSN FAIL");
		m_strMessage = "FTD_WirteHaierSN FAIL";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_Wirte: ";
	str_msg += sz_input;
	str_msg += " PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_WirteBT(char* szParam)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Write_BT_Address_Fail;
		TraceLog(MSG_ERROR, "FTD_WirteBT FAIL");
		m_strMessage = "FTD_WirteBT FAIL";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_Wirte: ";
	str_msg += sz_input;
	str_msg += " PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_WirteWIFI(char* szParam)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Write_WLAN_Address_Fail;
		TraceLog(MSG_ERROR, "FTD_WirteWIFI FAIL");
		m_strMessage = "FTD_WirteWIFI FAIL";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_Wirte: ";
	str_msg += sz_input;
	str_msg += " PASS";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_WirteSERIALNO(char* szParam)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		TraceLog(MSG_ERROR, "FTD_WirteSERIALNO FAIL");
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_WirteSERIALNO PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_WirteQLOCK(char* szParam)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_output[FTD_BUF_SIZE] = {0};

	strcpy(sz_input, szParam);
	if (!m_pIPhone->FTD_FAC_OTPWrite(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		TraceLog(MSG_ERROR, "FTD_WirteQLOCK FAIL");
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_WirteQLOCK PASS";
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

//---- READ FUNCTION  ----
bool OTP::FTD_ReadIMEI(char* szParam, char* szData)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Read_IMEI_Fail;
		TraceLog(MSG_ERROR, "FTD_ReadIMEI FAIL ");
		m_strMessage = "FTD_ReadIMEI FAIL ";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	if (strcmp(szData, sz_output) == 0)
	{
		str_msg = "FTD_ReadIMEI  : ";
		str_msg += sz_output;
	}
	else
	{
		str_msg = "FTD_ReadIMEI  : write[";// non match ";
		str_msg = str_msg + szData + "] read:[" + sz_output + "]";
		m_strMessage = str_msg;
		FactoryLog();
		return false;
	}
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_ReadBT(char* szParam, char* szData)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut , sz_input, sz_output))
	{
		m_strErrorCode = Read_BT_Address_Fail;
		TraceLog(MSG_ERROR, "FTD_ReadBT FAIL ");
		m_strMessage = "FTD_ReadBT FAIL";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	if (strcmp(szData, sz_output) == 0)
	{
		str_msg = "FTD_ReadBT  : ";
		str_msg += sz_output;
	}
	else
	{
		str_msg = "FTD_ReadBT  : write[";// non match ";
		str_msg = str_msg + szData + "] read:[" + sz_output + "]";
		m_strMessage = str_msg;
		FactoryLog();
		return false;
	}
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_ReadWIFI(char* szParam, char* szData)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		m_strErrorCode = Read_WLAN_Address_Fail;
		TraceLog(MSG_ERROR, "FTD_ReadWIFI FAIL ");
		m_strMessage = "FTD_ReadWIFI FAIL  ";
		FactoryLog();
		return false;
	}
	std::string str_msg;
	if (strcmp(szData, sz_output) == 0)
	{
		str_msg = "FTD_ReadWIFI  : ";
		str_msg += sz_output;
	}
	else
	{
		str_msg = "FTD_ReadWIFI  : write[";// non match ";
		str_msg = str_msg + szData + "] read:[" + sz_output + "]";
		m_strMessage = str_msg;
		FactoryLog();
		return false;
	}
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_ReadSERIALNO(char* szParam, char* szData)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		TraceLog(MSG_ERROR, "FTD_ReadSERIALNO FAIL ");
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_ReadSERIALNO  : ";
	str_msg += sz_output;
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool OTP::FTD_ReadQLOCK(char* szParam, char* szData)
{
	char sz_input[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, szParam);
	char sz_output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_output))
	{
		TraceLog(MSG_ERROR, "FTD_ReadQLOCK FAIL ");
		return false;
	}
	std::string str_msg;
	str_msg = "FTD_ReadQLOCK  : ";
	str_msg += sz_output;
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}
