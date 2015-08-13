#include "stdafx.h"
#include "GSM_Wireless_FTM_MeasureLoop.h"
#include <math.h>

#include "QLib.h"
#include "QLib_Defines.h"

RF_IMPLEMENT_DYNCREATE(CGSM_Wireless_FTM_MeasureLoop)

bool CGSM_Wireless_FTM_MeasureLoop::Run()
{
	m_strErrCode.clear() ;

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

	m_strBand = "GSM" + CStr::IntToStr(m_iBand);
	m_strChannel = CStr::IntToStr(m_iChannel);

	bool bRet = MainFunction();

	if (bRet)
	{
		m_strMsg = "Success to Run GSM TX Test" ;
        TraceLog(MSG_INFO, m_strMsg) ;
        
        FactoryLog(bRet, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}
	else
	{
		if ( m_strErrCode.empty() )
		{
            m_strErrCode = ERR_GSMTEST_STARTCALL ;
		}

        FactoryLog(bRet, m_strItem, m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg ) ;
	}


	return bRet;
}

bool CGSM_Wireless_FTM_MeasureLoop::MainFunction()
{
	const int iKey_AvgBurstPwAvg = 6 ;
	int iPCLAmount = m_vPCLTxPower.size() ;
	char szTmp[MAX_PATH];
	m_bItemResult = true ;

	// Set Error Code
	m_strErrCode = ERR_GSMTEST_TXPOWER ;

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
		sprintf_s(szTmp, MAX_PATH, "%.3f", fLower);
		m_strLower = szTmp;
		::ZeroMemory(szTmp, MAX_PATH) ;
		sprintf_s(szTmp, MAX_PATH, "%.3f", fUpper);
		m_strUpper = szTmp;
		m_strMeasured = "-";
		m_strUnit = "dBm";


		if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, iPCL) )
		{
			Sleep(1000) ;
			if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, iPCL) )
			{
				m_strMsg = "Fail on QMSL_FTM_GSM_BER_Handover" ;
				TraceLog(MSG_ERROR, m_strMsg) ;
				return false ;
			}
		}

		if ( !m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(iPCL) )
		{
			m_strMsg = "Fail to Set PCL in TxPower Test" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false ;
		}

		std::string strMeasured ;
		if (! m_pITesterDevice->GSM_FetchGMSKPower(strMeasured, m_iBurstTxPower) )
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
		sprintf_s(szTmp, MAX_PATH, "%.3f", dMeasured) ;
		m_strMeasured = szTmp ;

		::ZeroMemory(szTmp, MAX_PATH) ;
		std::string strRange;
		GSM_GetRangeForTxRxCableLoss(m_iBand, m_iChannel, strRange);

		if (_stricmp(strRange.c_str(),"low") == 0){
			sprintf_s(szTmp, 255, "TxPower_GSM_%d_LOW", m_iBand);
		}

		if (_stricmp(strRange.c_str(),"mid") == 0){
			sprintf_s(szTmp, 255, "TxPower_GSM_%d_MID", m_iBand);
		}

		if (_stricmp(strRange.c_str(),"high") == 0){
			sprintf_s(szTmp, 255, "TxPower_GSM_%d_HIGH", m_iBand);
		}

		std::string strKey1 = szTmp;

		bool bResult = InRange(fLower, fUpper, dMeasured) ;
		if ( bResult ) {
			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, ",C7_MeasurementData,GSM,TxPower,%d,%d,PCL_%d,,%.3f,1", m_iBand, m_iChannel, iPCL, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, "GSM TxPower (PCL:%d) PASS", iPCL);
			m_strMsg = szTmp ;
			TraceLog(MSG_INFO, szTmp);

			m_strPicsMsg=","+m_strMeasured;
			SetPICSDate(strKey1, "PASS"+m_strPicsMsg);

		} else {
			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, ",C7_MeasurementData,GSM,TxPower,%d,%d,PCL_%d,,%.3f,0", m_iBand, m_iChannel, iPCL, dMeasured);
			TraceLog(MSG_INFO, szTmp);

			::ZeroMemory(szTmp, MAX_PATH);
			sprintf_s(szTmp, MAX_PATH, "GSM TxPower (PCL:%d) FAIL", iPCL);
			m_strMsg = szTmp ;
			TraceLog(MSG_INFO, szTmp);

			m_strPicsMsg=","+m_strMeasured;
			SetPICSDate(strKey1, "FAIL"+m_strPicsMsg);
		}

		FactoryLog(bResult, m_strItem, "", "", "", m_strLower, m_strUpper, m_strMeasured, m_strUnit, m_strMsg) ;
		m_bItemResult = m_bItemResult && bResult ;
	}

	m_strItem = "GSM TxPower" ;
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";

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

bool CGSM_Wireless_FTM_MeasureLoop::InitData(std::map<std::string, std::string>& paramMap)
{
	// Copy Data
	m_paramMap = paramMap ;

	std::string strTmp;
	std::vector<std::string> vTmp;

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


	if (m_paramMap.find("Burst_TxPower") == m_paramMap.end()) {
		m_strMsg = "Fail to Get <Burst_TxPower> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	strTmp = m_paramMap["Burst_TxPower"];
	m_iBurstTxPower = atoi(strTmp.c_str()) ;

	return true ;
}