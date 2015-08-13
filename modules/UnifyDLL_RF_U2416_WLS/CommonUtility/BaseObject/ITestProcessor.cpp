#include "stdafx.h"
#include "ITestProcessor.h"
#include <io.h>
#include <direct.h>

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)

IPowerSupply* ITestProcessor::m_pIPS = NULL;
IPowerSupply* ITestProcessor::m_pIPS2 = NULL;
IPowerSupply* ITestProcessor::m_pIPS3 = NULL;
ITesterDevice* ITestProcessor::m_pITesterDevice = NULL;
ITesterDevice* ITestProcessor::m_pITesterDeviceFSP = NULL;
IPhone* ITestProcessor::m_pIPhone = NULL;

std::string ITestProcessor::g_str_modelName;
std::string ITestProcessor::g_str_station;

std::map<std::string, std::string> ITestProcessor::g_mapToolInfo;
std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapTxCableLossData;
std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapRxCableLossData;
std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapTxRefResultData;
std::map<std::string, std::map<std::string, std::string>> ITestProcessor::g_mapRxRefResultData;

clock_t ITestProcessor::g_totalTestDuration_Start;
clock_t ITestProcessor::g_testItemDuration_Start;

std::string ITestProcessor::g_strErrCode;
std::string ITestProcessor::g_strErrMsg;

CString ITestProcessor::m_cstrCableLossPath;
CString ITestProcessor::m_cstrTestItemFile;
//std::map<std::string, std::string> ITestProcessor::g_mapCableLossMatix;
std::string ITestProcessor::g_strPicasso;
CCPKLogItem ITestProcessor::gCpkRecord;

bool ITestProcessor::m_b_LogUpload = false;
std::string ITestProcessor::m_str_UploadServerIP = _T("10.85.68.13");

int ITestProcessor::m_nFtdPort = 2000;
int ITestProcessor::m_nFtdTimeOut = 15000;

// 
// CString ITestProcessor::m_strPicassoNum = _T("");
// CString ITestProcessor::m_strSWVersion = _T("");
// CString ITestProcessor::m_strToolMode = _T("");
// CString ITestProcessor::m_strModeName= _T("GCHIB1A");
// CString ITestProcessor::m_stationName = _T("");
// CString ITestProcessor::m_strToolVersion = _T("");


RF_IMPLEMENT_DYNAMIC(ITestProcessor)


ITestProcessor::ITestProcessor()
	: m_bForceToPass(false)
	, m_bUsingGPIB(false)
	, m_bUsingJig(false)
	, m_iGPIBCardAddress(0)
	, m_iGPIBInterfaceID(0)
	, m_iPSCount(0)
	,m_bCalibrationMode(false)
{
	DefineNotify(DLL_PATTERN);
	DefineNotify(DLL_TESTING_ITEM);
	DefineNotify(DLL_MESSAGE);
	DefineNotify(DLL_TEST_RESULT);
	DefineNotify(DLL_PICSDATA);
	DefineNotify(DLL_JIG_CONTROL);
	DefineNotify(DLL_SHOW_MSG_DLG);
	DefineNotify(DLL_LOG);

	StartTotalDurationCounter();
	StartTestItemDurationCounter();
}


// ----------------- Information functions -----------------

bool ITestProcessor::LoadToolInfo()
{
	bool bRes = false;


	if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		bRes = true;

	if (bRes)
	{
		XMLNodeList toolInfoNodeList = NULL;
		toolInfoNodeList = m_TestItemXML.GetChildNodes(_T("//Configuration//ToolInfo"));
		if (toolInfoNodeList != NULL)
			bRes = true;

		for (UINT i = 0; (bRes && (i < m_TestItemXML.GetNodeListSize(toolInfoNodeList))); ++i)
		{
			XMLNode toolInfoNode = m_TestItemXML.GetNodeListItem(toolInfoNodeList, i);
			if (toolInfoNode != NULL)
				bRes = true;
			else
				bRes = false;

			CString cstrNodeName = m_TestItemXML.GetNodeName(toolInfoNode);
			CString cstrNodeText = m_TestItemXML.GetNodeText(toolInfoNode);

			if (cstrNodeName.IsEmpty() != true)
			{
				CT2A pszNodeName(cstrNodeName);
				std::string strNodeName(pszNodeName);

				if (cstrNodeText.IsEmpty() != true)
				{
					CT2A pszNodeText(cstrNodeText);
					std::string strNodeText(pszNodeText);
					g_mapToolInfo[strNodeName] = strNodeText;
				}
				else
					g_mapToolInfo[strNodeName] = "-";
			}
		}
	}

	return bRes;
}

bool ITestProcessor::LoadTestItem()
{
	bool bRes = false;

	m_vTestItemList.clear();
	m_vTestItemTypeList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) == ERROR_SUCCESS)
		bRes = true;

	if (bRes)
	{
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

			CString cstrTestItemType = m_TestItemXML.GetNodeAttrValue(testItemNode, _T("Type"));
			if (cstrTestItemType.IsEmpty() != true)
			{
				CT2A pszTestItemType(cstrTestItemType);
				std::string strTestItemType(pszTestItemType);
				m_vTestItemTypeList.push_back(strTestItemType);
			}
			else
				m_vTestItemTypeList.push_back("NORMAL");
		}
	}

	return bRes;
}


// ------------------ Create phone object ------------------

bool ITestProcessor::CreatePhoneObject()
{
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// 1. Load phone parameters
	if (! LoadPhoneParams())
	{
		TraceLog(MSG_ERROR, "Fail to load phone parameters");
		FactoryLog(false, "CreatePhoneObject", "--", "--", "--", "--", "--", "Fail", "--", "Fail to load phone parameters");
		return false;
	}

	// 2. Create phone object
	CT2A szPhone(m_cstrPhone);
	std::string strPhone(szPhone);

	if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
	{
		TraceLog(MSG_ERROR, "Fail to create phone object");
		FactoryLog(false, "CreatePhoneObject", "FT1001", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object");
		return false;
	}

	if (! pcObject->InitData(m_mapPhoneParams))
	{
		TraceLog(MSG_ERROR, "Fail to execute InitDate() of phone object");
		FactoryLog(false, "CreatePhoneObject", "FT1001", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of phone object");
		return false;
	}

	m_pIPhone = dynamic_cast<IPhone*>(pcObject);
	return true;
}


// -------------------- Setup functions --------------------

bool ITestProcessor::CheckCalibrationMode()
{
	//bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	m_cstrKCableLossFile = m_cstrStationName + _T("_K_CableLoss.xml");
	m_cstrGSRefResultFile = m_cstrStationName + _T("_GS_RefResult.xml");

	CString cstrConfigXML_K = _T("");
	CString cstrConfigXML_GS = _T("");
	cstrConfigXML_K.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrKCableLossFile);
	cstrConfigXML_GS.Format(_T("%s\\Qisda\\%s"), szModulePath, m_cstrGSRefResultFile);

	if (::_taccess(cstrConfigXML_K, 0) == 0 && ::_taccess(cstrConfigXML_GS, 0) == 0)
	{
		m_cstrKCableLossFile = cstrConfigXML_K;
		m_cstrGSRefResultFile = cstrConfigXML_GS;

		if (m_KCableLossXML.Load(m_cstrKCableLossFile) == ERROR_SUCCESS && m_GSRefResultXML.Load(m_cstrGSRefResultFile) == ERROR_SUCCESS)
		{
			m_bCalibrationMode = true;	
		}
	}

	//bRes = true;
	return true;
}
bool ITestProcessor::LoadRefResult()
{
	if (m_CableLossXML.Load(m_cstrGSRefResultFile) != ERROR_SUCCESS)
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

	XMLNodeList rfPicassoNumList = m_CableLossXML.GetChildNodes(rfCable);
	if (rfPicassoNumList == NULL)
		return false;

	for (UINT i = 0; i < m_CableLossXML.GetNodeListSize(rfPicassoNumList); ++i)
	{
		XMLNode rfPicassoNum = m_CableLossXML.GetNodeListItem(rfPicassoNumList, i);
		if (rfPicassoNum == NULL)
			continue;

		CString cstrPicassoNumAttr = m_CableLossXML.GetNodeAttrValue(rfPicassoNum, _T("name"));
		//if (cstrRfLossesAttr.CompareNoCase(cstrCurrentRfLosses) != 0)
		if (cstrPicassoNumAttr.CompareNoCase(g_strPicasso.c_str()) != 0)
			continue;

		// Found matching cable loss set
		std::map<std::string, std::map<std::string, std::string>> mapTxCableLossData;
		std::map<std::string, std::map<std::string, std::string>> mapRxCableLossData;
		std::map<std::string, std::string> mapCableLoss;

		XMLNode rfType = m_CableLossXML.GetFirstChild(rfPicassoNum);
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
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));

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
				}
			}
			else if (cstrRfTypeAttr.CompareNoCase(_T("RX")) == 0)
			{
				// Start parsing RX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
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
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));

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
					}
					if (mapCableLoss.empty() != true)
					{
						CT2A szBand(cstrRfBandName);
						std::string strBand(szBand);
						mapRxCableLossData[strBand] = mapCableLoss;
					}
				}
			}

			rfType = rfType->GetnextSibling();
		}

		if (mapTxCableLossData.empty() != true)
			g_mapTxRefResultData = mapTxCableLossData;

		if (mapRxCableLossData.empty() != true)
			g_mapRxRefResultData = mapRxCableLossData;
	}

	return true;
}
bool ITestProcessor::LoadCableLoss()
{
	if (!m_bCalibrationMode)
	{
		if (m_CableLossXML.Load(m_cstrCableLossFile) != ERROR_SUCCESS)
			return false;
	}
	else
	{
		if (m_CableLossXML.Load(m_cstrKCableLossFile) != ERROR_SUCCESS)
			return false;
	}

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
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));

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
				}
			}
			else if (cstrRfTypeAttr.CompareNoCase(_T("RX")) == 0)
			{
				// Start parsing RX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
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
						XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
						if (rfChannel == NULL)
							continue;

						CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
						if (cstrRfChannelRange.IsEmpty())
							cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));

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
					}
					if (mapCableLoss.empty() != true)
					{
						CT2A szBand(cstrRfBandName);
						std::string strBand(szBand);
						mapRxCableLossData[strBand] = mapCableLoss;
					}
				}
			}

			rfType = rfType->GetnextSibling();
		}

		if (mapTxCableLossData.empty() != true)
			g_mapTxCableLossData = mapTxCableLossData;

		if (mapRxCableLossData.empty() != true)
			g_mapRxCableLossData = mapRxCableLossData;
	}

	return true;
}

bool ITestProcessor::LoadTestProcessorParams()
{
	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode UsingGPIB = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//UsingGPIB"));
	if (UsingGPIB != NULL)
	{
		CString cstrUsingGPIB = m_TestItemXML.GetNodeText(UsingGPIB);
		if (! cstrUsingGPIB.IsEmpty())
		{
			if (cstrUsingGPIB.CompareNoCase(_T("TRUE")) == 0)
				m_bUsingGPIB = true;
			else
				m_bUsingGPIB = false;
		}
	}

	XMLNode GPIBInterfaceID = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//GPIBInterfaceID"));
	if (GPIBInterfaceID != NULL)
	{
		CString cstrGPIBInterfaceID = m_TestItemXML.GetNodeText(GPIBInterfaceID);
		if (! cstrGPIBInterfaceID.IsEmpty())
			m_iGPIBInterfaceID = _ttoi(cstrGPIBInterfaceID);
	}

	XMLNode GPIBCardAddress = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//GPIBCardAddress"));
	if (GPIBCardAddress != NULL)
	{
		CString cstrGPIBCardAddress = m_TestItemXML.GetNodeText(GPIBCardAddress);
		if (! cstrGPIBCardAddress.IsEmpty())
			m_iGPIBCardAddress = _ttoi(cstrGPIBCardAddress);
	}

	XMLNode ForceToPass = m_TestItemXML.SearchNode(_T("//Configuration//TestProcessor//ForceToPass"));
	if (ForceToPass != NULL)
	{
		CString cstrForceToPass = m_TestItemXML.GetNodeText(ForceToPass);
		if (! cstrForceToPass.IsEmpty())
		{
			if (cstrForceToPass.CompareNoCase(_T("TRUE")) == 0)
				m_bForceToPass = true;
			else
				m_bForceToPass = false;
		}
	}

	return true;
}

bool ITestProcessor::LoadGPIBDeviceParams()
{
	std::vector<int> vPSAddr;
	std::map<std::string, std::string> mapTesterDevice, mapPowerSupply;

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	// Tester device parameters
	XMLNode testerDevice = m_TestItemXML.SearchNode(_T("//Configuration//TesterDevice"));
	if (testerDevice != NULL)
	{
		XMLNodeList testerDeviceList = m_TestItemXML.GetChildNodes(testerDevice);
		if (testerDeviceList != NULL)
		{
			for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(testerDeviceList); ++i)
			{
				XMLNode testerDeviceItem = m_TestItemXML.GetNodeListItem(testerDeviceList, i);
				if (testerDeviceItem != NULL)
				{
					CString cstrName = m_TestItemXML.GetNodeAttrValue(testerDeviceItem, _T("Name"));
					CString cstrClassName = m_TestItemXML.GetNodeAttrValue(testerDeviceItem, _T("ClassName"));
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

		XMLNode rfInputPort = m_TestItemXML.SearchNode(_T("//Configuration//TesterDevice//RFInputPort"));
		if (rfInputPort != NULL)
		{
			CString cstrRFInputPort = m_TestItemXML.GetNodeText(rfInputPort);
			if (! cstrRFInputPort.IsEmpty())
			{
				ITesterDevice::SetRFInputPort(_ttoi(cstrRFInputPort));
			}
		}

		XMLNode rfOutputPort = m_TestItemXML.SearchNode(_T("//Configuration//TesterDevice//RFOutputPort"));
		if (rfOutputPort != NULL)
		{
			CString cstrRFOutputPort = m_TestItemXML.GetNodeText(rfOutputPort);
			if (! cstrRFOutputPort.IsEmpty())
			{
				ITesterDevice::SetRFOutputPort(_ttoi(cstrRFOutputPort));
			}
		}
	}

	// Power supply parameters
	XMLNode powerSupplyCount = m_TestItemXML.SearchNode(_T("//Configuration//PowerSupply//PowerSupplyCount"));
	if (powerSupplyCount != NULL)
	{
		CString cstrPowerSupplyCount = m_TestItemXML.GetNodeText(powerSupplyCount);
		m_iPSCount = _ttoi(cstrPowerSupplyCount);
	}

	XMLNode powerSupplyList = m_TestItemXML.SearchNode(_T("//Configuration//PowerSupply//PowerSupplyList"));
	if (powerSupplyList != NULL)
	{
		XMLNodeList powerSupplyOrderList = m_TestItemXML.GetChildNodes(powerSupplyList);
		if (powerSupplyOrderList != NULL)
		{
			for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(powerSupplyOrderList); ++i)
			{
				XMLNode powerSupplyOrder = m_TestItemXML.GetNodeListItem(powerSupplyOrderList, i);
				if (powerSupplyOrder != NULL)
				{
					CString cstrPSAddr = m_TestItemXML.GetNodeText(powerSupplyOrder);
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

	XMLNode powerSupply = m_TestItemXML.SearchNode(_T("//Configuration//PowerSupply"));
	if (powerSupply != NULL)
	{
		XMLNodeList powerSupplyChildList = m_TestItemXML.GetChildNodes(powerSupply);
		if (powerSupplyChildList != NULL)
		{
			for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(powerSupplyChildList); ++i)
			{
				XMLNode powerSupplyItem = m_TestItemXML.GetNodeListItem(powerSupplyChildList, i);
				if (powerSupplyItem != NULL)
				{
					if (m_TestItemXML.GetNodeName(powerSupplyItem).CompareNoCase(_T("Item")) != 0)
						continue;

					CString cstrName = m_TestItemXML.GetNodeAttrValue(powerSupplyItem, _T("Name"));
					CString cstrClassName = m_TestItemXML.GetNodeAttrValue(powerSupplyItem, _T("ClassName"));
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

	m_vPowerSupplyAddr = vPSAddr;
	m_mapTesterDeviceParams = mapTesterDevice;
	m_mapPowerSupplyParams = mapPowerSupply;

	return true;
}

bool ITestProcessor::DetectGPIBDevice()
{
	if (! m_bUsingGPIB)
		return true;

	return IGPIBDevice::DetectGPIBDevice(m_iGPIBInterfaceID, m_iGPIBCardAddress);
}

bool ITestProcessor::LoadPhoneParams()
{
	std::map <std::string, std::string> mapPhoneParams;

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode phone = m_TestItemXML.SearchNode(_T("//Configuration//Phone"));
	if (phone == NULL)
		return false;

	XMLNodeList phoneList = m_TestItemXML.GetChildNodes(phone);
	if (phoneList == NULL)
		return false;

	for (UINT i = 0; i < m_TestItemXML.GetNodeListSize(phoneList); ++i)
	{
		XMLNode phoneParams = m_TestItemXML.GetNodeListItem(phoneList, i);
		if (NULL == phoneParams)
			continue;

		CString nodename = m_TestItemXML.GetNodeName(phoneParams);
		if (nodename.CompareNoCase (_T("ClassName")) == 0)
		{
			m_cstrPhone = m_TestItemXML.GetNodeText(phoneParams);
		}

		CString nodevalue = m_TestItemXML.GetNodeText(phoneParams);
		mapPhoneParams[nodename.GetString()] = nodevalue.GetString();
	}

	XMLNode m_LogUpload = m_TestItemXML.SearchNode(_T("//Configuration//Phone//LogUpload"));
	if (m_LogUpload != NULL)
	{
		CString str_LogUpload = m_TestItemXML.GetNodeText(m_LogUpload);
		m_b_LogUpload =  str_LogUpload.Compare("TRUE") == 0 ? true : false;
	}

	XMLNode m_UploadServerIP = m_TestItemXML.SearchNode(_T("//Configuration//Phone//UploadServerIP"));
	if (m_UploadServerIP != NULL)
	{
		CString str_UploadServerIP = m_TestItemXML.GetNodeText(m_UploadServerIP);
		m_str_UploadServerIP = str_UploadServerIP.GetString();
	}

	m_mapPhoneParams = mapPhoneParams;
	return true;
}

bool ITestProcessor::LoadTestItemParameters()
{
	std::map<std::string, std::string> mapObjectParams;

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet"));
	if (processObjectSet == NULL)
		return false;

	
	XMLNodeList processObjectsCategoryList = processObjectSet->GetchildNodes();
	if (processObjectsCategoryList == NULL)
		return false;

	
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
			}

			// Insert all parameters in m_mapTestItemParams
			if (mapObjectParams.empty() != true)
				m_mapTestItemParams[strName] = mapObjectParams;
		}
	}

	return true;
}

// ---------------- Get Parameter Functions ----------------

bool ITestProcessor::GetFactoryVersion(char* sz_value)
{
	strcpy(sz_value, g_mapToolInfo["FACTORY_VERSION"].c_str());
	return true;
}

bool ITestProcessor::GetSWVersion(char* sz_value)
{
	strcpy(sz_value, g_mapToolInfo["SW_VERSION"].c_str());
	return true;
}

// ---------------- Set/Get Parameter Value ----------------

bool ITestProcessor::SetParameterValue(char* sz_keyword, char* sz_value)
{
	g_mapToolInfo[std::string(sz_keyword)] = std::string(sz_value);
	if (_stricmp (sz_keyword, "PICASSO") == 0)
		g_strPicasso = sz_value;

	return true;
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


bool ITestProcessor::SetModelName(const char* p_sz_modelName)
{
	if (p_sz_modelName != NULL)
	{
		g_str_modelName = p_sz_modelName;
		return true;
	}

	return false;
}

bool ITestProcessor::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	if (g_mapToolInfo.find(std::string(sz_keyword)) == g_mapToolInfo.end())
	{
		TraceLog(MSG_ERROR, "Error in find the keywords!");
		return false;
	}

	sprintf_s(sz_value, (i_size - 1), "%s", g_mapToolInfo[std::string(sz_keyword)].c_str());
	return true;
}


// -------------------- Set/Get FA Data --------------------





// --------------------- Get test items --------------------

int ITestProcessor::GetItemCount()
{
	return (signed)m_vTestItemList.size();
}

bool ITestProcessor::GetItemByIndex(int i_index, char* sz_name, int i_size)
{
	if (i_index >= (signed)m_vTestItemList.size())
	{
		FactoryLog(false, "GetItemByIndex", "--", "--", "--", "--", "--", "Fail", "--", "Fail to find test item, index number invalid");
		return false;
	}

	sprintf_s(sz_name, (i_size - 1), "%s", m_vTestItemList[i_index].c_str());
	return true;
}


// ---------------- Response to Jig control ----------------

bool ITestProcessor::ResopnseToJig(bool b_wait)
{
	return true;
}


// ---------------- Response to show dialog ----------------

bool ITestProcessor::ResopnseToShowDlg(char* sz_showReslut)
{
	return true;
}

// ---------------- Execute Android Logcat ----------------
bool ITestProcessor::ExecAndroidLogcat(bool b_IsPass)
{
	char str_LogPath[1000] = _T("D:\\Log");  
	CString str_Date;
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	_mkdir(str_LogPath);
	strcat(str_LogPath, _T("\\Android"));
	_mkdir(str_LogPath);
	str_Date.Format(_T("\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(str_LogPath, str_Date);
	_mkdir(str_LogPath);

	CString str_command = _T("");
	CString str_msg = _T("");
	CString str_Picasso = _T("RD");
	if(! g_strPicasso.empty() )
		str_Picasso = g_strPicasso.c_str();
	if(b_IsPass)
	{
		str_command.Format(_T("/C adb logcat -v time -d > %s\\PASS_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), str_LogPath, str_Picasso,
																			  systemTime.wYear, systemTime.wMonth, systemTime.wDay,
																			  systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	}
	else
	{
		str_command.Format(_T("/C adb logcat -v time -d > %s\\FAIL_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), str_LogPath, str_Picasso,
																			  systemTime.wYear, systemTime.wMonth, systemTime.wDay,
																			  systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	}
	//ShellExecute(0, "open", "cmd", str_command, 0, SW_HIDE);
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";	
	ShExecInfo.lpParameters = str_command;	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000); 

	str_msg.Format(_T("Execute: %s"), str_command);
	TraceLog(MSG_INFO, str_msg.GetBuffer());
	return true;
}

// ------------- Log and UI control functions --------------

void ITestProcessor::TraceLog(int iSeverity, std::string strMsg)
{
	sTraceLog firedData;

	firedData.iSeverity = iSeverity;
	firedData.strMessage = strMsg;

	Fire(DLL_LOG, (long)&firedData);
}

void ITestProcessor::FactoryLog(bool bPass, std::string strMessage)
{
	sMessage firedData;
	SYSTEMTIME currTime;
	char szCurrTime[256];

	::GetLocalTime(&currTime);
	memset(szCurrTime, 0, 256);

	if (bPass)
	{
		sprintf_s(szCurrTime, 255, "%02d:%02d:%02d.%03d, PASS, ",
			currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);
	}
	else
	{
		sprintf_s(szCurrTime, 255, "%02d:%02d:%02d.%03d, FAIL, ",
			currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);
	}

	firedData.strMessage = szCurrTime + strMessage;

	Fire(DLL_MESSAGE, (long)&firedData);
}

void ITestProcessor::FactoryLog(bool bPass,
								  std::string strItemCode,
								  std::string strErrCode,
								  std::string strBand,
								  std::string strChannel,
								  std::string strLower,
								  std::string strUpper,
								  std::string strMeasured,
								  std::string strUnit,
								  std::string strMsg
								  )
{
	sTestResult firedData;
	SYSTEMTIME currTime;
	char szCurrTime[256];

	::GetLocalTime(&currTime);
	memset(szCurrTime, 0, 256);
	sprintf_s(szCurrTime, 255, "%02d:%02d:%02d.%03d",
		currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);

	firedData.strTime = szCurrTime;

	firedData.strItemCode = strItemCode;
	firedData.strErrCode = strErrCode;
	firedData.strBand = strBand;
	firedData.strChannel = strChannel;
	firedData.strLower = strLower;
	firedData.strUpper = strUpper;
	firedData.strMeasured = strMeasured;
	firedData.strUnit = strUnit;

	firedData.strElapse = GetTestDuration();

	firedData.strMsg = strMsg;

	if (bPass)
	{
		firedData.strResult = "PASS";
		firedData.strErrCode = "";
	}
	else
	{
		firedData.strResult = "FAIL";

		g_strErrCode = strErrCode;
		g_strErrMsg = strMsg;
	}

	Fire(DLL_TEST_RESULT, (long)&firedData);
}

void ITestProcessor::CPKLog(int iSeverity, 
							std::string strTech,
							std::string strItemName,
							std::string strBand,
							std::string strChannel,
							std::string strIdentify1,
							std::string strIdentify2,
							std::string strIdentify3,
							std::string strValue,
							bool result)
{
	char szTmp[512] ;
	::ZeroMemory(szTmp, 512) ;
	sprintf_s(szTmp, MAX_PATH, ",Detroit_CPK_MeasurementData,%s,%s,%s,%s,%s,%s,%s,%s,%d",
		strTech.c_str(), strItemName.c_str(), strBand.c_str(), strChannel.c_str(),
		strIdentify1.c_str(), strIdentify2.c_str(), strIdentify3.c_str(), strValue.c_str(),
		(result)?1:0 );

	sTraceLog firedData;

	firedData.iSeverity = iSeverity;
	firedData.strMessage = szTmp;

	Fire(DLL_LOG, (long)&firedData);
}


void ITestProcessor::SetUIPattern(std::string strPattern)
{
	sPattern firedData;

	firedData.strPattern = strPattern;

	Fire(DLL_PATTERN, (long)&firedData);
}

void ITestProcessor::SetTestingItem(std::string strTestItemName)
{
	sTestingItem firedData;

	firedData.strTestingItem = strTestItemName;

	Fire(DLL_TESTING_ITEM, (long)&firedData);
}

void ITestProcessor::SetPICSData(std::string strKey, std::string strValue)
{
	sPICSData firedData;

	firedData.strKey = strKey;
	firedData.strValue = strValue;

	Fire(DLL_PICSDATA, (long)&firedData);
}

void ITestProcessor::JigControl(std::string strSwitch)
{
	sJigControl firedData;

	firedData.strSwitch = strSwitch;

	Fire(DLL_JIG_CONTROL, (long)&firedData);
}

void ITestProcessor::ShowMsgDlg(std::string strMsg)
{
	sShowMsgDlg firedData;

	firedData.strMessage = strMsg;

	Fire(DLL_SHOW_MSG_DLG, (long)&firedData);
}


// ---------------- Test duration functions ----------------

void ITestProcessor::StartTotalDurationCounter()
{
	g_totalTestDuration_Start = clock();
}

void ITestProcessor::StartTestItemDurationCounter()
{
	g_testItemDuration_Start = clock();
}

std::string ITestProcessor::GetTestDuration()
{
	clock_t currentClock = clock();
	clock_t durationClock = currentClock - g_testItemDuration_Start;

	double dDuration = durationClock / (double)CLOCKS_PER_SEC;

	char szDuration[256];
	memset(szDuration, 0, 256);
	sprintf_s(szDuration, 255, "%.3lf", dDuration);

	return std::string(szDuration);
}

std::string ITestProcessor::GetTotalTestDuration()
{
	clock_t currentClock = clock();
	clock_t durationClock = currentClock - g_totalTestDuration_Start;

	double dDuration = durationClock / (double)CLOCKS_PER_SEC;

	char szDuration[256];
	memset(szDuration, 0, 256);
	sprintf_s(szDuration, 255, "%.3lf", dDuration);

	return std::string(szDuration);
}


// ----------------- Test result function ------------------

void ITestProcessor::SetTestResult(bool bPass)
{
	std::string strTotalDuration = GetTotalTestDuration();

	if (bPass)
	{
		std::string strMsg = "Test finished, total test duration : ";
		strMsg += strTotalDuration;

		FactoryLog(true, "--", "--", "--", "--", "--", "--", "Pass", "--", strMsg);
	}
	else
	{
		std::string strMsg = g_strErrMsg + " : Total test duration : ";
		strMsg += strTotalDuration;

		FactoryLog(false, "--", g_strErrCode, "--", "--", "--", "--","Fail", "", strMsg);
	}
}


// --------------------- Notification ----------------------

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

double ITestProcessor::GetRefResult(const std::string strRfType, const std::string strRfBand, std::string strRfRange)
{
	std::map<std::string, std::string> mapCableLoss;
	double dLoss = 0.0;

	if (strRfType == "TX")
	{
		if (g_mapTxRefResultData.find(strRfBand) != g_mapTxRefResultData.end())
		{
			mapCableLoss = g_mapTxRefResultData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}
	else if (strRfType == "RX")
	{
		if (g_mapRxRefResultData.find(strRfBand) != g_mapRxRefResultData.end())
		{
			mapCableLoss = g_mapRxRefResultData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}

	return dLoss;
}

double ITestProcessor::GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange)
{
	std::map<std::string, std::string> mapCableLoss;
	double dLoss = 0.0;

	if (strRfType == "TX")
	{
		if (g_mapTxCableLossData.find(strRfBand) != g_mapTxCableLossData.end())
		{
			mapCableLoss = g_mapTxCableLossData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}
	else if (strRfType == "RX")
	{
		if (g_mapRxCableLossData.find(strRfBand) != g_mapRxCableLossData.end())
		{
			mapCableLoss = g_mapRxCableLossData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}

	return dLoss;
}

bool ITestProcessor::UpdateCableLoss(std::string strRfType, std::string strRfBand, std::string strRfRange, std::string strNewLoss)
{
	if (m_CableLossXML.Load(m_cstrCableLossPath) != ERROR_SUCCESS)
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

		XMLNode rfType = m_CableLossXML.GetFirstChild(rfLosses);
		while (rfType)
		{
			CString cstrRfTypeAttr = m_CableLossXML.GetNodeAttrValue(rfType, _T("name"));
			if (cstrRfTypeAttr.CompareNoCase(_T(strRfType.c_str())) == 0)
			{
				// Start parsing TX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
					if (rfBand == NULL)
						continue; 

					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
					if (rfChannelList == NULL)
						continue;

					if (cstrRfBandName.CompareNoCase(_T(strRfBand.c_str())) == 0)
					{
						for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
						{
							XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
							if (rfChannel == NULL)
								continue;

							CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
							if (cstrRfChannelRange.IsEmpty())
								cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("name"));

							XMLNodeList rfLossList = m_CableLossXML.GetChildNodes(rfChannel);
							if (rfLossList == NULL)
								continue;


							for (UINT l = 0; l < m_CableLossXML.GetNodeListSize(rfLossList); ++l)
							{
								XMLNode rfLoss = m_CableLossXML.GetNodeListItem(rfLossList, l);
								if (rfLoss == NULL)
									continue;

								if (cstrRfChannelRange.CompareNoCase(_T(strRfRange.c_str())) == 0)
								{
									m_CableLossXML.SetNodeText(rfLoss,_T(strNewLoss.c_str()));
									m_CableLossXML.Save();
									return true;
								}

							}
						}

					}

				}
			}
			else if (cstrRfTypeAttr.CompareNoCase(_T(strRfType.c_str())) == 0)
			{
				// Start parsing RX cable losses
				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
				if (rfBandList == NULL)
					continue;

				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
				{
					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
					if (rfBand == NULL)
						continue; 

					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
					if (rfChannelList == NULL)
						continue;

					if (cstrRfBandName.CompareNoCase(_T(strRfBand.c_str())) == 0)
					{
						for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
						{
							XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
							if (rfChannel == NULL)
								continue;

							CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));


							XMLNodeList rfLossList = m_CableLossXML.GetChildNodes(rfChannel);
							if (rfLossList == NULL)
								continue;


							for (UINT l = 0; l < m_CableLossXML.GetNodeListSize(rfLossList); ++l)
							{
								XMLNode rfLoss = m_CableLossXML.GetNodeListItem(rfLossList, l);
								if (rfLoss == NULL)
									continue;

								if (cstrRfChannelRange.CompareNoCase(_T(strRfRange.c_str())) == 0)
								{
									m_CableLossXML.SetNodeText(rfLoss,_T(strNewLoss.c_str()));
									m_CableLossXML.Save();
									return true;
								}

							}
						}

					}

				}
			}

			rfType = rfType->GetnextSibling();
		}
	}

	return false;
}

bool ITestProcessor::InRange(int iLowerBound, int iUpperBound, int iMeasured)
{
	if (iMeasured <= iUpperBound && iMeasured >= iLowerBound)
		return true;

	return false;
}

bool ITestProcessor::InRange(double dLowerBound, double dUpperBound, double dMeasured)
{
	if (dMeasured <= dUpperBound && dMeasured >= dLowerBound)
		return true;

	return false;
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

/* upload log to server */
bool ITestProcessor::LogUpload(char *str_modelname)
{
	if(!m_b_LogUpload)
		return true;

	//設定Shell Execute參數
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
	char str_LocalPath[1000] = _T("D:\\Log");  
	char str_BackupPath[1000] = _T("D:\\LogUpload"); 
	char temp[2000];

	//-- 建立遠端路徑
	strcpy(str_RemotePath, _T("\\\\"));
	strcat(str_RemotePath, m_str_UploadServerIP.c_str());
	strcat(str_RemotePath, _T("\\TestLog"));

	char str_account[100] = _T("mpics\\TestLog");
	char str_pswd[100] = _T("qisda2013");
	sprintf(temp, "/C net use %s /user:%s %s", str_RemotePath, str_account, str_pswd );
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	//-- 測試建立連線是否成功
	sprintf(temp, "/C net use %s", str_RemotePath);
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	if(WaitForSingleObject(ShExecInfo.hProcess, 2000) == WAIT_TIMEOUT)
	{
		TraceLog(MSG_INFO, "Fail to connect server uploading.");
		return false;
	}

	_mkdir(str_RemotePath);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_modelname);
	_mkdir(str_RemotePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_ComputerName);
	_mkdir(str_RemotePath);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char systime[100];
	sprintf(systime, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay );
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, systime);

	_mkdir(str_RemotePath);
	
	//-- 複製所有檔案至遠端
	sprintf(temp, "/C xcopy /e /y %s %s", str_LocalPath, str_RemotePath);
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 20000); 

	//-- 確認資料完整傳送至遠端
	bool b_ok = DFSCheckFile(str_LocalPath, str_RemotePath, _T(""));

	//-- 複製log至本地指定目錄並刪除
	if(b_ok)
	{
		_mkdir(str_BackupPath);
		sprintf(temp, "/C xcopy /e /y /q %s %s", str_LocalPath, str_BackupPath);
		ShExecInfo.lpParameters = temp;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE); 
		
		DFSDeleteFile(str_LocalPath, _T(""));
		//sprintf(temp, "/C rmdir /s /q %s", str_LocalPath);
		//ShExecInfo.lpParameters = temp;
		//ShellExecuteEx(&ShExecInfo);
		//WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		TraceLog(MSG_INFO, "Check and Upload Pass.");
		return true;
	}
	else
	{
		TraceLog(MSG_INFO, "Copy and Check upload file Fail.");
		return false;
	}

}

/* Check upload file */
bool ITestProcessor::DFSCheckFile(char *str_LocalPath, char *str_RemotePath, char *str_TracePath)
{
	char szDir[1000];
	char str_tempPath[1000];
    HANDLE hList;
    WIN32_FIND_DATA FileData;

	sprintf(szDir, _T("%s\\%s\\*"), str_LocalPath, str_TracePath);
	if ( (hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE )	//此路徑沒下層路徑,有可能為檔案
	{	
		sprintf(szDir, _T("%s\\%s"), str_RemotePath, str_TracePath);			//確認遠端路徑下也有此檔案
		FindClose(hList);
		if(_taccess(szDir, 0) == -1){											//找不到遠端路徑下檔案
			TraceLog(MSG_INFO, str_TracePath);
			return false;
		}
		else{																	//找到遠端路徑下檔案
			return true;
		}
	}
	while (1)
	{
        if (!FindNextFile(hList, &FileData)) {
			if (GetLastError() == ERROR_NO_MORE_FILES){
				FindClose(hList);
                return true;
			}
        }
		if(strcmp(FileData.cFileName, _T("..")) != 0){
			strcpy(str_tempPath, str_TracePath);
			if(strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, _T("\\"));
			strcat(str_tempPath, FileData.cFileName);
			if(!DFSCheckFile(str_LocalPath, str_RemotePath, str_tempPath)){
				FindClose(hList);
				return false;
			}
		}
    }
}

/* Delete local file */
bool ITestProcessor::DFSDeleteFile(char *str_LocalPath, char *str_TracePath)
{
	char szDir[1000];
	char str_tempPath[1000];
    HANDLE hList;
    WIN32_FIND_DATA FileData;

	sprintf(szDir, _T("%s\\%s\\*"), str_LocalPath, str_TracePath);
	if ( (hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE )	//此路徑沒下層路徑,有可能為檔案
	{	
		bool b_DelFile = true;
		//.csv檔不刪
		if(strstr(str_TracePath, ".CSV") != NULL)	b_DelFile = false;

		//DB_Log今日檔不刪
		SYSTEMTIME systemTime;	
		GetLocalTime(&systemTime);
		char TodayKeepLog[1000];
		sprintf(TodayKeepLog, _T("DB_Log\\%04d_%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay );
		if(strstr(szDir, TodayKeepLog) != NULL)		b_DelFile = false;
		
		//VMS_Log今日檔不刪
		sprintf(TodayKeepLog, _T("VMS_Log\\%04d_%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay );
		if(strstr(szDir, TodayKeepLog) != NULL)		b_DelFile = false;

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

		if(b_DelFile)
		{
			sprintf(szDir, _T("%s\\%s"), str_LocalPath, str_TracePath);
			remove(szDir);
		}
		//TraceLog(MSG_INFO, szDir);
		FindClose(hList);
		return true;
	}
	while (1)
	{
        if (!FindNextFile(hList, &FileData)) {
			if (GetLastError() == ERROR_NO_MORE_FILES){
				FindClose(hList);
                return true;
			}
        }
		if(strcmp(FileData.cFileName, _T("..")) != 0){
			strcpy(str_tempPath, str_TracePath);
			if(strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, _T("\\"));
			strcat(str_tempPath, FileData.cFileName);
			if(!DFSDeleteFile(str_LocalPath, str_tempPath)){
				FindClose(hList);
				return false;
			}
		}
    }
}