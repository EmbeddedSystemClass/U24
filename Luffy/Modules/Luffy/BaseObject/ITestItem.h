#ifndef _I_TEST_ITEM_H_
#define _I_TEST_ITEM_H_

#include <sstream>
#include "CToolBaseObject.h"
#include "BaseObject/ITestProcessor.h"

#define CHECK_PARM( NODE_NAME , NODE_VAL , FAIL_VAL)													\
{																										\
	std::ostringstream out;																				\
	if(FAIL_VAL == NODE_VAL )																			\
	{																									\
		out << "<" << NODE_NAME  << "> for " << m_NickName ;											\
		TraceLog ( out.str() );																			\
		return false;																					\
	}																									\
	else																								\
	{																									\
		out << "<" << m_NickName  << "><"<< NODE_NAME << "> = " << NODE_VAL ;							\
		TraceLog ( out.str() );																			\
	}																									\
}																			

interface ITestItem  : public CToolBaseObject
{
GP_DECLARE_DYNAMIC(ITestItem)

// Data members
public:
	ITestProcessor *m_pIProc;
	std::string m_NickName;
protected:
private:


// Member functions
public:
	ITestItem();
	virtual ~ITestItem() {}

	/* Init data */
	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;

	/* Test sequence */
	virtual bool Begin() { return true; }
	virtual bool PreRun() { return true; }
	virtual bool Run(int iItem = 0) = 0;
	virtual bool PostRun() { return true; }
	virtual bool Stop() { return true; }
	virtual bool End() { return true; }

	virtual bool Initialization() { return true; }


	void TraceLog(std::string strMsg);
	void FactoryLog(bool bPass, std::string strMsg);
	void FactoryLog(bool bPass,
		std::string strItemCode,
		std::string strErrCode,
		std::string strBand,
		std::string strChannel,
		std::string strLower,
		std::string strUpper,
		std::string strMeasured,
		std::string strUnit,
		std::string strMsg
		);

	// UI control functions
	void SetPICSData(std::string strKey, std::string strValue);
	void SetUIMessage(std::string strMsg);
	void SetUIProgress(int iMainProgress, int iSubProgress);
	void CPKLog( const char* szTech, const char* szItemName, const char* szBandClass, const char* szChannel, const char* szIdentifier1, const char* szIdentifier2, const char* szLowerSpec, const char* szUpperSpec, const char* szMeasValue, bool bPass, int iSlot );
	void SetUiFailMsg(std::string strMsg);


	/* Public info */
	int GetCurrentSlot();
	p_fn_processor_callback GetProcessorCallback();

protected:


private:
};


#endif // End of #ifndef _I_TEST_ITEM_H_
