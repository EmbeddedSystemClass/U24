#ifndef _CABLELOSS_HANDLER_STANDARD_H_
#define _CABLELOSS_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CCablelossHdrStd : public CEnhanceController
{
public:
	CCablelossHdrStd(CEnhanceController* pNextCEC);
	virtual ~CCablelossHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckCablelossXMLExist();
	size_t LoadCableloss();
	size_t SetToolCableloss();

protected:
	CMyMSXML										m_CableLossXML;
	std::map<CString, std::map<CString, CString>> g_mapTxCableLossData;
	std::map<CString, std::map<CString, CString>> g_mapRxCableLossData;
};

#endif