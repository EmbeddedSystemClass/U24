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
#define SNSRgsensorAcc			_T("SNSRgsensorAcc")
#define LEDid					_T("LEDid")
#define BoardRev				_T("BoardRev")
#define MCPflashID				_T("MCPflashID")
#define MCPddrID				_T("MCPddrID")

#define MCPemmcsize				_T("MCPemmcsize")
#define MCPemmcFWversion		_T("MCPemmcFWversion")
#define MCPddrmr8				_T("MCPddrmr8")
#define MCPddrmr6				_T("MCPddrmr6")

#define AudioID					_T("AudioID") 
#define PMICID					_T("PMICID") 


#define RF_PA_QFE_ID			_T("RF_PA_QFE_ID")
#define RF_ASM_ID				_T("RF_ASM_ID")

#define SKU_ID					_T("SKU_ID")
#define SOCversion				_T("SOCversion")
#define SOCfeature_ID			_T("SOCfeature_ID")
#define SGAUGE					_T("Gauge")


#define SNSR_GsensorID			_T("SNSR_GsensorID")
#define SNSR_GsensorSelfTest	_T("SNSR_GsensorSelfTest")

#define SNSR_EcompassID			_T("SNSR_EcompassID")
#define SNSR_EcompassSelfTest	_T("SNSR_EcompassSelfTest")

#define FM_SelfTest				_T("FM_SelfTest")
#define BT_SelfTest				_T("BT_SelfTest")

#define NFC_ID					_T("NFC_ID")
#define FlashLED_ID				_T("FlashLED_ID")

#define BuckID					_T("BuckID")			// 2 , 18		
#define Buckfeature				_T("BuckFeature")		// 2 , 19	
#define BacklightDriver			_T("BacklightDriver")	// 4 , 14
#define GyroID					_T("GyroID")			
#define ChargerID				_T("ChargerID")	 // 2 , 5 

#define PMid					_T("PMid")
#define PMIid					_T("PMIid")


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

	bool ReadCPUid();
	bool ReadVERftd();
	bool ReadVERchange();
	bool ReadBATTrtcTime();
	bool ReadSNSRgsensorAcc();

	bool ReadLEDid();
	bool ReadBoardRev();
	bool ReadMCPflashID();
	bool ReadMCPddrID();
	
	bool ReadMCPddrmr8();  
	bool ReadMCPemmcsize();  
	bool ReadMCPemmcFWversion();  
	bool ReadMCPddrmr6();  


	bool ReadAudioID();  
	bool ReadPMICID();  


	
	bool ReadRF_PA_QFE_ID();  
	bool ReadRF_ASM_ID();  
	
	bool ReadSKU_ID(); 
	bool ReadSOCversion(); 
	bool ReadSOCfeature_ID(); 
	bool GaugeCheckID();


	bool Read_GsensorID();
	bool Read_GsensorSelfTest();
	bool Read_EcompassID();
	bool Read_EcompassSelfTest();


	bool Read_BTSelfTest();
	bool Read_FMSelfTest();

	bool Read_NFC_ID();
	bool Read_FlashLED_ID();

	bool Read_BuckID();
	bool Read_Buckfeature();
	bool Read_BacklightDriver();
	bool Read_GyroID();
	bool Read_ChargerID();

	bool ReadPMid();
	bool ReadPMIid();


};


#endif // End of #ifndef _C_String_Check_Proc_H_
