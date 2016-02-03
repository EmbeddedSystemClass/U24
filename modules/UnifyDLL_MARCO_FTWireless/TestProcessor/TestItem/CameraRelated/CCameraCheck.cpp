#include "stdafx.h"
#include "CCameraCheck.h"


#define MAXTIMEOUTS		2000	// timeout(ms)
#define EVENTCHAR       0x0d	// event char
#define XON			    0x11	// DCB XonChar
#define XOFF			0x13	// DCB XoffChar

RF_IMPLEMENT_DYNCREATE(CCameraCheck)

CCameraCheck* pCameraTest;


CCameraCheck::CCameraCheck(): nWaitResultTime(5000)
{
	m_bStopViewImg = false;
	m_bModuleDateSetPicsFlag = false;
	m_nCheckRearFuseID = 0;
	m_nCheckRearBlemish = 1;
	m_nCheckRearShading = 1;
	m_nCheckRearColor = 1;
	m_nCheckRearMTF = 1;
	m_nCheckFrontFuseID = 0;
	m_nCheckFrontBlemish = 1;
	m_nCheckFrontShading = 1;
	m_nCheckFrontColor = 1;
	m_nCheckFrontMTF = 1;
	m_bTestMode = false;

	m_nSleepRearBlemish = 0;
	m_nSleepRearShading = 0;
	m_nSleepRearMTF = 0;
	m_nSleepFrontBlemish = 0;
	m_nSleepFrontShading = 0;
	m_nSleepFrontBlemish = 0;

	m_csArrQPhoneFolder[QPHONE_SERIAL_0] = QPHONE_FOLDER_0;
	m_csArrQPhoneFolder[QPHONE_SERIAL_1] = QPHONE_FOLDER_1;
	m_csArrQPhoneFolder[QPHONE_SERIAL_2] = QPHONE_FOLDER_2;
	m_csArrQPhoneFolder[QPHONE_SERIAL_3] = QPHONE_FOLDER_3;
	m_csArrQPhoneFolder[QPHONE_SERIAL_4] = QPHONE_FOLDER_4;

	m_csArrQPhoneName[QPHONE_SERIAL_0] = QPHONE_NAME_0;
	m_csArrQPhoneName[ m_nQPhoneOrderSlot[QPHONE_SERIAL_1] ] = QPHONE_NAME_1;
	m_csArrQPhoneName[ m_nQPhoneOrderSlot[QPHONE_SERIAL_2] ] = QPHONE_NAME_2;
	m_csArrQPhoneName[ m_nQPhoneOrderSlot[QPHONE_SERIAL_3] ] = QPHONE_NAME_3;
	m_csArrQPhoneName[ m_nQPhoneOrderSlot[QPHONE_SERIAL_4] ] = QPHONE_NAME_4;

	m_nArrQPhonePort[QPHONE_SERIAL_0] = QPHONE_PORT_0;
	m_nArrQPhonePort[ m_nQPhoneOrderSlot[QPHONE_SERIAL_1] ] = QPHONE_PORT_1;
	m_nArrQPhonePort[ m_nQPhoneOrderSlot[QPHONE_SERIAL_2] ] = QPHONE_PORT_2;
	m_nArrQPhonePort[ m_nQPhoneOrderSlot[QPHONE_SERIAL_3] ] = QPHONE_PORT_3;
	m_nArrQPhonePort[ m_nQPhoneOrderSlot[QPHONE_SERIAL_4] ] = QPHONE_PORT_4;

	m_hQPhoneProcess[QPHONE_SERIAL_0] = NULL;
	m_hQPhoneProcess[QPHONE_SERIAL_1] = NULL;
	m_hQPhoneProcess[QPHONE_SERIAL_2] = NULL;
	m_hQPhoneProcess[QPHONE_SERIAL_3] = NULL;
	m_hQPhoneProcess[QPHONE_SERIAL_4] = NULL;

	g_MutexProcess = ::CreateMutex(NULL, FALSE, "MUTEX_PROCESS");
	g_MutexFtd     = ::CreateMutex(NULL, FALSE, "MUTEX_FTD");
	g_MutexReport  = ::CreateMutex(NULL, FALSE, "MUTEX_REPORT");
	g_MutexPics    = ::CreateMutex(NULL, FALSE, "MUTEX_PICS");
	g_MutexWait    = ::CreateMutex(NULL, FALSE, "MUTEX_WAIT");
}

CCameraCheck::~CCameraCheck()
{
}

bool CCameraCheck::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCameraCheck::Run()
{
	if (str_Name == MULTI_CAMERA_VERIFICATION)
	{
		return Run_Camera_Verification( TYPE_MULTI );
	}
	else if (str_Name == SINGLE_CAMERA_VERIFICATION)
	{
		return Run_Camera_Verification( TYPE_SINGLE );
	}
	return true;
}

bool CCameraCheck::PostRun()
{
	return true;
}

bool CCameraCheck::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		str_Name = paramMap[std::string("Name")];
	}

	/* CheckMatch */
	if (paramMap.find(std::string("Check")) != paramMap.end())
	{
		str_Check = paramMap[std::string("Check")];
	}

	/* RearAlgoList */
	if (paramMap.find(std::string("RearAlgoList")) != paramMap.end())
	{
		str_RearAlgoList = paramMap[std::string("RearAlgoList")];
	}

	/* RearCheckList */
	if (paramMap.find(std::string("RearCheckList")) != paramMap.end())
	{
		str_RearCheckList = paramMap[std::string("RearCheckList")];
	}

	/* FrontAlgoList */
	if (paramMap.find(std::string("FrontAlgoList")) != paramMap.end())
	{
		str_FrontAlgoList = paramMap[std::string("FrontAlgoList")];
	}

	/* FrontCheckList */
	if (paramMap.find(std::string("FrontCheckList")) != paramMap.end())
	{
		str_FrontCheckList = paramMap[std::string("FrontCheckList")];
	}

	/* SleepForRearBlemish */
	if (paramMap.find(std::string("SleepForRearBlemish")) != paramMap.end())
	{
		m_sleepRearBlemish = paramMap[std::string("SleepForRearBlemish")];
		m_nSleepRearBlemish = CStr::StrToInt(m_sleepRearBlemish);
	}
	else
	{
		m_sleepRearBlemish = "0";
	}

	/* SleepForRearShading */
	if (paramMap.find(std::string("SleepForRearShading")) != paramMap.end())
	{
		m_sleepRearShading = paramMap[std::string("SleepForRearShading")];
		m_nSleepRearShading = CStr::StrToInt(m_sleepRearShading);
	}
	else
	{
		m_sleepRearShading = "0";
	}

	/* SleepForRearMTF */
	if (paramMap.find(std::string("SleepForRearMTF")) != paramMap.end())
	{
		m_sleepRearMTF = paramMap[std::string("SleepForRearMTF")];
		m_nSleepRearMTF = CStr::StrToInt(m_sleepRearMTF);
	}
	else
	{
		m_sleepRearMTF = "0";
	}

	/* SleepForFrontBlemish */
	if (paramMap.find(std::string("SleepForFrontBlemish")) != paramMap.end())
	{
		m_sleepFrontBlemish = paramMap[std::string("SleepForFrontBlemish")];
		m_nSleepFrontBlemish = CStr::StrToInt(m_sleepFrontBlemish);
	}
	else
	{
		m_sleepFrontBlemish = "0";
	}

	/* SleepForFrontShading */
	if (paramMap.find(std::string("SleepForFrontShading")) != paramMap.end())
	{
		m_sleepFrontShading = paramMap[std::string("SleepForFrontShading")];
		m_nSleepFrontShading = CStr::StrToInt(m_sleepFrontShading);
	}
	else
	{
		m_sleepFrontShading = "0";
	}

	/* SleepForFrontMTF */
	if (paramMap.find(std::string("SleepForFrontMTF")) != paramMap.end())
	{
		m_sleepFrontMTF = paramMap[std::string("SleepForFrontMTF")];
		m_nSleepFrontMTF = CStr::StrToInt(m_sleepFrontMTF);
	}
	else
	{
		m_sleepFrontMTF = "0";
	}

	return true;
}

bool CCameraCheck::InitData()
{
	return true;
}

void CCameraCheck::Report(bool bRes, std::string strMessage, int nQPhoneNumber, std::string strErrorCode, int nAlgoOrder)
{
	SetProcessMutexLock(KEY_REPORT, true);

	m_strErrorCode = bRes ? "" : strErrorCode;
	int nSeverity  = bRes ? MSG_INFO : MSG_ERROR;
	m_strResult    = bRes ? "PASS" : "FAIL";

	CString csQPhoneNumber;
	csQPhoneNumber.Format(_T("%d"), nQPhoneNumber);
	CString csTraceMsg;
	if (nQPhoneNumber > 0 && m_i_QPhoneMultiCtrl >= 1)
	{
		m_strResult += _T(",#SLOT#") + csQPhoneNumber;
		csTraceMsg.Format(_T("Slot-%d: "), nQPhoneNumber);
	}
	TraceLog(nSeverity, std::string(csTraceMsg) + strMessage);
	m_strMessage = strMessage + " " + m_strResult;

	if (nAlgoOrder == ORDER_NOT_USE)
	{
		FactoryLog();
	}
	else
	{
		m_strLower = st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[nAlgoOrder];
		m_strMeasured = st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[nAlgoOrder];
		FactoryLog();
		m_strLower = "";
		m_strMeasured = "";
	}

	SetProcessMutexLock(KEY_REPORT, false);
}

void CCameraCheck::CheckStopCam(bool bRes, int nIndexQPhoneSlot, int frontRear, char* error_code, std::string pics_name, std::string msg, int check, int order)
{
	Report(bRes, pics_name + msg, nIndexQPhoneSlot, error_code, order);
	SetPICSDataWithLock(pics_name, bRes ? "PASS" : "FAIL", nIndexQPhoneSlot);
	if (!bRes && check == 1)
	{
		Qisda_StopCam(frontRear, nIndexQPhoneSlot);
		m_b_isSlotPass[nIndexQPhoneSlot] = false;
	}
}

void CCameraCheck::ReportTimeoutHandler(int nIndexQPhoneSlot, int nQisdaProcess)
{
	int slot = nIndexQPhoneSlot;//short naming
	switch (nQisdaProcess)
	{
	case REAR_CAMERA_MODULE_INFO:
		m_csArrRearFuseID[nIndexQPhoneSlot] = _T("000000000000");
		TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + _T(" Read Module Info Timeout.")));
		break;
	case REAR_CAMERA_STOP:
		Report(false, "Rear Camera shutdown Timeout!", nIndexQPhoneSlot, FunErr_Rear_Camera_Test_Shutdown_Fail);
		m_b_isSlotPass[nIndexQPhoneSlot] = false;
		break;

	case REAR_CAMERA_START:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Start_Fail,              "", "Start Rear Camera Timeout!");
		break;
	case REAR_CAMERA_CAPTURE_LP100:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_Macro_Capture_Fail, "", "Rear Camera shot LP100 Timeout!");
		break;
	case REAR_CAMERA_CAPTURE_MTF:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_Capture_Fail,       "", "Rear Camera shot MTF chart Timeout!");
		break;

	case REAR_CAMERA_ALGO_BLEMISH:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_Blemish_Fail, PICS_REAR_BLEMISH_RESULT, " Timeout!", m_nCheckRearBlemish, ORDER_BLEMISH_REAR);
		break;
	case REAR_CAMERA_ALGO_SHADING:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_Shading_Fail, PICS_REAR_SHADING_RESULT, " Timeout!", m_nCheckRearShading, ORDER_SHADING_REAR);
		break;
	case REAR_CAMERA_ALGO_COLOR:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_Color_Fail,   PICS_REAR_COLOR_RESULT,   " Timeout!", m_nCheckRearColor,   ORDER_COLOR_REAR);
		break;
	case REAR_CAMERA_ALGO_MTF:
		CheckStopCam(false, slot, REAR_CAMERA, FunErr_Rear_Camera_Test_MTF_Fail,     PICS_REAR_MTF_RESULT,     " Timeout!", m_nCheckRearMTF,     ORDER_MTF_REAR);
		break;

	case FRONT_CAMERA_MODULE_INFO:
		m_csArrFrontFuseID[nIndexQPhoneSlot] = _T("000000000000");
		TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + _T(" Read Module Info Timeout.")));
		break;
	case FRONT_CAMERA_STOP:
		Report(false, "Front Camera shutdown Timeout!", nIndexQPhoneSlot, FunErr_Front_Camera_Test_Shutdown_Fail);
		m_b_isSlotPass[nIndexQPhoneSlot] = false;
		break;

	case FRONT_CAMERA_START:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Start_Fail,              "", "Start Front Camera Timeout!");
		break;
	case FRONT_CAMERA_CAPTURE_LP100:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_Macro_Capture_Fail, "", "Front Camera shot LP picture Timeout!");
		break;
	case FRONT_CAMERA_CAPTURE_MTF:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_Capture_Fail,       "", "Front Camera shot MTF picture Timeout!");
		break;

	case FRONT_CAMERA_ALGO_BLEMISH:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_Blemish_Fail, PICS_FRONT_BLEMISH_RESULT, " Timeout!", m_nCheckFrontBlemish, ORDER_BLEMISH_FRONT);
		break;
	case FRONT_CAMERA_ALGO_SHADING:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_Shading_Fail, PICS_FRONT_SHADING_RESULT, " Timeout!", m_nCheckFrontShading, ORDER_SHADING_FRONT);
		break;
	case FRONT_CAMERA_ALGO_COLOR:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_Color_Fail,   PICS_FRONT_COLOR_RESULT,   " Timeout!", m_nCheckFrontColor,   ORDER_COLOR_FRONT);
		break;
	case FRONT_CAMERA_ALGO_MTF:
		CheckStopCam(false, slot, FRONT_CAMERA, FunErr_Front_Camera_Test_MTF_Fail,     PICS_FRONT_MTF_RESULT,     " Timeout!", m_nCheckFrontMTF,     ORDER_MTF_FRONT);
		break;

	case SHOW_RESULT:
		m_strErrorCode = "-";
		m_strMessage = "Camera Test Pass";
		SetColorOnDeviceScreen(nIndexQPhoneSlot, COLOR_GREEN);
		Report(true, m_strMessage, nIndexQPhoneSlot);
		break;
	}
}

bool CCameraCheck::Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse)
{
	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD nPipeSize = 1024;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		TraceLog(MSG_ERROR, "ERROR: CreatePipe fail!\r\n");
		return false;
	}

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	SetCurrentDirectory(WorkDir);

	bool isOk = false;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;
	CString Command = path;
	if (!param.IsEmpty())
	{
		Command = Command + _T(" ") + param;
	}
	TRACE(_T("exec: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, msTimeout);
		isOk = true;
		if (hasResponse)
		{
			DWORD bytesRead;
			char message[512];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, 511, &bytesRead, NULL);
			param = (strlen(message) > 0) ? param = CA2T(message) : _T("");
		}
	}
	else
	{
		isOk = false;
		char szErrorMsg[100];
		memset(szErrorMsg, 0, sizeof(szErrorMsg));
		strcpy(szErrorMsg, "ERROR: Execute fail!\r\ncmd:");
		strcat(szErrorMsg, Command);
		TraceLog(MSG_ERROR, szErrorMsg);
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	SetCurrentDirectory(szNowPath);

	return isOk;
}

static bool MkDir(CString strDesPath)
{
	CFileFind cfilefind;
	return (cfilefind.FindFile(strDesPath) || CreateDirectory(strDesPath, NULL));
}

CString CCameraCheck::MkDirImages(int nQPhoneNumber)
{
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString strDesPath;
	if (nQPhoneNumber == IMG_CODE_0 || nQPhoneNumber == IMG_CODE_1 || nQPhoneNumber == IMG_CODE_2 || nQPhoneNumber == IMG_CODE_3 || nQPhoneNumber == IMG_CODE_4)
	{
		strDesPath = pthToolDir + _T("\\Img");
		strDesPath += _T("\\") + m_csArrQPhoneName[nQPhoneNumber];
	}
	else if (nQPhoneNumber == IMG_CODE_ROOT || nQPhoneNumber == IMG_CODE_BG)
	{
		strDesPath = pthToolDir + _T("\\Img");
	}
	if (!MkDir(strDesPath))
		TraceLog(MSG_INFO, "Create folder Img Failed!");
	return strDesPath;
}

bool CCameraCheck::MkDirQPhones(CString strDesPath)
{
	if (!MkDir(strDesPath))
	{
		TraceLog(MSG_INFO, "Create QPHONE directory Failed!");
		return false;
	}
	for (int i = 0; i <= m_i_QPhoneMultiCtrl; i++)
	{
		if (!MkDir(strDesPath + _T("\\") + m_csArrQPhoneName[i]))
		{
			TraceLog(MSG_INFO, "Create QPHONE directory Failed!");
			return false;
		}
	}
	return true;
}

bool CCameraCheck::MkDirResult()
{
	if (!MkDirQPhones(RESULT_PASS_FOLDER)) return false;
	if (!MkDirQPhones(RESULT_FAIL_FOLDER)) return false;
	return true;
}

bool CCameraCheck::InitialTestParameter()
{
	// Qisda Tool Folder
	::GetModuleFileName(NULL, m_csQisdaToolFolder.GetBuffer(MAX_PATH), MAX_PATH);
	m_csQisdaToolFolder.ReleaseBuffer();
	m_csQisdaToolFolder = m_csQisdaToolFolder.Left(m_csQisdaToolFolder.ReverseFind('\\'));

	// Qisda Image EXE path
	m_csQisdaImgExe_Rear_Path = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear.exe");
	m_csQisdaImgExe_Front_Path = m_csQisdaToolFolder + _T("\\QisdaIMG_Front.exe");
	m_csArrQisdaImgExe_Rear_Path[0] = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear.exe");
	m_csArrQisdaImgExe_Rear_Path[1] = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear_1.exe");
	m_csArrQisdaImgExe_Rear_Path[2] = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear_2.exe");
	m_csArrQisdaImgExe_Rear_Path[3] = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear_3.exe");
	m_csArrQisdaImgExe_Rear_Path[4] = m_csQisdaToolFolder + _T("\\QisdaIMG_Rear_4.exe");
	m_csArrQisdaImgExe_Front_Path[0] = m_csQisdaToolFolder + _T("\\QisdaIMG_Front.exe");
	m_csArrQisdaImgExe_Front_Path[1] = m_csQisdaToolFolder + _T("\\QisdaIMG_Front_1.exe");
	m_csArrQisdaImgExe_Front_Path[2] = m_csQisdaToolFolder + _T("\\QisdaIMG_Front_2.exe");
	m_csArrQisdaImgExe_Front_Path[3] = m_csQisdaToolFolder + _T("\\QisdaIMG_Front_3.exe");
	m_csArrQisdaImgExe_Front_Path[4] = m_csQisdaToolFolder + _T("\\QisdaIMG_Front_4.exe");

	// Qisda Image BMP path
	m_csArrQisdaImgBMP_Front_Path[QPHONE_SERIAL_0] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[QPHONE_SERIAL_0] + _T("\\QQ_Front.bmp");
	for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		m_csArrQisdaImgBMP_Front_Path[i] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[i] + _T("\\QQ_Front.bmp");
	m_csArrQisdaImgBMP_Rear_Path[QPHONE_SERIAL_0] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[QPHONE_SERIAL_0] + _T("\\QQ_Rear.bmp");
	for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		m_csArrQisdaImgBMP_Rear_Path[i] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[i] + _T("\\QQ_Rear.bmp");

	// Qisda Image Debug path
	m_csArrQisdaImgDeBug_Path[QPHONE_SERIAL_0] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[QPHONE_SERIAL_0] + _T("\\Debug.txt");
	for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		m_csArrQisdaImgDeBug_Path[i] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[i] + _T("\\Debug.txt");

	// Qisda Image Result path
	m_csArrQisdaImgResult_Path[QPHONE_SERIAL_0] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[QPHONE_SERIAL_0] + _T("\\Result.txt");
	for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
		m_csArrQisdaImgResult_Path[i] = m_csQisdaToolFolder + _T("\\Img\\") + m_csArrQPhoneName[i] + _T("\\Result.txt");

	// Qisda Image Sample path
	m_csArrQisdaImgSample_Path[REAR_LP][0]   = m_csQisdaToolFolder + _T("\\Img\\SAMPLE\\Rear_LP.jpg");
	m_csArrQisdaImgSample_Path[REAR_MTF][0]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE\\Rear_MTF.jpg");
	m_csArrQisdaImgSample_Path[FRONT_LP][0]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE\\Front_LP.jpg");
	m_csArrQisdaImgSample_Path[FRONT_MTF][0] = m_csQisdaToolFolder + _T("\\Img\\SAMPLE\\Front_MTF.jpg");

	m_csArrQisdaImgSample_Path[REAR_LP][1]   = m_csQisdaToolFolder + _T("\\Img\\SAMPLE1\\Rear_LP.jpg");
	m_csArrQisdaImgSample_Path[REAR_MTF][1]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE1\\Rear_MTF.jpg");
	m_csArrQisdaImgSample_Path[FRONT_LP][1]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE1\\Front_LP.jpg");
	m_csArrQisdaImgSample_Path[FRONT_MTF][1] = m_csQisdaToolFolder + _T("\\Img\\SAMPLE1\\Front_MTF.jpg");

	m_csArrQisdaImgSample_Path[REAR_LP][2]   = m_csQisdaToolFolder + _T("\\Img\\SAMPLE2\\Rear_LP.jpg");
	m_csArrQisdaImgSample_Path[REAR_MTF][2]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE2\\Rear_MTF.jpg");
	m_csArrQisdaImgSample_Path[FRONT_LP][2]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE2\\Front_LP.jpg");
	m_csArrQisdaImgSample_Path[FRONT_MTF][2] = m_csQisdaToolFolder + _T("\\Img\\SAMPLE2\\Front_MTF.jpg");

	m_csArrQisdaImgSample_Path[REAR_LP][3]   = m_csQisdaToolFolder + _T("\\Img\\SAMPLE3\\Rear_LP.jpg");
	m_csArrQisdaImgSample_Path[REAR_MTF][3]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE3\\Rear_MTF.jpg");
	m_csArrQisdaImgSample_Path[FRONT_LP][3]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE3\\Front_LP.jpg");
	m_csArrQisdaImgSample_Path[FRONT_MTF][3] = m_csQisdaToolFolder + _T("\\Img\\SAMPLE3\\Front_MTF.jpg");

	m_csArrQisdaImgSample_Path[REAR_LP][4]   = m_csQisdaToolFolder + _T("\\Img\\SAMPLE4\\Rear_LP.jpg");
	m_csArrQisdaImgSample_Path[REAR_MTF][4]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE4\\Rear_MTF.jpg");
	m_csArrQisdaImgSample_Path[FRONT_LP][4]  = m_csQisdaToolFolder + _T("\\Img\\SAMPLE4\\Front_LP.jpg");
	m_csArrQisdaImgSample_Path[FRONT_MTF][4] = m_csQisdaToolFolder + _T("\\Img\\SAMPLE4\\Front_MTF.jpg");

	// Initail Algo Spec content string and Algo Measured content string
	for (int i = 0; i <= 4; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			st_QPhone_Info[i].m_csArrAlgoSpec[j] = _T("");
			st_QPhone_Info[i].m_csArrAlgoMeasured[j] = _T("");
		}
	}
	return true;
}

bool CCameraCheck::Qisda_StartCam(int frontRear, int nQPhoneNumber)  //normal
{
	/* setprop for AWB/AF log */
	char szOut[4096] = {0};
	char szErr[4096] = {0};

	CString csCommand(_T("shell setprop persist.camera.mobicat 2"));
	ExecAdbOut(csCommand, nQPhoneNumber, szOut, szErr);
	CString csMsg = _T("");
	csMsg.Format(_T("adb %s ([Output] %s [ErrorCode] %s)"), csCommand, szOut, szErr);
	TraceLog(MSG_INFO, std::string(csMsg), nQPhoneNumber);

	csCommand = _T("shell setprop persist.camera.stats.debug.mask 3080192");
	ExecAdbOut(csCommand, nQPhoneNumber, szOut, szErr);
	csMsg.Format(_T("Cmd: adb %s ([Output] %s [ErrorCode] %s)"), csCommand, szOut, szErr);
	TraceLog(MSG_INFO, std::string(csMsg), nQPhoneNumber);

	char* szFrontRear = (frontRear == FRONT_CAMERA) ? "front" : "back";
	char szBuff[FTD_BUF_SIZE] = {0};
	if (!CallFtdFunction(FUNC_CAM_START, m_nArrQPhonePort[nQPhoneNumber], m_nFtdTimeOut, szFrontRear, szBuff))
	{
		TraceLog(MSG_ERROR, "Qisda_StartCam fail", nQPhoneNumber);
		return false;
	}
	return true;
}

bool CCameraCheck::Qisda_StopCam(int frontRear, int nQPhoneNumber)  //normal
{
	char szFrontRear[64] = {0};
	char szBuff[FTD_BUF_SIZE] = {0};
	if (!(CallFtdFunction(FUNC_CAM_STOP, m_nArrQPhonePort[nQPhoneNumber],  m_nFtdTimeOut, szFrontRear, szBuff)))//LION 2012.4.18
	{
		TraceLog(MSG_ERROR, "Qisda_StopCam FAIL", nQPhoneNumber);
		return false;
	}
	TraceLog(MSG_INFO, "Qisda_StopCam Successful", nQPhoneNumber);
	return true;
}

bool CCameraCheck::Qisda_CapturePic(int i_shot_number, int frontRear, int nQPhoneNumber)  //normal
{
	/* Remove all [snapshot_*.jpg] */
	CString csCommandShell(_T("shell rm -r /data/snapshot_*.jpg"));
	char szOutPullCommand[4096] = {0};
	char szErrorCodePullCommand[4096] = {0};
	ExecAdbOut(csCommandShell, nQPhoneNumber, szOutPullCommand, szErrorCodePullCommand);

	//Get picture
	char sz_i_shot_number[16] = {0};
	itoa(i_shot_number, sz_i_shot_number, 10);
	char szCameraName[64] = {0};
	if (frontRear == 0)
		strcpy(szCameraName, "front,normal,");
	else
		strcpy(szCameraName, "back,normal,");
	strcat(szCameraName, sz_i_shot_number);

	// Add option parameter for shot type
	if (i_shot_number == REAR_MTF_PIC && frontRear == REAR_CAMERA)
		strcat(szCameraName, ",MTF");
	else if (i_shot_number == REAR_LP100_PIC && frontRear == REAR_CAMERA)
		strcat(szCameraName, ",Shading");
	else if (i_shot_number == FRONT_MTF_PIC && frontRear == FRONT_CAMERA)
		strcat(szCameraName, ",MTF");
	else if (i_shot_number == FRONT_LP100_PIC && frontRear == FRONT_CAMERA)
		strcat(szCameraName, ",Shading");
	// Add option parameter for Sapporo AWB
	strcat(szCameraName, ",Auto");

	std::string TraceMsg;
	TraceMsg = "ShotCAM parameter: ";
	TraceMsg += szCameraName;
	TraceLog(MSG_INFO, TraceMsg, nQPhoneNumber);

	//Show background picture
	CString csDirectory = MkDirImages(IMG_CODE_BG);
	CString csBGDImgName;
	csBGDImgName.Format(_T("%s%s"), _T("background"), _T(".jpg"));
	csDirectory += _T("\\");
	CString csPath;
	csPath.Format("%s%s", csDirectory, csBGDImgName);
	ShowPicture(false, csPath.GetString());

	// target remove delay 2s before CAMshot, tool add 2s
	Sleep(2000);
	char szBuff[FTD_BUF_SIZE] = {0};
	if (!(CallFtdFunction(FUNC_CAM_CAPTURE, m_nArrQPhonePort[nQPhoneNumber], m_nFtdTimeOut, szCameraName, szBuff)))
	{
		TraceLog(MSG_ERROR, "ERROR: capture picture fail", nQPhoneNumber);
		return false;
	}
	Sleep(2000);//target remove delay 2s after CAMshot, tool add 2s

	// get pic from target to PC  (adb pull /sdcard/DCIM/Camera)
	char szImgCount[5];
	memset(szImgCount, 0 , sizeof(szImgCount));
	_itoa_s(i_shot_number, szImgCount, 10);
	CString csImgName;
	csImgName.Format(_T("%s%s%s"), _T("snapshot_"), szImgCount, _T(".jpg "));
	CString csCommandPull;
	CString csQPhoneImgFolder = MkDirImages(nQPhoneNumber);
	csCommandPull.Format(_T("%s%s%s"), _T("pull /data/"), csImgName, csQPhoneImgFolder);
	if (!ExecAdbOut(csCommandPull, nQPhoneNumber, szOutPullCommand, szErrorCodePullCommand))
	{
		TraceLog(MSG_ERROR, "ERROR: pull picture fail", nQPhoneNumber);
		return false;
	}

	//Show Picture
	CString csQPhoneImgName;
	csQPhoneImgName.Format(_T("%s%s%s"), _T("snapshot_"), szImgCount, _T(".jpg"));
	CString csQPhoneImgPath;
	csQPhoneImgPath.Format(_T("%s\\%s"), csQPhoneImgFolder, csQPhoneImgName);
	TraceLog(MSG_INFO, std::string(_T("Image: ") + _T(csQPhoneImgPath)), nQPhoneNumber);
	m_csArrQisdaImg_Path[nQPhoneNumber] = csQPhoneImgPath;
	ShowPicture(false, m_csArrQisdaImg_Path[nQPhoneNumber].GetString());

	//Get Picture Size
	CFileStatus rStatus;
	rStatus.m_size = 0 ;
	CFile::GetStatus(csQPhoneImgPath, rStatus);
	ULONGLONG nSize = rStatus.m_size;
	if (nSize == 0)
	{
		TraceLog(MSG_ERROR, "ERROR: picture size = 0", nQPhoneNumber);
		return false;
	}
	return true;
}

bool CCameraCheck::Qisda_ReadModuleInfo(int frontRear, int nDeviceNumber, char* szBuff)
{
	bool bRetune;
	if (frontRear == REAR_CAMERA)
	{
		bRetune = CallFtdFunction(FUNC_REAR_MODULE_INFO, m_nArrQPhonePort[nDeviceNumber], m_nFtdTimeOut, "", szBuff);
	}
	else if (frontRear == FRONT_CAMERA)
	{
		bRetune = CallFtdFunction(FUNC_FRONT_MODULE_INFO, m_nArrQPhonePort[nDeviceNumber], m_nFtdTimeOut, "", szBuff);
	}
	return bRetune;
}

bool CCameraCheck::Qisda_WriteModuleInfo(int frontRear, int nDeviceNumber, char* szInBuff, char* szOutBuff)
{
	bool bRetune;
	if (frontRear == REAR_CAMERA)
		bRetune = true;
	else if (frontRear == FRONT_CAMERA)
		bRetune = CallFtdFunction(FUNC_FRONT_FUSE_ID_WRITE, m_nArrQPhonePort[nDeviceNumber], m_nFtdTimeOut, szInBuff, szOutBuff);
	return bRetune;
}

bool CCameraCheck::Run_Camera_Verification(int nType)
{
	//Initail FilePath and FileName
	InitialTestParameter();
	//Create folder for Testing
	MkDirImages(IMG_CODE_ROOT);
	MkDirResult();
	//Clear Old Pictures
	ClearOldPictures();

	//Split Rear Algo List
	CString csRearAlgoList = str_RearAlgoList.c_str();
	int nReadRearFuseID, nAlgoRearBlemish, nAlgoRearShading, nAlgoRearColor, nAlgoRearMTF;
	sscanf(csRearAlgoList, "%d, %d, %d, %d, %d", &nReadRearFuseID, &nAlgoRearBlemish, &nAlgoRearShading, &nAlgoRearColor, &nAlgoRearMTF);
	//Split Rear Check List
	CString csRearCheckList = str_RearCheckList.c_str();
	sscanf(csRearCheckList, "%d, %d, %d, %d, %d", &m_nCheckRearFuseID, &m_nCheckRearBlemish, &m_nCheckRearShading, &m_nCheckRearColor, &m_nCheckRearMTF);

	if (m_i_QPhoneMultiCtrl == 0) m_b_isSlotPass[0] = true;

	//rear camera test
	if (nAlgoRearBlemish >= 1 || nAlgoRearShading >= 1)
		MoveCoverBackCamLP100();								//Move LP100 cover
	if (nReadRearFuseID == 1)
		SelectExecution(REAR_CAMERA_MODULE_INFO, nType);		//Get Rear Camera Module Info
	if (nAlgoRearBlemish >= 1 || nAlgoRearShading >= 1 || nAlgoRearColor >= 1 || nAlgoRearMTF >= 1)
		SelectExecution(REAR_CAMERA_START, nType);				//Start Rear Camera
	if (nAlgoRearBlemish >= 1 || nAlgoRearShading >= 1 || nAlgoRearColor >= 1)
		SelectExecution(REAR_CAMERA_CAPTURE_LP100, nType);		//Capture Rear LP100
	MoveAwayBackCamLP100();										//move Rear LP100 away
	if (nAlgoRearBlemish == 1)
		SelectExecution(REAR_CAMERA_ALGO_BLEMISH, nType);		//Qisda_Algo_Blemish_Rear
	if (nAlgoRearShading == 1)
		SelectExecution(REAR_CAMERA_ALGO_SHADING, nType);		//Qisda_Algo_Shading_Rear
	if (nAlgoRearColor == 1)
		SelectExecution(REAR_CAMERA_ALGO_COLOR, nType);			//Qisda_Algo_Color_Rear
	if (nAlgoRearMTF >= 1)
		SelectExecution(REAR_CAMERA_CAPTURE_MTF, nType);		//Shot Rear MTF chart
	if (nAlgoRearMTF == 1)
		SelectExecution(REAR_CAMERA_ALGO_MTF, nType);			//Qisda_Algo_MTF_Rear
	if (nAlgoRearMTF == 2)
		SelectExecution(REAR_CAMERA_DIALOG_MTF, nType);			//Qisda_Dialog_MTF_Rear
	if (nAlgoRearBlemish >= 1 || nAlgoRearShading >= 1 || nAlgoRearColor >= 1 || nAlgoRearMTF >= 1)
		SelectExecution(REAR_CAMERA_STOP, nType);				//Stop Rear Camera

	//Split Front Alog List
	CString csFrontAlgoList = str_FrontAlgoList.c_str();
	int nReadFrontFuseID, nAlogFrontBlemish, nAlogFrontShading, nAlogFrontColor, nAlogFrontMTF;
	sscanf(csFrontAlgoList, "%d, %d, %d, %d, %d", &nReadFrontFuseID, &nAlogFrontBlemish, &nAlogFrontShading, &nAlogFrontColor, &nAlogFrontMTF);
	//Split Front Check List
	CString csFrontCheckList = str_FrontCheckList.c_str();
	sscanf(csFrontCheckList, "%d, %d, %d, %d, %d", &m_nCheckFrontFuseID, &m_nCheckFrontBlemish, &m_nCheckFrontShading, &m_nCheckFrontColor, &m_nCheckFrontMTF);

	//front camera test
	if (nAlogFrontBlemish >= 1 || nAlogFrontShading >= 1)
		MoveCoverFrontCamLP100();											//Move Front LP100 cover
	if (nReadFrontFuseID == 1)
		SelectExecution(FRONT_CAMERA_MODULE_INFO, nType);					//Get Front Camera Module Info type-1
	if (nReadFrontFuseID == 2)
		SelectExecution(FRONT_CAMERA_FUSE_ID_READ_WRITE, nType);			//Get Front Camera Module Info type-2
	if (nReadFrontFuseID == 3)
		SelectExecution(FRONT_CAMERA_FUSE_ID_READ_WRITE_NEW, nType);		//Get Front Camera Module Info type-3
	if (nAlogFrontBlemish >= 1 || nAlogFrontShading >= 1 || nAlogFrontColor >= 1 || nAlogFrontMTF >= 1)
		SelectExecution(FRONT_CAMERA_START, nType);							//Start Front Camera
	if (nAlogFrontBlemish >= 1 || nAlogFrontShading >= 1 || nAlogFrontColor >= 1)
		SelectExecution(FRONT_CAMERA_CAPTURE_LP100, nType);					//Shot Front LP100
	MoveAwayFrontCamLP100();												//Move Front LP100 away
	if (nAlogFrontBlemish == 1)
		SelectExecution(FRONT_CAMERA_ALGO_BLEMISH, nType);					//Qisda_Algo_Blemish_Front
	if (nAlogFrontShading == 1)
		SelectExecution(FRONT_CAMERA_ALGO_SHADING, nType);					//Qisda_Algo_Shading_Front
	if (nAlogFrontColor == 1)
		SelectExecution(FRONT_CAMERA_ALGO_COLOR, nType);					//Qisda_Algo_Color_Front()
	if (nAlogFrontMTF >= 1)
		SelectExecution(FRONT_CAMERA_CAPTURE_MTF, nType);					//Shot Front MTF chart
	if (nAlogFrontMTF == 1)
		SelectExecution(FRONT_CAMERA_ALGO_MTF, nType);						//Qisda_Algo_MTF_Front
	if (nAlogFrontBlemish >= 1 || nAlogFrontShading >= 1 || nAlogFrontColor >= 1 || nAlogFrontMTF >= 1)
		SelectExecution(FRONT_CAMERA_STOP, nType);							//Stop Front Camera

	SelectExecution(SHOW_RESULT, nType);									//Report result

	if (m_bTestMode == true)
		::AfxMessageBox(_T("<<NOT FOR RELEASE>>\n This is TestMode with sample pictures !"));

	// Return result
	if (m_i_QPhoneMultiCtrl != 0)
		return m_b_isSlotPass[1] || m_b_isSlotPass[2] || m_b_isSlotPass[3] || m_b_isSlotPass[4];
	return m_b_isSlotPass[0];
}

void CCameraCheck::SelectExecution(QISDA_PROCESS nQisdaProcess, int nType)
{
	if		( nType == TYPE_SINGLE )	Qisda_SingleExecution(nQisdaProcess);
	else if ( nType == TYPE_MULTI )		Qisda_MultiExecution(nQisdaProcess);
}

void CCameraCheck::Qisda_SingleExecution(QISDA_PROCESS nQisdaProcess)
{
	bool isPass = m_b_isSlotPass[QPHONE_SERIAL_0];
	if ( isPass ) Qisda_QPhoneProcess( QPHONE_SERIAL_0, nQisdaProcess);
}

bool CCameraCheck::ExecAdbOut(CString Command, int nQPhoneNumber, char* output, char* ErrorMsg)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorMsg, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorMsg, "ERROR: CreatePipe fail!");
		return false;
	}

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	if (m_i_QPhoneMultiCtrl == 0)
		Command = _T("\"") + path_adb + _T("\" ") + Command;
	else
		Command = _T("\"") + path_adb + _T("\" -s ") + m_csArrQPhoneName[nQPhoneNumber] +  + _T(" ") + Command;
	TraceLog(MSG_INFO, std::string(_T("Cmd: ") + _T(Command)), nQPhoneNumber);
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, ADB_CMD_TIMEOUT);
		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorMsg, "ERROR: Adb timeout");
		}
		else
		{
			isOk = true;
			DWORD bytesRead;
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';
			strcpy(output, message);
			strcpy(ErrorMsg, "Adb command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorMsg, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return isOk;
}

bool CCameraCheck::ExecQisdaIMGExe_Rear(CString csAlgo, int nQPhoneNumber, char* ErrorMsg)
{
	if (_taccess(m_csArrQisdaImgExe_Rear_Path[nQPhoneNumber], 0) == -1)
	{
		strcpy(ErrorMsg, "ERROR: No QisdaIMG_Rear.exe exist!");
		return false;
	}

	bool isOk = false;
	HANDLE hRead = NULL;
	HANDLE hWrite = NULL;
	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	CString csQisdaIMGExe_Rear_CMD;
	csQisdaIMGExe_Rear_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Rear_Path[nQPhoneNumber],
	                              csAlgo, m_csArrQisdaImg_Path[nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Rear_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);

	if ((_taccess(m_csArrQisdaImgSample_Path[REAR_LP][nQPhoneNumber], 0) != -1) && ((csAlgo.Compare(_T("blemish")) == 0) || (csAlgo.Compare(_T("shading")) == 0) || (csAlgo.Compare(_T("COLORSHADING")) == 0)))
	{
		csQisdaIMGExe_Rear_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Rear_Path[nQPhoneNumber],
		                              csAlgo, m_csArrQisdaImgSample_Path[REAR_LP][nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Rear_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);
		m_bTestMode = true;
	}
	if ((_taccess(m_csArrQisdaImgSample_Path[REAR_MTF][nQPhoneNumber], 0) != -1) && ((csAlgo.Compare(_T("MTF")) == 0)))
	{
		csQisdaIMGExe_Rear_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Rear_Path[nQPhoneNumber],
		                              csAlgo, m_csArrQisdaImgSample_Path[REAR_MTF][nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Rear_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);
		m_bTestMode = true;
	}
	TraceLog(MSG_INFO, "cmd: " + std::string(csQisdaIMGExe_Rear_CMD), nQPhoneNumber);
	if (::CreateProcess(NULL, csQisdaIMGExe_Rear_CMD.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 30 * 1000); // timeout in 30 seconds
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorMsg, "ERROR: QisdaIMG_Rear.exe timeout");
		}
		else
		{
			isOk = true;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorMsg, "ERROR: Execute QisdaIMG_Rear.exe fail!");
	}
	csQisdaIMGExe_Rear_CMD.ReleaseBuffer();
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;
	return isOk;
}

bool CCameraCheck::ExecQisdaIMGExe_Front(CString csAlgo, int nQPhoneNumber, char* ErrorMsg)
{
	if (_taccess(m_csArrQisdaImgExe_Front_Path[nQPhoneNumber], 0) == -1)
	{
		strcpy(ErrorMsg, "ERROR: No QisdaIMG_Front.exe exist!");
		return false;
	}

	bool isOk = false;
	HANDLE hRead = NULL;
	HANDLE hWrite = NULL;
	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	CString csQisdaIMGExe_Front_CMD;
	csQisdaIMGExe_Front_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Front_Path[nQPhoneNumber],
	                               csAlgo,	m_csArrQisdaImg_Path[nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Front_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);

	if ((_taccess(m_csArrQisdaImgSample_Path[FRONT_LP][nQPhoneNumber], 0) != -1) && ((csAlgo.Compare(_T("blemish")) == 0) || (csAlgo.Compare(_T("shading")) == 0) || (csAlgo.Compare(_T("COLORSHADING")) == 0)))
	{
		csQisdaIMGExe_Front_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Front_Path[nQPhoneNumber],
		                               csAlgo,	m_csArrQisdaImgSample_Path[FRONT_LP][nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Front_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);
		m_bTestMode = true;
	}
	if ((_taccess(m_csArrQisdaImgSample_Path[FRONT_MTF][nQPhoneNumber], 0) != -1) && ((csAlgo.Compare(_T("MTF")) == 0)))
	{
		csQisdaIMGExe_Front_CMD.Format(_T("\"%s\" %s %s %s %s %s"), m_csArrQisdaImgExe_Front_Path[nQPhoneNumber],
		                               csAlgo,	m_csArrQisdaImgSample_Path[FRONT_MTF][nQPhoneNumber], m_csArrQisdaImgResult_Path[nQPhoneNumber], m_csArrQisdaImgBMP_Front_Path[nQPhoneNumber], m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);
		m_bTestMode = true;
	}
	TraceLog(MSG_INFO, std::string("cmd: " + csQisdaIMGExe_Front_CMD), nQPhoneNumber);
	if (::CreateProcess(NULL, csQisdaIMGExe_Front_CMD.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 30 * 1000); // timeout in 30 seconds
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorMsg, "ERROR: QisdaIMG_Front.exe timeout");
		}
		else
		{
			isOk = true;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorMsg, "ERROR: Execute QisdaIMG_Front.exe fail!");
	}
	csQisdaIMGExe_Front_CMD.ReleaseBuffer();
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;

	return isOk;
}

bool CCameraCheck::SendResultToPICS(char* szAlgo, CString szFilePath, int nQPhoneNumber)
{
	SetProcessMutexLock(KEY_PICS, true);

	CStdioFile obj_stdioFile;
	if (!obj_stdioFile.Open(szFilePath, CFile::modeRead | CFile::modeNoTruncate | CFile::shareDenyNone, NULL))
	{
		TraceLog(MSG_ERROR, "SendResultToPICS Fail, can't open file");
		return false;
	}
	CString str_PassFail;
	CString str_Info;
	CString str_s1;
	CString str_s2;
	CString str_s3;
	CString str_s4;
	CString str_s5;
	CString str_s6;
	CString str_s7;
	CString str_s8;
	CString str_s9;
	try
	{
		obj_stdioFile.ReadString(str_PassFail);
		obj_stdioFile.ReadString(str_Info);
		obj_stdioFile.ReadString(str_s1);
		obj_stdioFile.ReadString(str_s2);
		obj_stdioFile.ReadString(str_s3);
		obj_stdioFile.ReadString(str_s4);
		obj_stdioFile.ReadString(str_s5);
		obj_stdioFile.ReadString(str_s6);
		obj_stdioFile.ReadString(str_s7);
		obj_stdioFile.ReadString(str_s8);
		obj_stdioFile.ReadString(str_s9);
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	obj_stdioFile.Close();

	/*send data to PICS Server, Rear Camera*/
	if (strcmp(szAlgo , "Rear_MTF") == 0)
	{
		SetPICSData("MTF_Rear_Center", (LPCTSTR)str_s1, nQPhoneNumber);
		SetPICSData("MTF_Rear_LU", (LPCTSTR)str_s2, nQPhoneNumber);
		SetPICSData("MTF_Rear_RU", (LPCTSTR)str_s3, nQPhoneNumber);
		SetPICSData("MTF_Rear_LB", (LPCTSTR)str_s4, nQPhoneNumber);
		SetPICSData("MTF_Rear_RB", (LPCTSTR)str_s5, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_MTF_REAR].Format(_T("%s , %s"), str_s7, str_s8);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_MTF_REAR].Format(_T("%s / %s / %s / %s / %s"), str_s1, str_s2, str_s3, str_s4, str_s5);
		return true;
	}
	if (strcmp(szAlgo, "Rear_Color") == 0)
	{
		SetPICSData("Color_Rear_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_COLOR_REAR].Format(_T("%s , %s"), str_s4, str_s5);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_COLOR_REAR].Format(_T("%s"), str_s1);
		
		return true;
	}
	if (strcmp(szAlgo, "Rear_Blemish") == 0)
	{
		SetPICSData("Blemish_Rear_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_BLEMISH_REAR].Format(_T("%s"), str_s3);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_BLEMISH_REAR].Format(_T("%s"), str_s1);
		if (!str_s1.IsEmpty())
		{
			int nBlemishValue = 0;
			sscanf((LPCTSTR)str_s1, "%d", &nBlemishValue);
		}
		return true;
	}
	if (strcmp(szAlgo, "Rear_Shading") == 0)
	{
		SetPICSData("Shading_Rear_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_SHADING_REAR].Format(_T("%s"), str_s3);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_SHADING_REAR].Format(_T("%s"), str_s1);
		return true;
	}
	/*send data to PICS Server, Front Camera*/
	if (strcmp(szAlgo, "Front_MTF") == 0)
	{
		SetPICSData("MTF_Front_Center", (LPCTSTR)str_s1, nQPhoneNumber);
		SetPICSData("MTF_Front_LU",     (LPCTSTR)str_s2, nQPhoneNumber);
		SetPICSData("MTF_Front_RU",     (LPCTSTR)str_s3, nQPhoneNumber);
		SetPICSData("MTF_Front_LB",     (LPCTSTR)str_s4, nQPhoneNumber);
		SetPICSData("MTF_Front_RB",     (LPCTSTR)str_s5, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_MTF_FRONT].Format(_T("%s , %s"), str_s7 , str_s8);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_MTF_FRONT].Format(_T("%s / %s / %s / %s / %s"), str_s1, str_s2, str_s3, str_s4, str_s5);
		return true;
	}
	if (strcmp(szAlgo, "Front_Color") == 0)
	{
		SetPICSData("Color_Front_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_COLOR_FRONT].Format(_T("%s , %s"), str_s4, str_s5);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_COLOR_FRONT].Format(_T("%s"), str_s1);
		
		return true;
	}
	if (strcmp(szAlgo, "Front_Blemish") == 0)
	{
		SetPICSData("Blemish_Front_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_BLEMISH_FRONT].Format(_T("%s"), str_s3);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_BLEMISH_FRONT].Format(_T("%s"), str_s1);
		if (!str_s1.IsEmpty())
		{
			int nBlemishValue = 0;
			sscanf((LPCTSTR)str_s1, "%d", &nBlemishValue);
		}
		return true;
	}
	if (strcmp(szAlgo, "Front_Shading") == 0)
	{
		SetPICSData("Shading_Front_Data", (LPCTSTR)str_s1, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoSpec[ORDER_SHADING_FRONT].Format(_T("%s"), str_s3, nQPhoneNumber);
		st_QPhone_Info[nQPhoneNumber].m_csArrAlgoMeasured[ORDER_SHADING_FRONT].Format(_T("%s"), str_s1);
		return true;
	}
	SetProcessMutexLock(KEY_PICS, false);
	return false;
}

bool CCameraCheck::GetResult(char* szAlgo, int nQPhoneNumber)
{
	CString strResultTxtPath;
	if (FindResultTxt(nQPhoneNumber, nWaitResultTime))
	{
		CFile cFile(m_csArrQisdaImgResult_Path[nQPhoneNumber], CFile::modeRead | CFile::modeNoTruncate);
		char readContent[5] = {0};
		cFile.SeekToBegin();
		cFile.Read(readContent, 4);
		cFile.Close();
		//if (strlen(readContent) < 4) return false;

		if (readContent[0] == 'P')
		{
			strResultTxtPath = BackupResultAndPicture(szAlgo, "PASS", nQPhoneNumber);
			if (!SendResultToPICS(szAlgo, strResultTxtPath, nQPhoneNumber)) return false;
			return true;
		}
		else
		{
			strResultTxtPath = BackupResultAndPicture(szAlgo, "FAIL", nQPhoneNumber);
			if (!SendResultToPICS(szAlgo, strResultTxtPath, nQPhoneNumber)) return false;
			return false;
		}
	}
	else
	{
		strResultTxtPath = BackupResultAndPicture(szAlgo, "FAIL", nQPhoneNumber);
		TraceLog(MSG_INFO, "Camera GetResult timeout!", nQPhoneNumber);
		return false;
	}
	return false;
}

bool CCameraCheck::FindResultTxt(int nQPhoneNumber, int nTimeout)
{
	bool bRes = false;
	CFileFind cFileFind;
	int nTryTimes = nTimeout / 100;
	for (int nFindTimes = 0; nFindTimes != nTryTimes; nFindTimes++)
	{
		if (cFileFind.FindFile(m_csArrQisdaImgResult_Path[nQPhoneNumber]))
		{
			bRes = true;
			break;
		}
		Sleep(100);
	}
	return bRes;
}

CString CCameraCheck::BackupResultAndPicture(char* szAlgo, char* flag, int nQPhoneNumber)
{
	//Set a String for Picasso When it is empty.
	CString csPicasso;
	if (m_i_QPhoneMultiCtrl == 0)
	{
		if (g_strPicasso.empty())
			csPicasso = _T("RD");
		else
			csPicasso = g_strPicasso.c_str();
	}
	else
	{
		if (m_csArrPicasso[nQPhoneNumber].IsEmpty() != 0)
			csPicasso.Format(_T("RD_Device%d"), nQPhoneNumber);
		else
			csPicasso.Format(_T("%s"), m_csArrPicasso[nQPhoneNumber]);
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString time;
	time.Format(_T("%d%d%d%d%d%d%d"), st.wYear, st.wMonth, st.wDay,
	            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	//Set Backup name
	CString strBackupName;
	if ((strcmp(szAlgo, "Rear_Blemish") == 0) ||
	    (strcmp(szAlgo, "Rear_Shading") == 0) ||
	    (strcmp(szAlgo, "Rear_Color") == 0) ||
	    (strcmp(szAlgo, "Rear_MTF") == 0))
	{
		strBackupName = csPicasso + _T("_") + m_csArrRearFuseID[nQPhoneNumber] + _T("_") + szAlgo + _T("_") + time;
	}
	else if (
	    (strcmp(szAlgo, "Front_Blemish") == 0) ||
	    (strcmp(szAlgo, "Front_Shading") == 0) ||
	    (strcmp(szAlgo, "Front_Color") == 0) ||
	    (strcmp(szAlgo, "Front_MTF") == 0))
	{
		strBackupName = csPicasso + _T("_") + m_csArrFrontFuseID[nQPhoneNumber] + _T("_") + szAlgo + _T("_") + time;
	}
	else
	{
		strBackupName = csPicasso + _T("_") + szAlgo + _T("_") + time;
	}

	CString strDesPath;
	if (strcmp(flag, _T("FAIL")) == 0)
		strDesPath  = RESULT_FAIL_FOLDER;
	else if (strcmp(flag, _T("ERROR")) == 0)
		strDesPath  = RESULT_ERROR_FOLDER;
	else
		strDesPath  = RESULT_PASS_FOLDER;

	//Backup Picture file
	CString strPicPath;
	strPicPath.Format(_T("%s\\%s\\%s.jpg"), strDesPath, m_csArrQPhoneName[nQPhoneNumber], strBackupName);
	CopyFile(m_csArrQisdaImg_Path[nQPhoneNumber], strPicPath , 1);
	TraceLog(MSG_INFO, std::string(_T("backup: ") + strPicPath), nQPhoneNumber);

	//Backup Debug file
	CString strLogPath;
	strLogPath.Format(_T("%s\\%s\\%s.log"), strDesPath, m_csArrQPhoneName[nQPhoneNumber], strBackupName);
	CopyFile(m_csArrQisdaImgDeBug_Path[nQPhoneNumber], strLogPath, 1);
	DeleteFile(m_csArrQisdaImgDeBug_Path[nQPhoneNumber]);
	TraceLog(MSG_INFO, std::string(_T("backup: ") + strLogPath), nQPhoneNumber);

	//Backup Result file
	CString strTxtPath;
	strTxtPath.Format(_T("%s\\%s\\%s.txt"), strDesPath, m_csArrQPhoneName[nQPhoneNumber], strBackupName);
	CopyFile(m_csArrQisdaImgResult_Path[nQPhoneNumber], strTxtPath, 1);
	DeleteFile(m_csArrQisdaImgResult_Path[nQPhoneNumber]);
	TraceLog(MSG_INFO, std::string(_T("backup: ") + strTxtPath), nQPhoneNumber);

	return strTxtPath;
}

bool CCameraCheck::Qisda_IMG_Algo_Front(CString AlgoType, char* ResultMatch, int nQPhoneNumber)
{
	TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Front Start")));
	char szErrorMsg[100] = {0};
	if (ExecQisdaIMGExe_Front(AlgoType, nQPhoneNumber, szErrorMsg))
	{
		if (GetResult(ResultMatch, nQPhoneNumber))
		{
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Front Result: Pass")));
			return true;
		}
		else
		{
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Front Result: Fail")));
			return false;
		}
	}
	else
	{
		BackupResultAndPicture(ResultMatch, "FAIL", nQPhoneNumber);
		TraceLog(MSG_INFO, szErrorMsg, nQPhoneNumber);
		return false;
	}
	TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Front End")));
	return true;
}

bool CCameraCheck::Qisda_IMG_Algo_Rear(CString AlgoType, char* ResultMatch, int nQPhoneNumber)
{
	TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Rear Start")));
	char szErrorMsg[100] = {0};
	if (ExecQisdaIMGExe_Rear(AlgoType, nQPhoneNumber, szErrorMsg))
	{
		if (GetResult(ResultMatch, nQPhoneNumber))
		{
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Rear Result: Pass")));
			return true;
		}
		else
		{
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Rear Result: Fail")));
			return false;
		}
	}
	else
	{
		BackupResultAndPicture(ResultMatch, "FAIL", nQPhoneNumber);
		TraceLog(MSG_INFO, szErrorMsg, nQPhoneNumber);
		return false;
	}
	TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nQPhoneNumber] + _T(" Algo_") + AlgoType + _T("_Rear End")));
	return true;
}

static const int MAX_IMAGE_NAMES = 6;
static std::string imageNames[MAX_IMAGE_NAMES] = {
	"snapshot_0.jpg",
	"snapshot_1.jpg",
	"snapshot_2.jpg",
	"snapshot_3.jpg",
	"QQ_Rear.bmp",
	"QQ_Front.bmp"
};
bool CCameraCheck::ClearOldPictures()
{
	// show background picture
	CString csBGDImgName;
	csBGDImgName.Format(_T("%s%s"), _T("background"), _T(".jpg"));
	CString csDirectory = MkDirImages(IMG_CODE_ROOT);
	csDirectory += _T("\\");
	CString csPath;
	csPath.Format(_T("%s%s"), csDirectory, csBGDImgName);
	ShowPicture(false, csPath.GetString());

	TraceLog(MSG_ERROR, "Start to Clear img folder Picture...");
	CString csPicPath;
	CFileFind cFileFind;
	for (int i = 0; i <= m_i_QPhoneMultiCtrl; i++)
	{
		for (int n = 0; n < MAX_IMAGE_NAMES; n++)
		{
			csPicPath.Format(_T("%s%s\\%s"), csDirectory, m_csArrQPhoneName[i], imageNames[n].c_str());
			TraceLog(MSG_INFO, std::string(csPicPath));
			DeleteFile(csPicPath);
			if (cFileFind.FindFile(csPicPath))
				TraceLog(MSG_WARNING, "Delete " + imageNames[n] + " Fail!");
		}
	}
	Report(true, "Clear OLD Picture done.", QPHONE_SERIAL_0);
	return true;
}

bool CCameraCheck::StrDec2Hex_8bytes(char* strInputDec_8bytes, char* strOutputHex_8bytes)
{
	unsigned long long _u64_InputDec = 0;
	sscanf(strInputDec_8bytes, "%I64u", &_u64_InputDec);
	sprintf(strOutputHex_8bytes, "%016I64X", _u64_InputDec);
	return true;
}

bool CCameraCheck::GenerateFuseID(char* strFuseID)
{
	SetProcessMutexLock(KEY_PROCESS, true);

	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);
	CString csTimeString;
	csTimeString.Format(_T("%02d%02d%02d%02d%02d%02d"),
		systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
	sprintf(strFuseID, "%s", csTimeString.Mid(0, 12));

	SetProcessMutexLock(KEY_PROCESS, false);
	return true;
}

bool CCameraCheck::TokenStrDec2Hex(char* strInputDec, char* strOutputHex)
{
	const size_t nSize = 12;
	const size_t nTokenSize = 2;
	const size_t nTokenNumber = nSize / nTokenSize ;
	CString csInputDec(strInputDec);
	size_t nInputDec[nTokenNumber] = {0};
	char szTemp[nTokenSize + 1] = {0};
	for (int i = 0; i < nTokenNumber; i++)
	{
		memset(szTemp, 0, nTokenSize + 1);
		strcpy_s(szTemp, csInputDec.Mid(i * nTokenSize, nTokenSize));
		nInputDec[i] = atoi(szTemp);
	}
	sprintf(strOutputHex, "%02X%02X%02X%02X%02X%02X", nInputDec[0], nInputDec[1], nInputDec[2], nInputDec[3], nInputDec[4], nInputDec[5]);
	return true;
}

bool CCameraCheck::SetColorOnDeviceScreen(int nQPhoneNumber, char* szColor)
{
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if ((m_b_isSlotPass[nQPhoneNumber] == true) && (m_b_UsableSlot[nQPhoneNumber] == true))
	{
		CallFtdFunction(FUNC_DISPLAY_LCD_RGB, (2000 + nQPhoneNumber), m_nFtdTimeOut, szColor, sz_outBuffer);
		CString csMsg;
		csMsg.Format(_T("Set QPhone%d screen color [%s] success!"), nQPhoneNumber, szColor);
		TraceLog(MSG_INFO, std::string(csMsg));
	}
	return true;
}

bool CCameraCheck::CheckRearMTFDialog(int nQPhoneNumber)
{
	char sz_msg[MAX_PATH] = {0};
	bool b_res = true;
	std::string str_msg;

	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	str_msg = _T("芞磁跡鎘? Is PASS ?") ;
	strncpy_s(sz_msg, sizeof(sz_msg), str_msg.c_str(), sizeof(sz_msg));

	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(sz_msg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = _T("Wait UI response time out.");
			TraceLog(MSG_INFO, _T("Wait UI response time out."));
			b_res = false;
			break;
		}
		else
		{
			i_waitCount++;
			Sleep(200);
			continue;
		}
	}

	if (b_res)
	{
		/* Get UI dialog operator response */
		if (m_b_DialogResponse == true)
		{
			BackupResultAndPicture("Rear_MTF", "PASS", nQPhoneNumber);
			str_msg = _T(" on/off is required pattern.");
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			BackupResultAndPicture("Rear_MTF", "FAIL", nQPhoneNumber);
			b_res = false;
			str_msg = _T(" on/off is not required pattern.");
			TraceLog(MSG_INFO, str_msg);
		}
	}
	return b_res;
}

/**********************************************/
/***************** Jig Control ****************/
/**********************************************/
void CCameraCheck::InitalJigPos()
{
	JigRequest("OUT");
	JigRequest("FOCUSOFF");
}

void CCameraCheck::MoveAwayBackCamLP100()
{
	//Rear LP 100 init
	JigRequest("OUT");
}

void CCameraCheck::MoveAwayFrontCamLP100()
{
	//Front LP 100 init
	JigRequest("FOCUSOFF");
}

void CCameraCheck::MoveCoverBackCamLP100()
{
	//Rear LP 100 cover
	JigRequest("IN");
}

void CCameraCheck::MoveCoverFrontCamLP100()
{
	//Front LP 100 cover
	JigRequest("FOCUSON");
}

/**********************************************/
/************ Multi Thread Control ************/
/**********************************************/
bool CCameraCheck::Qisda_MultiExecution(QISDA_PROCESS nQisdaProcess)
{
	CWinThread* pThread[5];//single:0,multi:1,2,3,4
	for (int i = 1; i <= 4; ++i)
	{
		m_hQPhoneProcess[i] = NULL;
		//SetProcessMutexLock( KEY_PROCESS, true );
		if (m_b_UsableSlot[i] && m_b_isSlotPass[i])
		{
			LPTHREAD_PARAM lpParam = new THREAD_PARAM;
			lpParam->nQPhoneNumber = i;
			lpParam->nQisdaProcess = nQisdaProcess;
			lpParam->pThisObject = this;
			pThread[i] = ::AfxBeginThread(CCameraCheck::AfxQPhoneThread, lpParam, THREAD_PRIORITY_HIGHEST, 0, 0, NULL);
			pThread[i]->m_bAutoDelete = false;
			m_hQPhoneProcess[i] = pThread[i]->m_hThread;
		}
		//SetProcessMutexLock( KEY_PROCESS, false );
	}

	for (int i = 1; i <= 4; ++i)
	{
		if (m_b_UsableSlot[i] && m_b_isSlotPass[i])
		{
			DWORD ret = WAIT_OBJECT_0;
			DWORD d = WaitForSingleObject(m_hQPhoneProcess[i], THREAD_HANDLE_TIMEOUT);
			if (WAIT_TIMEOUT == d)
			{
				ret = WAIT_TIMEOUT;
				ReportTimeoutHandler(i, nQisdaProcess);
			}
			else if (WAIT_ABANDONED == d)
			{
				ret = WAIT_ABANDONED_0 + i;
				ReportTimeoutHandler(i, nQisdaProcess);
			}
			if (WAIT_OBJECT_0 != ret)
			{
				CString csMsg;
				csMsg.Format(_T("QPhone%d Timeout! (type: %d)"), i, ret);
				TraceLog(MSG_INFO, std::string(csMsg));
			}
		}
		if (m_hQPhoneProcess[i] != NULL)
		{
			delete pThread[i];
			m_hQPhoneProcess[i] = NULL;
		}
	}
	return true;
}

UINT CCameraCheck::AfxQPhoneThread(LPVOID lpParam)
{
	LPTHREAD_PARAM pParam = (LPTHREAD_PARAM)lpParam;
	THREAD_PARAM stParam;
	stParam.pThisObject = pParam->pThisObject;
	stParam.nQPhoneNumber = pParam->nQPhoneNumber;
	stParam.nQisdaProcess = pParam->nQisdaProcess;

	CCameraCheck* pQPhoneProc = reinterpret_cast<CCameraCheck*>(stParam.pThisObject);
	ASSERT(pQPhoneProc != NULL);
	pQPhoneProc->Qisda_QPhoneProcess(stParam.nQPhoneNumber, stParam.nQisdaProcess);
	delete pParam;

	return TRUE;
}

void CCameraCheck::AlgoBeforeSleep(int nIndexQPhoneSlot, std::string msg, int order, int nTime)
{
	SetProcessMutexLock(KEY_WAIT, true);
	msg = "Sleep before " + msg + " ms";
	Report(true, msg, nIndexQPhoneSlot, "", order);
	Sleep(nTime);
	SetProcessMutexLock(KEY_WAIT, false);
}

bool CCameraCheck::Qisda_QPhoneProcess(int nIndexQPhoneSlot, int nQisdaProcess)
{
	if (REAR_CAMERA_MODULE_INFO == nQisdaProcess)
	{
		char szRearFuseID[FTD_BUF_SIZE] = {0};
		if (Qisda_ReadModuleInfo(REAR_CAMERA, nIndexQPhoneSlot, szRearFuseID))
		{
			m_csArrRearFuseID[nIndexQPhoneSlot] = szRearFuseID;
			m_csArrRearFuseID[nIndexQPhoneSlot].Replace("\n", "");
			m_csArrRearFuseID[nIndexQPhoneSlot].Replace("\r", "");
			m_csArrRearFuseID[nIndexQPhoneSlot].Replace("\t", "");
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + " Module: " + m_csArrRearFuseID[nIndexQPhoneSlot]));
			CString csMsg;
			csMsg.Format(_T("Rear Module Info: %s"), m_csArrRearFuseID[nIndexQPhoneSlot]);
			Report(true, std::string(csMsg), nIndexQPhoneSlot);
			SetPICSDataWithLock("Rear_FuseID", std::string(m_csArrRearFuseID[nIndexQPhoneSlot]), nIndexQPhoneSlot);
		}
		else
		{
			m_csArrRearFuseID[nIndexQPhoneSlot] = _T("000000000000");
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + " Read Module Info Fail."));
			SetPICSDataWithLock("Rear_FuseID", "N/A", nIndexQPhoneSlot);
		}
	}
	else if (REAR_CAMERA_STOP == nQisdaProcess)
	{
		bool bRes = Qisda_StopCam(REAR_CAMERA, nIndexQPhoneSlot);
		if (!bRes) m_b_isSlotPass[nIndexQPhoneSlot] = false;
		Report(bRes, "Rear Camera shutdown:", nIndexQPhoneSlot, FunErr_Rear_Camera_Test_Shutdown_Fail);
	}
	else if (REAR_CAMERA_START == nQisdaProcess)
	{
		CheckStopCam(Qisda_StartCam(REAR_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Start_Fail, "", "Start Rear Camera:");
	}
	else if (REAR_CAMERA_CAPTURE_LP100 == nQisdaProcess)
	{
		CheckStopCam(Qisda_CapturePic(REAR_LP100_PIC, REAR_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_Macro_Capture_Fail, "", "Rear Camera shot LP100:");
	}
	else if (REAR_CAMERA_ALGO_BLEMISH == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "RearBlemish " + m_sleepRearBlemish, ORDER_BLEMISH_REAR, m_nSleepRearBlemish);
		CheckStopCam(Qisda_IMG_Algo_Rear("blemish", "Rear_Blemish", nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_Blemish_Fail, PICS_REAR_BLEMISH_RESULT, "", m_nCheckRearBlemish, ORDER_BLEMISH_REAR);
	}
	else if (REAR_CAMERA_ALGO_SHADING == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "RearShading " + m_sleepRearShading, ORDER_SHADING_REAR, m_nSleepRearShading);
		CheckStopCam(Qisda_IMG_Algo_Rear("shading", "Rear_Shading", nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_Shading_Fail, PICS_REAR_SHADING_RESULT, "", m_nCheckRearShading, ORDER_SHADING_REAR);
	}
	else if (REAR_CAMERA_CAPTURE_MTF == nQisdaProcess)
	{
		CheckStopCam(Qisda_CapturePic(REAR_MTF_PIC, REAR_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_Capture_Fail, "", "Rear Camera shot MTF chart");
	}
	else if (REAR_CAMERA_ALGO_COLOR == nQisdaProcess)
	{
		CheckStopCam(Qisda_IMG_Algo_Rear("COLORSHADING", "Rear_Color", nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_Color_Fail, PICS_REAR_COLOR_RESULT, "", m_nCheckRearColor, ORDER_COLOR_REAR);
	}
	else if (REAR_CAMERA_ALGO_MTF == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "RearMTF " + m_sleepRearMTF, ORDER_MTF_REAR, m_nSleepRearMTF);
		CheckStopCam(Qisda_IMG_Algo_Rear("MTF", "Rear_MTF", nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_MTF_Fail, PICS_REAR_MTF_RESULT, "", m_nCheckRearMTF, ORDER_MTF_REAR);
	}
	else if (REAR_CAMERA_DIALOG_MTF == nQisdaProcess)
	{
		CheckStopCam(CheckRearMTFDialog(nIndexQPhoneSlot),
			nIndexQPhoneSlot, REAR_CAMERA, FunErr_Rear_Camera_Test_MTF_Fail, PICS_REAR_MTF_RESULT, "", m_nCheckRearMTF, ORDER_MTF_REAR);
	}
	else if (FRONT_CAMERA_MODULE_INFO == nQisdaProcess)
	{
		char szBuff[FTD_BUF_SIZE] = {0};
		if (Qisda_ReadModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szBuff))
		{
			char szFrontFuseID[64] = {0};
			StrDec2Hex_8bytes(szBuff, szFrontFuseID);
			m_csArrFrontFuseID[nIndexQPhoneSlot] = szFrontFuseID;
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + _T(" Module: ") + m_csArrFrontFuseID[nIndexQPhoneSlot]));
			CString csMsg;
			csMsg.Format(_T("Front Module Info: %s"), m_csArrFrontFuseID[nIndexQPhoneSlot]);
			Report(true, std::string(csMsg), nIndexQPhoneSlot);
			SetPICSDataWithLock("Front_FuseID", std::string(m_csArrFrontFuseID[nIndexQPhoneSlot]), nIndexQPhoneSlot);
		}
		else
		{
			m_csArrFrontFuseID[nIndexQPhoneSlot] = _T("000000000000");
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + _T(" Read Module Info Fail.")));
		}
	}
	else if (FRONT_CAMERA_STOP == nQisdaProcess)
	{
		bool bRes = Qisda_StopCam(FRONT_CAMERA, nIndexQPhoneSlot);
		if (!bRes) m_b_isSlotPass[nIndexQPhoneSlot] = false;
		Report(bRes, "Front Camera shutdown:", nIndexQPhoneSlot, FunErr_Front_Camera_Test_Shutdown_Fail);
	}
	else if (FRONT_CAMERA_START == nQisdaProcess)
	{
		CheckStopCam(Qisda_StartCam(FRONT_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Start_Fail, "", "Start Front Camera");
	}
	else if (FRONT_CAMERA_CAPTURE_LP100 == nQisdaProcess)
	{
		CheckStopCam(Qisda_CapturePic(FRONT_LP100_PIC, FRONT_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_Macro_Capture_Fail, "", "Front Camera shot LP picture");
	}
	else if (FRONT_CAMERA_ALGO_BLEMISH == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "FrontBlemish " + m_sleepFrontBlemish, ORDER_BLEMISH_FRONT, m_nSleepFrontBlemish);
		CheckStopCam(Qisda_IMG_Algo_Front("blemish", "Front_Blemish", nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_Blemish_Fail, PICS_FRONT_BLEMISH_RESULT, "", m_nCheckFrontBlemish, ORDER_BLEMISH_FRONT);
	}
	else if (FRONT_CAMERA_ALGO_SHADING == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "FrontShading " + m_sleepFrontShading, ORDER_SHADING_FRONT, m_nSleepFrontShading);
		CheckStopCam(Qisda_IMG_Algo_Front("shading", "Front_Shading", nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_Shading_Fail, PICS_FRONT_SHADING_RESULT, "", m_nCheckFrontShading, ORDER_SHADING_FRONT);
	}
	else if (FRONT_CAMERA_CAPTURE_MTF == nQisdaProcess)
	{
		CheckStopCam(Qisda_CapturePic(FRONT_MTF_PIC, FRONT_CAMERA, nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_Capture_Fail, "", "Front Camera shot MTF picture");
	}
	else if (FRONT_CAMERA_ALGO_COLOR == nQisdaProcess)
	{
		CheckStopCam(Qisda_IMG_Algo_Front("COLORSHADING", "Front_Color", nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_Color_Fail, PICS_FRONT_COLOR_RESULT, "", m_nCheckFrontColor, ORDER_COLOR_FRONT);
	}
	else if (FRONT_CAMERA_ALGO_MTF == nQisdaProcess)
	{
		AlgoBeforeSleep(nIndexQPhoneSlot, "FrontMTF " + m_sleepFrontMTF, ORDER_MTF_FRONT, m_nSleepFrontMTF);
		CheckStopCam(Qisda_IMG_Algo_Front("MTF", "Front_MTF", nIndexQPhoneSlot),
			nIndexQPhoneSlot, FRONT_CAMERA, FunErr_Front_Camera_Test_MTF_Fail, PICS_FRONT_MTF_RESULT, "", m_nCheckFrontMTF, ORDER_MTF_FRONT);
	}
	else if (FRONT_CAMERA_FUSE_ID_READ_WRITE == nQisdaProcess)
	{
		bool bRes = false;
		int nRWStatus = 0;
		char szReadBuff[FTD_BUF_SIZE] = {0};
		if (Qisda_ReadModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szReadBuff))
		{
			TraceLog(MSG_INFO, "Read Front FuseID: " + std::string(szReadBuff), nIndexQPhoneSlot);
			int nFuseID = atoi(szReadBuff);
			if (nFuseID != 0)
			{
				bRes = true;
			}
			else
			{
				char szWriteBuff[FTD_BUF_SIZE] = {0};
				GenerateFuseID(szWriteBuff);
				char szOutBuff[FTD_BUF_SIZE] = {0};
				if (!Qisda_WriteModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szWriteBuff, szOutBuff))
				{
					TraceLog(MSG_INFO, "Write Front FuseID Result: " + std::string(szOutBuff), nIndexQPhoneSlot);
				}
				Sleep(1000);

				memset(szReadBuff, 0, FTD_BUF_SIZE);
				if (Qisda_ReadModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szReadBuff))
				{
					bRes = true;
					nRWStatus = (strcmp(szReadBuff, szWriteBuff) == 0) ? 1 : 2;
				}
			}
		}
		char szDecValue[64] = {0};
		if (bRes)
		{
			char szZero[16] = {0};
			strcpy(szZero, "000000000000");
			if (strlen(szReadBuff) >= 12)
			{
				strncpy(szDecValue, szReadBuff, 12);
			}
			else
			{
				strncpy(szDecValue, szZero, 12 - strlen(szReadBuff));
				strcat(szDecValue, szReadBuff);
			}
		}
		if (bRes)
		{
			char szFrontFuseID[64] = {0};
			TokenStrDec2Hex(szDecValue, szFrontFuseID);
			m_csArrFrontFuseID[nIndexQPhoneSlot] = szFrontFuseID;
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + " Front FuseID Hex: " + m_csArrFrontFuseID[nIndexQPhoneSlot]));
			CString csMsg;
			csMsg.Format(_T("Front FuseID Hex: %s"), m_csArrFrontFuseID[nIndexQPhoneSlot]);
			Report(true, std::string(csMsg), nIndexQPhoneSlot);

			std::string strPicsData(m_csArrFrontFuseID[nIndexQPhoneSlot]);
			if (nRWStatus == 1)
				strPicsData += ",Y";
			else if (nRWStatus == 2)
				strPicsData += ",N";
			SetPICSDataWithLock("Front_FuseID", strPicsData, nIndexQPhoneSlot);
		}
		else
		{
			m_csArrFrontFuseID[nIndexQPhoneSlot] = _T("000000000000");
			TraceLog(MSG_INFO, std::string(m_csArrQPhoneName[nIndexQPhoneSlot] + " Read Module Info Fail."));
			SetPICSDataWithLock("Front_FuseID", "N/A", nIndexQPhoneSlot);
		}
	}
	else if (FRONT_CAMERA_FUSE_ID_READ_WRITE_NEW == nQisdaProcess)
	{
		bool bRes = false;
		int nRWStatus = 0;
		char szReadBuff[FTD_BUF_SIZE] = {0};
		if (Qisda_ReadModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szReadBuff))
		{
			TraceLog(MSG_INFO, "Read Front FuseID: " + std::string(szReadBuff), nIndexQPhoneSlot);
			int nFuseID = atoi(szReadBuff);
			if (nFuseID != 0)
			{
				bRes = true;
			}
			else
			{
				char szWriteBuff[FTD_BUF_SIZE] = {0};
				GenerateFuseID(szWriteBuff);
				strcat(szWriteBuff, ",1");
				char szOutBuff[FTD_BUF_SIZE] = {0};
				TraceLog(MSG_INFO, "Write Front FuseID: " + std::string(szWriteBuff), nIndexQPhoneSlot);
				if (Qisda_WriteModuleInfo(FRONT_CAMERA, nIndexQPhoneSlot, szWriteBuff, szOutBuff))
				{
					bRes = true;
					if (strcmp(szOutBuff, szWriteBuff) == 0)
					{
						nRWStatus = 1;
						TraceLog(MSG_INFO, "Write Front FuseID Success: " + std::string(szOutBuff), nIndexQPhoneSlot);
					}
					else
					{
						nRWStatus = 2;
						TraceLog(MSG_INFO, "Write Front FuseID Mismatch: " + std::string(szOutBuff), nIndexQPhoneSlot);
					}
					strncpy(szReadBuff, szOutBuff, 12);
				}
				else
				{
					nRWStatus = 3;
					TraceLog(MSG_INFO, "Write Front FuseID Fail: " + std::string(szOutBuff), nIndexQPhoneSlot);
				}
			}
		}

		char szDecValue[64] = {0};
		if (bRes == true)
		{
			char szZero[16] = {0};
			strcpy(szZero, "000000000000");
			if (strlen(szReadBuff) >= 12)
			{
				strncpy(szDecValue, szReadBuff, 12);
			}
			else
			{
				strncpy(szDecValue, szZero, 12 - strlen(szReadBuff));
				strcat(szDecValue, szReadBuff);
			}
		}
		if (bRes)
		{
			char szFrontFuseID[64] = {0};
			TokenStrDec2Hex(szDecValue, szFrontFuseID);
			m_csArrFrontFuseID[nIndexQPhoneSlot] = szFrontFuseID;
			CString csMsg;
			csMsg.Format(_T("Front FuseID Hex: %s"), m_csArrFrontFuseID[nIndexQPhoneSlot]);
			Report(true, std::string(csMsg), nIndexQPhoneSlot);

			std::string strPicsData(m_csArrFrontFuseID[nIndexQPhoneSlot]);
			if (nRWStatus == 0)
			{
				Report(true, "Read Front FuseID:", nIndexQPhoneSlot);
			}
			else if (nRWStatus == 1)
			{
				strPicsData += ",Y";
				Report(true, "Write Front FuseID:", nIndexQPhoneSlot);
			}
			else if (nRWStatus == 2)
			{
				strPicsData += ",N";
				Report(false, "Write Front FuseID mismatch!", nIndexQPhoneSlot, FunErr_Front_Camera_FuseID_Mismatch_Fail);
				m_b_isSlotPass[nIndexQPhoneSlot] = false;
			}
			SetPICSDataWithLock("Front_FuseID", strPicsData, nIndexQPhoneSlot);
		}
		else
		{
			if (nRWStatus == 0)
			{
				m_csArrFrontFuseID[nIndexQPhoneSlot] = _T("NA");
				Report(false, "Read Front FuseID:", nIndexQPhoneSlot, FunErr_Front_Camera_Read_FuseID_Fail);
			}
			else if (nRWStatus == 3)
			{
				m_csArrFrontFuseID[nIndexQPhoneSlot] = _T("000000000000");
				Report(false, "Write Front FuseID:", nIndexQPhoneSlot, FunErr_Front_Camera_Write_FuseID_Fail);
			}
			m_b_isSlotPass[nIndexQPhoneSlot] = false;
			std::string strPicsData(m_csArrFrontFuseID[nIndexQPhoneSlot]);
			SetPICSDataWithLock("Front_FuseID", strPicsData, nIndexQPhoneSlot);
		}
	}
	else if (SHOW_RESULT == nQisdaProcess)
	{
		m_strErrorCode = "-";
		m_strMessage = "Camera Test Pass";
		SetColorOnDeviceScreen(nIndexQPhoneSlot, COLOR_GREEN);
		Report(true, m_strMessage, nIndexQPhoneSlot);
	}
	return true;
}

bool CCameraCheck::SetProcessMutexLock(MUTEX_KEY nMutexKey, bool bMutexStatus)
{
	HANDLE h = NULL;
	DWORD msTimeout = 0;
	switch (nMutexKey)
	{
	case KEY_PROCESS:
		h = g_MutexProcess;
		msTimeout = MUTEX_TIMEOUT_PROCESS;
		break;
	case KEY_FTD:
		h = g_MutexFtd;
		msTimeout = MUTEX_TIMEOUT_FTD;
		break;
	case KEY_REPORT:
		h = g_MutexReport;
		msTimeout = MUTEX_TIMEOUT_REPORT;
		break;
	case KEY_PICS:
		h = g_MutexPics;
		msTimeout = MUTEX_TIMEOUT_PICS;
		break;
	case KEY_WAIT:
		h = g_MutexWait;
		msTimeout = MUTEX_TIMEOUT_WAIT;
		break;
	default:
		break;
	}
	if (h && bMutexStatus)
		::WaitForSingleObject(h, msTimeout);
	else
		::ReleaseMutex(h);

	return true;
}

bool CCameraCheck::CallFtdFunction(CALL_FUNC nFtdFunc, int nPort, int nTimeout, char* pszInput, char* pszOutput)
{
	SetProcessMutexLock(KEY_FTD, true);

	bool bFtdFuncReturn = false;
	switch (nFtdFunc)
	{
	case FUNC_REAR_MODULE_INFO:
		bFtdFuncReturn = m_pIPhone->FTD_CAM_OTP_AFInfinity(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_FRONT_MODULE_INFO:
		bFtdFuncReturn = m_pIPhone->FTD_CAM_OTP_FrontFuseID(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_FRONT_FUSE_ID_WRITE:
		bFtdFuncReturn = m_pIPhone->FTD_CAM_OTP_WriteFrontFuseID(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_CAM_START:
		bFtdFuncReturn = m_pIPhone->FTD_CAMstart(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_CAM_CAPTURE:
		bFtdFuncReturn = m_pIPhone->FTD_CAMshot(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_CAM_STOP:
		bFtdFuncReturn = m_pIPhone->FTD_CAMend(nPort, nTimeout, pszInput, pszOutput);
		break;
	case FUNC_DISPLAY_LCD_RGB:
		bFtdFuncReturn = m_pIPhone->Display_LCD_RGB(nPort, nTimeout, pszInput, pszOutput);
		break;
	}

	SetProcessMutexLock(KEY_FTD, false);
	return bFtdFuncReturn;
}

void CCameraCheck::SetPICSDataWithLock(std::string str_key, std::string str_value, int nQPhoneNumber)
{
	if (str_key.empty()) return;
	SetProcessMutexLock(KEY_PICS, true);
	SetPICSData(str_key, str_value, nQPhoneNumber);
	SetProcessMutexLock(KEY_PICS, false);
}
