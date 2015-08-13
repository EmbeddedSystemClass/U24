#pragma once
#include "Request\Requestbase.h"
#include "..\SerCOMLib\SerCOMLibrary.h"
#include <map>
#include "..\Common\CommonUtil\SimpleCriticalSection.h"
#include "..\Common\CommonUtil\RefObject.h"

namespace PLATFORM_TOOL
{
	class CRequestManager
	{
		class SimapleLock
		{
		public:
			SimapleLock(CSimpleCriticalSection& csCOM):m_csCOM(csCOM){csCOM.Lock();}
			~SimapleLock(){m_csCOM.Unlock();}
		private:
			CSimpleCriticalSection& m_csCOM;
		};
		class SafeCOMLibray  : public CRefObject
		{
			friend class CRequestManager;
		protected:
			SafeCOMLibray():bIsConnected(false),nPotocolType(NONE),m_lRefCount(0){}
			virtual ~SafeCOMLibray()
			{
				/*
				It will close COM port by itself lastly
				*/
				if(bIsConnected)
					ptCOMLibrary->Close();
			}
			CSerCOMLibraryPtr ptCOMLibrary;
			CSimpleCriticalSection csCOM;
			bool	 bIsConnected;
			PotocolType nPotocolType;		
			long m_lRefCount;
		};
	public:
		CRequestManager(void);
		virtual ~CRequestManager(void);
		void AddRef(const TSTRING& strDevice);
		void ReleaseRef(const TSTRING& strDevice);
	public:		
		bool Open(const TSTRING& strDevice);
		bool Close(const TSTRING& strDevice,bool bKeepPotocolType = false);
		bool IsOpen(const TSTRING& strDevice);
		bool Execuate(const TSTRING& strDevice,CRequestPtr& prRequest,const TSTRING& strPlatform);
		void Reset(const TSTRING& strDevice);
		void ResetMode(const TSTRING& strDevice);
		PotocolType GetPotocolType(const TSTRING& strDevice);
		bool GetLastError(const TSTRING& strDevice, LONG& nErrorCode);
		void PopLastLog(const TSTRING& strDevice);
	private:		
		typedef CRefObjectPtr<SafeCOMLibray> SafeCOMLibrayPtr;
		std::map<TSTRING,SafeCOMLibrayPtr>     m_mapDevice;
		typedef std::map<TSTRING,SafeCOMLibrayPtr>::iterator COMMAP_Iter;
		typedef std::map<TSTRING,SafeCOMLibrayPtr>::const_iterator COMMAP_CIter;
		typedef std::pair <TSTRING, SafeCOMLibrayPtr> COMMAP_Pair;
	private:
		bool GetCOMLibrary(const TSTRING& strDevice,COMMAP_Iter& Iter);
		bool SwitchDL_Qualcomm6240(const TSTRING& strDevice);
		bool SwitchDL_Qualcomm6245(const TSTRING& strDevice);
		bool SwitchDL_Qualcomm8x55_SecureOSBL(const TSTRING& strDevice);
	private:
		//Don't change this function,InternalExecuate(), to protected or public
		//It's not safe for any other class
		bool InternalExecuateProbe(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest);
		bool InternalExecuate(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest);
		bool SwitchPotocol(const TSTRING& strDevice,CRequestPtr& ptRequest,const std::string& strPlatform);
		bool GetMode(const TSTRING& strDevice,bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode);
		void AddLog(const TSTRING& strDevice,const TSTRING& strMsg);
	};
}