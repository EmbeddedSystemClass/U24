#ifndef _C_WRITE_DEVICE_INFO_H_
#define _C_WRITE_DEVICE_INFO_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CWriteDeviceInfo : public ITestItem
{
RF_DECLARE_DYNCREATE(CWriteDeviceInfo)

// Data members
public:
protected:
private:
	int m_iRetry;
	bool m_bWriteIMEI;
	bool m_bWriteBT;
	bool m_bWriteWIFI;
	bool m_bWriteMSN;
	bool m_bWriteFSN;
	bool m_bWriteSIMData;
	bool m_bClearData;

// Member functions
public:
	CWriteDeviceInfo() {}
	~CWriteDeviceInfo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool WriteIMEI();
	bool WriteBT();
	bool WriteWIFI();
	bool WriteMSN();
	bool WriteFSN();
	bool WriteSIMData();

	typedef bool (__cdecl * PFWriteSimDataNew_SPU)(char* szPort, char* szSimDataBuf);
};


#endif // End of #ifndef _C_WRITE_DEVICE_INFO_H_
