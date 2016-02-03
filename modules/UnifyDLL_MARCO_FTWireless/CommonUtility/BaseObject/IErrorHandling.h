#ifndef _I_ERROR_HANDLING_H_
#define _I_ERROR_HANDLING_H_

#include "../CommonUtil.h"//110323 gilpin add
#include "../CommonDefine.h"
#include "CToolBaseObject.h"
#include "../ErrorCode.h"
#include "../../../Common/ErrorDefine/ErrorDefine.h"
#include "../../../UnifyInterface/FacTestToolInterface.h"


interface IErrorHandling : public CToolBaseObject
{
RF_DECLARE_DYNAMIC(IErrorHandling)


// Data members
public:

protected:
	static std::string m_strResult;
	static std::string m_strErrorCode;
	static std::string m_strMessage;

private:


// Member functions
public:

	IErrorHandling() {}
	virtual ~IErrorHandling() {}

	//void GetErrorCode(CString& cstrErrorCode) { cstrErrorCode = m_strErrorCode.c_str(); }
	bool GetTestResult(int nIndex, st_UIResult* psResult);

	virtual bool SearchPortUsb(const UINT secTimeout) = 0;

protected:

private:

};


#endif // End of #ifndef _I_ERROR_HANDLING_H_
