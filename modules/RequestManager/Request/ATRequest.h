#pragma once

#include "Requestbase.h"
#include <vector>
#include <string>
#include "..\..\SPU\SPU\ISPU_CALLBACK.h"
#include <map>
#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'PLATFORM_TOOL::CATEnableKeypadEventRequest::m_strKeyPressMap' : class 'std::map<_Kty,_Ty>' needs to have dll-interface to be used by clients of class 'PLATFORM_TOOL::CATEnableKeypadEventRequest'


using namespace std;


namespace PLATFORM_TOOL
{


class CATBase : public CRequestBase
{
public:		
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
protected:
	CATBase(const TSTRING& strName,int nTimeout = 5000,bool bHadReturnvalue = true);
	virtual ~CATBase();
protected:
	bool IsOK(CBufferPtr& ptBuffer);
	bool IsFAIL(CBufferPtr& ptBuffer);
	bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend);
private:		
	CBufferPtr m_ptTempBufferReceived;
};


#pragma pack(push, 1)


class RM_CLASS_API CATEnableKeypadEventRequest  : public CATBase
{
public:		
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool WaitForWrite();
	virtual bool WaitForOtherResponse();
protected:
	bool CheckBreak();
	bool Timeout(int dwMilliseconds);
public:
	CATEnableKeypadEventRequest(bool bEnable,bool& bSuccess,int nTimeout,ISPU_CALLBACK *pISPU_CALLBACK,int nKeyCount);
	virtual ~CATEnableKeypadEventRequest();
private:		
	bool& m_bSuccess;
	bool m_bOK;		
	bool m_bWaitForOtherResponse;
	int m_nKeyCount;
	ISPU_CALLBACK *m_pISPU_CALLBACK;
	std::map<std::string,bool> m_strKeyPressMap;

	unsigned char m_bFake1;
	unsigned char m_bFake2;
	unsigned char m_bFake3;
	unsigned char m_bFake4;
};

class RM_CLASS_API CRegisterKeypadEventRequest  : public CDiagRequestBase
{
public:		
	virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	virtual bool WaitForWrite();
	virtual bool WaitForOtherResponse();
protected:
	bool CheckBreak();
	bool Timeout(int dwMilliseconds);
	bool IsOK(CBufferPtr& ptBuffer);
public:
	CRegisterKeypadEventRequest(bool bEnable,bool& bSuccess,int nTimeout,ISPU_CALLBACK *pISPU_CALLBACK,int nKeyCount);
	virtual ~CRegisterKeypadEventRequest();
private:		
	bool& m_bSuccess;
	bool m_bOK;		
	bool m_bWaitForOtherResponse;
	int m_nKeyCount;
	ISPU_CALLBACK *m_pISPU_CALLBACK;
	std::map<std::string,bool> m_strKeyPressMap;
	unsigned char m_bFake1;
	unsigned char m_bFake2;
	unsigned char m_bFake3;
	unsigned char m_bFake4;
};


#pragma pack(pop)


} // End of namespace PLATFORM_TOOL


#pragma warning (pop)
