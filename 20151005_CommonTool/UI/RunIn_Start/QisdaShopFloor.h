#ifndef __QISDASHOPFLOOR_H__
#define __QISDASHOPFLOOR_H__

//#include "..\..\inc\UnifyInterface\CommonDef.h"
#include <vector>
#include <afxmt.h>

#define ERROR_XMLMISSING				"DB1900"
#define ERROR_XMLNODELISTMISSING		"DB1901"
#define ERROR_SETTOPREDBFAIL			"DB1902"
#define ERROR_WRITESTRINGFAIL			"DB1903"
#define ERROR_OUTOFRANGEFAIL			"DB1904"

#define DefaultStringLength				512
#define DefaultDeviceInput				8

typedef enum DBInfo{
	DBI_SO,
	DBI_SOTYPE,
	DBI_EMPLOYEEID,
	DBI_PROCESS,
	DBI_MODEL,	
	DBI_PN,
	DBI_LINE,
	DBI_SEGMENT,	
	DBI_PCNAME,
	DBI_EMPLOYEELV,

	DBI_LAST,
} dbinfo_e;

class CQisdaShopFloor
{
public:
	CQisdaShopFloor();
	~CQisdaShopFloor();

private:
	CArray <std::vector <CStringA>, char *> m_cstrPICSdata;
	CArray <CStringA, char *> m_cstrDBInfo;
	CArray <CStringA, char *> m_cstrPICSItem;
	
	CCriticalSection m_CritSection_WritePICS;
	CCriticalSection m_CritSection_SetPICS;
	CCriticalSection m_CritSection_CheckRoute;

public:
	const bool GetDBInfo(dbinfo_e nItem, char *szItemBuffer, char *szErrCode, char *szErrMsg);

	void SetPICS(const char *szValue, int nSlot = 0);

	const bool Connect() const;
	void Disconnect() const;
	
	const bool QueryDB(const char* szSO, const char* szEmployee, char* szErrCode, char* szErrMsg);
	const bool CheckRoute(char *szID, char *szErrCode, char *szErrMsg);
	
	const bool Write(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot = 0);

private:
	const bool ParsingXML(char *szErrCode, char *szErrMsg);
	const bool WriteToPicsFile(bool res, char *szInErrcode, int nSlot);
	const bool WriteToQisdaShopFloor(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot);
};

#endif