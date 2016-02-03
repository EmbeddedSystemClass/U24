
#ifndef _C_UPLOAD_RECORD_TO_YRSTATION_H_
#define _C_UPLOAD_RECORD_TO_YRSTATION_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CUploadRecordToYrstation: public ITestItem
{
	GP_DECLARE_DYNCREATE(CUploadRecordToYrstation)

	std::string m_strErrCode;
	std::string m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	st_UIMessage m_st_UIMessage;

	unsigned char m_szPicasso[10];
	unsigned char m_szLine[5];
	unsigned char m_szStation[20];
	//unsigned char m_szStationSerNo[5];
	unsigned char m_szModel[10];
public:
	//function
	CUploadRecordToYrstation(void);
	virtual ~CUploadRecordToYrstation(void);

private:
	//function
	bool MainFunction(void);
	bool InitData(std::map<std::string, std::string>& paramMap);
	bool Run(int i_slot = 0);
	bool LoadPicasso(void);
	bool UploadData(void);
};
#endif