#include "stdafx.h"
#include "Wireless_GSM_DACTest.h"
#include "stdlib.h"
#include "QLib_Defines.h"


RF_IMPLEMENT_DYNCREATE(CWireless_GSM_DACTest)


#define GSM1900_DYN_RANGE		2749
#define GSM1800_DYN_RANGE		2745
#define GSM900_DYN_RANGE		2743
#define GSM850_DYN_RANGE		2747

#define GSM1900_TX_DAC			2552
#define GSM1800_TX_DAC			2536
#define GSM900_TX_DAC			2528
#define GSM850_TX_DAC			2544


bool CWireless_GSM_DACTest::Run()
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	char szBand[256];
	memset(szBand, 0, 256);
	sprintf_s(szBand, 255, "%d", m_iBand);

	char szChannel[256];
	memset(szChannel, 0, 256);
	sprintf_s(szChannel, 255, "%d", m_iChannel);

	if (! m_pIPhone->IsConnected_QMSL())
	{
		FactoryLog(false, "GSM TX Power Test", ERR_QLIB_PHONE_CONNECT, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm",
			"Fail to connect phone with QMSL");
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, "0.000");

		return false;
	}

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "GSM TX Power Test", "Pass", szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless GSM power PASS");
		SetPICSDate(m_strPICSName, "PASS");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}
	else
	{
		FactoryLog(false, "GSM TX Power Test", FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}

	if (! m_pIPhone->Disconnect_QMSL())
	{
		FactoryLog(false, "GSM TX Power Test", ERR_QLIB_DisconnectServer, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm",
			"Fail to disconnect phone with QMSL");
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, "0.000");

		return false;
	}

	return bRes;
}

bool CWireless_GSM_DACTest::MainFunction()
{
	unsigned char ucMode = 0;
	if (m_pIPhone->QMSL_IsFTM_Mode(&ucMode))
	{
		if (ucMode == 0)
		{
			if (! m_pIPhone->QMSL_ChangeFTM_Mode(3))
			{
				m_strMsg = "Fail to switch phone to FTM with QMSL";
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}			
		}
	}
	else
	{
		m_strMsg = "Fail to query phone mode with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Initial tester device
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->Initial_RF_Nonsignaling())
	{
		m_strMsg = "Fail to initial tester device : RF non-signaling";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	m_pITesterDevice->SetMode(1);

	if (! m_pITesterDevice->SetInputPort())
	{
		m_strMsg = "Fail to set RF input port";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetOutputPort())
	{
		m_strMsg = "Fail to set RF output port";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_InitialAnalyzer())
	{
		m_strMsg = "Fail to initial tester device for RF analyzer";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	double dTestFreq = GSM_ULChannelToFrequency(m_iBand, m_iChannel);
	if (! m_pITesterDevice->RF_SetAnalyzerFreq(dTestFreq))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetInputLoss(GetLossByBand(m_iBand)))
	{
		m_strMsg = "Fail to set input loss";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	int iDAC;
	short nPhoneMode;

	if ((iDAC = GetDAC(m_iBand, m_dDesirePower)) == -1)
	{
		m_strMsg = "Fail to get DAC value";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "DAC value is : %d", iDAC);
	TraceLog(MSG_INFO, szTmp);

	nPhoneMode = GetPhoneMode(m_iBand);
	if (! m_pIPhone->QMSL_FTM_SET_MODE(nPhoneMode))
	{
		m_strMsg = "Fail to set phone band with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_CHAN(m_iChannel))
	{
		m_strMsg = "Fail to set phone channel with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_PA_DAC_INPUT((unsigned int)iDAC))
	{
		m_strMsg = "Fail to set DAV value with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TRANSMIT_BURST(0, 0, 0, 0, 0))
	{
		m_strMsg = "Fail to set Tx burst with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set phone Tx ON with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(500);

	bool bPass = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (! m_pITesterDevice->RF_AnalyzerMeasurement(strResponse))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - GSM power : NAN", i + 1);
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		dPower = atof(strResponse.c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dPower);
		m_strMeasured = szTmp;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - GSM power : %.3f", i + 1, dPower);
		TraceLog(MSG_INFO, szTmp);

		if (! InRange(m_dLower, m_dUpper, dPower))
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "GSM power not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "GSM power not in range";
				TraceLog(MSG_ERROR, m_strMsg);
			}
		}
		else
		{
			bPass = true;
			break;
		}

		Sleep(500);
	}

	m_pIPhone->QMSL_FTM_SET_TX_OFF();
	return bPass;
}

int CWireless_GSM_DACTest::GetDAC(int iBand, double dDesiredPower)
{
	int iStep;
	unsigned short unDynRangeIndex;
	unsigned char pRange[8];
	unsigned short unPowerDACIndex;
	unsigned char pDAC[128];

	if (iBand == 1900)
	{
		unDynRangeIndex = GSM1900_DYN_RANGE;
		unPowerDACIndex = GSM1900_TX_DAC;
	}
	else if (iBand == 1800)
	{
		unDynRangeIndex = GSM1800_DYN_RANGE;
		unPowerDACIndex = GSM1800_TX_DAC;
	}
	else if (iBand == 900)
	{
		unDynRangeIndex = GSM900_DYN_RANGE;
		unPowerDACIndex = GSM900_TX_DAC;
	}
	else
	{
		unDynRangeIndex = GSM850_DYN_RANGE;
		unPowerDACIndex = GSM850_TX_DAC;
	}

	// 1. Read the dynamic range, and calculate the correct steps
	if (! m_pIPhone->ReadNV_QMSL(unDynRangeIndex, pRange, 8))
	{
		m_strMsg = "Fail to read GSM dynamic range NV from handset";
		TraceLog(MSG_ERROR, m_strMsg);
		return -1;
	}

	double dMaxPower = (*(int*)pRange) / 100.0;
	iStep = (int)((dMaxPower - m_dDesirePower) / 0.125);

	// 2. Find the DAC
	for (int i = 0; i < 8; ++i)
	{
		unsigned short unIndex = unPowerDACIndex - i;
		if (! m_pIPhone->ReadNV_QMSL(unIndex, pDAC, 128))
		{
			m_strMsg = "Fail to read GSM TX NV from handset";
			TraceLog(MSG_ERROR, m_strMsg);
			return -1;
		}

		for (int j = 126; j >= 0; j -= 2)
		{
			int iValue = *(unsigned short*)(pDAC + j);
			if (iValue <= 0)
				continue;
			else
			{
				int iIndex = j - (iStep * 2);
				if (iIndex >= 0)
					return *(unsigned short*)(pDAC + iIndex);
				else
				{
					iStep = (iIndex + 2) * (-1) / 2;
					break;
				}
			}
		}
	}

	return -1;
}

short CWireless_GSM_DACTest::GetPhoneMode(int iBand)
{
	switch (iBand)
	{
	case 1900:
		return PHONE_MODE_GSM_1900;
	case 1800:
		return PHONE_MODE_GSM_1800;
	case 900:
		return PHONE_MODE_GSM_900;
	default:
		return PHONE_MODE_GSM_850;
	}
}

double CWireless_GSM_DACTest::GetLossByBand(int iBand)
{
	switch (iBand)
	{
	case 1900:
		return GetCableLoss("TX", "WirelessGSM1900", "mid");
	case 1800:
		return GetCableLoss("TX", "WirelessGSM1800", "mid");
	case 900:
		return GetCableLoss("TX", "WirelessGSM900", "mid");
	default:
		return GetCableLoss("TX", "WirelessGSM850", "mid");
	}
}


bool CWireless_GSM_DACTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter PICSName for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("Band") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter Band for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	m_iBand = atoi(paramMap["Band"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter Channel for CWireless_GSM_DACTest" ;
		TraceLog(MSG_ERROR,m_strMsg);
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter RetryTimes for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("DesirePower") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter DesirePower for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_dDesirePower = atof(paramMap["DesirePower"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range for CWireless_GSM_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
