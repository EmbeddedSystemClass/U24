/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_Run_In_Proc_H_
#define _C_Run_In_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
/* ID Type */
//[1] 2D            
//[2] 3D            
//[3] CPU  => ²»œy£¬value always = -1         
//[4] Audio         
//[5] AudioToRece   
//[6] Video         
//[7] Camera        
//[8] Storage  => ²»œy£¬value always = -1  
//[9] Memory => ²»œy£¬value always = -1       
//[10] Sleep         
//[11] Reboot
             

#define runIn_2D				"runIn_2D"
#define runIn_3D				"runIn_3D"
#define runIn_CPU				"runIn_CPU"
#define runIn_Audio				"runIn_Audio"
#define runIn_AudioToRece		"runIn_AudioToRece"
#define runIn_Video				"runIn_Video"
#define runIn_Camera			"runIn_Camera"
#define runIn_Storage			"runIn_Storage"
#define runIn_Memory			"runIn_Memory"
#define runIn_Sleep				"runIn_Sleep"
#define runIn_Reboot			"runIn_Reboot"


#define ADB_CMD_TIMEOUT			25000

const int n_array_upper = 20;

class CRUN_IN : public ITestItem
{
RF_DECLARE_DYNCREATE(CRUN_IN)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_TestItem;
	std::string m_str_CMD;
	std::string m_str_OffCMD;
	std::string str_msg;

	std::string str_Pics ;

	int n_Delay;


	int m_i_SleepTime;
	int m_i_retryTime;
	int m_i_MexRetryTime;
	int m_i_Index;

	//bool b_res;


// Member functions
public:
	CRUN_IN() {}
	~CRUN_IN() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode, int nSleep);

	bool go_runIn_2D(int nIndex);
	bool go_runIn_3D(int nIndex);
	bool go_runIn_CPU(int nIndex);
	bool go_runIn_Audio(int nIndex);
	bool go_runIn_AudioToRece(int nIndex);
	bool go_runIn_Video(int nIndex);
	bool go_runIn_Camera(int nIndex);
	bool go_runIn_Storage(int nIndex);
	bool go_runIn_Memory(int nIndex);
	bool go_runIn_Sleep(int nIndex);
	bool go_runIn_Reboot(int nIndex);

	bool getTargetLog();
	bool readLog();
	bool copyDeleteLog();
	bool saveLogWhenFail();
	int runInResult[n_array_upper];
	
	CString str_filePath;
	CString GetDirectory();


};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
