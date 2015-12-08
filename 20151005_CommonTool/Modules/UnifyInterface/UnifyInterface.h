#ifndef _UnifyINTERFACE_H
#define _UnifyINTERFACE_H


#include "NotifyBase.h"

using namespace QSYNC_LIB;

interface UnifyInterface : public INotifyBase_Char
{
	virtual ~UnifyInterface(){}
	virtual void SetParameter(char* lpszKeyword, char* lpszValue) = 0;
	virtual bool GetParameter (char* lpszKeyword, char* lpszValue) = 0;

	virtual bool SetFAData (int slot, char* lpszValue) = 0;
	virtual bool GetFAData (int slot, char* lpszValue) = 0;

	virtual void SetStation(char* lpszStation) = 0;
	virtual void SetModel(char* lpszModel) = 0;

	virtual void GetTestItem(char* lpszTestItem) = 0;

	virtual bool PreRun () = 0;
	virtual bool Run () = 0;
	virtual bool PostRun () = 0;
};


typedef bool (*pfnNewUnifyInterface)(UnifyInterface**); 
typedef bool (*pfnDeleteUnifyInterface)(UnifyInterface*); 



#endif //_UnifyINTERFACE_H