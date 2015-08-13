/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraEnd object.
 * FileName   : CameraEnd.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "CameraEnd.h"


RF_IMPLEMENT_DYNCREATE(CCameraEnd)


bool CCameraEnd::PreRun()
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

bool CCameraEnd::Run()
{
	return CameraEnd();
}

bool CCameraEnd::PostRun()
{
	return true;
}

bool CCameraEnd::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CCameraEnd::CameraEnd()
{
	bool b_res = false;
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_CAMend(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer, sz_outBuffer)))
	{
		TraceLog(MSG_INFO, "Fail to turn off mobile Camera");
	}
	else
	{
		TraceLog(MSG_INFO, "Mobile Camera turn off success");
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Camera off test pass!";
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Camera off test fail!";
		TraceLog(MSG_INFO, "Camera off test fail!");
	}
	FactoryLog();
	return b_res;
}
