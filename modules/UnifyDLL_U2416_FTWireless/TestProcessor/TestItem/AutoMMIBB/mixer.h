// Mixer.h: interface for the CMixer class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"

#if !defined(AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_)
#define AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include "windows.h"
#include <mmsystem.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>

class CMixer 
{
public:
	CMixer();
	CMixer(const int VolRange);
	virtual ~CMixer();
	enum MixerDevice{
		SPEAKERS=0,
		WAVEOUT,
		SYNTHESIZER,
		MICROPHONE,
		LineIn,
	};
	bool GetMute(MixerDevice dev);   //检查设备是否静音
	bool SetMute(MixerDevice dev,bool vol);     //设置设备静音
	bool SetVolume(MixerDevice dev,int vol);   //设置设备的音量
	unsigned GetVolume(MixerDevice dev);    //得到设备的音量dev=0主音量，1WAVE ,2MIDI ,3 LINE IN

	//20130204 Johnny set record source
	bool SetMixerRecordSource(DWORD inLineType, bool szONOFF);
	bool SetMixerSpeakOut(DWORD inLineType, bool szONOFF);
	bool SelRecordLineAndSetVolume(DWORD dwComponentTypeOfLine,long lVolumeValue);

	//Cariel
	int  GetWaveInDevices();
	int  GetWaveInDevID(CString strDevName);
	int GetMixerDevID(CString csWaveInDeviceID);
	int addLog (std::string log);
	bool CheckWaveOutDevice();

	CString m_strAudioPath;
	CString m_strWaveOutDevKeyWord;
	MixerDevice m_MixerDevice;
	int m_nWaveInDevID;
	//int m_nSpeDevID;
	int m_nMixerDevID;
	int m_nMixerDestinations;

private:
	bool GetVolumeControl(HMIXER hmixer ,long componentType,long ctrlType,MIXERCONTROL* mxc);
	bool SetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc, long volume);
	bool SetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc, bool mute);
	unsigned GetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc);
	long GetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc);

	long m_VolRange ;
	std::vector<CString> m_vWaveInDevName;

};
#endif // !defined(AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_)
