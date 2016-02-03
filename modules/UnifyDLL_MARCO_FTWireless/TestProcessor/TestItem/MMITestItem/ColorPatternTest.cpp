/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .cpp file of CColorPatternTest object.
 * FileName   : ColorPatternTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2013/03/04
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/03/04       CY.Lee          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "ColorPatternTest.h"


RF_IMPLEMENT_DYNCREATE(CColorPatternTest)

bool CColorPatternTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(LCD_Test_BaseItemCode);
	m_strErrorCode = "-";
	m_strResult = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CColorPatternTest::Run()
{
	bool bRes;

	bRes = ColorPatternTest();
	if(!bRes)
	{
		m_strErrorCode = FunErr_LCD_Display_Fail;
		m_strResult = "FAIL";
	}

	FactoryLog();

	return bRes;
}

bool CColorPatternTest::PostRun()
{
	return true;
}

bool CColorPatternTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strTemp;
	if (paramMap.find(std::string("CameraDelay")) != paramMap.end())
	{
		strTemp = paramMap[std::string("CameraDelay")];
		m_iCameraDelay = CStr::StrToInt(strTemp);
	}
	else
	{
		m_iCameraDelay = 1000;
		TraceLog(MSG_INFO,_T("Camera Delay set by default 1s"));
	}

	if (paramMap.find(std::string("RedThreshold")) != paramMap.end())
	{
		strTemp = paramMap[std::string("RedThreshold")];
		m_iRedThreshold = CStr::StrToInt(strTemp);
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find RedThreshold."));
		return false;
	}

	if (paramMap.find(std::string("GreenThreshold")) != paramMap.end())
	{
		strTemp = paramMap[std::string("GreenThreshold")];
		m_iGreenThreshold = CStr::StrToInt(strTemp);
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find GreenThreshold."));
		return false;
	}

	if (paramMap.find(std::string("BlueThreshold")) != paramMap.end())
	{
		strTemp = paramMap[std::string("BlueThreshold")];
		m_iBlueThreshold = CStr::StrToInt(strTemp);
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find BlueThreshold."));
		return false;
	}

	if (paramMap.find(std::string("BackLight")) != paramMap.end())
	{
		m_strBackLight = paramMap[std::string("BackLight")];
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find BackLight. default set 100."));
		m_strBackLight = _T("100");
		return true;
	}

	if (paramMap.find(std::string("PROP_BRIGHTNESS")) != paramMap.end())
	{
		strTemp = paramMap[std::string("PROP_BRIGHTNESS")];
		d_Prop_Brightness = CStr::StrToDouble(strTemp);
		CString prop_log;
		prop_log.Format("PROP_BRIGHTNESS = %lf", d_Prop_Brightness);
		TraceLog(MSG_INFO, prop_log.GetString());
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find PROP_BRIGHTNESS. default set 50."));
		d_Prop_Brightness = 50.0;
		return true;
	}

	if (paramMap.find(std::string("PROP_BRIGHTNESS")) != paramMap.end())
	{
		strTemp = paramMap[std::string("PROP_BRIGHTNESS")];
		d_Prop_Brightness = CStr::StrToDouble(strTemp);
		CString prop_log;
		prop_log.Format("PROP_BRIGHTNESS = %lf", d_Prop_Brightness);
		TraceLog(MSG_INFO, prop_log.GetString());
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find PROP_BRIGHTNESS. default set 50."));
		d_Prop_Brightness = 50.0;
		return true;
	}

	//d_Prop_CONTRAST
	if (paramMap.find(std::string("PROP_CONTRAST")) != paramMap.end())
	{
		strTemp = paramMap[std::string("PROP_CONTRAST")];
		d_Prop_Brightness = CStr::StrToDouble(strTemp);
		CString prop_log;
		prop_log.Format("PROP_CONTRAST = %f", d_Prop_CONTRAST);
		TraceLog(MSG_INFO, prop_log.GetString());
	}
	else
	{
		TraceLog(MSG_INFO,_T("CColorPatternTest can't find PROP_CONTRAST default set 50."));
		d_Prop_CONTRAST = 50.0;
		return true;
	}

	return true;
}

bool CColorPatternTest::CaptureColor(char *Color)
{
	// Display required pattern on mobile LCD //
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Display_LCD_RGB(m_nFtdPort, m_nFtdTimeOut, Color, sz_outBuffer))
	{
		m_strMessage = "Fail to set Display_LCD_RGB command.";
		return false;
	}

	char strTemp[128];
	if(CFcvGrabFrame(m_capture))
	{
		CFcvRetrieveFrame(m_capture, 0);
		m_pImg = CFcvRetrieveFrame(m_capture, 0);
		sprintf(strTemp, "%s.jpg", Color);
		CFcvSaveImage(strTemp, m_pImg, 0);
		return true;
	}
	else
	{
		sprintf(strTemp, "Color %s Pattern cant get capture.", Color);
		m_strMessage = strTemp;
		return false;
	}
}

bool CColorPatternTest::Analysis(char *Color, int Threshold)
{
	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	//設定Shell Execute參數
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";
	ShExecInfo.lpDirectory = szModulePath;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char temp[1024];
	sprintf(temp, "/C ColorTest load %s.jpg %d %s", Color, Threshold, Color );
	TraceLog(MSG_INFO, temp);
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	HANDLE hRead;
	sprintf(temp, "%s\\log.txt", szModulePath );
	hRead = CreateFile(temp, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hRead == INVALID_HANDLE_VALUE)
	{
		m_strMessage = "CAN NOT OPEN HANDLE!";
		FactoryLog();
		return false;
	}

	DWORD dcount;
	char temp_buf[1024] = {0};
	::ReadFile(hRead, temp_buf, 1024, &dcount, NULL);
	TraceLog(MSG_INFO, temp_buf);

	bool bret = true;
	char *str_data[4];
	str_data[0] = strtok(temp_buf, "\n");
	str_data[1] = strtok(NULL, "\n");
	str_data[2] = strtok(NULL, "\n");
	str_data[3] = strtok(NULL, "\n");
	if(strcmp(str_data[3], "FAIL") == 0)
	{
		sprintf(temp, "%s %s", str_data[1], str_data[2] );
		m_strMessage = temp;
		FactoryLog();

		sprintf(temp, "[%s] Pattern Test fail.", Color);
		m_strMessage = temp;
		bret = false;
	}
	else
	{
		sprintf(temp, "[%s] Pattern Test pass.", Color);
		m_strMessage = temp;
		FactoryLog();
	}
	CloseHandle(hRead);
	hRead = NULL;
	return bret;
}

bool CColorPatternTest::CaptureCapKey()
{
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, "1,1,1", sz_outBuffer))
	{
		m_strMessage = "Fail to set Detroit_LEDon command.";
		return false;
	}

	//-- Turn the back light off  --//
	if(!m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer))
	{
		m_strMessage = "Fail to set LED_BacklightOnOff command.";
		return false;
	}

	if(CFcvGrabFrame(m_capture))
	{
		CFcvRetrieveFrame(m_capture, 0);
		m_pImg = CFcvRetrieveFrame(m_capture, 0);
		CFcvSaveImage(_T("CapKey.jpg"), m_pImg, 0);
	}
	else
	{
		m_strMessage = "CapKey cant get capture.";
		return false;
	}

	//-- Turn the back light lighter  --//
	char strInBuffer[3] = {0};
	strcpy(strInBuffer, m_strBackLight.c_str());
	if(!m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, strInBuffer, sz_outBuffer))
	{
		m_strMessage = "Fail to set LED_BacklightOnOff command.";
		return false;
	}
	if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, "0,0,0", sz_outBuffer))
	{
		m_strMessage = "Fail to set Detroit_LEDoff command.";
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "CapKey Capture pass!";
	FactoryLog();
	return true;
}

bool CColorPatternTest::ColorPatternTest()
{
	if(!LoadOpenCVDLL())
		return false;

	//-- Initial camera --//
	m_capture = CFcvCreateCameraCapturey(0);
	CFcvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH,640);
	CFcvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT,480);
	//CFcvSetCaptureProperty(m_capture, CV_CAP_PROP_BRIGHTNESS, d_Prop_Brightness);//liontest
	//CFcvSetCaptureProperty(m_capture, CV_CAP_PROP_CONTRAST, d_Prop_CONTRAST);//liontest

	if(!m_capture)
	{
		m_strMessage = "Camera open fail";
		return false;
	}

	Sleep(m_iCameraDelay);
	m_strResult = "PASS";
	m_strMessage = "Initial Capture OK.";
	FactoryLog();

	//-- Capture CapKey --//
	if(!CaptureCapKey())
	{
		CFcvReleaseCapture( &m_capture );		// Close camera
		return false;
	}

	//-- Turn the back light lighter  --//
	char strOutBuffer[FTD_BUF_SIZE] = {0};
	char strInBuffer[3] = {0};
	strcpy(strInBuffer, m_strBackLight.c_str());
	if(!m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, strInBuffer, strOutBuffer))
	{
		m_strMessage = "Fail to set LED_BacklightOnOff command.";
		CFcvReleaseCapture( &m_capture );		// Close camera
		return false;
	}

	//-- Color Test --//
	char *strColorModel[3] = {"r", "g", "b"};
	int iThreshold[3] = {m_iRedThreshold, m_iGreenThreshold, m_iBlueThreshold};
	for (int i = 0; i < 3; i++)
	{
		if(!CaptureColor(strColorModel[i]))
		{
			CFcvReleaseCapture( &m_capture );// Close camera
			return false;
		}
		if(!Analysis(strColorModel[i], iThreshold[i]))
		{
			CFcvReleaseCapture( &m_capture );// Close camera
			return false;
		}
	}
	CFcvReleaseCapture( &m_capture );// Close camera

	m_strResult = "PASS";
	m_strMessage = "Color Pattern Test pass!";
	return true;
}

bool CColorPatternTest::LoadOpenCVDLL()
{
	m_hHighgui = NULL;
	m_hHighgui = LoadLibrary("opencv_highgui240.dll");
	if(m_hHighgui == NULL)
	{
		m_strMessage = "Load openCV DLL fail";
		return false;
	}

	CFcvCreateCameraCapturey = (PFcvCreateCameraCapture)GetProcAddress(m_hHighgui, "cvCreateCameraCapture");
	if(CFcvCreateCameraCapturey == NULL)
	{
		m_strMessage = "Load function CFcvCreateCameraCapturey fail\n";
		return false;
	}
	CFcvSetCaptureProperty = (PFcvSetCaptureProperty)GetProcAddress(m_hHighgui, "cvSetCaptureProperty");
	if(CFcvSetCaptureProperty == NULL)
	{
		m_strMessage = "Load function CFcvSetCaptureProperty fail\n";
		return false;
	}
	CFcvSaveImage = (PFcvSaveImage)GetProcAddress(m_hHighgui, "cvSaveImage");
	if(CFcvSaveImage == NULL)
	{
		m_strMessage = "Load function CFcvSaveImage fail\n";
		return false;
	}
	CFcvGrabFrame = (PFcvGrabFrame)GetProcAddress(m_hHighgui, "cvGrabFrame");
	if(CFcvGrabFrame == NULL)
	{
		m_strMessage = "Load function CFcvGrabFrame fail\n";
		return false;
	}
	CFcvRetrieveFrame = (PFcvRetrieveFrame)GetProcAddress(m_hHighgui, "cvRetrieveFrame");
	if(CFcvRetrieveFrame == NULL)
	{
		m_strMessage = "Load function CFcvRetrieveFrame fail\n";
		return false;
	}
	CFcvReleaseCapture = (PFcvReleaseCapture)GetProcAddress(m_hHighgui, "cvReleaseCapture");
	if(CFcvReleaseCapture == NULL)
	{
		m_strMessage = "Load function CFcvReleaseCapture fail\n";
		return false;
	}
	return true;
}
