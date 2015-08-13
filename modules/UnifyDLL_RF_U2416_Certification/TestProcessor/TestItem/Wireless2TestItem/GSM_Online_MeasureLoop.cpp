#include "stdafx.h"
#include "GSM_Online_MeasureLoop.h"


const int iPCL_Delay = 500 ;


RF_IMPLEMENT_DYNCREATE(CGSM_Online_MeasureLoop)

CGSM_Online_MeasureLoop::CGSM_Online_MeasureLoop()
{
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_TxPower) ;
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_PowerVsTimeMask);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_PhaseErrorRMS);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_PhaseErrorPeak);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_FrequencyError);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_Modulation);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_Switching);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_RxLevel);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_RFER);
    m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_Sensitivity);
	m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_BER);
	m_vInitDataFunction.push_back(&CGSM_Online_MeasureLoop::InitData_CPK);
    

    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::TxPower);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::PowerVsTimeMask);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::PhaseErrorRMS);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::PhaseErrorPeak);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::FrequencyError);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::Modulation);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::Switching);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::RxLevel);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::RFER);
    m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::Sensitivity);
	m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::BER);
	m_vTestFunction.push_back(&CGSM_Online_MeasureLoop::CPK);
    
}


bool CGSM_Online_MeasureLoop::Run()
{
    TraceLog(MSG_INFO, "Start executing Run() of CGSM_MeasureLoop");


    // Query Band and Channel
    m_iBand = m_pITesterDevice->GSM_QueryBand() ;
    m_iChannel = m_pITesterDevice->GSM_QueryChannel() ;
    if ( 0>=m_iBand || 0>=m_iChannel ) 
	{
        m_strMsg = "Fail to Query Band and Channel" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        FactoryLog(false, m_strMsg) ;

        return false ;
    }

	m_strBand ="GSM" +  CStr::IntToStr(m_iBand);
	for(int j = 0;j< (signed)m_vChannelList.size(); ++j)
	{
		m_iChannel = atoi(m_vChannelList[j].c_str());
		
		m_strChannel = CStr::IntToStr(m_iChannel);


		if (!HandOverChannel(m_iChannel))
		{
			return false ;
		}

		for (int i = 0; i < (signed)m_vTestList.size(); ++i)
		{
			if ( !QueryTestItemIsEnable(m_vTestList, i) )
				continue;

			// Reset UI
			m_strItem = "-" ;
			m_strErrCode.clear() ;
			m_strLower = "-";
			m_strUpper = "-";
			m_strMeasured = "-";
			m_strUnit = "-";
			m_strMsg = "-";


			// Run Measure Function
			bool bRet = CALL_MEMBER_FN(*this, m_vTestFunction[i])() ;

			if ( bRet ) 
			{
				FactoryLog(bRet, m_strItem, "Pass", m_strChannel, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
			} 
			else 
			{
				if ( m_strErrCode.empty() )
					m_strErrCode = FunErr_WirelessGSM_Test_Set_Command_Fail ;


				FactoryLog(bRet, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;

				return false ;
			}
		}
	}
	

    return true ;
}


bool CGSM_Online_MeasureLoop::InitData(std::map<std::string, std::string>& paramMap)
{
    // Copy Data
    m_paramMap = paramMap ;

	if (m_paramMap.find("ChannelList") == m_paramMap.end()) 
	{
		TraceLog(MSG_ERROR, "Fail to Get <ChannelList> from XML") ;
		return false ; 
	}
	m_strTestList = m_paramMap["ChannelList"];
	STDStringTokenizer(m_strTestList, ",", m_vChannelList);
	

    if (m_paramMap.find("TestList") == m_paramMap.end()) 
	{
        TraceLog(MSG_ERROR, "Fail to Get <TestList> from XML") ;
        return false ; 
	}
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



//////////////////////////////////////////////////////////////////////////
//   InitData Functions
//////////////////////////////////////////////////////////////////////////

bool CGSM_Online_MeasureLoop::InitData_TxPower()
{
	m_iSleepTime = 1000;
    std::string strTmp;
    std::vector<std::string> vTmp;

	if (m_paramMap.find("SleepTime") == m_paramMap.end()) {
		m_strMsg = "Fail to Get <SleepTime> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	m_iSleepTime = atoi(m_paramMap["SleepTime"].c_str());

    if (m_paramMap.find("PCL_TxPower") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_TxPower> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_TxPower"];
    STDStringTokenizer(strTmp, ",", m_vPCLTxPower);


    if (m_paramMap.find("Lower_TxPower") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Lower_TxPower> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Lower_TxPower"];
    STDStringTokenizer(strTmp, ",", m_vLowerTxPower);
    if ( m_vLowerTxPower.size() != m_vPCLTxPower.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_TxPower Setting in XML") ;
        return false ;
    }


    if (m_paramMap.find("Upper_TxPower") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_TxPower> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_TxPower"];
    STDStringTokenizer(strTmp, ",", m_vUpperTxPower);
    if ( m_vUpperTxPower.size() != m_vPCLTxPower.size() ) {
        TraceLog(MSG_INFO, "Wrong Upper_TxPower Setting in XML") ;
        return false ;
    }

    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_PowerVsTimeMask()
{
    std::string strTmp;
    std::vector<std::string> vTmp;

    if (m_paramMap.find("PCL_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_PVT"];
    STDStringTokenizer(strTmp, ",", m_vPCLPVT);


    if (m_paramMap.find("UpperA_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <UpperA_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["UpperA_PVT"];
    STDStringTokenizer(strTmp, ",", m_vUpperAPVT);
    if ( m_vUpperAPVT.size() != m_vPCLPVT.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_TxPower Setting in XML") ;
        return false ;
    }


    if (m_paramMap.find("UpperB_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <UpperB_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["UpperB_PVT"];
    STDStringTokenizer(strTmp, ",", m_vUpperBPVT);
    if ( m_vUpperBPVT.size() != m_vPCLPVT.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_TxPower Setting in XML") ;
        return false ;
    }


    if (m_paramMap.find("UpperC_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <UpperC_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["UpperC_PVT"];
    STDStringTokenizer(strTmp, ",", m_vUpperCPVT);
    if ( m_vUpperCPVT.size() != m_vPCLPVT.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_TxPower Setting in XML") ;
        return false ;
    }


    if (m_paramMap.find("Burst_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_PVT"];
    m_iBurstPVT = atoi(strTmp.c_str()) ;


    if (m_paramMap.find("PowerLevel_PVT") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_PVT> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_PVT"];
    m_fPowerLevelPVT = (float)atof(strTmp.c_str()) ;


    return true ;
}


bool CGSM_Online_MeasureLoop::InitData_PhaseErrorRMS()
{
    std::string strTmp;
    std::vector<std::string> vTmp;

    // PCL
    if (m_paramMap.find("PCL_PhErrRMS") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_PhErrRMS> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_PhErrRMS"];
    STDStringTokenizer(strTmp, ",", m_vPCLPhErrRMS);

    // Lower
    if (m_paramMap.find("Lower_PhErrRMS") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Lower_PhErrRMS> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Lower_PhErrRMS"];
    STDStringTokenizer(strTmp, ",", m_vLowerPhErrRMS);
    if ( m_vLowerPhErrRMS.size() != m_vPCLPhErrRMS.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_PhErrRMS Setting in XML") ;
        return false ;
    }

    // Upper
    if (m_paramMap.find("Upper_PhErrRMS") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_PhErrRMS> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_PhErrRMS"];
    STDStringTokenizer(strTmp, ",", m_vUpperPhErrRMS);
    if ( m_vUpperPhErrRMS.size() != m_vPCLPhErrRMS.size() ) {
        TraceLog(MSG_INFO, "Wrong Upper_PhErrRMS Setting in XML") ;
        return false ;
    }

    // Burst
    if (m_paramMap.find("Burst_PhErrRMS") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_PhErrRMS> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_PhErrRMS"];
    m_iBurstPhErrRMS = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_PhErrRMS") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_PhErrRMS> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_PhErrRMS"];
    m_fPowerLevelPhErrRMS = (float)atof(strTmp.c_str()) ; 

    return true ;
}


bool CGSM_Online_MeasureLoop::InitData_PhaseErrorPeak()
{
    std::string strTmp;
    std::vector<std::string> vTmp;
    
    // PCL
    if (m_paramMap.find("PCL_PhErrPeak") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_PhErrPeak> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_PhErrPeak"];
    STDStringTokenizer(strTmp, ",", m_vPCLPhErrPeak);

    // Lower
    if (m_paramMap.find("Lower_PhErrPeak") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Lower_PhErrPeak> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Lower_PhErrPeak"];
    STDStringTokenizer(strTmp, ",", m_vLowerPhErrPeak);
    if ( m_vLowerPhErrPeak.size() != m_vPCLPhErrPeak.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_PhErrPeak Setting in XML") ;
        return false ;
    }

    // Upper
    if (m_paramMap.find("Upper_PhErrPeak") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_PhErrPeak> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_PhErrPeak"];
    STDStringTokenizer(strTmp, ",", m_vUpperPhErrPeak);
    if ( m_vUpperPhErrPeak.size() != m_vPCLPhErrPeak.size() ) {
        TraceLog(MSG_INFO, "Wrong Upper_PhErrPeak Setting in XML") ;
        return false ;
    }

    // Burst
    if (m_paramMap.find("Burst_PhErrPeak") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_PhErrPeak> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_PhErrPeak"];
    m_iBurstPhErrPeak = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_PhErrPeak") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_PhErrPeak> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_PhErrPeak"];
    m_fPowerLevelPhErrPeak = (float)atof(strTmp.c_str()) ;

    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_FrequencyError()
{
    std::string strTmp;
    std::vector<std::string> vTmp;

    // PCL
    if (m_paramMap.find("PCL_FreqError") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_FreqError> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_FreqError"];
    STDStringTokenizer(strTmp, ",", m_vPCLFreqErr);

    // Lower
    if (m_paramMap.find("Lower_FreqError") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Lower_FreqError> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Lower_FreqError"];
    STDStringTokenizer(strTmp, ",", m_vLowerFreqErr);
    if ( m_vLowerFreqErr.size() != m_vPCLFreqErr.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_FreqError Setting in XML") ;
        return false ;
    }

    // Upper
    if (m_paramMap.find("Upper_FreqError") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_FreqError> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_FreqError"];
    STDStringTokenizer(strTmp, ",", m_vUpperFreqErr);
    if ( m_vUpperFreqErr.size() != m_vPCLFreqErr.size() ) {
        TraceLog(MSG_INFO, "Wrong Upper_FreqError Setting in XML") ;
        return false ;
    }

    // Burst
    if (m_paramMap.find("Burst_FreqError") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_FreqError> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_FreqError"];
    m_iBurstFreqErr = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_FreqError") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_FreqError> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_FreqError"];
    m_fPowerLevelFreqErr = (float)atof(strTmp.c_str()) ; 

    return true ;
}


bool CGSM_Online_MeasureLoop::InitData_Modulation()
{
    std::string strTmp;
    std::vector<std::string> vTmp;
    
    // PCL
    if (m_paramMap.find("PCL_Mod") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_Mod> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_Mod"];
    STDStringTokenizer(strTmp, ",", m_vPCLMod);

    // Offset
    if (m_paramMap.find("Offset_Mod") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Offset_Mod> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Offset_Mod"];
    STDStringTokenizer(strTmp, ",", m_vOffsetMod) ;

    // Upper
    if (m_paramMap.find("Upper_Mod") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_Mod> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_Mod"];
    STDStringTokenizer(strTmp, ",", m_vUpperMod) ;
    if ( m_vUpperMod.size() != (m_vPCLMod.size()*m_vOffsetMod.size()) ) {
        TraceLog(MSG_INFO, "Wrong Upper_Mod Setting in XML") ;
        return false ;
    }

    // Burst
    if (m_paramMap.find("Burst_Mod") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_Mod> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_Mod"];
    m_iBurstMod = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_Mod") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_Mod> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_Mod"];
    m_fPowerLevelMod = (float)atof(strTmp.c_str()) ;


    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_Switching()
{
    std::string strTmp;
    std::vector<std::string> vTmp;
    
    // PCL
    if (m_paramMap.find("PCL_Switch") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_Switch> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_Switch"];
    STDStringTokenizer(strTmp, ",", m_vPCLSwitch);

    // Offset
    if (m_paramMap.find("Offset_Switch") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Offset_Switch> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Offset_Switch"];
    STDStringTokenizer(strTmp, ",", m_vOffsetSwitch) ;

    // Upper
    if (m_paramMap.find("Upper_Switch") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_Switch> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_Switch"];
    STDStringTokenizer(strTmp, ",", m_vUpperSwitch) ;
    if ( m_vUpperSwitch.size() != (m_vPCLSwitch.size()*m_vOffsetSwitch.size()) ) {
        TraceLog(MSG_INFO, "Wrong Upper_Switch Setting in XML") ;
        return false ;
    }

    // Burst
    if (m_paramMap.find("Burst_Switch") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_Switch> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_Switch"];
    m_iBurstSwitch = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_Switch") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_Switch> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_Switch"];
    m_fPowerLevelSwitch = (float)atof(strTmp.c_str()) ;

    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_RxLevel()
{
    std::string strTmp;
    std::vector<std::string> vTmp;
    
    // PowerLevel
    if (m_paramMap.find("Level_RxLevel") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Level_RxLevel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Level_RxLevel"];
    STDStringTokenizer(strTmp, ",", m_vLevelRxLv);

    // Upper
    if (m_paramMap.find("Upper_RxLevel") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Upper_RxLevel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Upper_RxLevel"];
    STDStringTokenizer(strTmp, ",", m_vUpperRxLv);
    if ( m_vUpperRxLv.size() != m_vLevelRxLv.size() ) {
        TraceLog(MSG_INFO, "Wrong Upper_RxLevel Setting in XML") ;
        return false ;
    }

    // Lower
    if (m_paramMap.find("Lower_RxLevel") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Lower_RxLevel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Lower_RxLevel"];
    STDStringTokenizer(strTmp, ",", m_vLowerRxLv);
    if ( m_vLowerRxLv.size() != m_vLevelRxLv.size() ) {
        TraceLog(MSG_INFO, "Wrong Lower_RxLevel Setting in XML") ;
        return false ;
    }

    // PCL
    if (m_paramMap.find("PCL_RxLevel") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_RxLevel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_RxLevel"];
    m_iPCLRxLv = atoi(strTmp.c_str()) ;

    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_RFER()
{
    std::string strTmp;
    std::vector<std::string> vTmp;
    
    // Range_RBER
    if (m_paramMap.find("Range_RBER") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Range_RBER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Range_RBER"];
    STDStringTokenizer(strTmp, ",", vTmp);
    if (vTmp.size() < 2)
        return false;
    m_dLowerRBER = atof(vTmp[0].c_str());
    m_dUpperRBER = atof(vTmp[1].c_str());

    // Range_FER
    if (m_paramMap.find("Range_FER") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Range_FER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Range_FER"];
    STDStringTokenizer(strTmp, ",", vTmp);
    if (vTmp.size() < 2)
        return false;
    m_dLowerFER = atof(vTmp[0].c_str());
    m_dUpperFER = atof(vTmp[1].c_str());

    // Burst
    if (m_paramMap.find("Burst_RBER_FER") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_RBER_FER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_RBER_FER"];
    m_iBurstRFER = atoi(strTmp.c_str()) ;

    // PCL
    if (m_paramMap.find("PCL_RBER_FER") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_RBER_FER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_RBER_FER"];
    m_iPCLRFER = atoi(strTmp.c_str()) ;

    // PowerLevel
    if (m_paramMap.find("PowerLevel_RBER_FER") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel_RBER_FER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_RBER_FER"];
    m_fPowerLevelRFER = (float)atof(strTmp.c_str()) ;


    return true ;
}

bool CGSM_Online_MeasureLoop::InitData_Sensitivity()
{
    std::string strTmp;
    std::vector<std::string> vTmp;

    // PCL
    if (m_paramMap.find("PCL_Sensitivity") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <PCL_Sensitivity> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_Sensitivity"];
    m_iPCLSensitivity = (int)(float)atof(strTmp.c_str()) ;

    // Range
    if (m_paramMap.find("Range_Sensitivity") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Range_Sensitivity> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Range_Sensitivity"];
    STDStringTokenizer(strTmp, ",", vTmp);
    if (vTmp.size() < 2)
        return false;
    m_dLowerSensitivity = atof(vTmp[0].c_str());
    m_dUpperSensitivity = atof(vTmp[1].c_str());

    // StartSearch
    if (m_paramMap.find("StartSearch_Sensitivity") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <StartSearch_Sensitivity> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["StartSearch_Sensitivity"];
    m_fPowerLevelSensitivity_Start = (float)atof(strTmp.c_str()) ;

    // Burst
    if (m_paramMap.find("Burst_Sensitivity") == m_paramMap.end()) {
        m_strMsg = "Fail to Get <Burst_Sensitivity> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_Sensitivity"];
    m_iBurstSensitivity = atoi(strTmp.c_str()) ;


    return true ;
}


bool CGSM_Online_MeasureLoop::InitData_BER()
{
    std::string strTmp;
    std::vector<std::string> vTmp;

    // Range
    if (m_paramMap.find("Range_BER") == m_paramMap.end()) 
	{
        m_strMsg = "Fail to Get <Range_BER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
	strTmp = m_paramMap["Range_BER"];
    STDStringTokenizer(strTmp, ",", vTmp);
    if (vTmp.size() < 2)
        return false;
    m_dLowerBER = atof(vTmp[0].c_str());
    m_dUpperBER = atof(vTmp[1].c_str());

    // Burst
    if (m_paramMap.find("Burst_BER") == m_paramMap.end()) 
	{
        m_strMsg = "Fail to Get <Burst_BER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["Burst_BER"];
    m_iBurstBER = atoi(strTmp.c_str()) ;

    // PCL
    if (m_paramMap.find("PCL_BER") == m_paramMap.end()) 
	{
        m_strMsg = "Fail to Get <PCL_BER> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PCL_BER"];
    m_iPCLBER = atoi(strTmp.c_str()) ;

	// Retry times
	if (m_paramMap.find("RetryTimes") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <RetryTimes> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["RetryTimes"];
	m_iRetryTimes = atoi(strTmp.c_str()) ;

    // PowerLevel_low
    if (m_paramMap.find("PowerLevel_BER_Low") == m_paramMap.end()) 
	{
        m_strMsg = "Fail to Get <PowerLevel_BER_Low> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }
    strTmp = m_paramMap["PowerLevel_BER_Low"];
    m_fPowerLevelBER_Low = (float)atof(strTmp.c_str()) ;

	// PowerLevel_mid
	if (m_paramMap.find("PowerLevel_BER_Mid") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_BER_Mid> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_BER_Mid"];
	m_fPowerLevelBER_Mid = (float)atof(strTmp.c_str()) ;

	// PowerLevel_high
	if (m_paramMap.find("PowerLevel_BER_High") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_BER_High> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_BER_High"];
	m_fPowerLevelBER_High = (float)atof(strTmp.c_str()) ;
    
	return true;
}

bool CGSM_Online_MeasureLoop::InitData_CPK()
{
	std::string strTmp;
	std::vector<std::string> vTmp;

	// Range
	if (m_paramMap.find("Range_BER") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <Range_BER> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["Range_BER"];
	STDStringTokenizer(strTmp, ",", vTmp);
	if (vTmp.size() < 2)
		return false;
	m_dLowerBER = atof(vTmp[0].c_str());
	m_dUpperBER = atof(vTmp[1].c_str());

	// Burst
	if (m_paramMap.find("Burst_BER") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <Burst_BER> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["Burst_BER"];
	m_iBurstBER = atoi(strTmp.c_str()) ;

	// PCL
	if (m_paramMap.find("PCL_BER") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PCL_BER> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PCL_BER"];
	m_iPCLBER = atoi(strTmp.c_str()) ;

	// Retry times
	if (m_paramMap.find("RetryTimes") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <RetryTimes> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["RetryTimes"];
	m_iRetryTimes = atoi(strTmp.c_str()) ;

	// PowerLevel_low
	if (m_paramMap.find("PowerLevel_BER_Low") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_BER_Low> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_BER_Low"];
	m_fPowerLevelBER_Low = (float)atof(strTmp.c_str()) ;

	// PowerLevel_mid
	if (m_paramMap.find("PowerLevel_BER_Mid") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_BER_Mid> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_BER_Mid"];
	m_fPowerLevelBER_Mid = (float)atof(strTmp.c_str()) ;

	// PowerLevel_high
	if (m_paramMap.find("PowerLevel_BER_High") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_BER_High> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_BER_High"];
	m_fPowerLevelBER_High = (float)atof(strTmp.c_str()) ;

	// PowerLevel_step
	if (m_paramMap.find("PowerLevel_Step") == m_paramMap.end()) 
	{
		m_strMsg = "Fail to Get <PowerLevel_Step> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["PowerLevel_Step"];
	m_fPowerLevelStep = (float)atof(strTmp.c_str()) ;


	return true;
}

//////////////////////////////////////////////////////////////////////////
//   Measure Functions
//////////////////////////////////////////////////////////////////////////

bool CGSM_Online_MeasureLoop::TxPower()
{
    const int iKey_AvgBurstPwAvg = 6 ;
    int iPCLAmount = m_vPCLTxPower.size() ;
    char szTmp[MAX_PATH];
	m_bItemResult = true ;

	// Set Error Code
	m_strErrCode = FunErr_WirelessGSM_Test_OutPower_OutRange ;
	m_strItem = "GSM TxPower" ;
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";

    for (int iCounter=0; iCounter<iPCLAmount; ++iCounter)
    {
        int iPCL = atoi( m_vPCLTxPower[iCounter].c_str() ) ;
        double fLower = atof( m_vLowerTxPower[iCounter].c_str() ) ;
        double fUpper = atof( m_vUpperTxPower[iCounter].c_str() ) ;

        // Determine UI
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "GSM TxPower", iPCL);
        m_strItem = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fLower);
        m_strLower = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fUpper);
        m_strUpper = szTmp;
        m_strMeasured = "-";
        m_strUnit = "dBm";
        
        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) )
        {
            m_strMsg = "Fail to Set PCL in TxPower Test" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }

		Sleep(m_iSleepTime);

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchGMSKPower(strMeasured, 10) )
        {
            m_strMsg = "Fail to Fetch GSM Max Power" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }

        std::vector<std::string> vMaxPowerList;
        if (! STDStringTokenizer(strMeasured, ",", vMaxPowerList))
        { 
            m_strMsg = "Fail to execute function STDStringTokenizer for GSM Max Power Data" ;
            TraceLog(MSG_ERROR, m_strMsg);
            return false;
        }

        double dMeasured = atof(vMaxPowerList[iKey_AvgBurstPwAvg].c_str());
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
        m_strMeasured = szTmp ;

		bool bResult = InRange(fLower, fUpper, dMeasured) ;
		if ( bResult ) 
		{
			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData,GSM,TxPower,%d,%d,PCL_%d,,%.3f,1", m_iBand, m_iChannel, iPCL, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, "GSM TxPower (PCL:%d) PASS", iPCL);
			m_strErrCode = "Pass";
			m_strMsg = szTmp ;
			TraceLog(MSG_INFO, szTmp);
		} 
		else 
		{
			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData,GSM,TxPower,%d,%d,PCL_%d,,%.3f,0", m_iBand, m_iChannel, iPCL, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, "GSM TxPower (PCL:%d) FAIL", iPCL);
			m_strMsg = szTmp ;
			TraceLog(MSG_INFO, szTmp);
		}

		FactoryLog(bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;

		m_bItemResult = m_bItemResult&&bResult ;
    }

   
    
	if (m_bItemResult)
	{
		m_strErrCode.clear() ;
		m_strMsg = m_strItem + " Loop Finish Successfully";
	}
	else
	{
		m_strMsg = m_strItem + " Loop Finish with Error" ;
	}

    return m_bItemResult;
}


bool CGSM_Online_MeasureLoop::PowerVsTimeMask()
{
    const float dPowerLower = -99 ;
    const int   iUpperAreaAmount = 6 ;
    const int   iUpperAreaDefine[iUpperAreaAmount] = {1,2,3,6,7,8} ;
    int iPCLAmount = m_vPCLPVT.size() ;
    char szTmp[MAX_PATH] ;
    std::string strMeasured ;

	m_strItem = "GSM PVT" ;
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM Power vs Time Mask Loop Finish";

    // Change Power Level
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelPVT) )
    {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During PVT Measured") ;
        return false ;
    }

    for (int iCounter=0; iCounter<iPCLAmount; ++iCounter)
    {
        int iPCL = atoi( m_vPCLPVT[iCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) {
            TraceLog(MSG_ERROR, "Fail to Set PCL in TxPower Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        // Determine UI
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "GSM PVT PCL%02d", iPCL);
        m_strItem = szTmp ;
        m_strLower = "-" ;
        m_strMeasured = "-" ;
        m_strUnit = "dBc" ;

        if (! m_pITesterDevice->GSM_FetchPowerVsTimeMask(strMeasured, m_iBurstPVT) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch Power VS Time Mask Array") ;
            return false ;
        }

        std::vector<std::string> vPowerVsTimeMask ;
        if (! STDStringTokenizer(strMeasured, ",", vPowerVsTimeMask))
        {
            TraceLog(MSG_ERROR, "Fail to execute function STDStringTokenizer for GSM Power VS Time Mask Data");
            return false;
        }

        // For each area.
        for (int iAreaCounter=0; iAreaCounter<iUpperAreaAmount; ++iAreaCounter)
        {
            int iArea = iUpperAreaDefine[iAreaCounter] ;
            double fUpper ;

            if ( 3==iArea||6==iArea ){
                fUpper = atof( m_vUpperAPVT[iCounter].c_str() ) ;
            } else if ( 2==iArea||7==iArea ) {
                fUpper = atof( m_vUpperBPVT[iCounter].c_str() ) ;
            } else if ( 1==iArea||8==iArea ) {
                fUpper = atof( m_vUpperCPVT[iCounter].c_str() ) ;
            }

            ::ZeroMemory(szTmp, MAX_PATH) ;
            sprintf_s(szTmp, MAX_PATH, "%4.3f", fUpper);
            m_strUpper = szTmp;

            bool ret ;
            strMeasured = "" ;
            ret = m_pITesterDevice->GSM_PowerVsTimeMaskPaser(vPowerVsTimeMask, iArea, fUpper, strMeasured) ;

            if ( (!ret)&&(""==strMeasured) ) {
                TraceLog(MSG_ERROR, "Fail to Query PowerVsTimeMask Line Limit") ;
                return false ;
            }

            m_strMeasured = strMeasured ;

            ::ZeroMemory(szTmp, MAX_PATH) ;
            if (ret) {
                m_bResult = true ;

                sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PVT,%d,%d,PCL_%d,Area_%d,%s,1", m_iBand, m_iChannel, iPCL, iArea, strMeasured.c_str());
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM PVT (PCL:%d ,Area:%d, avg.:%s) PASS", iPCL, iArea, strMeasured.c_str()) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            } else {
                m_bResult = false ;

                sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PVT,%d,%d,PCL_%d,Area_%d,%s,0", m_iBand, m_iChannel, iPCL, iArea, strMeasured.c_str());
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM PVT (PCL:%d, Area:%d, out of range value:%s) FAIL", iPCL, iArea, strMeasured.c_str()) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            }
			
			if (m_bResult)
			{
				FactoryLog(m_bResult, m_strItem, "Pass", m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
			}
			else
			{
				FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
			}		
            
        }
    }

  

    return true ;
}


bool CGSM_Online_MeasureLoop::PhaseErrorRMS()
{
    const int iKey_PhErrRMSAvg = 4 ;
    int iPhErrRMSAmount = m_vPCLPhErrRMS.size() ;
    char szTmp[MAX_PATH];

	m_strItem = "GSM PhaseErrorRMS" ;
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM PhErrRMS Loop Finish";

    // Change Power Level
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelPhErrRMS) ) {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Phase Error RMS Measured") ;
        return false ;
    }

    for (int iCounter=0; iCounter<iPhErrRMSAmount; ++iCounter)
    {
        int iPCL = atoi( m_vPCLPhErrRMS[iCounter].c_str() ) ;
        double fLower = atof( m_vLowerPhErrRMS[iCounter].c_str() ) ;
        double fUpper = atof( m_vUpperPhErrRMS[iCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) {
            TraceLog(MSG_ERROR, "Fail to Set PCL in Phase Error RMS Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        // Determine UI
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorRMS PCL%02d", iPCL);
        m_strItem = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fLower);
        m_strLower = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fUpper);
        m_strUpper = szTmp;
        m_strMeasured = "-";
        m_strUnit = "Deg" ;

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchGMSKPhaseError(strMeasured, m_iBurstPhErrRMS) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch Power") ;
            return false ;
        }

        std::vector<std::string> vPhErrRMSList;
        if (! STDStringTokenizer(strMeasured, ",", vPhErrRMSList))
        {
            TraceLog(MSG_ERROR, "Fail to execute function STDStringTokenizer for GSM Phase Error RMS");
            return false;
        }

        double dMeasured = atof(vPhErrRMSList[iKey_PhErrRMSAvg].c_str());

        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
        m_strMeasured = szTmp ;

        if ( InRange(fLower, fUpper, dMeasured) ) {
            m_bResult = true ; 

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PhaseErrorRMS,%d,%d,PCL_%d,,%4.3f,1", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorRMS (PCL:%d) PASS", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        } else {
            m_bResult = false ;

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PhaseErrorRMS,%d,%d,PCL_%d,,%4.3f,0", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorRMS (PCL:%d) FAIL", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        }

        FactoryLog(m_bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
    }

   

    return true;
}


bool CGSM_Online_MeasureLoop::PhaseErrorPeak()
{
    const int iKey_PhErrPeakAvg = 1 ;
    int iPhErrPeakAmount = m_vPCLPhErrPeak.size() ;
    char szTmp[MAX_PATH];

	m_strItem = "GSM PhaseErrorPeak" ;
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM PhaseErrorPeak Loop Finish";
    // Change Power Level
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelPhErrPeak) ) {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Phase Error Peak Measured") ;
        return false ;
    }

    for (int iCounter=0; iCounter<iPhErrPeakAmount; ++iCounter)
    {
        int iPCL = atoi( m_vPCLPhErrPeak[iCounter].c_str() ) ;
        double fLower = atof( m_vLowerPhErrPeak[iCounter].c_str() ) ;
        double fUpper = atof( m_vUpperPhErrPeak[iCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) {
            TraceLog(MSG_ERROR, "Fail to Set PCL in Phase Error Peak Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        // Determine UI
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorPeak PCL%02d", iPCL);
        m_strItem = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fLower);
        m_strLower = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fUpper);
        m_strUpper = szTmp;
        m_strMeasured = "-";
        m_strUnit = "Deg" ;

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchGMSKPhaseError(strMeasured, m_iBurstPhErrPeak) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch Power for PhaseErrorPeak") ;
            return false ;
        }

        std::vector<std::string> vPhErrPeakList;
        if (! STDStringTokenizer(strMeasured, ",", vPhErrPeakList))
        {
            TraceLog(MSG_ERROR, "Fail to execute function STDStringTokenizer for GSM Phase Error RMS");
            return false;
        }

        double dMeasured = atof(vPhErrPeakList[iKey_PhErrPeakAvg].c_str());

        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
        m_strMeasured = szTmp ;

        if ( InRange(fLower, fUpper, dMeasured) ) {
            m_bResult = true ;

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PhaseErrorPeak,%d,%d,PCL_%d,,%4.3f,1", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorPeak (PCL:%d) PASS", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        } else {
            m_bResult = false ;

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,PhaseErrorPeak,%d,%d,PCL_%d,,%4.3f,0", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM PhaseErrorPeak (PCL:%d) FAIL", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        }

        FactoryLog(m_bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
    }

   

    return true;
}


bool CGSM_Online_MeasureLoop::FrequencyError()
{
    const int iKey_FreqErrAvg = 7 ;
    int iFreqErrAmount = m_vPCLFreqErr.size() ;
    char szTmp[MAX_PATH];

	m_strItem = "GSM FrequencyError" ;
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM FrequencyError Loop Finish";

    // Change Power Level
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelFreqErr) ) {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Frequency Error Measured") ;
        return false ;
    }

    for (int iCounter=0; iCounter<iFreqErrAmount; ++iCounter)
    {
        int iPCL = atoi( m_vPCLFreqErr[iCounter].c_str() ) ;
        double fLower = atof( m_vLowerFreqErr[iCounter].c_str() ) ;
        double fUpper = atof( m_vUpperFreqErr[iCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) {
            TraceLog(MSG_ERROR, "Fail to Set PCL in Frequency Error Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        // Determine UI
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "GSM FrequencyError PCL%02d", iPCL);
        m_strItem = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fLower);
        m_strLower = szTmp;
        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", fUpper);
        m_strUpper = szTmp;
        m_strMeasured = "-";
        m_strUnit = "Hz" ;

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchGMSKPhaseError(strMeasured, m_iBurstFreqErr) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch Power") ;
            return false ;
        }

        std::vector<std::string> vMaxFreqErrorList;
        if (! STDStringTokenizer(strMeasured, ",", vMaxFreqErrorList))
        {
            TraceLog(MSG_ERROR, "Fail to execute function STDStringTokenizer for GSM Frequency Error");
            return false;
        }

        double dMeasured = atof(vMaxFreqErrorList[iKey_FreqErrAvg].c_str());

        ::ZeroMemory(szTmp, MAX_PATH) ;
        sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
        m_strMeasured = szTmp ;

        if ( InRange(fLower, fUpper, dMeasured) ) {
            m_bResult = true ;
            
            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,FrequrncyError,%d,%d,PCL_%d,,%4.3f,1", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM FrequencyError (PCL:%d) PASS", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        } else {
            m_bResult = false ;

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,FrequrncyError,%d,%d,PCL_%d,,%4.3f,0", m_iBand, m_iChannel, iPCL, dMeasured);
            TraceLog(MSG_INFO, szTmp);

            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "GSM FrequencyError (PCL:%d) FAIL", iPCL);
            m_strMsg = szTmp ;
            TraceLog(MSG_INFO, szTmp);
        }

        FactoryLog(m_bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
    }

  
    return true;
}


bool CGSM_Online_MeasureLoop::Modulation()
{
    const float dPowerLower = -99 ;
    int iMODOffsetAmount = m_vOffsetMod.size() ;
    int iMODPCLAmount = m_vPCLMod.size() ;
    char szTmp[MAX_PATH];

	m_strItem = "GSM Modulation" ;
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM Modulation Loop Finish";

    
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelMod) ) {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Modulation Measured") ;
        return false ;
    }

    for (int iPCLCounter=0; iPCLCounter<iMODPCLAmount; ++iPCLCounter)
    {
        int iPCL = atoi( m_vPCLMod[iPCLCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) {
            TraceLog(MSG_ERROR, "Fail to Set PCL in Modulation Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchModulationSpectrum(strMeasured, m_iBurstMod) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch Modulation Spectrum Array") ;
            return false ;
        }

        std::vector<std::string> vModulationSpectrum ;
        STDStringTokenizer(strMeasured, ",", vModulationSpectrum);
        if (strstr(vModulationSpectrum[0].c_str(), "NAN") != NULL)
        {
            TraceLog(MSG_ERROR, "Fail to measure Modulation, Fetch NAN. Retry...") ;

            if (! m_pITesterDevice->GSM_FetchModulationSpectrum(strMeasured, m_iBurstMod) )
            {
                TraceLog(MSG_ERROR, "Fail to Fetch Modulation Spectrum Array") ;
                return false ;
            }

            STDStringTokenizer(strMeasured, ",", vModulationSpectrum);
            if (strstr(vModulationSpectrum[0].c_str(), "NAN") != NULL) {
                TraceLog(MSG_ERROR, "Fail to measure Modulation, Fetch NAN") ;
                return false;
            }
        }

        for (int iOffsetCounter=0; iOffsetCounter<iMODOffsetAmount; ++iOffsetCounter)
        {
            int iOffset = atoi( m_vOffsetMod[iOffsetCounter].c_str() ) ;
            int iUpperIndex = (iPCLCounter*iMODOffsetAmount) + iOffsetCounter ;
            double dMeasured = atof(vModulationSpectrum[iOffsetCounter].c_str());           
            double dUpper = atof(m_vUpperMod[iUpperIndex].c_str()) ;

            ::ZeroMemory(szTmp, MAX_PATH) ;
            sprintf_s(szTmp, MAX_PATH, "GSM Modulation PCL%02d (%d)", iPCL, iOffset);
            m_strItem = szTmp;
            ::ZeroMemory(szTmp, MAX_PATH) ;
            sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
            m_strMeasured = szTmp ;
            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "%4.3f", dUpper) ;
            m_strUpper = szTmp ;

            m_strLower = "-" ;
            m_strUnit = "dBc" ;

            ::ZeroMemory(szTmp, MAX_PATH);
            if ( InRange(dPowerLower, dUpper, dMeasured))
            {
                m_bResult = true ;

                sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,Modulation,%d,%d,PCL_%d,Offset_%d,%4.3f,1", m_iBand, m_iChannel, iPCL, iOffset, dMeasured);
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM Modulation (PCL:%d, Offset:%d) PASS", iPCL, iOffset) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            }
            else
            {
                m_bResult = false ;

                sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,Modulation,%d,%d,PCL_%d,Offset_%d,%4.3f,0", m_iBand, m_iChannel, iPCL, iOffset, dMeasured);
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM Modulation (PCL:%d, Offset:%d) FAIL", iPCL, iOffset) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            }
            FactoryLog(m_bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
        }
    }

   
    return true;
}


bool CGSM_Online_MeasureLoop::Switching()
{
    const float dPowerLower = -99 ;
    int iSwitchOffsetAmount = m_vOffsetSwitch.size() ;
    int iSwitchPCLAmount = m_vPCLSwitch.size() ;
    char szTmp[MAX_PATH];

	m_strItem = "GSM Switching" ;
	m_strErrCode = FunErr_WirelessGSM_Online_Handover_Fail;
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM Switching Loop Finish";

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelSwitch) ) 
	{
		m_strMsg = "Fail to Set Traffic Channel Time slot Level, During Switching Measured";
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Switching Measured") ;
        return false ;
    }

    for (int iPCLCounter=0; iPCLCounter<iSwitchPCLAmount; ++iPCLCounter)
    {
        int iPCL = atoi( m_vPCLSwitch[iPCLCounter].c_str() ) ;

        if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) ) 
		{
			m_strMsg = "Fail to Set PCL in Switching Test";
            TraceLog(MSG_ERROR, "Fail to Set PCL in Switching Test") ;
            return false ;
        }
        Sleep(iPCL_Delay) ;

        std::string strMeasured ;
        if (! m_pITesterDevice->GSM_FetchSwitchingSpectrum(strMeasured, m_iBurstSwitch) )
        {
			m_strMsg = "Fail to Fetch Switching Spectrum Array";
            TraceLog(MSG_ERROR, "Fail to Fetch Switching Spectrum Array") ;
            return false ;
        }

        std::vector<std::string> vSwitchingSpectrum ;
        STDStringTokenizer(strMeasured, ",", vSwitchingSpectrum);
        if (strstr(vSwitchingSpectrum[0].c_str(), "NAN") != NULL)
        {
            TraceLog(MSG_ERROR, "Fail to measure Switching, Fetch NAN. Retry...") ;

            if (! m_pITesterDevice->GSM_FetchSwitchingSpectrum(strMeasured, m_iBurstSwitch) )
            {
				m_strMsg = "Fail to Fetch Switching Spectrum Array";
                TraceLog(MSG_ERROR, "Fail to Fetch Switching Spectrum Array") ;
                return false ;
            }

            STDStringTokenizer(strMeasured, ",", vSwitchingSpectrum);
            if (strstr(vSwitchingSpectrum[0].c_str(), "NAN") != NULL) 
			{
				m_strMsg = "Fail to measure Switching, Fetch NAN";
                TraceLog(MSG_ERROR, "Fail to measure Switching, Fetch NAN") ;
                return false;
            }
        }


        for (int iOffsetCounter=0; iOffsetCounter<iSwitchOffsetAmount; ++iOffsetCounter)
        {
            int iOffset = atoi( m_vOffsetSwitch[iOffsetCounter].c_str() ) ;
            int iUpperIndex = (iPCLCounter*iSwitchOffsetAmount) + iOffsetCounter ;
            double dMeasured = atof(vSwitchingSpectrum[iOffsetCounter].c_str());           
            double dUpper = atof(m_vUpperSwitch[iUpperIndex].c_str()) ;

            ::ZeroMemory(szTmp, MAX_PATH) ;
            sprintf_s(szTmp, MAX_PATH, "GSM Switching PCL%02d (%d)", iPCL, iOffset);
            m_strItem = szTmp;
            ::ZeroMemory(szTmp, MAX_PATH) ;
            sprintf_s(szTmp, MAX_PATH, "%4.3f", dMeasured) ;
            m_strMeasured = szTmp ;
            ::ZeroMemory(szTmp, MAX_PATH);
            sprintf_s(szTmp, MAX_PATH, "%4.3f", dUpper) ;
            m_strUpper = szTmp ;

            m_strLower = "-" ;
            m_strUnit = "dBm" ;

            ::ZeroMemory(szTmp, MAX_PATH);
            if ( InRange(dPowerLower, dUpper, dMeasured))
            {
                m_bResult = true ;
				m_strErrCode = "Pass";
                sprintf_s(szTmp, MAX_PATH, "Pass,Detroit_Wireless2_MeasurementData:GSM,Switching,%d,%d,PCL_%d,Offset_%d,%4.3f,1", m_iBand, m_iChannel, iPCL, iOffset, dMeasured);
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM Switching (PCL:%d, Offset:%d) PASS", iPCL, iOffset) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            }
            else
            {
                m_bResult = false ;

                sprintf_s(szTmp, MAX_PATH, "Fail,Detroit_Wireless2_MeasurementData:GSM,Switching,%d,%d,PCL_%d,Offset_%d,%4.3f,0", m_iBand, m_iChannel, iPCL, iOffset, dMeasured);
                TraceLog(MSG_INFO, szTmp);

                sprintf_s(szTmp, MAX_PATH, "GSM Switching (PCL:%d, Offset:%d) FAIL", iPCL, iOffset) ;
                m_strMsg = szTmp ;
                TraceLog(MSG_INFO, m_strMsg);
            }

            FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
        }
    }

  

    return m_bResult;
}


bool CGSM_Online_MeasureLoop::RxLevel()
{
    //int iLevelAmount = m_vLevelRxLv.size() ;

    //m_strItem = "GSM RxLevel" ;
    //m_strErrCode = "-";
    //m_strUpper = "-";
    //m_strLower = "-";
    //m_strMeasured = "-";
    //m_strUnit = "-";
    //m_strMsg = "-";


    //// Set PCL
    //if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, m_iPCLRxLv) )
    //{
    //    Sleep(1000) ;
    //    if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, m_iPCLRxLv) )
    //    {
    //        TraceLog(MSG_ERROR, "Fail on QMSL_FTM_GSM_BER_Handover") ;
    //        return false ;
    //    }
    //}

    //if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLRxLv) ) {
    //    TraceLog(MSG_ERROR, "Fail to Set PCL in RxLevel Test") ;
    //    return false ;
    //}

    //for (int iLevelCounter=0; iLevelCounter<iLevelAmount; ++iLevelCounter)
    //{
    //    double fPowerLevel = atof( m_vLevelRxLv[iLevelCounter].c_str() ) ;
    //    int iUpper = atoi( m_vUpperRxLv[iLevelCounter].c_str() ) ;
    //    int iLower = atoi( m_vLowerRxLv[iLevelCounter].c_str() ) ;

    //    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover((float)fPowerLevel) ) {
    //        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During RxLevel Measured") ;
    //        return false ;
    //    }

    //    int iRetry = 20 ;
    //    bool bRet = false ;
    //    unsigned char iRxLv, iRxQuality ;
    //    Sleep(5000) ;
    //    do {
    //        iRxLv=-1, iRxQuality=-1 ;
    //        m_pIPhone->QMSL_FTM_GSM_BER_GetRxMetrics(&iRxLv, &iRxQuality) ;

    //        if ( 0>iRxLv ) {
    //            TraceLog(MSG_INFO, "Fail to get Rx Metrics from phone, Retrying") ;
    //        } else {
    //            bRet = true ;
    //            break ;
    //        }

    //        --iRetry ;
    //        Sleep(500) ;
    //    } while ( (0<iRetry)&&(0>iRxLv) ) ;

    //    if (!bRet) {
    //        TraceLog(MSG_ERROR, "Fail to get Rx Metrics from phone") ;
    //        return false ;
    //    }

    //    int iMeasured = iRxLv ;

    //    char szTmp[MAX_PATH] ;
    //    ::ZeroMemory(szTmp, MAX_PATH) ;
    //    sprintf_s(szTmp, MAX_PATH, "GSM RxLevel (%.1f)", fPowerLevel) ;
    //    m_strItem = szTmp ;
    //    ::ZeroMemory(szTmp, MAX_PATH) ;
    //    sprintf_s(szTmp, MAX_PATH, "%d", iLower) ;
    //    m_strLower = szTmp ;
    //    ::ZeroMemory(szTmp, MAX_PATH) ;
    //    sprintf_s(szTmp, MAX_PATH, "%d", iUpper) ;
    //    m_strUpper = szTmp ;
    //    ::ZeroMemory(szTmp, MAX_PATH) ;
    //    sprintf_s(szTmp, MAX_PATH, "%d", iMeasured) ;
    //    m_strMeasured = szTmp ;

    //    if ( InRange(iLower, iUpper, iMeasured) )
    //    {
    //        m_bResult = true ;

    //        sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,RxLevel,%d,%d,Level_%.1f,PCL_%d,%d,1", m_iBand, m_iChannel, fPowerLevel, m_iPCLRxLv, iMeasured);
    //        TraceLog(MSG_INFO, szTmp);

    //        sprintf_s(szTmp, MAX_PATH, "GSM RxLevel (Level:%.1f) PASS", fPowerLevel) ;
    //        m_strMsg = szTmp ;
    //        TraceLog(MSG_INFO, m_strMsg);
    //    }
    //    else
    //    {
    //        m_bResult = false ;

    //        sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,RxLevel,%d,%d,Level_%.1f,PCL_%d,%d,0", m_iBand, m_iChannel, fPowerLevel, m_iPCLRxLv, iMeasured);
    //        TraceLog(MSG_INFO, szTmp);

    //        sprintf_s(szTmp, MAX_PATH, "GSM RxLevel (Level:%.1f) FAIL", fPowerLevel) ;
    //        m_strMsg = szTmp ;
    //        TraceLog(MSG_INFO, m_strMsg);
    //    }

    //    FactoryLog(m_bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
    //}

    //m_strItem = "GSM RxLevel" ;
    //m_strErrCode = "-";
    //m_strUpper = "-";
    //m_strLower = "-";
    //m_strMeasured = "-";
    //m_strUnit = "-";
    //m_strMsg = "GSM RxLevel Loop Finish";

   return true ;
}


bool CGSM_Online_MeasureLoop::RFER()
{
    const int iKey_ClassII = 1 ;
    const int iKey_FER = 3 ;
    char szTmp[MAX_PATH] ;

	m_strItem = "GSM RBER";
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM RBER Loop Finish";

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelRFER) )
    {
		m_strMsg = "Fail to Set TCH Timeslot Level, During RBER Measured";
        TraceLog(MSG_ERROR, "Fail to Set TCH Timeslot Level, During RBER Measured") ;
        return false ;
    }

    if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLRFER) ) 
	{
		m_strMsg = "Fail to Set PCL in RBER Test";
        TraceLog(MSG_ERROR, "Fail to Set PCL in RBER Test") ;
        return false ;
    }

    std::string strMeasured;
    if (! m_pITesterDevice->GSM_FetchRXQualityRFER(strMeasured, m_iBurstRFER) )
    {
		m_strMsg = "Fail to Fetch RX Quality RBER";
        TraceLog(MSG_ERROR, "Fail to Fetch RX Quality RBER") ;
        return false ;
    }

    std::vector<std::string> vRFERList ;
    STDStringTokenizer(strMeasured, ",", vRFERList);

    // Check RBER ClassII value
    if (strstr(vRFERList[iKey_ClassII].c_str(), "NAN") != NULL)
    {
        TraceLog(MSG_ERROR, "Fail to measure RBER ClassII value, NAN fetched");

        strMeasured.clear() ;
        if (! m_pITesterDevice->GSM_FetchRXQualityRFER(strMeasured, m_iBurstRFER) )
        {
			m_strMsg = "Fail to Fetch RX Quality RBER";
            TraceLog(MSG_ERROR, "Fail to Fetch RX Quality RBER") ;
            return false ;
        }

        vRFERList.clear() ;
        STDStringTokenizer(strMeasured, ",", vRFERList);
        if (strstr(vRFERList[iKey_ClassII].c_str(), "NAN") != NULL)
        {
			m_strMsg = "Fail to measure RBER ClassII value, NAN fetched";
            TraceLog(MSG_ERROR, "Fail to measure RBER ClassII value, NAN fetched");
            return false ;
        }
    }

    double dMeasuredRBER = atof(vRFERList[iKey_ClassII].c_str());


    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%.4f", m_dLowerRBER);
    m_strLower = szTmp;
    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%.4f", m_dUpperRBER);
    m_strUpper = szTmp;
    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%4.4f", dMeasuredRBER) ;
    m_strMeasured = szTmp ;

    ::ZeroMemory(szTmp, MAX_PATH);
    if ( InRange(m_dLowerRBER, m_dUpperRBER, dMeasuredRBER) )
    {
        m_bResult = true ; 
		m_strErrCode = "Pass";
		sprintf_s(szTmp, MAX_PATH, "Pass:Detroit_Wireless2_MeasurementData:GSM,RBER,%d,%d,,,%4.4f,1", m_iBand, m_iChannel, dMeasuredRBER);
        TraceLog(MSG_INFO, szTmp);

        m_strMsg = szTmp;
        TraceLog(MSG_INFO, m_strMsg);
    }
    else
    {
        m_bResult = false ;

		sprintf_s(szTmp, MAX_PATH, "Fail:Detroit_Wireless2_MeasurementData:GSM,RBER,%d,%d,,,%4.4f,0", m_iBand, m_iChannel, dMeasuredRBER);
        TraceLog(MSG_INFO, szTmp);

        m_strMsg = szTmp;
        TraceLog(MSG_INFO, m_strMsg);
    }
    FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;


	m_strItem = "GSM FER";
	m_strErrCode = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMsg = "GSM FER Loop Finish";
    // Check FER value
    if (strstr(vRFERList[iKey_FER].c_str(), "NAN") != NULL)
    {
        TraceLog(MSG_ERROR, "Fail to measure FER value, NAN fetched");

        strMeasured.clear() ;
        if (! m_pITesterDevice->GSM_FetchRXQualityRFER(strMeasured, m_iBurstRFER) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch RX Quality FER") ;
			m_strMsg = "Fail to Fetch RX Quality FER";
            return false ;
        }

        vRFERList.clear() ;
        STDStringTokenizer(strMeasured, ",", vRFERList);
        if (strstr(vRFERList[iKey_ClassII].c_str(), "NAN") != NULL)
        {
            TraceLog(MSG_ERROR, "Fail to measure RBER ClassII value, NAN fetched");
			m_strMsg = "ail to measure RBER ClassII value, NAN fetched";
            return false ;
        }
    }

    double dMeasuredFER = atof(vRFERList[iKey_FER].c_str());

    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%.4f", m_dLowerFER);
    m_strLower = szTmp;
    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%.4f", m_dUpperFER);
    m_strUpper = szTmp;
    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%4.4f", dMeasuredFER) ;
    m_strMeasured = szTmp ;

    ::ZeroMemory(szTmp, MAX_PATH);
    if ( InRange(m_dLowerFER, m_dUpperFER, dMeasuredFER) )
    {
        m_bResult = true ;
		m_strErrCode = "Pass";
		sprintf_s(szTmp, MAX_PATH, "Pass:Detroit_Wireless2_MeasurementData:GSM,FER,%d,%d,,,%4.4f,1", m_iBand, m_iChannel, dMeasuredFER);
        TraceLog(MSG_INFO, szTmp);

        m_strMsg = "GSM FER PASS" ;
        TraceLog(MSG_INFO, m_strMsg);
    }
    else
    {
        m_bResult = false ;

		sprintf_s(szTmp, MAX_PATH, "Fail:Detroit_Wireless2_MeasurementData:GSM,FER,%d,%d,,,%4.4f,0", m_iBand, m_iChannel, dMeasuredFER);
        TraceLog(MSG_INFO, szTmp);

        m_strMsg = szTmp ;
        TraceLog(MSG_ERROR, m_strMsg);
    }
    FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;

    return m_bResult;
}


bool CGSM_Online_MeasureLoop::Sensitivity()
{
    const int iKey_BER = 1 ;

    m_strItem = "GSM MinSensitivity";
    m_strMeasured = "-" ;
    m_strLower    = "-" ;
    m_strUpper    = "-" ;
    m_strUnit = "dBm";
	m_strErrCode = FunErr_WirelessGSM_Online_RSSI_OutRange;

    if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLSensitivity) ) {
        TraceLog(MSG_ERROR, "Fail to Set PCL in Sensitivity Test") ;
        return false ;
    }

    char szTmp[MAX_PATH];
    std::string strMeasured ;
    float fMaxPower = m_fPowerLevelSensitivity_Start ;
    bool  bStatus = false ;
    double dMeasured ;
    std::vector<std::string> vBERList ;
    float fDeviation ;

    // Run First Time to determine increasing or decreasing
    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevelSensitivity_Start) )
    {
        TraceLog(MSG_ERROR, "Fail to Set Traffic Channel Time slot Level, During Min Sensitivity Measured") ;
        return false ;
    }

    Sleep(iPCL_Delay) ;
    if (! m_pITesterDevice->GSM_FetchRXQualityBER(strMeasured, m_iBurstSensitivity) )
    {
        TraceLog(MSG_ERROR, "Fail to Fetch RX Quality BER") ;
        return false ;
    }
    
    STDStringTokenizer(strMeasured, ",", vBERList);
    if (strstr(vBERList[iKey_BER].c_str(), "NAN") != NULL)
    {
        TraceLog(MSG_ERROR, "Fail to measure BER value, NAN fetched");
        
        strMeasured.clear() ;
        if (! m_pITesterDevice->GSM_FetchRXQualityBER(strMeasured, m_iBurstSensitivity) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch RX Quality BER") ;
            return false ;
        }

        STDStringTokenizer(strMeasured, ",", vBERList);
        if (strstr(vBERList[iKey_BER].c_str(), "NAN") != NULL)
        {
            TraceLog(MSG_ERROR, "Fail to measure BER value, NAN fetched");
            return false ;
        }

    }
    dMeasured = atof(vBERList[iKey_BER].c_str()); ;


    // Determine increasing or decreasing
    if ( InRange(m_dLowerBER, m_dUpperBER, dMeasured) ) {
        fDeviation = 0.5f ;
    } else {
        fDeviation = -0.5f ;
    }


    // Start Search Mini. Sensitivity
    while (!bStatus)
    {
        fMaxPower = fMaxPower - fDeviation ;

        Sleep(iPCL_Delay) ;
        if (! m_pITesterDevice->GSM_FetchRXQualityBER(strMeasured, m_iBurstSensitivity) )
        {
            TraceLog(MSG_ERROR, "Fail to Fetch RX Quality BER") ;
            return false ;
        }

        STDStringTokenizer(strMeasured, ",", vBERList);
        if (strstr(vBERList[iKey_BER].c_str(), "NAN") != NULL)
        {
            TraceLog(MSG_ERROR, "Fail to measure BER value, NAN fetched");
            return false ;
        }
        dMeasured = atof(vBERList[iKey_BER].c_str());

        ::ZeroMemory(szTmp, MAX_PATH) ;
        if ( InRange(m_dLowerSensitivity, m_dUpperSensitivity, dMeasured) )
        {
            sprintf_s(szTmp, MAX_PATH, "GSM Sensitivity Measured PASS (%.1fdBm, %4.3f)", fMaxPower, dMeasured ) ;

            if ( 0>fDeviation )
                bStatus = true ;
        } else {
            sprintf_s(szTmp, MAX_PATH, "GSM Sensitivity Measured FAIL (%.1fdBm, %4.3f)", fMaxPower, dMeasured ) ;

            if ( 0<fDeviation )
                bStatus = true ;
        }

        TraceLog(MSG_INFO, szTmp) ;
    }

    
    if ( 0<fDeviation ) {
        // roll back last pass value.
        fMaxPower = fMaxPower + fDeviation ;
    }

    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "%4.2f", fMaxPower);
    m_strMeasured = szTmp ;
    
    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, ",Detroit_Wireless2_MeasurementData:GSM,Sensitivity,%d,%d,,,%.1f,1", m_iBand, m_iChannel, fMaxPower);
    TraceLog(MSG_INFO, szTmp);

    ::ZeroMemory(szTmp, MAX_PATH) ;
    sprintf_s(szTmp, MAX_PATH, "GSM MinSensitivity Loop Finish (%.1f)", fMaxPower ) ;
    TraceLog(MSG_INFO, szTmp) ;

    return true;
}



bool CGSM_Online_MeasureLoop::BER()
{
    const int iKey_ClassII = 1 ;
    char szTmp[MAX_PATH] ;

	m_strErrCode = ERR_GSMTEST_BER ;
    m_strUnit = "%";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strMsg = "GSM BER Test Pass";
	std::string str_range;

	::ZeroMemory(szTmp, MAX_PATH) ;
	sprintf_s(szTmp, MAX_PATH, "GSM%d_", m_iBand);
	m_strItem = szTmp;

	if (!GSM_GetRangeForTxRxCableLoss(m_iBand,m_iChannel,str_range))
	{
		m_strMsg = "Fail to Get range for GSM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	
	float f_powerlevelber;
	if (strstr(str_range.c_str(),"mid") != NULL )
	{
		f_powerlevelber = m_fPowerLevelBER_Mid;
	}
	else if (strstr(str_range.c_str(),"low") != NULL )
	{
		f_powerlevelber = m_fPowerLevelBER_Low;
	}
	else if (strstr(str_range.c_str(),"high") != NULL )
	{
		f_powerlevelber = m_fPowerLevelBER_High;
	}

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(f_powerlevelber) )
    {
		m_strMsg = "Fail to Set TCH Timeslot Level, During BFER Measured";
        TraceLog(MSG_ERROR, "Fail to Set TCH Timeslot Level, During BFER Measured") ;
        return false ;
    }


    if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLBER) ) 
	{
		m_strMsg = "Fail to Set PCL in BER Test";
        TraceLog(MSG_ERROR, "Fail to Set PCL in BER Test") ;
        return false ;
    }

	Sleep(1000);
    std::string strMeasured;
	::ZeroMemory(szTmp, MAX_PATH) ;
	sprintf_s(szTmp, MAX_PATH, "%.4f", m_dLowerBER);
	m_strLower = szTmp;
	::ZeroMemory(szTmp, MAX_PATH) ;
	sprintf_s(szTmp, MAX_PATH, "%.4f", m_dUpperBER);
	m_strUpper = szTmp;

	m_bResult = false;
	int i = 0;
	while (i++ < m_iRetryTimes)
	{
		Sleep(500);
		if (! m_pITesterDevice->GSM_FetchRXQualityBER(strMeasured, m_iBurstBER) )
		{
			m_strMsg = "Fail to Fetch RX Quality BER";
			TraceLog(MSG_ERROR, "Fail to Fetch RX Quality BER") ;
			break;
		}

		std::vector<std::string> vBERList ;
		STDStringTokenizer(strMeasured, ",", vBERList);

		// Check RBER ClassII value
		if (strstr(vBERList[iKey_ClassII].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure BER ClassII value, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		double dMeasured = atof(vBERList[iKey_ClassII].c_str());	
		::ZeroMemory(szTmp, MAX_PATH) ;
		sprintf_s(szTmp, MAX_PATH, "%4.4f", dMeasured) ;
		m_strMeasured = szTmp ;

		::ZeroMemory(szTmp, MAX_PATH);
		if ( InRange(m_dLowerBER, m_dUpperBER, dMeasured) )
		{
			m_bResult = true ;

			m_strErrCode = "Pass";

			sprintf_s(szTmp, MAX_PATH, "Pass:Detroit_Wireless2_MeasurementData:GSM,BER,%d,%d,,,%4.4f,1", m_iBand, m_iChannel, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			TraceLog(MSG_INFO, "GSM BER PASS");	
			break;
		}
		else
		{
			sprintf_s(szTmp, MAX_PATH, "Fail:Detroit_Wireless2_MeasurementData:GSM,BER,%d,%d,,,%4.4f,0", m_iBand, m_iChannel, dMeasured);

			m_strMsg = szTmp;

			TraceLog(MSG_INFO, szTmp);
		}
	} 

	if (m_bResult)
	{
		std::string strRSSIMeasured;
		int i =0;
		m_bResult = false;
		while (i++ < m_iRetryTimes)
		{
			Sleep(500);

			if (! m_pITesterDevice->GSM_FetchBERRSSI(strRSSIMeasured) )
			{
				m_strMsg = "Fail to Fetch RX Quality RSSI";
				TraceLog(MSG_ERROR, m_strMsg) ;
				break;
			}

			if (strstr(strRSSIMeasured.c_str(), "NAN") != NULL)
			{
				m_strMsg = "Fail to measure RSSI, NAN fetched";
				TraceLog(MSG_ERROR, m_strMsg);
				continue;
			}
			else
			{
				double f_rssi = atof(strRSSIMeasured.c_str()) - 110.0;
				::ZeroMemory(szTmp, MAX_PATH) ;
				sprintf_s(szTmp, MAX_PATH, "%.4f", f_rssi);
				strRSSIMeasured = szTmp;
				FactoryLog(true, "RSSI", "Pass", m_strBand, m_strChannel, "--", "--", strRSSIMeasured, m_strUnit, "Get RSSI pass") ;
				SetPICSDate(m_strItem + "RSSI",strRSSIMeasured);
				m_bResult = true;
				break;
			}	
		}
	}
	
	FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
	SetPICSDate(m_strItem + "BER",m_strMeasured);
    return m_bResult;
}


bool CGSM_Online_MeasureLoop::CPK()
{
	const int iKey_ClassII = 1 ;
	char szTmp[MAX_PATH] ;

	//m_strItem = "CPK";
	m_strErrCode = ERR_GSMTEST_BER ;
	m_strUnit = "%";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strMsg = "GSM CPK Test Pass";
	std::string str_range;

	::ZeroMemory(szTmp, MAX_PATH) ;
	sprintf_s(szTmp, MAX_PATH, "GSM%d_", m_iBand);
	m_strItem = szTmp;


	if (!GSM_GetRangeForTxRxCableLoss(m_iBand,m_iChannel,str_range))
	{
		m_strMsg = "Fail to Get range for GSM";
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	float f_powerStart;
	if (strstr(str_range.c_str(),"mid") != NULL )
	{
		f_powerStart = m_fPowerLevelBER_Mid;
	}
	else if (strstr(str_range.c_str(),"low") != NULL )
	{
		f_powerStart = m_fPowerLevelBER_Low;
	}
	else if (strstr(str_range.c_str(),"high") != NULL )
	{
		f_powerStart = m_fPowerLevelBER_High;
	}


	m_bResult = false;
	float f_powerTemp = f_powerStart;

	bool bSatus = false;

	while(f_powerTemp <= f_powerStart)
	{
		if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(f_powerTemp) )
		{
			m_strMsg = "Fail to Set TCH Timeslot Level, During BFER Measured";
			TraceLog(MSG_ERROR, "Fail to Set TCH Timeslot Level, During BFER Measured") ;
			return false ;
		}

		if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLBER) ) 
		{
			m_strMsg = "Fail to Set PCL in BER Test";
			TraceLog(MSG_ERROR, "Fail to Set PCL in BER Test") ;
			return false ;
		}

		Sleep(1000);
		std::string strMeasured;
		::ZeroMemory(szTmp, MAX_PATH) ;
		sprintf_s(szTmp, MAX_PATH, "%.4f", m_dLowerBER);
		m_strLower = szTmp;
		::ZeroMemory(szTmp, MAX_PATH) ;
		sprintf_s(szTmp, MAX_PATH, "%.4f", m_dUpperBER);
		m_strUpper = szTmp;
	
		if (! m_pITesterDevice->GSM_FetchRXQualityBER(strMeasured, m_iBurstBER) )
		{
			m_strMsg = "Fail to Fetch RX Quality BER";
			TraceLog(MSG_ERROR, "Fail to Fetch RX Quality BER") ;
			return false;
		}

		std::vector<std::string> vBERList ;
		STDStringTokenizer(strMeasured, ",", vBERList);

		// Check RBER ClassII value
		if (strstr(vBERList[iKey_ClassII].c_str(), "NAN") != NULL)
		{
			m_strMsg = "Fail to measure BER ClassII value, NAN fetched";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		double dMeasured = atof(vBERList[iKey_ClassII].c_str());	
		::ZeroMemory(szTmp, MAX_PATH);
		sprintf_s(szTmp, MAX_PATH, "Power= %4.4f, BER = %4.4f",f_powerTemp, dMeasured);
		TraceLog(MSG_INFO, szTmp);

		
		if ( InRange(m_dLowerBER, m_dUpperBER, dMeasured) )
		{
			::ZeroMemory(szTmp, MAX_PATH) ;
			sprintf_s(szTmp, MAX_PATH, "%4.4f", dMeasured) ;
			m_strMeasured = szTmp ;
			
			f_powerTemp -= m_fPowerLevelStep;
			m_bResult  = true;		
		}
		else
		{		
			if (!m_bResult)
			{
				::ZeroMemory(szTmp, MAX_PATH) ;
				sprintf_s(szTmp, MAX_PATH, "%4.4f", dMeasured) ;
				m_strMeasured = szTmp ;
			}
			else
			{
				f_powerTemp += m_fPowerLevelStep;
			}
			
			break;		
		}
	}

	if (m_bResult)
	{
		std::string strRSSIMeasured;
		int i =0;
		m_bResult = false;

		::ZeroMemory(szTmp, MAX_PATH) ;
		sprintf_s(szTmp, MAX_PATH, "%.4f", f_powerTemp);
		SetPICSDate(m_strItem + "CPK",szTmp);

		if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(f_powerTemp) )
		{
			m_strMsg = "Fail to Set TCH Timeslot Level, During BFER Measured";
			TraceLog(MSG_ERROR, "Fail to Set TCH Timeslot Level, During BFER Measured") ;
			return false ;
		}

		if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCLBER) ) 
		{
			m_strMsg = "Fail to Set PCL in BER Test";
			TraceLog(MSG_ERROR, "Fail to Set PCL in BER Test") ;
			return false ;
		}

		Sleep(1000);

		while (i++ < m_iRetryTimes)
		{
			Sleep(500);

			if (! m_pITesterDevice->GSM_FetchBERRSSI(strRSSIMeasured) )
			{
				m_strMsg = "Fail to Fetch RX Quality RSSI";
				TraceLog(MSG_ERROR, m_strMsg) ;
				break;
			}

			if (strstr(strRSSIMeasured.c_str(), "NAN") != NULL)
			{
				m_strMsg = "Fail to measure RSSI, NAN fetched";
				TraceLog(MSG_ERROR, m_strMsg);
				continue;
			}
			else
			{
				double f_rssi = atof(strRSSIMeasured.c_str()) - 110.0;
				::ZeroMemory(szTmp, MAX_PATH) ;
				sprintf_s(szTmp, MAX_PATH, "%.4f", f_rssi);
				strRSSIMeasured = szTmp;
				FactoryLog(true, "RSSI", "Pass", m_strBand, m_strChannel, "--", "--", strRSSIMeasured, m_strUnit, "Get RSSI pass") ;
				SetPICSDate(m_strItem + "RSSI",strRSSIMeasured);
				m_bResult = true;
				break;
			}	
		}
	}

	FactoryLog(m_bResult, m_strItem, m_strErrCode, m_strBand, m_strChannel, m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
	SetPICSDate(m_strItem + "BER",m_strMeasured);
	return m_bResult;
}

bool CGSM_Online_MeasureLoop::HandOverChannel( int i_channel )
{
	if (m_iChannel == m_pITesterDevice->GSM_QueryChannel())
	{
		return true;
	}

	if (! m_pITesterDevice->GSM_SetBandIndicator(m_iBand) )
	{
		m_strMsg = "Fail to Set Band Indicator, During Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	if (! m_pITesterDevice->GSM_SetTCH_Handover(i_channel) )
	{
		m_strMsg = "Fail to set Signaling Traffic Channel during Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	//if (! m_pITesterDevice->GSM_CheckCallStatusCED() )
	//{
	//	if (! m_pITesterDevice->GSM_CheckCallStatusCEST() )
	//	{
	//		m_strMsg = "Fail to Check Call Status" ;
	//		TraceLog(MSG_ERROR, m_strMsg) ;
	//		return false ;
	//	}
	//}	
/*
	if (! m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCL) )
	{
		m_strMsg = "Fail to set Traffic Channel Time Slot PCL during Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	if (! m_pITesterDevice->GSM_SetMSMaxPower(m_fMaxPower) )
	{
		m_strMsg = "Fail to Set Input Level during Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}


	TraceLog(MSG_INFO, "Success to Set Handover Parameters") ;
*/

	bool b_online = true;
	if (! m_pITesterDevice->GSM_CheckCallStatusCED() )
	{
		if (! m_pITesterDevice->GSM_CheckCallStatusCEST() )
		{
			m_strMsg = "Fail to Check Call Status, Start call again!" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			b_online = false ;
		}
	}

	if (!b_online)
	{
		if (!GSM_StartCall())
		{
			return false;
		}	
	}
	
	TraceLog(MSG_INFO, "GSM Handover Success.") ;

	return true;
}

bool CGSM_Online_MeasureLoop::GSM_StartCall()
{
	if (! m_pITesterDevice->GSM_MT_Call())
	{
		m_strMsg = "Fail to GSM MT call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	TraceLog(MSG_INFO, "MT call OK") ;
	return true;
}





