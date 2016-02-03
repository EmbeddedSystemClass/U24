/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: L-Sensor Lux Check.
 * FileName   : BostonLSensorLuxCheck.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/07/30
 * History    :
 *  NO      Date            Updated by            Content of change
 *	1		2012/07/30		Lion.Wu				  Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CBostonLSensorLuxCheck : public ITestItem
{
RF_DECLARE_DYNCREATE(CBostonLSensorLuxCheck)

public:
	CBostonLSensorLuxCheck() {}
	virtual ~CBostonLSensorLuxCheck() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

    std::string m_strDeviceName;
	std::string m_strKeyword;
	std::string m_strPROLIFICcomPort;
private:
	bool LuxCheckProc(void);

	bool SetDCB(HANDLE h_Com);
	bool ConfigureCOMPort(CString strPort);
	bool SendJigRequest(CString str_command);
	void Log_Value(CString message);
	bool DetectDevice();
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);	
private:
	CRangePair	m_obj_LuxSpec;
	HANDLE		m_h_Com;
	std::string m_str_comport;
	int         m_i_sleep_for_Jig;
	int         m_i_sleep_for_Data;
	int			m_i_repeat;
	int         m_i_shift;     
	int			m_i_retry;
	int			m_i_EnableJIG;
};
