#include "stdafx.h"
#include "A9_SensorRequest.h"
#include <math.h>


namespace PLATFORM_TOOL
{

//---------Start CA9ReadSensorIDRequest
CA9ReadSensorIDRequest::CA9ReadSensorIDRequest(char* szG, char* szGyro, char* szECompas, char* szPL, bool& bSuccess)
	: CDiagRequestBase(_T("CA9ReadSensorIDRequest"))
	, m_szG(szG)
	, m_szGyro(szGyro)
	, m_szECompas(szECompas)
	, m_szPL(szPL)
	, m_bSuccess(bSuccess)
{
	//sprintf(m_szSensorID, "");
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9ReadSensorIDRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[11];		// Command code
		//BYTE sensor_id[4];
		BYTE g_sensor_id;
		BYTE gyro_sensor_id;
		BYTE ecompas_id;
		BYTE pl_sensor_id;
	} A9_SensorID_Response;
#pragma pack (pop)

	A9_SensorID_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.diag_type == 0x80 )
	{
		m_bSuccess = true;
		sprintf (m_szG, "%u", response.g_sensor_id);
		sprintf (m_szGyro, "%u", response.gyro_sensor_id);
		sprintf (m_szECompas, "%u", response.ecompas_id);
		sprintf (m_szPL, "%u", response.pl_sensor_id);

	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9ReadSensorIDRequest------------//

//---------Start CA9SensorPSensorCalibrationRequest
CA9SensorPSensorCalibrationRequest::CA9SensorPSensorCalibrationRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorPSensorCalibrationRequest"), 60000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x03, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorPSensorCalibrationRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
		BYTE ADC[2];			//ADC
	// Response data
	} A9_PSensorCalibration_Response;
#pragma pack (pop)

	A9_PSensorCalibration_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
	char tmp[3]={0};
	string csRevTmp="";
	if (response.status_type != 0x04)
	{
		//sprintf(m_szResult, "%u", response.result);

		for (int i=0; i<2; i++)
		{
			sprintf(tmp, "%02X", *(response.ADC+i));
			csRevTmp += tmp;

			memset(tmp, '\0', 3);
		}
		BYTE temp = response.ADC[0];
		response.ADC[0] = response.ADC[1];
		response.ADC[1] = temp;

		unsigned short *iADC = (unsigned short *)response.ADC;

		sprintf(m_szResult, "%u; 0x%s (%d)", response.result, csRevTmp.c_str(), *iADC);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorPSensorCalibrationRequest------------//

//---------Start CA9SensorPSensorTestRequest
CA9SensorPSensorTestRequest::CA9SensorPSensorTestRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorPSensorTestRequest"), 20000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x07, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorPSensorTestRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
		BYTE ADC[2];			//ADC
	// Response data
	} A9_PSensorTest_Response;
#pragma pack (pop)

	A9_PSensorTest_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	char tmp[3]={0};
	string csRevTmp="";
	if (response.status_type != 0x04)
	{
		//sprintf(m_szResult, "%u", response.result);

		for (int i=0; i<2; i++)
		{
			sprintf(tmp, "%02X", *(response.ADC+i));
			csRevTmp += tmp;

			memset(tmp, '\0', 3);
		}
		BYTE temp = response.ADC[0];
		response.ADC[0] = response.ADC[1];
		response.ADC[1] = temp;

		unsigned short *iADC = (unsigned short *)response.ADC;

		sprintf(m_szResult, "%u; 0x%s (%d)", response.result, csRevTmp.c_str(), *iADC);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorPSensorTestRequest------------//

//---------Start CA9SensorPSensorFarTestRequest
CA9SensorPSensorFarTestRequest::CA9SensorPSensorFarTestRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorPSensorFarTestRequest"), 20000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;
	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x07, 0x01, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorPSensorFarTestRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
		BYTE ADC[2];			//ADC
	// Response data
	} A9_PSensorFarTest_Response;
#pragma pack (pop)
//08:38:45.750 0x80 40 02 00 00 00 00 00 08 00 01 00 06 00 00 00

	A9_PSensorFarTest_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	char tmp[3]={0};
	string csRevTmp="";
	if (response.status_type != 0x04)
	{
		//sprintf(m_szResult, "%u", response.result);

		for (int i=0; i<2; i++)
		{
			sprintf(tmp, "%02X", *(response.ADC+i));
			csRevTmp += tmp;

			memset(tmp, '\0', 3);
		}
		BYTE temp = response.ADC[0];
		response.ADC[0] = response.ADC[1];
		response.ADC[1] = temp;

		unsigned short *iADC = (unsigned short *)response.ADC;

		sprintf(m_szResult, "%u; 0x%s (%d)", response.result, csRevTmp.c_str(), *iADC);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorPSensorFarTestRequest------------//

//---------Start CA9SensorLSensorCalibrationRequest
CA9SensorLSensorCalibrationRequest::CA9SensorLSensorCalibrationRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorLSensorCalibrationRequest"), 60000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x02, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorLSensorCalibrationRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
		BYTE dump2[2];
		BYTE trans_ratio;
	} A9_LSensorCalibration_Response;
#pragma pack (pop)

	A9_LSensorCalibration_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.status_type != 0x04)
	{
		sprintf(m_szResult, "%u; %u", response.result, response.trans_ratio);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorLSensorCalibrationRequest------------//

//---------Start CA9SensorLSensorTestRequest
CA9SensorLSensorTestRequest::CA9SensorLSensorTestRequest(bool& bSuccess, char* szResult, char* szRange)
	: CDiagRequestBase(_T("CA9SensorLSensorTestRequest"), 30000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{

	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x06, 0x32, 0x00, 0x00};

	BYTE bRange = atoi(szRange);

	szCommand[13] = bRange;

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorLSensorTestRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
		BYTE lux[2];
		BYTE trans_ratio;
	} A9_LSensorTest_Response;
#pragma pack (pop)

	A9_LSensorTest_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
	if (response.status_type != 0x04)
	{
		//sprintf(m_szResult, "%u", response.result);

		BYTE temp = response.lux[0];
		response.lux[0] = response.lux[1];
		response.lux[1] = temp;

		unsigned short *iLux = (unsigned short *)response.lux;

		sprintf(m_szResult, "%u; %d; %u", response.result, *iLux, response.trans_ratio);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorLSensorTestRequest------------//


//---------Start CA9SensorGSensorCalibrationRequest
CA9SensorGSensorCalibrationRequest::CA9SensorGSensorCalibrationRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorGSensorCalibrationRequest"), 60000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x01, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorGSensorCalibrationRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//result pass or fail
	// Response data
	} A9_GSensorCalibration_Response;
#pragma pack (pop)

	A9_GSensorCalibration_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.status_type != 0x04)
	{
		sprintf(m_szResult, "%u", response.result);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorGSensorCalibrationRequest------------//

//---------Start CA9SensorGSensorTestRequest
CA9SensorGSensorTestRequest::CA9SensorGSensorTestRequest(bool& bSuccess, char* szResult)
	: CDiagRequestBase(_T("CA9SensorGSensorTestRequest"), 20000)
	, m_bSuccess(bSuccess)
	, m_szResult(szResult)
{
	m_bSuccess = false;

	BYTE szCommand[] = {0x80, 0x40, 0x02, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x03, 0x21, 0x00, 0x05, 0x00, 0x00, 0x00};

	CBufferPtr ptBufferSend;
	Encode(szCommand, sizeof(szCommand), ptBufferSend);
	SetSendData(ptBufferSend);
}

void CA9SensorGSensorTestRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
{ 
#pragma pack (push, 1)
	typedef struct
	{
		BYTE diag_type;			// Diagnostic type or error code
		BYTE cmd_code[2];		// Command code
		BYTE status_type;		//command Success or fail
		BYTE dump[8];
		BYTE result;			//test pass or fail
		BYTE xyz[3];			//coordinate
	// Response data
	} A9_GSensorTest_Response;
#pragma pack (pop)

	A9_GSensorTest_Response response = {0};
	Safememcopy((void*)&response, sizeof(response), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

	if (response.status_type != 0x04)
	{
		unsigned short iX = (unsigned short) response.xyz[0];
		unsigned short iY = (unsigned short) response.xyz[1];
		unsigned short iZ = (unsigned short) response.xyz[2];

		sprintf(m_szResult, "%u; %d, %d, %d", response.result, iX, iY, iZ);
		m_bSuccess = true;
	}

	CDiagRequestBase::OnDataReceiver(ptBufferReceived);
}
//----------End of CA9SensorGSensorTestRequest------------//

} // End of namespace PLATFORM_TOOL
