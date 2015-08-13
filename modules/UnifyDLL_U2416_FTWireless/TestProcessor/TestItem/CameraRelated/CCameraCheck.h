#ifndef _C_CameraCheck_Proc_H_
#define _C_CameraCheck_Proc_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"

/* Name */
#define FRONT_CAMERA_VERIFICATION		_T("frontPic")
#define REAR_CAMERA_VERIFICATION		_T("rearPic")
#define CLEAR_OLD_PICTURE				_T("deletePicture")

#define MULTI_CAMERA_VERIFICATION		_T("Multi_Camera_Verification")
#define SINGLE_CAMERA_VERIFICATION		_T("Single_Camera_Verification")
#define SINGLE_BLEMISH					_T("Single_Blemish")
#define SINGLE_MTF						_T("Single_MTF")

#define QPHONE_NAME_0					_T("QPHONE0")
#define QPHONE_NAME_1					_T("QPHONE1")
#define QPHONE_NAME_2					_T("QPHONE2")
#define QPHONE_NAME_3					_T("QPHONE3")
#define QPHONE_NAME_4					_T("QPHONE4")

#define QPHONE_FOLDER_0					_T("SINGLE_TEST")
#define QPHONE_FOLDER_1					_T("SLOT1")
#define QPHONE_FOLDER_2					_T("SLOT2")
#define QPHONE_FOLDER_3					_T("SLOT3")
#define QPHONE_FOLDER_4					_T("SLOT4")

#define RESULT_PASS_FOLDER				_T("D:\\Log\\ImgAndResult")
#define RESULT_FAIL_FOLDER				_T("D:\\Log\\ImgAndResult_Fail")
#define RESULT_ERROR_FOLDER				_T("D:\\Log\\ImgAndResult_Error")

#define COLOR_GREEN						_T("g")

/* Pics */
#define PICS_REAR_BLEMISH_RESULT		"Algo_Blemish_Rear"
#define PICS_REAR_SHADING_RESULT		"Algo_Shading_Rear"
#define PICS_REAR_COLOR_RESULT			"Algo_Color_Rear"
#define PICS_REAR_MTF_RESULT			"Algo_MTF_Rear"
#define PICS_FRONT_BLEMISH_RESULT		"Algo_Blemish_Front"
#define PICS_FRONT_SHADING_RESULT		"Algo_Shading_Front"
#define PICS_FRONT_COLOR_RESULT			"Algo_Color_Front"
#define PICS_FRONT_MTF_RESULT			"Algo_MTF_Front"

/* Value */
#define QPHONE_MULTI_MAX	4

#define QPHONE_SERIAL_0		0
#define QPHONE_SERIAL_1		1
#define QPHONE_SERIAL_2		2
#define QPHONE_SERIAL_3		3
#define QPHONE_SERIAL_4		4

#define QPHONE_PORT_0		2000
#define QPHONE_PORT_1		2001
#define QPHONE_PORT_2		2002
#define QPHONE_PORT_3		2003
#define QPHONE_PORT_4		2004

#define TYPE_MULTI			1
#define TYPE_SINGLE			2

#define FRONT_CAMERA		0
#define REAR_CAMERA			1

#define REAR_MTF_PIC		0
#define REAR_LP100_PIC		1
#define FRONT_MTF_PIC		2
#define FRONT_LP100_PIC		3

#define ORDER_NOT_USE		-1
#define ORDER_BLEMISH_REAR	0
#define ORDER_SHADING_REAR	1
#define ORDER_COLOR_REAR	2
#define ORDER_MTF_REAR		3
#define ORDER_BLEMISH_FRONT	4
#define ORDER_SHADING_FRONT	5
#define ORDER_COLOR_FRONT	6
#define ORDER_MTF_FRONT		7

#define IMG_CODE_0			0
#define IMG_CODE_1			1
#define IMG_CODE_2			2
#define IMG_CODE_3			3
#define IMG_CODE_4			4
#define IMG_CODE_ROOT		10
#define IMG_CODE_BG			10

#define MUTEX_TIMEOUT_PROCESS	10000
#define MUTEX_TIMEOUT_FTD		20000
#define MUTEX_TIMEOUT_REPORT	10000
#define MUTEX_TIMEOUT_PICS		10000
#define MUTEX_TIMEOUT_WAIT		10000

#define THREAD_HANDLE_TIMEOUT	65000
#define ADB_CMD_TIMEOUT			25000


typedef enum e_qisda_image_sample
{
	REAR_LP,
	REAR_MTF,
	FRONT_LP,
	FRONT_MTF,
	IMAGE_SAMPLE_COUNT
} QISDA_IMAGE_SAMPLE;

typedef enum e_qisda_process
{
	REAR_CAMERA_MODULE_INFO,
	REAR_CAMERA_START,
	REAR_CAMERA_CAPTURE_LP100,
	REAR_CAMERA_ALGO_BLEMISH,
	REAR_CAMERA_ALGO_SHADING,
	REAR_CAMERA_ALGO_COLOR,
	REAR_CAMERA_CAPTURE_MTF,
	REAR_CAMERA_ALGO_MTF,
	REAR_CAMERA_DIALOG_MTF,
	REAR_CAMERA_STOP,
	FRONT_CAMERA_MODULE_INFO,
	FRONT_CAMERA_FUSE_ID_READ_WRITE,
	FRONT_CAMERA_FUSE_ID_READ_WRITE_NEW,
	FRONT_CAMERA_START,
	FRONT_CAMERA_CAPTURE_LP100,
	FRONT_CAMERA_ALGO_BLEMISH,
	FRONT_CAMERA_ALGO_SHADING,
	FRONT_CAMERA_ALGO_COLOR,
	FRONT_CAMERA_CAPTURE_MTF,
	FRONT_CAMERA_ALGO_MTF,
	FRONT_CAMERA_STOP,
	SHOW_RESULT
} QISDA_PROCESS;

typedef enum e_mutex_key
{
	KEY_PROCESS,
	KEY_FTD,
	KEY_REPORT,
	KEY_PICS,
	KEY_WAIT
} MUTEX_KEY;

typedef enum e_call_func
{
	FUNC_REAR_MODULE_INFO,
	FUNC_FRONT_MODULE_INFO,
	FUNC_FRONT_FUSE_ID_WRITE,
	FUNC_CAM_START,
	FUNC_CAM_CAPTURE,
	FUNC_CAM_STOP,
	FUNC_DISPLAY_LCD_RGB

} CALL_FUNC;



class CCameraCheck : public ITestItem
{
RF_DECLARE_DYNCREATE(CCameraCheck)

// Data members
public:

typedef struct tagQPhoneThreadParam
{
	CCameraCheck* pThisObject;
	int nQPhoneNumber;
	int nQisdaProcess;

} THREAD_PARAM, *LPTHREAD_PARAM;

protected:

private:
	////parameters

	CWinThread *ThreadHandle;
	bool m_bStopViewImg;

	std::string str_Name;
	std::string str_Value;
	std::string str_Check;
	std::string str_RearAlgoList;
	std::string str_RearCheckList;
	std::string str_FrontAlgoList;
	std::string str_FrontCheckList;
	std::string m_sleepRearBlemish;
	std::string m_sleepRearShading;
	std::string m_sleepRearMTF;
	std::string m_sleepFrontBlemish;
	std::string m_sleepFrontShading;
	std::string m_sleepFrontMTF;

// Member functions
public:
	CCameraCheck();
	~CCameraCheck();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool InitData();
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	CString m_csQisdaToolFolder;
	CString m_csQisdaImgExe_Rear_Path;
	CString m_csQisdaImgExe_Front_Path;
	CString m_csArrQisdaImgExe_Rear_Path[QPHONE_MULTI_MAX+1];
	CString m_csArrQisdaImgExe_Front_Path[QPHONE_MULTI_MAX+1];

	CString m_csArrQisdaImg_Path[QPHONE_MULTI_MAX+1];
	CString m_csArrQisdaImgBMP_Rear_Path[QPHONE_MULTI_MAX+1];
	CString m_csArrQisdaImgBMP_Front_Path[QPHONE_MULTI_MAX+1];
	CString m_csArrQisdaImgDeBug_Path[QPHONE_MULTI_MAX+1];
	CString m_csArrQisdaImgResult_Path[QPHONE_MULTI_MAX+1];

	CString m_csArrQisdaImgSample_Path[IMAGE_SAMPLE_COUNT][QPHONE_MULTI_MAX+1];

	CString m_imgPath;

	CString m_csArrQPhoneFolder[QPHONE_MULTI_MAX+1];
	CString m_csArrQPhoneName[QPHONE_MULTI_MAX+1];
	CString m_csArrRearFuseID[QPHONE_MULTI_MAX+1];
	CString m_csArrFrontFuseID[QPHONE_MULTI_MAX+1];
	int m_nArrQPhonePort[QPHONE_MULTI_MAX+1];
	bool m_bModuleDateSetPicsFlag;
	int m_nCheckRearFuseID, m_nCheckRearBlemish, m_nCheckRearShading, m_nCheckRearColor, m_nCheckRearMTF;
	int m_nCheckFrontFuseID, m_nCheckFrontBlemish, m_nCheckFrontShading, m_nCheckFrontColor, m_nCheckFrontMTF;
	int m_nSleepRearBlemish;
	int m_nSleepRearShading;
	int m_nSleepRearMTF;
	int m_nSleepFrontBlemish;
	int m_nSleepFrontShading;
	int m_nSleepFrontMTF;
	bool m_bTestMode;

	int m_nProcess;
	HANDLE m_hQPhoneProcess[5];
	HANDLE g_MutexProcess;
	HANDLE g_MutexFtd;
	HANDLE g_MutexReport;
	HANDLE g_MutexPics;
	HANDLE g_MutexWait;

	struct QPhone_Info{
		CString m_csArrAlgoSpec[8];
		CString m_csArrAlgoMeasured[8];
	} st_QPhone_Info[QPHONE_MULTI_MAX+1];

	bool ExecAdbOut(CString Command, int nQPhoneNumber, char* output, char* ErrorCode);
	bool ExecQisdaIMGExe_Rear(CString Command, int nQPhoneNumber, char* ErrorMsg);
	bool ExecQisdaIMGExe_Front(CString csAlgo, int nQPhoneNumber, char* ErrorMsg);
	bool Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse);

	bool InitialTestParameter();
	bool GetRearCameraModuleInfo();

	CString MkDirImages(int nQPhoneNumber);
	bool MkDirQPhones(CString strDesPath);
	bool MkDirResult();
	bool ClearOldPictures();

	bool Qisda_IMG_Algo_Front( CString AlgoType, char* ResultMatch, int nQPhoneNumber);
	bool Qisda_IMG_Algo_Rear( CString AlgoType, char* ResultMatch, int nQPhoneNumber);

	void InitalJigPos();
	void MoveAwayFrontCamLP100();
	void MoveAwayBackCamLP100();
	void MoveCoverBackCamLP100();
    void MoveCoverFrontCamLP100();

	bool Qisda_CapturePic(int i_shot_number, int frontRear, int nQPhoneNumber) ;// frontRear=0-> front pic, =1-> rear pic

	bool Qisda_StartCam(int frontRear, int nDeviceNumber);  //normal
	bool Qisda_StopCam(int frontRear, int nDeviceNumber);  //normal
	bool Qisda_ReadModuleInfo(int frontRear, int nDeviceNumber, char* szBuff);
	bool Qisda_WriteModuleInfo(int frontRear, int nDeviceNumber, char* szInBuff, char* szOutBuff);

	bool Run_Camera_Verification(int nType);
	void SelectExecution(QISDA_PROCESS nQisdaProcess, int nType);
	void Qisda_SingleExecution(QISDA_PROCESS nProcess);

	bool FindResultTxt(int nQPhoneNumber, int nTimeout = 5000);
	CString BackupResultAndPicture(char* szAlgo, char* flag, int nQPhoneNumber);

	bool GetResult(char* szAlgo, int nQPhoneNumber);
	bool SendResultToPICS(char* szAlgo, CString szFilePath, int nQPhoneNumber);

	void Report(bool bRes, std::string strMessage, int nQPhoneNumber, std::string strErrorCode = "", int nAlgoOrder = -1);
	void CheckStopCam(bool bRes, int nIndexQPhoneSlot, int frontRear, char* error_code, std::string pics_name, std::string msg, int check = 1, int order = -1);
	void ReportTimeoutHandler(int nIndexQPhoneSlot, int nQisdaProcess);

	bool GenerateFuseID(char* strFuseID);
	bool StrDec2Hex_8bytes(char* strInputDec_8bytes, char* strOutputHex_8bytes);
	bool TokenStrDec2Hex(char* strInputDec, char* strOutputHex);
	bool SetColorOnDeviceScreen(int nQPhoneNumber, char* szColor);
	bool CheckRearMTFDialog(int nQPhoneNumber);

	bool Qisda_MultiExecution(QISDA_PROCESS nProcess);
	static UINT AfxQPhoneThread(LPVOID lpParam);

	void AlgoBeforeSleep(int nIndexQPhoneSlot, std::string msg, int order, int nTime);
	bool Qisda_QPhoneProcess(int nIndexQPhoneSlot, int nQisdaProcess);
	bool SetProcessMutexLock(MUTEX_KEY nMutexKey, bool bMutexStatus);
	bool CallFtdFunction(CALL_FUNC nFtdFunc, int nPort, int nTimeout, char* pszInput, char* pszOutput);
	void SetPICSDataWithLock(std::string str_key, std::string str_value, int nQPhoneNumber);

private:
	HANDLE         m_h_com;
	DWORD          m_dw_baud;
	INT            m_i_dataBits;
	INT            m_i_parity;
	INT            m_i_stopBits;
	INT            m_i_flowCtrl;
	CString        m_str_port;
	OVERLAPPED     m_os_write;
	OVERLAPPED     m_os_read;
	BOOL           m_b_connected;
	const int nWaitResultTime;
};

#endif // End of #ifndef _C_CameraCheck_Proc_H_
