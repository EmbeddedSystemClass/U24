#include "StdAfx.h"
#include "BBTest.h"
//#include "d:\source\C++\MyLuffy\Luffy\Modules\UnifyInterface\FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CBBTest)
bool CBBTest::Run(int i_slot)
{

	//CString path =m_szWorkingDir + _T("command.exe");
	////检查XML档中的配置路径是否存在测试tools
	//if (_taccess(path, 0) == -1) {
	//	//strcpy(ErrorCode, "ERROR: No adb.exe exist!");
	//	TraceLog("FAIL to BBTest :" + path + _T(" not exist"));
	//	return false;
	//}
	return MainFunction();
}

bool CBBTest::MainFunction(void)
{
	//Set buffer to get result of CMD
	
	//count
	int i = 0;
	
	std::string szReturn = "";
	do
	{
		i++;
		if(i > m_iRetryTime)
		{	break;
		}
		//PS Off
		m_pCGProc->SendMsgToUi("Luffy重启中");
		m_pCGProc->SendMsgToUi("Power supply off.");
		TraceLog("Power supply off.");
		m_pCGProc->m_pIPS->SetPowerSupplyOff();

		//waiting 1s
		Sleep(1000);

		//PS ON
		m_pCGProc->SendMsgToUi("Power supply on.");
		TraceLog("Power supply on.");
		m_pCGProc->m_pIPS->SetPowerSupplyOn();

		//Waiting 5s
		Sleep(5000);

		//detect ADFU devices
		m_pCGProc->SendMsgToUi("自动侦测ADFU设备中...");
		TraceLog("Auto detect ADFU device.");
		 if(false == m_pCGProc->AutoDetectDevice())
		{	m_pCGProc->SendMsgToUi("侦测不到ADFU设备,请检查机台是否有系统或接线正确");
			TraceLog("Detect device fail.");
			continue;
		 }

		//sprintf_s(m_st_UIMessage.sz_message,4096,"BBTest is running......");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		m_pCGProc->SendMsgToUi("BBTest正在测试中，请稍等");
		TraceLog("BBTest is running......");

		szReturn = m_pIPhone->BbTest(m_enabledPMIC,m_enabledNAND,m_enabledDDRSize,m_enabledHDMI,m_enabledUart,m_enabledEthernet,
			   m_enabledSDWriteProtected,m_enabledSDPlugin,m_enabledSDWriteRead,m_enabledSDeMMCSize,m_enabledSDClockRate,false);


		//if (!Buf.empty())
		//{	
		//	TraceLog(Buf);
		//	std::string::size_type i = 0;

		//	//找到第一个"[",并将其删除掉
		//	std::string::size_type j = Buf.find("[",i);
		//	if ( j != 0 )
		//	{
		//		TraceLog("FAIL to BBTest");
		//		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to BBTest");
		//		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//		continue;
		//	}

		//	Buf.erase(j,j + 1);

		//	std::string Temp = "";

		//	if (m_enabledPMIC)
		//	{
		//		//找到m_szPMIC
		//		i = Buf.find(m_szPMIC);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szPMIC之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to PMIC of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to PMIC of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to PMIC of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to PMIC of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in PMIC of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in PMIC of BBTest,no key word:PMIC");
		//			continue;
		//		}
		//	}

		//	if (m_enabledNAND)
		//	{
		//		//找到m_szNAND
		//		i = Buf.find(m_szNAND);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szNAND之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to Nand of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Nand of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					i = Temp.find(m_szNandSize);
		//					if( i == string::npos )
		//					{
		//						TraceLog("FAIL to NandSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to NandSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//						continue;
		//					}
		//					else
		//					{
		//						TraceLog("PASS to NandSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to NandSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in Nand of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in Nand of BBTest,no key word:Nand");
		//			continue;
		//		}
		//	}

		//	if (m_enabledDDRSize)
		//	{
		//		//找到m_szNAND
		//		i = Buf.find(m_szDDR);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szNAND之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to DDR of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to DDR of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					i = Temp.find(m_szDDRSize);
		//					if(i == string::npos )
		//					{
		//						TraceLog("FAIL to DDRSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to DDRSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//						continue;
		//					}
		//					else
		//					{
		//						TraceLog("PASS to DDRSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to DDRSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in DDRSize of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in DDRSize of BBTest,no key word:DDRSize");
		//			continue;
		//		}
		//	}

		//	if (m_enabledHDMI)
		//	{
		//		//找到m_szHDMI
		//		i = Buf.find(m_szHDMI);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szHDMI之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to HDMI of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to HDMI of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to HDMI of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to HDMI of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in HDMI of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in HDMI of BBTest,no key word:HDMI");
		//			continue;
		//		}
		//	}

		//	if (m_enabledUart)
		//	{
		//		//找到m_szUart
		//		i = Buf.find(m_szUart);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szUart之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to Uart of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Uart of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to Uart of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Uart of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in Uart of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in Uart of BBTest,no key word:Uart");
		//			continue;
		//		}
		//	}

		//	if (m_enabledEthernet)
		//	{
		//		//找到m_szEthernet
		//		i = Buf.find(m_szEthernet);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szEthernet之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to Ethernet of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Ethernet of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to Ethernet of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Ethernet of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in Ethernet of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in Ethernet of BBTest,no key word:Ethernet");
		//			continue;
		//		}
		//	}

		//	if(m_enabledSDWriteProtected)
		//	{
		//		//找到m_szSDWriteProtected
		//		i = Buf.find(m_szSDWriteProtected);
		//		if (i!=string::npos )
		//		{
		//			//找到m_szSDWriteProtected之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to SDWriteProtected of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SDWriteProtected of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					i = Temp.find(m_szSDProtected);
		//					if(i == string::npos )
		//					{
		//						TraceLog("FAIL to SD Write Protected of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SD Write Protected of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//						continue;
		//					}
		//					else
		//					{
		//						TraceLog("PASS to SD Write Protected of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to SD Write Protected of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in SDWriteProtected of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in SDWriteProtected of BBTest,no key word:SDWriteProtected");
		//			continue;
		//		}
		//	}

		//	if (m_enabledSDPlugin)
		//	{
		//		//找到m_szSDPlugin
		//		i = Buf.find(m_szSDPlugin);
		//		if ( i!=string::npos )
		//		{
		//			//找到m_szSDPlugin之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to SDPlugin of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SDPlugin of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					i = Temp.find(m_szSDType);
		//					if(i == string::npos )
		//					{
		//						TraceLog("FAIL to SD type of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SD type of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//						continue;
		//					}
		//					else
		//					{
		//						TraceLog("PASS to SD type of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to SD type of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in SDPlugin of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in SDPlugin of BBTest,no key word:SDPlugin");
		//			continue;
		//		}
		//	}

		//	if(m_enabledSDWriteRead)
		//	{
		//		//找到m_szSDWriteRead
		//		i = Buf.find(m_szSDWriteRead);
		//		if ( i!=string::npos )
		//		{
		//			//找到m_szSDWriteRead之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to SDWriteRead of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SDWriteRead of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to SDWriteRead of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to SDWriteRead of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in SDWriteRead of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in SDWriteRead of BBTest,no key word:SDWriteRead");
		//			continue;
		//		}
		//	}

		//	if(m_enabledSDeMMCSize)
		//	{
		//		//找到m_szSDeMMC
		//		i = Buf.find(m_szSDeMMC);
		//		if ( i!=string::npos )
		//		{
		//			//找到m_szSDeMMC之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to SDeMMCSize of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SDeMMCSize of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					i = Temp.find(m_szSDeMMCSize);
		//					if(i == string::npos )
		//					{
		//						TraceLog("Fail to eMMCSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to eMMCSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//						continue;
		//					}
		//					else
		//					{
		//						TraceLog("PASS to eMMCSize of BBTest");
		//						sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to eMMCSize of BBTest");
		//						m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in eMMCSize of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in eMMCSize of BBTest,no key word:eMMCSize");
		//			continue;
		//		}
		//	}

		//	if(m_enabledSDClockRate)
		//	{
		//		//找到m_szSDClockRate
		//		i = Buf.find(m_szSDClockRate);
		//		if ( i!=string::npos )
		//		{
		//			//找到m_szSDClockRate之后的最近的一个"["
		//			j = Buf.find("[",i);
		//			if(j == string::npos )
		//				j=Buf.length();
		//			//提取区间的字符串
		//			if (j >= i)
		//			{
		//				Temp = Buf.substr(i,j - i + 1);    //取出i至j之间的字符串
		//				Buf.erase(i,j);    //删除掉已取出的字符串
		//				i = Temp.find("successful");
		//				if(i == string::npos)    //找不到"successful"，认为测试失败
		//				{
		//					TraceLog("FAIL to SDClockRate of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to SDClockRate of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//					continue;
		//				}
		//				else
		//				{
		//					TraceLog("PASS to SDClockRate of BBTest");
		//					sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to SDClockRate of BBTest");
		//					m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		//				}
		//			}
		//			else
		//			{
		//				TraceLog("ERROR in SDClockRate of BBTest");
		//				continue;
		//			}
		//		}
		//		else
		//		{
		//			TraceLog("ERROR in SDClockRate of BBTest,no key word:SDClockRate");
		//			continue;
		//		}
		//	}
		//}
		//else
		//{
		//	TraceLog("FAIL to BBTest ; ExecuteCMDBlocking ERROR");
		//	continue;
		//}

	}
	while("PASS" != szReturn);

	if("PASS" == szReturn)
	{	TraceLog("Pass to all BBTest");
		//sprintf_s(m_st_UIMessage.sz_message,4096,"Pass to all BBTest");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi("Pass to all BBTest");
		return true;
	}
	else
	{	m_pCGProc->SetFailMsg(szReturn.c_str());
		//TraceLog("Fail to all BBTest");
		TraceLog(szReturn.c_str());
		//sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to all BBTest");
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SendMsgToUi(szReturn.c_str());
		return false;
	}
}

CBBTest::CBBTest(void)
{
}

CBBTest::~CBBTest(void)
{
}

bool CBBTest::InitData(std::map<std::string,std::string>& paramMap)
{

//	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//std::string szString;
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CDlFactoryOs");
		return false;
	}
	
	m_pCGProc->SetEraseFlag(false);
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	//m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();
	//m_szExeName = myParm_node.child("TESTEXE").text().as_string();

	m_iRetryTime = myParm_node.child("RetryTime").text().as_int();
	m_enabledPMIC = myParm_node.child("PMU").child("Enabled").text().as_bool();
	//if (m_enabledPMIC)
	//	m_szPMIC = myParm_node.child("PMU").child("ITEMCMD").text().as_string();
	//else
	//	m_szPMIC ="";

	m_enabledNAND = myParm_node.child("Nand").child("Enabled").text().as_bool();
	//if (m_enabledNAND)
	//{
	//	m_szNAND = myParm_node.child("Nand").child("ITEMCMD").text().as_string();
	//	m_szNandSize = myParm_node.child("Nand").child("NandSize").text().as_string();
	//	//m_dNandSize = atof(szString.c_str());
	//}
	//else
	//{
	//	m_szNAND = "";
	//	m_dNandSize = 0;
	//	m_szNandSize = "";
	//}

	m_enabledDDRSize = myParm_node.child("DDR").child("Enabled").text().as_bool();
	//if(m_enabledDDRSize)
	//{
	//	m_szDDR = myParm_node.child("DDR").child("ITEMCMD").text().as_string();
	//	m_szDDRSize = myParm_node.child("DDR").child("DDRSize").text().as_string();
	//	//m_dDDRSize = atof(szString.c_str());
	//}
	//else
	//{
	//	m_szDDR = "";
	//	m_dDDRSize = 0;
	//	m_szDDRSize = "";
	//}

	m_enabledHDMI = myParm_node.child("HDMI").child("Enabled").text().as_bool();
	//if(m_enabledHDMI)
	//	m_szHDMI = myParm_node.child("HDMI").child("ITEMCMD").text().as_string();
	//else
	//	m_szHDMI = "";

	m_enabledUart = myParm_node.child("Uart").child("Enabled").text().as_bool();
	//if(m_enabledUart)
	//	m_szUart = myParm_node.child("Uart").child("ITEMCMD").text().as_string();
	//else
	//	m_szUart = "";

	m_enabledEthernet = myParm_node.child("Ethernet").child("Enabled").text().as_bool();
	//if(m_enabledEthernet)
	//	m_szEthernet = myParm_node.child("Ethernet").child("ITEMCMD").text().as_string();
	//else
	//	m_szEthernet = "";

	//SD write ptotected
	m_enabledSDWriteProtected = myParm_node.child("SDWriteProtected").child("Enabled").text().as_bool();
	//if(m_enabledSDWriteProtected)
	//{
	//	m_szSDWriteProtected = myParm_node.child("SDWriteProtected").child("ITEMCMD").text().as_string();
	//	m_szSDProtected = myParm_node.child("SDWriteProtected").child("Protected").text().as_string();
	//}
	//else
	//{
	//	m_szSDWriteProtected = "";
	//	m_szSDProtected="";
	//}

	//SD plugin
	m_enabledSDPlugin = myParm_node.child("SDPlugin").child("Enabled").text().as_bool();
	//if(m_enabledSDPlugin)
	//{
	//	m_szSDPlugin = myParm_node.child("SDPlugin").child("ITEMCMD").text().as_string();
	//	m_szSDType = myParm_node.child("SDPlugin").child("SDType").text().as_string();
	//}
	//else
	//{
	//	m_szSDPlugin = "";
	//	m_szSDType="";
	//}

	//SD write and read
	m_enabledSDWriteRead = myParm_node.child("SDWriteRead").child("Enabled").text().as_bool();
	//if(m_enabledSDWriteRead)
	//	m_szSDWriteRead = myParm_node.child("SDWriteRead").child("ITEMCMD").text().as_string();
	//else
	//	m_szSDWriteRead = "";

	//SD eMMC size
	m_enabledSDeMMCSize = myParm_node.child("SDeMMCSize").child("Enabled").text().as_bool();
	//if(m_enabledSDeMMCSize)
	//{
	//	m_szSDeMMC = myParm_node.child("SDeMMCSize").child("ITEMCMD").text().as_string();
	//	m_szSDeMMCSize = myParm_node.child("SDeMMCSize").child("eMMCSize").text().as_string();
	//}
	//else
	//{
	//	m_szSDeMMC = "";
	//	m_szSDeMMCSize="";
	//}

	//SD clock rate
	m_enabledSDClockRate = myParm_node.child("SDClockRate").child("Enabled").text().as_bool();
	//if(m_enabledSDClockRate)
	//	m_szSDClockRate = myParm_node.child("SDClockRate").child("ITEMCMD").text().as_string();
	//else
	//	m_szSDClockRate = "";

	//sprintf_s(m_szCmdLine,"%s %s %s %s %s %s %s %s %s %s %s %s",m_szExeName.c_str(),m_szPMIC.c_str(),m_szNAND.c_str(),m_szDDR.c_str(),m_szHDMI.c_str(),m_szUart.c_str(),m_szEthernet.c_str(),m_szSDWriteProtected.c_str(),     \
	//	                                                        m_szSDPlugin.c_str(),m_szSDWriteRead.c_str(),m_szSDeMMC.c_str(),m_szSDClockRate.c_str());

	TraceLog("PASS to initdata for CBBTest");
	//sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CBBTest");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	m_pCGProc->SendMsgToUi("PASS to initdata for CBBTest");
	//TraceLog(m_szCmdLine);
	return true;
}
