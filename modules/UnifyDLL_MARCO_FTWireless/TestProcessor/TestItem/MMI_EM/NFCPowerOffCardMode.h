/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CNFCPowerOffCardMode object.
 * FileName   : CheckNfcFwVersion.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/03/15
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/15    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#ifndef _C_NFC_POWEROFF_CARD_MODE_H_
#define _C_NFC_POWEROFF_CARD_MODE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CNFCPowerOffCardMode : public ITestItem
{
RF_DECLARE_DYNCREATE(CNFCPowerOffCardMode)


// Data members
public:
protected:
private:
	//parameters
	float m_i_Distance;
	std::string m_str_Distance;
	std::string m_str_Message;
	std::string m_str_ErrorCode;


// Member functions
public:
	CNFCPowerOffCardMode() {}
	~CNFCPowerOffCardMode() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool NFCPowerOffCardModeTest();
};

#endif // End of #ifndef _C_NFC_POWEROFF_CARD_MODE_H_
