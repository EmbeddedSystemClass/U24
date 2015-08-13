
#include "stdafx.h"
#include "AudioFunctionTest.h"
#include "io.h"
#include <tlhelp32.h>

RF_IMPLEMENT_DYNCREATE(AudioFT)

bool AudioFT::InitData(std::map<std::string, std::string>& paramMap)
{
	//-- CommandDelay --//
	std::string str_CommandDelay;
	if (paramMap.find("CommandDelay") != paramMap.end())
	{
		str_CommandDelay = paramMap[std::string("CommandDelay")];
		m_i_CommandDelay = CStr::StrToInt(str_CommandDelay);
	}
	else
	{
		m_i_CommandDelay = 500;
		TraceLog(MSG_INFO, _T("CommandDelay set default 500ms."));
	}

	//-- DeviceDelay --//
	std::string str_DeviceDelay;
	if (paramMap.find("DeviceDelay") != paramMap.end())
	{
		str_DeviceDelay = paramMap[std::string("DeviceDelay")];
		m_i_DeviceDelay = CStr::StrToInt(str_DeviceDelay);
	}
	else
	{
		m_i_DeviceDelay = 500;
		TraceLog(MSG_INFO, _T("DeviceDelay set default 500ms."));
	}

	//-- Audio action --//
	std::string str_Action;
	if (paramMap.find("AudioAction") == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Get AudioAction Fail."));
		return false;
	}
	str_Action = paramMap[std::string("AudioAction")];
	m_i_Action = CStr::StrToInt(str_Action);

	//-- Spec --//
	std::string pstrRange;
	std::string str_JackDetection;
	switch (m_i_Action)
	{
	case 12:
		if (paramMap.find(std::string("SPKLTestSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("SPKLTestSpec")];
			m_obj_SPKLTestSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get SPKLTestSpec Fail."));
			return false;
		}

		if (paramMap.find(std::string("SPKL_F_COMSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("SPKL_F_COMSpec")];
			m_obj_SPKL_F_COMSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get SPKL_F_COMSpec Fail."));
			return false;
		}
		break;

	case 13:
		if (paramMap.find(std::string("EARTestSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("EARTestSpec")];
			m_obj_EARTestSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get EARTestSpec Fail."));
			return false;
		}

		if (paramMap.find(std::string("EAR_F_COMSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("EAR_F_COMSpec")];
			m_obj_EAR_F_COMSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get EAR_F_COMSpec Fail."));
			return false;
		}
		break;

	case 14:

		if (paramMap.find("JackDetection") != paramMap.end())
		{
			str_JackDetection = paramMap[std::string("JackDetection")];
			m_i_JackDetection = CStr::StrToInt(str_JackDetection);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Fail to set JackDetection."));
			return false;
		}

		if (paramMap.find(std::string("HPLTest")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("HPLTest")];
			m_obj_HPLSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get HPL Spec Fail."));
			return false;
		}

		if (paramMap.find(std::string("HPRTest")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("HPRTest")];
			m_obj_HPRSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get HPR Spec Fail."));
			return false;
		}

		break;

	case 16:

		if (paramMap.find(std::string("AU_MIC1_BIAS_Spec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("AU_MIC1_BIAS_Spec")];
			m_obj_AU_MIC_BIAS_1Spec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get AU_MIC1_BIAS Spec Fail."));
			return false;
		}

		if (paramMap.find(std::string("MIC1_EARSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("MIC1_EARSpec")];
			m_obj_MIC1_EARSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get MIC1_EAR+F Spec Fail."));
			return false;
		}
		break;

	case 17:

		if (paramMap.find(std::string("AU_MIC3_BIAS_Spec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("AU_MIC3_BIAS_Spec")];
			m_obj_AU_MIC_BIAS_3Spec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get AU_MIC3_BIAS Spec Fail."));
			return false;
		}

		if (paramMap.find(std::string("MIC3_EARSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("MIC3_EARSpec")];
			m_obj_MIC3_EARSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get MIC3_EAR+F Spec Fail."));
			return false;
		}
		break;
	case 18:
		if (paramMap.find(std::string("AU_HEAD_MIC_BIAS_Spec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("AU_HEAD_MIC_BIAS_Spec")];
			m_obj_AU_HEAD_MIC_BIAS_Spec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get AU_HEAD_MIC_BIAS Spec Fail."));
			return false;
		}

		if (paramMap.find(std::string("HEAD_MIC_EARSpec")) != paramMap.end())
		{
			pstrRange = paramMap[std::string("HEAD_MIC_EARSpec")];
			m_obj_AU_HEAD_MIC_EARSpec.SetRange(pstrRange);
		}
		else
		{
			TraceLog(MSG_INFO, _T("Get HEAD_MIC_EAR+F Spec Fail."));
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}

bool AudioFT::PreRun()
{
	return ResetData();
}

bool AudioFT::Run()
{
	bool b_res = false;
	switch (m_i_Action)
	{
	case 12:
		b_res = Speaker_Test();
		break;
	case 13:
		b_res = Handset_Ear_Receiver_Test();
		break;
	case 14:
		b_res = Headset_Test();
		break;
	case 16:
		b_res = Front_MIC_Test();
		break;
	case 17:
		b_res = Back_MIC_Test();
		break;
	case 18:
		b_res = Head_MIC_Test();
		break;
	default:
		break;
	}
	return b_res;
}

bool AudioFT::PostRun()
{
	return true;
}

bool AudioFT::RecordWAV(char* p_Switch, char* p_Testpart, char* p_Volume, char* p_path)
{
	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	strcpy(sz_inBuffer, p_Switch);
	strcat(sz_inBuffer, ",");
	strcat(sz_inBuffer, p_Testpart);
	strcat(sz_inBuffer, ",");
	strcat(sz_inBuffer, p_Volume);
	if (strlen(p_path) != 0)
	{
		strcat(sz_inBuffer, ",");
		strcat(sz_inBuffer, p_path);
	}
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = p_Switch;
		m_strMessage = m_strMessage + " " + p_Testpart + " record wav fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	return true;
}

bool AudioFT::PlayWAV(char* p_Switch, char* p_Testpart, char* p_Volume, char* p_path)
{
	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	strcpy(sz_inBuffer, p_Switch);
	strcat(sz_inBuffer, ",");
	strcat(sz_inBuffer, p_Testpart);
	strcat(sz_inBuffer, ",");
	strcat(sz_inBuffer, p_Volume);
	if (strlen(p_path) != 0)
	{
		strcat(sz_inBuffer, ",");
		strcat(sz_inBuffer, p_path);
	}
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		if (strcmp(p_Testpart, "speaker") == 0)
		{
			p_Testpart = "speaker";
		}
		else if (strcmp(p_Testpart, "receiver") == 0)
		{
			p_Testpart = "receiver";
		}
		else if (strcmp(p_Testpart, "headset") == 0)
		{
			p_Testpart = "headset";
		}
		else
		{
			p_Testpart = "Unknown device";
		}
		m_strMessage = p_Switch;
		m_strMessage = m_strMessage + " " + p_Testpart +  " play wav fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	return true;
}

// Detroit test item #12 //
bool AudioFT::Speaker_Test()
{
	if (!PlayWAV("on", "speaker", "75"))
		return false;
	Sleep(m_i_CommandDelay);

	// SPKL+,D0~2[000],D5[0] //
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	int nReTryCount = 2;
	double dw_value = 0;
	bool isRetryPass = true;
	do
	{
		char sz_SPKLTest[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_SPKLTest))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- SPKL+ Compare Spec ---//
		m_strMeasured = sz_SPKLTest;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_SPKLTestSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_SPKLTestSpec.GetMin());
		if (!m_obj_SPKLTestSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Speaker SPKL+ fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "Speaker test in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SPEAKER", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Speaker_R_Test_Fail;
		m_strMessage = "Speaker test is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SPEAKER", m_strMeasured);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	// SPKL_F_COM,D0~2[001],D5[0] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	nReTryCount = 2;
	do
	{
		char sz_SPKL_F_COM[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_SPKL_F_COM))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- SPKL_F_COM Compare Spec ---//
		m_strMeasured = sz_SPKL_F_COM;
		dw_value = 0;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_SPKL_F_COMSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_SPKL_F_COMSpec.GetMin());
		if (!m_obj_SPKL_F_COMSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Speaker SPKL_F_COM fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "SPEAKER_F_COM in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SPEAKER_F_COM", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Speaker_R_Test_Fail;
		m_strMessage = "SPEAKER_F_COM is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SPEAKER_F_COM", m_strMeasured);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	if (!PlayWAV("off", "speaker", "75"))
		return false;
	Sleep(m_i_CommandDelay);
	return true;
}
// Detroit test item #13 //
bool AudioFT::Handset_Ear_Receiver_Test()
{
	if (!PlayWAV("on", "receiver", "75"))
		return false;
	Sleep(m_i_CommandDelay);

	// EAR+F,D0~2[010],D5[1] //
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	int nReTryCount = 2;
	double dw_value = 0;
	bool isRetryPass = true;
	do
	{
		char sz_EARTest[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_EARTest))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- EAR+F Compare Spec ---//
		m_strMeasured = sz_EARTest;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_EARTestSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_EARTestSpec.GetMin());
		if (!m_obj_EARTestSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Receiver EAR+F fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "EAR+F Test in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("EAR_RECEIVER", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Receiver_Test_Fail;
		m_strMessage = "EAR+F Test is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("EAR_RECEIVER", m_strMeasured);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	// EAR_F_COM,D0~2[011],D5[1] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	nReTryCount = 2;
	do
	{
		char sz_EAR_F_COM[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_EAR_F_COM))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- EAR_F_COM Compare Spec ---//
		m_strMeasured = sz_EAR_F_COM;
		dw_value = 0;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_EAR_F_COMSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_EAR_F_COMSpec.GetMin());
		if (!m_obj_EAR_F_COMSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Receiver EAR_F_COM fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "EAR_F_COM in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("EAR_RECEIVER_F_COM", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Receiver_Test_Fail;
		m_strMessage = "EAR_F_COM is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("EAR_RECEIVER_F_COM", m_strMeasured);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	if (!PlayWAV("off", "receiver", "75"))
		return false;
	Sleep(m_i_CommandDelay);
	return true;
}
// Detroit test item #14 //
bool AudioFT::Headset_Test()
{
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUdetectHeadset(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "JackDetection Fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	//--- Compare JackDetection Value ---//
	m_strMeasured = sz_outBuffer;
	int i_JackDetectValue = CStr::StrToInt(sz_outBuffer);
	if (m_i_JackDetection != i_JackDetectValue)
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_Headset_Fail;
		m_strMessage = "JackDetection Fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	//--- Compare Value End ---//

	if (!PlayWAV("on", "headset", "75"))
		return false;
	Sleep(m_i_CommandDelay);

	// HPR,D0~2[000],D5[1] //
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	int nReTryCount = 2;
	double dw_value = 0;
	bool isRetryPass = true;
	do
	{
		char sz_HPR[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_HPR))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- HPR Compare Spec ---//
		m_strMeasured = sz_HPR;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_HPRSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_HPRSpec.GetMin());
		if (!m_obj_HPRSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Headset HPR fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "HPR in the spec.";
		SetPICSData("RIGHT_HEADSET", m_strMeasured);
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Headset_R_Test_Fail;
		m_strMessage = "HPR is not in the spec.";
		SetPICSData("RIGHT_HEADSET", m_strMeasured);
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	// HPL,D0~2[001],D5[1] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	nReTryCount = 2;
	do
	{
		char sz_HPL[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_HPL))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- HPL Compare Spec ---//
		m_strMeasured = sz_HPL;
		dw_value = 0;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_HPLSpec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_HPLSpec.GetMin());
		if (!m_obj_HPLSpec.InRange(dw_value))
		{
			TraceLog(MSG_INFO, "Headset HPL fail retry.");
			isRetryPass = false;
		}
		else
		{
			isRetryPass = true;
			break;
		}
		Sleep(1000);
		nReTryCount--;
	} while (!isRetryPass && nReTryCount >= 0);

	if (isRetryPass)
	{
		m_strResult = "PASS";
		m_strMessage = "HPL in the spec.";
		SetPICSData("LEFT_HEADSET", m_strMeasured);
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Headset_L_Test_Fail;
		m_strMessage = "HPL is not in the spec.";
		SetPICSData("LEFT_HEADSET", m_strMeasured);
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	//--- Compare Spec End ---//

	if (!PlayWAV("off", "headset", "75"))
		return false;
	Sleep(m_i_CommandDelay);
	return true;
}

// Detroit test item #16 //
bool AudioFT::Front_MIC_Test()
{
	m_strErrorCode = FunErr_Mic_Test_Fail;

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "on,front,80,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Front MIC open loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	// AU_MIC_BIAS_MAIN,D0~2[100],D5[0] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	char sz_AU_MIC_BIAS[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 1, sz_AU_MIC_BIAS))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}

	//--- AU_MIC_BIAS Compare Spec ---//
	m_strMeasured = sz_AU_MIC_BIAS;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_AU_MIC_BIAS_1Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_AU_MIC_BIAS_1Spec.GetMin());
	if (!m_obj_AU_MIC_BIAS_1Spec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Mic_Bias_Fail;
		m_strMessage = "AU_MIC_BIAS_1 is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("FRONT_MIC_BIAS", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "AU_MIC_BIAS_1 Test in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("FRONT_MIC_BIAS", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//

	// MIC1_EAR+F,D0~2[010],D5[1] //
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	char sz_MIC_EAR[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_MIC_EAR))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "off,front,80,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Front MIC close loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	//-- To stop music --//
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}

	//---MIC_EAR+F Compare Spec ---//
	m_strMeasured = sz_MIC_EAR;
	dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_MIC1_EARSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_MIC1_EARSpec.GetMin());
	if (!m_obj_MIC1_EARSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Mic_Test_Fail;
		m_strMessage = "FRONT_MIC is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("FRONT_MIC", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "FRONT_MIC in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("FRONT_MIC", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	Sleep(m_i_CommandDelay);
	return true;
}

// Detroit test item #17 //
bool AudioFT::Back_MIC_Test()
{
	m_strErrorCode = FunErr_Mic2_Test_Fail;

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "on,back,95,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Back MIC open loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	/*
	// AU_MIC_BIAS_SUB,D0~2[101],D5[0] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit0";
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1";
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2";
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5";
		return false;
	}
	Sleep(m_i_DeviceDelay);

	char sz_AU_MIC_BIAS[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 1, sz_AU_MIC_BIAS))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol";
		return false;
	}

	//--- AU_MIC_BIAS Compare Spec ---//
	m_strMeasured = sz_AU_MIC_BIAS;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_AU_MIC_BIAS_3Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_AU_MIC_BIAS_3Spec.GetMin());
	if (!m_obj_AU_MIC_BIAS_3Spec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Mic2_Bias_Fail;
		m_strMessage = "AU_MIC_BIAS_3 Test is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("BACK_MIC_BIAS", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "AU_MIC_BIAS_3 Test in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("BACK_MIC_BIAS", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	*/

	// MIC3_EAR+F,D0~2[010],D5[1]  //
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	char sz_MIC_EAR[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 0, sz_MIC_EAR))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "off,back,95,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Back MIC close loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	//---MIC_EAR+F Compare Spec ---//
	m_strMeasured = sz_MIC_EAR;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_MIC3_EARSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_MIC3_EARSpec.GetMin());
	if (!m_obj_MIC3_EARSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strMessage = "BACK_MIC is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("BACK_MIC_EAR", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "BACK_MIC in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("BACK_MIC_EAR", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	Sleep(m_i_CommandDelay);
	return true;
}

// Detroit test item #18 //
bool AudioFT::Head_MIC_Test()
{
	m_strErrorCode = FunErr_Headset_Mic_Fail;

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "on,headmic,95,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Head MIC open loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	// AU_MIC_BIAS_SUB,D0~2[101],D5[0] //
	if (!m_pIPhone->USB4702_SetDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	Sleep(m_i_DeviceDelay);

	char sz_AU_MIC_BIAS[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 1, sz_AU_MIC_BIAS))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}
	if (!m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, "off,headmic,95,receiver,75", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Head MIC close loopback fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	//--- AU_MIC_BIAS Compare Spec ---//
	m_strMeasured = sz_AU_MIC_BIAS;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_AU_HEAD_MIC_BIAS_Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_AU_HEAD_MIC_BIAS_Spec.GetMin());
	if (!m_obj_AU_HEAD_MIC_BIAS_Spec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strMessage = "AU_HEAD_MIC_BIAS Test is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("HEAD_MIC_BIAS", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "AU_HEAD_MIC_BIAS Test in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("HEAD_MIC_BIAS", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	Sleep(m_i_CommandDelay);
	return true;
}

bool AudioFT::ResetData()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}
