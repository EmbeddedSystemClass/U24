#ifndef _GPIB_HANDLER_STANDARD_H_
#define _GPIB_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CGPIBHdrStd : public CEnhanceController
{
public:
	CGPIBHdrStd(CEnhanceController* pNextCEC);
	virtual ~CGPIBHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckGPIBXMLExist();
	size_t LoadGPIB();
	size_t SetToolGPIB();

protected:
	CMyMSXML										m_GPIBXML;
	std::map<CString, CString>				m_mapGPIB;
};

#endif