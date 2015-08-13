#ifndef _TESTBOX_HANDLER_STANDARD_H_
#define _TESTBOX_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CTestBoxHdrStd : public CEnhanceController
{
public:
	CTestBoxHdrStd(CEnhanceController* pNextCEC);
	virtual ~CTestBoxHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckTestBoxXMLExist();
	size_t LoadTestBox();
	size_t SetToolTestBox();
	
protected:
	CMyMSXML										m_TestBoxXML;
	std::map<CString, CString>				m_mapTesteBox;
};

#endif