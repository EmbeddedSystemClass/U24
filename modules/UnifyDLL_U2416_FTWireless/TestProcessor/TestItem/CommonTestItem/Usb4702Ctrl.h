#ifndef _C_USB4702_CTRL_H_
#define _C_USB4702_CTRL_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CUsb4702Ctrl : public ITestItem
{
RF_DECLARE_DYNCREATE(CUsb4702Ctrl)


// Data members
public:
protected:
private:

	int m_i_list[8];
	int m_i_list_length;
	int m_i_Action;

// Member functions
public:
	CUsb4702Ctrl() {}
	~CUsb4702Ctrl() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:

	bool Init();
	bool SetDOBit();
	bool ClearDOBit();

};


#endif // End of #ifndef _C_USB4702_CTRL_H_
