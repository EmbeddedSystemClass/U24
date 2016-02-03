/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDublin2KeyTest object.
 * FileName   : BostonKeyTest.h
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.02.26
 ********************************************************************************/

#ifndef _C_KEY_TEST_H_
#define _C_KEY_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "SideKeyTestDlg.h"  //2KEY,  volumn up, volumn down


class CDublin2KeyTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CDublin2KeyTest)

// Data members
public:
	CDublin2KeyTestDlg *pKeyTestDlg;  //2KEY,  volumn up, volumn down
protected:
private:
	bool b_StopkKeyTest;
	int TimeOut;


// Member functions
public:
	CDublin2KeyTest();
	~CDublin2KeyTest();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	CWinThread *KeyTestThreadHandle;
	static UINT AfxKeyTestDlgThread(LPVOID pParam);

	bool KeyTest();
	bool InitialKeyTestDlg();
	bool RunKeyTestProcess();
	bool EndKeyTestDlg();
};


#endif // End of #ifndef _C_KEY_TEST_H_