#ifndef _POWERSUPPLY_HANDLER_STANDARD_H_
#define _POWERSUPPLY_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CPowerSupplyHdrStd : public CEnhanceController
{
public:
	CPowerSupplyHdrStd(CEnhanceController* pNextCEC);
	virtual ~CPowerSupplyHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckPSXMLExist();
	size_t LoadPS();
	size_t SetToolPS();
	size_t LoadPSCount();
	size_t LoadPSList();
	size_t LoadPSItem();

protected:
	CMyMSXML										m_PSXML;
	std::map<CString, CString>				m_mapPS;
	std::vector<CString>						m_mapPSList;
};

#endif