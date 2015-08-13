#include "stdafx.h"
#include "SensorRequest.h"


namespace PLATFORM_TOOL
{

	CLeopardGSensorRequest::CLeopardGSensorRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGSensorRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 60 0 1 0 0 0
		//response: 75 250 60 0 1 0 * 0 --> 1 =success

		
		CBufferPtr ptBufferSend;
		BYTE req[8] = {75, 250, 60, 0, 1, 0, 0, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardGSensorRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[8] = {0};
		Safememcopy((void*)&response, 8,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[6] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopardGSensorCalibrationRequest::CLeopardGSensorCalibrationRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGSensorCalibrationRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 62 00
		//response: 75 250 62 00 01 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 62, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardGSensorCalibrationRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[6] = {0};
		Safememcopy((void*)&response, 6,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[4] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfGSensorIsCalibratedRequest::CWolfGSensorIsCalibratedRequest(bool &bSuccess, BYTE& nByteParameter1, BYTE& nByteParameter2, BYTE& nByteParameter3)
		: CDiagRequestBase (_T("CWolfGSensorIsCalibratedRequest"))
		,m_bSuccess (bSuccess)
		,m_nByteParameter1(nByteParameter1)
		,m_nByteParameter2(nByteParameter2)
		,m_nByteParameter3(nByteParameter3)
	{
		//request:  75 250 63 0
		//response: 75 250 63 0 * X 0 0 0 0 0 0 0 0 0 0 0 Y 0 0 0 0 0 0 0 0 0 0 0 Z 0 0 0 0 0 0 0 0 0 0 0
		//          * => 1 = success; X(Y,Z) : offset of X(Y,Z) axis
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 63, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfGSensorIsCalibratedRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[41] = {0};
		Safememcopy((void*)&response, 41,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[4] == 1);
		
		m_nByteParameter1 = response[5];
		m_nByteParameter2 = response[17];
		m_nByteParameter3 = response[29];

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfGSensorSelfTestRequest::CWolfGSensorSelfTestRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CWolfGSensorSelfTestRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 65 0
		//response: 75 250 65 0 * 0 --> 1 =success
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 65, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfGSensorSelfTestRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[6] = {0};
		Safememcopy((void*)&response, 6,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[4] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CLeopardECompassRequest::CLeopardECompassRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardGSensorRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 60 0 2 0 0 0
		//response: 75 250 60 0 2 0 * 0 --> 1 =success

		
		CBufferPtr ptBufferSend;
		BYTE req[8] = {75, 250, 60, 0, 1, 0, 0, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardECompassRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[8] = {0};
		Safememcopy((void*)&response, 8,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[6] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CLeopardIOControlRequest::CLeopardIOControlRequest(int nFunction,BYTE& nByteParameter1,BYTE& nByteParameter2,BYTE& nByteParameter3,BYTE& nByteParameter4,int& nValue,bool& bSuccess)
		:CDiagRequestBase(_T("CLeopardIOControlRequest"), 30000)
		,m_bSuccess(bSuccess)
		,m_nValue(nValue)
		,m_nFunction(nFunction)
		,m_nByteParameter1(nByteParameter1)
		,m_nByteParameter2(nByteParameter2)
		,m_nByteParameter3(nByteParameter3)
		,m_nByteParameter4(nByteParameter4)
	{
		/*
		nFunction
		1  Set ALS
		2  Get ALS
		3  Set PRX
		4  Get PRX
		5  Get ALS parameter
		6  Get PRX parameter
		7  ALS calibration
		8  P-sensor calibration
		9  Get ALS GA value
		10 Set ALS GA value
		11 Get ALS_IRC data

		15 Enable Sensor // call this before any IOControl to enable sensor
		16 Disable Sensor // call this after any IOControl to disable sensor
		17 ALS Verification cmd (input ALS calibration LUX value)
		*/

		// 75 250 61 

#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Sensor_ID;
			BYTE command_ID;
			BYTE para1;
			BYTE para2;
			BYTE para3;
			short status;
		} diag_leopard_set_IO_req;

		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Sensor_ID;
			BYTE command_ID;
			DWORD data;
			short status;
		} diag_leopard_get_IO_req;
#pragma pack(pop)

		CBufferPtr ptBufferSend;
		if(m_nFunction == 1 || m_nFunction == 3 || m_nFunction==7 || m_nFunction==17 || m_nFunction==19 || m_nFunction==20)
		{
			diag_leopard_set_IO_req setReq = {75,250,61,0,m_nFunction,m_nByteParameter1,m_nByteParameter2,m_nByteParameter3,0};
			Encode((BYTE*)&setReq,12,ptBufferSend);
		}
		else if(m_nFunction == 2 || m_nFunction == 4 || m_nFunction == 5 || m_nFunction == 6 || 
				m_nFunction == 8 || m_nFunction==11 ||
				m_nFunction == 15 || m_nFunction == 16)
		{
			diag_leopard_get_IO_req getReq = {75,250,61,0,m_nFunction,0,0};
			Encode((BYTE*)&getReq,13,ptBufferSend);
		}
		else if (m_nFunction==9 || m_nFunction==10 || m_nFunction==12 || m_nFunction==21)
		{
			DWORD data;
			BYTE* pPtr = (BYTE*)&data;
			pPtr[0] = nByteParameter1;
			pPtr[1] = nByteParameter2;
			pPtr[2] = nByteParameter3;
			pPtr[3] = nByteParameter4;

			diag_leopard_get_IO_req getReq = {75,250,61,0,m_nFunction,data,0};
			Encode((BYTE*)&getReq,13,ptBufferSend);
		}
		else
			assert(0);

		SetSendData(ptBufferSend);
	}


	void CLeopardIOControlRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Sensor_ID;
			short command_ID;
			DWORD data;
			short status;
		} diag_leopard_IO_rsp;

		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Sensor_ID;
			short command_ID;
			BYTE data1;
			BYTE data2;
			BYTE data3;
			BYTE data4;
			short status;
		} diag_leopard_IO_2_rsp;

		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Sensor_ID;
			short command_ID;
			BYTE data1;
			BYTE data2;
			BYTE data3;
			BYTE data4;
			short status;
		} diag_leopard_IO_3_rsp;
#pragma pack(pop)

		diag_leopard_IO_rsp resp = {0};

		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_nValue = resp.data;
		m_bSuccess = (resp.status == 1);
		assert(m_bSuccess);
		if(m_nFunction == 5 || m_nFunction == 6 || m_nFunction==20 || m_nFunction==21)
		{
			diag_leopard_IO_2_rsp resp = {0};
			Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

			m_nByteParameter1 = resp.data1;
			m_nByteParameter2 = resp.data2;
			m_nByteParameter3 = resp.data3;
			m_nByteParameter4 = resp.data4;
		}
		else if (m_nFunction==9 || m_nFunction==10 || m_nFunction==11)
		{
			BYTE *pPtr = (BYTE*)&resp.data;
			m_nByteParameter1 = pPtr[0];
			m_nByteParameter2 = pPtr[1];
			m_nByteParameter3 = pPtr[2];
			m_nByteParameter4 = pPtr[3];
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CGSensorRequest::CGSensorRequest(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bSuccess(bSuccess)
		,m_nX(nOffsetX)
		,m_nY(nOffsetY)
		,m_nZ(nOffsetZ)
		,m_bIsGSensorWork(bSuccess)//Unused
		,m_bIsUpWork(bSuccess)//Unused
		,m_nVersion(4)
	{
		BYTE szBuf[4]={0x4B,0xF5,0x05,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::CGSensorRequest(int nFunction,int& nTemp, bool& bSuccess)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bSuccess(bSuccess)
		,m_nX(nTemp)
		,m_nY(nTemp)
		,m_nZ(nTemp)
		,m_bIsGSensorWork(bSuccess)//Unused
		,m_bIsUpWork(bSuccess)//Unused
		,m_nVersion(3)
	{
		BYTE szBuf[4]={0x4B,0xF5,nFunction,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::CGSensorRequest(int& nX,int& nY,int& nZ,  bool& bSuccess)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bSuccess(bSuccess)
		,m_nX(nX)
		,m_nY(nY)
		,m_nZ(nZ)
		,m_bIsGSensorWork(bSuccess)//Unused
		,m_bIsUpWork(bSuccess)//Unused
		,m_nVersion(2)
	{
		m_nX = 0;
		m_nY = 0;
		m_nZ = 0;
		m_bSuccess = false;

		BYTE szBuf[4]={0x4B,0xF5,0x02,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	//------------------------------------------------------------------------------//	
	CGSensorRequest::CGSensorRequest(bool& bIsGSensorWork,bool& bIsUpWork,int& nTemp)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bIsGSensorWork(bIsGSensorWork)
		,m_bIsUpWork(bIsUpWork)
		,m_bSuccess(bIsUpWork)//Unused
		,m_nX(nTemp)//Unused
		,m_nY(nTemp)//Unused
		,m_nZ(nTemp)//Unused
		,m_nVersion(1)
	{
		m_bIsGSensorWork = false;
		m_bIsUpWork = false;

		BYTE szBuf[4]={0x4B,0xF5,0x01,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::~CGSensorRequest()
	{
	}

	void CGSensorRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if(m_nVersion == 1)
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x01
				BYTE subsys_id2;//0x00
				BYTE gsensor_id;  
				BYTE uP_id;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)

			diag_get_gsensor_id_rsp_type resp = {0};
			Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
			m_bIsGSensorWork = resp.gsensor_id == 0x02;
			m_bIsUpWork = resp.uP_id == 0x01;
		}
		else if(m_nVersion == 2)
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x02
				BYTE subsys_id2;//0x00
				int nX; 
				int nY;
				int nZ;
				BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)

			diag_get_gsensor_id_rsp_type resp = {0};
			memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));

			m_nX = resp.nX;
			m_nY = resp.nY;
			m_nZ = resp.nZ;
			m_bSuccess = (resp.byteSuccess == 1 && resp.command_code == 0x4B);
		}
		else if(m_nVersion == 3)
		{			
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x02
				BYTE subsys_id2;//0x00
				BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)
			diag_get_gsensor_id_rsp_type resp = {0};
			if(ptBufferReceived->GetLength() >= 5)
				memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));

			if(resp.command_code == 0x4b && resp.subsys_id == 0xF5)
			{
				m_nX = resp.byteSuccess;
				switch(resp.subsys_id1)
				{
				case 3:
					/*
					DIAG_CAL_GSENSOR_OK	    0
					DIAG_CAL_GSENSOR_SKIP	1
					DIAG_CAL_GSENSOR_FAIL	2
					*/
					m_bSuccess = (m_nX == 0 || m_nX == 1);
					break;
				case 6:
					/*
					DIAG_ST_GSENSOR_OK	0
					DIAG_ST_GSENSOR_ST0_FAIL	1
					DIAG_ST_GSENSOR_ST1_FAIL	2
					DIAG_ST_GSENSOR_VECTOR_ST_FAIL	3
					*/
					m_bSuccess = (m_nX == 0);
					break;
				case 7:
					/*
					DIAG_GSENSOR_TEST_ALL_PASS	0
					DIAG_GSENSOR_TEST_ST0_FAIL	1
					DIAG_GSENSOR_TEST_ST1_FAIL	2
					DIAG_GSENSOR_TEST_VECTOR_ST_FAIL	3
					DIAG_GSENSOR_TEST_CAL_FAIL	4
					*/
					m_bSuccess = (m_nX == 0);
					break;
				};
			}
		}
		else if(m_nVersion == 4)
		{			
#pragma pack(push, 1)
			/*	typedef struct  
			{
			BYTE command_code;
			BYTE subsys_id;
			BYTE subsys_id1;//0x05
			BYTE subsys_id2;//0x00
			short nOffsetX;
			short nOffsetY;
			short nOffsetZ;
			BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;*/  
			//sunlight modify
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x05
				BYTE subsys_id2;//0x00
				BYTE byteSuccess;
				BYTE X3X4;
				BYTE X1X2;
				BYTE Y3Y4;
				BYTE Y1Y2;
				BYTE Z3Z4;
				BYTE Z1Z2;			
			} diag_get_gsensor_id_rsp_type;

#pragma pack(pop)
			diag_get_gsensor_id_rsp_type resp = {0};
			if(ptBufferReceived->GetLength() >= 5)
			{
				memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));
				/*				m_nX = resp.nOffsetX;
				m_nY = resp.nOffsetY;
				m_nZ = resp.nOffsetZ;*/
				m_nX=resp.X1X2*256+resp.X3X4;
				m_nY=resp.Y1Y2*256+resp.Y3Y4;
				m_nZ=resp.Z1Z2*256+resp.Z3Z4;

				m_bSuccess = (resp.byteSuccess == 0 && resp.command_code == 0x4B);
			}
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CGetRFComponentIDRequest::CGetRFComponentIDRequest(int nID,int nSubID,std::string& strID,bool& bSuccess)
		:CDiagRequestBase(_T("CGetRFComponentIDRequest"))
		,m_nID(nID)
		,m_nSubID(nSubID)
		,m_strID(strID)
		,m_bSuccess(bSuccess)
	{
		//m_nID : 40 WLAN address
		//m_nID : 50 BT&FM ID
		//m_nID : 70 Camera ID

#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short nCommandCode;
			short nDeviceType;
			BYTE data[16];
			short result;
		} diag_Get_RF_Component_REQ;
#pragma pack(pop)

		diag_Get_RF_Component_REQ Req = {0};
		Req.command_code = 75;
		Req.subsys_id = 250;
		Req.nCommandCode = m_nID;
		Req.nDeviceType = m_nSubID;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&Req, sizeof(diag_Get_RF_Component_REQ), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGetRFComponentIDRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short nCommandCode;
			short nDeviceType;
			BYTE data[16];
			short result;
		} diag_Get_RF_Component_rsp_type;
#pragma pack(pop)

		diag_Get_RF_Component_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if(m_nID == 40 || m_nID == 70 || m_nID == 50)
		{
			char szBuffer[20] = {0};
			memcpy(szBuffer,resp.data,16);
			m_bSuccess = resp.command_code == 75 && resp.subsys_id == 250 && resp.result == 1;
			if(m_bSuccess)
				m_strID = szBuffer;
		}

		assert(m_bSuccess);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CGetComponentIDRequest::CGetComponentIDRequest(int nID,int nSubID,std::string& strID,bool& bSuccess)
		:CDiagRequestBase(_T("CGetComponentIDRequest"))
		,m_nID(nID)
		,m_nSubID(nSubID)
		,m_strID(strID)
		,m_bSuccess(bSuccess)
	{		
		//m_nID : 80 Touch Pad ID
		//m_nID : 60 Sensor ID
		//m_nSubID : 0 GSensor ID
		//m_nSubID : 1 Compass ID
		//m_nSubID : 2 P/LAS ID

#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short nCommandCode;
			short nDeviceType;
			short result;
		} diag_Get_Component_REQ;
#pragma pack(pop)

		diag_Get_Component_REQ Req = {0};
		Req.command_code = 75;
		Req.subsys_id = 250;
		Req.nCommandCode = m_nID;
		Req.nDeviceType = m_nSubID;

		CBufferPtr ptBufferSend;
		Encode((BYTE*)&Req,sizeof(diag_Get_Component_REQ),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGetComponentIDRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short nCommandCode;
			short nDeviceType;
			short result;
		} diag_Get_Component_RSP;
#pragma pack(pop)

		//Sendor
		//result 0 pass
		//result 1 fail

		//Touch
		//result 0 pass
		//result 1 HW_ERROR
		//result 2 I2C_ERROR
		//result 4 IRQ_ERROR
		//result 8 UNKNOW

		diag_Get_Component_RSP resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if(m_nID == 80)
		{
			m_bSuccess =  resp.command_code == 75 && resp.subsys_id == 250 &&  0 == (resp.result & 0x0003);
		}
		else if(m_nID == 60)
		{
			m_bSuccess =  resp.command_code == 75 && resp.subsys_id == 250 &&  1 == resp.result;
		}

		assert(m_bSuccess);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}











}