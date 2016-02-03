/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "FQC.h"
#include <string> 

template <class T> 
std::string ConvertToString(T);

RF_IMPLEMENT_DYNCREATE(CFQC)

template <class T> 
std::string ConvertToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

bool CFQC::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CFQC::Run()
{
	bool passFail = false;

	if (m_str_TestItem == FineWifiAP)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_WLAN_Connect_Fail;
		str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = runFindWifiAP();
	}
	else if (m_str_TestItem == USBCharging)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_USB_Charging_Current_Test_Fail;
		str_Pics = CW2A(L"USB_Charging");
		passFail = runUSBCharging();
	}
	else if (m_str_TestItem == OLEDTest)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_LED_Test_Fail;
		str_Pics = CW2A(L"OLED_Test");
		passFail = runOLEDTest();
	}
	else if (m_str_TestItem == LEDTest)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_LED_Test_Fail;
		str_Pics = CW2A(L"LED_Test");
		passFail = runLEDTest();
	}
	else if (m_str_TestItem == BatteryTest)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_ID_Fail;
		str_Pics = "Battery_Test";
		passFail = runBatteryTest();
	}/***/
	else if ( m_str_TestItem == SensorBoardEEPROM )
	{
		m_str_TestItem = CStr::IntToStr(Camera_Test_BaseItemCode);
		m_strErrorCode = FunErr_Front_Camera_Write_FuseID_Fail;
		str_Pics = "SensorBoard_EEPROM";
		passFail = runSensorBoardEEPROMTest();
		
	}
	else if ( m_str_TestItem == SensorBoardFPGA_ID )
	{
		m_str_TestItem = CStr::IntToStr(ReadFPGA_ID_BaseItemCode);
		m_strErrorCode = FunErr_FPGA_ID_Fail;
		str_Pics = "SensorBoardFPGA_ID";
		passFail = runSensorBoardFpgaID();
		
	}
	else if ( m_str_TestItem == CameraTest )
	{
		m_str_TestItem = CStr::IntToStr(Camera_Test_BaseItemCode);
		m_strErrorCode = FunErr_Camera_Test_Fail;
		str_Pics = "Camera_Test";
		passFail = runCameraTest();
		
	}
	else if ( m_str_TestItem == ReadBatteryVoltage )
	{
		m_str_TestItem = CStr::IntToStr(BatteryCapacity_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_Voltage_Fail;
		str_Pics = "Bettery_Voltage";
		passFail = runReadBattery_Voltage();
		
	}
	else if ( m_str_TestItem == ReadBatteryCurrent )
	{
		m_str_TestItem = CStr::IntToStr(BatteryCapacity_BaseItemCode);
		m_strErrorCode = FunErr_Check_Battery_Current_Fail;
		str_Pics = "Bettery_Current";
		passFail = runReadBattery_Current();
		
	}

	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}

	return passFail;

}

bool CFQC::PostRun()
{
	return true;
}

bool CFQC::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CFQC'");
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("XMLSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLSleepTime")];
		m_i_SleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CFQC");
		return false;
	}

	if (paramMap.find(std::string("XMLCMDItem")) != paramMap.end())
	{
		m_str_CMD = paramMap[std::string("XMLCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLCMDItem for CFQC");
		return false;
	}

	if (paramMap.find(std::string("XMLOffCMDItem")) != paramMap.end())
	{
		m_str_OffCMD = paramMap[std::string("XMLOffCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLOffCMDItem for CFQC");
		return false;
	}

	std::string str_MexRetryTime;
	if (paramMap.find(std::string("XMLRetryTime")) != paramMap.end())
	{
		str_MexRetryTime = paramMap[std::string("XMLRetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_MexRetryTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CFQC");
		return false;
	}

	return true;
}

bool CFQC::runSensorBoardEEPROMTest()
{
	bool b_res = true;
	bool b_res_check = false;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	std::string std_input = m_str_CMD;
	std::string std_output;
	/*open camera*/

	if (!m_pIPhone->FTD_CAMstart(m_nFtdPort, m_nFtdTimeOut, _T("b"), szOutnput ))
	{
		str_msg = "CFQC, FTD_CAMstart fail 1";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	Sleep(m_i_SleepTime);
	/*write*/
	if (b_res){
		strcpy(szInput, m_str_CMD.c_str());				
		if (!m_pIPhone->FTD_CAM_OTP_WriteFrontFuseID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "CFQC, FTD_CAM_OTP_WriteFrontFuseID fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	/*read*/
	if (b_res){
		strcpy(szInput, CW2A(L""));				
		if (!m_pIPhone->FTD_CAM_OTP_FrontFuseID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "CFQC, FTD_CAM_OTP_FrontFuseID fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	CString csOutnput = szOutnput;
	csOutnput.Trim();
	
	char sz_outputValue[20] = {0};
	if (b_res){
		std_output = CT2A(csOutnput);
		if (std_input == std_output){
			b_res_check = true;
			str_msg = _T("CFQC, write = read, pass! w = ") + std_input + _T("r = ") + std_output;
			TraceLog(MSG_INFO, str_msg);
		}else{
			b_res_check = false;
			str_msg = _T("CFQC, write != read, false! w = ") + std_input + _T("r = ") + std_output;
			TraceLog(MSG_INFO, str_msg);
		}

		/*stop camera*/
		if (!m_pIPhone->FTD_CAMend(m_nFtdPort, 45000,  CW2A(L""), szOutnput ))
		{
			//str_msg = "CFQC, FTD_CAMend fail";
			TraceLog(MSG_INFO,  _T("CFQC, FTD_CAMend fail"));
			b_res = false;
		}
	}

	/* test result */
	if( b_res_check && b_res )
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "SensorBoardEEPROMTest PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		SetPICSData(str_Pics, "FAIL");
		TraceLog(MSG_INFO, "SensorBoardEEPROMTest FAIL!");
		FactoryLog();
		return false;
	}

	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;

}

bool CFQC::runSensorBoardFpgaID()
{
	bool b_res = true;
	bool b_res_check = false;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	std::string std_input = m_str_CMD;
	std::string std_output;
	/*read , 0h412b4043*/
	strcpy(szInput,  CW2A(L""));

	if (!m_pIPhone->FTD_CAM_Read_Bridge_ID(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
	{
		str_msg = "runSensorBoardFpgaID";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	if (b_res){
		 /*compare input and output*/
		std_output = szOutnput;
		if ( std_input == std_output){
			b_res_check = true;
			str_msg = _T("CFQC,runSensorBoardFpgaID  pass! data = ") +  std_output;
			TraceLog(MSG_INFO, str_msg);
		}else{
			b_res_check = false;
			str_msg = _T("CFQC,runSensorBoardFpgaID fail! xmldata = ") +  std_input + _T(", readdata = ") + std_output;
			TraceLog(MSG_INFO, str_msg);
		}
	}else{
		str_msg = _T("CFQC,runSensorBoardFpgaID CMD fail! data = ");
		TraceLog(MSG_INFO, str_msg);
	}

	/* test result */
	if( b_res_check && b_res )
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runSensorBoardFpgaID PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		SetPICSData(str_Pics, "FAIL");
		TraceLog(MSG_INFO, "runSensorBoardFpgaID FAIL!");
		FactoryLog();
		return false;
	}

	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;
}


CString CFQC::GetDirectory_Img()
{
	CString strDesPath;
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	strDesPath = pthToolDir;
	strDesPath += _T("\\Img");
	CFileFind cfilefind;
	if (!cfilefind.FindFile(strDesPath))
	{
		if (!CreateDirectory(strDesPath, NULL))
		{
			AfxMessageBox(_T("Create folder Img Failed!"));
		}
	}
	return strDesPath;
}

bool CFQC::GetDirectory_D_LOG_IMG()
{
	CString strDesPath, strDesPathFail;
	strDesPath = _T("D:\\LOG\\ImgAndResult");
	strDesPathFail = _T("D:\\LOG\\ImgAndResult_Fail");
	CFileFind cfilefind;
	if (!cfilefind.FindFile(strDesPath))
	{
		if (!CreateDirectory(strDesPath, NULL))
		{
			AfxMessageBox(_T("Create GetDirectory_D_LOG_IMG Failed!"));
			return false;
		}
	}
	if (!cfilefind.FindFile(strDesPathFail))
	{
		if (!CreateDirectory(strDesPathFail, NULL))
		{
			AfxMessageBox(_T("Create GetDirectory_D_LOG_IMG Failed!"));
			return false;
		}
	}
	return true;
}

bool CFQC::DeleteOldPicture()
{
	// show rearground picture
	CString csBGDImgName;
	csBGDImgName.Format("%s%s", _T("background"), _T(".jpg "));
	CString csDirectory = GetDirectory_Img();
	csDirectory += _T("\\");
	CString csPath;
	csPath.Format("%s%s", csDirectory, csBGDImgName);
	m_imgPath = csPath;
	ShowPicture(false, csPath.GetBuffer());
	TraceLog(MSG_ERROR, "ShowPicture Successful");

	CString csImgName0, csImgName1, csImgName2, csImgName3, csImgName4;
	csImgName0.Format("%s", _T("snapshot_0.jpg"));

	CString csPicPath;
	csPicPath.Format("%s%s", csDirectory, csImgName0);
	DeleteFile(csPicPath);//Delete Old Image
	m_strMessage = "Delete OLD Picture Fail ";
	CFileFind cFileFind;
	if (cFileFind.FindFile(csPicPath)) /*make sure old picture is not exist*/
	{
		AfxMessageBox("Delete OLD Picture Fail 0");
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	m_strMessage = "Delete OLD Picture PASS";
	TraceLog(MSG_ERROR, m_strMessage);
	return true;
}


bool CFQC::runCameraTest()/*on going*/
{
	GetDirectory_Img();//create fold to keep img
	GetDirectory_D_LOG_IMG();

	bool b_res = true;
	bool b_res_check = false;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	std::string std_input = m_str_CMD;
	std::string std_output;

	/* Remove all [snapshot_*.jpg] */
	CString csCommandShell(_T("shell rm -r /data/snapshot_*.jpg"));
	char szOutPullCommand[4096] = {0};
	char szErrorCodePullCommand[4096] = {0};
	ExecAdbOut(csCommandShell, szOutPullCommand, szErrorCodePullCommand);

	DeleteOldPicture();
	///*start camera*/
	strcpy(szInput,  CW2A(L"back"));
	if (!m_pIPhone->FTD_CAMstart(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
	{
		str_msg = "runCameraTest FTD_CAMstart fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	///* take picture*/
	if (b_res){
		std_input = m_str_CMD;// back,normal,0,MTF,Auto
		//strcpy(szInput,  CW2A(L""));
		if (!m_pIPhone->FTD_CAMshot(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "runCameraTest FTD_CAMshot fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	//3.get pic from target to PC  (adb pull /sdcard/DCIM/Camera)
	CString csImgName;
	char szImgCount[5];
	memset(szImgCount, 0 , sizeof(szImgCount));
	int i_shot_number = 0;
	_itoa_s(i_shot_number, szImgCount, 10);
	csImgName.Format("%s%s%s", _T("snapshot_"), szImgCount, _T(".jpg "));
	CString csCommandPull;
	CString csDirectory = GetDirectory_Img();
	csCommandPull.Format("%s%s%s", _T("pull /data/"), csImgName, csDirectory);
	if (!ExecAdbOut(csCommandPull, szOutPullCommand, szErrorCodePullCommand))
	{
		TraceLog(MSG_ERROR, "pull picture fail ");
		return false;
	}

	//5.show pic
	CString csPicPath;
	CString csImgName2;
	csImgName2.Format("%s%s%s", _T("snapshot_"), szImgCount, _T(".jpg "));
	csDirectory += _T("\\");
	csPicPath.Format("%s%s", csDirectory, csImgName2);;
	m_imgPath = csPicPath;
	ShowPicture(false, csPicPath.GetBuffer());

	/*get img size */
	//char* pszFile;
	CFileStatus rStatus;
	rStatus.m_size = 0 ;
	CFile::GetStatus(csPicPath, rStatus);
	ULONGLONG nSize = rStatus.m_size;
	if (nSize < 1)
	{
		AfxMessageBox("picture size = 0 ");
		return false;
	}
	/*get img size */

	///*turn off camera*/
	if (b_res){
		if (!m_pIPhone->FTD_CAMend(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "runCameraTest FTD_CAMend fail";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	std::string str_msg = CW2A(L"图片合格吗? Is PASS ?");
	b_res_check = showMsg(str_msg.c_str());


	if ( b_res && b_res_check )
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runCameraTest PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		SetPICSData(str_Pics, "FAIL");
		TraceLog(MSG_INFO, "runCameraTest FAIL!");
		FactoryLog();
		return false;
	}


	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;
}



	//bool ();
	

bool CFQC::runReadBattery_Voltage()
{
	bool b_res_Voltage = false;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};

	std::string msg;
	memset(szOutnput, 0, FTD_BUF_SIZE);
	memset(szInput, 0, FTD_BUF_SIZE);

	m_str_CMD = CW2A(L"i");// turn on 
	strcpy(szInput, m_str_CMD.c_str());//
	/***************** battery voltage ****************/
	m_strErrorCode = FunErr_Check_Battery_Voltage_Fail;
	b_res_Voltage = m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//

	if (b_res_Voltage)
	{
		str_msg = CW2A(L"runReadBattery_Voltage Voltage cmd pass = ") + szOutnput;
	}
	else
	{
		str_msg =  CW2A(L"runReadBattery_Voltage Voltage cmd Fail") ;

	}
	TraceLog(MSG_INFO, str_msg);


	if ( !b_res_Voltage ) /*fail*/
	{
		m_strResult = "Voltage cmd Fail";
		SetPICSData(str_Pics, "Voltage cmd Fail");
		FactoryLog();
		return false;
	}else{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		SetPICSData(str_Pics , szOutnput );
	}
	FactoryLog();
	return true;
}

bool CFQC::runReadBattery_Current()
{
	bool b_res_Current = false;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};

	std::string msg;

	memset(szOutnput, 0, FTD_BUF_SIZE);
	memset(szInput, 0, FTD_BUF_SIZE);

	m_str_CMD = CW2A(L"i");// turn on 
	strcpy(szInput, m_str_CMD.c_str());//


	b_res_Current = m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//

	if (b_res_Current)
	{
		str_msg = CW2A(L"runReadBattery_Current cmd pass = ") + szOutnput ;
	}
	else
	{
		str_msg =  CW2A(L"runReadBattery_Current cmd Fail");

	}
	TraceLog(MSG_INFO, str_msg);


	if ( !b_res_Current ) /*fail*/
	{
		m_strResult = "Fail";
		SetPICSData(str_Pics, "Current cmd Fail");
		FactoryLog();
		return false;
	}else{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		SetPICSData(str_Pics , szOutnput );
	}
	FactoryLog();
	return true;
}


bool CFQC::runBatteryTest()
{
	m_i_retriedTime = 0;
	std::string msg;
	CString cs_TestTime;

	m_strItemCode = CStr::IntToStr(BatteryCapacity_BaseItemCode);
	bool b_res, b_res_a, b_res_b;
	int n_Voltage = 0;
	int  n_Temperature = 0;
	float f_Voltage_Limit = 0;
	int  n_Temperature_LowBond = 0;
	int  n_Temperature_UpBond = 0;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
		
	b_res = false;
	b_res_a = false;
	b_res_b = false;
	b_res_off = false;
	b_res_msg_try = false;
	b_res_msg_retry = false;


	StrVtr vToken;
	CStr::ParseString(m_str_CMD.c_str(), _T(";"), vToken);
	sscanf(vToken[0].c_str(),"%f", &f_Voltage_Limit);
	f_Voltage_Limit = f_Voltage_Limit * 1000000;

	CStr::ParseString(vToken[1].c_str(), _T(","), vToken);
	//n_Voltage_Limit = CStr::StrToInt(vToken[0].c_str());
	n_Temperature_LowBond = CStr::StrToInt(vToken[0].c_str());
	n_Temperature_LowBond = n_Temperature_LowBond * 10;
	n_Temperature_UpBond = CStr::StrToInt(vToken[1].c_str());
	n_Temperature_UpBond = n_Temperature_UpBond * 10;

    /*****************  ****************/

	memset(szOutnput, 0, FTD_BUF_SIZE);
	memset(szInput, 0, FTD_BUF_SIZE);

	m_str_CMD = CW2A(L"i");// turn on 
	strcpy(szInput, m_str_CMD.c_str());//

	/***************** battery voltage ****************/
	b_res_a = m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//

	if (b_res_a)
	{
		sscanf(szOutnput,"%d", &n_Voltage);
		if( n_Voltage > f_Voltage_Limit){
			str_msg =  _T("runBatteryTest Voltage value Pass = ") + CStr::IntToStr(n_Voltage);
		}else{
			str_msg =  _T("runBatteryTest Voltage value Fail = ") +  CStr::IntToStr(n_Voltage);
		}	
	}
	else
	{
		str_msg =  CW2A(L"runBatteryTest Voltage cmdFail");
		b_res_a = false;
	}
	TraceLog(MSG_INFO, str_msg);

	/***************** battery temp *****************/
	b_res_b = m_pIPhone->FTD_PWRgaugeTemperature(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);

	if (b_res_b)
	{
		sscanf(szOutnput,"%d", &n_Temperature);
		if(( n_Voltage > n_Temperature_LowBond)&&( n_Voltage < n_Temperature_UpBond)){
			str_msg =  _T("runBatteryTest Voltage value Pass = ") + CStr::IntToStr(n_Temperature);
		}else{
			str_msg =  _T("runBatteryTest Voltage value Fail = ") + CStr::IntToStr(n_Temperature);
		}
	}
	else
	{
		str_msg =  CW2A(L"runBatteryTest Temperature cmd Fail");
		b_res_b = false;
	}
	TraceLog(MSG_INFO, str_msg);

	/************************************************/

	cs_TestTime.Format(CW2A(L"runBatteryTest - TestTime = %d"), m_i_retriedTime);
	str_msg = cs_TestTime.GetString();
	TraceLog(MSG_INFO, str_msg);

	m_strMessage =   _T("Vol = ") + CStr::IntToStr(n_Voltage) + _T(", Temp = ") + CStr::IntToStr(n_Temperature);

	if ((!b_res_a) || (!b_res_b)) /*fail*/
	{
		m_strResult = "FAIL";
		SetPICSData(str_Pics, "FAIL");
		b_res = false;
	}else{
		b_res = true;
	}

	m_strResult = "PASS";
	m_strErrorCode = "-";
	SetPICSData(str_Pics , "PASS");

	FactoryLog();

	return b_res;
}



bool CFQC::runLEDTest()
{
	m_i_retriedTime = 0;
	std::string msg;
	CString cs_TestTime;
	
	std::string str_CMD_Msg_Pass, str_CMD_Msg_Fail, str_Result_Msg_Pass, str_Result_Msg_Fail;
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);

	msg = CW2A(L"Is the LED lighting ?\n LED有亮吗?\n");

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(szOutnput, 0, FTD_BUF_SIZE);
		memset(szInput, 0, FTD_BUF_SIZE);
		strcpy(szInput, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//turn on
	//	Sleep(m_i_SleepTime);
		cs_TestTime.Format(CW2A(L"runLEDTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = str_CMD_Msg_Pass;
		}
		else
		{
			str_msg = str_CMD_Msg_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(msg.c_str());
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = str_Result_Msg_Pass;
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = str_Result_Msg_Fail;
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(szInput, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput);//turn on
		//Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = str_CMD_Msg_Fail;
		}
		else
		{
			str_msg = str_CMD_Msg_Pass;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}

	SetPICSData(str_Pics , "PASS");
	return true;
}


bool CFQC::runOLEDTest()
{
	m_i_retriedTime = 0;
	m_strItemCode = CStr::IntToStr(Backlight_Test_BaseItemCode);
	CString cs_TestTime;
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		m_str_CMD = CW2A(L"1");// turn on 
		memset(szOutnput, 0, FTD_BUF_SIZE);
		memset(szInput, 0, FTD_BUF_SIZE);
		strcpy(szInput, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_OLEDTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput );//turn on
	//	Sleep(m_i_SleepTime);

		cs_TestTime.Format(CW2A(L"runOLEDTest - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);


		if (b_res_on)
		{
			str_msg = "runOLEDTest Pass";
		}
		else
		{
			str_msg = "runOLEDTest Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Is OLED on?\n OLED有亮吗?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runOLEDTest pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runOLEDTest fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		m_str_CMD = CW2A(L"0");// turn off
		strcpy(szInput, m_str_CMD.c_str());
		b_res_off = m_pIPhone->FTD_OLEDTest(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput );//turn on

		//Sleep(m_i_OffSleepTime);

		if (b_res_off)
		{
			str_msg = "runOLEDTest Cmd pass!";
		}
		else
		{
			str_msg = "runOLEDTest Cmd fail!";

			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}

	SetPICSData(str_Pics, "PASS");
	return true;
}

bool CFQC::runUSBCharging()
{
	bool b_res = true;
	bool checkCurrent = false;
	int ChargerOffCurrent = 0;
	int chargerOnCurrent = 0;

	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};

	m_str_CMD = CW2A(L"1");// turn on 
	strcpy(szInput, m_str_CMD.c_str());

	if (!m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
	{
		str_msg = "CFQC, FTD_PWRchargerOn fail 1";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	Sleep(m_i_SleepTime);
	if (b_res){
		m_str_CMD = CW2A(L"i");
		strcpy(szInput, m_str_CMD.c_str());				
		if (!m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "CFQC, FTD_PWRgaugeCurrent fail 1";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	Sleep(m_i_SleepTime);
	if (b_res){
		sscanf(szOutnput,"%d", &ChargerOffCurrent);
		m_str_CMD = CW2A(L"1");
		strcpy(szInput, m_str_CMD.c_str());		

		if (!m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "CFQC, FTD_PWRchargerOn fail 2";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}

	Sleep(m_i_SleepTime);
	if (b_res){
		m_str_CMD = CW2A(L"i");
		strcpy(szInput, m_str_CMD.c_str());	

		if (!m_pIPhone->FTD_PWRgaugeCurrent(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
		{
			str_msg = "CFQC, FTD_PWRgaugeCurrent fail 2";
			TraceLog(MSG_INFO, str_msg);
			b_res = false;
		}
	}


	if (b_res){
		sscanf(szOutnput,"%d", &chargerOnCurrent);

		if ( chargerOnCurrent < 0 ) checkCurrent = true; //rule 1
		else//rule 2
		{
			if (chargerOnCurrent - ChargerOffCurrent < 0 ) checkCurrent = true;
		}
		str_msg = "chargerOnCurrent:" +  ConvertToString(chargerOnCurrent)  + ", ChargerOffCurrent:" 
				+  ConvertToString(ChargerOffCurrent);
	}

	/* test result */
	if((b_res)&&(checkCurrent))
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "USBCharging PASS!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		SetPICSData(str_Pics, "FAIL");
		TraceLog(MSG_INFO, "USBCharging FAIL!");
		FactoryLog();
		return false;
	}

	SetPICSData(str_Pics, "PASS");
	FactoryLog();
	return true;
}


bool CFQC::runFindWifiAP()
{
	bool b_res = true;

	/* read ID from mobile */
	char szInput[FTD_BUF_SIZE] = {0};
	char szOutnput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, m_str_CMD.c_str());	
	if (!m_pIPhone->FTD_WLAN_ScanAP(m_nFtdPort, m_nFtdTimeOut, szInput, szOutnput ))
	{
		b_res = false;
	}

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"FindWifiAP PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "FindWifiAP PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"FindWifiAP ");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "FindWifiAP FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}


bool CFQC::showMsg(const char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		i_waitCount++;
		Sleep(200);
	}
	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}


bool CFQC::ExecAdbOut(CString Command, char* output, char* ErrorCode)
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
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
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
			strcpy(ErrorCode, "Adb command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}