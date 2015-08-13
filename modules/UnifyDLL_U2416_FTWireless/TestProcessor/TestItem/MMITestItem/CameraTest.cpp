/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraTest object.
 * FileName   : CameraTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "CameraTest.h"


RF_IMPLEMENT_DYNCREATE(CCameraTest)


bool CCameraTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Camera_Test_BaseItemCode);
	m_strErrorCode = FunErr_Camera_Test_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCameraTest::Run()
{
	return CameraFrontBack();
}

bool CCameraTest::PostRun()
{
	return true;
}

bool CCameraTest::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("CameraType")) != paramMap.end())
	{
		m_str_patternType = paramMap[std::string("CameraType")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter CameraIndex for CCameraTest");
		return false;
	}
	return true;
}

bool CCameraTest::CameraFrontBack()
{
	bool b_res = true;
	std::string str_pattern_English;
	std::string str_pattern_Chinese;
	std::string str_msg;
	std::string str_Pics;

	/* Get Camera pattern */
	if (m_str_patternType.compare("front") == 0)
	{
		str_pattern_English = "front";
		str_pattern_Chinese = CW2A(L"前镜头");
		str_Pics = "CAMERA_FRONT_TEST";
	}
	else if (m_str_patternType.compare("back") == 0)
	{
		str_pattern_English = "back";
		str_pattern_Chinese = CW2A(L"後镜头");
		str_Pics = "CAMERA_BACK_TEST";
	}
	else
	{
		str_msg = _T("required Camera type is not defined.");
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}
	if (b_res)
	{
		char sz_CameraType[FTD_BUF_SIZE] = {0};
		strcpy(sz_CameraType, m_str_patternType.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_CAMstart(m_nFtdPort, m_nFtdTimeOut, sz_CameraType, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Fail to turn on mobile Camera";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Mobile Camera turn on success";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	/* Query operator if camera pattern is required pattern */
	if (b_res)
	{
		/* Init dialog response flag to false */
		m_b_DialogResponse = false;
		m_b_waitFlag = false;

		/* Notify UI to show message dialog */
		str_msg = std::string("Is this camera turn on from ") + str_pattern_English + "?\n" +
			std::string(CW2A(L"手机上显示的画面是")) + str_pattern_Chinese + std::string(CW2A(L"吗？"));
		ShowDialog(str_msg.c_str());

		/* Wait UI response(Max:10s) */
		int i_waitCount = 0;
		while (m_b_waitFlag != true)
		{
			if (i_waitCount == 50)
			{
				str_msg = "Wait UI response time out.";
				TraceLog(MSG_INFO, "Wait UI response time out.");
				b_res = false;
				break;
			}
			i_waitCount++;
			Sleep(200);
		}
		if (b_res)
		{
			/* Get UI dialog operator response */
			if (m_b_DialogResponse == true)
			{
				str_msg = "The camera is required type.";
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				b_res = false;
				str_msg = "The camera is not required type.";
				TraceLog(MSG_INFO, str_msg);
			}
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Camera front/back test pass!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Camera front/back test fail!");
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
