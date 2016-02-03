/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CStringCheckProc object.
 * FileName   : CStringCheckProc.h
 * Version    : $Revision:   1.1  $
 * Author     : Neil.Du
 * Time       : 2011/11/22
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/11/22      Neil.Du             Create.
 ********************************************************************************/

#ifndef _C_String_Check_Proc_H_
#define _C_String_Check_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

/* Name */
#define CPUid					_T("CPUid")				
#define VERftd					_T("VERftd")			
#define VERchange				_T("VERchange")
#define BATTrtcTime				_T("BATTrtcTime")
#define LEDid					_T("LEDid")
#define MCPflashID				_T("MCPflashID")
#define MCPddrID				_T("MCPddrID")

#define MCPemmcsize				_T("MCPemmcsize")
#define MCPemmcFWversion		_T("MCPemmcFWversion")
#define MCPddrmr8				_T("MCPddrmr8")

#define PMICID					_T("PMICID") 


#define RF_PA_QFE_ID			_T("RF_PA_QFE_ID")
#define RF_ASM_ID				_T("RF_ASM_ID")

#define SKU_ID					_T("SKU_ID")
#define SOCversion				_T("SOCversion")
#define SOCfeature_ID			_T("SOCfeature_ID")
#define SGAUGE					_T("Gauge")


#define BuckID					_T("BuckID")			// 2 , 18		
#define Buckfeature				_T("BuckFeature")		// 2 , 19	
#define BacklightDriver			_T("BacklightDriver")	// 4 , 14
#define GyroID					_T("GyroID")			
#define ChargerID				_T("ChargerID")	 // 2 , 5 

#define PMid					_T("PMid")
#define PMIid					_T("PMIid")
#define PMIid					_T("PMIid")
#define APQ_UART				_T("APQ_UART")
#define HDMI_DDC_RX				_T("HDMI_DDC_RX")
#define HDMI_OUTPUT				_T("HDMI_OUTPUT")
#define WLAN_MODULE				_T("WLAN_MODULE")
#define BT_INQUIRY				_T("BT_INQUIRY")



class CStringCheckProc : public ITestItem
{
RF_DECLARE_DYNCREATE(CStringCheckProc)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_Name;
	std::string m_str_Value;
	bool m_b_CheckFromServer;
	bool m_b_CheckMatch;
	int m_i_sleep;
	std::string str_msg;

// Member functions
public:
	CStringCheckProc() {}
	~CStringCheckProc() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool ReadMCPflashID();
	bool ReadMCPddrID();
	
	bool ReadMCPddrmr8();  
	bool ReadMCPemmcsize();  
	bool ReadMCPemmcFWversion();  

	bool ReadSOCversion(); 

	
	bool Read_BuckID();
	bool Read_Buckfeature();
	bool Read_BacklightDriver();
	bool Read_GyroID();
	bool Read_ChargerID();

	bool ReadPMid();
	bool ReadPMIid();
	bool Check_APQ_UART();

	bool Check_HDMI_OUTPUT();
	bool Check_HDMI_DDC_RX();
	
	bool Check_WLAN_MODULE();
	bool Check_BT_INQUIRY();
	bool showMsg(const char* szMsg);

};


#endif // End of #ifndef _C_String_Check_Proc_H_

