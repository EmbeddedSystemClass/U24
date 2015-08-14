#include "stdafx.h"
#include "GPProcessor.h"

using namespace StringUtility;
using namespace ComDeviceUtility;

CGPProcessor::CGPProcessor()
: m_pITestBox(NULL), 
  m_pIPhone_Base(NULL),
  m_pIPS(NULL),
  m_pIPS2(NULL),
  m_pIPS3(NULL),
  m_pIPS4(NULL),
  m_pXMLTestItem(NULL),
  m_pXMLConfig(NULL),
  m_pXMLCableloss(NULL)
{
	m_mapTestItemParams.clear();

	// All device
	m_strTestBoxClassName = "";
	m_mapTesterDeviceParams.clear();

	// Power supply
	m_iPSCount = 0;
	m_vPowerSupplyAddr.clear();
	m_mapPowerSupplyParams.clear();

	// Phone
	m_mapIPhone.clear();

	// GPIB
	m_bUsingGPIB = false;
	m_iGPIBInterfaceID = 0;
	m_iGPIBCardAddress = 0;
	m_mapITestBox.clear();
	
	// Others
	m_bStopped = false;
	m_bEraseOs = false;
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	sprintf_s(m_szFailMsg, sizeof(m_szFailMsg)/sizeof(char), "");
}

/**********************************************************
/* Test sequence 
***********************************************************/

bool CGPProcessor::OnAPPLaunch()
{
	TraceLog(__FUNCTION__);

	return true;
}


bool CGPProcessor::PreRun(int i_slot)
{
	SetSlot(i_slot);

	//if no picasso , still need to create log
	if (g_mapToolInfo.find(std::string("PICASSO")) == g_mapToolInfo.end())
	{
		char szLogName[MAX_PATH];
		string strTimestamp = CLogger::GetTimeStamp(); 
		sprintf_s(szLogName, MAX_PATH, "%s_[%d]",
			strTimestamp.c_str(), i_slot);
		m_pLog->SetLogName(szLogName);
		m_LogName=szLogName;
	}


	StartTotalDurationCounter();

	/* Check handset is connected */
	int iCurComPort;
	char szErrMsg[MAX_PATH];
	::ZeroMemory(szErrMsg, MAX_PATH) ;
	iCurComPort = atoi (this->g_mapToolInfo["CURRENT_COMPORT"].c_str());
	if (iCurComPort > 0)
	{
		if (IsCOMPortAvailable(iCurComPort, szErrMsg) == false)
		{
			std::string strMsg;
			strMsg = MakeSTDstring("IsCOMPortAvailable() fail, COM: %d, Return: \n%s", iCurComPort, szErrMsg);
			TraceLog(strMsg);

			FactoryLog(false, "PreRun", CommErr_No_Phone_Connected, "", "", "", "", "", "", "Fail to connect phone");
			return false;
		}
	}

	// Check and Create GPIB Test box
	m_bUsingGPIB = m_pXMLTestItem->child("Configuration").child("GPIB").child("UsingGPIB").text().as_bool();
	if (m_bUsingGPIB)
	{
		// 4. Detect GPIB instruments
		if (! DetectGPIBDevice())
		{
			TraceLog("Fail to detect GPIB devices");
			FactoryLog(false, "PreRun", CommErr_Detect_RF_Tester_Fail, "", "", "", "", "", "", "Fail to detect GPIB devices");
			sprintf_s(m_szFailMsg, sizeof(m_szFailMsg)/sizeof(char), "没有侦测到PowerSupply");
			return false;
		}

		std::vector<std::string> vDeviceList;
		if (! IGPIBDevice::GetDeviceList(vDeviceList))
		{
			TraceLog("Fail to get GPIB device list");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to get GPIB device list");
			return false;
		}

		// 5. Create power supply objects & tester device object
		if(!this->CreateTestBox(vDeviceList, i_slot))
		{
			TraceLog("Fail to create test box");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create test box");
			return false;
		}

		if(!this->CreatePS(vDeviceList))
		{
			TraceLog("Fail to create PS");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create PS");
			sprintf_s(m_szFailMsg, sizeof(m_szFailMsg)/sizeof(char), "Fail to create PS");
			return false;
		}
	}

	return true;
}

bool CGPProcessor::DetectAndCreatePhone()
{
	// Create phone object
	if(!this->CreatePhone(this->GetCurrentSlot()))
	{
		TraceLog("Fail to create phone");
		FactoryLog(false, "DetectAndCreatePhone", "", "", "", "", "", "", "", "Fail to create phone");
		return false;
	}


	return true;
}

bool CGPProcessor::RunTestingProcedureByName(const char *szItem , int i_slot)
{
	SetSlot(i_slot);

	//Log
	string strITem = szItem;
	std::string strLog = "RunTestingProcedureByName : " + strITem ;
	TraceLog(strLog);

	CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);   //pcObjectFactory指针实际指向CToolBaseObject::classCToolBaseObject.此处即相当于把CToolBaseObject::classCToolBaseObject赋值给pcObjectFactory
	CToolBaseObject* pcObject = NULL;

	// from szItem to find real classname
	pugi::xml_node myParm_node = m_pXMLTestItem->child("Configuration").child("Param").child(szItem);
	string classname = myParm_node.attribute("ClassName").value();

	//create class by classname
	if ((pcObject = pcObjectFactory->CreateObject(classname)) == NULL)
	{
		std::string strTmp = "Fail to create " + classname + " object , from Item" + strITem ;
		TraceLog(strTmp);
		FactoryLog(false, "PreRun", "", "", "", "", "", "", "", strTmp);
		return false;
	}

	//cast 
	ITestItem* piTestItem = dynamic_cast<ITestItem*>(pcObject);

	//ToDo: Fix me
	piTestItem->m_pIProc = this;
	piTestItem->m_NickName = szItem;

	// Run the object's InitData() API
	std::map<std::string, std::string> mapObjectParam;

 	if ( !piTestItem->InitData(mapObjectParam) )
	{
		std::string strTmp = "Fail to execute InitData() of " + piTestItem->m_NickName + " object";
		TraceLog(strTmp);
		FactoryLog(false, "InitData", "", "", "", "", "", "", "", strTmp);

		piTestItem->Release();
		return false;
	}

	// Run the object's PreRun() API
	if ( !piTestItem->PreRun() )
	{
		std::string strTmp = "Fail to execute PreRun() of " + piTestItem->m_NickName + " object";
		TraceLog(strTmp);
		FactoryLog(false, "PreRun", "", "", "", "", "", "", "", strTmp);

		piTestItem->Release();
		return false;
	}

	// Run the object's PreRun() API
	if ( !piTestItem->Run(i_slot) )
	{
		std::string strTmp = "Fail to execute Run() of " + piTestItem->m_NickName + " object";
		TraceLog(strTmp);
		//FactoryLog(false, "Run", "", "", "", "", "", "", "", strTmp);

		piTestItem->Release();
		return false;
	}

	piTestItem->Release();

	return true;
}

bool CGPProcessor::RunInitialSettingByName(const char *szItem, int i_slot)
{
	SetSlot(i_slot);

	//Log
	string strITem = szItem;
	std::string strLog = "RunInitialSettingByName : " + strITem ;
	TraceLog(strLog);

	CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// from szItem to find real classname
	pugi::xml_node myParm_node = m_pXMLTestItem->child("Configuration").child("Param").child(szItem);
	string classname = myParm_node.attribute("ClassName").value();

	//create class by classname
	if ((pcObject = pcObjectFactory->CreateObject(classname)) == NULL)
	{
		std::string strTmp = "Fail to create " + classname + " object , from Item" + strITem ;
		TraceLog(strTmp);
		FactoryLog(false, "Initialization()", "", "", "", "", "", "", "", strTmp);
		return false;
	}

	//cast 
	ITestItem* piTestItem = dynamic_cast<ITestItem*>(pcObject);

	//ToDo: Fix me
	piTestItem->m_pIProc = this;
	piTestItem->m_NickName = szItem;

	// Run the object's InitData() API
	std::map<std::string, std::string> mapObjectParam;

	if ( !piTestItem->InitData(mapObjectParam) )
	{
		std::string strTmp = "Fail to execute InitData() of " + piTestItem->m_NickName + " object";
		TraceLog(strTmp);
		FactoryLog(false, "InitData", "", "", "", "", "", "", "", strTmp);

		piTestItem->Release();
		return false;
	}

	// Run the object's RunInitialization() API
	if ( !piTestItem->Initialization() )
	{
		std::string strTmp = "Fail to execute Initialization() of " + piTestItem->m_NickName + " object";
		TraceLog(strTmp);
		FactoryLog(false, "Initialization()", "", "", "", "", "", "", "", strTmp);

		piTestItem->Release();
		return false;
	}

	piTestItem->Release();

	return true;
}

bool CGPProcessor::PostRun(int i_slot)
{
	SetSlot(i_slot);

	bool bRes = true;

	// 1. Clear all data
	m_mapTestItemParams.clear();

	// 2. Delete phone object
	if (m_pIPhone_Base != NULL)
	{
		m_pIPhone_Base->Release();
		m_pIPhone_Base = NULL;
	}

	return bRes;
}

bool CGPProcessor::Stop()
{
	m_bStopped = true;
	return true;
}


bool CGPProcessor::OnAPPEnd()
{
	// 1. Execute PostRun() API
	//PostRun(i_slot);



	// 2. Delete all other existing objects
	if (m_pIPS != NULL)
	{
		m_pIPS->Release();
		m_pIPS = NULL;
	}
	if (m_pIPS2 != NULL)
	{
		m_pIPS2->Release();
		m_pIPS2 = NULL;
	}
	if (m_pIPS3 != NULL)
	{
		m_pIPS3->Release();
		m_pIPS3 = NULL;
	}
	if (m_pIPS4 != NULL)
	{
		m_pIPS4->Release();
		m_pIPS4 = NULL;
	}

	if (m_pITestBox != NULL)
	{
		m_pITestBox->Release();
		m_pITestBox = NULL;
	}

	for (std::map<std::string, ITestBox*>::iterator itr = m_mapITestBox.begin();
		itr != m_mapITestBox.end();
		++itr)
	{
		if (itr->second != NULL)
			itr->second = NULL;
	}

	/*for (std::map<std::string, IVISADevice*>::iterator itr = m_mapITestBoxVISA.begin();
		itr != m_mapITestBoxVISA.end();
		++itr)
	{
		if (itr->second != NULL)
			itr->second = NULL;
	}*/


	return true;
}

/**********************************************************
/*  Create  TestBox , powersupply , phone , GPIB , XML
***********************************************************/
bool CGPProcessor::CreateTestBox(std::vector<std::string> vGPIBDeviceList, int iSlot)
{
	CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	std::string strClassName, strName;
	pugi::xml_node itemlist = m_pXMLTestItem->child("Configuration").child("TesterDevice");

	for (unsigned int i = 0; i < vGPIBDeviceList.size(); ++i)
	{
		for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
		{
			strClassName = item.attribute("ClassName").value();
			strName = item.attribute("Name").value();

			if(vGPIBDeviceList[i].find(strName) != std::string::npos)
			{
				
					if ((pcObject = pcObjectFactory->CreateObject(strClassName)) == NULL)
					{
						TraceLog("Fail to create tester device object");
						FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create tester device object");
						return false;
					}

					m_pITestBox = dynamic_cast<ITestBox*>(pcObject);

					m_strTestBoxClassName = strClassName;

					// Set log object to test box
					m_pITestBox->m_iSlot = iSlot;
					m_pITestBox->SetLog(m_pLog);

					if (m_pITestBox->InitData(vGPIBDeviceList[i],
						m_pITestBox->GetNI488Address(i),
						m_pITestBox->GetPrimaryAddress(i),
						m_pITestBox->GetSecondaryAddress(i))
						!= true)
					{
						m_pITestBox->Release();
						TraceLog("Fail to execute InitDate() of tester device object");
						FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to execute InitDate() of tester device object");
						return false;
					}

					// Collect all GPIB equipments
					int iSameEquip = 0;
					for (std::map<std::string, ITestBox*>::iterator itr = m_mapITestBox.begin();
						itr != m_mapITestBox.end();
						++itr)
					{
						if (itr->first == m_strTestBoxClassName)
						{
							iSameEquip ++;
							m_strTestBoxClassName  += MakeSTDstring("%d", iSameEquip);
						}
					}
					m_mapITestBox[m_strTestBoxClassName] = m_pITestBox;
				
			} // if cstEnable == "TRUE"	
		}
	}

	return true;
}

bool CGPProcessor::CreatePS(std::vector<std::string> vGPIBDeviceList)
{
	// Read power supply list
	pugi::xml_node pslist = m_pXMLTestItem->child("Configuration").child("PowerSupply");
	
	for(int iCounter = 1; iCounter <= pslist.child("PowerSupplyCount").text().as_int(); iCounter++)
	{
		int iPSAdd;
		std::string strAddressNode = MakeSTDstring("PS%dAddress", iCounter);
		iPSAdd = pslist.child("PowerSupplyList").child(strAddressNode.c_str()).text().as_int();
		if (iPSAdd != ERROR_INT)
			m_vPowerSupplyAddr.push_back(iPSAdd);
	}
	
	CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	std::string strClassName, strName;

	for (unsigned int i = 0; i < vGPIBDeviceList.size(); ++i)
	{
		for (pugi::xml_node item = pslist.child("Item"); item; item = item.next_sibling("Item"))
		{
			strClassName = item.attribute("ClassName").value();
			strName = item.attribute("Name").value();

			if (vGPIBDeviceList[i].find(strName) != std::string::npos)
			{
				if ((pcObject = pcObjectFactory->CreateObject(strClassName)) == NULL)
				{
					TraceLog("Fail to create power supply object");
					FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create power supply object");
					return false;
				}

				IPowerSupply* pIPS = dynamic_cast<IPowerSupply*>(pcObject);

				if (m_vPowerSupplyAddr[0] == pIPS->GetPrimaryAddress(i))
					m_pIPS = dynamic_cast<IPowerSupply*>(pcObject);
				else if (m_vPowerSupplyAddr[1] == pIPS->GetPrimaryAddress(i))
					m_pIPS2 = dynamic_cast<IPowerSupply*>(pcObject);
				else if (m_vPowerSupplyAddr[2] == pIPS->GetPrimaryAddress(i))
					m_pIPS3 = dynamic_cast<IPowerSupply*>(pcObject);
				else if (m_vPowerSupplyAddr[3] == pIPS->GetPrimaryAddress(i))
					m_pIPS4 = dynamic_cast<IPowerSupply*>(pcObject);
				else
				{
					pIPS->Release();
					continue;
				}

				if (pIPS->InitData(vGPIBDeviceList[i],
					pIPS->GetNI488Address(i),
					pIPS->GetPrimaryAddress(i),
					pIPS->GetSecondaryAddress(i))
					!= true)
				{
					pIPS->Release();
					TraceLog("Fail to execute InitDate() of power supply object");
					FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to execute InitDate() of power supply object");
					return false;
				}
				if ( false == pIPS->LoadPowerSupplySetting(m_pXMLTestItem))
				{
					pIPS->Release();
					TraceLog("Fail to Load power supply setting");
					FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to Load power supply setting");
					return false;
				}
			}
		}
	}

	return true;
}

bool CGPProcessor::CreatePhone(int iSlot)
{
	if (m_pIPhone_Base == NULL)
	{
		CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);   //定义了一个CToolRuntimeClass的指针，指向类CToolBaseObject;
		CToolBaseObject* pcObject = NULL;
		string ver = m_pXMLTestItem->child("Configuration").child("Phone").child("ClassName").child_value();
		std::string strPhone(ver);

		if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
		{
			TraceLog("Fail to create phone object");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create phone object");
			return false;
		}

		m_pIPhone_Base = dynamic_cast<IPhone*>(pcObject);

		m_pIPhone_Base->SetLog(m_pLog);

		// Set slot & comport to Phone
		m_pIPhone_Base->m_iSlot = iSlot;
		m_mapIPhone[iSlot] = m_pIPhone_Base; 
		m_pIPhone_Base->SetComPort(this->g_mapToolInfo["CURRENT_COMPORT"]);

		if (! m_pIPhone_Base->InitData(m_pXMLTestItem, m_pXMLConfig))
		{
			TraceLog("Fail to execute InitDate() of phone object");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to execute InitDate() of phone object");
			return false;
		}

		// Auto DetectDevice
		if (this->g_mapToolInfo["CURRENT_COMPORT"] == "0")
		{
			bool bDetected = false;
			for (int i = 0; i < 80; ++i)
			{
				if ( m_pIPhone_Base->DetectDevice())
				{
					bDetected = true;
					this->g_mapToolInfo["CURRENT_COMPORT"] = m_pIPhone_Base->GetComPort();
					break;		
				}

				Sleep(500);
				continue;
			}

			if (!bDetected)
			{
				std::string strMsg;
				strMsg = "Fail to auto detect device";
				TraceLog(strMsg);
				FactoryLog(false, "PreRun", CommErr_No_Phone_Connected, "", "", "", "", "", "", "Fail to connect phone");
				return false;
			}
		}
	}
	else
	{
		if (! m_pIPhone_Base->InitData(m_pXMLTestItem, m_pXMLConfig))
		{
			TraceLog("Fail to execute InitDate() of phone object");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to execute InitDate() of phone object");
			return false;
		}
	}

	//Set working dir to IPhone
	m_pIPhone_Base->SetWorkingDir(this->g_mapToolInfo["WORKING_DIR"].c_str());

	return true;
}

bool CGPProcessor::DetectGPIBDevice()
{
	if (! m_bUsingGPIB)
		return true;

	return IGPIBDevice::DetectGPIBDevice(m_iGPIBInterfaceID, m_iGPIBCardAddress);
}

bool CGPProcessor::SetXML( const char* sz_keyword, pugi::xml_document* pXML )
{
	std::string strKeyword = sz_keyword;

	if ( "TESTITEM" == strKeyword )		
	{
		this->m_pXMLTestItem = pXML;
	}

	if ( "CONFIG" == strKeyword )
	{
		this->m_pXMLConfig = pXML;
	}
	/*if ( "CABLELOSS" == strKeyword )
	{
		this->m_pXMLCableloss = pXML;
	}*/

	return true;
}



/**********************************************************
/*  Get info API for testitem
***********************************************************/

//double CGPProcessor::GetCableLoss(const std::string &strRfType, const std::string &strRfBand, std::string &strRfRange)
//{
//	pugi::xml_node node =m_pXMLCableloss->child("CableLoss").child(strRfType.c_str()).child(strRfBand.c_str()).child(strRfRange.c_str());
//	double dLoss = node.text().as_double(ERROR_DOUBLE);
//	if (ERROR_DOUBLE == dLoss)
//	{
//		TraceLog( MakeSTDstring("ERROR: CableLoss not found! (%s %s %s)", strRfType.c_str(), strRfBand.c_str(), strRfRange.c_str()) ) ;
//	}
//
//	return dLoss;
//}

ITestBox* CGPProcessor::GetTestBoxByName(std::string strTestBoxClassName)
{
	for (std::map<std::string, ITestBox*>::iterator itr = m_mapITestBox.begin();
		itr != m_mapITestBox.end();
		++itr)
	{
		if (itr->first == strTestBoxClassName)
			return itr->second;
	}

	return NULL;
}

//IVISADevice* CGPProcessor::GetTestBoxVisaByName(std::string strTestBoxClassName)
//{
//	for (std::map<std::string, IVISADevice*>::iterator itr = m_mapITestBoxVISA.begin();
//		itr != m_mapITestBoxVISA.end();
//		++itr)
//	{
//		if (itr->first == strTestBoxClassName)
//			return itr->second;
//	}
//
//	return NULL;
//}

IPhone* CGPProcessor::GetPhoneByName(string name)
{
	int iCurSlot;
	iCurSlot = GetCurrentSlot();
	for (std::map<int, IPhone*>::iterator itr = m_mapIPhone.begin();
		itr != m_mapIPhone.end();
		++itr)
	{
		if (itr->first == iCurSlot)
			return itr->second;
	}
	
	return NULL;
}





/**********************************************************
/*  Set/Get FA Data
***********************************************************/

bool CGPProcessor::SetFAData(char* sz_value)
{
	if (! m_pIPhone_Base->Initial())
	{
		TraceLog("Fail to initial Phone");
		FactoryLog(false, "SetFAData", "", "", "", "", "", "", "", "Fail to initial Phone");
		return false;
	}

	if (! m_pIPhone_Base->SetFAData(sz_value))
	{
		TraceLog("Fail to set FA");
		FactoryLog(false, "SetFAData", "", "", "", "", "", "", "", "Fail to set FA");
		return false;
	}

	FactoryLog(true, "SetFAData", "", "", "", "", "", "", "", "Set FA SUCCESS");
	return true;
}

bool CGPProcessor::GetSerialNumber(int i_slot, char* sz_value, int i_size)
{
	std::string strMsg;

	SetSlot(i_slot);

	// 1. Create Phone
	if (m_pIPhone_Base == NULL)
	{
		// Create phone object
		if(!this->CreatePhone(i_slot))
		{
			strMsg = "Fail to create phone";
			TraceLog(strMsg);
			FactoryLog(false, "GetSerialNumber", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
			return false;
		}
	}

	// 2. Initial
 	if (! m_pIPhone_Base->Initial())
	{
		strMsg = "Fail to initial";
		TraceLog(strMsg);
		FactoryLog(false, "GetSerialNumber", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
		return false;
	}

	// 3. Get Serial Number
	if (! m_pIPhone_Base->GetSerialNumber(sz_value, i_size))
	{
		strMsg = "Fail to get serial number";
		TraceLog(strMsg);
		FactoryLog(false, "GetSerialNumber", CommErr_Read_Serial_Number_Fail, "", "", "", "", "", "", strMsg);
		return false;
	}

	return true;
}

bool CGPProcessor::GetTimeStamp(int i_slot, char* sz_value, int i_size)
{
	std::string strMsg;

	SetSlot(i_slot);

	// 1. Create Phone
	if (m_pIPhone_Base == NULL)
	{
		// Create phone object
		if(!this->CreatePhone(i_slot))
		{
			strMsg = "Fail to create phone";
			TraceLog(strMsg);
			FactoryLog(false, "GetTimeStamp", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
			return false;
		}
	}

	// 2. Initial
	if (! m_pIPhone_Base->Initial())
	{
		strMsg = "Fail to initial";
		TraceLog(strMsg);
		FactoryLog(false, "GetTimeStamp", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
		return false;
	}

	// 3. Get GetTimeStamp
	if (! m_pIPhone_Base->GetTimeStamp(sz_value, i_size))
	{
		strMsg = "Fail to get GetTimeStamp";
		TraceLog(strMsg);
		FactoryLog(false, "GetTimeStamp", "", "", "", "", "", "", "", strMsg); //Need to add new error code
		return false;
	}

	return true;
}

void CGPProcessor::SetEvent(const char* szEvent, long nParam)
{
	std::string strEvent = szEvent;

	if ("UIMessage" == strEvent)
	{
		char* szMessage = (char*)nParam;
		std::string strMsg = szMessage;
		SetUIMessage(strMsg);
	}
	else if ("UIProgress" == strEvent)
	{
		sProgress* sPgs = (sProgress*)nParam;
		SetUIProgress(sPgs->iMainProgress, sPgs->iSubProgress);
	}
}

bool CGPProcessor::CreateAndInitPhone(int i_slot)
{

	std::string strMsg;

	SetSlot(i_slot);

	// 1. Create Phone
	if (m_pIPhone_Base == NULL)
	{
		// Create phone object
		if(!this->CreateMyPhone(i_slot))
		{
			strMsg = "Fail to create phone";
			TraceLog(strMsg);
			FactoryLog(false, "GetTimeStamp", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
			return false;
		}
	}

	// 2. Initial
	if (! m_pIPhone_Base->Initial())
	{
		strMsg = "Fail to initial";
		TraceLog(strMsg);
		FactoryLog(false, "GetTimeStamp", CommErr_No_Phone_Connected, "", "", "", "", "", "", strMsg);
		return false;
	}

	return true;

}

bool CGPProcessor::CreateMyPhone(int iSlot)
{
	if (m_pIPhone_Base == NULL)
	{
		CToolRuntimeClass* pcObjectFactory = GP_RUNTIME_CLASS(CToolBaseObject);   //定义了一个CToolRuntimeClass的指针，指向类CToolBaseObject;
		CToolBaseObject* pcObject = NULL;
		string ver = m_pXMLTestItem->child("Configuration").child("Phone").child("ClassName").child_value();
		std::string strPhone(ver);

		if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
		{
			TraceLog("Fail to create phone object");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to create phone object");
			return false;
		}

		m_pIPhone_Base = dynamic_cast<IPhone*>(pcObject);

		m_pIPhone_Base->SetLog(m_pLog);

		// Set slot & comport to Phone
		m_pIPhone_Base->m_iSlot = iSlot;
		m_mapIPhone[iSlot] = m_pIPhone_Base; 
		m_pIPhone_Base->SetComPort(this->g_mapToolInfo["CURRENT_COMPORT"]);

		if (! m_pIPhone_Base->InitData(m_pXMLTestItem, m_pXMLConfig))
		{
			TraceLog("Fail to execute InitDate() of phone object");
			FactoryLog(false, "PreRun", "", "", "", "", "", "", "", "Fail to execute InitDate() of phone object");
			//SendMsgToUi("Load XML date fail");
			sprintf_s(m_szFailMsg, sizeof(m_szFailMsg)/sizeof(char), "Initial XML fail");
			return false;
		}
	}

	//Set working dir to IPhone
	m_pIPhone_Base->SetWorkingDir(this->g_mapToolInfo["WORKING_DIR"].c_str());

	return true;
}

bool CGPProcessor::AutoDetectDevice() 
{
	// Auto DetectDevice

		bool bDetected = false;
		for (int i = 0; i < 80; ++i)
		{
			if ( m_pIPhone_Base->DetectDevice())
			{
				bDetected = true;
				//this->g_mapToolInfo["CURRENT_COMPORT"] = m_pIPhone_Base->GetComPort();
				break;
			}

			Sleep(250);
			continue;
		}

		if (!bDetected)
		{
			std::string strMsg;
			strMsg = "Fail to auto detect device";
			TraceLog(strMsg);
			//FactoryLog(false, "PreRun", CommErr_No_Phone_Connected, "", "", "", "", "", "", "Fail to connect phone");
			return false;
		}
		else
		{
			std::string strMsg;
			strMsg = "Successful to auto detect device";
			TraceLog(strMsg);
			//FactoryLog(true, "AutoDetectDevice", "", "", "", "", "", "", "", "Successful to connect phone");
			return true;
		}
}

bool CGPProcessor::SetIMEI(char* szvalue, int iSize)
{
	return m_pIPhone_Base->SetIMEI(szvalue,iSize);
}

bool CGPProcessor::SetSN(char* szvalue, int iSize)
{
	return m_pIPhone_Base->SetSN(szvalue,iSize);
}

bool CGPProcessor::ReadId(char *sz_value, int i_size)
{
	return m_pIPhone_Base->GetAndReturnId(sz_value,i_size);
}

bool CGPProcessor::LoadPhoneInfo(void)
{	
	return m_pIPhone_Base->LoadPhoneInfo();
}

void CGPProcessor::SetEraseFlag(bool bFlag)
{	m_bEraseOs = bFlag;
}

void CGPProcessor::SendMsgToUi(const char *szMsg)
{
	sprintf_s(m_st_UIMessage.sz_message,4096,szMsg); 
	m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
}

void CGPProcessor::SetFailMsg(const char *szMsg)
{
	sprintf_s(m_szFailMsg, sizeof(m_szFailMsg)/sizeof(char), szMsg);
}