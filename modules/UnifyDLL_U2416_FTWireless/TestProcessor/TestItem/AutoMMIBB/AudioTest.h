/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAudioTest  object.
 * FileName   : BostonKeyTest.h
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.05.29
 ********************************************************************************/

#ifndef _C_AUDIO_TEST_H_
#define _C_AUDIO_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "mixer.h"
#include "RTAudio.h"
#include "AudioSpectrum.h"
#include "../../../CommonUtility/XML/MyMSXML.h"


#define AUDIO_1K_TONE_FILENAME      _T("AudioData\\1k.wav")
#define AUDIO_COLLECTOR_FILENAME    _T("AudioData\\AudioCollect.exe")
#define AUDIO_COLLECT_RESULT        _T("AudioData\\TestRecord.txt")

//#define AUDIO_RETRY					1
#define AUDIO_DATA_DEGREE           3
#define RESULT_UPPERBOUND           10
bool  m_firemsg = true;
bool  bReturn_Result=false;
static string m_txtpath;


class CAudioTest  : public ITestItem
{
RF_DECLARE_DYNCREATE(CAudioTest)


// Data members
public:
	
protected:
	bool GetFullPath (TCHAR* fullpath, const TCHAR* chFileName);

private:
	int nRetry;
	std::string std_AudioFunction;
	std::string std_AudioCammand;
	std::string std_Audio_RMS;
	std::string std_Audio_FREQ;
	std::string std_Audio_DB_lower;
	std::string std_Audio_DevID;
	std::string std_Audio_Mic_Line;
	std::string std_Audio_Vol_Wave;
	std::string std_Audio_AnaResult;
	std::string std_Audio_Delay;
	std::string std_Audio_AnaDelay;
	std::string std_Mrta_Max_Accumlate; 
	std::string std_Audio_Card_In;
	std::string std_Audio_Card_Out;

	CString cs_Lower;
	CString cs_Upper;
	CString cs_Measured;
	CMyMSXML m_AudioXML; 
	CString  m_cstrAudioItemFile;
	CString m_cstrAudioFile;

// Member functions
public:
	CAudioTest();
	virtual ~CAudioTest();
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

public:
	int n_Audio_Line;
	int n_Audio_Mic;
	int n_Audio_Vol;
	int n_Audio_Wave;
	int m_iMixerPath;
	int m_iretrytime;
	int n_delay;
	int n_Anadelay;
    int n_Audio_Result;
	int	n_Mrta_Max_Accumlate;

	bool check_All_RMS_Freq_Result[RESULT_UPPERBOUND];
	bool check_All_RMS_Freq();
	bool checkRMS_Freq_DB(float fRMS_Result, float fFreq_Result, float fDB_Result);

	void saveLog(CString message);

public:
	int mRTA_queue_szie;
	int mRTA_max_accumlate;
	RTAudio::RTAudio_Channel mRTA_channel;

	float mSPEC_RMS_lower;
	float mSPEC_RMS_upper;
	float mSPEC_Freq_lower;
	float mSPEC_Freq_upper;
	float mSPEC_DB_lower;
	float mSPEC_DB_difference;

	int n_DevID;
	int StartAnalysisThread();

public:    
	//2013 01 30
	RTAudio *m_pRTAudio;
	AudioSpectrum *m_pAudioSpectrum;
	CMixer *p_mix;

private:
//	void SetPICSData(std::string strKey, std::string strValue);
//	void SetResultToPICS(TSTRING strTestItem, bool bTestResult);
//	void KillAplay();
	bool PlayAudio(const TCHAR* szFilePath);
	bool PCPlay();
	bool AudioPushWav();

	bool Audio_Receiver_PlayWav_Analyze(CString csTestItem);
	bool Audio_Speaker_PalyWav_Analyze(CString csTestItem);

	bool Audio_Front_Headset_LoopBack_Analyze(CString csTestItem);
	bool Audio_Back_Headset_LoopBack_Analyze(CString csTestItem);
	bool Audio_HeadMic_Headset_LoopBack_Analyze(CString csTestItem);

	bool AudioOff(TCHAR* offCommand);
	bool AudioOffLoopBack(TCHAR* offCommand);

	void InitalResult();
	bool LoadAudioCardXMLSetting(CString csTestItem);
};

#endif // End of #ifndef _C_KEY_TEST_H_
