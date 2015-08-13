/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of BostonButtonCheck object.
 * FileName   : BostonButtonCheck.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu	
 * Time       : 2012.03.15

 ********************************************************************************/

#ifndef _C_BOSTON_BUTTON_CHECK_TEST_H_
#define _C_BOSTON_BUTTON_CHECK_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "HeadsetBottonCountTestDlg.h" 

class BostonButtonCheck : public ITestItem
{
RF_DECLARE_DYNCREATE(BostonButtonCheck)

// Data members
public:
	CButtonCountTestDlg *pButtonTestDlg; 
protected:
private:
	//parameters
	int m_i_Type;


// Member functions
public:
	BostonButtonCheck() {}
	~BostonButtonCheck() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool runButtonCount();

	CWinThread *KeyTestThreadHandle;
	static UINT AfxKeyTestDlgThread(LPVOID pParam);

	int TimeOut;
	bool b_StopkKeyTest;
	bool InitialKeyTestDlg();
	int  RunButtonTestProcess();
	bool EndButtonTestDlg();
};


#endif // End of #ifndef _C_JACK_DETECT_TEST_H_
