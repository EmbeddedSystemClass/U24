#ifndef _C_Range_Check_Proc_H_
#define _C_Range_Check_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

/* Name */
#define BoardADC					_T("BoardADC")
#define BATTCoinCellVoltage			_T("BATTCoinCellVoltage")
#define PWRgaugeVoltage				_T("PWRgaugeVoltage")
#define PWRgaugeCurrent				_T("PWRgaugeCurrent")
#define PWRgaugeTemperature			_T("PWRgaugeTemperature")
#define SNSRlsensorValue			_T("SNSRlsensorValue")
#define SNSRpsensorValue			_T("SNSRpsensorValue")
#define MCPddrSize					_T("MCPddrSize")		//-- CY.Lee 120604 --//

#define RFBandIDTemperature			_T("RFBandIDTemperature")		
#define Board_ID					_T("Board_ID")
#define PowerMaxCurrent				_T("PowerMaxCurrent")



class CRangeCheckProc : public ITestItem
{
RF_DECLARE_DYNCREATE(CRangeCheckProc)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_Name;
	std::string m_str_Value;
	CRangePair m_rp_Value;
	bool m_b_CheckMatch;
	int m_i_sleep;

// Member functions
public:
	CRangeCheckProc() {}
	~CRangeCheckProc() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool ReadBoardADC();
	bool ReadPWRgaugeVoltage();
	bool ReadPWRgaugeCurrent();
	bool ReadPWRgaugeTemperature();
	bool ReadSNSRlsensorValue();
	bool ReadSNSRpsensorValue();
	bool ReadMCPddrSize();			//-- CY.Lee 120604 --//
	bool ReadRFBandIDTemperature();
	bool ReadBoard_ID(); 
	bool ReadPowerMaxCurrent(); 
	
};


#endif // End of #ifndef _C_Range_Check_Proc_H_
