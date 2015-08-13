#ifndef _C_QUERY_OFFLINE_DEVICE_INFO_H_
#define _C_QUERY_OFFLINE_DEVICE_INFO_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"

#include "Base64.h"
#include "SIMEncoder_API.h"
#include "../../../../../Lib/rapidxml/rapidxml.hpp"

// CSV Mapping Table ITEM Define
#define     _MAPPING_ITEM_INDEX         0
#define     _MAPPING_ITEM_IMEI          1
#define     _MAPPING_ITEM_BT            3
#define     _MAPPING_ITEM_WIFI          4
#define     _MAPPING_ITEM_PVKID         5
#define     _MAPPING_ITEM_PVKFILENAME   6
#define     _MAPPING_ITEM_FSN           7
#define     _MAPPING_ITEM_NWCODE        8
#define     _MAPPING_ITEM_RSA           9
#define     _MAPPING_ITEM_LOC          10

class CQueryOfflineDeviceInfo : public ITestItem
{
RF_DECLARE_DYNCREATE(CQueryOfflineDeviceInfo)

// Data members
public:
protected:
private:
	int m_iRetry;
	char m_szIMEI[32];
	char m_szFSN[128];
	std::string m_strCSVPath;

	SIMEncoder_Function	*m_pSIMEncoder;

    typedef char Ch;
    typedef rapidxml::xml_document<Ch> XmlDocument;
    typedef rapidxml::xml_node<Ch> XmlNode;
    static BYTE DEFAULT_PRIVATE_EXPONENT[];
    static BYTE DEFAULT_PUBLIC_EXPONENT[];
    static BYTE DEFAULT_MODULUS[];

// Member functions
public:
	CQueryOfflineDeviceInfo() {}
	~CQueryOfflineDeviceInfo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	void GetFullPath(char* chPath, char* chFullPath);
	bool SeekCSVFileParam(char* szIMEI);
	bool GetSIMData(char* szSIMFilePath, char* szPublicKey, char* szSIMData);

	bool LoadRsaKey(int type, char* szRSAKey, char* szErrMsg);
	UINT LoadRsaKeyFromString(const CString& RSAString, T_RSA * const rsa);
	UINT LoadKeyFromNode(const XmlNode * const node, unsigned char * const key, const size_t key_len);
	UINT LoadKeyFromBase64(const char * base64, unsigned char * const key, const size_t key_len );

};


#endif // End of #ifndef _C_QUERY_OFFLINE_DEVICE_INFO_H_
