#include "stdafx.h"
#include "CableLossHandler_Standard.h"

using namespace ECIErrorCode::CablelossHdr;

CCablelossHdrStd::CCablelossHdrStd(CEnhanceController* pNextCEC) : CEnhanceController(pNextCEC)
{
}

CCablelossHdrStd::~CCablelossHdrStd()
{
}

size_t CCablelossHdrStd::Enhance_OnAPPLaunch()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPLaunch();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CCablelossHdrStd::Enhance_OnRunInit(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR != ret)
		return ret;

	ret = this->CheckCablelossXMLExist();
	if(NOERROR != ret)
		return ret;

	ret = this->LoadCableloss();
	if(NOERROR != ret)
		return ret;

	ret = this->SetToolCableloss();
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CCablelossHdrStd::Enhance_RunTest(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CCablelossHdrStd::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR != ret)
		return ret;

	return ret;
}

size_t CCablelossHdrStd::Enhance_OnAPPEnd()
{
	size_t ret = NOERROR;

	ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR != ret)
		return ret;
	
	g_mapTxCableLossData.clear();
	g_mapRxCableLossData.clear();

	return ret;
}

size_t CCablelossHdrStd::CheckCablelossXMLExist()
{
	CString cstrCablelossPath;
	cstrCablelossPath.Empty();
	if(this->m_Parametermap[ParameterKeyWord::STATIONNAME].Find(_T("_CableLoss")) != -1)
	{
		cstrCablelossPath =	this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
													_T("Qisda\\") + 
													this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
													_T("_") + 
													this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
													_T(".xml");
	}
	else
	{
		cstrCablelossPath =	this->m_Parametermap[ParameterKeyWord::CONFIGDIR] + 
													this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
													_T("_") + 
													this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
													_T("_CableLoss.xml");
	}
	
	size_t ret = NOERROR;

	if (::_taccess(cstrCablelossPath, 0) == 0)
	{
		if (m_CableLossXML.Load(cstrCablelossPath) != ERROR_SUCCESS)
		{
			ret = LOAD_CABLELOSS_SETTING_FAIL;
		}
	}
	else
	{
		ret = LOAD_CABLELOSS_SETTING_FAIL;
	}

	return ret;
}

size_t CCablelossHdrStd::LoadCableloss()
{
	g_mapTxCableLossData.clear();
	g_mapRxCableLossData.clear();

	size_t ret = NOERROR;

	XMLNode currentRfLosses = m_CableLossXML.SearchNode(_T("//FrameworkConfig//RfSettings//RfCable//CurrentRfLosses"));
	if (currentRfLosses == NULL)
	{
		return LOAD_CABLELOSS_SETTING_FAIL;
	}

	CString cstrCurrentRfLosses = m_CableLossXML.GetNodeText(currentRfLosses);
	if (cstrCurrentRfLosses.IsEmpty())
	{
		return LOAD_CABLELOSS_SETTING_FAIL;
	}

	XMLNodeList rfLossesList = m_CableLossXML.GetChildNodes(_T("//FrameworkConfig//RfSettings//RfCable"));
	if (rfLossesList == NULL)
	{
		return LOAD_CABLELOSS_SETTING_FAIL;
	}

	for (UINT i = 0; i < m_CableLossXML.GetNodeListSize(rfLossesList); ++i)
	{
		XMLNode rfLosses = m_CableLossXML.GetNodeListItem(rfLossesList, i);
		if (rfLosses == NULL)
			continue;

		CString cstrRfLossesAttr = m_CableLossXML.GetNodeAttrValue(rfLosses, _T("name"));
		if (cstrRfLossesAttr.CompareNoCase(cstrCurrentRfLosses) != 0)
			continue;

		// Found matching cable loss set
		std::map<CString, std::map<CString, CString>> mapTxCableLossData;
		std::map<CString, std::map<CString, CString>> mapRxCableLossData;
		std::map<CString, CString> mapCableLoss;

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
								mapCableLoss[cstrRfChannelRange] = cstrRfLoss;
							}
						}
					}

					if (mapCableLoss.empty() != true)
					{
						mapTxCableLossData[cstrRfBandName] = mapCableLoss;
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
						mapCableLoss[cstrRfChannelRange] = cstrRfLoss;
					}
					if (mapCableLoss.empty() != true)
					{
						mapRxCableLossData[cstrRfBandName] = mapCableLoss;
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


	return ret;
}

size_t CCablelossHdrStd::SetToolCableloss()
{
	size_t ret = NOERROR;

	//Set TX
	for(	std::map<CString, std::map<CString, CString>>::iterator iter = this->g_mapTxCableLossData.begin();
			iter != this->g_mapTxCableLossData.end();
			iter++)
	{
		for(	std::map<CString, CString>::iterator iter2 = iter->second.begin();
				iter2 != iter->second.end();
				iter2++)
		{
			if(!this->m_pITI->SetCableloss(_T("TX"), iter->first.GetString(), iter2->first.GetString(), iter2->second.GetString()))
			{
				ret = TOOL_SET_CABLELOSS_FAIL;
				break;
			}
		}
	}

	//Set RX
	for(	std::map<CString, std::map<CString, CString>>::iterator iter = this->g_mapRxCableLossData.begin();
			iter != this->g_mapRxCableLossData.end();
			iter++)
	{
		for(	std::map<CString, CString>::iterator iter2 = iter->second.begin();
				iter2 != iter->second.end();
				iter2++)
		{
			if(!this->m_pITI->SetCableloss(_T("RX"), iter->first.GetString(), iter2->first.GetString(), iter2->second.GetString()))
			{
				ret = TOOL_SET_CABLELOSS_FAIL;
				break;
			}
		}
	}

	return ret;
}