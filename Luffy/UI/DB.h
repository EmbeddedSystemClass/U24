
// DB.h : header file
//

#include "Utils\CommDef.h"

#define DUMMY_DB _T("DummyDB.txt")

class CDB
{
public:
	CDB(void);
	~CDB(void){};

	void SetidParameter(st_UIIDparameter st_idParameter);
	void SetslotPICSData(std::map<int, std::map<CString, CString>> map_slotPICSData);
	bool ReadRecordFromDB(int &iTotal, int &iPass, int &iFail);
	bool WriteRecordToDB(int iTotal, int iPass, int iFail);
	bool InsertPICSDataToDB(int iSlot);

private:
	bool ConnectDataBase();
	void ReadRecord(int &iTotal, int &iPass, int &iFail);
	void WriteRecord(int iTotal, int iPass, int iFail);
	bool InsertPICSData(int iSlot);

public:
	CString m_str_errorCode;
	CString m_str_errorMsg;

private:
	static CCriticalSection m_obj_DBLock;

	st_UIIDparameter m_st_idParameter;
	std::map<int, std::map<CString, CString>> m_map_slotPICSData;

	CString m_strDummyDB;

	bool m_bDBReady;
};