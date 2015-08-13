#ifndef _DownLoad_HANDLER_STANDARD_H_
#define _DownLoad_HANDLER_STANDARD_H_

#include "EnhanceController.h"
#include "..\..\Common\CommonUtil\XML\MyMSXML.h"

class CDownLoadHdrStd : public CEnhanceController
{
public:
	CDownLoadHdrStd(CEnhanceController* pNextCEC);
	virtual ~CDownLoadHdrStd();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot);

	virtual size_t Enhance_RunTest(int i_slot);

	virtual size_t Enhance_OnRunFinish(int i_slot);

	virtual size_t Enhance_OnAPPEnd();

protected:
	size_t CheckTestItemXMLExist();
	size_t LoadImage();



protected:
	CMyMSXML										m_TestItemXML;
};

#endif