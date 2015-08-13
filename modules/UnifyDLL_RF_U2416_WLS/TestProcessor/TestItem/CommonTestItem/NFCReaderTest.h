
#ifndef _C_NFC_READER_TEST_H_
#define _C_NFC_READER_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CNFCReaderTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CNFCReaderTest)


// Data members
public:
protected:
private:
	//parameters
	bool m_WriteCPKLog;
	std::vector <std::string> m_CPKHeaderList;

	std::string m_strMessage;
	std::string m_strMeasured;
	CString tmp_strMeasured;

// Member functions
public:
	CNFCReaderTest();
	~CNFCReaderTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool CheckNFCReaderID();
	bool ParseCPKItems();

};


#endif // End of #ifndef _C_NFC_READER_TEST_H_
