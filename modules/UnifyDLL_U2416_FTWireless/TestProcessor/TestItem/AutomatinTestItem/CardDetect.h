/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CardDetect object.
 * FileName   : CardDetect.h
 * Version    : $Revision:   1.1  $
 * Author     : Lion.Wu
 * Time       : 2012.3.14
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/14    Lion.Wu             Create.
 ********************************************************************************/

#ifndef CardDetect_Proc_H_
#define CardDetect_Proc_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"

/* Name */
#define SIMCardDetect					 _T("SIMCardDetect")
#define SDCardDetect					 _T("SDCardDetect")
#define SIMCardDataDetect					 _T("SIMCardDataDetect")
#define SDCardDataDetect					 _T("SDCardDataDetect")

class CardDetectProc : public ITestItem
{
RF_DECLARE_DYNCREATE(CardDetectProc)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_Name;
	std::string m_str_Value;
	std::string str_msg;

	CRangePair m_rp_Value;
	bool m_b_CheckMatch;
	int m_i_sleep;

// Member functions
public:
	CardDetectProc() {}
	~CardDetectProc() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool runSIMCardDetect();
	bool runSDCardDetect();
	bool runSIMCardDataDetect();
	bool runSDCardDataDetect();

	bool showMsg(char* szMsg);
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
};


#endif // End of #ifndef _C_Range_Check_Proc_H_
