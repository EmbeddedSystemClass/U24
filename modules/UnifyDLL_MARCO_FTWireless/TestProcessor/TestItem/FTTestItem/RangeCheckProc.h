/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CRangeCheckProc object.
 * FileName   : CRangeCheckProc.h
 * Version    : $Revision:   1.1  $
 * Author     : Neil.Du
 * Time       : 2011/11/22
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/11/22      Neil.Du             Create.
 ********************************************************************************/

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
#define PCBVerMPP3				_T("PCBVerMPP3")
#define PCBVerMPP2				_T("PCBVerMPP2")


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

	bool m_b_CheckFromServer;
	//std::string m_str_ServerValue;

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
	bool ReadPCBVerMPP3();
	bool ReadPCBVerMPP2();
};


#endif // End of #ifndef _C_Range_Check_Proc_H_
