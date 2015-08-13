/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of CReadPicassoNum object.
 * FileName   : CReadPicassoNum.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/16
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/16      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef _C_Read_Picasso_Num_H_
#define _C_Read_Picasso_Num_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CReadPicassoNum : public ITestItem
{
RF_DECLARE_DYNCREATE(CReadPicassoNum)


// Data members
public:
protected:
private:
	//parameters
	bool m_b_CheckMatch;
	int m_i_sleep;

// Member functions
public:
	CReadPicassoNum() {}
	~CReadPicassoNum() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool ReadPicassoNum();
};


#endif // End of #ifndef _C_Read_Picasso_Num_H_
