#ifndef __UNIFYADAPTER_H__
#define __UNIFYADAPTER_H__

#include "FacTestToolInterface.h"
#include "CommonDef.h"
#include "..\Common\CommonUtil\CGLog.h"
#include <afxmt.h>

using namespace QSYNC_LIB;

class CUnifyAdapter : public IFacTestToolInterface, 
					  public CNotifyBase_Char
{
public:
	CUnifyAdapter();
	virtual ~CUnifyAdapter();

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                          INotifyBase_Char
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool Register(INotify_Char* pNotify,const char* strEvent);
	virtual bool Register(EventFunc_Char* pEventFunc,const char* strEvent);

	virtual bool Unregister(INotify_Char* pNotify,const char* strEvent);
	virtual bool Unregister(EventFunc_Char* pEventFunc,const char* strEvent);

	virtual void DefineNotify(const char* strNewEvent);
	virtual void Fire(const char* strEvent,long nParam);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                          UnifyInterface
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size);

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

	/* Test sequence */
	virtual bool Begin    (int i_slot = 1);
	virtual bool PreRun   (int i_slot = 1);
	virtual bool Run      (int i_slot = 1);
	virtual bool PostRun  (int i_slot = 1);
	virtual bool Stop     (int i_slot = 1);
	virtual bool End      (int i_slot = 1);

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait) = 0;

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut) = 0;

protected:
	virtual bool Log(const char *szLog, const int Severity = __INFO, const int i_slot = 1);
	
	bool SetLogName(const char *szLogName, const int i_slot = 1);

protected:
	CStringA m_cstrModulePath;

private:
	std::vector<CStringA> m_vecLogFileName;

	CGLog *cGlog;

	CCriticalSection m_CritSection_Log;
	CCriticalSection m_CritSection_SetLogName;

private:
	bool SetLogFullName(const char *szLogName);
	void DefineUINotify();
};
#endif//__UNIFYADAPTER_H__