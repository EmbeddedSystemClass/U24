/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CHeadsetInfo object.
 * FileName   : HeadsetInfo.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/01/16    Evonne.Hsieh            Create for Boston project BB test
 ********************************************************************************/

#ifndef _C_HEADSET_INFO_H_
#define _C_HEADSET_INFO_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CHeadsetInfo : public ITestItem
{
RF_DECLARE_DYNCREATE(CHeadsetInfo)

// Data members
public:
protected:
private:


// Member functions
public:
	CHeadsetInfo() {}
	~CHeadsetInfo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool HeadsetInfo();
};


#endif // End of #ifndef _C_HEADSET_INFO_H_