/*********************************************************************************
 * COPYRIGHT Qisda WR24 2011
 *
 * Description: .h file of CBootCurrent object.
 * FileName   : CBootCurrent.h
 * Version    : $Revision:   1.1  $
 * Author     : CY.Lee
 * Time       : 2012/05/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/05/23     CY.Lee            Create.
 ********************************************************************************/

#ifndef _C_Boot_Current_H_
#define _C_Boot_Current_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CBootCurrent : public ITestItem
{
RF_DECLARE_DYNCREATE(CBootCurrent)


// Data members
private:

	int m_i_WaitTimeForFetch;
	int m_i_delayTimeMS;
	CRangePair m_obj_LeakageCurrent_Spec;
	CRangePair m_obj_PowerOnShort_Spec;
		

// Member functions
public:
	CBootCurrent() {}
	~CBootCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool BootCurrentTest();
};


#endif // End of #ifndef _C_Boot_Current_H_
