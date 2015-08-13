#include "stdafx.h"
#include "Wireless_WCDMA_DACTest.h"
#include "stdlib.h"
#include "QLib_Defines.h"


RF_IMPLEMENT_DYNCREATE(CWireless_WCDMA_DACTest)


#define WCDMA_TX_POWER_BC1		2010
#define WCDMA_TX_POWER_BC2		2031
#define WCDMA_TX_POWER_BC4		4052
#define WCDMA_TX_POWER_BC5		2056
#define WCDMA_TX_POWER_BC8		3707

#define WCDMA_TX_PDM_BC1		3972
#define WCDMA_TX_PDM_BC2		3980
#define WCDMA_TX_PDM_BC4		4115
#define WCDMA_TX_PDM_BC5		3976
#define WCDMA_TX_PDM_BC8		3984


bool CWireless_WCDMA_DACTest::Run()
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
		FactoryLog(false, "WCDMA TX Power Test", ERR_QLIB_PHONE_CONNECT, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm",
			"Fail to connect phone with QMSL");
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, "0.000");

		return false;
	}

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "WCDMA TX Power Test", "Pass", szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless WCDMA power PASS");
		SetPICSDate(m_strPICSName, "PASS");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}
	else
	{
		FactoryLog(false, "WCDMA TX Power Test", FunErr_WirelessWCDMA_Test_OutPower_OutRange, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}

	if (! m_pIPhone->Disconnect_QMSL())
	{
		FactoryLog(false, "WCDMA TX Power Test", ERR_QLIB_DisconnectServer, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm",
			"Fail to disconnect phone with QMSL");
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, "0.000");

		return false;
	}

	return bRes;
}

bool CWireless_WCDMA_DACTest::MainFunction()
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

	double dTestFreq = WCDMAUplinkChannelToFrequency(m_iBand, m_iChannel);
	if (! m_pITesterDevice->RF_SetAnalyzerFreq(dTestFreq))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetInputLoss(GetLossByBand(m_iBand)))
		return false;

	int iPDM;
	short nPhoneMode;

	if ((iPDM = GetPDM(m_iBand, m_dDesirePower)) == -1)
	{
		m_strMsg = "Fail to get PDM value";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "PDM value is : %d", iPDM);
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

	if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set phone Tx ON with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_CDMA_CW_WAVEFORM(0))
	{
		m_strMsg = "Fail to set phone to generate WCDMA waveform with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_PA_RANGE(3))
	{
		m_strMsg = "Fail to set phone PA range with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_PDM(iPDM))
	{
		m_strMsg = "Fail to set phone PDM with QMSL";
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
			sprintf_s(szTmp, 255, "%0d - WCDMA power : NAN", i + 1);
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		dPower = atof(strResponse.c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%.3f", dPower);
		m_strMeasured = szTmp;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WCDMA power : %.3f", i + 1, dPower);
		TraceLog(MSG_INFO, szTmp);

		if (! InRange(m_dLower, m_dUpper, dPower))
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "WCDMA power not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "WCDMA power not in range";
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

int CWireless_WCDMA_DACTest::GetPDM(int iBand, double dDesiredPower)
{
	int iPDM;
	unsigned short unPowerIndex;
	unsigned short unPDMIndex;
	unsigned char pData[64];

	if (iBand == 2100)
	{
		unPowerIndex = WCDMA_TX_POWER_BC1;
		unPDMIndex = WCDMA_TX_PDM_BC1;
	}
	else if (iBand == 1900)
	{
		unPowerIndex = WCDMA_TX_POWER_BC2;
		unPDMIndex = WCDMA_TX_PDM_BC2;
	}
	else if (iBand == 1700)
	{
		unPowerIndex = WCDMA_TX_POWER_BC4;
		unPDMIndex = WCDMA_TX_PDM_BC4;
	}
	else if (iBand == 850)
	{
		unPowerIndex = WCDMA_TX_POWER_BC5;
		unPDMIndex = WCDMA_TX_PDM_BC5;
	}
	else
	{
		unPowerIndex = WCDMA_TX_POWER_BC8;
		unPDMIndex = WCDMA_TX_PDM_BC8;
	}

	if (! m_pIPhone->ReadNV_QMSL(unPowerIndex, pData, 64))
	{
		m_strMsg = "Fail to read WCDMA TX NV from handset";
		TraceLog(MSG_ERROR, m_strMsg);
		return -1;
	}

	int iIndex;
	int iDesiredAGC = (int)((dDesiredPower + 57.0) * (1024.0 / 85.3) + 0.5);
	int iWinner = 100000;

	for (int i = 0; i < 64; i += 2)
	{
		int iValue = *(unsigned short*)(pData + i);
		int iDelta = abs(iValue - iDesiredAGC);

		if (iDelta < iWinner)
		{
			iWinner = iDelta;
			iIndex = i;
		}
	}

	if (! m_pIPhone->ReadNV_QMSL(unPDMIndex, pData, 64))
	{
		m_strMsg = "Fail to read WCDMA PDM NV from handset";
		TraceLog(MSG_ERROR, m_strMsg);
		return -1;
	}

	iPDM = *(unsigned short*)(pData + iIndex);
	return iPDM;
}

short CWireless_WCDMA_DACTest::GetPhoneMode(int iBand)
{
	switch (iBand)
	{
	case 2100:
		return PHONE_MODE_WCDMA_IMT;
	case 1900:
		return PHONE_MODE_WCDMA_1900B;
	case 1700:
		return PHONE_MODE_WCDMA_BC4;
	case 850:
		return PHONE_MODE_WCDMA_800;
	default:
		return PHONE_MODE_WCDMA_BC8;
	}
}

double CWireless_WCDMA_DACTest::GetLossByBand(int iBand)
{
	switch (iBand)
	{
	case 2100:
		return GetCableLoss("TX", "WirelessWCDMA2100", "mid");
	case 1900:
		return GetCableLoss("TX", "WirelessWCDMA1900", "mid");
	case 1700:
		return GetCableLoss("TX", "WirelessWCDMA1700", "mid");
	case 850:
		return GetCableLoss("TX", "WirelessWCDMA850", "mid");
	default:
		return GetCableLoss("TX", "WirelessWCDMA900", "mid");
	}
}


bool CWireless_WCDMA_DACTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter PICSName for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("Band") == paramMap.end())
	{
		m_strMsg =  "Fail to find parameter Band for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg);
		return false;
	}
	m_iBand = atoi(paramMap["Band"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter Channel for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter RetryTimes for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("DesirePower") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter DesirePower for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_dDesirePower = atof(paramMap["DesirePower"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		m_strMsg = "Fail to find parameter Range for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		m_strMsg = "Fail to find parameter Range for CWireless_WCDMA_DACTest";
		TraceLog(MSG_ERROR,m_strMsg );
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
