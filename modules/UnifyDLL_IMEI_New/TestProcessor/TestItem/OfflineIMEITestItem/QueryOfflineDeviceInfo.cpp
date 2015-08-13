#include "stdafx.h"
#include "QueryOfflineDeviceInfo.h"

RF_IMPLEMENT_DYNCREATE(CQueryOfflineDeviceInfo)

BYTE CQueryOfflineDeviceInfo::DEFAULT_PRIVATE_EXPONENT[] = {
    0x49, 0x98, 0xC3, 0xD0, 0xCA, 0x34, 0xC9, 0x7E, 0x14,
    0x8B, 0xCE, 0x73, 0xEF, 0x22, 0x93, 0xA7, 0x03, 0x33,
    0xDC, 0xD3, 0xB3, 0x2E, 0x5F, 0x51, 0x19, 0x21, 0x8D,
    0x45, 0x24, 0xCB, 0xE4, 0xA4, 0x8D, 0x27, 0x57, 0x02,
    0xD2, 0xC1, 0xC2, 0xCD, 0x46, 0xC9, 0xB5, 0x3D, 0x4F,
    0x3F, 0x5A, 0xDB, 0x4F, 0x33, 0x0B, 0x66, 0x50, 0x0A,
    0x41, 0x27, 0x9C, 0xEC, 0x74, 0x05, 0x0D, 0xD2, 0xE2,
    0x6F, 0x8B, 0x1E, 0x5E, 0xD9, 0xE0, 0xAC, 0x58, 0x5D,
    0xB9, 0xCA, 0xD8, 0xAA, 0xA5, 0xEA, 0x50, 0x4D, 0xAF,
    0xA6, 0x8F, 0x36, 0xC8, 0x53, 0xA9, 0xF1, 0xF8, 0x0A,
    0xB7, 0xCF, 0xA2, 0x75, 0x9A, 0x3E, 0x79, 0xFB, 0xE1,
    0x29, 0x3B, 0xDC, 0x79, 0x4C, 0x01, 0x4A, 0x90, 0x08,
    0xED, 0xCA, 0x15, 0xD0, 0x2B, 0x14, 0x8A, 0x3E, 0x80,
    0xB2, 0x90, 0xCF, 0x36, 0x56, 0x31, 0xBA, 0x22, 0xFE,
    0x35, 0x79 };

BYTE CQueryOfflineDeviceInfo::DEFAULT_MODULUS[] = {
    0xE0, 0xBA, 0x09, 0xC6, 0x76, 0x44, 0xA0, 0x94, 0x11,
    0x88, 0x36, 0x4A, 0x78, 0x6D, 0x19, 0x2C, 0x8F, 0x36,
    0x48, 0x4C, 0x9F, 0x54, 0x15, 0x39, 0x82, 0x01, 0x6F,
    0xA3, 0x46, 0xC1, 0x1F, 0x99, 0xEC, 0x08, 0x35, 0x45,
    0x2A, 0x31, 0xCD, 0xD2, 0x49, 0xF6, 0x56, 0xCB, 0xF1,
    0x37, 0xC3, 0x27, 0x5F, 0x94, 0x4D, 0x42, 0x3A, 0x0B,
    0xC6, 0x57, 0x43, 0x36, 0x57, 0x4D, 0xAE, 0x95, 0x06,
    0x7D, 0x32, 0xEE, 0x96, 0xC2, 0xE8, 0xBB, 0x04, 0x74,
    0x71, 0xA4, 0x0A, 0x85, 0x3E, 0x8D, 0xF2, 0x5D, 0x16,
    0x14, 0x05, 0x96, 0x4F, 0x33, 0x62, 0x98, 0xBE, 0x06,
    0xC6, 0xEB, 0xBD, 0x26, 0x97, 0x3C, 0xB3, 0x31, 0x40,
    0x26, 0xB3, 0xDE, 0xAE, 0x53, 0x47, 0xE6, 0xA4, 0x02,
    0x4C, 0xD8, 0x67, 0xD7, 0xB7, 0x08, 0x72, 0x1B, 0xBE,
    0xC6, 0x46, 0x61, 0xA3, 0xCC, 0xC9, 0xC7, 0xC0, 0xB0,
    0x16, 0xCB };

BYTE CQueryOfflineDeviceInfo::DEFAULT_PUBLIC_EXPONENT[] = {
    0x01, 0x00, 0x01 };

bool CQueryOfflineDeviceInfo::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CQueryOfflineDeviceInfo::Run()
{
	bool bRes = MainFunction();

	return bRes;
}

bool CQueryOfflineDeviceInfo::PostRun()
{
	return true;
}

bool CQueryOfflineDeviceInfo::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CQueryOfflineDeviceInfo' default Retry=3"));
		m_iRetry = 3;
	}
	
	if (paramMap.find(std::string("CSVFile")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'CSVFile' for class 'CQueryOfflineDeviceInfo."));
		return false;
	}
	m_strCSVPath = paramMap[std::string("CSVFile")];

	return true;
}

bool CQueryOfflineDeviceInfo::MainFunction()
{
	TraceLog(MSG_INFO, "Query Offline Device Info START");

	char szSIMFilePath[1024] = {0};
	char szPublicKey[2048] = {0};
	char szSIMData[2048] = {0};
	strcpy(m_szIMEI, g_strIMEI.c_str());

	if( !SeekCSVFileParam(m_szIMEI)) return false;

	strcpy(m_szFSN, m_mapQueryData["FSN"].c_str());
	strcpy(szSIMFilePath, m_mapQueryData["SIMFileName"].c_str());

	if( !GetSIMData(szSIMFilePath, szPublicKey, szSIMData))
		return false;

	m_mapQueryData["SIMData"] = szSIMData;
	m_mapQueryData["SIMPublicKey"] = szPublicKey;

	m_strMessage = "Query Offline Device Info success";
	m_strResult = "PASS";
	FactoryLog();
	return true;
}
void CQueryOfflineDeviceInfo::GetFullPath(char* chPath, char* chFullPath)
{
    if ( !::PathIsRelative(chPath) )
	{
        _tcscpy_s(chFullPath, MAX_PATH, chPath) ;
        return;
    }

    char chTmp[MAX_PATH] ;
    ::GetModuleFileName(NULL, chTmp, MAX_PATH) ;
    ::PathRemoveFileSpec(chTmp) ;
    ::PathAddBackslash(chTmp) ;
    ::PathAppend(chTmp, chPath) ;

    _tcscpy_s(chFullPath, MAX_PATH, chTmp) ;
}

bool CQueryOfflineDeviceInfo::SeekCSVFileParam(char* szIMEI)
{
	char szTmp[MAX_PATH] = {0} ;
    sprintf_s(szTmp, MAX_PATH, _T("Qisda\\%s"), m_strCSVPath.c_str() );
    GetFullPath(szTmp, szTmp);
    if ( !::PathFileExists(szTmp) )
	{
		m_strMessage = "CSV File must be in \\Qisda folder, but it is not Exist.";
		FactoryLog();
        return false ;
    }
	CString CStrCSVFullPath;
	CStrCSVFullPath.Format(_T("%s"), szTmp);

	CStdioFile fReadCSV;
	CString CStrTemp, csLine;
	bool isFound = false;
    if ( fReadCSV.Open(CStrCSVFullPath, CFile::modeRead | CFile::shareDenyNone, NULL) )
    {
        while (fReadCSV.ReadString(csLine))
        {
            AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_IMEI , _T(',') ) ;

            if (CStrTemp.CompareNoCase(szIMEI) == 0 )
			{
				TraceLog(MSG_INFO, MakeStr("Get CSV param IMEI: %s", CStrTemp));
                isFound = true;
                break;
            }
        }
    }
    fReadCSV.Close();

    if (!isFound)
	{
		m_strMessage = "IEMI is Not in Mapping Table";
		FactoryLog();
        return false;
    }

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_BT , _T(','));
	m_mapQueryData["BT_ADDR"] = CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param BT MAC Address: %s", CStrTemp));

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_WIFI , _T(','));
	m_mapQueryData["MAC_ADDR"] = CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param WIFI Address: %s", CStrTemp));

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_PVKID , _T(','));
	m_mapQueryData["MSN"] = CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param MSN: %s", CStrTemp));

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_FSN , _T(','));
	m_mapQueryData["FSN"] = CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param FSN: %s", CStrTemp));

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_NWCODE , _T(','));
	m_mapQueryData["Network_Code"] = CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param Network_Code: %s", CStrTemp));

	AfxExtractSubString(CStrTemp, csLine, _MAPPING_ITEM_LOC , _T(','));
	m_mapQueryData["SIMFileName"] = ".\\Qisda\\" + CStrTemp;
	TraceLog(MSG_INFO, MakeStr("Get CSV param SIM File Name: %s", CStrTemp));

    sprintf_s(szTmp, MAX_PATH, _T("Qisda\\%s"), CStrTemp );
    GetFullPath(szTmp, szTmp);
    if ( !::PathFileExists(szTmp) )
	{
		m_strMessage = "SIM File must be in \\Qisda folder, but it is not Exist.";
		FactoryLog();
        return false ;
    }
	return true;
}
bool CQueryOfflineDeviceInfo::GetSIMData(char* szSIMFilePath, char* szPublicKey, char* szSIMData)
{
	char szErrMsg[FTD_BUF_SIZE] = {0};
	CString cstrTmp;

	int simType = 10;

	if ( !m_pSIMEncoder->Initial_SIMEncoder(szErrMsg))
	{
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_AllocSimEncObj(simType, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_EnableTextLog(true, "D:\\Log\\SimEncoder.log", szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_SetImei(m_szIMEI, strlen(m_szIMEI), szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_SetFlashSerialNumber(m_szFSN, strlen(m_szFSN), szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_SetSimLockFile(szSIMFilePath, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	char szNetworkCode[128] = {0};
	strcpy(szNetworkCode, m_mapQueryData["Network_Code"].c_str());

	T_SIM_UNLOCK_CODE tSimUnlockCode;
    tSimUnlockCode.szNetworkCode			= szNetworkCode;
    tSimUnlockCode.szServiceProviderCode	= szNetworkCode;
    tSimUnlockCode.szCorporateCode			= szNetworkCode;
    tSimUnlockCode.szNetworkSubsetCode		= szNetworkCode;
    tSimUnlockCode.szSimCode				= szNetworkCode;
    tSimUnlockCode.szUsimCode			    = szNetworkCode;
    tSimUnlockCode.szReserved				= NULL;

	if ( !m_pSIMEncoder->ISimEnc_SetUnlockCodes(&tSimUnlockCode, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	char szRSAKey[2048] = {0};
	strcpy(szRSAKey, "<RSAKeyValue><Modulus>qXbknqK5IfzOJLqpxHmmVjCPFa/cqWbR53yaF+cgNQz+g4uXF7VaeTItGPcRY+epjN7PcUqnMgRRS2dR+KvLmMFh2T2HhCI+QiuDCKL8hUlBWJBBb3RAITYp3Mutuba5z2nujmvMa0G5Mx/A+0F0ZEEpAVcOmEiRIJcQDJQ4mU8=</Modulus><Exponent>AQAB</Exponent><P>4oxY8AQKmLX68PUuUxoopqZcs+M6085cJ5itG+Jg45aLeAXcXkEGk+grWAyhFZL18lIx8gag4eDRVzd6uiVsSQ==</P><Q>v37Ds4mEWBuY6f6aYV+6fwMZaRwamB9peDr0Axamm+uQd2OoDolmYSGgH+dLmVY0EAjGdKsTCI9lUoNIwmVo1w==</Q><DP>Q2IlTfaVxEhUhHkYt0UPlEnlRuguZjN5+5kQ3DtgQDFV+HQd0Lccor+qrLzM6OLWXqYODwPjtGuvOHba/SUGaQ==</DP><DQ>fkcIpI8tj7B7MOrtQQMQh4syqYaX3vLv6RHvMU4xhiZb+O7ltthShADZh9BNoQsgf+ZZgDwD7CQra7MZOuAGtw==</DQ><InverseQ>dm+cYUqN44GflkGLblOWVp0qEreYPAXrn1g7RHQ6zGPj7Y2xKO5NbAijgbat1fW1tjCBZJzzOM7QPgkZZDeB9g==</InverseQ><D>fxn/SsP9WvKDoqSmJAihq09sYfWu6ReHI7oe0QK281M1qXAO+Oc6XmA11t2oG2hrkI6kw/IcCLWYS5fIsVwYrNYCdh5+jVpuiyc86dR4Aif0CRItZ90J+Et/je5w8KHdky23jtHGqvbVMEd40w8ygZMNcULmSRoYUZuCNfoeiGE=</D></RSAKeyValue>");
	if ( !LoadRsaKey(simType, szRSAKey, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		if(strcmp(szErrMsg, "") != 0)	m_strMessage = szErrMsg;
		else							m_strMessage = "Load RSA Key for SIMLock Fail";
		FactoryLog();
		return false;
	}

	// Get SIM Data //
	int nSimLockData = 2048;
	char pcSimLockData[2048] = {0};
	if ( !m_pSIMEncoder->ISimEnc_GenSimLockData(pcSimLockData, &nSimLockData, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	if ( !m_pSIMEncoder->ISimEnc_CheckSimLockData(pcSimLockData, nSimLockData, szErrMsg))
	{
		m_pSIMEncoder->ISimEnc_FreeSimEncObj();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	char szTemp[4] = {0};
	strcpy(szSIMData, "");
	for (int i = 0; i < nSimLockData; i++)
	{
		sprintf(szTemp, "%02X", (BYTE)pcSimLockData[i]);
		strcat(szSIMData, szTemp);
	}
	m_strMessage = MakeStr("SIM Data: %s", szSIMData);
	FactoryLog();

	// Take Public Key //
	CString CStrPublicKey(szSIMData);
	CString csPublicKey = CStrPublicKey.Left(256);
	strcpy(szPublicKey, (LPCTSTR)csPublicKey);
	m_strMessage = MakeStr("Split Public Key: %s", szPublicKey);
	FactoryLog();

	m_pSIMEncoder->ISimEnc_FreeSimEncObj();
	m_strMessage = "Get SIMData from SIM Encoder Success";
	FactoryLog();
	return true;

}

bool CQueryOfflineDeviceInfo::LoadRsaKey(int type, char* szRSAKey, char* szErrMsg)
{
    if (type != 7 && type != 10) return true;

    T_RSA rsa;
    // XXX: See T_RSA definition
    rsa.iModulusSize = sizeof(DEFAULT_PUBLIC_EXPONENT);
    rsa.iPrivateExpSize = sizeof(DEFAULT_PRIVATE_EXPONENT);
    rsa.iPublicExpSize = sizeof(DEFAULT_MODULUS);

    std::vector<BYTE> public_exponent;
    std::vector<BYTE> private_exponent;
    std::vector<BYTE> modulus;

    if (strcmp(szRSAKey, "") != 0)
	{
        public_exponent.resize(sizeof(DEFAULT_PUBLIC_EXPONENT));
        private_exponent.resize(sizeof(DEFAULT_PRIVATE_EXPONENT));
        modulus.resize(sizeof(DEFAULT_MODULUS));

        // XXX: See T_RSA definition
        rsa.pModulus = &public_exponent[0];
        rsa.pPrivateExp = &private_exponent[0];
        rsa.pPublicExp = &modulus[0];

        UINT8 ret = LoadRsaKeyFromString(szRSAKey, &rsa);
        if (SIM_RET_SUCCESS != ret)	return false;

    }
	else
	{
        // XXX: See T_RSA definition
        rsa.pModulus = DEFAULT_PUBLIC_EXPONENT;
        rsa.pPrivateExp = DEFAULT_PRIVATE_EXPONENT;
        rsa.pPublicExp = DEFAULT_MODULUS;
    }

    return m_pSIMEncoder->ISimEnc_SetRSAKey(&rsa, szErrMsg);
}

UINT CQueryOfflineDeviceInfo::LoadRsaKeyFromString(const CString& RSAString, T_RSA * const rsa)
{
    ASSERT(rsa);

    int stringSize = RSAString.GetLength() ;
    std::vector<char> content(stringSize+1) ;
    for (int i=0; i<stringSize; ++i)
    {
        content[i] = RSAString.GetAt(i) ;
    }
    content[stringSize] = 0 ;

    if (content.empty()) {
        return SIM_RET_NO_RSAKEY;
    }

    XmlDocument doc;
    try {
        doc.parse<0>(&content[0]);
    } catch (rapidxml::parse_error&) {
        return SIM_RET_NO_RSAKEY;
    }

    XmlNode* rsa_node = doc.first_node("RSAKeyValue");
    if (!rsa_node) {
        return SIM_RET_NO_RSAKEY;
    }

    UINT8 ret = 0;

    ret = LoadKeyFromNode(
        rsa_node->first_node("Modulus"),
        rsa->pPublicExp,
        rsa->iPublicExpSize);
    if (SIM_RET_SUCCESS != ret) {
        return ret;
    }

    ret = LoadKeyFromNode(
        rsa_node->first_node("Exponent"),
        rsa->pModulus,
        rsa->iModulusSize);
    if (SIM_RET_SUCCESS != ret) {
        return ret;
    }

    ret = LoadKeyFromNode(
        rsa_node->first_node("D"),
        rsa->pPrivateExp,
        rsa->iPrivateExpSize);
    if (SIM_RET_SUCCESS != ret) {
        return ret;
    }

    return SIM_RET_SUCCESS;
}

UINT CQueryOfflineDeviceInfo::LoadKeyFromNode(const XmlNode * const node, unsigned char * const key, const size_t key_len)
{
    ASSERT(key);
    ASSERT(key_len);

    if (!node) {
        return SIM_RET_NO_RSAKEY;
    }

    XmlNode * child = node->first_node();

    for (XmlNode * child = node->first_node(); child;
        child = child->next_sibling()) {
            if (rapidxml::node_data == child->type()) {
                return LoadKeyFromBase64(child->value(), key, key_len);
            }
    }

    return SIM_RET_NO_RSAKEY;
}

UINT CQueryOfflineDeviceInfo::LoadKeyFromBase64(const char * base64, unsigned char * const key, const size_t key_len )
{
    ASSERT(base64);
    ASSERT(key);
    ASSERT(key_len);

    std::string result = Base64::Base64Decode(std::string(base64));

    if (result.length() != key_len) {
        return SIM_RET_NO_RSAKEY;
    }

#pragma warning(push)
#pragma warning(disable:4996)
    // The length of key is checked before, so it is save to use
    // std::copy().
    std::copy(result.begin(), result.end(), key);
#pragma warning(pop)

    return SIM_RET_SUCCESS;
}

