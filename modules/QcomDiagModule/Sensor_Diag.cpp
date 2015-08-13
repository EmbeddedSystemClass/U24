#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\SensorRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Sensor_CheckGsensor()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardGSensorRequest(bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (bRes == false)
		PopLastLog();

	return bRes;
}


bool CQcomDiagModule::Sensor_CalibrateGSensor()
{
	using namespace PLATFORM_TOOL;

	bool bRes			= false;

	CRequestPtr ptRequest = new CLeopardGSensorCalibrationRequest(bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Sensor_IsCalibratedGSensor(BYTE& nByteParameter1, BYTE& nByteParameter2, BYTE& nByteParameter3)
{
	using namespace PLATFORM_TOOL;

	bool bRes			= false;

	CRequestPtr ptRequest = new CWolfGSensorIsCalibratedRequest(bRes, nByteParameter1, nByteParameter2, nByteParameter3);
	bRes = (Execute(ptRequest) && bRes);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Sensor_SelfTestGSensor()
{
	using namespace PLATFORM_TOOL;

	bool bRes			= false;
	bool bResExecute	= false;

	CRequestPtr ptRequest = new CWolfGSensorSelfTestRequest(bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Sensor_CheckECompass()
{
	using namespace PLATFORM_TOOL;

	bool bRes;

	CRequestPtr ptRequest = new CLeopardECompassRequest(bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (bRes == false)
		PopLastLog();

	return bRes;
}


bool CQcomDiagModule::Sensor_LPSensor(int nFunction,BYTE& nByteParameter1,BYTE& nByteParameter2,BYTE& nByteParameter3,BYTE& nByteParameter4,int& nValue,bool& bSuccess)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardIOControlRequest(nFunction,nByteParameter1,nByteParameter2,nByteParameter3,nByteParameter4,nValue,bSuccess);
	bRes = Execute(ptRequest);

	if(bRes == false || bSuccess == false)
		PopLastLog();

	return bSuccess;
}


bool CQcomDiagModule::Sensor_LPSensor(int nFunction,DWORD *pdata,int& nValue,bool& bSuccess)
{
	BYTE *pPtr = (BYTE*) pdata;
	return Sensor_LPSensor (nFunction, pPtr[0], pPtr[1], pPtr[2], pPtr[3], nValue, bSuccess);
}

bool CQcomDiagModule::Sensor_GetComponentValue(int nID, int nSubID, std::string& strID, bool& bSuccess)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	if(nID == 40 || nID == 50 || nID == 70)
	{
		CRequestPtr ptRequest = new CGetRFComponentIDRequest(nID,nSubID,strID,bSuccess);
		bRes = Execute(ptRequest);
	}
	else if(nID == 60 || nID == 80)
	{
		CRequestPtr ptRequest = new CGetComponentIDRequest(nID,nSubID,strID,bSuccess);
		bRes = Execute(ptRequest);
	}
	else 
	{
		assert(0);
	}

	if(bRes == false || bSuccess == false)
		PopLastLog();

	return bSuccess;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/
QCOM_DIAG_API bool Sensor_CheckGSensor_SPU(const char* szPort)
{
	CQcomDiagModule diag(::atoi(szPort));
	return diag.Sensor_CheckGsensor();
}


QCOM_DIAG_API bool Sensor_CalibrateGSensor_SPU(const char* szPort)
{
	CQcomDiagModule diag(::atoi(szPort));
	return diag.Sensor_CalibrateGSensor();
}

QCOM_DIAG_API bool Sensor_IsCalibratedGSensor_SPU(const char* szPort, char* szAxis_X, char* szAxis_Y, char* szAxis_Z)
{
	bool bRes = false;
	BYTE nByteAxis_X, nByteAxis_Y, nByteAxis_Z;

	CQcomDiagModule diag(::atoi(szPort));
	bRes = diag.Sensor_IsCalibratedGSensor(nByteAxis_X, nByteAxis_Y, nByteAxis_Z);

	sprintf(szAxis_X, "%d", nByteAxis_X);
	sprintf(szAxis_Y, "%d", nByteAxis_Y);
	sprintf(szAxis_Z, "%d", nByteAxis_Z);

	return bRes;
}

QCOM_DIAG_API bool Sensor_SelfTestGSensor_SPU(const char* szPort)
{
	CQcomDiagModule diag(::atoi(szPort));
	return diag.Sensor_SelfTestGSensor();
}

QCOM_DIAG_API bool Sensor_CheckECompass_SPU(const char* szPort)
{
	CQcomDiagModule diag(::atoi(szPort));
	return diag.Sensor_CheckECompass();
}


QCOM_DIAG_API bool Sensor_LPSensor_SPU(const char* szPort,char* szFunction,char* szPara1,char* szPara2,char* szPara3,char* szValue)
{
	//JY , 
	//CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CQcomDiagModule diag(::atoi(szPort));
	int nValue = 0;
	bool bSuccess = false;

	int func = ::atoi(szFunction);

	if (7==func) //ALS calibration
	{
		WORD wPara1 = ::atoi(szPara1);
		BYTE *pPtr = (BYTE*)&wPara1;
		BYTE dummy1=0, dummy2=0;
		bRes = diag.Sensor_LPSensor (::atoi(szFunction),pPtr[0],pPtr[1],dummy1,dummy2,nValue,bSuccess);
		if (bRes && bSuccess)
		{
			// return value is GA value(float)
			float fVal = *((float*)&nValue);
			::sprintf (szValue, "%f", fVal);
		}
	}
	else if (17==func) //ALS verification
	{
		WORD wPara1 = ::atoi(szPara1);
		BYTE *pPtr = (BYTE*)&wPara1;
		BYTE dummy1=0, dummy2=0;
		bRes = diag.Sensor_LPSensor (::atoi(szFunction),pPtr[0],pPtr[1],dummy1,dummy2,nValue,bSuccess);
	}
	else if (9==func || 10==func || 12==func || 21==func) //use float
	{
		float fVal = (float) atof (szPara1);
		DWORD dwValue = *((DWORD*)&fVal);
		bRes = diag.Sensor_LPSensor(::atoi(szFunction),&dwValue, nValue,bSuccess);

		if (bRes && bSuccess)
		{
			fVal = *((float*)&dwValue);
			::sprintf(szValue,"%f",fVal);
		}
	}
	else if (11==func)
	{
		BYTE data[4] = {0};
		bRes = diag.Sensor_LPSensor(::atoi(szFunction),data[0],data[1],data[2],data[3],nValue,bSuccess);
		if (bRes && bSuccess)
		{
			//b3 b2 b1 b0 (IR data:b0b1 ; C data:b2b3 )

			WORD IRdata = (data[3]<<8) | data[2];
			WORD Cdata = (data[1]<<8) | data[0];
			::sprintf(szPara1,"%d",IRdata);
			::sprintf(szPara2,"%d",Cdata);
		}
	}
	else if (19 == func) //ALS verification3
	{
		WORD wPara1 = ::atoi(szPara1);
		BYTE *pPtr = (BYTE*)&wPara1;
		BYTE dummy1=0, dummy2=0;
		bRes = diag.Sensor_LPSensor (::atoi(szFunction),pPtr[0],pPtr[1],dummy1,dummy2,nValue,bSuccess);
		
		sprintf(szValue, "%d", nValue);
		bRes = bRes & bSuccess;

	}
	else
	{
		BYTE nPara1 = ::atoi(szPara1);
		BYTE nPara2 = ::atoi(szPara2);
		BYTE nPara3 = ::atoi(szPara3);
		BYTE dummy=0;
		bRes = diag.Sensor_LPSensor(::atoi(szFunction),nPara1,nPara2,nPara3,dummy,nValue,bSuccess);

		if(bRes && bSuccess)
		{
			if (6==func)
				::sprintf (szValue, "%d", dummy); // because the script will print this value too.			
			else
				::sprintf(szValue,"%d",nValue);

			::sprintf(szPara1,"%d",nPara1);
			::sprintf(szPara2,"%d",nPara2);
			::sprintf(szPara3,"%d",nPara3);
		}
	}
	return bRes;
}

QCOM_DIAG_API bool Sensor_GetComponentValue_SPU(const char* szPort, const char* szID, const char* szSubID,char* szValue)
{
	bool bSuccess = false;
	std::string strID = ".....................................................................................................................";

	CQcomDiagModule diag(::atoi(szPort));
	bool ret = diag.Sensor_GetComponentValue (atoi(szID), atoi(szSubID), strID, bSuccess);
	if (ret)
		strcpy (szValue, strID.c_str());
	else
		szValue[0] = 0;

	return ret && bSuccess;
}
