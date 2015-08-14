#include "stdafx.h"
#include "WriteMacInfo.h"
#include <algorithm>

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CWriteMacInfo)

CWriteMacInfo::CWriteMacInfo(void)
{
	m_szImagerVer[0] = '\0';
	m_sz9JPartNo[0] = '\0';
	m_szCmdLine[0] = '\0';
}

CWriteMacInfo::~CWriteMacInfo(void)
{
}

bool CWriteMacInfo::InitData(std::map<std::string,std::string> &paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	
	m_st_UIMessage.i_slot = 0;

	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CReadIMEI");
		return false;
	}

	
	//if(_taccess(_T("E:\\source\\C++\\Luffy_ModifyCurrentSpec_AddCPKLog\\Luffy\\Solution\\TEST_Debug\\DB\\INI\\VMS_DBConfig.ini"),0) != 0)
	//{	return false;
	//}

	WCHAR Buf[1000];
	//GetCurrentDirectory(1000,Buf);

        //int len= WideCharToMultiByte(CP_ACP,0,Buf,wcslen(Buf),NULL,0,NULL,NULL);  
        //char *m_char=new char[len+1];
        //WideCharToMultiByte(CP_ACP,0,Buf,wcslen(Buf),m_char,len,NULL,NULL);  
        //m_char[len]='\0';  


	//std::string a;
 //   //a.assign(m_char);
 //   a.append("\\..\\");

	//CHAR buf[1000] ="E:\\source\\C++\\Luffy_ModifyCurrentSpec_AddCPKLog\\Luffy\\Solution\\TEST_Debug\\";

	//        WCHAR *m_char=new WCHAR[1000+1];

 //       int len= MultiByteToWideChar(CP_ACP,0,buf,strlen(buf)+1,m_char,1001);  

 //       //MultiByteToWideChar(CP_ACP,0,buf,strlen(buf)+1,m_char,len);  
 //       m_char[len]='\0';  

	//SetCurrentDirectory(m_char);

	GetCurrentDirectory(1000,Buf);

	int n = wcslen(Buf);
	char* szBuf = new char[n + 1];
	int LEN = WideCharToMultiByte( CP_ACP, 0, Buf, wcslen(Buf), szBuf, n, NULL, NULL );
	szBuf[n] = '\0';

	sprintf_s(m_st_UIMessage.sz_message,4096,"%s",szBuf);
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	delete [] szBuf;
	//if(_taccess(_T(".\\DB\\INI\\VMS_DBConfig.ini"),0) != 0)
	//{	return false;
	//}

	if(_taccess(PATH1,0) != 0)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"VMS_DBConfig.ini is not exist");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("VMS_DBConfig.ini fail");
		return false;
	}

	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string(ERROR_STRING);
	if(m_szWorkingDir == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"WorkingDir is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	m_szADBWorkingDir  = myParm_node.child("ADBWorkingDir").text().as_string(ERROR_STRING);
	if(m_szADBWorkingDir == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"ADBWorkingDir is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	m_szExeName = myParm_node.child("TESTEXE").text().as_string(ERROR_STRING);
	if(m_szExeName == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"TESTEXE is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	m_szStep = myParm_node.child("STEP").text().as_string(ERROR_STRING);
	if (m_szStep == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"STEP is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	else
	{	transform(m_szStep.begin(),m_szStep.end(),m_szStep.begin(),toupper);
	}

	m_blnNeedPushOtaInfo = myParm_node.child("PUSHOTAINFO").text().as_bool(ERROR_BOOL);
	if(m_blnNeedPushOtaInfo == ERROR_BOOL)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"PUSHOTAINFO is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}

	m_szRemotServerPath = myParm_node.child("SERVERPATH").text().as_string(ERROR_STRING);
	if(ERROR_STRING == m_szRemotServerPath)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"SERVERPATH is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}

	TraceLog("PASS to initdata for CUsbTest");

	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CWriteMacInfo");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	return true;
}

bool CWriteMacInfo::Run(int i_slot)
{
	m_st_UIMessage.i_slot = i_slot;
	//Read ID
	//m_pIPhone->Id.ReadId();
	//if(m_pIPhone->Id.GetId().empty() || m_pIPhone->Id.GetId().length() != 11)
	//{	sprintf_s(m_st_UIMessage.sz_message,4096,"Read ID: %s fail",m_pIPhone->Id.GetId().c_str());
	//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//	return false;
	//}
	//else
	//{	sprintf_s(m_st_UIMessage.sz_message,4096,"ID: %s",m_pIPhone->Id.GetId().c_str());
	//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//}

	if(m_pIPhone->IfRepeatedId())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Id repeated.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Id repeated.");
		//return false;
	}

	if(m_pIPhone->IfRepeatedLabel())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Sn repeated.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Sn repeated.");
		//return false;
	}
	//Load 9J
	m_pIPhone->Runcard.GetRunCardById(m_pIPhone->Id.GetId());
	if(!m_pIPhone->Runcard.GetRuncard().empty())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Runcard: %s",m_pIPhone->Runcard.GetRuncard().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get runcard: %s failed",m_pIPhone->Runcard.GetRuncard().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Get runcard fail");
		return false;
	}

	m_pIPhone->So.GetSoByRuncard(m_pIPhone->Runcard.GetRuncard());
	if( !m_pIPhone->So.GetSo().empty())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"SO: %s",m_pIPhone->So.GetSo().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get SO: %s fail",m_pIPhone->So.GetSo().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Get SO fail");
		return false;
	}

	m_pIPhone->PartNo.SearchPartNo(m_pIPhone->So.GetSo());
	if( !m_pIPhone->PartNo.GetPartNo().empty())
	{	m_szPartNo = m_pIPhone->PartNo.GetPartNo();
		sprintf_s(m_st_UIMessage.sz_message,4096,"PartNo: %s",m_pIPhone->PartNo.GetPartNo().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get partno: %s fail",m_pIPhone->PartNo.GetPartNo().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Get partno fail");
		return false;
	}

	m_pIPhone->WbcFileName.SearchWbcFileName(m_pIPhone->PartNo.GetPartNo());
	if( !m_pIPhone->WbcFileName.GetWbcFileName().empty())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"WbcFileName: %s",m_pIPhone->WbcFileName.GetWbcFileName().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get WbcFileName: %s fail",m_pIPhone->WbcFileName.GetWbcFileName().c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Get WbcFileName fail");
		return false;
	}

	m_pIPhone->SetWorkingDir(m_szADBWorkingDir.c_str());

	if("WRITE" == m_szStep)
	{
		if(m_pIPhone->GetRunInResult())
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Runin OK");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
		else
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"No runin or runin fail.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			m_strMsg = "No runin or runin fail";
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("No runin record");
			return false;
		}

		char szLocalPath[128]={0};
		sprintf_s(szLocalPath, 128, "%s%s.txt", m_pCGProc->g_mapToolInfo["WORKING_DIR"].c_str(), m_pIPhone->Id.GetId().c_str());
		if(!m_pIPhone->pullCPULoadLog(szLocalPath))
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to push file to local.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			m_strMsg = "Fail to push file to local.";
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("Push CPU log fail");
			return false;
		}
		
		char szServerPath[128]={0};
		sprintf_s(szServerPath, 128, "%s\\%s.txt",m_szRemotServerPath.c_str(), m_pIPhone->Id.GetId().c_str());

		bool bRet = false;
		0 != MoveFileExA(szLocalPath, szServerPath, MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING) ? bRet = true : bRet = false;
		DWORD uu = GetLastError();
		if(!bRet)
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to move file to server.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			m_strMsg = "Fail to move file to server.";
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("move CPU log fail");
			return false;
		}
	}
	//for(i = 0; i < m_szPartNo.length(), i++)
	//{	m_sz9JPartNo[i] = m_szPartNo[i];
	//}
	//m_sz9JPartNo[m_szPartNo.length()] = '\0';
	//m_szPartNo = "9J.2EE11.32A";
	sprintf_s(m_sz9JPartNo,31,"%s",m_szPartNo.c_str());

	if(CheckImeiAndPartNo())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"IMEI match PartNo pass.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"IMEI not match PartNo.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		m_strMsg = "IMEI not match PartNo.";
		TraceLog(m_strMsg);
		m_pCGProc->SetFailMsg("IMEI not match");
		return false;
	}
	
	if(m_pIPhone->CheckLabelPartNo())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Check 22code pass.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Acer 22code not match.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		m_strMsg = "Acer 22code not match.";
		TraceLog(m_strMsg);
		m_pCGProc->SetFailMsg("SN not match");
		return false;
	}

	//if(m_pIPhone->pullRunInLog())
	//{	sprintf_s(m_st_UIMessage.sz_message,4096,"Check runin pass");
	//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//}
	//else
	//{	sprintf_s(m_st_UIMessage.sz_message,4096,"runin uncomplete...");
	//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	//	m_strMsg = "runin uncomplete...";
	//	TraceLog(m_strMsg);
	//	return false;
	//}

	//Write Sn
	if(m_szStep == "WRITE")
	{
		if(WriteSn())
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Write SN pass.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
		else
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Write SN fail.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			//log
			m_strMsg = "Write SN fail.";
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("Write SN fail.");
			return false;
		}
	}

	if(ReadSn())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Read SN pass.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Read SN fail.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		//log
		m_strMsg = "Read SN fail.";
		TraceLog(m_strMsg);
		m_pCGProc->SetFailMsg("Read SN fail.");
		return false;
	}

	//Get User Image
	if(m_szStep == "WRITE")
	{
		if(GetSWVersion())
		{	//sprintf_s(m_szImagerVer,31,"v01.003.02.b");
			sprintf_s(m_st_UIMessage.sz_message,4096,"SWVersion: %s",m_szImagerVer);
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			//{	sprintf_s(m_szImagerVer,31,"v01.106.01.b_00");
			//	sprintf_s(m_st_UIMessage.sz_message,4096,"SWVersion: %s",m_szImagerVer);
			//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			//}
			//else
			//{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get SWVersion fail.");
			//	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			//	//log
			//	m_strMsg = "Get SWVersion fail.";
			//	TraceLog(m_strMsg);
			//	return false;
			//}

			//push User Image
			if(m_blnNeedPushOtaInfo)
			{	if(this->CreatedVersionFile())
				{	sprintf_s(m_st_UIMessage.sz_message,4096,"Created otainfo.txt pass.");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
					if(this->PushDownLoadFile())
					{	sprintf_s(m_st_UIMessage.sz_message,4096,"Push otainfo.txt pass.");
						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
					}
					else
					{	sprintf_s(m_st_UIMessage.sz_message,4096,"Push otainfo.txt fail.");
						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

						//log
						m_strMsg = "Push otainfo.txt fail.";
						TraceLog(m_strMsg);
						m_pCGProc->SetFailMsg("Read otainfo fail.");
						return false;
					}
				}
				else
				{	sprintf_s(m_st_UIMessage.sz_message,4096,"Created otainfo.txt fail.");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

					//log
					m_strMsg = "Created otainfo.txt fail.";
					TraceLog(m_strMsg);
					m_pCGProc->SetFailMsg("Created otainfo fail.");
					return false;
				}
			}
		}
		else
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get Image version fail");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			//log
			m_strMsg = "Get Image version fail.";
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("Image version fail.");
			return false;
		}
	}

	//Get Ethernet mac Address
	if(GetMacAddr())
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"ETHMAC: %s",m_szMACAddr.c_str());
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		if(m_szStep == "WRITE")
		{
		}
		else
		{	if(m_szHWver != m_pIPhone->Id.GetId())
			{	sprintf_s(m_st_UIMessage.sz_message,4096,"ID is not the same one.");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

				//log
				m_strMsg = "ID is not the same one.";
				TraceLog(m_strMsg);
				m_pCGProc->SetFailMsg("ID not same");
				return false;
			}
		}
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Get ETHMAC fail");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		//log
		m_strMsg = "Get ETHMAC fail.";
		TraceLog(m_strMsg);
		m_pCGProc->SetFailMsg("Get ETHMAC fail");
		return false;
	}

	//Get WIFIMAC
	transform(m_szPartNo.begin(),m_szPartNo.end(),m_szPartNo.begin(),toupper);
	if(m_szStep == "WRITE" && (m_szPartNo == "9J.2EE11.32A" || m_szPartNo == "9J.2EE12.33E"))
	{	if(ReadWifiAdd())
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"WIFIMAC: %s",m_szWifiAdd.c_str());
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			m_strMsg = "Get WIFIMAC,message = ";
			m_strMsg.append(m_szWifiAdd);
			TraceLog(m_strMsg);
			//去掉m_szWifiAdd中的：
			//m_szWifiAdd = "01:23:45:67:89:AB";
			int qwe = m_szWifiAdd.length();
			if(m_szWifiAdd.length() == 19)
			{
				m_szWifiAdd.erase(2,1);
				m_szWifiAdd.erase(4,1);
				m_szWifiAdd.erase(6,1);
				m_szWifiAdd.erase(8,1);
				m_szWifiAdd.erase(10,1);
			}
			else
			{	m_szWifiAdd = "";
			}
		}
		else
		{	printf_s(m_st_UIMessage.sz_message,4096,"Get WIFIMAC fail,message = %s",m_szWifiAdd.c_str());
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			//log
			m_strMsg = "Get WIFIMAC fail,message = ";
			m_strMsg.append(m_szWifiAdd);
			TraceLog(m_strMsg);
			m_pCGProc->SetFailMsg("Get WIFIMAC fail");
			return false;
		}
	}

	if(MainFunction())
	{	//Update Status = "I"
		if(m_szStep == "WRITE")
		{	if(UpdateIMEIStatus_I())
			{	sprintf_s(m_st_UIMessage.sz_message,4096,"Upload IMEI status PASS");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
				if(UploadYrstation())
				{	sprintf_s(m_st_UIMessage.sz_message,4096,"Upload yrstation PASS");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
				}
				else
				{	sprintf_s(m_st_UIMessage.sz_message,4096,"Upload yrstation fail");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
					m_pCGProc->SetFailMsg("Upload yrstation fail");
					return false;
				}
			}
			else
			{	sprintf_s(m_st_UIMessage.sz_message,4096,"Upload IMEI status fail");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
				return false;
			}
		}
	}
	else
	{	return false;
	}

	return true;
}

bool CWriteMacInfo::MainFunction(void)
{
	sprintf_s(m_st_UIMessage.sz_message,4096,"WriteMac is running......");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	//Set working dirszWorkingDir
	char szWorkingDir[512]={0};
	sprintf_s(szWorkingDir,512,m_szWorkingDir.c_str());
	char szTestExe[100] = "";
	sprintf_s(szTestExe,100,m_szExeName.c_str());

	sprintf_s(m_szCmdLine, 512, " -r ETHMAC");
/*
	//Set buffer to get result of CMD
	std::string Buf = "";
	ExecCmdUtil::ExecuteCmdBlocking(szTestExe,m_szCmdLine,szWorkingDir, &Buf);

	if(!Buf.empty())
	{	//find "ETHMAC="
		std::string::size_type i = 0;
		std::string::size_type j = Buf.find("ETHMAC=",i);
		std::string szEthnetMac = Buf.substr(7,17);
		if( szEthnetMac.compare(m_szMACAddr) == 0 )
		{	//一致则不烧
			return true;
		}
		else
		{	//不一致则烧
			sprintf_s(m_szCmdLine, 512, " -w ETHMAC=%s", m_szMACAddr.c_str());
			ExecCmdUtil::ExecuteCmdBlocking(szTestExe,m_szCmdLine,szWorkingDir, &Buf);

			if(Buf.empty())
			{	i = 0;
				j = Buf.find("Write successfully",i);
				if(j != string::npos)
				{	//找到"Write successfully",则认为烧录成功
					return true;
				}
				else
				{	//若找不到"Write successfully",则认为失败
					return false;
				}
			}
			else
			{	return false;
			}

		}
	}
	else
	{	return false;
	}
*/
	//char **buf = NULL;
	char *BUF = (char*) malloc( 512 * sizeof(char));
	//BUF = (char*) malloc( 512 * sizeof(char));
	memset(BUF,'\0',512);
	//buf = &BUF;
	//int asc = strlen(*buf);
	ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szTestExe,m_szCmdLine,szWorkingDir,&BUF,4000);

	int asc = strlen(BUF);

	sprintf_s(m_st_UIMessage.sz_message,4096,"Read ETHMAC message = %s",BUF);
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	
	//Log
	m_strMsg = "Read ETHMAC message = ";
	m_strMsg.append(BUF);
	TraceLog(m_strMsg);

	if(strstr(BUF,"ETHMAC=") != 0)
	{	if(strstr(BUF,m_szMACAddr.c_str()) != 0)
		{	//一致则不烧
			sprintf_s(m_st_UIMessage.sz_message,4096,"ETHMAC is the same one.");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

			//Log
			m_strMsg = "ETHMAC is the same one.";
			TraceLog(m_strMsg);

			free(BUF);
			BUF = NULL;
			return true;
		}
		else
		{	//不一致则烧
			if(m_szStep == "WRITE")
			{	sprintf_s(m_szCmdLine, 512, " -w ETHMAC=%s", m_szMACAddr.c_str());
				ExecCmdUtil::ExecuteCmdBlockingWithTimeout(szTestExe,m_szCmdLine,szWorkingDir,&BUF,4000);

				sprintf_s(m_st_UIMessage.sz_message,4096,"Read ETHMAC message = %s",BUF);
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

				//Log
				m_strMsg = "Read ETHMAC message = ";
				m_strMsg.append(BUF);
				TraceLog(m_strMsg);

				if(strstr(BUF,"Write successfully") != 0)
				{	//找到"Write successfully",则认为烧录成功
					sprintf_s(m_st_UIMessage.sz_message,4096,"Write ETHMAC PASS.");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

					//Log
					m_strMsg = "Write ETHMAC PASS.";
					TraceLog(m_strMsg);

					free(BUF);
					BUF = NULL;
					return true;
				}
				else   // == 0表示找不到
				{	//若找不到"Write successfully",则认为失败
					sprintf_s(m_st_UIMessage.sz_message,4096,"Write ETHMAC FAIL.");
					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

					//Log
					m_strMsg = "Write ETHMAC fail.";
					TraceLog(m_strMsg);

					free(BUF);
					BUF = NULL;
					return false;
				}
			}
			else
			{	sprintf_s(m_st_UIMessage.sz_message,4096,"ETHMAC is error.");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

				//Log
				m_strMsg = "ETHMAC is error.";
				TraceLog(m_strMsg);

				free(BUF);
				BUF = NULL;
				return false;
			}
		}
	}
	else // == 0表示找不到
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Error when read ETHMAC.");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		free(BUF);
		BUF = NULL;
		return false;
	}

}

bool CWriteMacInfo::GetSWVersion(void)
{	
	bool bReturn = false;

	char szModel[8] = "GLUFB1A";
	//char szModel[8] = "GNAMB1A";
	char szSWVersion[30] = "";
	char szErrCode[128] = "";
	char szErrMsg[512] = "";
	char szPartNoInfo[128] = "";

	CString str_dllVMS_DBACCESS = VMS_DBACCESS;
	HMODULE hDll = ::LoadLibrary(str_dllVMS_DBACCESS);
	if(hDll == NULL)
	{	goto Exit;
	}

	//WCHAR Buf[1000];
	//GetCurrentDirectory(1000,Buf);

	typedef bool (*fpVMS_ConnectDB)(void);
	typedef bool (*fpVMS_CloseDB)(void);
	fpVMS_CloseDB iVMS_CloseDB = (fpVMS_CloseDB)::GetProcAddress (hDll,"VMS_CloseDB");
	if(iVMS_CloseDB == NULL)
	{	//没有VMS_ConnectDB
		goto Exit_FreeLibrary;
	}
	//char PartNo9J[31] = "9J.2EE12.33E";

	fpVMS_ConnectDB iVMS_ConnectDB = (fpVMS_ConnectDB)::GetProcAddress(hDll,"VMS_ConnectDB");
	if (iVMS_ConnectDB != NULL)
	{	if(iVMS_ConnectDB())
		{	typedef bool (*fpVMS_QuerySQNinfo_S3)(char *szModel, char *szPartNo, char *szSWVersion, char *st_PartnoInfo, char *szErrCode, char *szErrMsg);
			fpVMS_QuerySQNinfo_S3 iVMS_QuerySQNinfo_S3 = (fpVMS_QuerySQNinfo_S3)::GetProcAddress(hDll,"VMS_QuerySQNinfo_S3");
			if (iVMS_QuerySQNinfo_S3 != NULL )
			{	bool bRef = iVMS_QuerySQNinfo_S3(szModel, m_sz9JPartNo, m_szImagerVer, szPartNoInfo, szErrCode, szErrMsg);
				if(bRef)
				{	m_szSWver = m_szImagerVer;
					bReturn = true;
				}
				else
				{	//查询失败
					goto Exit_CloseDB;
				}
			}
			else
			{	//没有VMS_QuerySQNinfo
				goto Exit_CloseDB;
			}
		}
		else
		{	//链接VMS数据失败
			goto Exit_FreeLibrary;
		}
	}
	else
	{	//没有VMS_ConnectDB
		goto Exit_FreeLibrary;
	}

Exit_CloseDB:
	iVMS_CloseDB();
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CWriteMacInfo::GetMacAddr(void)
{
	bool bReturn = false;
	char szStationState[4] = "B";  //"P"
	char szPartNo[31] = "";
	char szIMEI[31] = "";    //QCS340000011
	char szMSN[31] = "";
	char szBTAddr[31] = "";
	char szMACAddr[31] = "";
	char szSWver[31] = "";
	char szHWver[31] = "";
	char szPVK[31] = "";
	char szErrCode[31] = "";
	char szErrMsg[1024] = "";
	char szStatusOutput[31] = "";

	CString str_dllPICS_S3_FACTORY = PICS_S3_FACTORY;
	HMODULE hDll = ::LoadLibrary(str_dllPICS_S3_FACTORY);
	if(hDll != NULL)
	{	typedef bool (*fpSSS_QueryIMEI_SSS02_V2)(char *szStationState, 
	                                             char *pIMEI, 
												 char *szMSN, 
												 char *szBTAddr, 
												 char *szMACAddr, 
												 char *szSWver, 
												 char *szHWver, 
												 char *szPVK, 
												 char *szErrCode, 
												 char *szErrMsg, 
												 char *szStatusOutput);
		fpSSS_QueryIMEI_SSS02_V2 iSSS_QueryIMEI_SSS02_V2 = (fpSSS_QueryIMEI_SSS02_V2)::GetProcAddress(hDll,"QueryIMEI_SSSO2_V2");
		if (iSSS_QueryIMEI_SSS02_V2 != NULL)
		{	//sprintf_s(szStationState,4 "%s","B");
			if(m_szStep == "WRITE")
			{	sprintf_s(szStationState,4 ,"B");
			}
			else
			{	sprintf_s(szStationState,4 ,"P");
			}
			sprintf_s(szIMEI,31, "%s", m_pIPhone->m_szIMEI.c_str());
			if(iSSS_QueryIMEI_SSS02_V2(szStationState,szIMEI,szMSN,szBTAddr,szMACAddr,szSWver,szHWver,szPVK,szErrCode,szErrMsg,szStatusOutput))
			{	m_szMSN = szMSN;
				m_szBTAddr = szBTAddr;
				m_szMACAddr = szMACAddr;
				m_szMACAddr.insert(2,1,':');
				m_szMACAddr.insert(5,1,':');
				m_szMACAddr.insert(8,1,':');
				m_szMACAddr.insert(11,1,':');
				m_szMACAddr.insert(14,1,':');
				//m_szSWver = m_szImagerVer;
				m_szHWver = szHWver;
				m_szPVK = szPVK;
				m_szErrCode = szErrCode;
				m_szErrMsg = szErrMsg;
				m_szStatusOutput = szStatusOutput;
				bReturn = true;
			}
			else
			{	goto Exit_FreeLibrary;
			}
		}
		else
		{	goto Exit_FreeLibrary;
		}
	} 
	else
	{	goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CWriteMacInfo::CheckImeiAndPartNo(void)
{
	bool bReturn = false;

	CString str_dllPICS_S3_FACTORY = PICS_S3_FACTORY;
	HMODULE hDll = ::LoadLibrary(str_dllPICS_S3_FACTORY);
	if(hDll != NULL)
	{	typedef bool (*fpCheck_MAL_PN_SSSO2)(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg);
		fpCheck_MAL_PN_SSSO2 iCheck_MAL_PN_SSSO2 = (fpCheck_MAL_PN_SSSO2)::GetProcAddress(hDll,"Check_MAL_PN_SSSO2");
		if(iCheck_MAL_PN_SSSO2 != NULL)
		{	char szImei[16] = "";
			char szErrCode[31] = "";
			char szErrMsg[1024] = "";
			char szMal[10] = "";
			sprintf_s(szImei,16,"%s",m_pIPhone->m_szIMEI.c_str());
			if(iCheck_MAL_PN_SSSO2(szImei,m_sz9JPartNo,szMal,szErrCode,szErrMsg))
			{	bReturn = true;
			}
			else
			{	goto Exit_FreeLibrary;
			}
		}
		else
		{	goto Exit_FreeLibrary;
		}
	}
	else
	{	goto Exit;
	}

Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CWriteMacInfo::UpdateIMEIStatus_I(void)
{
	bool bReturn = false;
	char szIMEI[31] = "";  //QCS340000011
	char szId[15] = "";
	char szErrCode[31] = "";
	char szErrMsg[1024] = "";

	CString str_dllPICS_S3_FACTORY = PICS_S3_FACTORY;
	HMODULE hDll = ::LoadLibrary(str_dllPICS_S3_FACTORY);
	if(hDll != NULL)
	{	typedef bool (*fpUpdateIMEIStatus_I_V2)(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);
		fpUpdateIMEIStatus_I_V2 iUpdateIMEIStatus_I = (fpUpdateIMEIStatus_I_V2)::GetProcAddress(hDll,"UpdateIMEIStatus_I_V2");
		if(iUpdateIMEIStatus_I != NULL)
		{	sprintf_s(szIMEI,31,"%s",m_pIPhone->m_szIMEI.c_str());
			sprintf_s(szId,15,"%s",m_pIPhone->Id.GetId().c_str());
			char szMal[16] = "";
			sprintf_s(szMal,16,"%s", m_szWifiAdd.c_str());
			if(iUpdateIMEIStatus_I(szIMEI,m_szImagerVer,szId,szMal,szErrCode,szErrMsg))
			{	bReturn = true;
			}
			else
			{	goto Exit_FreeLibrary;
			}
		}
		else
		{	goto Exit_FreeLibrary;
		}
	}
	else
	{	goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CWriteMacInfo::UploadYrstation(void)
{
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;
	HMODULE hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef bool (_stdcall *lpInsertYrstation)(const unsigned char* Model,     unsigned short ModelLen,
				   		        				   const unsigned char* Id,        unsigned short IdLen,
												   const unsigned char* Shift,     unsigned short ShiftLen,
												   const unsigned char* Station,   unsigned short StationLen,
												   const unsigned char* Trid,      unsigned short TridLen,
												   const unsigned char* Operator,  unsigned short OperatorLen,
												   unsigned short CycleTime,
												   const unsigned char* CheckInfo, unsigned short CheckInfoLen);
		lpInsertYrstation iInsertYrstation = (lpInsertYrstation)::GetProcAddress(hDll,"InsertYrstation");
		if(iInsertYrstation != NULL)
		{	unsigned char Model[20] = "";
			unsigned char Id[15] = "";
			unsigned char Shift[4] = "R11";
			unsigned char Station[20] = "IMEI";
			unsigned char Trid[23] = "";
			unsigned char Operator[10] = "";
			unsigned char CheckInfo[10] = "";
			sprintf_s((char*)Trid, 23,"%s",m_pIPhone->m_szSN.c_str());
			sprintf_s((char*)Model,20,"L%s",m_pIPhone->WbcFileName.GetWbcFileName().c_str());
			sprintf_s((char*)Id,15,"%s",m_pIPhone->Id.GetId().c_str());
			if(iInsertYrstation(Model,    (unsigned short)strlen((char*)Model),
				                Id,       (unsigned short)strlen((char*)Id),
								Shift,    (unsigned short)strlen((char*)Shift),
								Station,  (unsigned short)strlen((char*)Station),
								Trid,     (unsigned short)strlen((char*)Trid),
								Operator, (unsigned short)strlen((char*)Operator),
								0,
								CheckInfo,(unsigned short)strlen((char*)CheckInfo)))
			{	bReturn = true;
			}
			else
			{	goto Exit_FreeLibrary;
			}
		}
		else
		{	goto Exit_FreeLibrary;
		}
	}
	else
	{ goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CWriteMacInfo::ReadWifiAdd(void)
{
	bool bReturn = false;
	m_pIPhone->SetWifiEnabled(true);
	Sleep(1000);
	bReturn = m_pIPhone->ReadWifiAddress(m_szExeName,m_szWorkingDir,&m_szWifiAdd);
	Sleep(500);
	m_pIPhone->SetWifiEnabled(false);
	Sleep(500);
	return bReturn;
}

bool CWriteMacInfo::WriteSn(void)
{
	return m_pIPhone->WriteSn(m_szExeName,m_szWorkingDir);
}

bool CWriteMacInfo::ReadSn(void)
{
	std::string szSn ="";
	return m_pIPhone->ReadSn(m_szExeName, m_szWorkingDir, &szSn);
}

bool CWriteMacInfo::CreatedVersionFile(void)
{
	CFile myFile;
	CFileException e;
	char szFilePath[1000];
	szFilePath[0] = '0';
	sprintf_s(szFilePath,512,"%sotainfo.txt",m_pCGProc->g_mapToolInfo["WORKING_DIR"].c_str());

	WCHAR m_char[1001];
	m_char[0]='0';
    int len= MultiByteToWideChar(CP_ACP,0,szFilePath,strlen(szFilePath)+1,m_char,1001);  
    MultiByteToWideChar(CP_ACP,0,szFilePath,strlen(szFilePath)+1,m_char,len);  
    m_char[len]='\0';  

	if (myFile.Open(m_char,CFile::modeReadWrite | CFile::modeCreate, &e))
	{	BYTE buf[0x1000];
		sprintf_s((char *)buf,0x1000,"SWEP_GLUFB1A_Luffy_%s.zip",m_szSWver.c_str());
		myFile.SeekToBegin();
		myFile.Write(buf,strlen((const char*)buf));
		myFile.Close();
		//DeleteFile(m_char);
		return true;
	}
	else
	{	e.ReportError();
		return false;
	}
}

bool CWriteMacInfo::PushDownLoadFile()
{	return m_pIPhone->PushDownFileToSDCard(m_pCGProc->g_mapToolInfo["WORKING_DIR"]);
}
