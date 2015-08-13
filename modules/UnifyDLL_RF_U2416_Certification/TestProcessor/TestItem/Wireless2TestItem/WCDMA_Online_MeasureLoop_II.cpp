#include "stdafx.h"
#include <math.h>
#include "WCDMA_Online_MeasureLoop_II.h"


RF_IMPLEMENT_DYNCREATE(CWCDMA_Online_MeasureLoop_II)


CWCDMA_Online_MeasureLoop_II::CWCDMA_Online_MeasureLoop_II()
{
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_MaximunPower);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_MinimunPower);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_ErrorVectorMagnitude);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_FrequencyError);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_PeakCodeDomainError);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_AdjacentChannelLeakageRatio);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_OccupiedBandwidth);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_SpectrumEmissionMask);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_InnerLoopPowerControl);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_RefBER);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_MaxBER);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_MinBER);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_OffPower);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_OpenLoopPowerControl);
	m_vInitDataFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InitData_CPK);


	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::MaximunPower);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::MinimunPower);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::ErrorVectorMagnitude);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::FrequencyError);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::PeakCodeDomainError);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::AdjacentChannelLeakageRatio);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::OccupiedBandwidth);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::SpectrumEmissionMask);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::InnerLoopPowerControl);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::RefBER);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::MaxBER);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::MinBER);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::OffPower);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::OpenLoopPowerControl);
	m_vTestFunction.push_back(&CWCDMA_Online_MeasureLoop_II::CPK);
}


bool CWCDMA_Online_MeasureLoop_II::Run()
{
    TraceLog(MSG_INFO, "Start executing Run of CWCDMA_Online_MeasureLoop") ;


	m_strItem = "WCDMA Measure Loop";
	m_strErrCode = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMsg = "WCDMA measure loop FINISH";

	for (int j = 0; j< (signed)m_vChannelList.size();j++)
	{
		m_iChannel = atoi(m_vChannelList[j].c_str());
		m_strChannel = m_vChannelList[j];

		if (!HandoverUMTS())
		{
			return false;
		}
		

		for (int i = 0; i < (signed)m_vTestList.size(); ++i)
		{
			if (strstr(m_vTestList[i].c_str(), "1") == NULL)
				continue;

			// Reset UI
			m_strItem = "-" ;
			m_strErrCode.clear() ;
			m_strLower = "-";
			m_strUpper = "-";
			m_strMeasured = "-";
			m_strUnit = "-";
			m_strMsg = "-";

			bool bRet = CALL_MEMBER_FN(*this, m_vTestFunction[i])() ;

			if ( bRet ) 
			{
				m_strErrCode = "Pass";
			} 
			else 
			{
				if ( m_strErrCode.empty() )
				{
					m_strErrCode = FunErr_WirelessWCDMA_Test_Set_Command_Fail ;
				}
			}
			FactoryLog(bRet, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

			if (!bRet)
			{
				return false;
			}

		}

	}
		
    return true;
}


bool CWCDMA_Online_MeasureLoop_II::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find("Band") == paramMap.end())
		return false;
	m_iBand = atoi(paramMap["Band"].c_str());
	m_strBand = "UMTSBand" + paramMap["Band"];

	if (paramMap.find("ChannelList") == paramMap.end())
		return false;
	//m_iChannel = atoi(paramMap["Channel"].c_str());
	m_strChannel = paramMap["ChannelList"];
	STDStringTokenizer(m_strChannel, ",", m_vChannelList);

	if (paramMap.find("BSOutputPower") == paramMap.end())
		return false;
	m_dBSOutputPower = atof(paramMap["BSOutputPower"].c_str());

	if (paramMap.find("MaxMSPower") == paramMap.end())
		return false;
	m_dMaxMSPower = atof(paramMap["MaxMSPower"].c_str());

	if (paramMap.find("MSTargetPower") == paramMap.end())
		return false;
	m_dMSTargetPower = atof(paramMap["MSTargetPower"].c_str());


	// Copy Data
	m_paramMap = paramMap ;

	if (m_paramMap.find("TestList") == m_paramMap.end()) {
		TraceLog(MSG_ERROR, "Fail to Get <TestList> from XML") ;
		return false ; }
	m_strTestList = m_paramMap["TestList"];
	STDStringTokenizer(m_strTestList, ",", m_vTestList);


	for (int i = 0; i < (signed)m_vInitDataFunction.size(); ++i)
	{
		if ( !QueryTestItemIsEnable(m_vTestList, i) )
			continue;

		bool bRet = CALL_MEMBER_FN(*this, m_vInitDataFunction[i])() ;

		if ( !bRet ) {
			FactoryLog(bRet, m_strMsg) ;
			return false ;
		}
	}

return true ;
	
}

bool CWCDMA_Online_MeasureLoop_II::CheckCallStatus()
{
	if (! m_pITesterDevice->WCDMA_QuerryCallStatus())
	{
		/*
		m_strMsg = "Fail to query call status, starting make call procedure";
		TraceLog(MSG_ERROR, m_strMsg) ;

		if (! m_pITesterDevice->WCDMA_SetBSOutputPower(-60.0))
		{
			m_strMsg = "Fail to set WCDMA BS output power to -60.0";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->WCDMA_SetMaxMSPower(33.0))
		{
			m_strMsg = "Fail to set WCDMA maximum allow power";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->WCDMA_SetMSTargetPower(-20.0))
		{
			m_strMsg = "Fail to set WCDMA MS target power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! WCDMA_StartCall())
		{
			m_strMsg = "Fail to WCDMA MT call status, try again";
			TraceLog(MSG_ERROR, m_strMsg) ;

			if (! m_pIPhone->Reboot())
			{
				m_strMsg = "Fail to reboot handset";
				TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}

			Sleep(25000);

			int iRetry = 30;
			bool bIsConnected = false;
			do
			{
				if (m_pIPhone->IsConnected())
				{
					bIsConnected = true;
					break;
				}

				Sleep(500);
				--iRetry;
			} while (iRetry >= 0);

			if (! bIsConnected)
			{
				m_strMsg = "Fail to connect handset after reboot";
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}

			if (! WCDMA_StartCall())
			{
				m_strMsg = "Fail to WCDMA MT call";
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}
		}
		*/
		if (! m_pITesterDevice->WCDMA_QuerryCallStatus())
		{
			m_strMsg = "Fail to Querry Call Status";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::WCDMA_StartCall()
{
	if (! m_pITesterDevice->WCDMA_QuerryCampStatus())
	{
		m_strMsg = "Fail to camp on BS, try MT call without register" ;
		TraceLog(MSG_ERROR, m_strMsg) ;

		if (! m_pITesterDevice->WCDMA_MTCall())
		{
			m_strMsg = "Fail to WCDMA MT call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		else
			return true;
	}

	if (! m_pITesterDevice->WCDMA_MTCall())
	{
		m_strMsg = "Fail to WCDMA MT call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//   InitData Functions
//////////////////////////////////////////////////////////////////////////

bool CWCDMA_Online_MeasureLoop_II::InitData_MaximunPower()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 1.  Maximum Power
	if (m_paramMap.find("Range_MaxPower") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_MaxPower"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerMaxPower = atof(vTmp[0].c_str());
	m_dUpperMaxPower = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_MinimunPower()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 2.  Minimum Power
	if (m_paramMap.find("Range_MinPower") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_MinPower"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerMinPower = atof(vTmp[0].c_str());
	m_dUpperMinPower = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_ErrorVectorMagnitude()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 3.  EVM
	if (m_paramMap.find("MSTargetPower_EVM") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["MSTargetPower_EVM"];
	STDStringTokenizer(strTmp, ",", m_vMSTargetPower_EVM);

	if (m_paramMap.find("Range_EVM") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_EVM"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerEVM = atof(vTmp[0].c_str());
	m_dUpperEVM = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_FrequencyError()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 4.  Frequency Error
	if (m_paramMap.find("BSOutputPower_FE") == m_paramMap.end())
		return false;
	m_dBSOutputPower_FE = atof(m_paramMap["BSOutputPower_FE"].c_str());

	if (m_paramMap.find("MSTargetPower_FE") == m_paramMap.end())
		return false;
	m_dMSTargetPower_FE = atof(m_paramMap["MSTargetPower_FE"].c_str());

	if (m_paramMap.find("Range_FE") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_FE"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerFE = atof(vTmp[0].c_str());
	m_dUpperFE = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_PeakCodeDomainError()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 5.  Peak Code Domain Error
	if (m_paramMap.find("MSTargetPower_PCDE") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["MSTargetPower_PCDE"];
	STDStringTokenizer(strTmp, ",", m_vMSTargetPower_PCDE);

	if (m_paramMap.find("Range_PCDE") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_PCDE"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerPCDE = atof(vTmp[0].c_str());
	m_dUpperPCDE = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_AdjacentChannelLeakageRatio()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 6.  ACLR
	if (m_paramMap.find("MSTargetPower_ACLR") == m_paramMap.end())
		return false;
	m_dMSTargetPower_ACLR = atof(m_paramMap["MSTargetPower_ACLR"].c_str());

	if (m_paramMap.find("Range_ACLR_5MHz") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_ACLR_5MHz"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerACLR_5MHz = atof(vTmp[0].c_str());
	m_dUpperACLR_5MHz = atof(vTmp[1].c_str());

	if (m_paramMap.find("Range_ACLR_10MHz") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_ACLR_10MHz"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerACLR_10MHz = atof(vTmp[0].c_str());
	m_dUpperACLR_10MHz = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_OccupiedBandwidth()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 7.  OBW
	if (m_paramMap.find("MSTargetPower_OWB") == m_paramMap.end())
		return false;
	m_dMSTargetPower_OBW = atof(m_paramMap["MSTargetPower_OWB"].c_str());

	if (m_paramMap.find("Range_OWB") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_OWB"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerOBW = atof(vTmp[0].c_str());
	m_dUpperOBW = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_SpectrumEmissionMask()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 8.  SEM
	if (m_paramMap.find("MSTargetPower_SEM") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter MSTargetPower_SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dMSTargetPower_SEM = atof(m_paramMap["MSTargetPower_SEM"].c_str());

	if (m_paramMap.find("FreqList_SEM") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter FreqList_SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	STDStringTokenizer(m_paramMap["FreqList_SEM"], ",", m_vFreqList_SEM);

	if (m_paramMap.find("Range_SEM") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range_SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	STDStringTokenizer(m_paramMap["Range_SEM"], ",", m_vRange_SEM);

	if (m_vFreqList_SEM.size() != m_vRange_SEM.size())
	{
		m_strMsg = "FreqList_SEM and Range_SEM size mismatch";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_InnerLoopPowerControl()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 9.  InnerLoop
	if (m_paramMap.find("MaxMSPower_ILPC") == m_paramMap.end())
		return false;
	m_dMaxMSPower_ILPC = atof(m_paramMap["MaxMSPower_ILPC"].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_RefBER()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 10. Reference BER
	if (m_paramMap.find("BSOutputPower_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter BSOutputPower_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dBSOutputPower_RefBER = atof(m_paramMap["BSOutputPower_RefBER"].c_str());

	if (m_paramMap.find("MSTargetPower_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter MSTargetPower_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dMSTargetPower_RefBER = atof(m_paramMap["MSTargetPower_RefBER"].c_str());

	if (m_paramMap.find("Range_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	strTmp = m_paramMap["Range_RefBER"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dLowerRefBER = atof(vTmp[0].c_str());
	m_dUpperRefBER = atof(vTmp[1].c_str());

	// Retry times
	if (m_paramMap.find("RetryTimes") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <RetryTimes> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	m_iRetryTimes = atoi(m_paramMap["RetryTimes"].c_str()) ;

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_MaxBER()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 11. Maximum BER
	if (m_paramMap.find("BSOutputPower_MaxBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter BSOutputPower_MaxBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dBSOutputPower_MaxBER = atof(m_paramMap["BSOutputPower_MaxBER"].c_str());

	if (m_paramMap.find("MSTargetPower_MaxBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter MSTargetPower_MaxBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dMSTargetPower_MaxBER = atof(m_paramMap["MSTargetPower_MaxBER"].c_str());

	if (m_paramMap.find("Range_MaxBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range_MaxBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	strTmp = m_paramMap["Range_MaxBER"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range_MaxBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dLowerMaxBER = atof(vTmp[0].c_str());
	m_dUpperMaxBER = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_MinBER()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 12. Minimum BER
	if (m_paramMap.find("BSOutputPower_MinBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter BSOutputPower_MinBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dBSOutputPower_MinBER = atof(m_paramMap["BSOutputPower_MinBER"].c_str());

	if (m_paramMap.find("MSTargetPower_MinBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter MSTargetPower_MinBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dMSTargetPower_MinBER = atof(m_paramMap["MSTargetPower_MinBER"].c_str());

	if (m_paramMap.find("Range_MinBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range_MinBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	strTmp = m_paramMap["Range_MinBER"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range_MinBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dLowerMinBER = atof(vTmp[0].c_str());
	m_dUpperMinBER = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_OffPower()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 13. OffPower
	if (m_paramMap.find("BSOutputPower_Off") == m_paramMap.end())
		return false;
	m_dBSOutputPower_Off = atof(m_paramMap["BSOutputPower_Off"].c_str());

	if (m_paramMap.find("PCPICH_Power_Off") == m_paramMap.end())
		return false;
	m_dPCPICHPower_Off = atof(m_paramMap["PCPICH_Power_Off"].c_str());

	if (m_paramMap.find("ULInterference_Off") == m_paramMap.end())
		return false;
	m_dULInterference_Off = atof(m_paramMap["ULInterference_Off"].c_str());

	if (m_paramMap.find("ConstantValue_Off") == m_paramMap.end())
		return false;
	m_dConstantValue_Off = atof(m_paramMap["ConstantValue_Off"].c_str());

	if (m_paramMap.find("WaitTimeForFetch_Off") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter WaitTimeForFetch_Off for CWCDMA_Online_MeasureLoop";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_iWaitTimeForFetch_Off = atoi(m_paramMap["WaitTimeForFetch_Off"].c_str());

	if (m_paramMap.find("Range_Off") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_Off"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerOff = atof(vTmp[0].c_str());
	m_dUpperOff = atof(vTmp[1].c_str());

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_OpenLoopPowerControl()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 14. OpenLoop
	if (m_paramMap.find("MaxMSPower_OL") == m_paramMap.end())
		return false;
	m_dMaxMSPower_OL = atof(m_paramMap["MaxMSPower_OL"].c_str());

	if (m_paramMap.find("BSOutputPower_OL") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["BSOutputPower_OL"];
	STDStringTokenizer(strTmp, ",", m_vBSOutputPower_OL);

	if (m_paramMap.find("PCPICH_Power_OL") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["PCPICH_Power_OL"];
	STDStringTokenizer(strTmp, ",", m_vPCPICHPower_OL);

	if (m_paramMap.find("ULInterference_OL") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["ULInterference_OL"];
	STDStringTokenizer(strTmp, ",", m_vULInterference_OL);

	if (m_paramMap.find("ConstantValue_OL") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["ConstantValue_OL"];
	STDStringTokenizer(strTmp, ",", m_vConstantValue_OL);

	if (m_paramMap.find("WaitTimeForFetch_OL") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter WaitTimeForFetch_OL for CWCDMA_Online_MeasureLoop";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_iWaitTimeForFetch_OL = atoi(m_paramMap["WaitTimeForFetch_OL"].c_str());

	if (m_paramMap.find("Range_OL") == m_paramMap.end())
		return false;
	strTmp = m_paramMap["Range_OL"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerOL = atof(vTmp[0].c_str());
	m_dUpperOL = atof(vTmp[1].c_str());

	return true;
}

//////////////////////////////////////////////////////////////////////////
//   Measure Functions
//////////////////////////////////////////////////////////////////////////

bool CWCDMA_Online_MeasureLoop_II::MaximunPower()
{
	m_strItem = "MaxPower";
	m_strMeasured = "-";
	m_strUnit = "dBm";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerMaxPower);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperMaxPower);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitMaxPowerMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA maximum power measurement";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchMaxPower(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA maximum power";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vMaxPowerList;
	if (! STDStringTokenizer(strPesponse, ",", vMaxPowerList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA maximum power";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dAvgMaxPower = atof(vMaxPowerList[4].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dAvgMaxPower);
	m_strMeasured = szTmp;

	if (InRange(m_dLowerMaxPower, m_dUpperMaxPower, dAvgMaxPower))
	{
		m_bResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MaxPower,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dAvgMaxPower);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA maximum power PASS") ;
	}
	else
	{
		m_bResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MaxPower,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dAvgMaxPower);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA maximum power FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	return m_bResult;
}

bool CWCDMA_Online_MeasureLoop_II::MinimunPower()
{
	m_strItem = "MinPower";
	m_strMeasured = "-";
	m_strUnit = "dBm";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerMinPower);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperMinPower);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitMinPowerMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA minimum power measurement";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchMinPower(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA minimum power";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vMinPowerList;
	if (! STDStringTokenizer(strPesponse, ",", vMinPowerList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA minimum power";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dAvgMinPower = atof(vMinPowerList[4].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dAvgMinPower);
	m_strMeasured = szTmp;

	if (InRange(m_dLowerMinPower, m_dUpperMinPower, dAvgMinPower))
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MinPower,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dAvgMinPower);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA minimum power PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MinPower,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dAvgMinPower);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA minimum power FAIL" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::ErrorVectorMagnitude()
{
	m_strItem = "EVM";
	m_strMeasured = "-";
	m_strUnit = "%";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerEVM);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperEVM);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	for (int i = 0; i < (signed int)m_vMSTargetPower_EVM.size(); ++i)
	{
		double dTmp = atof(m_vMSTargetPower_EVM[i].c_str());

		if (! m_pITesterDevice->WCDMA_SetMSTargetPower(dTmp))
		{
			m_strMsg = "Fail to set WCDMA MS target power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! CheckCallStatus())
		{
			m_strMsg = "Fail to check call status, loss of connection";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->WCDMA_InitModulationMeasurement())
		{
			m_strMsg = "Fail to initial tester device for WCDMA EVM measurement";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::string strPesponse;
		if (! m_pITesterDevice->WCDMA_FetchModulation(strPesponse))
		{
			m_strMsg = "Fail to fetch WCDMA EVM";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::vector<std::string> vModulationList;
		if (! STDStringTokenizer(strPesponse, ",", vModulationList))
		{
			m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA EVM";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		dTmp = atof(vModulationList[4].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dTmp);
		m_strMeasured = szTmp;

		if (InRange(m_dLowerEVM, m_dUpperEVM, dTmp))
		{
            m_bTestResult = true ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,EVM,%d,%d,%s,,%.3f,1", m_iBand, m_iChannel, m_vMSTargetPower_EVM[i].c_str(), dTmp);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA EVM at MS target power %s dBm PASS", m_vMSTargetPower_EVM[i].c_str());
			TraceLog(MSG_INFO, szTmp);
		}
		else
		{
            m_bTestResult = false ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,EVM,%d,%d,%s,,%.3f,0", m_iBand, m_iChannel, m_vMSTargetPower_EVM[i].c_str(), dTmp);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA EVM at MS target power %s dBm FAIL", m_vMSTargetPower_EVM[i].c_str());
			m_strMsg = szTmp;
            TraceLog(MSG_ERROR, szTmp);

		}

		if ((signed)m_vMSTargetPower_EVM.size() > 1 && i < ((signed)m_vMSTargetPower_EVM.size() - 1))
			FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::FrequencyError()
{
	m_strItem = "FreqErr";
	m_strMeasured = "-";
	m_strUnit = "Hz";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerFE);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperFE);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower_FE))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_FE))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitModulationMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA Frequency Error measurement";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchModulation(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA Frequency Error";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vModulationList;
	if (! STDStringTokenizer(strPesponse, ",", vModulationList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA Frequency Error";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dTmp = atof(vModulationList[25].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dMSTargetPower_FE);
	std::string strTmp = szTmp;

	if (InRange(m_dLowerEVM, m_dUpperEVM, dTmp))
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,FreqError,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA Frequency Error PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,FreqError,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA Frequency Error FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::PeakCodeDomainError()
{
	m_strItem = "PCDE";
	m_strMeasured = "-";
	m_strUnit = "dB";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerPCDE);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperPCDE);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	for (int i = 0; i < (signed)m_vMSTargetPower_PCDE.size(); ++i)
	{
		double dTmp = atof(m_vMSTargetPower_PCDE[i].c_str());
		if (! m_pITesterDevice->WCDMA_SetMSTargetPower(dTmp))
		{
			m_strMsg = "Fail to set WCDMA MS target power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! CheckCallStatus())
		{
			m_strMsg = "Fail to check call status, loss of connection";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->WCDMA_InitModulationMeasurement())
		{
			m_strMsg = "Fail to initial tester device for WCDMA Peak Code Domain Error";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::string strPesponse;
		if (! m_pITesterDevice->WCDMA_FetchModulation(strPesponse))
		{
			m_strMsg = "Fail to fetch WCDMA Peak Code Domain Error";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::vector<std::string> vModulationList;
		if (! STDStringTokenizer(strPesponse, ",", vModulationList))
		{
			m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA Peak Code Domain Error";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		dTmp = atof(vModulationList[31].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dTmp);
		m_strMeasured = szTmp;

		if (InRange(m_dLowerPCDE, m_dUpperPCDE, dTmp))
		{
            m_bTestResult = true ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,PCDE,%d,%d,%s,,%.3f,1", m_iBand, m_iChannel, m_vMSTargetPower_PCDE[i].c_str(), dTmp);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA PCDE at MS target power %s dBm PASS", m_vMSTargetPower_PCDE[i].c_str());
			TraceLog(MSG_INFO, szTmp);
		}
		else
		{
            m_bTestResult = false ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,PCDE,%d,%d,%s,,%.3f,0", m_iBand, m_iChannel, m_vMSTargetPower_PCDE[i].c_str(), dTmp);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA PCDE at MS target power %s dBm FAIL", m_vMSTargetPower_PCDE[i].c_str());
			m_strMsg = szTmp;
            TraceLog(MSG_ERROR, m_strMsg) ;
		}

		if ((signed)m_vMSTargetPower_PCDE.size() > 1 && i < ((signed)m_vMSTargetPower_PCDE.size() - 1))
			FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::AdjacentChannelLeakageRatio()
{
	m_strItem = "ACLR";
	m_strLower = "-";
	m_strUpper = "-";
	m_strMeasured = "-";
	m_strUnit = "dB";

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_ACLR))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitACLR_OBW_Measurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA ACLR";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchACLR_OBW(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ACLR";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vACLRList;
	if (! STDStringTokenizer(strPesponse, ",", vACLRList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA ACLR";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dTmp;
	char szTmp[256];

	// -10 MHz
	dTmp = atof(vACLRList[6].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerACLR_10MHz);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperACLR_10MHz);
	m_strUpper = szTmp;

	if (InRange(m_dLowerACLR_10MHz, m_dUpperACLR_10MHz, dTmp))
	{
        m_bTestResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,-10,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ACLR -10.0 MHz PASS") ;
	}
	else
	{
        m_bTestResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,-10,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ACLR -10.0 MHz FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// -5 MHz
	dTmp = atof(vACLRList[7].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerACLR_5MHz);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperACLR_5MHz);
	m_strUpper = szTmp;

	if (InRange(m_dLowerACLR_5MHz, m_dUpperACLR_5MHz, dTmp))
	{
        m_bTestResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,-5,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ACLR -5.0 MHz PASS") ;
	}
	else
	{
        m_bTestResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,-5,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_ERROR, "WCDMA ACLR -5.0 MHz FAIL") ;
	}

	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// +5 MHz
	dTmp = atof(vACLRList[8].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerACLR_5MHz);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperACLR_5MHz);
	m_strUpper = szTmp;

	if (InRange(m_dLowerACLR_5MHz, m_dUpperACLR_5MHz, dTmp))
	{
        m_bTestResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,+5,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ACLR +5.0 MHz PASS") ;
	}
	else
	{
        m_bTestResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,+5,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ACLR +5.0 MHz FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// +10 MHz
	dTmp = atof(vACLRList[9].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerACLR_10MHz);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperACLR_10MHz);
	m_strUpper = szTmp;

	if (InRange(m_dLowerACLR_10MHz, m_dUpperACLR_10MHz, dTmp))
	{
        m_bTestResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,+10,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ACLR +10.0 MHz PASS") ;
	}
	else
	{
        m_bTestResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ACLR,%d,%d,+10,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ACLR +10.0 MHz FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::OccupiedBandwidth()
{
	m_strItem = "OBW";
	m_strMeasured = "-";
	m_strUnit = "MHz";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerOBW);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperOBW);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_OBW))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitACLR_OBW_Measurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA OBW";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchACLR_OBW(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA OBW";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vOBWList;
	if (! STDStringTokenizer(strPesponse, ",", vOBWList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA OBW";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dTmp = atof(vOBWList[15].c_str());
	dTmp /= 1000000;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dTmp);
	m_strMeasured = szTmp;

	if (InRange(m_dLowerOBW, m_dUpperOBW, dTmp))
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OBW,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA OBW PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OBW,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dTmp);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA OBW FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::SpectrumEmissionMask()
{
	m_strItem = "SEM";
	m_strLower = "-99.0";
	m_strUpper = "-";
	m_strMeasured = "-";
	m_strUnit = "dBm";
	m_strMsg = "";

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_SEM))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitSEMMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	if (! m_pITesterDevice->WCDMA_FetchSEM(strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vSEMList;
	if (! STDStringTokenizer(strPesponse, ",", vSEMList))
	{
		m_strMsg = "Fail to execute function STDStringTokenizer for WCDMA SEM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	else
	{
		if (vSEMList.size() < 1340)
		{
			m_strMsg = "Fail to test WCDMA SEM, data not enough or NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	/*
	1    ~ 321  : -12.000 MHz ~ -4.000 MHz,  divided by 25 KHz
	322  ~ 1019 : -3.485 MHz  ~ +3.485 MHz,  divided by 10 KHz
	1020 ~ 1340 : +4.000      ~ +12.000 MHz, divided by 25 KHz
	*/

	bool bRes = true;

	for (int i = 0; i < (signed)m_vFreqList_SEM.size(); ++i)
	{
		char szTmp[256];
		double dTargetFreq = atof(m_vFreqList_SEM[i].c_str());
		double dMeasured = 0.0;
		double dUpper = atof(m_vRange_SEM[i].c_str());

		m_strUpper = m_vRange_SEM[i];

		if (dTargetFreq >= -12.0 && dTargetFreq <= -4.0)
		{
			double dBegin = -12.0;
			int iBegin = 0;

			int iIndex = iBegin + (int)((dTargetFreq - dBegin) / 0.025);
			dMeasured = atof(vSEMList[iIndex].c_str());
		}
		else if (dTargetFreq >= -3.485 && dTargetFreq <= 3.485)
		{
			if (dTargetFreq == -3.485)
				dMeasured = atof(vSEMList[321].c_str());
			else if (dTargetFreq == 3.485)
				dMeasured = atof(vSEMList[1018].c_str());
			else
			{
				double dBegin = -3.49;
				int iBegin = 321;

				int iIndex = iBegin + (int)((dTargetFreq - dBegin) / 0.01);
				dMeasured = atof(vSEMList[iIndex].c_str());
			}
		}
		else if (dTargetFreq >= 4.0 && dTargetFreq <= 12.0)
		{
			double dBegin = 4.0;
			int iBegin = 1019;

			int iIndex = iBegin + (int)((dTargetFreq - dBegin) / 0.025);
			dMeasured = atof(vSEMList[iIndex].c_str());
		}
		else
		{
			m_strMsg = "Fail to test WCDMA SEM, invalid target frequency";
			TraceLog(MSG_ERROR, m_strMsg) ;
		}

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dMeasured);
		m_strMeasured = szTmp;

		if (InRange(-99.0, dUpper, dMeasured))
		{
            m_bTestResult = true ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,SEM,%d,%d,%.3f,,%.3f,1", m_iBand, m_iChannel, dTargetFreq, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA SEM %.3f PASS", dTargetFreq);
			m_strMsg = szTmp;
		}
		else
		{
            m_bTestResult = false ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,SEM,%d,%d,%.3f,,%.3f,0", m_iBand, m_iChannel, dTargetFreq, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "WCDMA SEM %.3f FAIL", dTargetFreq);
			m_strMsg = szTmp;
		}

		if (i < ((signed)m_vFreqList_SEM.size() - 1))
			FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}

	return bRes;
}

bool CWCDMA_Online_MeasureLoop_II::InnerLoopPowerControl()
{
	m_strItem = "ILPC";
	m_strLower = "-";
	m_strUpper = "-";
	m_strMeasured = "-";
	m_strUnit = "P/F";

	char szTmp[256];

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower_ILPC))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! CheckCallStatus())
	{
		m_strMsg = "Fail to check call status, loss of connection";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetTPCPattern("SET1"))
	{
		m_strMsg = "Fail to set TPC pattern to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetTPCPatternType("CLOP"))
	{
		m_strMsg = "Fail to set TPC pattern type to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitILPC())
	{
		m_strMsg = "Fail to initial tester device for WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strPesponse;
	std::vector<std::string> vILPCList;
	bool bPass;

	// ILPC Step A
	if (! m_pITesterDevice->WCDMA_FetchILPC("A", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step A measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 1; (i < ((signed)vILPCList.size() - 1)) && bPass; ++i)
	{
		double dFront = atof(vILPCList[i - 1].c_str());
		double dCurrent = atof(vILPCList[i].c_str());
		double dRare = atof(vILPCList[i + 1].c_str());

		if (abs(dCurrent - dFront) > 0.1 && abs(dRare - dCurrent) > 0.1)
			bPass = false;
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,A,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step A PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,A,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step A FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step B
	if (! m_pITesterDevice->WCDMA_FetchILPC("B", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step B measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 0; (i < (signed)vILPCList.size()) && bPass; i = i + 5)
	{
		double dFront, dRare;
		double dNum[5];
		int iCount = 0;

		if (i > 0)
			dFront = atof(vILPCList[i - 1].c_str());

		if (i < (signed)vILPCList.size())
		{
			dNum[0] = atof(vILPCList[i].c_str());
			++iCount;
		}

		if ((i + 1) < (signed)vILPCList.size())
		{
			dNum[1] = atof(vILPCList[i + 1].c_str());
			++iCount;
		}

		if ((i + 2) < (signed)vILPCList.size())
		{
			dNum[2] = atof(vILPCList[i + 2].c_str());
			++iCount;
		}

		if ((i + 3) < (signed)vILPCList.size())
		{
			dNum[3] = atof(vILPCList[i + 3].c_str());
			++iCount;
		}

		if ((i + 4) < (signed)vILPCList.size())
		{
			dNum[4] = atof(vILPCList[i + 4].c_str());
			++iCount;
		}

		if ((i + 5) < (signed)vILPCList.size())
			dRare = atof(vILPCList[i + 5].c_str());

		for (int j = 1; (j < iCount) && bPass; ++j)
		{
			if (abs(dNum[j] - dNum[j - 1]) > 0.1)
				bPass = false;
		}

		if (i > 0 && bPass)
		{
			if ((dNum[0] - dFront) < 0.5 || (dNum[0] - dFront) > 1.5)
				bPass = false;
		}

		if ((i + 5) < (signed)vILPCList.size() && bPass)
		{
			if ((dRare - dNum[4]) < 0.5 || (dRare - dNum[4]) > 1.5)
				bPass = false;
		}
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,B,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step B PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,B,,1,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step B FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step C
	if (! m_pITesterDevice->WCDMA_FetchILPC("C", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step C measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 0; (i < (signed)vILPCList.size()) && bPass; i = i + 5)
	{
		double dFront, dRare;
		double dNum[5];
		int iCount = 0;

		if (i > 0)
			dFront = atof(vILPCList[i - 1].c_str());

		if (i < (signed)vILPCList.size())
		{
			dNum[0] = atof(vILPCList[i].c_str());
			++iCount;
		}

		if ((i + 1) < (signed)vILPCList.size())
		{
			dNum[1] = atof(vILPCList[i + 1].c_str());
			++iCount;
		}

		if ((i + 2) < (signed)vILPCList.size())
		{
			dNum[2] = atof(vILPCList[i + 2].c_str());
			++iCount;
		}

		if ((i + 3) < (signed)vILPCList.size())
		{
			dNum[3] = atof(vILPCList[i + 3].c_str());
			++iCount;
		}

		if ((i + 4) < (signed)vILPCList.size())
		{
			dNum[4] = atof(vILPCList[i + 4].c_str());
			++iCount;
		}

		if ((i + 5) < (signed)vILPCList.size())
			dRare = atof(vILPCList[i + 5].c_str());

		for (int j = 1; (j < iCount) && bPass; ++j)
		{
			if (abs(dNum[j] - dNum[j - 1]) > 0.1)
				bPass = false;
		}

		if (i > 0 && bPass)
		{
			if ((dFront - dNum[0]) < 0.5 || (dFront - dNum[0]) > 1.5)
				bPass = false;
		}

		if ((i + 5) < (signed)vILPCList.size() && bPass)
		{
			if ((dNum[4] - dRare) < 0.5 || (dNum[4] - dRare) > 1.5)
				bPass = false;
		}
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,C,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step C PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,C,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step C FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step D
	if (! m_pITesterDevice->WCDMA_FetchILPC("D", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	// ILPC Step E
	if (! m_pITesterDevice->WCDMA_FetchILPC("E", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step E measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 1; (i < ((signed)vILPCList.size() - 1)) && bPass; ++i)
	{
		double dFront = atof(vILPCList[i - 1].c_str());
		double dCurrent = atof(vILPCList[i].c_str());
		double dRare = atof(vILPCList[i + 1].c_str());

		if ((dFront - dCurrent) < 0.5 || (dFront - dCurrent) > 1.5 || (dCurrent - dRare) < 0.5 || (dCurrent - dRare) > 1.5)
		{
			if (dFront <= -57.0 || dCurrent <= -57.0 || dRare <= -57.0)
			{
				bPass = true;
				break;
			}
			else
				bPass = false;
		}
	}

	if (bPass)
	{
        m_bTestResult = true ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,E,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step E PASS") ;
	}
	else
	{
        m_bTestResult = false ;
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,E,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step E FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step F
	if (! m_pITesterDevice->WCDMA_FetchILPC("F", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step F measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 1; (i < ((signed)vILPCList.size() - 1)) && bPass; ++i)
	{
		double dFront = atof(vILPCList[i - 1].c_str());
		double dCurrent = atof(vILPCList[i].c_str());
		double dRare = atof(vILPCList[i + 1].c_str());

		if ((dCurrent - dFront) < 0.5 || (dCurrent - dFront) > 1.5 || (dRare - dCurrent) < 0.5 || (dRare - dCurrent) > 1.5)
		{
			if (dFront >= 23.0 || dCurrent >= 23.0 || dRare >= 23.0)
			{
				bPass = true;
				break;
			}
			else
				bPass = false;
		}
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,F,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step F PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,F,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step F FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step G
	if (! m_pITesterDevice->WCDMA_FetchILPC("G", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = true;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step G measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 1; (i < ((signed)vILPCList.size() - 1)) && bPass; ++i)
	{
		double dFront = atof(vILPCList[i - 1].c_str());
		double dCurrent = atof(vILPCList[i].c_str());
		double dRare = atof(vILPCList[i + 1].c_str());

		if ((dFront - dCurrent) < 1.5 || (dFront - dCurrent) > 2.5 || (dCurrent - dRare) < 1.5 || (dCurrent - dRare) > 2.5)
		{
			if (dFront <= -57.0 || dCurrent <= -57.0 || dRare <= -57.0)
			{
				bPass = true;
				break;
			}
			else
				bPass = false;
		}
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,G,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step G PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,G,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step G FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

    m_bTestResult = bPass ;
	FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

	// ILPC Step H
	if (! m_pITesterDevice->WCDMA_FetchILPC("H", strPesponse))
	{
		m_strMsg = "Fail to fetch WCDMA ILPC";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bPass = false;
	STDStringTokenizer(strPesponse, ",", vILPCList);

	for (int i = 0; i < (signed)vILPCList.size(); ++i)
	{
		TraceLog(MSG_INFO, "ILPC step H measured :") ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%00d : %.3f", i, atof(vILPCList[i].c_str()));
		TraceLog(MSG_INFO, szTmp);
	}

	for (int i = 1; i < ((signed)vILPCList.size() - 1); ++i)
	{
		double dFront = atof(vILPCList[i - 1].c_str());
		double dCurrent = atof(vILPCList[i].c_str());
		double dRare = atof(vILPCList[i + 1].c_str());

		if ((dCurrent - dFront) < 1.5 || (dCurrent - dFront) > 2.5 || (dRare - dCurrent) < 1.5 || (dRare - dCurrent) > 2.5)
		{
			if (dFront >= 23.0 || dCurrent >= 23.0 || dRare >= 23.0)
			{
				bPass = true;
				break;
			}
			else
				bPass = false;
		}
	}

	if (bPass)
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,H,,1,1", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA ILPC Step H PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,ILPC,%d,%d,H,,0,0", m_iBand, m_iChannel);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA ILPC Step H FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	m_pITesterDevice->WCDMA_SetTPCPattern("SET1") ;
	m_pITesterDevice->WCDMA_SetTPCPatternType("CLOP") ;
	m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower);

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::RefBER()
{
	//m_strItem = "RefBER";
	m_strMeasured = "-";
	m_strUnit = "%";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerRefBER);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperRefBER);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower_RefBER))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_RefBER))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitBERMMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA BER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	std::string strMeasured;
	if (! m_pITesterDevice->WCDMA_FetchBERMMeasurement(strMeasured))
	{
		m_strMsg = "Fail to fetch WCDMA BER value from tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vTmp;
	STDStringTokenizer(strMeasured, ",", vTmp);

	if (strstr(vTmp[0].c_str(), "NAN") != NULL)
	{
		m_strMsg = "Fail to measure BER value, NAN fetched";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dMeasured = atof(vTmp[0].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMeasured);
	m_strMeasured = szTmp;

	if (! InRange(m_dLowerRefBER, m_dUpperRefBER, dMeasured))
	{
		m_bResult = false ;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless2_MeasurementData,WCDMA,RefBER,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA reference BER not in range";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}
	else
	{
		m_bResult = true ;

		SetPICSDate(m_strBand + "_BER",m_strMeasured);

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,RefBER,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA reference BER PASS") ;
	}


	Sleep(1000);
	std::string strRSCPMeasured;
	if (! m_pITesterDevice->WCDMA_FetchRSCPMeasurement(strRSCPMeasured))
	{
		m_strMsg = "Fail to fetch WCDMA BER value from tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	else
	{
		STDStringTokenizer(strRSCPMeasured, ",", vTmp);

		if (strstr(vTmp[0].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure BER value, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		double dMeasured0, dMeasured1;
		dMeasured0 = atof(vTmp[0].c_str());
		dMeasured1 = atof(vTmp[1].c_str());
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dMeasured1);
		FactoryLog(true, "RSCP", "Pass", m_strBand, m_strChannel, "--", "--", szTmp, m_strUnit, "Get RSCP pass!" ) ;
		SetPICSDate(m_strBand + "_RSCP",szTmp);
	}
	return m_bResult;
}

bool CWCDMA_Online_MeasureLoop_II::MaxBER()
{
	m_strItem = "MaxBER";
	m_strMeasured = "-";
	m_strUnit = "%";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerMaxBER);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperMaxBER);
	m_strUpper = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower_MaxBER))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_MaxBER))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitBERMMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA BER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	Sleep(3000);

	std::string strMeasured;
	if (! m_pITesterDevice->WCDMA_FetchBERMMeasurement(strMeasured))
	{
		m_strMsg = "Fail to fetch WCDMA BER value from tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::vector<std::string> vTmp;
	STDStringTokenizer(strMeasured, ",", vTmp);

	if (strstr(vTmp[0].c_str(), "NAN") != NULL)
	{
		m_strMsg = "Fail to measure BER value, NAN fetched";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	double dMeasured = atof(vTmp[0].c_str());

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMeasured);
	m_strMeasured = szTmp;

	if (! InRange(m_dLowerMaxBER, m_dUpperMaxBER, dMeasured))
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MaxBER,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA maximum BER not in range";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MaxBER,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA maximum BER PASS") ;
		
		Sleep(1000);
		std::string strRSCPMeasured;
		if (! m_pITesterDevice->WCDMA_FetchRSCPMeasurement(strRSCPMeasured))
		{
			m_strMsg = "Fail to fetch WCDMA BER value from tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		else
		{
			FactoryLog(true, "RSCP", "Pass", m_strBand, m_strChannel, "--", "--", strRSCPMeasured, m_strUnit, "Get RSCP pass!" ) ;
		}

	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::MinBER()
{
	m_strItem = "MinBER";
	m_strMeasured = "-";
	m_strUnit = "%";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerMinBER);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperMinBER);
	m_strUpper = szTmp;

	double dBSPower = m_dBSOutputPower_MinBER;

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_MinBER))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitBERMMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA BER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	int iStatus = 0;
	while (dBSPower <= m_dBSOutputPower_RefBER)
	{
		if (! m_pITesterDevice->WCDMA_SetBSOutputPower(dBSPower))
		{
			m_strMsg = "Fail to set BS output power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(1000);

		std::string strMeasured;
		if (! m_pITesterDevice->WCDMA_FetchBERMMeasurement(strMeasured))
		{
			m_strMsg = "Fail to fetch WCDMA BER value from tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::vector<std::string> vTmp;
		STDStringTokenizer(strMeasured, ",", vTmp);

		if (strstr(vTmp[0].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure BER value, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		double dMeasured = atof(vTmp[0].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "BS output power = %.3f, BER = %.3f", dBSPower, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dBSPower);
		m_strMeasured = szTmp;

		if (InRange(m_dLowerMinBER, m_dUpperMinBER, dMeasured))
		{
			if (iStatus == +1)
			{
				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, "%.3f", dBSPower);
				m_strMeasured = szTmp;

				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MinBER,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dBSPower);
				TraceLog(MSG_INFO, szTmp);

				TraceLog(MSG_INFO, "WCDMA minimum BER FINISH") ;

				return true;
			}
			else
			{
				dBSPower -= 0.1;
				iStatus = -1;
				continue;
			}
		}
		else
		{
			if (iStatus == 0)
			{
				dBSPower += 0.1;
				iStatus = +1;
				continue;
			}
			else if (iStatus == -1)
			{
				dBSPower += 0.1;

				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, "%.3f", dBSPower);
				m_strMeasured = szTmp;

				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MinBER,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dBSPower);
				TraceLog(MSG_INFO, szTmp);

				TraceLog(MSG_INFO, "WCDMA minimum BER FINISH") ;
				return true;
			}
			else
			{
				dBSPower += 0.1;
				iStatus = +1;
				continue;
			}
		}
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,MinBER,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dBSPower);
	TraceLog(MSG_INFO, szTmp);

	TraceLog(MSG_INFO, "WCDMA minimum BER FAIL") ;

	return false;
}

bool CWCDMA_Online_MeasureLoop_II::OffPower()
{
	m_strItem = "OffPower";
	m_strMeasured = "-";
	m_strUnit = "dBm";
	m_strMsg = "";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerOff);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperOff);
	m_strUpper = szTmp;

	if (! m_pIPhone->Reboot())
	{
		m_strMsg = "Fail to reboot phone for WCDMA off power measurement";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->SetSignalOnOff(false))
	{
		m_strMsg = "Fail to set tester device signal OFF";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dBSOutputPower_Off))
	{
		m_strMsg = "Fail to set BS output power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strResponse;
	std::vector<std::string> vOffPowerList;

	sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:INTerference %.3f; *OPC?", m_dULInterference_Off);
	m_pITesterDevice->GPIB_OPComplete(szTmp);

	sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:CVALue %.3f; *OPC?", m_dConstantValue_Off);
	m_pITesterDevice->GPIB_OPComplete(szTmp);

	sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer %.3f; *OPC?", m_dPCPICHPower_Off);
	m_pITesterDevice->GPIB_OPComplete(szTmp);

	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:POWer:ONOFf:CONTrol:MITem MIOO; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:POWer:ONOFf:CONTrol:MPReamble MP4; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:NETWork:RAC:PREamble:SSIZe 1; *OPC?") ;

	if (! m_pITesterDevice->SetSignalOnOff(true))
	{
		m_strMsg = "Fail to set tester device signaling on";
			TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bool bConnect = false;
	int iRetry = 40;
	while (iRetry > 0)
	{
		if (m_pIPhone->IsConnected())
		{
			bConnect = true;
			break;
		}

		--iRetry;
		Sleep(500);
	}

	if (! bConnect)
	{
		m_strMsg = "Fail to connect phone after reboot";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	Sleep(m_iWaitTimeForFetch_Off);

	if (! FetchOff_OL(vOffPowerList))
	{
		m_strMsg = "Fail to fetch WCDMA off power, reboot phone and try again";
		TraceLog(MSG_ERROR, m_strMsg) ;

		if (! m_pIPhone->Reboot())
		{
			m_strMsg = "Fail to reboot phone for WCDMA off power measurement";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(25000);

		bConnect = false;
		iRetry = 20;
		while (iRetry > 0)
		{
			if (m_pIPhone->IsConnected())
			{
				bConnect = true;
				break;
			}

			--iRetry;
			Sleep(500);
		}

		if (! bConnect)
		{
			m_strMsg = "Fail to connect phone after reboot";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(m_iWaitTimeForFetch_Off);

		if (! FetchOff_OL(vOffPowerList))
		{
			m_strMsg = "Fail to fetch WCDMA off power";
			TraceLog(MSG_ERROR, m_strMsg) ;

			m_pITesterDevice->WCDMA_EndCall();
			m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:INTerference -80.0; *OPC?") ;
			m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:CVALue -29.0; *OPC?") ;
			m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer 31.0; *OPC?") ;
			m_pITesterDevice->GPIB_OPComplete("17;CONFigure:POWer:ONOFf:CONTrol:MPReamble MP2; *OPC?") ;
			m_pITesterDevice->GPIB_OPComplete("17;CONFigure:NETWork:RAC:PREamble:SSIZe 3; *OPC?") ;

			return false;
		}
	}

	double dBefore = atof(vOffPowerList[1].c_str());
	double dAfter = atof(vOffPowerList[2].c_str());
	double dMeasured = (dBefore + dAfter) / 2.0;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMeasured);
	m_strMeasured = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerOff);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperOff);
	m_strUpper = szTmp;

	if (InRange(m_dLowerOff, m_dUpperOff, dMeasured))
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OffPower,%d,%d,,,%.3f,1", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		TraceLog(MSG_INFO, "WCDMA off power PASS") ;
	}
	else
	{
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OffPower,%d,%d,,,%.3f,0", m_iBand, m_iChannel, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		m_strMsg = "WCDMA off power FAIL";
		TraceLog(MSG_ERROR, m_strMsg) ;
	}

	m_pITesterDevice->WCDMA_EndCall();
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:INTerference -80.0; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:CVALue -29.0; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer 31.0; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:POWer:ONOFf:CONTrol:MPReamble MP2; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:NETWork:RAC:PREamble:SSIZe 3; *OPC?") ;

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::OpenLoopPowerControl()
{
	m_strItem = "OLPC";
	m_strLower = "-";
	m_strUpper = "-";
	m_strMeasured = "-";
	m_strUnit = "dBm";
	m_strMsg = "";

	char szTmp[256];

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower_OL))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	for (int i = 0; i < (signed)m_vBSOutputPower_OL.size(); ++i)
	{
		std::vector<std::string> vOpenLoopList;
		double dMeasured;

		double dBSPower = atof(m_vBSOutputPower_OL[i].c_str());
		double dPCPICHPower = atof(m_vPCPICHPower_OL[i].c_str());
		double dULInterference = atof(m_vULInterference_OL[i].c_str());
		double dConstantValue = atof(m_vConstantValue_OL[i].c_str());

		m_strMeasured = "-";

		m_pITesterDevice->WCDMA_EndCall();

		m_pITesterDevice->WCDMA_SetBSOutputPower(dBSPower);

		sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:INTerference %.3f; *OPC?", dULInterference);
		m_pITesterDevice->GPIB_OPComplete(szTmp);

		sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:CVALue %.3f; *OPC?", dConstantValue);
		m_pITesterDevice->GPIB_OPComplete(szTmp);

		sprintf_s(szTmp, 255, "17;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer %.3f; *OPC?", dPCPICHPower);
		m_pITesterDevice->GPIB_OPComplete(szTmp);

		Sleep(m_iWaitTimeForFetch_OL);

		if (! FetchOff_OL(vOpenLoopList))
		{
			m_strMsg = "Fail to fetch WCDMA open loop power, reboot phone and try again";
			TraceLog(MSG_ERROR, m_strMsg) ;

			if (! m_pIPhone->Reboot())
			{
				m_strMsg = "Fail to reboot phone for WCDMA open loop power measurement";
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}

			Sleep(25000);

			bool bConnect = false;
			int iRetry = 20;
			while (iRetry > 0)
			{
				if (m_pIPhone->IsConnected())
				{
					bConnect = true;
					break;
				}

				--iRetry;
				Sleep(500);
			}

			if (! bConnect)
			{
				m_strMsg = "Fail to connect phone after reboot";
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}

			Sleep(m_iWaitTimeForFetch_OL);

			if (! FetchOff_OL(vOpenLoopList))
			{
				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OLPC,%d,%d,%d,,%.3f,0", m_iBand, m_iChannel, i, dMeasured);
				TraceLog(MSG_INFO, szTmp);

				m_strMsg = "Fail to fetch WCDMA open loop power";
			    TraceLog(MSG_ERROR, m_strMsg) ;

				if (i < (signed)m_vBSOutputPower_OL.size() - 1)
					FactoryLog(false, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

				continue;
			}
		}

		dMeasured = atof(vOpenLoopList[0].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dMeasured);
		m_strMeasured = szTmp;

		double dExpectedPower = atof(vOpenLoopList[4].c_str());
		double dLower = dExpectedPower + m_dLowerOL;
		double dUpper = dExpectedPower + m_dUpperOL;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dLower);
		m_strLower = szTmp;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dUpper);
		m_strUpper = szTmp;

		if (InRange(dLower, dUpper, dMeasured))
		{
            m_bTestResult = true ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OLPC,%d,%d,%d,,%.3f,1", m_iBand, m_iChannel, i, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			TraceLog(MSG_INFO, "WCDMA open loop Power PASS") ;
		}
		else
		{
            m_bTestResult = false ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, ",Detroit_Wireless3_MeasurementData,WCDMA,OLPC,%d,%d,%d,,%.3f,0", m_iBand, m_iChannel, i, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			m_strMsg = "WCDMA open loop Power FAIL";
			TraceLog(MSG_ERROR, m_strMsg) ;
		}

		if (i < (signed)m_vBSOutputPower_OL.size() - 1)
			FactoryLog(m_bTestResult, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}

	m_pITesterDevice->WCDMA_EndCall();
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:INTerference -80.0; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:CVALue -29.0; *OPC?") ;
	m_pITesterDevice->GPIB_OPComplete("17;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer 31.0; *OPC?") ;

	return true;
}


bool CWCDMA_Online_MeasureLoop_II::FetchOff_OL(std::vector<std::string>& vResult)
{
	vResult.clear();

	std::string strResponse;

	m_pITesterDevice->GPIB_OPComplete("17;INITiate:POWer:ONOFf; *OPC?") ;

	while (true)
	{
		m_pITesterDevice->GPIB_Querry("17;FETCh:POWer:ONOFf:STATus?", strResponse);

		if (strstr(strResponse.c_str(), "RUN") != NULL)
		{
			Sleep(500);
			continue;
		}
		else
			break;
	}

	m_pITesterDevice->GPIB_Querry("17;FETCh:SCALar:POWer:ONOFf:CURRent?", strResponse);
	STDStringTokenizer(strResponse, ",", vResult);
	if (strstr(vResult[0].c_str(), "NAN") != NULL)
	{
		m_strMsg = "Fail to measure WCDMA off power, NAN fetched, try MT call";
		TraceLog(MSG_ERROR, m_strMsg) ;

		m_pITesterDevice->WCDMA_EndCall();

		m_pITesterDevice->GPIB_OPComplete("17;INITiate:POWer:ONOFf; *OPC?") ;
		m_pITesterDevice->WCDMA_MTCall();

		while (true)
		{
			m_pITesterDevice->GPIB_Querry("17;FETCh:POWer:ONOFf:STATus?", strResponse);

			if (strstr(strResponse.c_str(), "RUN") != NULL)
			{
				Sleep(500);
				continue;
			}
			else
				break;
		}

		m_pITesterDevice->GPIB_Querry("17;FETCh:SCALar:POWer:ONOFf:CURRent?", strResponse);
		STDStringTokenizer(strResponse, ",", vResult);
		if (strstr(vResult[0].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure WCDMA off power, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::HandoverUMTS()
{
	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "UMTS%d", m_iBand);
	m_strBand = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", m_iChannel);
	m_strChannel = szTmp;

// 	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(-106.0))
// 	{
// 		m_strMsg = "Fail to set WCDMA BS output power to -106.0" ;
// 		TraceLog(MSG_ERROR, m_strMsg) ;
// 		return false;
// 	}
// 
// 	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(33.0))
// 	{
// 		m_strMsg = "Fail to set WCDMA maximum allow power" ;
// 		TraceLog(MSG_ERROR, m_strMsg) ;
// 		return false;
// 	}
// 
// 	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(-20.0))
// 	{
// 		m_strMsg = "Fail to set WCDMA MS target power to tester device" ;
// 		TraceLog(MSG_ERROR, m_strMsg) ;
// 		return false;
// 	}

	


	TraceLog(MSG_INFO, "Success to setup test device for WCDMA handover") ;


	int iDownlinkChannel = WCDMA_GetDLChannel(m_iBand, m_iChannel);
	if (iDownlinkChannel == 0)
	{
		m_strMsg = "Fail to get WCDMA downlink channel, check XML parameters" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetBandAndDownlinkChannel(m_iBand, iDownlinkChannel))
	{
		m_strMsg = "Fail to set band and channel for WCDMA handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	std::string strBandName;
	std::string strRange;

	if (WCDMA_GetBandNameForCableLoss(m_iBand, strBandName) && WCDMA_GetRangeForTxCableLoss(m_iBand, m_iChannel, strRange))
	{
		if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", "Wireless" + strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", "Wireless" +strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}
	else
	{
		m_strMsg = "Fail to get WCDMA band name & range for Tx cable loss" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	if (! m_pITesterDevice->WCDMA_QuerryCallStatus())
	{
		m_strMsg = "Fail to handover, starting make call procedure" ;
		TraceLog(MSG_ERROR, m_strMsg) ;

		if (!WCDMA_StartCall())
		{
			m_strMsg = "Fail to WCDMA MT call, try again" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::InitData_CPK()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// 11. CPK
	if (m_paramMap.find("BSOutputPower_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter BSOutputPower_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dBSOutputPower_RefBER = atof(m_paramMap["BSOutputPower_RefBER"].c_str());

	if (m_paramMap.find("MSTargetPower_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter MSTargetPower_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dMSTargetPower_RefBER = atof(m_paramMap["MSTargetPower_RefBER"].c_str());

	if (m_paramMap.find("Range_RefBER") == m_paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	strTmp = m_paramMap["Range_RefBER"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range_RefBER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dLowerRefBER = atof(vTmp[0].c_str());
	m_dUpperRefBER = atof(vTmp[1].c_str());

	// Retry times
	if (m_paramMap.find("RetryTimes") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <RetryTimes> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	m_iRetryTimes = atoi(m_paramMap["RetryTimes"].c_str()) ;

	// PowerLevel_step
	if (m_paramMap.find("PowerLevel_Step") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_Step> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_Step"];
	m_fPowerLevelStep = atof(strTmp.c_str()) ;

	return true;
}

bool CWCDMA_Online_MeasureLoop_II::CPK()
{
	m_strItem = "CPK";
	m_strMeasured = "-";
	m_strUnit = "%";

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dLowerMinBER);
	m_strLower = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", m_dUpperMinBER);
	m_strUpper = szTmp;

	double dBSPower = m_dBSOutputPower_RefBER;

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set maximum MS power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower_RefBER))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_InitBERMMeasurement())
	{
		m_strMsg = "Fail to initial tester device for WCDMA BER";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	bool bStatus = false;
	while (dBSPower <= m_dBSOutputPower_RefBER)
	{
		if (! m_pITesterDevice->WCDMA_SetBSOutputPower(dBSPower))
		{
			m_strMsg = "Fail to set BS output power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(1000);

		std::string strMeasured;
		if (! m_pITesterDevice->WCDMA_FetchBERMMeasurement(strMeasured))
		{
			m_strMsg = "Fail to fetch WCDMA BER value from tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		std::vector<std::string> vTmp;
		STDStringTokenizer(strMeasured, ",", vTmp);

		if (strstr(vTmp[0].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure BER value, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		double dMeasured = atof(vTmp[0].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "BS output power = %.3f, BER = %.3f", dBSPower, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		if (InRange(m_dLowerMinBER, m_dUpperMinBER, dMeasured))
		{
			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%.3f", dBSPower);
			m_strMeasured = szTmp;
			dBSPower -= m_fPowerLevelStep;
			bStatus  = true;			
		}
		else
		{
			if (!bStatus)
			{
				::ZeroMemory(szTmp, 256) ;
				sprintf_s(szTmp, 256, "%4.4f", dMeasured) ;
				m_strMeasured = szTmp ;
			}
			else
			{
				dBSPower += m_fPowerLevelStep;
			}

			break;		
		}
	}

	if (bStatus)
	{
		bStatus  = false;
		if (! m_pITesterDevice->WCDMA_SetBSOutputPower(dBSPower))
		{
			m_strMsg = "Fail to set BS output power to tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(1500);
		std::string strRSCPMeasured;
		if (! m_pITesterDevice->WCDMA_FetchRSCPMeasurement(strRSCPMeasured))
		{
			m_strMsg = "Fail to fetch WCDMA BER value from tester device";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		else
		{
			std::vector<std::string> vTmp;
			STDStringTokenizer(strRSCPMeasured, ",", vTmp);

			if (strstr(vTmp[0].c_str(), "NAN") != NULL)
			{
				m_strMsg = "Fail to measure RSCP value, NAN fetched";
				TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}
			else
			{
				bStatus = true;
				double dMeasured1;
				dMeasured1 = atof(vTmp[1].c_str());
				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, "%.3f",dMeasured1);
				FactoryLog(true, "RSCP", "Pass", m_strBand, m_strChannel, "--", "--", szTmp, m_strUnit, "Get RSCP pass!" ) ;

			}		
		}
	}
	
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "Band= %d, Channel= %d, Power= %.3f, BER = %s", m_iBand, m_iChannel, dBSPower, m_strMeasured);
	TraceLog(MSG_INFO, szTmp);

	return bStatus;
}
