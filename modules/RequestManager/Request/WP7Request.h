#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{

	class RM_CLASS_API CPVKRequest: public CDiagRequestBase
	{
	public:
		CPVKRequest();
		virtual ~CPVKRequest(){}		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
	};

	class RM_CLASS_API CWolfWritePVKRequest: public CDiagRequestBase
	{
	public:
		CWolfWritePVKRequest(int &bStatus, int& nIsBegin, int& nIsMore, int& nCheckSum, int& nDataLength, TCHAR* pData);
		virtual ~CWolfWritePVKRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int& m_nStatus;
	};

	class RM_CLASS_API CWolfPVKisWriteRequest: public CDiagRequestBase
	{
	public:
		CWolfPVKisWriteRequest(bool &bWritten);
		virtual ~CWolfPVKisWriteRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool& m_bWritten;
	};

	class RM_CLASS_API CDeletePVKRequest: public CDiagRequestBase
	{
	public:
		CDeletePVKRequest();
		virtual ~CDeletePVKRequest(){}		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
	};

	class RM_CLASS_API CWolfClearPVKRequest: public CDiagRequestBase
	{
	public:
		CWolfClearPVKRequest(bool &bSuccess);
		virtual ~CWolfClearPVKRequest(){}		
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool  &m_bSuccess;
	};

	class RM_CLASS_API CCheckPVKRequest: public CDiagRequestBase
	{
	public:
		CCheckPVKRequest(int &status);
		virtual ~CCheckPVKRequest(){}		
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int &m_nStatus;
	};

	class RM_CLASS_API CWolfCheckPVKRequest: public CDiagRequestBase
	{
	public:
		CWolfCheckPVKRequest(int &status);
		virtual ~CWolfCheckPVKRequest(){}		
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int &m_nStatus;
	};
	
	class RM_CLASS_API CWolfBackUpPVKRequest: public CDiagRequestBase
	{
	public:
		CWolfBackUpPVKRequest(int &status);
		virtual ~CWolfBackUpPVKRequest(){}		
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int &m_nStatus;
	};
	
	class RM_CLASS_API CWolfRestorePVKRequest: public CDiagRequestBase
	{
	public:
		CWolfRestorePVKRequest(int &status);
		virtual ~CWolfRestorePVKRequest(){}		
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int &m_nStatus;
	};

	class RM_CLASS_API CLeopardMasterClearRequest: public CDiagRequestBase
	{
	public:
		CLeopardMasterClearRequest(bool &bSuccess);
		virtual ~CLeopardMasterClearRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};

	class RM_CLASS_API CWolfResetRequest: public CDiagRequestBase
	{
	public:
		CWolfResetRequest(bool &bSuccess);
		virtual ~CWolfResetRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool &m_bSuccess;
	};


	class RM_CLASS_API CLeopardHWversionRequest: public CDiagRequestBase
	{
	public:
		CLeopardHWversionRequest(int& variant, int& mem, int& evt, int& evtMinor, int& pilotRun);
		virtual ~CLeopardHWversionRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int &m_varient;
		int &m_mem;
		int &m_evt;
		int &m_evtMinor;
		int &m_pilotRun;
	};

	class RM_CLASS_API CLeopardSWversionRequest: public CDiagRequestBase
	{
	public:
		CLeopardSWversionRequest (std::string &SWver);
		virtual ~CLeopardSWversionRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		std::string &m_SWversion;

	};


	class RM_CLASS_API CWolfHWversionRequest: public CDiagRequestBase
	{
	public:
		CWolfHWversionRequest(int& nVariant, int& nPCBStage, int& nMajorSN, int& nMinorSN, int& nMem, int& neMMC, int& nTouchPanel, int& nModule);
		virtual ~CWolfHWversionRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int& m_Variant;
		int& m_PCBStage;
		int& m_MajorSN;
		int& m_MinorSN;
		int& m_Mem;
		int& m_eMMC;
		int& m_TouchPanel;
		int& m_Module;
	};


	class RM_CLASS_API CLeopardSetLocalTime: public CDiagRequestBase
	{
	public:
		CLeopardSetLocalTime (bool &bSuccess);
		virtual ~CLeopardSetLocalTime(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool  &m_Success;
	};


	class RM_CLASS_API CTargetTargetOperatorRequest: public CDiagRequestBase
	{
	public:
		CTargetTargetOperatorRequest (std::string &devName);
		virtual ~CTargetTargetOperatorRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		std::string& m_strOperatorName;
	};

	class RM_CLASS_API CWP7CITSpecifyFileNameRequest: public CDiagRequestBase
	{
	public:
		CWP7CITSpecifyFileNameRequest(int &nStatus, int& nFileNameLength, TCHAR* pFileName);
		virtual ~CWP7CITSpecifyFileNameRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int& m_nStatus;
	};

	class RM_CLASS_API CWP7CITWriteRequest: public CDiagRequestBase
	{
	public:
		CWP7CITWriteRequest(int &bStatus, int& nIsBegin, int& nIsMore, int& nDataLength, TCHAR* pData);
		virtual ~CWP7CITWriteRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int& m_nStatus;
	};

	class RM_CLASS_API CWP7CITReadRequest: public CDiagRequestBase
	{
	public:
		CWP7CITReadRequest(int &bStatus, int& nIsBegin, int& nIsMore, int& nDataLength, char* pData);
		virtual ~CWP7CITReadRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		int& m_nStatus;
		int& m_nIsMore;
		int& m_nDataLength;
		char* m_pData;
	};

	class RM_CLASS_API CSetupSleepRequest: public CDiagRequestBase
	{
	public:
		CSetupSleepRequest(bool &nStatus, int& nTime);
		virtual ~CSetupSleepRequest(){}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived1)
		{		
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived1);
		}
	private:
		bool& m_nStatus;
		int& m_nTime;
	};
} //PLATFORM_TOOL