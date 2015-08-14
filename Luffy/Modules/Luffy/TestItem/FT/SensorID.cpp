#include "stdafx.h"
#include "SensorID.h"
#include <math.h>


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CSensorID)


bool CSensorID::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "Sensor Test", "", "", "", "", "", "", "", "Success to Sensor Test");

		SetPICSData("SensorTestResult", "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_GSensor_ID_Fail;

		FactoryLog(false, "Sensor Test", m_strErrCode, "", "", "", "", "", "", m_strMsg);

		SetPICSData("SensorTestResult", "FAIL");
	}

	return bRes;
}

bool CSensorID::MainFunction()
{
	char szInputCmd[4096] = {0};
	memset(szInputCmd, 0, sizeof szInputCmd);

	char chReadAcceID[512] = {0};
	memset(chReadAcceID, 0, sizeof chReadAcceID);

	char chReadGyroID[512] = {0};
	memset(chReadGyroID, 0, sizeof chReadGyroID);

	char chReadEcompassID[512] = {0};
	memset(chReadEcompassID, 0, sizeof chReadEcompassID);

	char chReadLightSensorID[512] = {0};
	memset(chReadLightSensorID, 0, sizeof chReadLightSensorID);

	char chReadCameraSensorID[512] = {0};
	memset(chReadCameraSensorID, 0, sizeof chReadCameraSensorID);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		return false;
	}

	if (m_byteEnable & 0x01) // For AcceID
	{
		sprintf_s(szInputCmd, 4096, "%s", "18;1;0;0");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadAcceID, sizeof(chReadAcceID)))
		{
			m_strErrCode = FunErr_GSensor_ID_Fail;

			m_strMsg = "Fail to get AcceID with FTD";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMeasured = chReadAcceID;
			SetPICSData("AccelID", m_strMeasured);

			TraceLog( MakeSTDstring("AccelID: %s", chReadAcceID) );

			if ( m_strMeasured != m_strAcceID)
			{
				m_strErrCode = FunErr_GSensor_ID_Fail;

				m_strMsg = MakeSTDstring("Fail:ReadAcceID is not match, target is %s ", m_strAcceID.c_str());
				TraceLog(m_strMsg);
				return false;
			}
		}
	}
	

	if (m_byteEnable & 0x02) // For GyroID
	{
		sprintf_s(szInputCmd, 4096, "%s", "18;16;0;0");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadGyroID, sizeof(chReadGyroID)))
		{
			m_strErrCode = FunErr_Gyro_ID_Fail;

			m_strMsg = "Fail to get GyroID with FTD";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strErrCode = FunErr_Gyro_ID_Fail;

			m_strMeasured = chReadGyroID;
			SetPICSData("GyroID", m_strMeasured);

			TraceLog( MakeSTDstring("GyroID: %s", chReadGyroID) );

			if ( m_strMeasured != m_strGyroID)
			{
				m_strMsg = MakeSTDstring("Fail:ReadGyroID is not match, target is %s ", m_strGyroID.c_str());
				TraceLog(m_strMsg);
				return false;
			}
		}
	}
	
	if (m_byteEnable & 0x04) // For EcompassID
	{
		sprintf_s(szInputCmd, 4096, "%s", "18;5;0;0");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadEcompassID, sizeof(chReadEcompassID)))
		{
			m_strErrCode = FunErr_ECompassr_Test_Fail;

			m_strMsg = "Fail to get EcompassID with FTD";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMeasured = chReadEcompassID;
			SetPICSData("EcompassID", m_strMeasured);

			TraceLog( MakeSTDstring("EcompassID: %s", chReadEcompassID) );

			if ( m_strMeasured != m_strEcompassID)
			{
				m_strErrCode = FunErr_ECompassr_Test_Fail;

				m_strMsg = MakeSTDstring("Fail:EcompassID is not match, target is %s ", m_strEcompassID.c_str());
				TraceLog(m_strMsg);
				return false;
			}
		}
	}

	if (m_byteEnable & 0x08) // For LightSensorID
	{
		sprintf_s(szInputCmd, 4096, "%s", "18;9;0;0");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadLightSensorID, sizeof(chReadLightSensorID)))
		{
			m_strErrCode = FunErr_LSensor_Check_ID_Fail;

			m_strMsg = "Fail to get LightSensorID with FTD";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strErrCode = FunErr_LSensor_Check_ID_Fail;

			m_strMeasured = chReadLightSensorID;
			SetPICSData("LightSensorID", m_strMeasured);

			TraceLog( MakeSTDstring("LightSensorID: %s", chReadLightSensorID) );

			if ( m_strMeasured != m_strLightSensorID)
			{
				m_strMsg = MakeSTDstring("Fail:LightSensorID is not match, target is %s ", m_strLightSensorID.c_str());
				TraceLog(m_strMsg);
				return false;
			}
		}
	}


	if (m_byteEnable & 0x10) // For CameraSensorID
	{
		sprintf_s(szInputCmd, 4096, "%s", "6;7;0;0");
		if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadCameraSensorID, sizeof(chReadCameraSensorID)))
		{
			m_strErrCode = FunErr_LSensor_Check_ID_Fail;

			m_strMsg = "Fail to get CameraSensorID with FTD";
			TraceLog(m_strMsg);
			return false;
		}
		else
		{
			m_strMeasured = chReadCameraSensorID;
			SetPICSData("CameraSensorID", m_strMeasured);

			TraceLog( MakeSTDstring("CameraSensorID: %s", chReadCameraSensorID) );

			if ( m_strMeasured != m_strCameraSensorID)
			{
				m_strErrCode = FunErr_LSensor_Check_ID_Fail;

				m_strMsg = MakeSTDstring("Fail:CameraSensorID is not match, target is %s ", m_strCameraSensorID.c_str());
				TraceLog(m_strMsg);
				return false;
			}
		}
	}

	return true;
}


bool CSensorID::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSpeakerTest");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	std::string strSensorEnableList;
	std::vector<std::string> vTmp;
	strSensorEnableList =  myParm_node.child("SensorEnableList").text().as_string(ERROR_STRING);
	CHECK_PARM("SensorEnableList", strSensorEnableList, ERROR_STRING);
	STDStringTokenizer(strSensorEnableList, ",", vTmp);
	int iSum = 0;
	for (unsigned int icount = 0; icount < vTmp.size(); icount++)
	{
		if (atoi(vTmp[icount].c_str()) == 1)
			iSum += (int)pow(2.0, (int)icount);
	}
	m_byteEnable = (BYTE)iSum;

	m_strAcceID =  myParm_node.child("AccelerometerID").text().as_string(ERROR_STRING);
	CHECK_PARM("AccelerometerID", m_strAcceID, ERROR_STRING);

	m_strGyroID =  myParm_node.child("GyroID").text().as_string(ERROR_STRING);
	CHECK_PARM("GyroID", m_strGyroID, ERROR_STRING);

	m_strEcompassID =  myParm_node.child("EcompassID").text().as_string(ERROR_STRING);
	CHECK_PARM("EcompassID", m_strEcompassID, ERROR_STRING);

	m_strLightSensorID =  myParm_node.child("LightSensorID").text().as_string(ERROR_STRING);
	CHECK_PARM("LightSensorID", m_strLightSensorID, ERROR_STRING);

	m_strCameraSensorID =  myParm_node.child("CameraSensorID").text().as_string(ERROR_STRING);
	CHECK_PARM("CameraSensorID", m_strCameraSensorID, ERROR_STRING);

    return true ;
}
