/*********************************************************************************
 * COPYRIGHT Qisda WR24 2014
 *
 * Author     : Lion.Wu
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSensorLuxCheckHShark : public ITestItem
{
RF_DECLARE_DYNCREATE(CSensorLuxCheckHShark)

public:
	CSensorLuxCheckHShark(); 
	virtual ~CSensorLuxCheckHShark() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

    std::string str_DeviceName;
	std::string str_Keyword;
	std::string str_PROLIFICcomPort;

	std::string m_str_SensorLuxTestItem;
	std::string strLuxVersion;
	std::string strChipVersion;
	
private:

	bool SetDCB(HANDLE h_Com);
	bool ConfigureCOMPort(CString strPort);
	bool SendJigRequest(CString str_command);
	void Log_Value(CString message);
	bool DetectDevice();
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);	

	bool initialHandle();
	bool closeHandle();
	bool runLuxCheckHShark();
	bool runCloseLight();
	bool sendCMD(char *sz_cmd);
	bool receCMD(const char *sz_cmd);
private:
	HANDLE		m_h_Com;
	DWORD numberOfBytesRead;
	OVERLAPPED overlappedWrite, overlappedRead;

	char szTmp[256];
	static const int bufferSize = 100;
	int iFindPos;

	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	int			m_i_repeat;
	int         m_i_shift;     
	int			m_i_retry;
	int			m_i_EnableJIG;
};
