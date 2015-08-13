#ifndef _TOOL_LOADER_H_
#define _TOOL_LOADER_H_

#include "EnhanceController.h"

class CToolLoader : public CEnhanceController
{
public:
	CToolLoader(CEnhanceController* pNextCEC);
	~CToolLoader();

	virtual size_t Enhance_OnAPPLaunch();

	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);

	virtual size_t Enhance_OnRunFinish(int i_slot = 1);

	virtual size_t Enhance_OnAPPEnd();

	virtual size_t Enhance_SetResopnseToShowDlg(char* sz_showReslut);

protected:
	size_t FreeToolDLL();
	size_t LoadToolDLL();

protected:
	HMODULE							m_hDLL;
};

#endif