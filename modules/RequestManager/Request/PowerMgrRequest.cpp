#include "stdafx.h"
#include "PowerMgrRequest.h"

namespace PLATFORM_TOOL
{

/*****************************************************************************/
//
//  Gauge IC
//
/*****************************************************************************/
	CGaugeICRequest::CGaugeICRequest(int nType, int& nValue,bool& bSuccess)
		:CDiagRequestBase(_T("CGaugeICRequest"))
		,m_nType(nType)
		,m_bSuccess(bSuccess)
		,m_nValue(nValue)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	    2	Mode ID V set to 14 for Headset Detect
		*/
		BYTE szRAWCommand[] = {75, 36, 16, 0};
		if(nType == 1) //Voltage
			szRAWCommand[2] = 17;

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGaugeICRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V 15 for Ecompass verify
		Result	2	Result V current
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short result;
		} diag_gaugeic_rsp_type;
#pragma pack(pop)
		diag_gaugeic_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_bSuccess = (resp.command_code == 0x4B) && (resp.subsys_id== 36);
		m_nValue = resp.result;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



	CGasGaugeDeviceNameRequest::CGasGaugeDeviceNameRequest (std::string &devName)
		: CDiagRequestBase(_T("CGasGaugeDeviceNameRequest"))
		, m_strDeviceName (devName)
	{
		BYTE szRAWCommand[] = {75, 36, 20, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGasGaugeDeviceNameRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 20 for read gas gauge device name
		Result	1	Success: 1, Fail: 0
		Device Name	32	ASCII (Hex): 07 62 71 32 37 35 30 30  (Actual device name only 8 bytes)
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			BYTE result;
			BYTE  DeviceName[32]; //ASCII
		} diag_gaugeic_rsp_type;
#pragma pack(pop)

		diag_gaugeic_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		bool bSuccess = resp.result == 1;
		if (bSuccess)
		{
			char szBuffer[32] = {0};
			memcpy (szBuffer, resp.DeviceName, 32);
			m_strDeviceName = szBuffer;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



	CGasGaugeICDeviceIDRequest::CGasGaugeICDeviceIDRequest (BYTE& ID)
		: CDiagRequestBase(_T("CGasGaugeICDeviceIDRequest"))
		, m_ChargeIC_ID (ID)
	{
		BYTE szRAWCommand[] = {75, 36, 22, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGasGaugeICDeviceIDRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		0x4B 24 16 00 01 0A
		I2C success: 0x1 Fail: 0x0
		Charger IC ID: 0A  (只要前五個bit數值是0x08，應該就是SMB136，後面3-bit 是silicon revision )

		*/

		BYTE resp[6] = {0};
		Safememcopy((void*)resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		bool bSuccess = resp[4] == 1;
		if (bSuccess)
		{
			m_ChargeIC_ID =  resp[5];
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



/*****************************************************************************/
//
//  Charging mode
//
/*****************************************************************************/

	CChargingStatusRequest::CChargingStatusRequest(bool& bIsCharging,bool& bSuccess)
		:CDiagRequestBase(_T("CChargingStatusRequest"))
		,m_bIsCharging(bIsCharging)
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 10 for Get charging status.
		*/
		BYTE szRAWCommand[] = {0x4B, 36, 10, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CChargingStatusRequest::~CChargingStatusRequest()
	{
	}

	void CChargingStatusRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 10 for Get charging status.
		Result	1	Result V 0: Charging Complete
		1: Charging
		2: No charging
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			BYTE Result;
		} diag_USBChargingStatus_rsp_type;
#pragma pack(pop)
		diag_USBChargingStatus_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.command_code == 0x4B;
		m_bIsCharging = resp.Result == 1;
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


		CChargingSwitchRequest::CChargingSwitchRequest(short Mode_ID,bool bEnable,bool& bSuccess)
		:CDiagRequestBase(_T("CChargingSwitchRequest"))
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 6 for USB charging switch.
		set to 4 for coin cell charging switch.
		Enable/Disable	1	Enable/Disable V Enable: 1  Disable: 0
		*/
		BYTE szRAWCommand[] = {0x4B, 36, 6, 0,1};
		szRAWCommand[2] = (BYTE)Mode_ID;
		if(bEnable)
			szRAWCommand[4] = 1;
		else
			szRAWCommand[4] = 0;

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CChargingSwitchRequest::~CChargingSwitchRequest()
	{
	}

	void CChargingSwitchRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 6 for USB charging switch.
		Result	2	Result V USB charging status has been updated: 1
		USB charging status hasnt been updated: 0 or 1
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Result;
		} diag_USBChargingSwitch_rsp_type;
#pragma pack(pop)
		diag_USBChargingSwitch_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.Result == 0 || resp.Result == 1)  && resp.command_code == 0x4B;
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CChargingSwitchRequestWithVol::CChargingSwitchRequestWithVol(bool bEnable,bool& bSuccess,int iVol)
		:CDiagRequestBase(_T("CChargingSwitchRequestWithVol"))
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 6 for USB charging switch.
		Enable/Disable	1	Enable/Disable V Enable: 1  Disable: 0
		Voltage            0   no voltage 1 100mA  2  500mA
		*/
		BYTE szRAWCommand[] = {0x4B, 36, 6, 0,1,0};
		szRAWCommand[5] = iVol;
		if(bEnable)
			szRAWCommand[4] = 1;
		else
			szRAWCommand[4] = 0;

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CChargingSwitchRequestWithVol::~CChargingSwitchRequestWithVol()
	{
	}

	void CChargingSwitchRequestWithVol::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 6 for USB charging switch.
		Result	2	Result V USB charging status has been updated: 1
		USB charging status hasnt been updated: 0 or 1
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Result;
		} diag_USBChargingSwitch_rsp_type;
#pragma pack(pop)
		diag_USBChargingSwitch_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.Result == 0 || resp.Result == 1)  && resp.command_code == 0x4B;
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CReadBatteryVoltage::CReadBatteryVoltage(int* nVoltage, bool& bSuccess)
		:CDiagRequestBase(_T("CReadBatteryVoltage"), 10000)
		, m_pnVoltage(nVoltage)
		, m_bSuccess(bSuccess)
	{
		CBufferPtr ptBufferSend;
		unsigned char szSendCmd[] = {75, 36, 21, 0};

		Encode(szSendCmd, sizeof(szSendCmd), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CReadBatteryVoltage::~CReadBatteryVoltage() 
	{
	}

	void CReadBatteryVoltage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		// 75, 36, 21, 0, CheckByte, Volt_L, Volt_H
		// The measured voltage value is Volt_L + Volt_H * 256

		char szCmpBuf[] = {0x4b, 0x24, 0x15, 0x00};
		const unsigned char* retVal = (const unsigned char*)ptBufferReceived->GetData();
		if (memcmp(szCmpBuf, retVal, 4) == 0)
		{
			if (retVal[4] == 0)
			{
				*m_pnVoltage = 0;
				m_bSuccess = false;
			}
			else
			{
				*m_pnVoltage = retVal[5] + retVal[6] * 256;
				m_bSuccess = true;
			}
		}
		else
		{
		    *m_pnVoltage = 0;
			m_bSuccess = false;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
    /*
	bool CReadBatteryVoltage::Encode(CBufferPtr& ptBufferSend) 
	{
		// byte 3 is changed form 2 to 21 for Wolf. 20110511
		//unsigned char szSendCmd[0x09] = {0x4b, 0x24, 0x2, 0x00, 0x00, 0x00, 0x8c, 0xac, 0x7e};
		unsigned char szSendCmd[] = {0x4b, 0x24, 21, 0};
		Encode (ptBuffer
		ptBufferSend = new CBuffer(0x09, (const char*)szSendCmd);
		return true;
	}
	*/

	bool CReadBatteryVoltage::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}



/*****************************************************************************/
//
//  Coin Cell
//
/*****************************************************************************/

	CReadCoincellVoltageRequest::CReadCoincellVoltageRequest(int& nVoltage,bool& bSuccess)
		:CDiagRequestBase(_T("CReadCoincellVoltageRequest"))
		,m_bSuccess(bSuccess)
		,m_nVoltage(nVoltage)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 11 for read coin cell voltage.
		*/
		BYTE szRAWCommand[] = {75, 36, 11, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CReadCoincellVoltageRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID V set to 11 for read coin cell voltage.
		Result	2	Coin Cell Voltage in mV.
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short RESULT;
		} diag_coincell_rsp_type;
#pragma pack(pop)
		diag_coincell_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_nVoltage = resp.RESULT;
		m_bSuccess = (resp.RESULT >= 0 && resp.command_code == 0x4B);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	/*****************************************************************************/
	//
	//  key event
	//
	/*****************************************************************************/
	CPowerKeyEventRequest::CPowerKeyEventRequest()
		:CDiagRequestBase(_T("CPowerKeyEventRequest"), 0, false)
	{
		CBufferPtr ptBufferSend;
		unsigned char szSendCmd[] = {0x4B, 0xFA, 0x01, 0x00};

		Encode(szSendCmd, sizeof(szSendCmd), ptBufferSend);
		SetSendData(ptBufferSend);
	}
} //PLATFORM_TOOL