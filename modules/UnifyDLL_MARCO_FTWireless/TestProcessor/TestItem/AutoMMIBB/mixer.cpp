// Mixer.cpp: implementation of the CMixer class.
//
//#include "stdwx.h"
#include "stdafx.h"
#include "Mixer.h"
#include "..\Glog\src\windows\glog\logging.h"
#include "..\Glog\src\windows\glog\log_severity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMixer::CMixer()
{
	m_VolRange = 100;
	m_nMixerDevID = -1;
}
CMixer::~CMixer()
{

}
CMixer::CMixer(const int VolRange)
{
	m_VolRange = VolRange;
}
//----------------------------设定音量---------------------------------------
bool CMixer::GetVolumeControl(HMIXER hmixer ,long componentType,long ctrlType,MIXERCONTROL* mxc)
{
	MIXERLINECONTROLS mxlc;
	MIXERLINE mxl;
	bool exist = false;
	mxl.cbStruct = sizeof(mxl);
	mxl.dwComponentType = componentType;
	if (componentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)
	{//获取录音麦克风设备
		mxl.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
		//   得到录制总线中的连接数
		mixerGetLineInfo(   (HMIXEROBJ)hmixer,   &mxl,
			MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_COMPONENTTYPE   );
		//   将连接数保存
		DWORD   dwConnections   =   mxl.cConnections;
		//   准备获取麦克风设备的ID
		DWORD   dwLineID   =   0;
		for   (   DWORD   i   =   0;   i   <   dwConnections;   i++   )
		{
			//   枚举每一个设备，当Source的ID等于当前的迭代记数
			mxl.dwSource   =   i;
			//   根据SourceID获得连接的信息
			MMRESULT mr   =   mixerGetLineInfo(   (HMIXEROBJ)hmixer,   &mxl,
				MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_SOURCE   );
			//   判断函数执行错误
			if   (   mr   !=   0   )
			{
				break;
			}
			//   如果当前设备类型是麦克风，则跳出循环。
			if   (   mxl.dwComponentType   ==
				MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE   )
			{
				exist = true;
				break;
			}
			//LOG(_INFO) << "[CMixer] 获取录音麦克风设备 OK " ;
		}
	}else 	if (componentType == MIXERLINE_COMPONENTTYPE_SRC_LINE)
	{//获取录音Linein设备
		mxl.dwComponentType   =   MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
		//   得到录制总线中的连接数
		mixerGetLineInfo(   (HMIXEROBJ)hmixer,   &mxl,
			MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_COMPONENTTYPE   );
		//   将连接数保存
		DWORD   dwConnections   =   mxl.cConnections;
		//   准备获取麦克风设备的ID
		DWORD   dwLineID   =   0;
		for   (   DWORD   i   =   0;   i   <   dwConnections;   i++   )
		{
			//   枚举每一个设备，当Source的ID等于当前的迭代记数
			mxl.dwSource   =   i;
			//   根据SourceID获得连接的信息
			MMRESULT mr   =   mixerGetLineInfo(   (HMIXEROBJ)hmixer,   &mxl,
				MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_SOURCE   );
			//   判断函数执行错误
			if   (   mr   !=   0   )
			{
				break;
			}
			//   如果当前设备类型是Linein，则跳出循环。
			if   (   mxl.dwComponentType   ==
				MIXERLINE_COMPONENTTYPE_SRC_LINE   )
			{
				exist = true;
				break;
			}
		}
		LOG(_INFO) << CStringA(L"[CMixer] 获取Linein设备 OK ").GetString();
	}
	else if(!mixerGetLineInfo((HMIXEROBJ)hmixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE))
	{
		LOG(_INFO) << CStringA(L"[CMixer] 获取 mixerGetLineInfo ").GetString();
		exist = true;
	}

	if (exist)
	{
		mxlc.cbStruct = sizeof(mxlc);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = ctrlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = mxc;
		if(mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE))
			return 0;
		else
			return 1;
	}else{
		LOG(_INFO) << CStringA(L"[CMixer] 获取 Nothing ").GetString();
	}
	return 0;
}
//---------------------------------------------------------------------------
long CMixer::GetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	mxcd.hwndOwner = 0;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbDetails = sizeof(mxcdMute);
	mxcd.paDetails = &mxcdMute;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	if (mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd,MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return -1;
	return mxcdMute.fValue;
}
//---------------------------------------------------------------------------
unsigned CMixer::GetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol;
	vol.dwValue=0;
	mxcd.hwndOwner = 0;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if(mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return -1;
	return vol.dwValue;
}
//---------------------------------------------------------------------------
bool CMixer::SetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc, bool mute)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	mxcdMute.fValue=mute;
	mxcd.hwndOwner = 0;
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(mxcdMute);
	mxcd.paDetails = &mxcdMute;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	if (mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return 0;
	return 1;
}
//---------------------------------------------------------------------------
bool CMixer::SetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc, long volume)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol;
	vol.dwValue = volume;
	mxcd.hwndOwner = 0;
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if(mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return 0;
	return 1;
}

//---------------------------------------------------------------------------
unsigned /*WINAPI */CMixer::GetVolume(MixerDevice dev)//得到设备的音量dev=0主音量，1WAVE ,2MIDI ,3 LINE IN
{

	long device;
	unsigned rt=0;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch (dev)
	{
	case WAVEOUT:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; break;
	case SYNTHESIZER:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER; break;
	case MICROPHONE:
		// device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break; // cd 音量
		device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE; break;   //麦克风音量
		// device=MIXERLINE_COMPONENTTYPE_SRC_LINE; break;         //PC 扬声器音量

	case LineIn:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break;

	case SPEAKERS:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS; break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
	if(!GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
	{
		mixerClose(hmixer);
		return 0;
	}
	rt=GetVolumeValue(hmixer,&volCtrl)*m_VolRange /volCtrl.Bounds.lMaximum;
	mixerClose(hmixer);
	return rt;

}
//---------------------------------------------------------------------------
bool /*WINAPI*/ CMixer::SetVolume(MixerDevice dev,int vol)//设置设备的音量
{
	// dev =0,1,2   分别表示主音量,波形,MIDI ,LINE IN
	// vol=0-m_VolRange      表示音量的大小 , 设置与返回音量的值用的是百分比，即音量从0 - m_VolRange ，而不是设备的绝对值
	// retrun false 表示设置音量的大小的操作不成功
	// retrun true 表示设置音量的大小的操作成功

	if ((dev < 0) || (m_nMixerDevID < 0))
	{
		CString csID;
		csID.Format("dev = %d,m_nMixerDevID = %d)", dev, m_nMixerDevID);
		LOG(_INFO) << "[CMixer] FAIL SetVolume dev & m_nMixerDevID " << csID;
		return false;
	}

	long device;
	bool rc=false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch (dev)
	{
	case WAVEOUT:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
		LOG(_INFO) << "[CMixer] SetVolume MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT" ;
		break;
	case SYNTHESIZER:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		LOG(_INFO) << "[CMixer] SetVolume MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER" ;
		break;
	case MICROPHONE:
		// device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break; // cd 音量
		device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		LOG(_INFO) << " [CMixer]  SetVolume MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE" ;
		break;   //麦克风音量
		// device=MIXERLINE_COMPONENTTYPE_SRC_LINE; break;         //PC 扬声器音量
		//device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break;

	case LineIn:
		device = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		LOG(_INFO) << "[CMixer] SetVolume MIXERLINE_COMPONENTTYPE_SRC_LINE " ;
		break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		//LOG(_INFO) << "[CMixer] SetVolume MIXERLINE_COMPONENTTYPE_DST_SPEAKERS default" ;
	}

	if(mixerOpen(&hmixer, m_nMixerDevID, 0, 0, MIXER_OBJECTF_MIXER)){
		LOG(_INFO) << "[CMixer] FAIL mixerOpen " ;
		return 0;
	}

	if(GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
	{
		vol=vol*volCtrl.Bounds.lMaximum/m_VolRange ;
		if(SetVolumeValue(hmixer,&volCtrl,vol)){
			//LOG(_INFO) << "[CMixer] OK  SetVolumeValue " ;
			rc=true;
		}else{
			LOG(_INFO) << "[CMixer] FAIL SetVolumeValue " ;
		}
	}
	mixerClose(hmixer);
	return rc;
}
//---------------------------------------------------------------------------
bool /*WINAPI*/ CMixer::SetMute(MixerDevice dev,/*long*/bool vol)//设置设备静音
{
	// dev =0,1,2   分别表示主音量,波形,MIDI ,LINE IN
	// vol=0,1      分别表示取消静音,设置静音
	// retrun false 表示取消或设置静音操作不成功
	// retrun true 表示取消或设置静音操作成功

	long device;
	bool rc=false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch (dev)
	{
	case WAVEOUT:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; break;
	case SYNTHESIZER:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER; break;
	case MICROPHONE:
		// device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break; // cd 音量
		device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE; break;   //麦克风音量
		// device=MIXERLINE_COMPONENTTYPE_SRC_LINE; break;         //PC 扬声器音量
		//device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
	if(GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		if(SetMuteValue(hmixer,&volCtrl,(bool)vol))
			rc=true;
	mixerClose(hmixer);
	return rc;
}
//---------------------------------------------------------------------------
bool /*WINAPI */CMixer::GetMute(MixerDevice dev)//检查设备是否静音
{
	//dev =0,1,2 分别表示主音量，波形,MIDI ,LINE IN
	// retrun false 表示没有静音
	// retrun true 表示静音
	long device;
	bool rc=false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch (dev)
	{
	case WAVEOUT:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; break;
	case SYNTHESIZER:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER; break;
	case MICROPHONE:
		// device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break; // cd 音量
		//device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE; break;   //麦克风音量
		// device=MIXERLINE_COMPONENTTYPE_SRC_LINE; break;         //PC 扬声器音量
		//device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break;
		device=MIXERLINE_COMPONENTTYPE_DST_WAVEIN; break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;

	if(GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		rc = !!GetMuteValue(hmixer,&volCtrl);
	mixerClose(hmixer);
	return rc;
}

bool CMixer::SelRecordLineAndSetVolume(DWORD dwComponentTypeOfLine,long lVolumeValue)
{
	LOG(_INFO) << "[Mixer] SelRecordLineAndSetVolume, Start";

	if (m_nMixerDevID < 0)
	{
		LOG(_INFO) << "[Mixer] fail, m_nMixerDevID < 0";
		return false;
	}
	else
	{
	//	LOG(_INFO) << "[Mixer] m_nMixerDevID=" << m_nMixerDevID;
	}

	bool bResult = false;
	MMRESULT mmResult;
	HMIXER hMixer;
	MIXERCAPS mixercaps;

	MIXERLINE mixerline;
	memset(&mixerline,0,sizeof(MIXERLINE));
	mixerline.cbStruct = sizeof(MIXERLINE);

	MIXERLINECONTROLS mlc;
	memset(&mlc,0,sizeof(MIXERLINECONTROLS));
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);

	MIXERCONTROL mc;
	memset(&mc,0,sizeof(MIXERCONTROL));
	mc.cbStruct = sizeof(MIXERCONTROL);

	if (mixerOpen(&hMixer,m_nMixerDevID,0,0,MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
	{
		LOG(_INFO) << "[Mixer] Open MixerDevice failed.";
	}
	else
	{
		if (mixerGetDevCaps(m_nMixerDevID,&mixercaps,sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
		{
			LOG(_INFO) << "[Mixer] mixerGetDevCaps failed.";
		}
		else
		{
			for (int j = 0; j < (int)mixercaps.cDestinations; j++)
			{
				mixerline.dwDestination = j;
				if (mixerGetLineInfo((HMIXEROBJ)hMixer,&mixerline,MIXER_GETLINEINFOF_DESTINATION) == MMSYSERR_NOERROR)
				{
					if ((mixerline.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_WAVEIN) && (mixerline.dwLineID == 4294901761)) //4294901761 = 0xffff0001
					{
						mlc.cControls = 1;
						mlc.cbmxctrl = sizeof(MIXERCONTROL);
						mlc.pamxctrl = &mc;
						mlc.dwLineID = mixerline.dwLineID;
						mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;

						if (mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR)
						{
							DWORD dwTotalItems;
							DWORD dwStereoSelControlID;

							dwStereoSelControlID = mc.dwControlID;
							dwTotalItems = mc.cMultipleItems;

							MIXERLINE line;
							memset(&line,0,sizeof(MIXERLINE));
							line.cbStruct = sizeof(MIXERLINE);
							line.dwDestination = j;

							MIXERCONTROLDETAILS_BOOLEAN *pmxcdMute = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(sizeof(MIXERCONTROLDETAILS_BOOLEAN)*dwTotalItems);
							for ( int i = 0; i < (int)dwTotalItems; i++ )
							{
								line.dwSource = i;
								mmResult = mixerGetLineInfo((HMIXEROBJ)hMixer,&line,MIXER_GETLINEINFOF_SOURCE);
								if ( line.dwComponentType == dwComponentTypeOfLine ) // && line.dwLineID == 65537 )
								{
									int nIndex = dwTotalItems-1-i;
									for ( int k = 0; k < (int)dwTotalItems; k++ )
									{
										pmxcdMute[k].fValue = FALSE;
									}
									pmxcdMute[nIndex].fValue = TRUE;
								}else;
							}

							MIXERCONTROLDETAILS mixercontroldetails;
							memset(&mixercontroldetails,0,sizeof(MIXERCONTROLDETAILS));

							mixercontroldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
							mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
							mixercontroldetails.cChannels = 1;
							mixercontroldetails.paDetails = pmxcdMute;
							mixercontroldetails.dwControlID = dwStereoSelControlID;
							mixercontroldetails.cMultipleItems = dwTotalItems;

							mmResult = mixerSetControlDetails((HMIXEROBJ)hMixer,&mixercontroldetails,MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
							free(pmxcdMute);

							int nConnections = (int)mixerline.cConnections;
							for ( int count = 0; count < nConnections; count++ )
							{
								mixerline.dwSource = count;
								mmResult = mixerGetLineInfo((HMIXEROBJ)hMixer,&mixerline,MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE);
								if ( mixerline.dwComponentType == dwComponentTypeOfLine ) // && mixerline.dwLineID == 65537 )
								{
									int nChannel = (int)mixerline.cChannels;

									mlc.cControls = 1;
									mlc.cbmxctrl = sizeof(MIXERCONTROL);
									mlc.pamxctrl = &mc;
									mlc.dwLineID = mixerline.dwLineID;
									mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
									mmResult = mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);

									MIXERCONTROLDETAILS_BOOLEAN* pControlDetail_BOOL = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nChannel*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
									if ( !pControlDetail_BOOL )
									{
										return bResult;
									}else;

									for ( int i = 0; i < nChannel; i++ )
									{
										pControlDetail_BOOL[i].fValue = lVolumeValue*mc.Bounds.lMaximum/m_VolRange;
									}

									MIXERCONTROLDETAILS mixercontroldetails;
									memset(&mixercontroldetails,0,sizeof(MIXERCONTROLDETAILS));

									mixercontroldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
									mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
									mixercontroldetails.cChannels = 1;
									mixercontroldetails.cMultipleItems = 0;
									mixercontroldetails.paDetails = &pControlDetail_BOOL[0];
									mixercontroldetails.dwControlID = mc.dwControlID;

									mmResult = mixerSetControlDetails((HMIXEROBJ)hMixer,&mixercontroldetails,MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
									free(pControlDetail_BOOL);

									bResult = true;
									LOG(_INFO) << "[Mixer] mixerSetControlDetails success.";

									break;
								}else;
							}//for ( int count = 0; count < nConnections; count++ )
						}else;

						break;
					}else;
				}else;
			}//for (int j = 0; j < mixercaps.cDestinations; j++)

			if (!bResult)
			{
				LOG(_INFO) << "[Mixer] mixerSetControlDetails fail.";
			}else;
		}
		mixerClose(hMixer);
	}

	LOG(_INFO) << "[Mixer] SelRecordLineAndSetVolume, End";

	return bResult;
}
bool CMixer::SetMixerSpeakOut(DWORD inLineType, bool szONOFF)
{
	if (m_nMixerDevID < 0)
	{
		return false;
	}

	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROLDETAILS_LISTTEXT *pmxcdSelectText;
	DWORD dwi = 0;

	DWORD g_dwIndex;
	DWORD g_dwSelectControlID;
	DWORD g_dwMultipleItems;
	DWORD g_dwControlType;


 	HMIXER g_hMixer;
	if (mixerOpen(&g_hMixer, m_nMixerDevID, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
	{
		TRACE("Could not open mixer device.");
		return false;
	}


	// get dwLineID
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (mixerGetLineInfo((HMIXEROBJ)(g_hMixer), &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
	{
		return false;
	}

	// get dwControlID
	g_dwControlType = MIXERCONTROL_CONTROLTYPE_MIXER;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = g_dwControlType;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (mixerGetLineControls((HMIXEROBJ)(g_hMixer), &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		// no mixer, try MUX
		g_dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = g_dwControlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = &mxc;
		if (mixerGetLineControls((HMIXEROBJ)(g_hMixer), &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
		{
			return false;
		}
	}

	// store dwControlID, cMultipleItems
	g_dwSelectControlID = mxc.dwControlID;
	g_dwMultipleItems = mxc.cMultipleItems;

	if (g_dwMultipleItems == 0)
	{
		return false;
	}

	// get the index of the Microphone Select control
	pmxcdSelectText =  (MIXERCONTROLDETAILS_LISTTEXT*)malloc(sizeof(MIXERCONTROLDETAILS_LISTTEXT)*g_dwMultipleItems);

	if(pmxcdSelectText != NULL)
	{
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = g_dwSelectControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = g_dwMultipleItems;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
		mxcd.paDetails = pmxcdSelectText;
		if (mixerGetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_LISTTEXT) == MMSYSERR_NOERROR)
		{
			// determine which controls the inpute source line
			for (dwi = 0; dwi < g_dwMultipleItems; dwi++)
			{
				// get the line information
				MIXERLINE mxl;
				mxl.cbStruct = sizeof(MIXERLINE);
				mxl.dwLineID = pmxcdSelectText[dwi].dwParam1;
				if (mixerGetLineInfo((HMIXEROBJ)(g_hMixer), &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_LINEID) == MMSYSERR_NOERROR && mxl.dwComponentType == inLineType)
				{
					// found, dwi is the index.
					g_dwIndex = dwi;
					break;
				}
			}

			if (dwi >= g_dwMultipleItems)
			{
				// could not find it using line IDs, some mixer drivers have
				// different meaning for MIXERCONTROLDETAILS_LISTTEXT.dwParam1.
				// let's try comparing the item names.
				for (dwi = 0; dwi < g_dwMultipleItems; dwi++)
				{
					if(inLineType == MIXERLINE_COMPONENTTYPE_DST_SPEAKERS)
					{
						if (lstrcmp(pmxcdSelectText[dwi].szName, "SPEAKERS") == 0)
						{
							// found, dwi is the index.
							g_dwIndex = dwi;
							LOG(_INFO) << "[Mixer] SetMixerRecord SPEAKERS OK" ;
							break;
						}
					}
					else if(inLineType == MIXERLINE_COMPONENTTYPE_SRC_LINE)
					{
						if (lstrcmp(pmxcdSelectText[dwi].szName, "WAVEOUT") == 0)
						{
							g_dwIndex = dwi;
							LOG(_INFO) << "[Mixer] SetMixerRecord WAVEOUT OK" ;
							break;
						}

					}
				}
			}
		}

		free(pmxcdSelectText);
	}


////////Set value to choosen source/////

	bool bRetVal = false;
	MIXERCONTROLDETAILS_BOOLEAN *pmxcdSelectValue;
	if (g_hMixer == NULL || g_dwMultipleItems == 0 || g_dwIndex >= g_dwMultipleItems)
	{
		return false;
	}


	// get all the values first
	pmxcdSelectValue = (MIXERCONTROLDETAILS_BOOLEAN*) malloc(sizeof( MIXERCONTROLDETAILS_BOOLEAN)*g_dwMultipleItems);

	if (pmxcdSelectValue != NULL)
	{
		MIXERCONTROLDETAILS mxcd2;
		mxcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd2.dwControlID = g_dwSelectControlID;
		mxcd2.cChannels = 1;
		mxcd2.cMultipleItems = g_dwMultipleItems;
		mxcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd2.paDetails = pmxcdSelectValue;
		if (mixerGetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd2, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)
		{
			ASSERT(g_dwControlType == MIXERCONTROL_CONTROLTYPE_MIXER || g_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX);

			// MUX restricts the line selection to one source line at a time.
			if (szONOFF != 0 && g_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX)
			{
				ZeroMemory(pmxcdSelectValue,
				g_dwMultipleItems * sizeof(MIXERCONTROLDETAILS_BOOLEAN));
			}

			// set the Choosen source value
			pmxcdSelectValue[g_dwIndex].fValue = szONOFF;

			mxcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
			mxcd2.dwControlID = g_dwSelectControlID;
			mxcd2.cChannels = 1;
			mxcd2.cMultipleItems = g_dwMultipleItems;
			mxcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
			mxcd2.paDetails = pmxcdSelectValue;
			if (mixerSetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd2, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)
			{
				bRetVal = true;
			}
		}

		free(pmxcdSelectValue);
	}

	mixerClose(g_hMixer);

	return bRetVal;

}

//20130204 Johnny
bool CMixer::SetMixerRecordSource(DWORD inLineType, bool szONOFF)
{
	if (m_nMixerDevID < 0)
	{
		return false;
	}

	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROLDETAILS_LISTTEXT *pmxcdSelectText;
	DWORD dwi = 0;

	DWORD g_dwIndex;
	DWORD g_dwSelectControlID;
	DWORD g_dwMultipleItems;
	DWORD g_dwControlType;


 	HMIXER g_hMixer;
	if (mixerOpen(&g_hMixer, m_nMixerDevID, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
	{
		TRACE("Could not open mixer device.");
		return false;
	}


	// get dwLineID
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
	if (mixerGetLineInfo((HMIXEROBJ)(g_hMixer), &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
	{
		return false;
	}

	// get dwControlID
	g_dwControlType = MIXERCONTROL_CONTROLTYPE_MIXER;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = g_dwControlType;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (mixerGetLineControls((HMIXEROBJ)(g_hMixer), &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		// no mixer, try MUX
		g_dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = g_dwControlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = &mxc;
		if (mixerGetLineControls((HMIXEROBJ)(g_hMixer), &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
		{
			return false;
		}
	}

	// store dwControlID, cMultipleItems
	g_dwSelectControlID = mxc.dwControlID;
	g_dwMultipleItems = mxc.cMultipleItems;

	if (g_dwMultipleItems == 0)
	{
		return false;
	}

	// get the index of the Microphone Select control
	pmxcdSelectText =  (MIXERCONTROLDETAILS_LISTTEXT*)malloc(sizeof(MIXERCONTROLDETAILS_LISTTEXT)*g_dwMultipleItems);

	if(pmxcdSelectText != NULL)
	{
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = g_dwSelectControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = g_dwMultipleItems;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
		mxcd.paDetails = pmxcdSelectText;
		if (mixerGetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_LISTTEXT) == MMSYSERR_NOERROR)
		{
			// determine which controls the inpute source line
			for (dwi = 0; dwi < g_dwMultipleItems; dwi++)
			{
				// get the line information
				MIXERLINE mxl;
				mxl.cbStruct = sizeof(MIXERLINE);
				mxl.dwLineID = pmxcdSelectText[dwi].dwParam1;
				if (mixerGetLineInfo((HMIXEROBJ)(g_hMixer), &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_LINEID) == MMSYSERR_NOERROR && mxl.dwComponentType == inLineType)
				{
					// found, dwi is the index.
					g_dwIndex = dwi;
					break;
				}
			}

			if (dwi >= g_dwMultipleItems)
			{
				// could not find it using line IDs, some mixer drivers have
				// different meaning for MIXERCONTROLDETAILS_LISTTEXT.dwParam1.
				// let's try comparing the item names.
				for (dwi = 0; dwi < g_dwMultipleItems; dwi++)
				{
					if(inLineType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)
					{
						if (lstrcmp(pmxcdSelectText[dwi].szName, "Microphone") == 0)
						{
							// found, dwi is the index.
							g_dwIndex = dwi;
							LOG(_INFO) << "[Mixer] SetMixerRecord Microphone OK" ;
							break;
						}
					}
					else if(inLineType == MIXERLINE_COMPONENTTYPE_SRC_LINE)
					{
						if (lstrcmp(pmxcdSelectText[dwi].szName, "Line In") == 0)
						{
							g_dwIndex = dwi;
							LOG(_INFO) << "[Mixer] SetMixerRecord LineIn OK" ;
							break;
						}

					}
				}
			}
		}

		free(pmxcdSelectText);
	}


////////Set value to choosen source/////

	bool bRetVal = false;
	MIXERCONTROLDETAILS_BOOLEAN *pmxcdSelectValue;
	if (g_hMixer == NULL || g_dwMultipleItems == 0 || g_dwIndex >= g_dwMultipleItems)
	{
		return false;
	}


	// get all the values first
	pmxcdSelectValue = (MIXERCONTROLDETAILS_BOOLEAN*) malloc(sizeof( MIXERCONTROLDETAILS_BOOLEAN)*g_dwMultipleItems);

	if (pmxcdSelectValue != NULL)
	{
		MIXERCONTROLDETAILS mxcd2;
		mxcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd2.dwControlID = g_dwSelectControlID;
		mxcd2.cChannels = 1;
		mxcd2.cMultipleItems = g_dwMultipleItems;
		mxcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd2.paDetails = pmxcdSelectValue;
		if (mixerGetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd2, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)
		{
			ASSERT(g_dwControlType == MIXERCONTROL_CONTROLTYPE_MIXER || g_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX);

			// MUX restricts the line selection to one source line at a time.
			if (szONOFF != 0 && g_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX)
			{
				ZeroMemory(pmxcdSelectValue,
				g_dwMultipleItems * sizeof(MIXERCONTROLDETAILS_BOOLEAN));
			}

			// set the Choosen source value
			pmxcdSelectValue[g_dwIndex].fValue = szONOFF;

			mxcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
			mxcd2.dwControlID = g_dwSelectControlID;
			mxcd2.cChannels = 1;
			mxcd2.cMultipleItems = g_dwMultipleItems;
			mxcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
			mxcd2.paDetails = pmxcdSelectValue;
			if (mixerSetControlDetails((HMIXEROBJ)(g_hMixer), &mxcd2, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)
			{
				bRetVal = true;
			}
		}

		free(pmxcdSelectValue);
	}

	mixerClose(g_hMixer);

	return bRetVal;

}
//---------------------------------------------------------------------------
int CMixer::GetWaveInDevices()
{
	int nDevNum = waveInGetNumDevs();
	char buf[256]={0};
	//LOG(_INFO) << "[Mixer] waveInGetNumDevs:" << nDevNum;

	m_vWaveInDevName.clear();
	if (nDevNum > 0)
	{
		CString strDevName;
		for (int i=0; i<nDevNum; i++)
		{
			WAVEINCAPS wic;
			MMRESULT   mmresult;

			mmresult = waveInGetDevCaps (i, &wic, sizeof(WAVEINCAPS));
			if (mmresult == 0 ){
				addLog("waveInGetDevCaps OK");
			}

			//waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS));
			strDevName.Format(_T("%s"), wic.szPname);
			m_vWaveInDevName.push_back(strDevName.MakeLower());
			memset(buf, 0, 256);
			sprintf_s(buf,256,"[Mixer] waveInDev[%d]: %s", i, strDevName );
			addLog(buf);
		}
	}else;

	return nDevNum;
}
//---------------------------------------------------------------------------
int CMixer::GetWaveInDevID(CString strDevName)
{
	int nDevNum = m_vWaveInDevName.size();
	int nFoundDevNum = 0;
	char buf[256]={0};

	m_nWaveInDevID = -1;
	sprintf_s(buf,256,"[Mixer] WaveInDevice KeyWord:%s", strDevName);
	addLog(buf);

	//mapping device name & audio path
	if ((strDevName.GetLength() >= 2) && (strDevName.GetLength() < 32))
	{
		strDevName.MakeLower(); //(1) [strDevName]Input from SPU "C-Media USB Headphone Set" => "c-media usb headphone set"

		LOG(_INFO) << "[Mixer] Try to mapping KeyWord, Start";
		for (int i=0; i<nDevNum; i++)
		{
			if (m_vWaveInDevName[i].Find(strDevName) != -1)
			{
				m_nWaveInDevID = i;
				nFoundDevNum ++;
//				LOG(_INFO) << "[Mixer] -----Matched!!----- WaveInDev[" << i << "] " << m_vWaveInDevName[i];
			}else;
		}
		LOG(_INFO) << "[Mixer] Try to mapping KeyWord, End";

		if (nFoundDevNum != 1)
		{
			if (nFoundDevNum > 1)
			{
				m_nWaveInDevID = -1;
				LOG(_INFO) << "[Mixer] Invalid KeyWord. Found more than 1 devices.";
			}
			else
			{
				LOG(_INFO) << "[Mixer] Can't find specific WaveInDevice.";
			}
		}else;
	}
	else
	{
		LOG(_INFO) << "[Mixer] Invalid KeyWord, KeyWord Length <2 or >31";
	}

	return m_nWaveInDevID;
}
//---------------------------------------------------------------------------
int CMixer::GetMixerDevID(CString csWaveInDeviceID)
{
		m_nMixerDevID = -1;

	//if (nWaveInDeviceID >= 0)
	//{
		UINT uNum = mixerGetNumDevs();
	  //LOG(_INFO) << "[Mixer] mixerGetNumDevs:" << uNum;
		char buf[256]={0};

		if (uNum > 0)
		{
			LOG(_INFO) << "[Mixer] List all MixerDevices and try to mapping KeyWord, Start";
			for ( UINT nid = 0; nid < uNum; nid++ )
			{
				MIXERCAPS mixercaps;
				CString strTempDevName;
				mixerGetDevCaps(nid,&mixercaps,sizeof(MIXERCAPS));
				strTempDevName.Format(_T("%s"), mixercaps.szPname);
				strTempDevName.MakeLower();
				csWaveInDeviceID.MakeLower();
				memset(buf, 0, 256);
				sprintf_s(buf,256,"[Mixer] MixerDev[%d]:%s", nid,strTempDevName );
				addLog(buf);

				if (strTempDevName.Find(csWaveInDeviceID) != -1)
				{
					m_nMixerDevID = nid;
					m_nMixerDestinations = (int)mixercaps.cDestinations;
				//	LOG(_INFO) << "[Mixer] -----Matched!!----- MixerDev[" << m_nMixerDevID << "], m_nMixerDestinations=" << m_nMixerDestinations;
					break;
				}else;
			}
			LOG(_INFO) << "[Mixer] List all MixerDevices and try to mapping KeyWord, End";
		}else;

		if (m_nMixerDevID < 0)
		{
			LOG(_INFO) << "[Mixer] Mapping MixerDevID fail.";
		}else;
	//}
	//else
	//{
	//	LOG(_INFO) << "[Mixer] Invalid WaveInDevID:" << nWaveInDeviceID;
	//}

	return m_nMixerDevID;
	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//int CMixer::GetMixerDevID(int nWaveInDeviceID)
//{
//	m_nMixerDevID = -1;
//
//	if (nWaveInDeviceID >= 0)
//	{
//		UINT uNum = mixerGetNumDevs();
//	  //LOG(_INFO) << "[Mixer] mixerGetNumDevs:" << uNum;
//		char buf[256]={0};
//
//		if (uNum > 0)
//		{
//			LOG(_INFO) << "[Mixer] List all MixerDevices and try to mapping KeyWord, Start";
//			for ( UINT nid = 0; nid < uNum; nid++ )
//			{
//				MIXERCAPS mixercaps;
//				CString strTempDevName;
//				mixerGetDevCaps(nid,&mixercaps,sizeof(MIXERCAPS));
//				strTempDevName.Format(_T("%s"), mixercaps.szPname);
//				strTempDevName.MakeLower();
//				memset(buf, 0, 256);
//				sprintf_s(buf,256,"[Mixer] MixerDev[%d]:%s", nid,strTempDevName );
//				addLog(buf);
//
//				if (strTempDevName.Find(m_vWaveInDevName[nWaveInDeviceID]) != -1)
//				{
//					m_nMixerDevID = nid;
//					m_nMixerDestinations = (int)mixercaps.cDestinations;
//				//	LOG(_INFO) << "[Mixer] -----Matched!!----- MixerDev[" << m_nMixerDevID << "], m_nMixerDestinations=" << m_nMixerDestinations;
//					break;
//				}else;
//			}
//			LOG(_INFO) << "[Mixer] List all MixerDevices and try to mapping KeyWord, End";
//		}else;
//
//		if (m_nMixerDevID < 0)
//		{
//			LOG(_INFO) << "[Mixer] Mapping MixerDevID fail.";
//		}else;
//	}
//	else
//	{
//		LOG(_INFO) << "[Mixer] Invalid WaveInDevID:" << nWaveInDeviceID;
//	}
//
//	return m_nMixerDevID;
//}
//---------------------------------------------------------------------------
int CMixer::addLog( std::string log )
{
	LOG(_INFO) << log ;
	return 0;
}
//---------------------------------------------------------------------------
bool CMixer::CheckWaveOutDevice()
{
	bool bResult = false;
	int nDevNum = waveOutGetNumDevs();

	if (nDevNum <= 0)
	{
		LOG(_INFO) << "[Mixer] Can't find any WaveOutDevices";
	}
	else
	{
		LOG(_INFO) << "[Mixer] waveOutGetNumDevs:" << nDevNum;
		LOG(_INFO) << "[Mixer] WaveOutDevKeyWord:" << m_strWaveOutDevKeyWord;

		if (m_strWaveOutDevKeyWord.GetLength() < 3)
		{
			LOG(_INFO) << "[Mixer] Fail: the length of WaveOutDevKeyWord < 3" << nDevNum;
			return bResult;
		}else;

		CString strDevName;
		char buf[256]={0};
		bool bOpenUSB = false;

		for (int i=0; i<nDevNum; i++)
		{
			WAVEOUTCAPS wic;
			waveOutGetDevCaps(i, &wic, sizeof(WAVEOUTCAPS));
			strDevName.Format(_T("%s"), wic.szPname);
			memset(buf, 0, 256);
			sprintf_s(buf,256,"[Mixer] waveOutDev[%d]: %s", i, strDevName );
			addLog(buf);

			strDevName.MakeLower();
			if (strDevName.Find(m_strWaveOutDevKeyWord) != -1)
			{
				LOG(_INFO) << "[Mixer] Try to open specific WaveOutDevice...";

				HWAVEOUT hWaveOut;
				WAVEFORMATEX wavform;
				wavform.wFormatTag = WAVE_FORMAT_PCM;
				wavform.nChannels = 2;
				wavform.nSamplesPerSec = 48000;
				wavform.nAvgBytesPerSec = 48000*16*2/8;
				wavform.nBlockAlign = 4;
				wavform.wBitsPerSample = 16;
				wavform.cbSize = 0;
				MMRESULT resResult;
				resResult = waveOutOpen(&hWaveOut, i, &wavform, NULL, 0, NULL);
				if (resResult == MMSYSERR_NOERROR)
				{
					LOG(_INFO) << "[Mixer] Open specific WaveOutDevice: Success";

					bOpenUSB = true;
					waveOutClose(hWaveOut);
					hWaveOut = 0;
					break;
				}
				else
				{
					LOG(_INFO) << "[Mixer] Open specific WaveOutDevice: Fail";
				}
			}else;
		}

		bResult = bOpenUSB;
		if (!bResult)
		{
			LOG(_INFO) << "[Mixer] Can't find any specific WaveOutDevice";
		}else;
	}

	return bResult;
}