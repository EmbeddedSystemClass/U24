// RequestManager.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "RequestManager.h"
#include "Request\QualcommRequest.h"
#include <cassert>
#include "..\Common\CommonUtil\cloud.h"

namespace PLATFORM_TOOL
{
	CRequestManager::CRequestManager(void)
	{}

	CRequestManager::~CRequestManager(void)
	{
	}
	
	void CRequestManager::AddRef(const TSTRING& strDevice)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			::InterlockedIncrement(&Iter->second->m_lRefCount);
		}
	}

	void CRequestManager::ReleaseRef(const TSTRING& strDevice)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			::InterlockedDecrement(&Iter->second->m_lRefCount);
			if(Iter->second->m_lRefCount == 0)
				m_mapDevice.erase(Iter);
		}
	}

	void CRequestManager::AddLog(const TSTRING& strDevice,const TSTRING& strMsg)
	{		
		//e.g. device bane samples
		//\\?\USB#Vid_413c&Pid_b007#5&1b8ed9a6&0&8#{a5dcbf10-6530-11d2-901f-00c04fb951ed}		
		//\\.\COM44		
		TSTRING strLogName;
		if(strDevice.size() > 5)
		{
			strLogName = _T("") + strDevice.substr(4) + _T("_LASTERROR");
			CCloud::Put(strLogName,static_cast<int>(strMsg.size()*sizeof(TCHAR)),(void*)(strMsg.c_str()));
		}
	}	

	bool CRequestManager::Open(const TSTRING& strDevice)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{			
			SimapleLock lock(Iter->second->csCOM);

			bRes = Iter->second->bIsConnected;
			if(bRes == false)
			{
				bRes = RUN_SUCCESSFULLY == Iter->second->ptCOMLibrary->OpenComport(_T(""),strDevice.c_str(),true);
				if(bRes == false)
					AddLog(strDevice,_T("CRequestManager::Open::Opencomport Fail"));
				//assert(bRes);//Should it need to wait for device ?
				Iter->second->bIsConnected = bRes;
			}
		}
		return bRes;
	}

	bool CRequestManager::Close(const TSTRING& strDevice,bool bKeepPotocolType)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			
			bRes = Iter->second->bIsConnected == false;
			if(bRes == false)
			{
				Iter->second->ptCOMLibrary->CloseComport();
				if(bKeepPotocolType == false)
					Iter->second->nPotocolType = NONE;
				bRes = true;
				Iter->second->bIsConnected = false;
			}
		}
		return bRes;
	}

	bool CRequestManager::IsOpen(const TSTRING& strDevice)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);

			bRes = Iter->second->bIsConnected;
		}
		return bRes;
	}

	void CRequestManager::Reset(const TSTRING& strDevice)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			//Iter->second->nPotocolType = NONE;
			Iter->second->ptCOMLibrary->ResetRequestMode();
		}
	}
	void CRequestManager::ResetMode(const TSTRING& strDevice)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			Iter->second->nPotocolType = NONE;
		}
	}

	bool CRequestManager::InternalExecuateProbe(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest)
	{
		ptCOMLib->ptCOMLibrary->ResetRequestMode();
		return ptCOMLib->ptCOMLibrary->ExecuateProbe(ptRequest);
	}

	bool CRequestManager::InternalExecuate(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest)
	{
		return ptCOMLib->ptCOMLibrary->ExecuateRequest(ptRequest);
	}

	bool CRequestManager::Execuate(const TSTRING& strDevice,CRequestPtr& ptRequest,const std::string& strPlatform)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter) && Iter->second->bIsConnected)
		{
			//1. Lock 
			SimapleLock lock(Iter->second->csCOM);

			//2. Makesure in the correctly potocol and the execuate the command
			if (( strPlatform.find("Qualcomm8x26") != std::string::npos ) &&
				( strPlatform.find("Qualcomm8x10") != std::string::npos ) &&
				( strPlatform.find("Qualcomm8916") != std::string::npos ))
			{  /*for NOT 8x26 & 8x10 platform*/
				if(SwitchPotocol(strDevice,ptRequest,strPlatform))
					bRes = InternalExecuate(Iter->second,ptRequest);
			}else{
				bRes = InternalExecuate(Iter->second,ptRequest);
			}
		}
		return bRes;
	}

	bool CRequestManager::GetMode(const TSTRING& strDevice,bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode)
	{
		bool bRes = false;

		
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter) && Iter->second->bIsConnected)
		{
			bIsDiagMode = false;
			bIsDLMode = false;
			bIsDLMode = false;

			int nMaxRetry = 10;
			while(nMaxRetry-- > 0 && bRes == false)
			{
				CRequestPtr ptProbeRequest = new CAskDLDIAGModeRequest(bIsDiagMode,bIsDLMode,bIsATMode);
				if(InternalExecuateProbe(Iter->second,ptProbeRequest))
				{
					if(ptProbeRequest->WaitForWrite())
					{
						bRes = bIsDiagMode || bIsDLMode || bIsATMode;
						if(bIsDiagMode)
							Iter->second->nPotocolType = DIAG;
						else if(bIsDLMode)
							Iter->second->nPotocolType = DL;
						else if(bIsATMode)
							Iter->second->nPotocolType = AT;
						else
						{
							AddLog(strDevice,_T("CRequestManager::GetMode::CheckMode Fail"));
							bRes = false;
						}
					}
					else
					{
						AddLog(strDevice,_T("CRequestManager::GetMode::Timeout Fail"));
						//Need to retry or change timeout or other issue
					}
				}
				else
				{
					AddLog(strDevice,_T("CRequestManager::GetMode::Execuate Fail"));
				}

				//delete ptProbeRequest;

			};//End of retry

			/*Temp for 8260
			Iter->second->nPotocolType = DIAG;
			bIsDiagMode = true;
			bRes = true;
			*/
		}

		return bRes;
	}

	PotocolType CRequestManager::GetPotocolType(const TSTRING& strDevice)
	{
		PotocolType type = NONE;

		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter) && Iter->second->bIsConnected)
		{
			type = Iter->second->nPotocolType;
			if(type == NONE)
			{
				bool bIsDiagMode = false;
				bool bIsDLMode = false;
				bool bIsATMode = false;

				if(GetMode(strDevice,bIsDiagMode,bIsDLMode,bIsATMode))
				{
					if(bIsDiagMode)
						type = DIAG;
					else if(bIsDLMode)
						type = DL;
					else if(bIsATMode)
						type = AT;
				}
			}
		}
		return type;
	}

	bool CRequestManager::SwitchPotocol(const TSTRING& strDevice,CRequestPtr& ptRequest,const std::string& strPlatform)
	{
		bool bRes = false;

		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			if(Iter->second->nPotocolType == NONE)
			{//Need to ask which mode is.
				bool bIsDiagMode = false;
				bool bIsDLMode = false;
				bool bIsATMode = false;
				if(GetMode(strDevice,bIsDiagMode,bIsDLMode,bIsATMode))
				{
					if(bIsDiagMode)
						Iter->second->nPotocolType = DIAG;
					else if(bIsDLMode)
						Iter->second->nPotocolType = DL;
					else if(bIsATMode)
						Iter->second->nPotocolType = AT;
				}
				else
				{
					assert(0);//Need to check reason
					//Need to retry or change timeout or other issue
				}
			}

			if(ptRequest->GetPotocolType() != Iter->second->nPotocolType)
			{
				switch(ptRequest->GetPotocolType())
				{
				case NONE:
					//Do nothing
					bRes = true;
					break;
				case DIAG:
					assert(0);//Need to implement
					//ptCOMLib->ptCOMLibrary->MakesureToDIAGMode();
					//ptCOMLib->nPotocolType = DIAG;
					bRes = false;
					break;
				case DL:
					if("Qualcomm6245" == strPlatform)
						bRes = SwitchDL_Qualcomm6245(strDevice);
					else if(("Qualcomm8650" == strPlatform) || ("Qualcomm8x55" == strPlatform))
						bRes = SwitchDL_Qualcomm6240(strDevice);
					else if(("Qualcomm6240" == strPlatform) || ("Qualcomm6270" == strPlatform))
						bRes = SwitchDL_Qualcomm6240(strDevice);
					else if("Qualcomm6280" == strPlatform)
						bRes = SwitchDL_Qualcomm6245(strDevice);
					else if("Qualcomm7x27" == strPlatform)
						bRes = SwitchDL_Qualcomm6245(strDevice);
					//20110819 Dave.Wu
					else if("Qualcomm8x55_SecureOSBL" == strPlatform)
						bRes = SwitchDL_Qualcomm8x55_SecureOSBL(strDevice);
					else assert(0);//Need to support the platform
					break;
				case QLIB:
					//ptCOMLib->nPotocolType = QLIB;
					bRes = false;
					assert(0);//Need to implement
					break;
				case AT:
					//ptCOMLib->nPotocolType = AT;
					bRes = false;
					assert(0);//Need to implement
					break;
				default:
					bRes = false;
					assert(0);//Need to implement
					break;
				};
			}
			else bRes = true;
		}

		return bRes;
	}
	//20110819 Dave.Wu
	bool CRequestManager::SwitchDL_Qualcomm8x55_SecureOSBL(const TSTRING& strDevice)
	{
		bool bRes = false;
		Sleep(100);

		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			CRequestPtr ptRequest1 = new CDIAG2DL6280Request();
			bRes = InternalExecuate(Iter->second,ptRequest1);
			//assert(bRes);
			if(bRes)
				bRes = ptRequest1->WaitForWrite();
			//assert(bRes);

			//delete ptRequest1;
			
			int nRetryTimes = 60;
			if(bRes)
			{
				bool bIsDiagMode = false;
				bool bIsDLMode = false;
				bool bIsATMode = false;
				do
				{
					Sleep(500);
				}
				while(GetMode(strDevice,bIsDiagMode,bIsDLMode,bIsATMode) == false && nRetryTimes--); //It needs amount 5s. I set the timeout as 30s.
			}
		}
		return bRes;
	}

	bool CRequestManager::SwitchDL_Qualcomm6245(const TSTRING& strDevice)
	{
		bool bRes = false;
		Sleep(100);
		
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{
			//Close log first
			CRequestPtr ptRequest1 = new CCloseLog1Request();
			bRes = InternalExecuate(Iter->second,ptRequest1);
			//assert(bRes);
			if(bRes)
				bRes = ptRequest1->WaitForWrite();
			//assert(bRes);

			Sleep(100);
			//20080704 happen fail for [bRes = InternalExecuate(ptCOMLib,ptRequest2);]

			CRequestPtr ptRequest2 = new CCloseLog2Request();

			bRes = InternalExecuate(Iter->second,ptRequest2);
			//assert(bRes);
			if(bRes)
				bRes = ptRequest2->WaitForWrite();
			//assert(bRes);

			Sleep(100);

			CRequestPtr ptRequest3 = new CCloseLog3Request();

			bRes = InternalExecuate(Iter->second,ptRequest3);
			//assert(bRes);
			if(bRes)
				bRes = ptRequest3->WaitForWrite();
			//assert(bRes);

			Sleep(100);

			//Change 2 DL
			CRequestPtr ptRequestChangeDL = new CDIAG2DL6280Request();

			bRes = InternalExecuate(Iter->second,ptRequestChangeDL);
			//assert(bRes);
			if(bRes)
				bRes = ptRequestChangeDL->WaitForWrite();
			//assert(bRes);

			int nRetryTimes = 60;
			if(bRes)
			{
				bool bIsDiagMode = false;
				bool bIsDLMode = false;
				bool bIsATMode = false;
				do
				{
					Sleep(500);
				}
				while(GetMode(strDevice,bIsDiagMode,bIsDLMode,bIsATMode) == false && nRetryTimes--); //It needs amount 5s. I set the timeout as 30s.
			}
		}
		return bRes;
	}
	bool CRequestManager::SwitchDL_Qualcomm6240(const TSTRING& strDevice)
	{
		bool bRes = false;
		Sleep(100);

	
		COMMAP_Iter Iter;
		if(GetCOMLibrary(strDevice,Iter))
		{

			//Sleep(100);
			CRequestPtr ptRequest2 = new CUnknown2Request(); //0x29,0x01,0x00,0x31,0x40,0x7E // Mode Change Request message 0x01,0x00 =Offline Digital mode
			bRes = InternalExecuate(Iter->second,ptRequest2);
			if(bRes)
				bRes = ptRequest2->WaitForWrite();


			Sleep(100);

			CRequestPtr ptRequest3 = new CUnknown3Request(); //0x0C,0x14,0x3A,0x7E
			bRes = InternalExecuate(Iter->second,ptRequest3);

			if(bRes)
				bRes = ptRequest3->WaitForWrite();

			Sleep(100);

			bool bCanChange2DLMode = false;
			for(int i=0; i<10000 && !bCanChange2DLMode; i++)
			{
				//Send
				//4B 04 0E 00 0D D3 7E
				//Response: It can't switch to DL
				//4B 04 0E 00 01 C7 3A 7E
				//Response: It can switch to DL
				//4B 04 0E 00 00 4E 2B 7E
				CRequestPtr ptRequest = new CDiagAskStateRequest(bCanChange2DLMode);
				if(InternalExecuate(Iter->second, ptRequest))
					bRes = ptRequest->WaitForWrite();
			}

			//Change 2 DL_SECURE_BOOT_2
			if(bCanChange2DLMode)
			{
				CRequestPtr ptRequestChangeDL = new CDIAG2DL6280Request(); // 0x3A,0xA1,0x6E,0x7E

				bRes = InternalExecuate(Iter->second,ptRequestChangeDL);
				//assert(bRes);
				//Reopen comport
				/************************************************************************/
				/* It costs 15s to wait unit HW resetting and comport re-coming out     */
				/* Sleep 10s + close then wait 5s										*/
				/************************************************************************/
				if(bRes)
					Sleep(20000); // wait for the handset HW reset and comport re-coming out
				if(bRes)
				{
					if(Close(strDevice))
					{
						bRes = false;
						for(int i=0; i<3 && !bRes; i++)
						{
							Sleep(5000);
							bRes = Open(strDevice);
						}	
					}
					else
						bRes = false;
				}
			}
			else
				bRes = false;

			if(bRes)
				Iter->second->nPotocolType = DL;
		}
		return bRes;
	}

	bool CRequestManager::GetCOMLibrary(const TSTRING& strDevice,COMMAP_Iter& Iter)
	{
		bool bRes = false;
		Iter = m_mapDevice.find(strDevice);
		if(Iter == m_mapDevice.end())
		{
			SafeCOMLibrayPtr ptLib = new SafeCOMLibray;
			ptLib->ptCOMLibrary = new CSerCOMLibrary;
			m_mapDevice.insert(COMMAP_Pair(strDevice,ptLib));

			//Find again
			Iter = m_mapDevice.find(strDevice);
			bRes = (Iter != m_mapDevice.end());
		}
		else
		{
			bRes  = true;
		}
		return bRes;
	}
	bool CRequestManager::GetLastError(const TSTRING& strDevice, LONG& nErrorCode)
	{
		bool bRes = false;
		COMMAP_Iter Iter = m_mapDevice.find(strDevice);
		if (Iter != m_mapDevice.end())
		{
			Iter->second->ptCOMLibrary->GetLastError();
		}
		return bRes;
	}

	void CRequestManager::PopLastLog(const TSTRING& strDevice)
	{
		COMMAP_Iter Iter = m_mapDevice.find(strDevice);
		if (Iter != m_mapDevice.end())
		{
			Iter->second->ptCOMLibrary->PopLastLog();
		}
	}
}