/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CTouchPanelProc object.
 * FileName   : CTouchPanelProc.cpp
 * Version    : $Revision:   1.5  $
 * Author     : Gilpin.Xi
 * Time       : 2011/04/18
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/04/18      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CTouchPanelProc.h"

RF_IMPLEMENT_DYNCREATE(CTouchPanelProc)

CTouchPanelProc* pTouchPanelProc;

CTouchPanelProc::CTouchPanelProc()
{
	m_bStopThread = false;
	m_bStopTouchTest = false;

	pTouchPanelProc = NULL;

	pTouchLineDlg = NULL;
	pTouchCircleDlg = NULL;
	pTouchGraphDlg = NULL;
	pTouchParallelDlg = NULL;
	pTouchPointDlg = NULL;

	TouchLineThreadHandle = NULL;
	TouchCircleThreadHandle = NULL;
	TouchGraphThreadHandle = NULL;
	TouchParallelThreadHandle = NULL;
	TouchPointThreadHandle = NULL;
}

CTouchPanelProc::~CTouchPanelProc()
{
}

bool CTouchPanelProc::PreRun()
{
	m_strItemCode = CStr::IntToStr(TOUCH_Test_BaseItemCode);
	m_strErrorCode = FunErr_Touch_General_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CTouchPanelProc::Run()
{
	bool bRes = false;
	bRes = TouchPanelTest();
	if (bRes)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Touch Panel Test Pass!";
		TraceLog(MSG_INFO, "Touch Panel Test Pass!");
	}
	else
	{
		m_strResult = "FAIL";
		TraceLog(MSG_ERROR, "Touch Panel Test Fail!");
	}
	FactoryLog();
	return bRes;
}

bool CTouchPanelProc::PostRun()
{
	return true;
}

bool CTouchPanelProc::InitData(std::map<std::string, std::string>& paramMap)
{
	StrVtr vtrstr;
	int ielement;

	//Test TouchItem_Sequence
	std::string strTouchItem_Sequence;
	if (paramMap.find(std::string("TouchItem_Sequence")) != paramMap.end())
	{
		strTouchItem_Sequence = paramMap[std::string("TouchItem_Sequence")];
		m_vTouchItem.clear();
		vtrstr.clear();
		CStr::ParseString(strTouchItem_Sequence, ",", vtrstr);
		for (StrVtrItr itr = vtrstr.begin(); itr != vtrstr.end(); ++itr)
		{
			ielement = CStr::StrToInt(*itr);
			m_vTouchItem.push_back(ielement);
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'TouchItem_Sequence' for class 'CTouchPanelProc'");
		return false;
	}

	//Touch Panel Pixel
	std::string strPanelPixel;
	if (paramMap.find(std::string("PanelPixel")) != paramMap.end())
	{
		strPanelPixel = paramMap[std::string("PanelPixel")];
		vtrstr.clear();
		CStr::ParseString(strPanelPixel, _T(","), vtrstr);
		if (vtrstr.size() != 2)
		{
			TraceLog(MSG_ERROR, "Get 'PanelPixel' for class 'CTouchPanelProc' is not correct number 2");
			return false;
		}
		m_iPanelHPixel = CStr::StrToInt(vtrstr[0]);
		m_iPanelVPixel = CStr::StrToInt(vtrstr[1]);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'PanelPixel' for class 'CTouchPanelProc'");
		return false;
	}

	//Touch Panel Block Size
	std::string strBlockSize;
	if (paramMap.find(std::string("BlockSize")) != paramMap.end())
	{
		strBlockSize = paramMap[std::string("BlockSize")];
		vtrstr.clear();
		CStr::ParseString(strBlockSize, _T(","), vtrstr);
		if (vtrstr.size() != 2)
		{
			TraceLog(MSG_ERROR, "Get 'BlockSize' for class 'CTouchPanelProc' is not correct number 2");
			return false;
		}
		m_iBlockLenght = CStr::StrToInt(vtrstr[0]);
		m_iBlockWidth = CStr::StrToInt(vtrstr[1]);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'BlockSize' for class 'CTouchPanelProc'");
		return false;
	}

	//UI Display Rate
	std::string strRate;
	if (paramMap.find(std::string("DisplayRate")) != paramMap.end())
	{
		strRate = paramMap[std::string("DisplayRate")];
		m_iDisplayRate = CStr::StrToInt(strRate);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'DisplayRate' for class 'CTouchPanelProc'");
		return false;
	}

	/* Delay ms between every test pattern display gap */
	std::string str_GapMs;
	if (paramMap.find(std::string("DisplayGap_ms")) != paramMap.end())
	{
		str_GapMs = paramMap[std::string("DisplayGap_ms")];
		m_i_DisplayGap_ms = CStr::StrToInt(str_GapMs);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'DisplayGap_ms' for class 'CTouchPanelProc'");
		return false;
	}

	//Test Touch Circle Count
	std::string strTouchCircleCount;
	if (paramMap.find(std::string("CircleCount")) != paramMap.end())
	{
		strTouchCircleCount = paramMap[std::string("CircleCount")];
		m_iTouchCircleCount = CStr::StrToInt(strTouchCircleCount);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'CircleCount' for class 'CTouchPanelProc'");
		return false;
	}

	//Distance
	std::string strDistance;
	if (paramMap.find(std::string("Distance")) != paramMap.end())
	{
		strDistance = paramMap[std::string("Distance")];
		m_iDistance = CStr::StrToInt(strDistance);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'Distance' for class 'CTouchPanelProc'");
		return false;
	}

	//Test TimeOut
	std::string strTimeOut;
	if (paramMap.find(std::string("TimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("TimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'TimeOut' for class 'CTouchPanelProc'");
		return false;
	}

	/* ShowTrack */
	std::string str_ShowTrack;
	if (paramMap.find(std::string("ShowTrack")) != paramMap.end())
	{
		str_ShowTrack = paramMap[std::string("ShowTrack")];
		m_b_ShowTrack = CStr::StrToBool(str_ShowTrack);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get filed 'ShowTrack' for class 'CTouchPanelProc'");
		return false;
	}

	/* TargetPath */
	if (paramMap.find(std::string("TrackFilePath")) != paramMap.end())
	{
		m_str_TrackFilePath = paramMap[std::string("TrackFilePath")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TrackFilePath' for class 'CTouchPanelProc'");
		return false;
	}

	return InitData();
}

bool CTouchPanelProc::InitData()
{
	return true;
}

bool CTouchPanelProc::CopyTouchTrackPictureToMobile()
{
	bool b_res = false;

	CString str_command = _T("");
	CString str_currentPath = _T("");
	CString str_SourcePath = _T("");
	CString str_TargetPath = _T("");
	CString str_OutMessage = _T("");
	char sz_errorMsg[512] = {0};
	char* msgBuffer = new char[1024 * 1024 * 2];

	/* Get current path */
	::GetModuleFileName(NULL, str_currentPath.GetBuffer(MAX_PATH), MAX_PATH);
	str_currentPath.ReleaseBuffer();
	str_currentPath = str_currentPath.Left(str_currentPath.ReverseFind('\\'));

	/* Get source track file path and target save path  */
	str_SourcePath.Format(_T("%s\\Pictures\\TOUCH\\%s"), str_currentPath, g_str_modelName.c_str());
	str_TargetPath.Format(_T("%s/%s"), m_str_TrackFilePath.c_str(), g_str_modelName.c_str());

	/* command */
	str_command.Format(_T("push %s %s"), str_SourcePath, str_TargetPath);

	/* Generate mobile log */
	b_res = m_pIPhone->ExecAdbCommand(str_command, msgBuffer, sz_errorMsg);

	TraceLog(MSG_INFO, m_strMessage);
	TraceLog(MSG_INFO, msgBuffer);

	str_OutMessage = msgBuffer;
	if ((str_OutMessage.Find("error") != -1))
		b_res = false;

	delete[] msgBuffer;

	return b_res;
}

bool CTouchPanelProc::TouchPanelTest()
{
	bool bRes = false;
	if (!(bRes = m_pIPhone->ExactOpenDevice(m_nFtdPort, m_nFtdTimeOut)))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		SetPICSData("TOUCH_PANEL_TEST", "FAIL");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	/* If need not to show touch track on mobile, start/stop touch test only once */
	char chOutData[FTD_BUF_SIZE] = {0};
	if (!m_b_ShowTrack)
	{
		//Start Touch Panel Test(Enter->1/true)
		if (!(bRes = m_pIPhone->TouchPanelTest(m_nFtdPort, m_nFtdTimeOut, true, chOutData)))
		{
			m_strMessage = "Touch Panel Test Start Command fail!";
			TraceLog(MSG_ERROR, "Touch Panel Test Start Command fail!");
			SetPICSData("TOUCH_PANEL_TEST", "FAIL");
			return false;
		}
		else
		{
			TraceLog(MSG_INFO, "Touch Panel Test Start Command Succeed.");
		}
	}

	IntVtrItr itr;
	for (itr = m_vTouchItem.begin(); itr != m_vTouchItem.end(); ++itr)
	{
		m_iCurrentTouchItem = *itr;
		if (bRes)
		{
			switch (m_iCurrentTouchItem)
			{
			//Linearity Test
			case 1:
				bRes = InitialDiagonalLineDlg();
				if (bRes)
					bRes = RunDiagonalLineProcess();
				EndDiagonalLineDlg();
				break;
			//Circle-Touch Test
			case 2:
				bRes = InitialCircleTouchDlg();
				if (bRes)
					bRes = RunCircleTouchProcess();
				EndCircleTouchDlg();
				break;
			//Multi-Touch(Graph-8 lines)
			case 3:
				bRes = InitialMultiTouchDlg();
				if (bRes)
					bRes = RunMultiTouchProcess();
				EndMultiTouchDlg();
				break;
			//Multi-Touch(2 Parallel lines)
			case 4:
				bRes = InitialParallelTouchDlg();
				if (bRes)
					bRes = RunParallelTouchProcess();
				EndParallelTouchDlg();
				break;
			//Multi points Touch
			case 5:
				bRes = InitialTouchPointDlg();
				if (bRes)
					bRes = RunTouchPointProcess();
				EndTouchPointDlg();
				break;
			case 6:
				break;
			default:
				bRes = false;
				TraceLog(MSG_ERROR, "This Touch Panel Test Graph is not defined.");
				break;
			}
		}
		if (!bRes)
			break;
	}

	/* If need not to show touch track on mobile, start/stop touch test only once */
	if (!m_b_ShowTrack)
	{
		/* Need to wait some time before stop touch test */
		Sleep(1000);
		TraceLog(MSG_ERROR, "touch !m_b_ShowTrack => Sleep 1000ms");

		/* Stop Touch Panel Test(Exit->0/false) */
		if (!m_pIPhone->TouchPanelTest(m_nFtdPort, 1000, false, chOutData))
			TraceLog(MSG_ERROR, "Touch Panel Test Stop Command fail!");
		else
			TraceLog(MSG_INFO, "Touch Panel Test Stop Command Succeed.");
	}

	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		SetPICSData("TOUCH_PANEL_TEST", "FAIL");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	/* Set PICS Data */
	if (bRes)
		SetPICSData("TOUCH_PANEL_TEST", "PASS");
	else
		SetPICSData("TOUCH_PANEL_TEST", "FAIL");

	return bRes;
}

bool CTouchPanelProc::TouchCapkeyTest()
{
	bool bRes = false;
	char chOutData[FTD_BUF_SIZE] = {0};
	if (!(bRes = m_pIPhone->ExactOpenDevice(m_nFtdPort, m_nFtdTimeOut)))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		SetPICSData("TOUCH_CAPKEY_TEST", "FAIL");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	/* If need not to show touch track on mobile, start/stop touch test only once */
	if (!m_b_ShowTrack)
	{
		//Start Touch Panel Test(Enter->1/true)
		if (!(bRes = m_pIPhone->TouchCapkeyTest(m_nFtdPort, m_nFtdTimeOut, true, chOutData)))
		{
			m_strMessage = "Touch Capkey Test Start Command fail!";
			TraceLog(MSG_ERROR, "Touch Capkey Test Start Command fail!");
			SetPICSData("TOUCH_CAPKEY_TEST", "FAIL");
			return false;
		}
		else
		{
			TraceLog(MSG_INFO, "Touch Capkey Test Start Command Succeed.");
		}
	}

	IntVtrItr itr;
	for (itr = m_vTouchItem.begin(); itr != m_vTouchItem.end(); ++itr)
	{
		m_iCurrentTouchItem = *itr;
		if (bRes)
		{
			switch (m_iCurrentTouchItem)
			{
			case 1://Linearity Test
			case 2://Circle-Touch Test
			case 3://Multi-Touch(Graph-8 lines)
			case 4://Multi-Touch(2 Parallel lines)
			case 5://Multi points Touch
				break;
			//Capkey Touch Test
			case 6:
				bRes = InitialTouchCapkeyDlg();
				if (bRes)
					bRes = RunTouchCapkeyProcess();
				EndTouchCapkeyDlg();
				break;
			default:
				bRes = false;
				TraceLog(MSG_ERROR, "This Touch Panel Test Graph is not defined.");
				break;
			}
		}
		if (!bRes)
			break;
	}

	/* If need not to show touch track on mobile, start/stop touch test only once */
	if (!m_b_ShowTrack)
	{
		/* Need to wait some time before stop touch test */
		Sleep(1000);
		TraceLog(MSG_ERROR, "key !m_b_ShowTrack => Sleep 1000ms");

		/* Stop Touch Panel Test(Exit->0/false) */
		if (!m_pIPhone->TouchCapkeyTest(m_nFtdPort, 1000, false, chOutData))
			TraceLog(MSG_ERROR, "Touch Key Test Stop Command fail!");
		else
			TraceLog(MSG_INFO, "Touch Key Test Stop Command Succeed.");
	}

	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		SetPICSData("TOUCH_CAPKEY_TEST", "FAIL");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	SetPICSData("TOUCH_CAPKEY_TEST", bRes ? "PASS" : "FAIL");

	return bRes;
}

//Diagonal Line
UINT CTouchPanelProc::AfxTouchLineDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;

	pTouchPanelProc->pTouchLineDlg->DoModal();

	pTouchPanelProc->pTouchLineDlg->FreeResource();
	delete pTouchPanelProc->pTouchLineDlg;
	pTouchPanelProc->pTouchLineDlg = NULL;

	return true;
}

bool CTouchPanelProc::CloseTouchLineDlgThread()
{
	if (TouchLineThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchLineThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchLineThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialDiagonalLineDlg()
{
	pTouchLineDlg = new CTouchLineDlg;
	if (pTouchLineDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate) != true)
	{
		m_strMessage = "Initial Touch Line dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Line dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial touch linearity test dialog success!");
	}

	TouchLineThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchLineDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchLineThreadHandle == NULL)
	{
		m_strMessage = "Create Touch Line dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch Line dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch linearity test dialog success!");
	}

	return true;
}

bool CTouchPanelProc::RunDiagonalLineProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Linearity Test
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {4, 2, 0, 0}; //NV_DT_TOUCH
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			pTouchLineDlg->Check_TouchPoint_Position(chPointCoord);
			if (pTouchLineDlg->Check_Touch_Result())
				return true;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndDiagonalLineDlg()
{
	pTouchLineDlg->OnDestroy();
	//CloseTouchLineDlgThread();
	//delete pTouchLineDlg;
	//pTouchLineDlg = NULL;
	return true;
}

//Circle-Touch
UINT CTouchPanelProc::AfxTouchCircleDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;

	pTouchPanelProc->pTouchCircleDlg->DoModal();

	pTouchPanelProc->pTouchCircleDlg->FreeResource();
	delete pTouchPanelProc->pTouchCircleDlg;
	pTouchPanelProc->pTouchCircleDlg = NULL;

	return true;
}

bool CTouchPanelProc::CloseTouchCircleDlgThread()
{
	if (TouchCircleThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchCircleThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchCircleThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialCircleTouchDlg()
{
	pTouchCircleDlg = new CTouchCircleDlg;
	if (!pTouchCircleDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate, m_iTouchCircleCount, m_iDistance))
	{
		m_strMessage = "Initial Touch Circle dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Circle dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial touch circle test dialog success!");
	}

	TouchCircleThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchCircleDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchCircleThreadHandle == NULL)
	{
		m_strMessage = "Create Touch Circle dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch Circle dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch circle test dialog success!");
	}
	return true;
}

bool CTouchPanelProc::RunCircleTouchProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Circle-Touch Test
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {4, 2, 0, 0}; //NV_DT_TOUCH
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			pTouchCircleDlg->Check_TouchPoint_Position(chPointCoord);
			if (pTouchCircleDlg->Check_Touch_Result())
				return true;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndCircleTouchDlg()
{
	pTouchCircleDlg->OnDestroy();
	//CloseTouchCircleDlgThread();
	//delete pTouchCircleDlg;
	//pTouchCircleDlg = NULL;
	return true;
}

//Multi-Touch(Graph-8 lines)
UINT CTouchPanelProc::AfxTouchGraphDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;
	pTouchPanelProc->pTouchGraphDlg->DoModal();
	pTouchPanelProc->pTouchGraphDlg->FreeResource();
	delete pTouchPanelProc->pTouchGraphDlg;
	pTouchPanelProc->pTouchGraphDlg = NULL;
	return true;
}

bool CTouchPanelProc::CloseTouchGraphDlgThread()
{
	if (TouchGraphThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchGraphThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchGraphThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialMultiTouchDlg()
{
	pTouchGraphDlg = new CTouchGraphDlg;
	if (!pTouchGraphDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate, m_iDistance))
	{
		m_strMessage = "Initial Touch Graph dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Graph dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial touch graph test dialog success!");
	}

	TouchGraphThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchGraphDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchGraphThreadHandle == NULL)
	{
		m_strMessage = "Create Touch Graph dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch Graph dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch graph test dialog success!");
	}
	return true;
}

bool CTouchPanelProc::RunMultiTouchProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Multi-Touch Test
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {4, 2, 0, 0};//NV_DT_TOUCH
	//1:Pair:1;Line:1-2
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			pTouchGraphDlg->Check_TouchPoint_Position(chPointCoord, 1);
			if (pTouchGraphDlg->Check_Touch_Result(1))
			{
				pTouchGraphDlg->Show_Test_TouchLine(3);
				break;
			}
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}

	//3:Pair:3;Line:1-2
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			pTouchGraphDlg->Check_TouchPoint_Position(chPointCoord, 3);
			if (pTouchGraphDlg->Check_Touch_Result(3))
			{
				pTouchGraphDlg->Show_Test_TouchLine(4);
				break;
			}
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}

	//4:Pair:4;Line:1-2
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			//TraceLog(MSG_INFO,strPointCoord);
			pTouchGraphDlg->Check_TouchPoint_Position(chPointCoord, 4);
			if (pTouchGraphDlg->Check_Touch_Result(4))
			{
				pTouchGraphDlg->Show_Test_TouchLine(2);
				break;
			}
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}

	//2:Pair:2;Line:1-2
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord) == true)
		{
			strPointCoord = chPointCoord;
			pTouchGraphDlg->Check_TouchPoint_Position(chPointCoord, 2);
			if (pTouchGraphDlg->Check_Touch_Result(2))
				break;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndMultiTouchDlg()
{
	pTouchGraphDlg->OnDestroy();
	//CloseTouchGraphDlgThread();
	//delete pTouchGraphDlg;
	//pTouchGraphDlg = NULL;
	return true;
}

//Multi-Touch(2 Parallel lines)
UINT CTouchPanelProc::AfxTouchParallelDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;

	pTouchPanelProc->pTouchParallelDlg->DoModal();

	pTouchPanelProc->pTouchParallelDlg->FreeResource();
	delete pTouchPanelProc->pTouchParallelDlg;
	pTouchPanelProc->pTouchParallelDlg = NULL;
	return true;
}

bool CTouchPanelProc::CloseTouchParallelDlgThread()
{
	if (TouchParallelThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchParallelThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchParallelThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialParallelTouchDlg()
{
	pTouchParallelDlg = new CTouchParallelDlg;
	if (!(pTouchParallelDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate, m_iDistance)))
	{
		m_strMessage = "Initial Touch Parallel dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Parallel dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial touch parallel test dialog success!");
	}

	TouchParallelThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchParallelDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchParallelThreadHandle == NULL)
	{
		m_strMessage = "Create Touch Parallel dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch Parallel dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch parallel test dialog success!");
	}
	return true;
}

bool CTouchPanelProc::RunParallelTouchProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Multi-Touch(2 Parallel lines) Test
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {4, 2, 0, 0}; //NV_DT_TOUCH
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord) == true)
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			/* Check touch point position */
			pTouchParallelDlg->Check_TouchPoint_Position(chPointCoord);
			/* Check touch result */
			if (pTouchParallelDlg->Check_Touch_Result())
				break;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndParallelTouchDlg()
{
	pTouchParallelDlg->OnDestroy();
	//CloseTouchParallelDlgThread();
	//delete pTouchParallelDlg;
	//pTouchParallelDlg = NULL;
	return true;
}

//Multi points Touch
UINT CTouchPanelProc::AfxTouchPointDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;
	pTouchPanelProc->pTouchPointDlg->DoModal();
	pTouchPanelProc->pTouchPointDlg->FreeResource();
	delete pTouchPanelProc->pTouchPointDlg;
	pTouchPanelProc->pTouchPointDlg = NULL;
	return true;
}

bool CTouchPanelProc::CloseTouchPointDlgThread()
{
	if (TouchPointThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchPointThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchPointThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialTouchPointDlg()
{
	pTouchPointDlg = new CTouchPointDlg;
	if (!pTouchPointDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate))
	{
		m_strMessage = "Initial Touch Point dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Point dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial touch Point test dialog success!");
	}
	TouchPointThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchPointDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchPointThreadHandle == NULL)
	{
		m_strMessage = "Create Touch point dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch point dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch point test dialog success!");
	}
	return true;
}

bool CTouchPanelProc::RunTouchPointProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Multi touch & touch point
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {4, 2, 0, 0}; //NV_DT_TOUCH
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			if (pTouchPointDlg->Check_Directly_Four_TouchPoint(chPointCoord))
				break;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Panel Test time out!";
		TraceLog(MSG_INFO, "Touch Panel Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndTouchPointDlg()
{
	pTouchPointDlg->OnDestroy();
	//CloseTouchPointDlgThread();
	//delete pTouchPointDlg;
	//pTouchPointDlg = NULL;
	return true;
}

//Capkey Touch Test
UINT CTouchPanelProc::AfxTouchCapkeyDlgThread(LPVOID pParam)
{
	pTouchPanelProc = (CTouchPanelProc*)pParam;
	pTouchPanelProc->pTouchCapkeyDlg->DoModal();
	pTouchPanelProc->pTouchCapkeyDlg->FreeResource();
	delete pTouchPanelProc->pTouchCapkeyDlg;
	pTouchPanelProc->pTouchCapkeyDlg = NULL;
	return true;
}

bool CTouchPanelProc::CloseTouchCapkeyDlgThread()
{
	if (TouchPointThreadHandle->m_hThread != NULL)
	{
		CloseHandle((HANDLE)TouchPointThreadHandle->m_hThread);
		//TerminateThread(ThreadHandle->m_hThread,0);
		TouchPointThreadHandle->m_hThread = NULL;
	}
	return true;
}

bool CTouchPanelProc::InitialTouchCapkeyDlg()
{
	pTouchCapkeyDlg = new CTouchCapkeyDlg;
	if (!pTouchCapkeyDlg->Initial_Parameter(m_iPanelHPixel, m_iPanelVPixel, m_iBlockLenght, m_iBlockWidth, m_iDisplayRate))
	{
		m_strMessage = "Initial Touch Capkey dialog fail!";
		TraceLog(MSG_ERROR, "Initial Touch Capkey dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial Touch Capkey test dialog success!");
	}

	TouchCapkeyThreadHandle = AfxBeginThread(CTouchPanelProc::AfxTouchCapkeyDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (TouchCapkeyThreadHandle == NULL)
	{
		m_strMessage = "Create Touch Capkey dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch Capkey dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create Touch Capkey test dialog success!");
	}
	return true;
}

bool CTouchPanelProc::RunTouchCapkeyProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;

	//Multi touch & touch point
	std::string strPointCoord;
	char chPointCoord[FTD_BUF_SIZE] = {0};
	char command[4] = {7, 1, 0, 0};//NV_DT_TOUCH
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, chPointCoord))
		{
			strPointCoord = chPointCoord;
			TraceLog(MSG_INFO, strPointCoord);
			pTouchCapkeyDlg->Check_TouchPoint_Position(chPointCoord);
			if (pTouchCapkeyDlg->Check_Touch_Result())
				break;
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			m_bStopTouchTest = true;
			break;
		}
	}

	//stop Touch Test
	if (m_bStopTouchTest)
	{
		m_strMessage = "Touch Capkey Test time out!";
		TraceLog(MSG_INFO, "Touch Capkey Test time out!");
		return false;
	}
	return true;
}

bool CTouchPanelProc::EndTouchCapkeyDlg()
{
	pTouchCapkeyDlg->OnDestroy();
	//CloseTouchPointDlgThread();
	//delete pTouchPointDlg;
	//pTouchPointDlg = NULL;
	return true;
}
