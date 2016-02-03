#include "stdafx.h"
#include "AudioTest.h"
#include "..\Glog\src\windows\glog\logging.h"

#define AUDIO_1K_TONE_FILENAME      _T("AudioData\\1k.wav")
#define AUDIO_COLLECTOR_FILENAME    _T("AudioData\\AudioCollect.exe")

#define AUDIO_COLLECT_RESULT        _T("AudioData\\TestRecord.txt")

#define AUDIO_RETRY					3
#define AUDIO_DATA_DEGREE           3

double d_Log_Vrms[RESULT_UPPERBOUND] = {0};
//double m_RMS_ResultFFT_Thread = 0.0;
//double m_Freq_ResultFFT_Thread = 0.0;

float m_RMS_Result = 0.0;
float m_Freq_Result = 0.0;
float m_DB_Result = 0.0;
float m_DB_Difference = 0.0;

RF_IMPLEMENT_DYNCREATE(CAudioTest)


bool CAudioTest::InitData(std::map<std::string, std::string>& paramMap)
{
	///* PatternIndex */

	if (paramMap.find(std::string("AudioFunction")) != paramMap.end())
	{
		std_AudioFunction = paramMap[std::string("AudioFunction")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter std_AudioFunction for AudioFunction");
		return false;
	}

	if (paramMap.find(std::string("AudioCammand")) != paramMap.end())
	{
		std_AudioCammand = paramMap[std::string("AudioCammand")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter std_AudioCammand for AudioFunction");
		//return false;
	}

	if (paramMap.find(std::string("Audio_RMS")) != paramMap.end())
	{
		std_Audio_RMS = paramMap[std::string("Audio_RMS")];
		sscanf(std_Audio_RMS.c_str(), "%e,%e", &mSPEC_RMS_lower, &mSPEC_RMS_upper);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_RMS for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_FREQ")) != paramMap.end())
	{
		std_Audio_FREQ = paramMap[std::string("Audio_FREQ")];
		sscanf(std_Audio_FREQ.c_str(), "%e,%e", &mSPEC_Freq_lower, &mSPEC_Freq_upper);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_FREQ for AudioFunction");
	}

	//mSPEC_DB_lower
	if (paramMap.find(std::string("Audio_DB_lower")) != paramMap.end())
	{
		std_Audio_DB_lower = paramMap[std::string("Audio_DB_lower")];
		sscanf(std_Audio_DB_lower.c_str(), "%e", &mSPEC_DB_lower);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_FREQ for AudioFunction");
	}


	if (paramMap.find(std::string("Audio_DevID")) != paramMap.end())
	{
		std_Audio_DevID = paramMap[std::string("Audio_DevID")];
		sscanf(std_Audio_DevID.c_str(), "%d", &n_DevID);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_DevID for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_Mic_Line")) != paramMap.end())
	{
		std_Audio_Mic_Line = paramMap[std::string("Audio_Mic_Line")];
		sscanf(std_Audio_Mic_Line.c_str(), "%d,%d", &n_Audio_Mic, &n_Audio_Line);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_MixerVol for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_Vol_Wave")) != paramMap.end())
	{
		std_Audio_Vol_Wave = paramMap[std::string("Audio_Vol_Wave")];
		sscanf(std_Audio_Vol_Wave.c_str(), "%d, %d", &n_Audio_Vol, &n_Audio_Wave);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_Vol_Wave for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_Mrta_Max_Accumlate")) != paramMap.end())
	{
		std_Mrta_Max_Accumlate = paramMap[std::string("Audio_Mrta_Max_Accumlate")];
		sscanf(std_Mrta_Max_Accumlate.c_str(), "%d", &n_Mrta_Max_Accumlate);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Mrta_Max_Accumlate for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_Delay")) != paramMap.end())
	{
		std_Audio_Delay = paramMap[std::string("Audio_Delay")];
		sscanf(std_Audio_Delay.c_str(), "%d", &n_delay);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_Delay for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_AnaDelay")) != paramMap.end())
	{
		std_Audio_AnaDelay = paramMap[std::string("Audio_AnaDelay")];
		sscanf(std_Audio_AnaDelay.c_str(), "%d", &n_Anadelay);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_AnaDelay for AudioFunction");
	}

	if (paramMap.find(std::string("Audio_AnaResult")) != paramMap.end())
	{
		std_Audio_AnaResult = paramMap[std::string("Audio_AnaResult")];
		sscanf(std_Audio_AnaResult.c_str(), "%d", &n_Audio_Result);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Audio_AnaResult for AudioFunction");
	}

	//if (paramMap.find(std::string("Audio_Card_In")) != paramMap.end())
	//{
	//	std_Audio_Card_In = paramMap[std::string("Audio_Card_In")];
	//}
	//else
	//{
	//	TraceLog(MSG_INFO, "Fail to find parameter std_Audio_Card_In for AudioFunction"));
	//}

	//if (paramMap.find(std::string("Audio_Card_Out")) != paramMap.end())
	//{
	//	std_Audio_Card_Out = paramMap[std::string("Audio_Card_Out")];
	//}
	//else
	//{
	//	TraceLog(MSG_INFO, "Fail to find parameter std_Audio_Card_Out for AudioFunction"));
	//}

	return true;
}

bool CAudioTest::LoadAudioCardXMLSetting(CString csTestItem)//liontest
{
	bool bRes = false;
	char szModulePath[MAX_PATH] = {0};
	CString cstrConfigXML = _T("");
	CString csTestItemXML = _T("");
	m_cstrAudioFile.Format("%s%s", g_str_modelName.c_str() , _T("_Audio.xml"));
	cstrConfigXML.Format(_T("C:\\PE_config\\%s\\Setting\\%s"), g_str_modelName.c_str() , m_cstrAudioFile);
	if (::_taccess(cstrConfigXML, 0) == 0)
	{
		m_cstrAudioFile = cstrConfigXML;
		if (m_AudioXML.Load(m_cstrAudioFile) != ERROR_SUCCESS)
		{
			m_strMessage = "m_AudioXML Fail";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
	}

	//XMLNode currendXmlNode = m_AudioXML.SearchNode(_T("//Configuration//Audio_Receiver_PlayWav_Analyze//Audio_Card_In"));
	csTestItemXML.Format(_T("//Configuration//%s//Audio_Card_In"), csTestItem);
	//XMLNode currendXmlNode = m_AudioXML.SearchNode(_T("//Configuration//Audio_Receiver_PlayWav_Analyze//Audio_Card_In"));
	XMLNode currendXmlNode = m_AudioXML.SearchNode(csTestItemXML);
	if (currendXmlNode == NULL)
		return false;

	CString cstrcurrendXmlNode = m_AudioXML.GetNodeText(currendXmlNode);
	if (cstrcurrendXmlNode.IsEmpty())
		return false;
	std_Audio_Card_In = cstrcurrendXmlNode.GetString();

	csTestItemXML.Format(_T("//Configuration//%s//Audio_Card_Out"), csTestItem);
	//currendXmlNode = m_AudioXML.SearchNode(_T("//Configuration//Audio_Receiver_PlayWav_Analyze//Audio_Card_Out"));
	currendXmlNode = m_AudioXML.SearchNode(csTestItemXML);
	if (currendXmlNode == NULL)
		return false;

	cstrcurrendXmlNode = m_AudioXML.GetNodeText(currendXmlNode);
	if (cstrcurrendXmlNode.IsEmpty())
		return false;
	std_Audio_Card_Out = cstrcurrendXmlNode.GetString();

	m_strMessage = "loadAudioCardXMLSetting Successful";
	TraceLog(MSG_ERROR, m_strMessage);
	return true;
}

CAudioTest::CAudioTest()
{
	m_pAudioSpectrum = new AudioSpectrum;
	p_mix = new CMixer;

	mRTA_queue_szie = 10;
	mRTA_max_accumlate = 15;//max 40 time, lion 20130924
	RTAudio::RTAudio_Channel mRTA_channel = RTAudio::LR_Channel;

	mSPEC_RMS_lower = (float)0.01;
	mSPEC_RMS_upper = (float)2.00;
	mSPEC_Freq_lower = 900;
	mSPEC_Freq_upper = 1100;

	n_DevID = 0;
	m_pRTAudio = NULL;

	n_Audio_Result = 0;
	n_Audio_Line = 0;
	n_Audio_Mic = 0;
	n_Audio_Vol = 0;
	n_Audio_Wave = 0;
	n_delay = 0;
	n_Anadelay = 0;
}

CAudioTest::~CAudioTest()
{
	//RunAdbKillServer(false);
	if (m_pAudioSpectrum != NULL)
	{
		delete m_pAudioSpectrum;
		m_pAudioSpectrum = NULL;
	}
	if (p_mix != NULL)
	{
		delete p_mix;
		p_mix = NULL;
	}
	if (m_pRTAudio != NULL)
	{
		delete m_pRTAudio;
		m_pRTAudio = NULL;
	}
}

bool CAudioTest::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(Vibrator_Vibrate_BaseItemCode);
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CAudioTest::Run()
{
	if (m_pIPhone == NULL)
		return false;

	CString AudioFunction = std_AudioFunction.c_str();
	bool b_res = false;
	bReturn_Result = false ;
	m_pAudioSpectrum->LoadAudioDLL();

	if (AudioFunction.Compare("AudioPushWav") == 0)
		b_res = AudioPushWav();

	else if (AudioFunction.Compare("Audio_Receiver_PlayWav_Analyze") == 0)
		b_res = Audio_Receiver_PlayWav_Analyze(AudioFunction);

	else if (AudioFunction.Compare("Audio_Speaker_PalyWav_Analyze") == 0)
		b_res = Audio_Speaker_PalyWav_Analyze(AudioFunction);

	else if (AudioFunction.Compare("Audio_Front_Headset_LoopBack_Analyze") == 0)
		b_res = Audio_Front_Headset_LoopBack_Analyze(AudioFunction);

	else if (AudioFunction.Compare("Audio_Back_Headset_LoopBack_Analyze") == 0)
		b_res = Audio_Back_Headset_LoopBack_Analyze(AudioFunction);

	else if (AudioFunction.Compare("Audio_HeadMic_Headset_LoopBack_Analyze") == 0)
		b_res = Audio_HeadMic_Headset_LoopBack_Analyze(AudioFunction);

	return b_res;
}

bool CAudioTest::PostRun()
{
	return true;
}

bool CAudioTest::AudioPushWav()
{
	//std::string strCMD = " push 1k.wav /data/ ";
	//CString str_command = _T(" push 1k.wav /data/ ");
	for (int i = 0 ; i < RESULT_UPPERBOUND ; i ++) d_Log_Vrms[i] = 0.0;

	char cmdOut[50000] = {0};
	char errMsg[50000] = {0};
	char csCommandShell[50000] = {0};
	strcpy(csCommandShell, " push 1k.wav /data/ ");
	if (!m_pIPhone->ExecAdbCommand(csCommandShell, cmdOut, errMsg))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Audio_Push_Wav_Fail;
		m_strMessage = "Push music fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	return true;
}

bool CAudioTest::Audio_Back_Headset_LoopBack_Analyze(CString csTestItem)
{
	bool b_Res = false;

	InitalResult();
	if (!(LoadAudioCardXMLSetting(csTestItem)))
	{
		m_strMessage = "LoadAudioCardXMLSetting Fail";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/*set line out*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		//if (p_mix->GetMixerDevID(std_Audio_Card_Out.c_str()) >= 0){
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::SPEAKERS , n_Audio_Vol);
			p_mix->SetVolume(CMixer::WAVEOUT , n_Audio_Wave);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_Out < 0");
		b_Res = false;
	}

	/*set line in*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::LineIn  , n_Audio_Line);
			p_mix->SetVolume(CMixer::MICROPHONE , n_Audio_Mic);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_In < 0");
		b_Res = false;
	}

	if (b_Res)
	{
		p_mix->m_MixerDevice = CMixer::LineIn;
		p_mix->SetMixerRecordSource(MIXERLINE_COMPONENTTYPE_SRC_LINE, true);

		mRTA_queue_szie = 10;
		mRTA_max_accumlate = n_Mrta_Max_Accumlate;
		mRTA_channel = RTAudio::LR_Channel;

		char szOutput[FTD_BUF_SIZE] = {0};
		char szInput[FTD_BUF_SIZE] = {0};
		sprintf(szInput, "%s", std_AudioCammand.c_str());
		b_Res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
		if (!b_Res)
		{
			AfxMessageBox(_T("Adb Cmd Fail :Audio_Back_Headset_LoopBack"));
			TraceLog(MSG_ERROR, "Adb Cmd Fail :Audio_Back_Headset_LoopBack");
			return false;
		}

		if (PCPlay())
		{
			StartAnalysisThread();
		}
		else
		{
			TraceLog(MSG_ERROR, "PCPlay Fail :Audio_Back_Headset_LoopBack");
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL Audio_Back_Headset_LoopBack_Analyze PCPlay");
		return false;
	}
	TraceLog(MSG_ERROR, "[Back2Headset] Audio Back2Headset LoopBack command_Analyze Success");

	if (AudioOffLoopBack(_T("off,back,50,headset,50")))
	{
		TraceLog(MSG_ERROR, "[Off LoopBack] Audio OFF Back2Headset LoopBack Success");
	}
	cs_Lower.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", mSPEC_RMS_lower , mSPEC_Freq_lower, mSPEC_DB_lower);
	cs_Upper.Format("Vrms:%.3f, Freq:%.2f", mSPEC_RMS_upper , mSPEC_Freq_upper);
	cs_Measured.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", m_RMS_Result , m_Freq_Result, m_DB_Result);
	m_strLower = cs_Lower.GetString();
	m_strUpper = cs_Upper.GetString();
	m_strMeasured = cs_Measured.GetString();
	d_Log_Vrms[4] = m_RMS_Result;
	if (bReturn_Result && b_Res)
	{
		m_strErrorCode = _T("-");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = " Audio Back2Headset Success";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strErrorCode = FunErr_Loopback_Mic2_To_Headset_Fail;
		m_strResult = "FAIL";
		m_strMessage = "Audio Back2Headset fail!";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		CString cs_Save_Log;
		cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
		saveLog(cs_Save_Log);
		return false;
	}
	FactoryLog();
	CString cs_Save_Log;
	cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
	saveLog(cs_Save_Log);
	return true;
}

bool CAudioTest::Audio_HeadMic_Headset_LoopBack_Analyze(CString csTestItem)
{
	InitalResult();
	if (!LoadAudioCardXMLSetting(csTestItem))
	{
		m_strMessage = "LoadAudioCardXMLSetting Fail";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/*set line out*/
	bool b_Res = false;
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		//if (p_mix->GetMixerDevID(std_Audio_Card_Out.c_str()) >= 0){
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::SPEAKERS , n_Audio_Vol);
			p_mix->SetVolume(CMixer::WAVEOUT , n_Audio_Wave);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_Out < 0");
		b_Res = false;
	}

	/*set line in*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::LineIn  , n_Audio_Line);
			p_mix->SetVolume(CMixer::MICROPHONE , n_Audio_Mic);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_In < 0");
		b_Res = false;
	}

	if (b_Res)
	{
		p_mix->m_MixerDevice = CMixer::LineIn;
		p_mix->SetMixerRecordSource(MIXERLINE_COMPONENTTYPE_SRC_LINE, true);

		mRTA_queue_szie = 10;
		mRTA_max_accumlate = n_Mrta_Max_Accumlate;
		mRTA_channel = RTAudio::LR_Channel;

		TraceLog(MSG_ERROR, "[HeadMic2Headset]Audio_HeadMic_Headset_LoopBack_Analyze PCPlay..");
		char szOutput[FTD_BUF_SIZE] = {0};
		char szInput[FTD_BUF_SIZE]  = {0};
		sprintf(szInput, "%s", std_AudioCammand.c_str());
		b_Res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
		if (!b_Res)
		{
			AfxMessageBox(_T("Adb Cmd Fail :Audio_HeadMic_Headset_LoopBack"));
			TraceLog(MSG_ERROR, "Adb Cmd Fail :Audio_HeadMic_Headset_LoopBack");
			return false;
		}

		if (PCPlay())
		{
			StartAnalysisThread();
		}
		else
		{
			TraceLog(MSG_ERROR, "FAIL HeadMic2Headset PCPlay");
			return false;
		}
		TraceLog(MSG_ERROR, "[HeadMic2Headset] Audio HeadMic2Headset LoopBack command_Analyze Success");

		if (AudioOffLoopBack(_T("off,headmic,50,headset,50")))
		{
			TraceLog(MSG_ERROR, "[Off LoopBack] Audio OFF HeadMic2Headset LoopBack Success");
		}
	}

	cs_Lower.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", mSPEC_RMS_lower , mSPEC_Freq_lower, mSPEC_DB_lower);
	cs_Upper.Format("Vrms:%.3f, Freq:%.2f", mSPEC_RMS_upper , mSPEC_Freq_upper);
	cs_Measured.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", m_RMS_Result , m_Freq_Result, m_DB_Result);
	m_strLower = cs_Lower.GetString();
	m_strUpper = cs_Upper.GetString();
	m_strMeasured = cs_Measured.GetString();
	d_Log_Vrms[4] = m_RMS_Result;
	if (bReturn_Result && b_Res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = " Audio HeadMic2Headset Success";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strErrorCode = FunErr_Loopback_Headmic_To_Headset_Fail;
		m_strResult = "FAIL";
		m_strMessage = "Audio HeadMic2Headset fail!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "FAIL");
		FactoryLog();
		CString cs_Save_Log;
		cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
		saveLog(cs_Save_Log);
		return false;
	}
	FactoryLog();
	return true;
}

bool CAudioTest::Audio_Front_Headset_LoopBack_Analyze(CString csTestItem)
{
	InitalResult();
	if (!(LoadAudioCardXMLSetting(csTestItem)))
	{
		m_strMessage = "LoadAudioCardXMLSetting Fail";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/*set line out*/
	bool b_Res = false;
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::SPEAKERS , n_Audio_Vol);
			p_mix->SetVolume(CMixer::WAVEOUT , n_Audio_Wave);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_Out < 0");
		b_Res = false;
	}

	/*set line in*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::LineIn  , n_Audio_Line);
			p_mix->SetVolume(CMixer::MICROPHONE , n_Audio_Mic);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_In < 0");
		b_Res = false;
	}

	if (b_Res)
	{
		p_mix->m_MixerDevice = CMixer::LineIn;
		p_mix->SetMixerRecordSource(MIXERLINE_COMPONENTTYPE_SRC_LINE, true);


		mRTA_queue_szie = 10;
		mRTA_max_accumlate = n_Mrta_Max_Accumlate;
		mRTA_channel = RTAudio::LR_Channel;

		char szOutput[FTD_BUF_SIZE] = {0};
		char csCommandShell[FTD_BUF_SIZE]  = {0};
		sprintf(csCommandShell, "%s", std_AudioCammand.c_str());
		b_Res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, csCommandShell, szOutput);
		if (!b_Res)
		{
			AfxMessageBox(_T("Adb Cmd Fail :Audio_Front_Headset_LoopBack"));
			TraceLog(MSG_ERROR, "Adb Cmd Fail :Audio_Front_Headset_LoopBack");
			return false;
		}
		TraceLog(MSG_ERROR, "[Front2Headset]Audio_Front_Headset_LoopBack_Analyze PCPlaying..");
		if (PCPlay())
		{
			StartAnalysisThread();
		}
		else
		{
			TraceLog(MSG_ERROR, "FAIL Audio_Front_Headset_LoopBack_Analyze PCPlay");
			return false;
		}
		TraceLog(MSG_ERROR, "[Front2Headset] Audio Front2Headset LoopBack command_Analyze Success");

		if (AudioOffLoopBack(_T("off,front,50,headset,50")))
		{
			TraceLog(MSG_ERROR,  "[Off LoopBack] Audio OFF Front2Headset LoopBack Success");
		}
	}

	cs_Lower.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", mSPEC_RMS_lower , mSPEC_Freq_lower, mSPEC_DB_lower);
	cs_Upper.Format("Vrms:%.3f, Freq:%.2f", mSPEC_RMS_upper , mSPEC_Freq_upper);
	cs_Measured.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", m_RMS_Result , m_Freq_Result, m_DB_Result);
	m_strLower = cs_Lower.GetString();
	m_strUpper = cs_Upper.GetString();
	m_strMeasured = cs_Measured.GetString();
	d_Log_Vrms[4] = m_RMS_Result;
	if (bReturn_Result && b_Res)
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = " Audio Front2Headset Success";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "PASS");
	}
	else
	{
		m_strErrorCode = FunErr_Loopback_Mic_To_Headset_Fail;
		m_strResult = "FAIL";
		m_strMessage = "Audio Front2Headset fail!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "FAIL");
		FactoryLog();
		CString cs_Save_Log;
		cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
		saveLog(cs_Save_Log);
		return false;
	}

	FactoryLog();
	return true;
}


bool CAudioTest::Audio_Receiver_PlayWav_Analyze(CString csTestItem)
{
	InitalResult();
	if (!(LoadAudioCardXMLSetting(csTestItem)))
	{
		m_strMessage = "LoadAudioCardXMLSetting Fail";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/*set line in*/
	bool b_Res = false;
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::LineIn  , n_Audio_Line);
			p_mix->SetVolume(CMixer::MICROPHONE , n_Audio_Mic);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_In < 0");
		b_Res = false;
	}

	/*set line out*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::SPEAKERS , n_Audio_Vol);
			p_mix->SetVolume(CMixer::WAVEOUT , n_Audio_Wave);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_Out < 0");
		b_Res = false;
	}
	if (b_Res)
	{
		p_mix->m_MixerDevice = CMixer::MICROPHONE;
		p_mix->SetMixerRecordSource(MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE, true);

		mRTA_queue_szie = 10;
		mRTA_max_accumlate = n_Mrta_Max_Accumlate;
		mRTA_channel = RTAudio::LR_Channel;

		char szInput[FTD_BUF_SIZE] = {0};
		sprintf(szInput, "%s", std_AudioCammand.c_str());
		char szOutput[FTD_BUF_SIZE] = {0};
		b_Res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
		if (!b_Res)
		{
			AfxMessageBox(_T("Adb Cmd Fail :Audio_Receiver_PlayWav"));
			TraceLog(MSG_ERROR, "Adb Cmd Fail :Audio_Receiver_PlayWav");
			return false;
		}
		StartAnalysisThread();
		TraceLog(MSG_INFO, "[Receiver] Audio Receiver Play Success ");

		if (AudioOff(_T("off,receiver,50,/data/1k.wav")))
			TraceLog(MSG_ERROR, "[Off] Audio OFF Receiver Success");
	}

	cs_Lower.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", mSPEC_RMS_lower , mSPEC_Freq_lower, mSPEC_DB_lower);
	cs_Upper.Format("Vrms:%.3f, Freq:%.2f", mSPEC_RMS_upper , mSPEC_Freq_upper);
	cs_Measured.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", m_RMS_Result , m_Freq_Result, m_DB_Result);
	m_strLower = cs_Lower.GetString();
	m_strUpper = cs_Upper.GetString();
	m_strMeasured = cs_Measured.GetString();
	d_Log_Vrms[0] = m_RMS_Result;
	if (bReturn_Result && b_Res)
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = " Audio  Receiver Success";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "PASS");
	}
	else
	{
		m_strErrorCode = FunErr_Receiver_Play_Fail;
		m_strResult = "FAIL";
		m_strMessage = "Audio Receiver fail!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "FAIL");
		FactoryLog();

		CString cs_Save_Log;
		cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
		saveLog(cs_Save_Log);
		return false;
	}
	FactoryLog();
	return true;
}

bool CAudioTest::Audio_Speaker_PalyWav_Analyze(CString csTestItem)
{
	InitalResult();
	if (!(LoadAudioCardXMLSetting(csTestItem)))
	{
		m_strMessage = "LoadAudioCardXMLSetting Fail";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/*set line in*/
	bool b_Res = false;
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::LineIn  , n_Audio_Line);
			p_mix->SetVolume(CMixer::MICROPHONE , n_Audio_Mic);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_In < 0");
		b_Res = false;
	}

	/*set line out*/
	n_DevID = p_mix->GetMixerDevID(std_Audio_Card_In.c_str());
	if (n_DevID >= 0)
	{
		//if (p_mix->GetMixerDevID(std_Audio_Card_Out.c_str()) >= 0){
		if (p_mix->m_nMixerDestinations >= 0)
		{
			p_mix->m_nWaveInDevID = n_DevID;
			p_mix->SetVolume(CMixer::SPEAKERS , n_Audio_Vol);
			p_mix->SetVolume(CMixer::WAVEOUT , n_Audio_Wave);
			b_Res = true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "FAIL std_Audio_Card_Out < 0");
		b_Res = false;
	}

	if (b_Res)
	{
		p_mix->m_MixerDevice = CMixer::MICROPHONE;
		p_mix->SetMixerRecordSource(MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE, true);

		mRTA_queue_szie = 10;
		mRTA_max_accumlate = n_Mrta_Max_Accumlate;
		mRTA_channel = RTAudio::LR_Channel;

		char szInput[FTD_BUF_SIZE] = {0};
		sprintf(szInput, "%s", std_AudioCammand.c_str());
		char szOutput[FTD_BUF_SIZE] = {0};
		b_Res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
		if (!b_Res)
		{
			AfxMessageBox(_T("Adb Cmd Fail :Audio_Speaker_PalyWav"));
			TraceLog(MSG_ERROR, "Adb Cmd Fail :Audio_Speaker_PalyWav");
			return false;
		}
		StartAnalysisThread();
		TraceLog(MSG_INFO, "[Receiver] Audio Speaker Play Success ");

		if (AudioOff(_T("off,speaker,50,/data/1k.wav")))
		{
			TraceLog(MSG_ERROR, "[Off] Audio OFF Speaker Success");
		}
	}

	cs_Lower.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", mSPEC_RMS_lower , mSPEC_Freq_lower, mSPEC_DB_lower);
	cs_Upper.Format("Vrms:%.3f, Freq:%.2f", mSPEC_RMS_upper , mSPEC_Freq_upper);
	cs_Measured.Format("Vrms:%.3f, Freq:%.2f, Db:%.2f", m_RMS_Result , m_Freq_Result, m_DB_Result);
	m_strLower = cs_Lower.GetString();
	m_strUpper = cs_Upper.GetString();
	m_strMeasured = cs_Measured.GetString();
	d_Log_Vrms[4] = m_RMS_Result;
	if (bReturn_Result)
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = " Audio Speaker Success";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strErrorCode = FunErr_Speaker_Play_Fail;
		m_strResult = "FAIL";
		m_strMessage = "Audio Speaker fail!";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		CString cs_Save_Log;
		cs_Save_Log.Format("%s Vrms, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf", g_strPicasso.c_str(), d_Log_Vrms[0], d_Log_Vrms[1], d_Log_Vrms[2], d_Log_Vrms[3], d_Log_Vrms[4]);
		saveLog(cs_Save_Log);
		return false;
	}
	FactoryLog();
	return true;
}

UINT /*CAudioTest::*/RTAudio_Thread(LPVOID pVar)
{
	CAudioTest* pAudio = (CAudioTest*)pVar;
	pAudio->m_pRTAudio->addLog("[Record thread ] start thread ");
	pAudio->m_pRTAudio->SetChannel(pAudio->mRTA_channel);
	if (pAudio->m_pRTAudio->Start(pAudio->p_mix->m_nWaveInDevID) == -1)
	{
		pAudio->TraceLog(MSG_INFO, "[RTAudio_Thread] Start Fail ");
		return -1;
	}
	pAudio->m_pRTAudio->addLog("[Record thread ] end thread ");
	return 0;
}


UINT FFT_Thread(LPVOID pVar)
{
	int   m_index = 0;
	CAudioTest* pAudio = (CAudioTest*)pVar;
	pAudio->m_pRTAudio->addLog("[FFT thread] begin thread ");
	UINT count = 0;

	while (pAudio->m_pRTAudio->IsStoped() == 0)
	{
		fft_audacity_warp* pFFT = pAudio->m_pRTAudio->GetFirstFFT();
		if (NULL != pFFT)
		{
			//RMS
			pFFT->DoRMS();
			m_RMS_Result = pFFT->GetRMS();
			LOG(_INFO) << "[FFT] RMS= " << m_RMS_Result;

			//Do FFT
			pFFT->Calc();
			m_DB_Result = pFFT->GetPeakFreqFirstDB();
			m_DB_Difference = m_DB_Result - pFFT->GetPeakFreqSecondDB();
			LOG(_INFO) << "[FFT] max_db= " << m_DB_Result;
			LOG(_INFO) << "[FFT] Second_db= " << pFFT->GetPeakFreqSecondDB();
			LOG(_INFO) << "[FFT] DB_Difference= " << m_DB_Difference;
			LOG(_INFO) << "[FFT] max_index= " << pFFT->GetPeakFreqIndex();
			m_index = pFFT->GetPeakFreqIndex() ;
			//m_Freq_Result = (float)(m_index * 10.76); //44100
			m_Freq_Result = (float)(m_index * 11.71); //48000
			LOG(_INFO) << "[FFT] max_frequency= " << m_Freq_Result ;
			LOG(_INFO) << "Audio_Fire_Process" ;

			bReturn_Result = pAudio->checkRMS_Freq_DB(m_RMS_Result, m_Freq_Result, m_DB_Result);
			//pFFT->dump_done_buff_for_draw(strOutputFile);
			//system("wgnuplot.exe www.txt");
			//m_ShowRealPic(true,"output.png");

			//Free
			pAudio->m_pRTAudio->PopFirstFFT();
			pAudio->m_pRTAudio->addLog("[FFT] Complete FFT object ");

			if (bReturn_Result)
				pAudio->m_pRTAudio->Stop();
		}
		Sleep(10);
	}
	pAudio->m_pRTAudio->addLog("[FFT thread] End Analysis ");
	//pAudio->m_pMMIHelper->RenameSpectrumTxt();
	pAudio->m_pRTAudio->addLog("[FFT thread] End Thread ");
	return bReturn_Result;
}

int CAudioTest::StartAnalysisThread()
{
	if (NULL == m_pRTAudio)
	{
		m_pRTAudio = new RTAudio(mRTA_queue_szie, mRTA_max_accumlate);
		TraceLog(MSG_INFO, "[AudioObject] new RTAudio ");
	}

	CWinThread* thRTAudio = AfxBeginThread(RTAudio_Thread , (LPVOID)this);
	CWinThread* thFFT = AfxBeginThread(FFT_Thread , (LPVOID)this);
	TraceLog(MSG_INFO, "[AudioObject] Start Analysis Thread ");

	WaitForSingleObject(thRTAudio->m_hThread, INFINITE);
	WaitForSingleObject(thFFT->m_hThread, INFINITE);
	TraceLog(MSG_INFO, "[AudioObject] thread thRTAudio end , thread thFFT end");

	delete m_pRTAudio;
	m_pRTAudio = NULL;
	TraceLog(MSG_INFO, "[AudioObject] Free m_pRTAudio and set m_pRTAudio = NULL ");
	return 0;
}

bool CAudioTest::checkRMS_Freq_DB(float fRMS_Result, float fFreq_Result, float fDB_Result)
{
	if ((fRMS_Result > mSPEC_RMS_lower && fRMS_Result < mSPEC_RMS_upper)
	    && (fFreq_Result > mSPEC_Freq_lower && fFreq_Result < mSPEC_Freq_upper)
	    && (fDB_Result >= mSPEC_DB_lower))
	{
		return true;
	}
	return false;
}

bool CAudioTest::PCPlay()
{
	TCHAR sz1kTonePath[MAX_PATH] = {0};
	if (!GetFullPath(sz1kTonePath, AUDIO_1K_TONE_FILENAME))
	{
		TraceLog(MSG_INFO, "1kHz Tone does NOT Exist");
		m_strErrorCode = FunErr_Audio_PC_Play_Wav_Fail;
		return false;
	}

	//Get WaveIn devices
	if (p_mix->GetWaveInDevices() <= 0)
	{
		TraceLog(MSG_INFO, "[AudioObject] Can't find any WaveInDevice.");
		return false;
	}

	for (int iCounter = 1; iCounter <= AUDIO_RETRY; ++iCounter)
	{
		if (PlayAudio(sz1kTonePath))
			break ;

		TraceLog(MSG_INFO, "Play 1kHz Tone Fail, retrying...") ;
		Sleep(n_delay);

		if (AUDIO_RETRY == iCounter)
		{
			TraceLog(MSG_INFO, "FunErr_Audio_PC_Play_Wav_Fail");
			return false;
		}
	}
	TraceLog(MSG_INFO, "Audio Front2Headset Play 1K Tone End");
	return true;
}

bool CAudioTest::PlayAudio(const TCHAR* szFilePath)
{
	char szCommand[MAX_PATH] ;
	::ZeroMemory(szCommand, MAX_PATH) ;
	sprintf_s(szCommand, MAX_PATH, "play %s", szFilePath) ;
	DWORD ret = mciSendString(szCommand, NULL, NULL, NULL) ;
	//The list of the MCI error strings and numbers
	//http://msdn.microsoft.com/en-us/library/aa228215(v=vs.60).aspx
	//MCIERR_INVALID_DEVICE_NAME=263
	if (0 != ret)
	{
		LOG(_INFO) << "[AudioTest] PlayPCAudio: fail, ret=" << ret;
		return false ;
	}
	else
	{
		LOG(_INFO) << "[AudioTest] PlayPCAudio: success";
		return true ;
	}
}

bool CAudioTest::AudioOffLoopBack(TCHAR* offCommand)
{
	Sleep(n_delay);
	char szInput[FTD_BUF_SIZE] = {0};
	sprintf(szInput, "%s", offCommand);
	char szOutput[FTD_BUF_SIZE] = {0};
	bool b_res = m_pIPhone->FTD_AUloopbackTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
	Sleep(n_delay);

	if (!b_res)
	{
		m_strResult = "FAIL";
		m_strMessage = "AudioOff fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
	}
	return b_res;
}

bool CAudioTest::AudioOff(TCHAR* offCommand)
{
	Sleep(n_delay);
	char szInput[FTD_BUF_SIZE] = {0};
	sprintf(szInput, "%s", offCommand);
	char szOutput[FTD_BUF_SIZE] = {0};
	bool b_res = false;
	b_res = m_pIPhone->FTD_AUplayWAV(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput);
	Sleep(n_delay);
	if (!b_res)
	{
		m_strResult = "FAIL";
		m_strMessage = "AudioOff fail.";
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	return true;
}

void CAudioTest::saveLog(CString message)
{
	SYSTEMTIME tLocal;
	::GetLocalTime(&tLocal);
	CString Logtime;
	Logtime.Format(_T("%04d%02d%02d-%02d:%02d:%02d"),
	               tLocal.wYear, tLocal.wMonth, tLocal.wDay,
	               tLocal.wHour, tLocal.wMinute, tLocal.wSecond);

	CString pthLog;
	pthLog = _T("D:\\Log\\UI_RD");

	if (::GetFileAttributes(pthLog) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	CStdioFile fLog;
	pthLog = _T("D:\\Log\\UI_RD\\AudioData.CSV");
	UINT nOpenFlags =
	    CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;
	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		message = Logtime + _T("\t") + message + _T("\n");
		fLog.WriteString(message);
		fLog.Close();
	}
}

bool CAudioTest::GetFullPath(TCHAR* fullpath, const TCHAR* chFileName)
{
	TCHAR szCurrentPath[MAX_PATH] = {0};

	::GetModuleFileName(NULL, szCurrentPath, MAX_PATH) ;
	::PathRemoveFileSpec(szCurrentPath) ;
	::PathAppend(szCurrentPath, chFileName) ;

	_tcscpy(fullpath, szCurrentPath) ;
	// Check AudioOutputData.exe exist
	return PathFileExists(fullpath) == TRUE;
}

void CAudioTest::InitalResult()
{
	m_RMS_Result = 0 ;
	m_Freq_Result = 0 ;
	m_DB_Result = 0 ;
}
