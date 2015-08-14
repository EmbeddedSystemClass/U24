// FactoryToolController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Shlwapi.h>
#include <sstream>
#include "FactoryToolController.h"
#include "FactoryDef.h"
#include "utils/StringUtil.h"

using namespace std;
//using namespace QSYNC_LIB;
using namespace pugi;
using namespace StringUtility;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**********************************************************
Global function and variable for callback notify
***********************************************************/
CFactoryToolController* g_pController =NULL;

void NotifyToController (const char* szEvent, long nParam) 
{
	if (NULL ==g_pController)
	{
		return;
	}

	g_pController->Fire(szEvent,nParam);
}

void NotifyToProcessor(const char* szEvent, long nParam, int iSlot )
{
	if (NULL == g_pController)
	{
		return;
	}

	g_pController->SendEventToProcessor(szEvent, nParam, iSlot);
}



/**********************************************************
CFactoryToolController
***********************************************************/

CFactoryToolController::CFactoryToolController(void)
{
	//
	this->SetLogName();
	m_log.TraceLog(  __FUNCTION__ );

	g_pController = this;

	m_mapProcessor.clear();
	m_mapDeviceStruct.clear();
	m_mapPicasso.clear();
	m_mapResult.clear();
	
	m_bProcessorReady = false;
	m_bForceToPass = false;

	m_pFacTestToolInterfaceCallback = NULL;
}

CFactoryToolController::~CFactoryToolController(void)
{
	// global
	g_pController = NULL;

	m_mapProcessor.clear();
	m_mapDeviceStruct.clear();
	m_mapPicasso.clear();
	m_mapResult.clear();
}


/**********************************************************
/* Set/Get Parameter Value 
***********************************************************/

bool CFactoryToolController::SetParameterValue(char* sz_keyword, char* sz_value, int i_slot)
{
	std::string strKey(sz_keyword), strValue(sz_value);

	// Add to map
	if (i_slot == -1)
	{
		m_Parametermap[strKey] = strValue;
	}
	else
	{
		std::string strData;

		if (m_Parametermap[strKey].size() <= 0)
		{
			strData = MakeSTDstring("%d,%s", i_slot, sz_value);
		}
		else
		{
			strData = MakeSTDstring(";%d,%s", i_slot, sz_value);
		}

		m_Parametermap[strKey].append(strData);
	}

	// Log it
	string strTmp = MakeSTDstring("UI Parameter [%s] : %s", sz_keyword, sz_value );
	m_log.TraceLog( strTmp.c_str());

	// Set to GPProcessor
	if (m_bProcessorReady)
	{
		if(strKey == "PICASSO")
		{
			if (!SetPICASSO(sz_keyword, sz_value, i_slot))
				return false;
		}
		else
		{
			if (i_slot == -1)
			{
				for (std::map<int, CGPProcessor*>::iterator itr = m_mapProcessor.begin();
					itr != m_mapProcessor.end();
					++itr)
				{
					if( NULL == itr->second)
						return false;

					if (!itr->second->SetParameterValue(sz_keyword, sz_value))
						return false;
				}
			}
			else
			{
				if (!m_mapProcessor[i_slot]->SetParameterValue(sz_keyword, sz_value))
					return false;
			}
		}
	}

	return true;
}

bool CFactoryToolController::SetPICASSO(char* sz_keyword, char* sz_value, int i_slot)
{
	if (m_mapDeviceStruct.size() == 1) //SingleDevice
	{
		i_slot = DEFAULT_SLOT;
	}
	else //MultiDevice
	{
		if (i_slot == -1)
		{
			return false;
		}
	}

	std::string strValue(sz_value);
	m_mapPicasso[i_slot] = strValue;

	// Set to GPProcessor
	char szSlot[8] = {0};
	sprintf_s(szSlot, 8, "%d", i_slot);
	m_mapProcessor[i_slot]->SetParameterValue(KEYWORD_CURSLOT, szSlot);

	if (!m_mapProcessor[i_slot]->SetParameterValue(sz_keyword, sz_value))
		return false;

	return true;
}

bool CFactoryToolController::GetParameterValue(char* sz_keyword, char* sz_value, int i_size, int i_slot)
{
	if (i_slot == -1)
	{
		if (m_Parametermap.find(std::string(sz_keyword)) == m_Parametermap.end())
		{
			string strTmp = MakeSTDstring( "Fail to find value with keyword: %s" , sz_keyword);
			m_log.TraceLog(strTmp.c_str());
			return false;
		}

		std::string strValue = m_Parametermap[std::string(sz_keyword)];
		sprintf_s(sz_value, (i_size - 1), "%s", strValue.c_str());
	}
	else
	{		
		if (!m_mapProcessor[i_slot]->GetParameterValue(sz_keyword, sz_value, i_size))
		return false;
	}

	return true;
}


/**********************************************************
/* Set/Get FA Data 
***********************************************************/

bool CFactoryToolController::SetFAData(int i_slot, char* sz_value)
{
	if(this->m_mapProcessor[i_slot])
		return this->m_mapProcessor[i_slot]->SetFAData( sz_value);
		//return this->m_mapProcessor[i_slot]->SetFAData(i_slot, sz_value);

	return false;
}

bool CFactoryToolController::GetSerialNumber(int i_slot, char* sz_value, int i_size)
{
	if(this->m_mapProcessor[i_slot])
		return this->m_mapProcessor[i_slot]->GetSerialNumber(i_slot, sz_value, i_size);

	return false;
}

bool CFactoryToolController::GetTimeStamp(int i_slot, char* sz_value, int i_size)
{
	if(this->m_mapProcessor[i_slot])
		return this->m_mapProcessor[i_slot]->GetTimeStamp(i_slot, sz_value, i_size);

	return false;
}

/**********************************************************
/* Test sequence  when UI open and Close
***********************************************************/

bool CFactoryToolController::Begin(int i_slot)
{
	m_log.TraceLog(  __FUNCTION__ );

	//set working DIR , log DIR , config dir
	this->InitDirParameter();

	//open xml 
	if(!this->InitXML())
		return false;

	this->GetCOMPortList();

	if (!this->InitGPProcessor())
		return false;

	return true;
}


int CFactoryToolController::GetItemCount()
{
	m_log.TraceLog(  __FUNCTION__ );

	// push to vector
	string strName, strEnable;
	pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("TestItemList");
	for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
	{
		strName = item.attribute("Name").value();
		strEnable = item.attribute("Enabled").value();

		if( "TRUE" == strEnable)
		{
			this->m_vTestItemList.push_back(strName);
		}	
	}

	return this->m_vTestItemList.size();
}

bool CFactoryToolController::GetItemByIndex(int i_index, char* sz_name, int i_size)
{
	//Request index larger than vector size
	if(i_index >= (int)this->m_vTestItemList.size())
		return false;

	strcpy_s(sz_name, i_size, this->m_vTestItemList[i_index].c_str() );

	return true;
}

bool CFactoryToolController::End(int i_slot)
{
	m_log.TraceLog(  __FUNCTION__ );

	for (std::map<int, CGPProcessor*>::iterator itr = m_mapProcessor.begin();
		itr != m_mapProcessor.end();
		++itr)
	{
		if( NULL != itr->second)
		{
			if(!itr->second->OnAPPEnd())
				continue;

			delete itr->second;
			itr->second = NULL;
		}
	}

	m_mapProcessor.clear();

	return true;
}

bool CFactoryToolController::RegCallBack(p_fn_FacTestToolInterface_callback pCallback)
{
	m_pFacTestToolInterfaceCallback = pCallback;

	return true;
}

/**********************************************************
/* Test sequence  when UI click GO 
***********************************************************/

bool CFactoryToolController::PreRun(int i_slot)
{
	//Reset Log timer
	m_log.TraceLog(  __FUNCTION__ , i_slot );
	m_log.TraceLog( CLogger::GetTimeStamp().c_str() , i_slot );

	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
		return false;

	// Call GCProcessor PreRun() 
	if(!this->m_mapProcessor[i_slot])
		return false;

	// Clean error code
	this->m_mapDeviceStruct[i_slot].strErrorCode = "";

	if(!this->m_mapProcessor[i_slot]->PreRun(i_slot))
		return false;

	// RunTestItemByName  that type ="PRERUN" 
	this->m_mapResult[i_slot] = 0;
	string strName , strType , strEnable;
	pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("TestItemList");
	for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
	{
		strName = item.attribute("Name").value();
		strType = item.attribute("Type").value();
		strEnable = item.attribute("Enabled").value();

		if( "TRUE" == strEnable  && "PRERUN" == strType)
		{
			if ( false == this->m_mapProcessor[i_slot]->RunTestingProcedureByName( strName.c_str() , i_slot ))
			{
				//m_bForceToPass=false;
				this->m_mapResult[i_slot] = 1;
				if (m_bForceToPass)
					continue;

				break;
			}
		}
	}

	return NOERROR == this->m_mapResult[i_slot];
}

bool CFactoryToolController::Run(int i_slot)
{
	m_log.TraceLog(  __FUNCTION__ , i_slot );
	m_log.TraceLog( CLogger::GetTimeStamp().c_str() , i_slot );

	if(!this->m_mapProcessor[i_slot])
		return false;
	long i = 0;
	// RunTestItemByName  that type ="" 
	string strName , strType , strEnable;
	pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("TestItemList");
	for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
	{
		strName = item.attribute("Name").value();
		strType = item.attribute("Type").value();
		strEnable = item.attribute("Enabled").value();

		if( "TRUE" == strEnable  && "" == strType )
		{
			//this->m_mapProcessor[i_slot]->reg_event_callback("UINextItem",&i);
			this->Fire("UINextItem",i);
			if ( false == this->m_mapProcessor[i_slot]->RunTestingProcedureByName( strName.c_str() , i_slot ))
			{
 				this->m_mapResult[i_slot] = 1;
				if (m_bForceToPass)
					continue;

				break;
			}
			i++;
		}
	}

	//RunRecoverItemByName,当Fail时,进入RecoverItemList
	if (this->m_mapResult[i_slot] == 1 && true == this->m_mapProcessor[i_slot]->m_bEraseOs)
	{
		i = (long)m_vTestItemList.size();
		pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("RecoverItemList");
		for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
		{
			strName = item.attribute("Name").value();
			strType = item.attribute("Type").value();
			strEnable = item.attribute("Enabled").value();

			if( "TRUE" == strEnable  && "" == strType )
			{
				//this->m_mapProcessor[i_slot]->reg_event_callback("UINextItem",&i);
				this->Fire("UINextItem",i);
				if ( false == this->m_mapProcessor[i_slot]->RunTestingProcedureByName( strName.c_str() , i_slot ))
				{
 					this->m_mapResult[i_slot] = 1;
					if (m_bForceToPass)
						continue;

					break;
				}
				i++;
			}
		}
	}

	return NOERROR == this->m_mapResult[i_slot];
}

bool CFactoryToolController::PostRun(int i_slot)
{
	m_log.TraceLog(  __FUNCTION__  , i_slot);
	m_log.TraceLog( CLogger::GetTimeStamp().c_str() , i_slot );

	if(!this->m_mapProcessor[i_slot])
		return false;

	// RunTestItemByName  that type ="POSTRUN" 
	string strName , strType , strEnable;
	pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("TestItemList");
	for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
	{
		strName = item.attribute("Name").value();
		strType = item.attribute("Type").value();
		strEnable = item.attribute("Enabled").value();

		if( "TRUE" == strEnable && "POSTRUN" == strType )
		{
			if ( false == this->m_mapProcessor[i_slot]->RunTestingProcedureByName( strName.c_str() , i_slot ))
			{
				this->m_mapResult[i_slot] = 1;
				if (m_bForceToPass)
					continue;

				break; 
			} 
		}	
	}

	// Call GCProcessor PostRun() 
	if (this->m_mapProcessor.size() == 1)
	{
		m_Parametermap["PICASSO"] = "";
		this->m_mapProcessor[i_slot]->PostRun(i_slot);
	}
	else if (this->m_mapPicasso.size() == 1)
	{
		for (std::map<int, CGPProcessor*>::iterator itr = m_mapProcessor.begin();
			itr != m_mapProcessor.end();
			++itr)
		{
			if( NULL != itr->second)
			{
				itr->second->PostRun(itr->first);
			}
		}
	}

	m_mapPicasso.erase(i_slot);
	m_mapResult.erase(i_slot);

	return  NOERROR == this->m_mapResult[i_slot];
}

bool CFactoryToolController::Stop(int i_slot)
{
	return true;
}

void CFactoryToolController::InitDirParameter()
{
	//Get module path as working dir
	char szModulePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpecA(szModulePath);
	this->m_Parametermap[KEYWORD_WORKINGDIR] = MakeSTDstring("%s\\", szModulePath);

	//Log dir
	this->m_Parametermap[KEYWORD_LOGDIR] = "D:\\Log\\DLL\\";

	// config dir
	this->m_Parametermap[KEYWORD_CONFIGDIR] = MakeSTDstring("C:\\PE_Config\\%s\\Setting\\", this->m_Parametermap[KEYWORD_MODELNAME].c_str());
}

void CFactoryToolController::SetTestResult(int i_slot)
{
	st_UIResult Result;

	ZeroMemory(&Result, sizeof st_UIResult);
	Result.i_slot = i_slot;

	sprintf_s(Result.sz_result, MAX_BUFFER_SIZE, "%s", this->m_mapResult[i_slot] == NOERROR ? "PASS" : "FAIL");
	
	sprintf_s(Result.sz_errCode, MAX_BUFFER_SIZE, "%s", this->m_mapResult[i_slot] == NOERROR ? "PASS" : this->m_mapDeviceStruct[i_slot].strErrorCode.c_str());

	sprintf_s(Result.sz_msg, MAX_BUFFER_SIZE, "%s", "Finish Testitems in RUN phase");

//	this->Fire(UI_RESULT, (long)&Result);
}





bool CFactoryToolController::CreateProcessorForEachSlot()
{
	m_log.TraceLog(__FUNCTION__);

	int iCount = 0;
	for (std::map<int, DeviceStruct>::iterator itr = m_mapDeviceStruct.begin();
		itr != m_mapDeviceStruct.end();
		++itr)
	{
		if (itr->second.iCOMPort != 0)
		{
			iCount++;
			string strTmp = MakeSTDstring("newGPProcessCount:%d, Slot:%d, ComPort:%d", iCount, itr->first, itr->second.iCOMPort);
			m_log.TraceLog(strTmp.c_str());

			m_mapProcessor[itr->first] = new CGPProcessor();
		}
	}

	if (m_mapProcessor.empty())
	{
		iCount++;
		string strTmp = MakeSTDstring("newGPProcessCount:%d, Slot:%d, ComPort:%d, Need to auto DetectDevice!", iCount, DEFAULT_SLOT, 0);
		m_log.TraceLog(strTmp.c_str());

		m_mapProcessor[DEFAULT_SLOT] = new CGPProcessor();		
	}

	string strTmp = MakeSTDstring("----- Total GPProcess Count: %d -----", iCount);
	m_log.TraceLog(strTmp.c_str());


	return true;
}


bool CFactoryToolController::SetLogName()
{
	m_log.TraceLog(  __FUNCTION__);

	char szLogName[MAX_PATH];
	memset(szLogName, 0, MAX_PATH);

	string strTimestamp = CLogger::GetTimeStamp();  

	sprintf_s(szLogName, MAX_PATH, "%s_controller", strTimestamp.c_str() );
	
	m_log.SetLogName(szLogName);

	return true;
}







/**********************************************************
Init XML
***********************************************************/

bool CFactoryToolController::InitXML()
{
	m_log.TraceLog(__FUNCTION__);

	// test item XML
	string strTmp;

	std::string strStationName = m_Parametermap[KEYWORD_STATIONNAME];
	std::string strModelName = m_Parametermap[KEYWORD_MODELNAME];
	std::string strWorkDir = m_Parametermap[KEYWORD_WORKINGDIR];
	std::string strConfigDir = m_Parametermap[KEYWORD_CONFIGDIR];

	// 1. Determine test item XML file name
	string strTestItemPath = strWorkDir+ "Qisda\\" +strModelName + "_" + strStationName + std::string("_TestItem.xml");
	strTmp = MakeSTDstring("Test item XML path: %s",strTestItemPath.c_str() );
	m_log.TraceLog(strTmp.c_str());

	pugi::xml_parse_result resultTestITem = m_xmlTestItem.load_file (strTestItemPath.c_str());
	if ( pugi::status_ok != resultTestITem.status )
	{
		strTmp = MakeSTDstring("Fail to load Test item XML, %s", resultTestITem.description());
		m_log.TraceLog(  strTmp.c_str());
		return false;
	}

	// 2. Determine config XML file name
	string strConfigPath = strConfigDir + strModelName + "_" + strStationName + std::string("_Config.xml");
	strTmp = MakeSTDstring("Config XML path: %s",strConfigPath.c_str() );
	m_log.TraceLog(strTmp.c_str());

	pugi::xml_parse_result resultConfig = m_xmlConfig.load_file (strConfigPath.c_str());
	if ( pugi::status_ok != resultConfig.status )
	{
		strTmp = MakeSTDstring("Fail to load Test item XML, %s", resultConfig.description());
		m_log.TraceLog( strTmp.c_str());
		return false;
	}

	return true;
}

void CFactoryToolController::GetCOMPortList()
{
	m_log.TraceLog(__FUNCTION__);

	/* Init */
	DeviceStruct st_device = {0};

	int iDevice = 0;
	std::string strComPort = "";
	std::string strComPortList = "";

	// Assign com port number
	pugi::xml_node myParm_node_Phone = m_xmlConfig.child("Configuration").child("Phone");
	for (int i=0; i<MAX_SLOT; i++)
	{
		int iComPort = 0;
		char szSlot[8] = {0};
		sprintf_s(szSlot, 8, "Slot_0%d", i);

		strComPort =  myParm_node_Phone.child(szSlot).text().as_string(ERROR_STRING);
		if (strComPort == ERROR_STRING)
		{
			strComPort = "0";
		}
		else
		{
			iDevice++;
		}

		iComPort = atoi(strComPort.c_str());
		strComPortList.append(strComPort);

		if (i != (MAX_SLOT - 1)) 
		{
			strComPortList.append(",");
		}

		/* Save to Map */
		if (iComPort == 0)
		{
			st_device.strErrorCode = "No_Phone_Connected";
		}
		else
		{
			st_device.strErrorCode = "";
		}
		st_device.iCOMPort = iComPort;

		m_mapDeviceStruct[i] = st_device;
	}

	// Set default com port 
	if (iDevice == 0)
	{
		strComPortList = "0";
		m_mapDeviceStruct.clear();

		/* Save to Map */
		st_device.iCOMPort = 0;
		st_device.strErrorCode = "No_Phone_Connected";
		m_mapDeviceStruct[DEFAULT_SLOT] = st_device;
	}

	this->m_Parametermap[KEYWORD_COMPORTLIST] = strComPortList;
}


bool CFactoryToolController::InitGPProcessor()
{
	// New GCProcessor
	if(!this->CreateProcessorForEachSlot())
		return false;

	// Get ToolVersion
	string ver = m_xmlTestItem.child("Configuration").child("ToolInfo").child("TOOL_VERSION").child_value();
	m_Parametermap[KEYWORD_TOOLVERSION] = ver.c_str();

	// Get ForceToPass
	m_bForceToPass = m_xmlTestItem.child("Configuration").child("TestProcessor").child("ForceToPass").text().as_bool();

	for (std::map<int, CGPProcessor*>::iterator itr = m_mapProcessor.begin();
		itr != m_mapProcessor.end();
		++itr)
	{
		if( NULL == itr->second)
		{
			return false;
		}

		// Reg callback ASAP
		itr->second->reg_event_callback(NotifyToController);
		itr->second->reg_processor_callback(NotifyToProcessor);

		// Set XML
		itr->second->SetXML("TESTITEM", &m_xmlTestItem);
		itr->second->SetXML("CONFIG", &m_xmlConfig);

		// Set parameters
		char szKeyword[MAX_PATH];
		char szValue[MAX_BUFFER_SIZE];
		for (std::map<std::string, std::string>::iterator itrParaMap = m_Parametermap.begin();
			itrParaMap != m_Parametermap.end();
			++itrParaMap)
		{
			memset(szKeyword, 0, MAX_PATH);
			memset(szValue, 0, MAX_BUFFER_SIZE);

			sprintf_s(szKeyword, MAX_PATH, "%s", itrParaMap->first.c_str());
			sprintf_s(szValue, MAX_BUFFER_SIZE, "%s", itrParaMap->second.c_str());

			if (!itr->second->SetParameterValue(szKeyword, szValue))
				return false;
		}

		// Set current ComPortNumber
		char szComPort[8] = {0};
		sprintf_s(szComPort, 8, "%d", m_mapDeviceStruct[itr->first].iCOMPort);
		itr->second->SetParameterValue(KEYWORD_CURCOMPORT, szComPort);

		// Run OnAPPLaunch
		if(!itr->second->OnAPPLaunch())
			return false;
	}

	m_bProcessorReady = true;

	return true;
}


void CFactoryToolController::SendEventToProcessor(const char* szEvent, long nParam, int iSlot)
{
	if (m_mapProcessor[iSlot] != NULL)
	{
		m_mapProcessor[iSlot]->SetEvent(szEvent, nParam);
	}
}

void CFactoryToolController::Fire(const char* szEvent, long nParam)
{
	std::string strEvent(szEvent);

	if(strEvent == UI_RESULT)
	{
		st_UIResult *pst_UIResult = (st_UIResult*)nParam;
		std::string strResult;
		const char* szResult;
		char szUpperResult[8] = {0};
		int i = -1;

		strResult = pst_UIResult->sz_errCode;
		szResult = strResult.c_str();

		while(szResult[i++])
		{
			if (i >= 5)
				break;

			szUpperResult[i] = toupper(szResult[i]);
		}
		strResult = szUpperResult;

		if (!(strResult == "PASS"))
			this->m_mapDeviceStruct[pst_UIResult->i_slot].strErrorCode = pst_UIResult->sz_errCode;
	}

	m_pFacTestToolInterfaceCallback(szEvent, nParam);
}

bool CFactoryToolController::CreateAndInitPhone (const int i_slot)
{
	if(this->m_mapProcessor[i_slot])
		return this->m_mapProcessor[i_slot]->CreateAndInitPhone(i_slot);

	return false;
}

int CFactoryToolController::GetRecoverItemCount()
{
	m_log.TraceLog(  __FUNCTION__ );

	// push to vector
	string strName, strEnable;
	pugi::xml_node itemlist = m_xmlTestItem.child("Configuration").child("RecoverItemList");
	for (pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling("Item"))
	{
		strName = item.attribute("Name").value();
		strEnable = item.attribute("Enabled").value();

		if( "TRUE" == strEnable)
		{
			this->m_vRecoverItemList.push_back(strName);
		}	
	}

	return this->m_vRecoverItemList.size();

}

bool CFactoryToolController::SetIMEI(int i_slot,char* sz_value,int i_size)
{
	if(this->m_mapProcessor[i_slot])
	{	return this->m_mapProcessor[i_slot]->SetIMEI(sz_value,i_size);
	}
	return false;
}

bool CFactoryToolController::SetSN(int i_slot,char* sz_value,int i_size)
{
	if(this->m_mapProcessor[i_slot])
	{	return this->m_mapProcessor[i_slot]->SetSN(sz_value,i_size);
	}
	return false;
}

bool CFactoryToolController::ReadId(int i_slot, char *sz_value, int i_size)
{
	if(this->m_mapProcessor[i_slot])
	{	return this->m_mapProcessor[i_slot]->ReadId(sz_value,i_size);
	}
	return false;
}

bool CFactoryToolController::LoadPhoneInfo(int i_slot)
{
	if(this->m_mapProcessor[i_slot])
	{	return this->m_mapProcessor[i_slot]->LoadPhoneInfo();
	}
	return false;
}

bool CFactoryToolController::GetRecoverItemByIndex(int i_index, char *sz_name, int i_size)
{
	//Request index larger than vector size
	if(i_index >= (int)this->m_vRecoverItemList.size())
		return false;

	strcpy_s(sz_name, i_size, this->m_vRecoverItemList[i_index].c_str() );

	return true;
}

bool CFactoryToolController::GetFailItem(int i_slot, char* sz_value, int i_size)
{
	sprintf_s(sz_value, i_size, this->m_mapProcessor[i_slot]->m_szFailMsg);

	return true;
}

/**********************************************************
Export functions in FactoryToolController.dll
***********************************************************/

FACTORYTOOLCONTROLLER_API bool NewInterface(IFacTestToolInterface** ppIFacTestToolInterface)
{
	bool bRes = true;

	*ppIFacTestToolInterface = new CFactoryToolController;

	CFactoryToolController* pToolInstance = dynamic_cast<CFactoryToolController*>(*ppIFacTestToolInterface);

	if (!pToolInstance)
	{
		delete pToolInstance;
		*ppIFacTestToolInterface = NULL;
		bRes = false;
	}

	return bRes;
}

FACTORYTOOLCONTROLLER_API bool DeleteInterface(IFacTestToolInterface* pIFacTestToolInterface)
{
	if (pIFacTestToolInterface)
	{
		delete dynamic_cast<CFactoryToolController*>(pIFacTestToolInterface);
		pIFacTestToolInterface = NULL;
	}

	return true;
}
