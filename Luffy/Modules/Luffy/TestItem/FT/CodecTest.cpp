#include "stdafx.h"
#include "CodecTest.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CCodecTest)


bool CCodecTest::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "Codec Test", "", "", "", "", "", m_strMeasured, "", "Success to Codec Test");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Codec_ID_Check_Fail;

		FactoryLog(false, "Codec Test", m_strErrCode, "", "", "", "", m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CCodecTest::MainFunction()
{
	char szInputCmd[4096] = {0};
	memset(szInputCmd, 0, sizeof szInputCmd);

	char chReadAudioCodec[4096] = {0};
	memset(chReadAudioCodec, 0, sizeof chReadAudioCodec);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		return false;
	}

	sprintf_s(szInputCmd, 4096, "%s", "21;6;0;0");
	if (! m_pIPhone->FTDCommonCommand(szInputCmd, chReadAudioCodec, 4096))
	{
		m_strMsg = "Fail to get AudioCodecID with FTD";
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chReadAudioCodec;
	m_strMeasured.erase(m_strMeasured.length()-1,m_strMeasured.length()-1);

	if ( m_strMeasured != m_strAudioCodecID)
	{
		m_strMsg = MakeSTDstring("Fail:AudioCodecID is not match, target is %s ", m_strMeasured.c_str());
		TraceLog(m_strMsg);
		return false;
	}
	else
	{
		m_strMsg = MakeSTDstring("Pass:AudioCodecID is %s ",m_strMeasured.c_str());
		TraceLog(m_strMsg);
	}

	return true;
}


bool CCodecTest::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSpeakerTest");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	m_strAudioCodecID =  myParm_node.child("AudioCodecID").text().as_string(ERROR_STRING);
	CHECK_PARM("AudioCodecID", m_strAudioCodecID, ERROR_STRING);

    return true ;
}
