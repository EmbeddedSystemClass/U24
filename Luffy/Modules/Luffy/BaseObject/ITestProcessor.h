#ifndef _I_TOOL_PROCESSOR_H_
#define _I_TOOL_PROCESSOR_H_


#include "Utils/CommonDefine.h"
#include "Utils/StringUtil.h"
#include "Controller/FactoryDef.h"
#include "utils/Logger/CLogger.h"

interface ITestProcessor 
{
// Data member
public:
	std::map<std::string, std::string> g_mapToolInfo;

	//callback
	p_fn_event_callback m_p_event_callback;
	//p_fn_log_callback m_p_log_callback;
	p_fn_processor_callback m_p_processor_callback;

	//CLog
	std::string  m_LogName;
	CLogger* m_pLog;

protected:
private:
	
	int m_iSlot;

	std::string m_strErrCode;
	std::string m_strErrMsg;

	clock_t m_totalTestDuration_Start;
	clock_t m_testItemDuration_Start;


// Member functions
public:
	ITestProcessor();
	~ITestProcessor();

	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	/* Set/Get FA Data */
	virtual bool SetFAData(char* sz_value) = 0;
	virtual bool GetSerialNumber(int i_slot, char* sz_value, int i_size) = 0;
	virtual bool GetTimeStamp(int i_slot, char* sz_value, int i_size) = 0;

	/* Test sequence */
	virtual bool OnAPPLaunch() = 0;
	virtual bool PreRun(int i_slot) = 0;
	virtual bool RunTestingProcedureByName(const char *szItem , int i_slot) = 0;
	virtual bool RunInitialSettingByName(const char *szItem, int i_slot) = 0;
	virtual bool PostRun(int i_slot) = 0;
	virtual bool Stop() = 0;
	virtual bool OnAPPEnd() = 0;

	// UI control functions
	void SetUIPattern(std::string strPattern);
	void SetTestingItem(std::string strTestItemName);
	void SetPICSData(std::string strKey, std::string strValue);
	void SetTestResult(bool bPass);
	void SetUIMessage(std::string strMsg);
	void SetUIProgress(int iMainProgress, int iSubProgress);

	// Test duration functions
	void StartTotalDurationCounter();
	void StartTestItemDurationCounter();
	std::string GetTestDuration();
	std::string GetTotalTestDuration();

	//Fire functions
	void Fire(const std::string& strEvent, long nParam);
	void FireToUI(const std::string& strEvent, long nParam);

	// Log callback to controller
	void TraceLog(std::string strMsg);

	// extend log functions 
	void FactoryLog(bool bPass, std::string strMsg);
	void FactoryLog( bool bPass, std::string strItemCode, std::string strErrCode, std::string strBand, std::string strChannel, std::string strLower, std::string strUpper, std::string strMeasured, std::string strUnit, std::string strMsg);
	void CPKLog( const char* szTech, const char* szItemName, const char* szBandClass, const char* szChannel, const char* szIdentifier1, const char* szIdentifier2, const char* szLowerSpec, const char* szUpperSpec, const char* szMeasValue, bool bPass, int iSlot );
	
	/* Public info */
	void SetSlot(int iSlot){m_iSlot = iSlot;}
	int GetCurrentSlot(){return m_iSlot;}
	p_fn_processor_callback GetProcessorCallback(){return m_p_processor_callback;}
	
	//callback
	//bool reg_log_callback( p_fn_log_callback pcallback );
	bool reg_event_callback( p_fn_event_callback pcallback );
	bool reg_processor_callback( p_fn_processor_callback pcallback );
};


#endif // End of #ifndef _I_TOOL_PROCESSOR_H_
