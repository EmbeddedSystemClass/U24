/***************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name   : Page Interface class  
* File name     : PageInterface.h
* Summary       : Station Pages Interface class .           
* Version     Author             Date                Abstract                      
* 0.1        Gilpin.Xi        2011/07/11           First version  
****************************************************************************/

#pragma once
#include <map>
#include "afxcmn.h"

class CPageInterface: public CPropertyPage
{
//	DECLARE_DYNAMIC(IPropertyPage)
public:
	CPageInterface(UINT nIDTemplate)
		: CPropertyPage(nIDTemplate)
	{

	}

	virtual ~CPageInterface(){}

	/* Get Picasso from station Pages */
	virtual bool GetPicasso(std::map<int, CString> &map_picasso) = 0;
	virtual bool SetPicasso(std::map<int, CString> map_picasso) = 0;

	/* Get Tag from station Pages */
	virtual bool GetTag(std::map<int, CString> &map_Tag) = 0;
	virtual bool SetTag(std::map<int, CString> map_Tag) = 0;
	virtual bool GetSn(std::map<int, CString> &map_Sn) = 0;
	virtual bool SetSn(std::map<int, CString> map_Sn) = 0;
	virtual void SetTestResult(int i_slot, CString str_testresult) = 0;

	/* Station Pages test progress */
	virtual bool Run() = 0;
	virtual bool Stop() = 0;
	virtual bool ResetPage() = 0;
	virtual bool PostRun() = 0;
	virtual void ChangeGB() = 0;
};
