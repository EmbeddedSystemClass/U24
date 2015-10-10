#include "stdafx.h"
#include "ITestProcessor.h"
#include <direct.h>

std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapTxCableLossData;
std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapRxCableLossData;
std::string ITestProcessor::g_strPicasso;
std::string ITestProcessor::g_strScalarID;
CString ITestProcessor::m_csArrPicasso[5] = {"", "", "", "", ""};
std::string ITestProcessor::g_str_modelName;
std::string ITestProcessor::g_str_station;
std::string ITestProcessor::g_str_SWVersion;
std::string ITestProcessor::g_str_FactoryVersion;
std::string ITestProcessor::g_str_HWVersion;
std::string ITestProcessor::g_strIMEI;//LION 2012.4.25

std::map<std::string, std::string> ITestProcessor::m_map_parameterListSeverCheck; 

IPowerSupply* ITestProcessor::m_pIPS = NULL;
IPowerSupply* ITestProcessor::m_pIPS2 = NULL;
IPowerSupply* ITestProcessor::m_pIPS3 = NULL;
ITesterDevice* ITestProcessor::m_pITesterDevice = NULL;
IPhone* ITestProcessor::m_pIPhone = NULL;

std::string ITestProcessor::m_strResult;
std::string ITestProcessor::m_strItemCode;
std::string ITestProcessor::m_strErrorCode;
std::string ITestProcessor::m_strBand;
std::string ITestProcessor::m_strChannel;
std::string ITestProcessor::m_strUpper;
std::string ITestProcessor::m_strLower;
std::string ITestProcessor::m_strMeasured;
std::string ITestProcessor::m_strUnit;
std::string ITestProcessor::m_strMessage;

/* CPhone parameters */
int ITestProcessor::m_nCOMPort = 3;
int ITestProcessor::m_nFtdPort = 2000;
int ITestProcessor::m_nFtdTimeOut = 15000;
bool ITestProcessor::m_b_AutoFTDMode = false;
bool ITestProcessor::m_b_SetFTDMode = false;
int ITestProcessor::m_i_WaitTimeStartingUp = 1000;	//Default: 1s
int ITestProcessor::m_i_RebootDelayTime = 1000;		//Default: 1s
int ITestProcessor::m_i_diagCmdFlag = 0;
bool ITestProcessor::m_b_IsNeedQXDMLog = false;
bool ITestProcessor::m_b_AdbShellStart = false;
int ITestProcessor::m_i_QPhoneMultiCtrl = 0;
int ITestProcessor::m_nQPhoneOrderSlot[5] = {0, 1, 2, 3, 4};
bool ITestProcessor::m_b_LogUpload = false;
std::string ITestProcessor::m_str_UploadServerIP = _T("10.85.68.13");
bool ITestProcessor::m_b_jigRequest = false;
bool ITestProcessor::m_b_waitFlag = false;
bool ITestProcessor::m_b_UsableSlot[5] = {false, false, false, false, false}; // [0] dont use
bool ITestProcessor::m_b_isSlotPass[5] = {false, false, false, false, false}; // [0] dont use

/* USB4702 Parameters */
bool ITestProcessor::m_b_IsUsed4702 = false;

/* Show dialog action result */
bool ITestProcessor::m_b_DialogResponse = false;


RF_IMPLEMENT_DYNAMIC(ITestProcessor)


ITestProcessor::ITestProcessor()
	: m_iGPIBCardAddress(0)
	, m_iGPIBInterfaceID(0)
	, m_iPSCount(0)
{
	m_b_PS1_OnOff = false;
	m_b_PS2_OnOff = false;

	DefineNotify(TRACE_LOG);
	DefineNotify(FACTORY_LOG);
	DefineNotify(PICS_DATA);
	DefineNotify(SHOW_PICTURE);
	DefineNotify(JIG_REQUEST);
	DefineNotify(SHOW_DIALOG);
	DefineNotify(UI_RELAYBOARD);
}


// ---------------------- Test flow functions --------------------
bool ITestProcessor::GetDeviceName(char* szName, int iSize)
{
	if (!ParsePhoneParams())
		return false;

	if (m_mapPhoneParams.find(std::string("COMPort")) == m_mapPhoneParams.end())
		return false;

	if (sprintf_s(szName, iSize - 1, "%s", m_mapPhoneParams[std::string("COMPort")].c_str()) == -1)
		return false;

	return true;
}

bool ITestProcessor::SetPicasso(const char* szPicasso)
{
	if (szPicasso != NULL)
	{
		g_strPicasso = szPicasso;
		return true;
	}

	return false;
}

bool ITestProcessor::SetPicassoMulti(const char* szPicasso, const int nSlot)
{
	if (szPicasso != NULL)
	{
		m_csArrPicasso[nSlot].Format(_T("%s"), szPicasso);
		return true;
	}

	return false;
}

bool ITestProcessor::SetSlotUsable(const char* szUsable, const int nSlot)
{
	if (szUsable != NULL)
	{
		if (strcmp(szUsable, "0") == 0)
			m_b_UsableSlot[nSlot] = false;
		return true;
	}
	return false;
}

bool ITestProcessor::SetIMEI(const char* szIMEI)
{
	if (szIMEI != NULL)
	{
		g_strIMEI = szIMEI;
		return true;
	}

	return false;
}

bool ITestProcessor::SetModelName(const char* p_sz_modelName)
{
	if (p_sz_modelName != NULL)
	{
		g_str_modelName = p_sz_modelName;
		return true;
	}

	return false;
}

bool ITestProcessor::SetStation(const char* p_sz_station)
{
	if (p_sz_station != NULL)
	{
		g_str_station = p_sz_station;
		return true;
	}

	return false;
}

bool ITestProcessor::SetSWVersion(const char* p_sz_SWVersion)
{
	if (p_sz_SWVersion != NULL)
	{
		g_str_SWVersion = p_sz_SWVersion;
		return true;
	}

	return false;
}

bool ITestProcessor::GetSWVersion(char* p_sz_SWVersion)
{
	strcpy(p_sz_SWVersion, g_str_SWVersion.c_str());
	return true;
}

bool ITestProcessor::SetFactoryVersion(const char* p_sz_FactoryVersion)
{
	if (p_sz_FactoryVersion != NULL)
	{
		g_str_FactoryVersion = p_sz_FactoryVersion;
		return true;
	}

	return false;
}

bool ITestProcessor::GetFactoryVersion(char* p_sz_FactoryVersion)
{
	strcpy(p_sz_FactoryVersion, g_str_FactoryVersion.c_str());
	return true;
}

bool ITestProcessor::SetHWVersion(const char* p_sz_HWVersion)
{
	if (p_sz_HWVersion != NULL)
	{
		g_str_HWVersion = p_sz_HWVersion;
		return true;
	}
	return false;
}

bool ITestProcessor::SetDialogResponse(bool b_Resopnse)
{
	m_b_waitFlag = true;
	m_b_DialogResponse = b_Resopnse;
	return true;
}

bool ITestProcessor::GPIBDeviceCreateObject()
{
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strLower = "";
	m_strUpper = "";
	m_strUnit = "";
	m_strMeasured = "";
	m_strMessage = "";

	if (m_pITesterDevice == NULL)  
	{
		// 1. Parse XML for GPIB instrument name and class name
		if (!ParseGPIBDeviceParams())
		{
			m_strMessage = "Fail to parse XML for GPIB devices parameters";
			AfxMessageBox( m_strMessage.c_str() );
			return false;
		}

		// 2. Detect GPIB instruments
		if (!DetectGPIBDevice())
		{
			m_strMessage = "Fail to detect GPIB devices";
			//AfxMessageBox( m_strMessage.c_str() );
			//return false;//EASON
		}
		// 3. Create tester device object & power supply object
		std::vector<std::string> vDeviceList;
		if (!IGPIBDevice::GetDeviceList(vDeviceList))
		{
			m_strMessage = "Fail to get GPIB device list";
			AfxMessageBox( m_strMessage.c_str() );
			return false;
		}

		for (size_t i = 0; i < vDeviceList.size(); ++i)
		{
			for (std::map<std::string, std::string>::iterator itr = m_mapTesterDeviceParams.begin();
			     itr != m_mapTesterDeviceParams.end();
			     ++itr)
			{
				if (vDeviceList[i].find(itr->first) != std::string::npos)
				{
					if ((pcObject = pcObjectFactory->CreateObject(itr->second)) == NULL)
					{
						m_strMessage = "Fail to create tester device object";
						AfxMessageBox( m_strMessage.c_str() );
						return false;
					}

					m_pITesterDevice = dynamic_cast<ITesterDevice*>(pcObject);

					if (m_pITesterDevice->InitTestDeviceData(vDeviceList[i],
					        m_pITesterDevice->GetNI488Address(i),
					        m_pITesterDevice->GetPrimaryAddress(i),
					        m_pITesterDevice->GetSecondaryAddress(i))
					    != true)
					{
						m_pITesterDevice->Release();
						m_strMessage = "Fail to execute InitData() of tester device object";
						AfxMessageBox( m_strMessage.c_str() );
						return false;
					}
				}
			}

			for (std::map<std::string, std::string>::iterator itr = m_mapPowerSupplyParams.begin();
			     itr != m_mapPowerSupplyParams.end();
			     ++itr)
			{
				if (vDeviceList[i].find(itr->first) != std::string::npos)
				{
					if ((pcObject = pcObjectFactory->CreateObject(itr->second)) == NULL)
					{
						m_strMessage = "Fail to create power supply object";
						AfxMessageBox( m_strMessage.c_str() );
						return false;
					}

					IPowerSupply* pIPS = dynamic_cast<IPowerSupply*>(pcObject);

					//vDeviceList[i]
					if (m_vPowerSupplyAddr[0] == pIPS->GetPrimaryAddress(i))
						m_pIPS = dynamic_cast<IPowerSupply*>(pcObject);
					else if (m_vPowerSupplyAddr[1] == pIPS->GetPrimaryAddress(i))
						m_pIPS2 = dynamic_cast<IPowerSupply*>(pcObject);
					else if (m_vPowerSupplyAddr[2] == pIPS->GetPrimaryAddress(i))
						m_pIPS3 = dynamic_cast<IPowerSupply*>(pcObject);
					else
					{
						pIPS->Release();
						AfxMessageBox( "m_vPowerSupplyAddr fail" );
						return false;
					}

					if (pIPS->InitPowerSupplyData(vDeviceList[i],
					                              pIPS->GetNI488Address(i),
					                              pIPS->GetPrimaryAddress(i),
					                              pIPS->GetSecondaryAddress(i))
					    != true)
					{
						pIPS->Release();
						m_strMessage = "Fail to execute InitData() of power supply object";
						AfxMessageBox( m_strMessage.c_str() );
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool ITestProcessor::PhoneCreateObject()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strLower = "";
	m_strUpper = "";
	m_strUnit = "";
	m_strMeasured = "";
	m_strMessage = "";

	// 1. Parse phone parameters
	if (!ParsePhoneParams())
	{
		m_strMessage = "Fail to parse XML for phone parameters";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Parse phone parameters success");

	// 2. Create phone object
	CT2A szPhone(m_cstrPhone);
	std::string strPhone(szPhone);
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;
	if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
	{
		m_strMessage = "Fail to create phone object";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Create phone object success");

	// 3. Run phone object's InitData() API
	if (!pcObject->InitData(m_mapPhoneParams))
	{
		m_strMessage = "Fail to execute InitData() of phone object";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	m_pIPhone = dynamic_cast<IPhone*>(pcObject);
	TraceLog(MSG_INFO, "Execute InitDate() of phone object success");

	return true;
}

bool ITestProcessor::InitialPC()
{
	if (m_i_QPhoneMultiCtrl == 0)
	{
		if (!m_pIPhone->InitDevice(m_nFtdPort, m_nFtdTimeOut))
		{
			m_strMessage = "Fail to Initial PC port";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (m_pIPhone->m_devices.size() > 1)
		{
			for (size_t i = 0; i < m_pIPhone->m_devices.size(); i++)
			{
				int qphone_port = atoi(m_pIPhone->m_devices[i].c_str() + 6);//"QPHONE1"
				int port = (m_pIPhone->m_devices.size() > 1) ? m_nFtdPort + qphone_port: m_nFtdPort;
				m_pIPhone->InitDevice(port, m_nFtdTimeOut);
			}
		}
		TraceLog(MSG_INFO, "Initial PC port success.");
	}
	else
	{
		for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		{
			if (!m_pIPhone->InitDevice((2000 + m_nQPhoneOrderSlot[i]), m_nFtdTimeOut))
			{
				CString csTraceMsg;
				csTraceMsg.Format(_T("Fail to Initial Slot-%d PC port: %d"), i, (2000 + m_nQPhoneOrderSlot[i]));
				m_strMessage = csTraceMsg;
				TraceLog(MSG_ERROR, m_strMessage);
			}
			else
			{
				CString csTraceMsg;
				csTraceMsg.Format(_T("Success to Initial Slot-%d PC port: %d"), i, (2000 + m_nQPhoneOrderSlot[i]));
				TraceLog(MSG_INFO, std::string(csTraceMsg));
			}

		}
	}
	return true;
}

bool ITestProcessor::InitialRelayBoard()
{
	TraceLog(MSG_INFO, _T("------------------------------------ Begin ----------------------------------------"));
	return true;
}

bool ITestProcessor::InitialUSB4702()  //120517 for detroit by cy.lee
{
	// 1. Parse USB4702 parameters
	if (!ParseUSB4702Parameters())
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to parse XML for USB4702 parameters";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Parse USB4702 parameters success");
	}

	if (m_b_IsUsed4702)
	{
		if (!m_pIPhone->InitControlBoard())
		{
			m_strResult = "FAIL";
			m_strMessage = "Fail to Initial USB4702.";
			TraceLog(MSG_ERROR, "USB4702 Initial Fail");
			return false;
		}
		TraceLog(MSG_INFO, "USB4702 Initial Success");


		//-- Clear USB4702 --//
		if (!m_pIPhone->USB4702_SetAOVol(0, 0, "0.0"))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_SetAOVol0,0.0";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}

		if (!m_pIPhone->USB4702_SetAOVol(0, 1, "5.0"))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_SetAOVol1,5.0";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}

		if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit0";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit1";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit2";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit5";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 3))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit3";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 6))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit6";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 7))
		{
			m_strMessage = "Fail to execute m_pIPhone->USB4702_ClearDOBit7";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}

		/* Set power supply 1 off */
		if (!m_pIPS->SetPSOnOff(false))
		{
			m_strErrorCode = CommErr_PowerSupply_Control_Fail;
			m_strMessage = "Fail to set power supply 1 off";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		Sleep(2000);//wait power supply 1 off
		TraceLog(MSG_INFO, "Set power supply 1 off success");

		/* Set power supply 1 on */
		if (!m_pIPS->SetPSOnOff(true))
		{
			m_strErrorCode = CommErr_PowerSupply_Control_Fail;
			m_strMessage = "Fail to set power supply 1 on";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		Sleep(2000);//wait power supply 1 on
		TraceLog(MSG_INFO, "Set power supply 1 on success");

		// press power key
		if (!m_pIPhone->USB4702_SetDOBit(1, 3))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit3");
			return false;
		}
		Sleep(5000);

		// release power key
		if (!m_pIPhone->USB4702_ClearDOBit(1, 3))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit3");
			return false;
		}

		//-- USB_5V on ADB --//
		if (!m_pIPhone->USB4702_SetDOBit(1, 7))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit7");
			return false;
		}
		Sleep(18000);
	}
	else
	{
		TraceLog(MSG_INFO, "No using USB4702");
	}

	return true;
}

bool ITestProcessor::CloseUSB4702()  //120521 for detroit by cy.lee
{
	if (!m_b_IsUsed4702) return true;

	if (!m_pIPhone->USB4702_Close())
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to close USB4702.";
		TraceLog(MSG_ERROR, "USB4702 Close Fail");
		return false;
	}
	TraceLog(MSG_ERROR, "USB4702 Close Success");
	return true;
}

bool ITestProcessor::InitialPowerSupply()
{
	//Eason 

	if ( !ParseLeakageCurrentPowerOnCheckParameters() )
	{
		m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		m_strMessage = "Fail get PowerOnCheck Value";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	if ( m_b_IsUsePowerOnCheck )
	{
		/* 1. Start power supply 1 to provide power */
		// Use in PS1 => Dummy Battry , PS2 => USB Power Relay 
		// When PS2 Power on , MotherBoard Run and Check "PS1" Current (not check PS2)
		// PS Interfave Interval Time = 200 ms		
		m_strMessage = "StartPowerOnCheck";
		TraceLog(MSG_ERROR, m_strMessage);

		if ( m_pIPS == NULL || m_pIPS2 == NULL )
		{
			if ( m_pIPS == NULL )
			{
				m_strMessage = "No Find power supply 1";
				TraceLog(MSG_INFO, "No Find power supply 1");
				
			}
			else
			{
				m_strMessage = "No Find power supply 2";
				TraceLog(MSG_INFO, "No Find power supply 2");
				
			}
			return false;
		}

		if (m_pIPS)
		{
			if (m_pIPS->PowerSupplyIsAvaliable())
			{
				if (!m_pIPS->CheckInitialized())
				{
					if (!(m_pIPS->InitDevice(m_vPowerSupplyAddr[0], 1, 1)))
					{
						m_strErrorCode = CommErr_PowerSupply_Control_Fail;
						m_strMessage = "Failed to init PowerSupply 1";
						TraceLog(MSG_ERROR, m_strMessage);
						return false;
					}
				}
				/* Set power supply 1 voltage */
				if (!m_pIPS->SetVoltage(m_d_PSVoltage))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 voltage";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 1 current */
				if (!m_pIPS->SetCurrentLimit(m_d_PSCurrent))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 current";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* set power supply 1 range */
				if (!m_pIPS->SetCurrRangeMax())
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 current range";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				//virtual bool SetCapacitance(int nFlag);//0 = low, 1 = high
				/* set SetCapacitance */
				if (!m_pIPS->SetCapacitance(1))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to SetCapacitance 1";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 1 on */
				if (!m_pIPS->SetPSOnOff(true))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 on";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
				//-------------------------
				if ( m_i_PS1Check){
					UTimer PS1CheckTimer;
					PS1CheckTimer.SetTaskTimeCounter(1000);
					CRangePair checkValue; 
					checkValue.SetRange( m_str_PS1Value );
					do 
					{
						PS1CheckTimer.SetTaskTimeON();
						double f_PowerOnShort1 = 0.0;
						m_pIPS->GetCurrent(f_PowerOnShort1);
						if( !checkValue.InRange(f_PowerOnShort1) )
						{
							//Sleep(100);
							//f_PowerOnShort2 = 0.0;
							//if( !checkValue2.InRange(f_PowerOnShort2) )
							//{
								// First Close Power
								if (!m_pIPS->SetPSOnOff(false))
								{
									//m_strErrorCode = CommErr_PowerSupply_Control_Fail;
									//m_strMessage = "Fail to set power supply 1 off";
									//TraceLog(MSG_ERROR, m_strMessage);
									//return false;
								}		
								if (!m_pIPS2->SetPSOnOff(false))
								{
									//m_strErrorCode = CommErr_PowerSupply_Control_Fail;
									//m_strMessage = "Fail to set power supply 2 off";
									//TraceLog(MSG_ERROR, m_strMessage);
									//return false;
								}	


								//m_strErrorCode = FunErr_Max_Current_Test_Fail;
								m_strErrorCode = FunErr_Boot_Current_Test_Fail; //Eason
								CString str_trace;
								str_trace.Format(_T("Power1 On Short Current Over Range : %f") , f_PowerOnShort1);
								m_strMessage = std::string(CT2A(str_trace));
								TraceLog(MSG_ERROR, m_strMessage);
								return false;
							//}
						}
						if (m_pIPhone->IsConnected())
						{	
							break;
						}
						Sleep(200);					
					}
					while( !PS1CheckTimer.TaskTimeOff() );
				}// end if check ps value
				//------------------------------
				TraceLog(MSG_INFO, "Set power supply 1 on success");
				
			}
		}
		//else
		//{
		//	TraceLog(MSG_INFO, "No Find power supply 1");
		//}
		//------------------------------------------------------------------------------------------
		/* 2. Start power supply 2 to link USB cable */
		Sleep(1000);
		if (m_pIPS2)
		{
			if (m_pIPS2->PowerSupplyIsAvaliable())
			{
				if (!m_pIPS2->CheckInitialized())
				{
					if (!(m_pIPS2->InitDevice(m_vPowerSupplyAddr[1], 1, 1)))
					{
						m_strErrorCode = CommErr_PowerSupply_Control_Fail;
						m_strMessage = "Failed to init PowerSupply 2";
						TraceLog(MSG_ERROR, m_strMessage);
						return false;
					}
				}
				if (!m_pIPS2->SetVoltage(m_d_PSVoltage2))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 2 voltage";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
				/* Set power supply 1 current */
				if (!m_pIPS2->SetCurrentLimit(m_d_PSCurrent2))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 2 current";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				//virtual bool SetCapacitance(int nFlag);//0 = low, 1 = high
				/* set SetCapacitance */
				if (!m_pIPS2->SetCapacitance(1))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to SetCapacitance 2";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 2 on */
				if (!m_pIPS2->SetPSOnOff(true))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 2 on";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
			}
			TraceLog(MSG_INFO, "Set power supply 2 on");
			//-------------------------
			if ( m_i_PS2Check){
				UTimer PS2CheckTimer;
				PS2CheckTimer.SetTaskTimeCounter(m_i_PowerCheckOnTime*1000);
				CRangePair checkValue2; 
				checkValue2.SetRange( m_str_PS2Value );
				do 
				{
					PS2CheckTimer.SetTaskTimeON();
					double f_PowerOnShort2 = 0.0;
					m_pIPS2->GetCurrent(f_PowerOnShort2);
					if( !checkValue2.InRange(f_PowerOnShort2) )
					{
						//Sleep(100);
						//f_PowerOnShort2 = 0.0;
						//if( !checkValue2.InRange(f_PowerOnShort2) )
						//{
							// First Close Power
							if (!m_pIPS->SetPSOnOff(false))
							{
								//m_strErrorCode = CommErr_PowerSupply_Control_Fail;
								//m_strMessage = "Fail to set power supply 1 off";
								//TraceLog(MSG_ERROR, m_strMessage);
								//return false;
							}		
							if (!m_pIPS2->SetPSOnOff(false))
							{
								//m_strErrorCode = CommErr_PowerSupply_Control_Fail;
								//m_strMessage = "Fail to set power supply 2 off";
								//TraceLog(MSG_ERROR, m_strMessage);
								//return false;
							}	


							//m_strErrorCode = FunErr_Max_Current_Test_Fail;
							m_strErrorCode = FunErr_Boot_Current_Test_Fail; //Eason
							CString str_trace;
							str_trace.Format(_T("Power2 On Short Current Over Range : %f") , f_PowerOnShort2);
							m_strMessage = std::string(CT2A(str_trace));
							TraceLog(MSG_ERROR, m_strMessage);
							return false;
						//}
					}
					if (m_pIPhone->IsConnected())
					{	
						break;
					}
					Sleep(200);					
				}
				while( !PS2CheckTimer.TaskTimeOff() );
			}//end if for check ps 2
			//------------------------------
		}
		Sleep(1000); // ADB delay 
		//else
		//{
		//	TraceLog(MSG_INFO, "No Find power supply 2");
		//}
		/* 3. Start power supply 3 to link USB cable */
		//if (m_pIPS3)
		//{
		//	if (m_pIPS3->PowerSupplyIsAvaliable())
		//	{
		//		if (!m_pIPS3->CheckInitialized())
		//		{
		//			if (!(m_pIPS3->InitDevice(m_vPowerSupplyAddr[2], 1, 1)))
		//			{
		//				m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		//				m_strMessage = "Failed to init PowerSupply 3";
		//				TraceLog(MSG_ERROR, m_strMessage);
		//				return false;
		//			}
		//		}

		//		/* Set power supply 3 voltage */
		//		if (!m_pIPS3->SetVoltage(m_d_PSVoltage3))
		//		{
		//			m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		//			m_strMessage = "Fail to set power supply 3 voltage";
		//			TraceLog(MSG_ERROR, m_strMessage);
		//			return false;
		//		}

		//		/* Set power supply 3 current */
		//		if (!m_pIPS3->SetCurrentLimit(m_d_PSCurrent3))
		//		{
		//			m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		//			m_strMessage = "Fail to set power supply 3 current";
		//			TraceLog(MSG_ERROR, m_strMessage);
		//			return false;
		//		}
		//	}
		//	TraceLog(MSG_INFO, "Set power supply 3 on");
		//}

		if ( m_pIPS && m_pIPS2 )
		{
			TraceLog(MSG_INFO, "PowerOnCheck OK");
		}

		return true;
	}
	else
	{
		/* 1. Start power supply 1 to provide power */
		if (m_pIPS)
		{
			if (m_pIPS->PowerSupplyIsAvaliable())
			{
				if (!m_pIPS->CheckInitialized())
				{
					if (!(m_pIPS->InitDevice(m_vPowerSupplyAddr[0], 1, 1)))
					{
						m_strErrorCode = CommErr_PowerSupply_Control_Fail;
						m_strMessage = "Failed to init PowerSupply 1";
						TraceLog(MSG_ERROR, m_strMessage);
						return false;
					}
				}

				/* Set power supply 1 voltage */
				if (!m_pIPS->SetVoltage(m_d_PSVoltage))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 voltage";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 1 current */
				if (!m_pIPS->SetCurrentLimit(m_d_PSCurrent))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 current";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* set power supply 1 range */
				if (!m_pIPS->SetCurrRangeMax())
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 current range";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 1 on */
				if (!m_pIPS->SetPSOnOff(true))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 1 on";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
				Sleep(1000);//wait power supply 1 on
				TraceLog(MSG_INFO, "Set power supply 1 on success");
			}
		}

		/* 2. Start power supply 2 to link USB cable */
		if (m_pIPS2)
		{
			if (m_pIPS2->PowerSupplyIsAvaliable())
			{
				if (!m_pIPS2->CheckInitialized())
				{
					if (!(m_pIPS2->InitDevice(m_vPowerSupplyAddr[1], 1, 1)))
					{
						m_strErrorCode = CommErr_PowerSupply_Control_Fail;
						m_strMessage = "Failed to init PowerSupply 2";
						TraceLog(MSG_ERROR, m_strMessage);
						return false;
					}
				}

				/* Set power supply 2 voltage */
				if (!m_pIPS2->SetVoltage(5.0))
				//if (!m_pIPS2->SetVoltage(3.8))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 2 voltage";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 2 on */
				if (!m_pIPS2->SetPSOnOff(true))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 2 on";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
			}
			TraceLog(MSG_INFO, "Set power supply 2 on");
		}

		/* 3. Start power supply 3 to link USB cable */
		if (m_pIPS3)
		{
			if (m_pIPS3->PowerSupplyIsAvaliable())
			{
				if (!m_pIPS3->CheckInitialized())
				{
					if (!(m_pIPS3->InitDevice(m_vPowerSupplyAddr[2], 1, 1)))
					{
						m_strErrorCode = CommErr_PowerSupply_Control_Fail;
						m_strMessage = "Failed to init PowerSupply 3";
						TraceLog(MSG_ERROR, m_strMessage);
						return false;
					}
				}

				/* Set power supply 3 voltage */
				if (!m_pIPS3->SetVoltage(m_d_PSVoltage3))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 3 voltage";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}

				/* Set power supply 3 current */
				if (!m_pIPS3->SetCurrentLimit(m_d_PSCurrent3))
				{
					m_strErrorCode = CommErr_PowerSupply_Control_Fail;
					m_strMessage = "Fail to set power supply 3 current";
					TraceLog(MSG_ERROR, m_strMessage);
					return false;
				}
			}
			TraceLog(MSG_INFO, "Set power supply 3 on");
		}
		return true;

	}
}

bool ITestProcessor::MobileAutoFTDMode()
{
	if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "Mobile is already in ftd mode, Do not need to change mode!");
		return true;
	}
	TraceLog(MSG_INFO, "Mobile is not in ftd mode, Auto change ftd mode......");
	if (m_pIPhone->MobileAutoFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "MobileAutoFTDMode success!");
		return true;
	}
	m_strMessage = "Fail to enter FTD mode!";
	TraceLog(MSG_INFO, "Fail to enter ftd mode!");
	return false;
}

bool ITestProcessor::MobileSetFTDMode()
{
	if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))
	{
		TraceLog(MSG_INFO, "Mobile is already in ftd mode, Do not need to set mode!");
		return true;
	}

	TraceLog(MSG_INFO, "Mobile is not in ftd mode, set ftd mode......");
	CString str_command = _T("shell set-mode 3");
	char errMsg[128] = {0};
	char cmdOut[1024] = {0};
	if (!m_pIPhone->ExecAdbCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, errMsg);
		TraceLog(MSG_INFO, "Fail to set ftd mode!");
		return false;
	}
	TraceLog(MSG_INFO, "Execute 'set-mode 3' adb command success!");
	CString str_OutMsg;
	str_OutMsg.Format(_T("%s"), cmdOut);
	if (str_OutMsg.Find(_T("error")) != -1)
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, "Fail to set FTD mode!");
		return false;
	}

	/* Reboot phone */
	TraceLog(MSG_INFO, "Reboot mobile phone......");
	str_command = _T("reboot");
	if (!m_pIPhone->ExecAdbCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, errMsg);
		TraceLog(MSG_INFO, "Fail to reboot mobile phone!");
		return false;
	}
	TraceLog(MSG_INFO, "Execute reboot mobile adb command success!");
	str_OutMsg.Format(_T("%s"), cmdOut);
	if (str_OutMsg.Find(_T("error")) != -1)
	{
		m_strMessage = "Fail to set FTD mode!";
		TraceLog(MSG_INFO, "Fail to reboot mobile phone!");
		return false;
	}
	Sleep(m_i_RebootDelayTime);//wait for phone reboot
	return true;
}

bool ITestProcessor::KillADBThread()
{
	if (!m_pIPhone) return false;

	char errMsg[128] = {0};
	char cmdOut[1024] = {0};
	return m_pIPhone->ExecAdbCommand(_T("kill-server"), cmdOut, errMsg);
}

/* FA data operate functions */
bool ITestProcessor::ReadFAData(char* pszFAdata, int i_size)
{
	return false;
}

bool ITestProcessor::WriteFAData(char* pszFAdata, size_t i_InSize, char* pszOutput)
{
	return false;
}

//QTH 2013.3.14 Neil
bool ITestProcessor::ReadFAData_New(int i_slot, int i_sectorNum, char* pszFAdata, int i_size)
{
	if (!m_pIPhone) return false;

	char sz_FAData[FTD_BUF_SIZE] = {0};
	char sz_Address[10] = {0};
	_itoa_s(i_sectorNum * 512, sz_Address, 10, 10); // int to char
	strcat(sz_Address, ",512");
	int port = m_nFtdPort;
	if (m_i_QPhoneMultiCtrl == 0)
	{
		int qphone_port = atoi(m_pIPhone->m_devices[0].c_str() + 6);//"QPHONE1"
		port = (m_pIPhone->m_devices.size() > 1) ? m_nFtdPort + qphone_port : m_nFtdPort;
	}
	else
	{
		if (!m_b_UsableSlot[i_slot])
		{
			CString csTeaceMsg;
			csTeaceMsg.Format(_T("By Pass to Read QPhone%d FA data"), i_slot);
			TraceLog(MSG_INFO, std::string(CT2A(csTeaceMsg)));
			return false;
		}
		port = 2000 + m_nQPhoneOrderSlot[i_slot];
	}
	bool b_res = m_pIPhone->FTD_FAC_FARead(port, m_nFtdTimeOut, sz_Address, sz_FAData);
	if (b_res)
	{
		memcpy(pszFAdata, sz_FAData, i_size);
		CString csTeaceMsg;
		csTeaceMsg.Format(_T("Read FA data from QPhone%d success"), i_slot);
		TraceLog(MSG_INFO, std::string(csTeaceMsg));
	}
	else
	{
		m_strErrorCode = CommErr_Read_Factory_Area_Fail;
		m_strMessage = "Fail to read FA data from mobile";
		CString csTeaceMsg;
		csTeaceMsg.Format(_T("Fail to read FA data from QPhone%d"), i_slot);
		TraceLog(MSG_ERROR, std::string(csTeaceMsg));
	}
	return b_res;
}

bool ITestProcessor::WriteFAData_New(int i_sectorNum, char* pszFAdata, size_t i_size, char* pszOutput)
{
	if (!m_pIPhone) return false;

	char szInput[FTD_BUF_SIZE] = {0} ;
	_itoa_s(i_sectorNum * 512, szInput, 10, 10); // int to char
	strcat(szInput, ",512,");
	strcat(szInput, pszFAdata);
	bool b_res = m_pIPhone->FTD_FAC_FAWrite(m_nFtdPort, m_nFtdTimeOut, szInput, pszOutput);
	if (b_res)
	{
		TraceLog(MSG_INFO, "Write FA data to mobile success");
	}
	else
	{
		m_strErrorCode = CommErr_Write_Factory_Area_Fail;
		m_strMessage = "Fail to write FA data to mobile";
		TraceLog(MSG_ERROR, m_strMessage);
	}
	return b_res;
}

//Test items related functions
bool ITestProcessor::LoadTestITems()
{
	bool bRes = false;
	if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		bRes = true;

	if (!bRes) return false;

	XMLNodeList testItemNodeList = NULL;
	if (bRes)
		testItemNodeList = m_TestItemXML.GetChildNodes(_T("//Configuration//TestItemList"));
	if (testItemNodeList != NULL)
		bRes = true;

	for (UINT i = 0; (bRes && (i < m_TestItemXML.GetNodeListSize(testItemNodeList))); ++i)
	{
		XMLNode testItemNode = m_TestItemXML.GetNodeListItem(testItemNodeList, i);
		if (testItemNode != NULL)
			bRes = true;
		else
			bRes = false;

		CString cstrEnabled = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Enabled"));
		if (cstrEnabled.CompareNoCase(_T("TRUE")) != 0)
			continue;

		CString cstrTestItemName = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Name"));
		if (cstrTestItemName.IsEmpty() != true)
		{
			CT2A pszTestItemName(cstrTestItemName);
			std::string strTestItemName(pszTestItemName);
			m_vTestItemList.push_back(strTestItemName);
		}
	}
	return bRes;
}

int ITestProcessor::GetTestItemCount()
{
	return (signed)m_vTestItemList.size();
}

bool ITestProcessor::GetTestItemByIndex(int nIndex, char* szName, int nSize)
{
	bool bRes = false;
	if (nIndex < (signed)m_vTestItemList.size())
	{
		if (sprintf_s(szName, nSize, "%s", m_vTestItemList[nIndex].c_str()) != -1)
			bRes = true;
	}
	return bRes;
}

bool ITestProcessor::GetTestResult(int nIndex, st_UIResult* psResult)
{
	if (psResult == NULL) return false;

	sprintf_s(psResult->sz_result, MAX_BUFFER_SIZE - 1, "%s", m_strResult.c_str());
	sprintf_s(psResult->sz_itemCode, MAX_BUFFER_SIZE - 1, "%s", m_strItemCode.c_str());
	sprintf_s(psResult->sz_errCode, MAX_BUFFER_SIZE - 1, "%s", m_strErrorCode.c_str());
	sprintf_s(psResult->sz_band, MAX_BUFFER_SIZE - 1, "%s", m_strBand.c_str());
	sprintf_s(psResult->sz_channel, MAX_BUFFER_SIZE - 1, "%s", m_strChannel.c_str());
	sprintf_s(psResult->sz_upper, MAX_BUFFER_SIZE - 1, "%s", m_strUpper.c_str());
	sprintf_s(psResult->sz_lower, MAX_BUFFER_SIZE - 1, "%s", m_strLower.c_str());
	sprintf_s(psResult->sz_measured, MAX_BUFFER_SIZE - 1, "%s", m_strMeasured.c_str());
	sprintf_s(psResult->sz_unit, MAX_BUFFER_SIZE - 1, "%s", m_strUnit.c_str());
	sprintf_s(psResult->sz_msg, MAX_BUFFER_SIZE - 1, "%s", m_strMessage.c_str());
	return true;
}

//Initial functions
bool ITestProcessor::LoadCableLossXML()
{
	if (m_CableLossXML.Load(m_cstrCableLossFile) != ERROR_SUCCESS)
		return false;

	XMLNode currentRfLosses = m_CableLossXML.SearchNode(_T("//FrameworkConfig//RfSettings//RfCable//CurrentRfLosses"));
	if (currentRfLosses == NULL)
		return false;

	CString cstrCurrentRfLosses = m_CableLossXML.GetNodeText(currentRfLosses);
	if (cstrCurrentRfLosses.IsEmpty())
		return false;

	XMLNode rfCable = m_CableLossXML.SearchNode(_T("//FrameworkConfig//RfSettings//RfCable"));
	if (rfCable == NULL)
		return false;

	XMLNodeList rfLossesList = m_CableLossXML.GetChildNodes(rfCable);
	if (rfLossesList == NULL)
		return false;

	for (UINT i = 0; i < m_CableLossXML.GetNodeListSize(rfLossesList); ++i)
	{
		XMLNode rfLosses = m_CableLossXML.GetNodeListItem(rfLossesList, i);
		if (rfLosses == NULL)
			continue;

		CString cstrRfLossesAttr = m_CableLossXML.GetNodeAttrValue(rfLosses, _T("name"));
		if (cstrRfLossesAttr.CompareNoCase(cstrCurrentRfLosses) != 0)
			continue;

		// Found matching cable loss set
		std::map<std::string, std::map<std::string, std::string>> mapTxCableLossData;
		std::map<std::string, std::map<std::string, std::string>> mapRxCableLossData;
		std::map<std::string, std::string> mapCableLoss;
		XMLNode rfType = m_CableLossXML.GetFirstChild(rfLosses);
		while (rfType)
		{
			CString cstrRfTypeAttr = m_CableLossXML.GetNodeAttrValue(rfType, _T("name"));
			if (cstrRfTypeAttr.CompareNoCase(_T("TX")) == 0)
			{
				// Start parsing TX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
					// FrameworkConfig//RfSettings//RfCable//RfType//RfBand
					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
					if (rfBand == NULL)
						continue;

					mapCableLoss.clear();
					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
					if (rfChannelList == NULL)
						continue;

					for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
					{
						// FrameworkConfig//RfSettings//RfCable//RfType//RfBand//RfChannel
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));
						// FrameworkConfig//RfSettings//RfCable//RfType//RfBand//RfChannel//RfLoss
						XMLNodeList rfLossList = m_CableLossXML.GetChildNodes(rfChannel);
						if (rfLossList == NULL)
							continue;

						for (UINT l = 0; l < m_CableLossXML.GetNodeListSize(rfLossList); ++l)
						{
							XMLNode rfLoss = m_CableLossXML.GetNodeListItem(rfLossList, l);
							if (rfLoss == NULL)
								continue;

							CString cstrRfLossNodeName = m_CableLossXML.GetNodeName(rfLoss);
							if (cstrRfLossNodeName.CompareNoCase(_T("RfLoss")) == 0)
							{
								CString cstrRfLoss = m_CableLossXML.GetNodeText(rfLoss);

								// Insert a cable loss value in map
								CT2A szRange(cstrRfChannelRange);
								std::string strRange(szRange);
								CT2A szRfLoss(cstrRfLoss);
								std::string strRfLoss(szRfLoss);
								mapCableLoss[strRange] = strRfLoss;
							}
						}
					}

					if (mapCableLoss.empty() != true)
					{
						CT2A szBand(cstrRfBandName);
						std::string strBand(szBand);
						mapTxCableLossData[strBand] = mapCableLoss;
					}
				}//for (UINT k
			}
			else if (cstrRfTypeAttr.CompareNoCase(_T("RX")) == 0)
			{
				// Start parsing RX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
					// FrameworkConfig//RfSettings//RfCable//RfType//RfBand
					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
					if (rfBand == NULL)
						continue;

					mapCableLoss.clear();
					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
					if (rfChannelList == NULL)
						continue;

					for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
					{
						// FrameworkConfig//RfSettings//RfCable//RfType//RfBand//RfChannel
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));
						// FrameworkConfig//RfSettings//RfCable//RfType//RfBand//RfChannel//RfLoss
						XMLNode rfLoss = m_CableLossXML.GetFirstChild(rfChannel);
						if (rfLoss == NULL)
							continue;

						CString cstrRfLoss = m_CableLossXML.GetNodeText(rfLoss);
						// Insert a cable loss value in map
						CT2A szRange(cstrRfChannelRange);
						std::string strRange(szRange);
						CT2A szRfLoss(cstrRfLoss);
						std::string strRfLoss(szRfLoss);
						mapCableLoss[strRange] = strRfLoss;
					}//for (UINT k
					if (mapCableLoss.empty() != true)
					{
						CT2A szBand(cstrRfBandName);
						std::string strBand(szBand);
						mapRxCableLossData[strBand] = mapCableLoss;
					}
				}//for (UINT j
			}
			rfType = rfType->GetnextSibling();
		}//while (rfType)

		if (mapTxCableLossData.empty() != true)
			g_mapTxCableLossData = mapTxCableLossData;

		if (mapRxCableLossData.empty() != true)
			g_mapRxCableLossData = mapRxCableLossData;
	}//for (UINT i

	return true;
}

bool ITestProcessor::ParseGPIBDeviceParams()
{
	if (m_DeviceItemXML.Load(m_cstrDeviceItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode GPIBInterfaceID = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//GPIBInterfaceID"));
	if (GPIBInterfaceID != NULL)
	{
		CString cstrGPIBInterfaceID = m_DeviceItemXML.GetNodeText(GPIBInterfaceID);
		if (!cstrGPIBInterfaceID.IsEmpty())
			m_iGPIBInterfaceID = _ttoi(cstrGPIBInterfaceID);
	}

	XMLNode GPIBCardAddress = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//GPIBCardAddress"));
	if (GPIBCardAddress != NULL)
	{
		CString cstrGPIBCardAddress = m_DeviceItemXML.GetNodeText(GPIBCardAddress);
		if (!cstrGPIBCardAddress.IsEmpty())
			m_iGPIBCardAddress = _ttoi(cstrGPIBCardAddress);

		IFunc_Dev::DoGPIBSelect(m_iGPIBCardAddress);//110324 gilpin add for CMU object use
	}

	std::map<std::string, std::string> mapTesterDevice;
	XMLNode testerDevice = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//TesterDevice"));
	if (testerDevice != NULL)
	{
		XMLNodeList testerDeviceList = m_DeviceItemXML.GetChildNodes(testerDevice);
		if (testerDeviceList != NULL)
		{
			for (UINT i = 0; i < m_DeviceItemXML.GetNodeListSize(testerDeviceList); ++i)
			{
				XMLNode testerDeviceItem = m_DeviceItemXML.GetNodeListItem(testerDeviceList, i);
				if (testerDeviceItem != NULL)
				{
					CString cstrName = m_DeviceItemXML.GetNodeAttrValue(testerDeviceItem, _T("Name"));
					CString cstrClassName = m_DeviceItemXML.GetNodeAttrValue(testerDeviceItem, _T("ClassName"));
					if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
						continue;

					// Insert <Name, ClassName> to map
					CT2A szName(cstrName);
					std::string strName(szName);
					CT2A szClassName(cstrClassName);
					std::string strClassName(szClassName);
					mapTesterDevice[strName] = strClassName;
				}
			}
		}

		XMLNode rfInputPort = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//TesterDevice//RFInputPort"));
		if (rfInputPort != NULL)
		{
			CString cstrRFInputPort = m_DeviceItemXML.GetNodeText(rfInputPort);
			if (!cstrRFInputPort.IsEmpty())
				ITesterDevice::SetRFInputPort(_ttoi(cstrRFInputPort));
		}

		XMLNode rfOutputPort = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//TesterDevice//RFOutputPort"));
		if (rfOutputPort != NULL)
		{
			CString cstrRFOutputPort = m_DeviceItemXML.GetNodeText(rfOutputPort);
			if (!cstrRFOutputPort.IsEmpty())
				ITesterDevice::SetRFOutputPort(_ttoi(cstrRFOutputPort));
		}
	}

	XMLNode powerSupplyCount = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//PowerSupplyCount"));
	if (powerSupplyCount != NULL)
	{
		CString cstrPowerSupplyCount = m_DeviceItemXML.GetNodeText(powerSupplyCount);
		m_iPSCount = _ttoi(cstrPowerSupplyCount);
	}

	std::vector<int> vPSAddr;
	XMLNode powerSupplyList = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//PowerSupplyList"));
	if (powerSupplyList != NULL)
	{
		XMLNodeList powerSupplyOrderList = m_DeviceItemXML.GetChildNodes(powerSupplyList);
		if (powerSupplyOrderList != NULL)
		{
			for (UINT i = 0; i < m_DeviceItemXML.GetNodeListSize(powerSupplyOrderList); ++i)
			{
				XMLNode powerSupplyOrder = m_DeviceItemXML.GetNodeListItem(powerSupplyOrderList, i);
				if (powerSupplyOrder != NULL)
				{
					CString cstrPSAddr = m_DeviceItemXML.GetNodeText(powerSupplyOrder);
					if (cstrPSAddr.IsEmpty())
					{
						vPSAddr.push_back(0);
					}
					else
					{
						int iAddr = _ttoi(cstrPSAddr);
						vPSAddr.push_back(iAddr);
					}
				}
			}
		}
	}

	std::map<std::string, std::string> mapPowerSupply;
	XMLNode powerSupply = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply"));
	if (powerSupply != NULL)
	{
		XMLNodeList powerSupplyChildList = m_DeviceItemXML.GetChildNodes(powerSupply);
		if (powerSupplyChildList != NULL)
		{
			for (UINT i = 0; i < m_DeviceItemXML.GetNodeListSize(powerSupplyChildList); ++i)
			{
				XMLNode powerSupplyItem = m_DeviceItemXML.GetNodeListItem(powerSupplyChildList, i);
				if (powerSupplyItem != NULL)
				{
					if (m_DeviceItemXML.GetNodeName(powerSupplyItem).CompareNoCase(_T("Item")) != 0)
						continue;

					CString cstrName = m_DeviceItemXML.GetNodeAttrValue(powerSupplyItem, _T("Name"));
					CString cstrClassName = m_DeviceItemXML.GetNodeAttrValue(powerSupplyItem, _T("ClassName"));
					if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
						continue;

					// Insert <Name, ClassName> to map
					CT2A szName(cstrName);
					std::string strName(szName);
					CT2A szClassName(cstrClassName);
					std::string strClassName(szClassName);
					mapPowerSupply[strName] = strClassName;
				}
			}
		}
	}

	XMLNode PSVoltage = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Voltage"));
	if (PSVoltage != NULL)
	{
		CString str_voltage = m_DeviceItemXML.GetNodeText(PSVoltage);
		if (!str_voltage.IsEmpty())
			m_d_PSVoltage = atof(str_voltage);
	}
	XMLNode PSVoltage2 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Voltage2"));
	if (PSVoltage2 != NULL)
	{
		CString str_voltage2 = m_DeviceItemXML.GetNodeText(PSVoltage2);
		if (!str_voltage2.IsEmpty())
			m_d_PSVoltage2 = atof(str_voltage2);
	}
	XMLNode PSVoltage3 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Voltage3"));
	if (PSVoltage3 != NULL)
	{
		CString str_voltage3 = m_DeviceItemXML.GetNodeText(PSVoltage3);
		if (!str_voltage3.IsEmpty())
			m_d_PSVoltage3 = atof(str_voltage3);
	}

	XMLNode PSCurrent = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Current"));
	if (PSCurrent != NULL)
	{
		CString str_current = m_DeviceItemXML.GetNodeText(PSCurrent);
		if (!str_current.IsEmpty())
			m_d_PSCurrent = atof(str_current);
	}
	XMLNode PSCurrent2 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Current2"));
	if (PSCurrent2 != NULL)
	{
		CString str_current2 = m_DeviceItemXML.GetNodeText(PSCurrent2);
		if (!str_current2.IsEmpty())
			m_d_PSCurrent2 = atof(str_current2);
	}
	XMLNode PSCurrent3 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//Current3"));
	if (PSCurrent3 != NULL)
	{
		CString str_current3 = m_DeviceItemXML.GetNodeText(PSCurrent3);
		if (!str_current3.IsEmpty())
			m_d_PSCurrent3 = atof(str_current3);
	}

	XMLNode PS1_OnOff = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//PS1_OnOff"));
	if (PS1_OnOff != NULL)
	{
		CString str_OnOff = m_DeviceItemXML.GetNodeText(PS1_OnOff);
		if (!str_OnOff.IsEmpty())
			m_b_PS1_OnOff = (str_OnOff.CompareNoCase(_T("TRUE")) == 0);
	}

	XMLNode PS2_OnOff = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerSupply//PS2_OnOff"));
	if (PS2_OnOff != NULL)
	{
		CString str_OnOff = m_DeviceItemXML.GetNodeText(PS2_OnOff);
		if (!str_OnOff.IsEmpty())
			m_b_PS2_OnOff = (str_OnOff.CompareNoCase(_T("TRUE")) == 0);
	}

	m_vPowerSupplyAddr = vPSAddr;
	m_mapTesterDeviceParams = mapTesterDevice;
	m_mapPowerSupplyParams = mapPowerSupply;
	return true;
}

bool ITestProcessor::DetectGPIBDevice()
{
#ifdef _DONT_DETECT_GPIB // for debugging purpose
	return true;
#endif
	return IGPIBDevice::DetectGPIBDevice(m_iGPIBInterfaceID, m_iGPIBCardAddress);
}

bool ITestProcessor::ParsePhoneParams()
{
	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode phone = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone"));
	if (phone == NULL)
		return false;

	XMLNode m_COMPort = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//COMPort"));
	if (m_COMPort != NULL)
	{
		CString cstrCOMPort = m_TestItemXML.GetNodeText(m_COMPort);
		m_nCOMPort = atoi(cstrCOMPort);
	}

	XMLNode m_FtdPort = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//FtdPort"));
	if (m_FtdPort != NULL)
	{
		CString cstrComPort = m_TestItemXML.GetNodeText(m_FtdPort);
		m_nFtdPort = atoi(cstrComPort);
	}

	XMLNode m_FtdTimeOut = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//FtdTimeOut"));
	if (m_FtdTimeOut != NULL)
	{
		CString cstrFtdTimeOut = m_TestItemXML.GetNodeText(m_FtdTimeOut);
		m_nFtdTimeOut = atoi(cstrFtdTimeOut);
	}

	XMLNode m_AutoFTDMode = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//AutoFTDMode"));
	if (m_AutoFTDMode != NULL)
	{
		CString str_AutoFTDMode = m_TestItemXML.GetNodeText(m_AutoFTDMode);
		m_b_AutoFTDMode = CStr::StrToBool(str_AutoFTDMode.GetString());
	}

	XMLNode m_SetFTDMode = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//SetFTDMode"));
	if (m_SetFTDMode != NULL)
	{
		CString str_SetFTDMode = m_TestItemXML.GetNodeText(m_SetFTDMode);
		m_b_SetFTDMode = CStr::StrToBool(str_SetFTDMode.GetString());
	}

	XMLNode m_WaitTime = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//WaitTimeStartingUp"));
	if (m_WaitTime != NULL)
	{
		CString str_WaitTime = m_TestItemXML.GetNodeText(m_WaitTime);
		m_i_WaitTimeStartingUp = atoi(str_WaitTime);
	}

	XMLNode m_RebootDelayTime = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//RebootDelayTime"));
	if (m_RebootDelayTime != NULL)
	{
		CString str_RebootDelayTime = m_TestItemXML.GetNodeText(m_RebootDelayTime);
		m_i_RebootDelayTime = atoi(str_RebootDelayTime);
	}

	XMLNode m_IsDiagCmd = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//IsDiagCmd"));
	if (m_IsDiagCmd != NULL)
	{
		CString IsDiagCmd = m_TestItemXML.GetNodeText(m_IsDiagCmd);
		m_i_diagCmdFlag = atoi(IsDiagCmd);
	}

	/* Need to generate QXDM log of CPhone */
	XMLNode IsNeedQXDMLog = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//IsNeedQXDMLog"));
	if (IsNeedQXDMLog != NULL)
	{
		CString str_IsNeedQXDMLog = m_TestItemXML.GetNodeText(IsNeedQXDMLog);
		m_b_IsNeedQXDMLog = CStr::StrToBool(str_IsNeedQXDMLog.GetString());
	}

	XMLNodeList phoneList = m_TestItemXML.GetChildNodes(phone);
	if (phoneList == NULL)
		return false;

	std::map <std::string, std::string> mapPhoneParams;
	for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_TestItemXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_TestItemXML.GetNodeName(phoneParams);
		if (nodename.CompareNoCase(_T("ClassName")) == 0)
			m_cstrPhone = m_TestItemXML.GetNodeText(phoneParams);

		CString nodevalue = m_TestItemXML.GetNodeText(phoneParams);
		mapPhoneParams[nodename.GetString()] = nodevalue.GetString();
	}

	XMLNode m_QPhoneMulti = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//QPhoneMulti"));
	if (m_QPhoneMulti != NULL)
	{
		CString str_QPhoneMulti = m_TestItemXML.GetNodeText(m_QPhoneMulti);
		m_i_QPhoneMultiCtrl = atoi(str_QPhoneMulti);
	}

	XMLNode m_QPhoneOrder = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//QPhoneOrder"));
	if (m_QPhoneOrder != NULL)
	{
		String parameters = m_TestItemXML.GetNodeText(m_QPhoneOrder).GetString();
		StrVtr vtrstr;
		CStr::ParseString(parameters, ",", vtrstr);
		m_nQPhoneOrderSlot[1] = CStr::StrToInt(vtrstr[0]);
		m_nQPhoneOrderSlot[2] = CStr::StrToInt(vtrstr[1]);
		m_nQPhoneOrderSlot[3] = CStr::StrToInt(vtrstr[2]);
		m_nQPhoneOrderSlot[4] = CStr::StrToInt(vtrstr[3]);
	}

	XMLNode m_AdbShellStart = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//AdbShellStart"));
	if (m_AdbShellStart != NULL)
	{
		CString str_AdbShellStart = m_TestItemXML.GetNodeText(m_AdbShellStart);
		m_b_AdbShellStart = CStr::StrToBool(str_AdbShellStart.GetString());
	}

	XMLNode m_LogUpload = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//LogUpload"));
	if (m_LogUpload != NULL)
	{
		CString str_LogUpload = m_TestItemXML.GetNodeText(m_LogUpload);
		m_b_LogUpload = CStr::StrToBool(str_LogUpload.GetString());
	}

	XMLNode m_UploadServerIP = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//UploadServerIP"));
	if (m_UploadServerIP != NULL)
	{
		CString str_UploadServerIP = m_TestItemXML.GetNodeText(m_UploadServerIP);
		m_str_UploadServerIP = str_UploadServerIP.GetString();
	}

	// Search variant
	XMLNode toolinfo = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//Phone//Variant"));
	if (toolinfo == NULL)
		return false;

	mapPhoneParams["Variant"] = CT2A(m_TestItemXML.GetNodeText(toolinfo).GetString());

	m_mapPhoneParams = mapPhoneParams;
	return !m_cstrPhone.IsEmpty();
}

bool ITestProcessor::ParseRelayBoardParameters()
{
	if (m_DeviceItemXML.Load(m_cstrDeviceItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode RelayBoard = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//RelayBoard"));
	if (RelayBoard == NULL)
		return false;

	XMLNode IsUsed = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//RelayBoard//IsUsed"));
	if (IsUsed != NULL)
	{
		CString str_IsUsed = m_DeviceItemXML.GetNodeText(IsUsed);
		m_b_IsUsed = CStr::StrToBool(str_IsUsed.GetString());
	}

	XMLNode RelayBoard_Control = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//RelayBoard//RelayBoard_Control"));
	if (RelayBoard_Control != NULL)
	{
		String parameters = m_DeviceItemXML.GetNodeText(RelayBoard_Control).GetString();
		StrVtr vtrstr;
		CStr::ParseString(parameters, ",", vtrstr);
		if (vtrstr.size() != 5)
		{
			TraceLog(MSG_ERROR, _T("Get initial relay board control parameter 'RelayBoard_Control' is not correct number 3"));
			return false;
		}
		m_i_board   = CStr::StrToInt(vtrstr[0]);
		m_i_slot[0] = CStr::StrToInt(vtrstr[1]);
		m_i_slot[1] = CStr::StrToInt(vtrstr[2]);
		m_i_slot[2] = CStr::StrToInt(vtrstr[3]);
		m_i_slot[3] = CStr::StrToInt(vtrstr[4]);
	}

	XMLNodeList RelayBoardList = m_DeviceItemXML.GetChildNodes(RelayBoard);
	if (RelayBoardList == NULL)
		return false;

	std::map <std::string, std::string> mapRelayBoardParams;
	for (UINT i = 0; i < m_DeviceItemXML.GetNodeListSize(RelayBoardList); ++i)
	{
		XMLNode phoneParams = m_DeviceItemXML.GetNodeListItem(RelayBoardList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename  = CStringA(m_DeviceItemXML.GetNodeName(phoneParams));
		CString nodevalue = CStringA(m_DeviceItemXML.GetNodeText(phoneParams));
		mapRelayBoardParams[nodename.GetString()] = nodevalue.GetString();
	}

	return true;
}

bool ITestProcessor::ParseUSB4702Parameters()  //120517 for detroit by cy.lee
{
	if (m_DeviceItemXML.Load(m_cstrDeviceItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode USB4702 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//USB4702"));
	if (USB4702 == NULL)
		return false;

	XMLNode IsUsed4702 = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//USB4702//IsUsed"));
	if (IsUsed4702 == NULL)
		return false;

	CString str_IsUsed4702 = m_DeviceItemXML.GetNodeText(IsUsed4702);
	m_b_IsUsed4702 = CStr::StrToBool(str_IsUsed4702.GetString());
	return true;
}



bool ITestProcessor::ParseLeakageCurrentPowerOnCheckParameters()  //Eason
{
	if (m_DeviceItemXML.Load(m_cstrDeviceItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode PowerOnCheck = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck"));
	if (PowerOnCheck == NULL)
		return false;
	
	XMLNode IsUsePowerOnCheck = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//IsUsed"));
	if (IsUsePowerOnCheck == NULL)
		return false;

	XMLNode IsPowerCheckOnTime = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//PowerCheckOnTime"));
	if (IsPowerCheckOnTime == NULL)
		return false;

	XMLNode IsPS1Value = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//PS1Value"));
	if (IsPS1Value == NULL)
		return false;
	
	XMLNode IsPS1Check = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//PS1Check"));
	if (IsPS1Value == NULL)
		return false;

	XMLNode IsPS2Value = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//PS2Value"));
	if (IsPS2Value == NULL)
		return false;

	XMLNode IsPS2Check = m_DeviceItemXML.SearchNode(_T("//Configuration//TestProcessor//PowerOnCheck//PS2Check"));
	if (IsPS1Value == NULL)
		return false;

	CString str_IsUsePowerOnCheck = m_DeviceItemXML.GetNodeText(IsUsePowerOnCheck);
	m_b_IsUsePowerOnCheck = CStr::StrToBool(str_IsUsePowerOnCheck.GetString());

	//
	CString str_IsPowerCheckOnTime = m_DeviceItemXML.GetNodeText(IsPowerCheckOnTime);
	m_i_PowerCheckOnTime = CStr::StrToInt(str_IsPowerCheckOnTime.GetString());	

	m_str_PS1Value = m_DeviceItemXML.GetNodeText(IsPS1Value);
	m_str_PS2Value = m_DeviceItemXML.GetNodeText(IsPS2Value);

	CString str_PS1Check, str_PS2Check;
	str_PS1Check =  m_DeviceItemXML.GetNodeText(IsPS1Check);
	str_PS2Check =  m_DeviceItemXML.GetNodeText(IsPS2Check);

	m_i_PS1Check =  CStr::StrToInt(str_PS1Check.GetString());
	m_i_PS2Check =  CStr::StrToInt(str_PS2Check.GetString());	

	return true;
}


bool ITestProcessor::ParseTestItemParameters()
{
	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet"));
	if (processObjectSet == NULL)
		return false;

	XMLNodeList processObjectsCategoryList = processObjectSet->GetchildNodes();
	if (processObjectsCategoryList == NULL)
		return false;

	std::map<std::string, std::string> mapObjectParams;
	for (UINT j = 0; j < m_TestItemXML.GetNodeListSize(processObjectsCategoryList); ++j)
	{
		// Configuration//ProcessObjectSet//Category
		XMLNode processObjectsCategory = m_TestItemXML.GetNodeListItem(processObjectsCategoryList, j);
		if (processObjectsCategory == NULL)
			continue;

		XMLNodeList processObjectList = processObjectsCategory->GetchildNodes();
		if (processObjectList == NULL)
			continue;

		for (UINT k = 0; k < m_TestItemXML.GetNodeListSize(processObjectList); ++k)
		{
			// Configuration//ProcessObjectSet//Category//ProcessObject
			XMLNode processObject = m_TestItemXML.GetNodeListItem(processObjectList, k);
			if (processObject == NULL)
				continue;

			// Parse the process object
			mapObjectParams.clear();
			CString cstrName = m_TestItemXML.GetNodeAttrValue(processObject, _T("Name"));
			CString cstrClassName = m_TestItemXML.GetNodeAttrValue(processObject, _T("ClassName"));
			if (cstrName.IsEmpty() || cstrClassName.IsEmpty())
				continue;

			// Insert <Name, ClassName> to parameter map
			CT2A szName(cstrName);
			std::string strName(szName);
			CT2A szClassName(cstrClassName);
			std::string strClassName(szClassName);
			mapObjectParams[strName] = strClassName;

			XMLNodeList processObjectParamList = processObject->GetchildNodes();
			if (processObjectParamList == NULL)
				continue;
			for (UINT l = 0; l < m_TestItemXML.GetNodeListSize(processObjectParamList); ++l)
			{
				XMLNode processObjectParam = m_TestItemXML.GetNodeListItem(processObjectParamList, l);
				if (processObjectParam == NULL)
					continue;

				CString cstrParamName = m_TestItemXML.GetNodeName(processObjectParam);
				CString cstrParamData = m_TestItemXML.GetNodeText(processObjectParam);
				if (cstrParamName.IsEmpty() || cstrParamData.IsEmpty())
					continue;

				// Insert <ParamName, ParamData> to parameter map
				CT2A szParamName(cstrParamName);
				std::string strParamName(szParamName);
				CT2A szParamData(cstrParamData);
				std::string strParamData(szParamData);
				mapObjectParams[strParamName] = strParamData;
			}//for (UINT l = 0; l < m_TestItemXML.GetNodeListSize(processObjectParamList); ++l)
			// Insert all parameters in m_mapTestItemParams
			if (mapObjectParams.empty() != true)
				m_mapTestItemParams[strName] = mapObjectParams;
		}//for (UINT k = 0; k < m_TestItemXML.GetNodeListSize(processObjectList); ++k)
	}//for (UINT j = 0; j < m_TestItemXML.GetNodeListSize(processObjectsCategoryList); ++j)
	return true;
}

//Eason Check from Server S-----------------------------------------------
void ITestProcessor::RecordServerPara(  std::string Station , std::string Value  ) 
{
	m_map_parameterListSeverCheck[ Station ] = Value;	
}

std::string ITestProcessor::GetServerPara(  std::string Station  ) 
{

	for (std::map<std::string,std::string>::iterator Itr = m_map_parameterListSeverCheck.begin();Itr!= m_map_parameterListSeverCheck.end();Itr++)
	{
		if ( Itr->first == Station )
		{
			return Itr->second;
		}
	}
	return "";
}
//Eason Check from Server E-----------------------------------------------

//Notification
bool ITestProcessor::Register(INotify_ANSI* pNotify, const std::string& strEvent)
{
	m_piToolNotify = pNotify;
	return CNotifyBase_ANSI::Register(pNotify, strEvent);
}

bool ITestProcessor::Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}

bool ITestProcessor::Unregister(INotify_ANSI* pNotify, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}

bool ITestProcessor::Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}

void ITestProcessor::DefineNotify(const std::string& strNewEvent)
{
	CNotifyBase_ANSI::DefineNotify(strNewEvent);
}

void ITestProcessor::Fire(const std::string& strEvent, long nParam)
{
	CNotifyBase_ANSI::Fire(strEvent, nParam);
}

//DLL Event functions
//Logging file
void ITestProcessor::TraceLog(int iSeverity, std::string strMsg)
{
	sTraceLog traceLog;
	traceLog.iSeverity = iSeverity;
	traceLog.strMessage = strMsg;
	Fire(TRACE_LOG, (long)&traceLog);
}

void ITestProcessor::TraceLog(int iSeverity, std::string strMsg, int nQPhoneNumber)
{
	char szQphoneMsg[100] = {0};
	sprintf(szQphoneMsg, "[QPHONE%d] ", nQPhoneNumber);

	sTraceLog traceLog;
	traceLog.iSeverity = iSeverity;
	traceLog.strMessage = szQphoneMsg;
	traceLog.strMessage += strMsg;
	Fire(TRACE_LOG, (long)&traceLog);
}

//Factory log
void ITestProcessor::FactoryLog()
{
	Fire(FACTORY_LOG, NULL);
}

//Set PICS data
void ITestProcessor::SetPICSData(std::string str_key, std::string str_value)
{
	if (str_key.empty()) return;
	st_PICSData picsData;
	picsData.i_slot = 1;
	strcpy(picsData.sz_key, str_key.c_str());
	strcpy(picsData.sz_value, str_value.c_str());

	Fire(PICS_DATA, (long)&picsData);

	TraceLog(MSG_INFO, "SetPICS:[" + str_key + "]= " + str_value);
}

void ITestProcessor::SetPICSData(std::string str_key, std::string str_value, int nQPhoneNumber)
{
	if (str_key.empty()) return;
	st_PICSData picsData;
	picsData.i_slot = (m_i_QPhoneMultiCtrl == 0) ? 1 :nQPhoneNumber;
	strcpy(picsData.sz_key, str_key.c_str());
	strcpy(picsData.sz_value, str_value.c_str());

	Fire(PICS_DATA, (long)&picsData);

	TraceLog(MSG_INFO, "SetPICS:[" + str_key + "]= " + str_value, nQPhoneNumber);
}

//Show picture
void ITestProcessor::ShowPicture(bool b_append, const char* p_sz_path)
{
	st_UIPattern picture;
	picture.i_slot = 1;
	picture.b_append = b_append;
	sprintf_s(picture.sz_pattern, "%s", p_sz_path);
	Fire(SHOW_PICTURE, (long)&picture);
}

//Jig Request
void ITestProcessor::JigRequest(const char* p_sz_switch)
{
	st_JigRequest st_switch;
	st_switch.i_slot = 1;
	sprintf_s(st_switch.sz_switch, "%s", p_sz_switch);
	Fire(JIG_REQUEST, (long)&st_switch);
	return;
}

//Show Dialog
void ITestProcessor::ShowDialog(const char* p_sz_message)
{
	st_ShowDialog dialog;
	dialog.i_slot = 1;
	sprintf_s(dialog.sz_message, "%s", p_sz_message);
	Fire(SHOW_DIALOG, (long)&dialog);
}

bool ITestProcessor::CopyQXDMMobileLogToPC()
{
	if (!m_b_IsNeedQXDMLog) return false;

	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);
	CString currentPath;
	::GetModuleFileName(NULL, currentPath.GetBuffer(MAX_PATH), MAX_PATH);
	currentPath.ReleaseBuffer();
	currentPath = currentPath.Left(currentPath.ReverseFind('\\'));
	CString logPath;
	logPath.Format(_T("%s\\LOG\\%s\\%04d%02d%02d\\Mobile\\"), currentPath, g_str_station.c_str(), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	int i_DirCreateResult = SHCreateDirectoryEx(NULL, logPath, NULL);//Create save file directory path
	bool b_Res = false;
	switch (i_DirCreateResult)
	{
	case ERROR_SUCCESS:
	case ERROR_ALREADY_EXISTS:
		b_Res = true;
		break;
	default:
		CString errorMsg;
		errorMsg.Format(CString(L"创建路径%s失败!"), logPath);
		TraceLog(MSG_INFO, CStringA(errorMsg).GetString());
		b_Res = false;
		break;
	}
	if (b_Res)
	{
		/* Get current time */
		CString currTime;
		currTime.Format(_T("%04d%02d%02d_%02d%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		/* pull QXDM log to PC command */
		CString command;
		command.Format(_T("pull /data/%s_qxdm/diag_log_1.qmdl %s/%s_qxdm_%s.qmdl"), g_strPicasso.c_str(), logPath, g_strPicasso.c_str(), currTime);
		/* pull mobile QXDM log to PC */
		TraceLog(MSG_INFO, "Execute pull QXDM log adb command.");
		char errMsg[128] = {0};
		char cmdOut[1024] = {0};
		b_Res = m_pIPhone->ExecAdbCommand(command, cmdOut, errMsg);
		TraceLog(MSG_INFO, errMsg);
	}
	return b_Res;
}

//Execute Android Logcat
bool ITestProcessor::ExecAndroidLogcat(bool isPass)
{
	char szLogPath[1000] = "D:\\Log";
	_mkdir(szLogPath);
	strcat(szLogPath, "\\Android");
	_mkdir(szLogPath);
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);
	CString str_Date;
	str_Date.Format(_T("\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(szLogPath, str_Date);
	_mkdir(szLogPath);

	CString str_Picasso = _T("RD");
	if (!g_strPicasso.empty())
		str_Picasso = g_strPicasso.c_str();

	CString strPassFail = isPass ? _T("PASS") : _T("FAIL");
	CString msg;
	CString command;
	if (m_i_QPhoneMultiCtrl == 0)
	{
		CString csDevice;
		if (m_pIPhone->m_devices.size() > 1)
			csDevice.Format(_T("-s %s"), m_pIPhone->m_devices[0].c_str());
		command.Format(_T("/C adb %s logcat -v time -d > %s\\%s_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"),
			csDevice, szLogPath, strPassFail, str_Picasso,
			systemTime.wYear, systemTime.wMonth, systemTime.wDay,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = "cmd";
		ShExecInfo.lpParameters = command;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, 3000);

		msg.Format(_T("Execute: %s"), command);
		TraceLog(MSG_INFO, std::string(CT2A(msg)));
	}
	else
	{
		for (int nQPhoneNumber = 1; nQPhoneNumber <= m_i_QPhoneMultiCtrl; nQPhoneNumber++)
		{
			if ((m_b_UsableSlot[nQPhoneNumber] == true) && (m_b_isSlotPass[nQPhoneNumber] == isPass))
			{
				command.Format(_T("/C adb -s QPHONE%d logcat -v time -d > %s\\%s_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), m_nQPhoneOrderSlot[nQPhoneNumber],
				               szLogPath, strPassFail, m_csArrPicasso[nQPhoneNumber],
				               systemTime.wYear, systemTime.wMonth, systemTime.wDay,
				               systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				SHELLEXECUTEINFO ShExecInfo = {0};
				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				ShExecInfo.hwnd = NULL;
				ShExecInfo.lpVerb = "open";
				ShExecInfo.lpFile = "cmd";
				ShExecInfo.lpParameters = command;
				ShExecInfo.lpDirectory = NULL;
				ShExecInfo.nShow = SW_HIDE;
				ShExecInfo.hInstApp = NULL;
				ShellExecuteEx(&ShExecInfo);
				WaitForSingleObject(ShExecInfo.hProcess, 3000);

				msg.Format(_T("Execute: %s"), command);
				TraceLog(MSG_INFO, std::string(CT2A(msg)));
			}
		}
	}
	return true;
}

//upload log to server by BAT
bool ITestProcessor::LogUploadByBAT(char* str_modelname)
{
	if (!m_b_LogUpload) return true;

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char sysdate[100];
	sprintf(sysdate, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	char path_bat[2000];
	sprintf(path_bat, "%s\\Qisda\\Uplog.bat", szModulePath);
	ShExecInfo.lpFile = path_bat;

	char bat_param[2000];
	sprintf(bat_param, "%s %s %s %s", m_str_UploadServerIP.c_str(), str_modelname, str_ComputerName, sysdate);
	ShExecInfo.lpParameters = bat_param;

	ShellExecuteEx(&ShExecInfo);
	return true;
}

//upload log to server
bool ITestProcessor::LogUpload(char* str_modelname)
{
	if (!m_b_LogUpload) return true;

	m_strResult = "-";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char str_RemotePath[1000];
	char str_LocalPath[1000] = "D:\\Log";
	char str_BackupPath[1000] = "D:\\LogUpload";
	char temp[2000];

	//-- 建立遠端路徑
	strcpy(str_RemotePath, _T("\\\\"));
	strcat(str_RemotePath, m_str_UploadServerIP.c_str());
	strcat(str_RemotePath, _T("\\TestLog"));

	char str_account[100] = _T("mpics\\TestLog");
	char str_pswd[100] = _T("qisda2013");
	sprintf(temp, "/C net use %s /user:%s %s", str_RemotePath, str_account, str_pswd);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	//-- 測試建立連線是否成功
	sprintf(temp, "/C net use %s", str_RemotePath);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if (WaitForSingleObject(ShExecInfo.hProcess, 2000) == WAIT_TIMEOUT)
	{
		m_strResult = _T("FAIL");
		m_strMessage = "Fail to connect server uploading.";
		FactoryLog();
		return false;
	}

	_mkdir(str_RemotePath);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_modelname);
	_mkdir(str_RemotePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);
	strcat(str_RemotePath, "\\");
	strcat(str_RemotePath, str_ComputerName);
	_mkdir(str_RemotePath);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char systime[100];
	sprintf(systime, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(str_RemotePath, "\\");
	strcat(str_RemotePath, systime);
	_mkdir(str_RemotePath);

	int nUploadTimeout = 20000;

	//-- 複製所有檔案至遠端
	sprintf(temp, "/C xcopy /e /y %s %s", str_LocalPath, str_RemotePath);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if(WaitForSingleObject(ShExecInfo.hProcess, nUploadTimeout) == WAIT_TIMEOUT)
	{
		CString csMsg;
		csMsg.Format(_T("Log uploading timeout with %d sec"), nUploadTimeout / 1000);
		m_strMessage = _T(csMsg);
		FactoryLog();
	}

	//-- 確認資料完整傳送至遠端
	bool b_ok = DFSCheckFile(str_LocalPath, str_RemotePath, _T(""));

	//-- 複製log至本地指定目錄並刪除
	if (b_ok)
	{
		_mkdir(str_BackupPath);
		sprintf(temp, "/C xcopy /e /y /q %s %s", str_LocalPath, str_BackupPath);
		ShExecInfo.lpParameters = temp;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		DFSDeleteFile(str_LocalPath, _T(""));
		m_strMessage = "Check and Upload Pass.";
		FactoryLog();
		return true;
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Copy and Check upload file Fail.";
		FactoryLog();
		return false;
	}
}

/* Check upload file */
bool ITestProcessor::DFSCheckFile(char* str_LocalPath, char* str_RemotePath, char* str_TracePath)
{
	char szDir[1000];
	sprintf(szDir, "%s\\%s\\*", str_LocalPath, str_TracePath);
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	if ((hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE)	//此路徑沒下層路徑,有可能為檔案
	{
		sprintf(szDir, "%s\\%s", str_RemotePath, str_TracePath);			//確認遠端路徑下也有此檔案
		FindClose(hList);
		if (_taccess(szDir, 0) == -1) 										//找不到遠端路徑下檔案
		{
			std::string strMsg = str_RemotePath;
			strMsg += "\\";
			strMsg += str_TracePath;
			TraceLog(MSG_INFO, "Not to find the remote file: " + strMsg);
			return false;
		}
		else 																//找到遠端路徑下檔案
		{
			return true;
		}
	}
	char str_tempPath[1000];
	for (;;)
	{
		if (!FindNextFile(hList, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				FindClose(hList);
				return true;
			}
		}
		if (strcmp(FileData.cFileName, _T("..")) != 0)
		{
			strcpy(str_tempPath, str_TracePath);
			if (strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, "\\");
			strcat(str_tempPath, FileData.cFileName);
			if (!DFSCheckFile(str_LocalPath, str_RemotePath, str_tempPath))
			{
				FindClose(hList);
				return false;
			}
		}
	}
}

/* Delete local file */
bool ITestProcessor::DFSDeleteFile(char* str_LocalPath, char* str_TracePath)
{
	if (!m_b_LogUpload) return true;

	char szDir[1000];
	sprintf(szDir, "%s\\%s\\*", str_LocalPath, str_TracePath);
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	if ((hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE)//此路徑沒下層路徑,有可能為檔案
	{
		bool b_DelFile = true;
		//.csv檔不刪
		if(strstr(str_TracePath, ".CSV") != NULL)	b_DelFile = false;

		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		char TodayKeepLog[1000];

		//DB_Log今日檔不刪
		sprintf(TodayKeepLog, "DB_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//VMS_Log今日檔不刪
		sprintf(TodayKeepLog, "VMS_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//UI_Log今日檔不刪
		sprintf(TodayKeepLog, _T("UILog_%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL) b_DelFile = false;

		//統計csv今日檔不刪
		sprintf(TodayKeepLog, "%04d%02d%02d.csv", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//本站DLL資料夾今日log不刪
		sprintf(TodayKeepLog, "DLL\\%s\\%04d%02d%02d\\", g_str_station.c_str(), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//Android logcat今日檔不刪
		sprintf(TodayKeepLog, "Android\\%04d%02d%02d\\", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//EntranceTool_Log今日檔不刪
		sprintf(TodayKeepLog, "EntranceTool_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;


		if (b_DelFile)
		{
			sprintf(szDir, "%s\\%s", str_LocalPath, str_TracePath);
			remove(szDir);
		}
		//TraceLog(MSG_INFO, szDir);
		FindClose(hList);
		return true;
	}
	char str_tempPath[1000];
	for (;;)
	{
		if (!FindNextFile(hList, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				FindClose(hList);
				return true;
			}
		}
		if (strcmp(FileData.cFileName, _T("..")) != 0)
		{
			strcpy(str_tempPath, str_TracePath);
			if (strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, "\\");
			strcat(str_tempPath, FileData.cFileName);
			if (!DFSDeleteFile(str_LocalPath, str_tempPath))
			{
				FindClose(hList);
				return false;
			}
		}
	}
}

bool ITestProcessor::PullDeviceLogOut(bool b_res)
{
	if ((!b_res) && (m_i_QPhoneMultiCtrl == 0))
	{
		if (IsConnected())
			ExecAndroidLogcat(false);
	}
	else if (m_i_QPhoneMultiCtrl != 0)
	{
		if (IsConnectedMulti())
			ExecAndroidLogcat(false);
	}
	return true;
}

bool ITestProcessor::ShowPassFailOnDeviceLCD()
{
	char out_buf[FTD_BUF_SIZE] = {0};
	for (int nQPhoneNumber = 1; nQPhoneNumber <= m_i_QPhoneMultiCtrl; nQPhoneNumber++)
	{
		if ((m_b_isSlotPass[nQPhoneNumber] == true) && (m_b_UsableSlot[nQPhoneNumber] == true))
		{
			m_pIPhone->Display_LCD_RGB((2000 + nQPhoneNumber), m_nFtdTimeOut, "g", out_buf);
			CString csMsg;
			csMsg.Format(_T("Set QPhone%d LCD as GREEN color"), nQPhoneNumber);
			TraceLog(MSG_INFO, std::string(csMsg));
		}
	}
	return true;
}
