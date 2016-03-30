#ifndef _TESTITEM_HANDLER_STANDARD_H_
#define _TESTITEM_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CTestItemHdrStd : public CEnhanceController
{
public:
	CTestItemHdrStd(CEnhanceController* pNextCEC);
	virtual ~CTestItemHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

	virtual size_t Enhance_SetResopnseToShowDlg(char* sz_showReslut);

protected:
	size_t CheckTestItemXMLExist();
//	size_t SetTestItemXMLValueForCSDWrite();
//	size_t LoadTestProcessor();
//	size_t GetTestItem();
	size_t SetToolTestItem();
	size_t SetOtherParameter();
	void RunMustTestItem(size_t StartItem);
	bool RunTestItem(size_t StartItem, CString strRunType);

protected:
//	CMyMSXML				m_TestItemXML;
	std::vector<CString>	m_vTestItemTypeList;
	bool					m_bForcePass;
	CString					m_strProcessorName;
};

#endif