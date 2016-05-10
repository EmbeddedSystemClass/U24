#include "stdafx.h"
#include "ITestItem.h"


RF_IMPLEMENT_DYNAMIC(ITestItem)


//double ITestItem::GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange)
//{
//	std::map<std::string, std::string> mapCableLoss;
//	double dLoss = 0.0;
//
//	if (strRfType == "TX")
//	{
//		if (g_mapTxCableLossData.find(strRfBand) != g_mapTxCableLossData.end())
//		{
//			mapCableLoss = g_mapTxCableLossData[strRfBand];
//			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
//			{
//				std::string strCableLoss = mapCableLoss[strRfRange];
//				dLoss = atof(strCableLoss.c_str());
//			}
//		}
//	}
//	else if (strRfType == "RX")
//	{
//		if (g_mapRxCableLossData.find(strRfBand) != g_mapRxCableLossData.end())
//		{
//			mapCableLoss = g_mapRxCableLossData[strRfBand];
//			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
//			{
//				std::string strCableLoss = mapCableLoss[strRfRange];
//				dLoss = atof(strCableLoss.c_str());
//			}
//		}
//	}
//
//	return dLoss;
//}
//
//bool ITestItem::UpdateCableLoss(std::string strRfType, std::string strRfBand, std::string strRfRange, std::string strNewLoss)
//{
//	if (m_CableLossXML.Load(m_cstrCableLossPath) != ERROR_SUCCESS)
//		return false;
//
//	XMLNode currentRfLosses = m_CableLossXML.SearchNode(_T("//FrameworkConfig//RfSettings//RfCable//CurrentRfLosses"));
//	if (currentRfLosses == NULL)
//		return false;
//
//	CString cstrCurrentRfLosses = m_CableLossXML.GetNodeText(currentRfLosses);
//	if (cstrCurrentRfLosses.IsEmpty())
//		return false;
//
//	XMLNode rfCable = m_CableLossXML.SearchNode(_T("//FrameworkConfig//RfSettings//RfCable"));
//	if (rfCable == NULL)
//		return false;
//
//	XMLNodeList rfLossesList = m_CableLossXML.GetChildNodes(rfCable);
//	if (rfLossesList == NULL)
//		return false;
//
//	for (UINT i = 0; i < m_CableLossXML.GetNodeListSize(rfLossesList); ++i)
//	{
//		XMLNode rfLosses = m_CableLossXML.GetNodeListItem(rfLossesList, i);
//		if (rfLosses == NULL)
//			continue;
//
//		CString cstrRfLossesAttr = m_CableLossXML.GetNodeAttrValue(rfLosses, _T("name"));
//		if (cstrRfLossesAttr.CompareNoCase(cstrCurrentRfLosses) != 0)
//			continue;
//
//		XMLNode rfType = m_CableLossXML.GetFirstChild(rfLosses);
//		while (rfType)
//		{
//			CString cstrRfTypeAttr = m_CableLossXML.GetNodeAttrValue(rfType, _T("name"));
//			if (cstrRfTypeAttr.CompareNoCase(_T(strRfType.c_str())) == 0)
//			{
//				// Start parsing TX cable losses
//				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
//				if (rfBandList == NULL)
//					continue;
//
//				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
//				{
//					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
//					if (rfBand == NULL)
//						continue; 
//
//					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
//					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
//					if (rfChannelList == NULL)
//						continue;
//
//					if (cstrRfBandName.CompareNoCase(_T(strRfBand.c_str())) == 0)
//					{
//						for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
//						{
//							XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
//							if (rfChannel == NULL)
//								continue;
//
//							CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
//
//
//							XMLNodeList rfLossList = m_CableLossXML.GetChildNodes(rfChannel);
//							if (rfLossList == NULL)
//								continue;
//
//
//							for (UINT l = 0; l < m_CableLossXML.GetNodeListSize(rfLossList); ++l)
//							{
//								XMLNode rfLoss = m_CableLossXML.GetNodeListItem(rfLossList, l);
//								if (rfLoss == NULL)
//									continue;
//
//								if (cstrRfChannelRange.CompareNoCase(_T(strRfRange.c_str())) == 0)
//								{
//									m_CableLossXML.SetNodeText(rfLoss,_T(strNewLoss.c_str()));
//									m_CableLossXML.Save();
//									return true;
//								}
//
//							}
//						}
//
//					}
//
//				}
//			}
//			else if (cstrRfTypeAttr.CompareNoCase(_T(strRfType.c_str())) == 0)
//			{
//				// Start parsing RX cable losses
//				XMLNodeList rfBandList = m_CableLossXML.GetChildNodes(rfType);
//				if (rfBandList == NULL)
//					continue;
//
//				for (UINT j = 0; j < m_CableLossXML.GetNodeListSize(rfBandList); ++j)
//				{
//					XMLNode rfBand = m_CableLossXML.GetNodeListItem(rfBandList, j);
//					if (rfBand == NULL)
//						continue; 
//
//					CString cstrRfBandName = m_CableLossXML.GetNodeAttrValue(rfBand, _T("name"));
//					XMLNodeList rfChannelList = m_CableLossXML.GetChildNodes(rfBand);
//					if (rfChannelList == NULL)
//						continue;
//
//					if (cstrRfBandName.CompareNoCase(_T(strRfBand.c_str())) == 0)
//					{
//						for (UINT k = 0; k < m_CableLossXML.GetNodeListSize(rfChannelList); ++k)
//						{
//							XMLNode rfChannel = m_CableLossXML.GetNodeListItem(rfChannelList, k);
//							if (rfChannel == NULL)
//								continue;
//
//							CString cstrRfChannelRange = m_CableLossXML.GetNodeAttrValue(rfChannel, _T("range"));
//
//
//							XMLNodeList rfLossList = m_CableLossXML.GetChildNodes(rfChannel);
//							if (rfLossList == NULL)
//								continue;
//
//
//							for (UINT l = 0; l < m_CableLossXML.GetNodeListSize(rfLossList); ++l)
//							{
//								XMLNode rfLoss = m_CableLossXML.GetNodeListItem(rfLossList, l);
//								if (rfLoss == NULL)
//									continue;
//
//								if (cstrRfChannelRange.CompareNoCase(_T(strRfRange.c_str())) == 0)
//								{
//									m_CableLossXML.SetNodeText(rfLoss,_T(strNewLoss.c_str()));
//									m_CableLossXML.Save();
//									return true;
//								}
//
//							}
//						}
//
//					}
//
//				}
//			}
//
//			rfType = rfType->GetnextSibling();
//		}
//	}
//
//	return false;
//}

bool ITestItem::InRange(int iLowerBound, int iUpperBound, int iMeasured)
{
	if (iMeasured <= iUpperBound && iMeasured >= iLowerBound)
		return true;

	return false;
}

bool ITestItem::InRange(double dLowerBound, double dUpperBound, double dMeasured)
{
	if (dMeasured <= dUpperBound && dMeasured >= dLowerBound)
		return true;

	return false;
}


bool ITestItem::Initial()
{
	return true;
}

bool ITestItem::IsConnected()
{
	return true;
}

// bool ITestItem::GetFA(char* szValue, int iSize)
// {
// 	return true;
// }

bool ITestItem::Begin()
{
	return true;
}

bool ITestItem::PreRun()
{
	return true;
}

bool ITestItem::PostRun()
{
	return true;
}

bool ITestItem::Stop()
{
	return true;
}

bool ITestItem::Disconnect()
{
	return true;
}

bool ITestItem::End()
{
	return true;
}

bool ITestItem::SetFAData( int i_slot, char* sz_value )
{
	return true;
}

bool ITestItem::GetFAData( int i_slot, char* sz_value, int i_size )
{
	return true;
}

bool ITestItem::GetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType )
{
	return true;
}

bool ITestItem::SetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	return true;
}
