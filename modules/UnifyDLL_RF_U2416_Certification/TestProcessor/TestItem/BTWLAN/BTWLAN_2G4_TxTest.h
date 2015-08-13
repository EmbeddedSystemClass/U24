#ifndef _C_BTWLAN_2_4G_TX_TEST_H_
#define _C_BTWLAN_2_4G_TX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CBTWLAN_2G4_Test : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CBTWLAN_2G4_Test)

private:
	std::string m_strMsg;
	std::string m_strInput;

public:
	CBTWLAN_2G4_Test(void);
	virtual ~CBTWLAN_2G4_Test(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(void);

private:
	bool MainFunction(void);

};

#endif // End of #ifndef _C_BTWLAN_2_4G_TX_TEST_H_