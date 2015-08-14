#ifndef _C_SENSOR_ID_H_
#define _C_SENSOR_ID_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CSensorID : public ITestItem
{
GP_DECLARE_DYNCREATE(CSensorID)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;

	BYTE			m_byteEnable;

	std::string		m_strAcceID;
	std::string		m_strGyroID;
	std::string		m_strEcompassID;
	std::string		m_strLightSensorID;
	std::string		m_strCameraSensorID;

	std::string		m_strMeasured;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CSensorID() {}
	~CSensorID() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_SENSOR_ID_H_
