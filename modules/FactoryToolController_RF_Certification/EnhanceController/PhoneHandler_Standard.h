#ifndef _PHONE_HANDLER_STANDARD_H_
#define _PHONE_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CPhoneHdrStd : public CEnhanceController
{
public:
	CPhoneHdrStd(CEnhanceController* pNextCEC);
	virtual ~CPhoneHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckTestItemXMLExist();
	size_t CheckConfigXMLExist();
	size_t LoadPhoneInTestItem();
	size_t LoadPhoneInConfig();
	size_t SetToolPhone();

protected:
	CMyMSXML						m_TestItemXML, m_ConfigXML;
	std::map<CString, CString>		m_mapPhone;
};

#endif