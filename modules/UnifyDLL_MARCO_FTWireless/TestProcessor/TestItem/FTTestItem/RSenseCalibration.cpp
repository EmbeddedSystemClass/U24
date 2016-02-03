/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .cpp file of CRSenseCalibration object.
 * FileName   : CRSenseCalibration.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/07/29
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/07/29    Evonne.Hsieh            Create for Saporro BB test
 ********************************************************************************/

#include "stdafx.h"
#include "RSenseCalibration.h"


RF_IMPLEMENT_DYNCREATE(CRSenseCalibration)


bool CRSenseCalibration::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = FunErr_Battery_RSense_Calibration_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = CStr::FloatToStr((float)m_rp_RSenseRange.GetMax());
	m_strLower = CStr::FloatToStr((float)m_rp_RSenseRange.GetMin());
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CRSenseCalibration::Run()
{
	bool b_res = false;
	std::string str_msg;

	b_res = RSenseCalibration();

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "RSense calibration PASS.";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		m_strResult = "FAIL";
		str_msg = "RSense calibration FAIL.";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	}

	SetPICSData(_T("RSENSE_CALIBRATION"), m_strMeasured);
	FactoryLog();

	return b_res;
}

bool CRSenseCalibration::PostRun()
{
	return true;
}

bool CRSenseCalibration::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str_WaitTime;
	if (paramMap.find(std::string("WaitTime")) != paramMap.end())
	{
		str_WaitTime = paramMap[std::string("WaitTime")];
		m_i_WaitTime = CStr::StrToInt(str_WaitTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter 'WaitTime' for 'CRSenseCalibration'");
		return false;
	}

	std::string str_Bias;
	if (paramMap.find(std::string("Bias")) != paramMap.end())
	{
		str_Bias = paramMap[std::string("Bias")];
		m_i_Bias = CStr::StrToInt(str_Bias);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter 'Bias' for 'CRSenseCalibration'");
		return false;
	}

	std::string str_Count;
	if (paramMap.find(std::string("RetryCount")) != paramMap.end())
	{
		str_Count = paramMap[std::string("RetryCount")];
		m_i_RetryCount = CStr::StrToInt(str_Count);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter 'RetryCount' for 'CRSenseCalibration'");
		return false;
	}

	// Current_Spec /
	std::string str_RSenseSpec;
	if (paramMap.find(std::string("RSenseSpec")) != paramMap.end())
	{
		str_RSenseSpec = paramMap[std::string("RSenseSpec")];
		m_rp_RSenseRange.SetRange(str_RSenseSpec);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Current_Spec for ChargeCurrent");
		return false;
	}

	//sample number
	if (paramMap.find(std::string("SampleNum")) != paramMap.end())
	{
		m_str_SampleNum = paramMap[std::string("SampleNum")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter 'SampleNum' for 'CRSenseCalibration'");
		return false;
	}

	return true;
}

bool CRSenseCalibration::RSenseCalibration()
{
	bool b_res = true;

	char sz_OutData[FTD_BUF_SIZE] = {0};
	if (!(m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "1", sz_OutData)))
	{
		TraceLog(MSG_INFO, "Fail to set Detroit_PWRchargerOn on.");
		return false;
	}
	Sleep(3000);

	//restore initial rsense value to device
	if (!(m_pIPhone->FTD_PWRWriteRsenseData(m_nFtdPort, m_nFtdTimeOut, _T("10000"), sz_OutData)))
	{
		TraceLog(MSG_INFO, "Fail to write bms rsense data 10000.");
		return false;
	}
	TraceLog(MSG_INFO, "Success to write bms rsense data 10000.");

	Sleep(m_i_WaitTime);
	char sz_InData[FTD_BUF_SIZE] = {0};
	memcpy(sz_InData, m_str_SampleNum.c_str(), m_str_SampleNum.length());
	memset(sz_OutData, 0, FTD_BUF_SIZE);
	double d_I_Adc = 0;
	if (b_res = m_pIPhone->FTD_PWRAverageGaugeCurrent(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData))
	{
		TraceLog(MSG_INFO, "Success to get charge current.");
		d_I_Adc = atof(sz_OutData);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to set Detroit_PWRAverageGaugeCurrent.");
	}

	char szMsgBuffer[4096] = {0};
	double d_I_PowerSupply;
	double d_Gain = 0;
	double d_RSense = 10000;
	if (b_res)
	{
		// get initial power supply value
		if (!(b_res = m_pIPS->GetCurrent(d_I_PowerSupply)))
		{
			TraceLog(MSG_INFO, "Fail to get power supply current.");
			return false;
		}

		// ³æ¦ì´«ºâ
		d_I_Adc /= 1000;
		d_I_PowerSupply *= 1000;
		d_Gain = d_I_Adc / d_I_PowerSupply;
		d_RSense *= d_Gain;
		sprintf(szMsgBuffer, "Initial Iadc=%f mA,Ipowersupply=%f mA,Gain=%f", d_I_Adc, d_I_PowerSupply, d_Gain);
		TraceLog(MSG_INFO, szMsgBuffer);

		do {
			// start calibration
			memset(sz_InData, 0, FTD_BUF_SIZE);
			sprintf(sz_InData, "%d", (int)d_RSense);
			sprintf(szMsgBuffer, "Test %d: Rsense=%f", m_i_RetryCount, d_RSense);
			TraceLog(MSG_INFO, szMsgBuffer);
			if (b_res = m_pIPhone->FTD_PWRWriteRsenseData(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData))
			{
				sprintf(szMsgBuffer, "Test %d: Success to write bms rsense data. Input value=%s", m_i_RetryCount, sz_InData);
				TraceLog(MSG_INFO, szMsgBuffer);
				if (b_res = m_pIPS->GetCurrent(d_I_PowerSupply))
				{
					d_I_PowerSupply *= 1000;
					memset(sz_InData, 0, FTD_BUF_SIZE);
					memcpy(sz_InData, m_str_SampleNum.c_str(), m_str_SampleNum.length());
					sprintf(szMsgBuffer, "Sample number=%s", sz_InData);
					TraceLog(MSG_INFO, szMsgBuffer);
					Sleep(m_i_WaitTime);
					if (b_res = m_pIPhone->FTD_PWRAverageGaugeCurrent(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData))
					{
						d_I_Adc = atof(sz_OutData);
						d_I_Adc /= 1000;
						sprintf(szMsgBuffer, "Test %d: Iadc=%f mA,Ipowersupply=%f mA", m_i_RetryCount, d_I_Adc, d_I_PowerSupply);
						TraceLog(MSG_INFO, szMsgBuffer);
						double diff = fabs(d_I_Adc - d_I_PowerSupply);
						sprintf(szMsgBuffer, "Diff=%f", diff);
						TraceLog(MSG_INFO, szMsgBuffer);
						if (diff <= 30)
						{
							if(m_rp_RSenseRange.InRange(d_RSense)) {
								sprintf(szMsgBuffer, "Test %d: RSense is in range", m_i_RetryCount);
								TraceLog(MSG_INFO, szMsgBuffer);
								break;
							}
							else
							{
								sprintf(szMsgBuffer, "Test %d: RSense is not in range", m_i_RetryCount);
								TraceLog(MSG_INFO, szMsgBuffer);
								b_res = false;
							}
						}
						else
						{
							if (m_i_RetryCount != 1)
							{
								if (d_I_Adc > d_I_PowerSupply)
								{
									sprintf(szMsgBuffer, "Test %d: Rsense minu %d", m_i_RetryCount, m_i_Bias);
									TraceLog(MSG_INFO, szMsgBuffer);
									d_RSense -= (double)m_i_Bias;
								}
								else
								{
									sprintf(szMsgBuffer, "Test %d: Rsense plus %d", m_i_RetryCount, m_i_Bias);
									TraceLog(MSG_INFO, szMsgBuffer);
									d_RSense += (double)m_i_Bias;
								}
							}
							else
							{
								b_res = false;
								break;
							}
						}
					}
					else
					{
						TraceLog(MSG_INFO, "Fail to set Detroit_PWRAverageGaugeCurrent after calibration.");
						break;
					}
				}
				else
				{
					TraceLog(MSG_INFO, "Fail to get power supply current.");
					break;
				}
			}
			else
			{
				TraceLog(MSG_INFO, "Fail to set Detroit_PWRWriteRsenseData.");
				break;
			}
			m_i_RetryCount --;
		} while(m_i_RetryCount > 0);
	}
	else
	{
		TraceLog(MSG_INFO, "Calculate initial gain fail.");
	}
	m_strMeasured = CStr::DblToStr(d_RSense);
	if (!(m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "0", sz_OutData)))
	{
		TraceLog(MSG_INFO, "Fail to set Detroit_PWRchargerOn off.");
		return false;
	}
	return b_res;
}
