/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of AudioFT object.
 * FileName   : AudioFunctionTest.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/05/17
 * History    :
 *  NO   Date            Updated by        Content of change
 *	1	 2012/05/17      CY.Lee            Create.
 ********************************************************************************/

#pragma once

#include "mmsystem.h" 
#include "afxwin.h"
#pragma comment(lib,"winmm.lib")

#include "../../../CommonUtility/BaseObject/ITestItem.h"

//================================== Macro ==============================================//
#define  EXEFILENAME                   _T("AudioCollect.exe")
#define  OUTPUTDATAFILENAME            _T("TestRecord.txt")
#define  MUSIC                         _T("Music.wav")
#define  TRY_TIME                      3
//================================== Macro ==============================================//

class AudioFT : public ITestItem
{
RF_DECLARE_DYNCREATE(AudioFT)

public:
	AudioFT() {}
	~AudioFT() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:

	bool ResetData(void);

	bool RecordWAV(char* p_Switch, char* p_Testpart, char* p_Volume, char* p_path = "");
	bool PlayWAV(char* p_Switch, char* p_Testpart, char* p_Volume, char* p_path = "");

	//-- Detroit test item #12 --//
	bool Speaker_Test();

	//-- Detroit test item #13 --//
	bool Handset_Ear_Receiver_Test();

	//-- Detroit test item #14 --//
	bool Headset_Test();

	//-- Detroit test item #16 --//
	bool Front_MIC_Test();

	//-- Detroit test item #17 --//
	bool Back_MIC_Test();

	//-- Detroit test item #18 --//
	bool Head_MIC_Test();

private:

	int m_i_Action;
	int m_i_CommandDelay;
	int m_i_DeviceDelay;

	//-- Detroit test item #12 --//
	CRangePair m_obj_SPKLTestSpec;
	CRangePair m_obj_SPKL_F_COMSpec;

	//-- Detroit test item #13 --//
	CRangePair m_obj_EARTestSpec;
	CRangePair m_obj_EAR_F_COMSpec;

	//-- Detroit test item #14 --//
	int m_i_JackDetection;
	CRangePair m_obj_HPLSpec;
	CRangePair m_obj_HPRSpec;

	//-- Detroit test item #16 --//
	CRangePair m_obj_AU_MIC_BIAS_1Spec;
	CRangePair m_obj_MIC1_EARSpec;

	//-- Detroit test item #17 --//
	CRangePair m_obj_AU_MIC_BIAS_3Spec;
	CRangePair m_obj_MIC3_EARSpec;

	//-- Detroit test item #18 --//
	CRangePair m_obj_AU_HEAD_MIC_BIAS_Spec;
	CRangePair m_obj_AU_HEAD_MIC_EARSpec;
};
