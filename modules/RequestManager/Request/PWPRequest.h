#ifndef __PWPREQUEST__H__
#define __PWPREQUEST__H__

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
	class RM_CLASS_API CIniPWPRequest : public CDiagRequestBase
	{
	public:
		CIniPWPRequest( bool &bResult , BYTE funcIndex)  ;
		virtual ~CIniPWPRequest() ;
	protected:
		virtual bool IsCompletePacket( CBufferPtr& ptBufferReceived ) ;
		virtual void OnDataReceiver( CBufferPtr& ptBufferReceived ) ;
	private:
		bool& m_bResult;
	};

	class RM_CLASS_API CWolfWriteDevNumRequest : public CDiagRequestBase
	{
	public:
		CWolfWriteDevNumRequest( bool &bResult, BYTE* DevNum ) ;
		virtual ~CWolfWriteDevNumRequest() ;
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) ;
	private:
		bool& m_bResult ;
	};

	class RM_CLASS_API CWolfReadDevNumRequest : public CDiagRequestBase
	{
	public:
		CWolfReadDevNumRequest( char* deviceNum, bool& bResult );
		virtual ~CWolfReadDevNumRequest() ;
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) ;
	private:
		bool& m_bResult ;
		char*  m_DeviceNumber;
	};

	class RM_CLASS_API CWolfWritePublicKeyRequest : public CDiagRequestBase
	{
	public:
		CWolfWritePublicKeyRequest( bool& bResult, BYTE* PublicKey ) ;
		virtual ~CWolfWritePublicKeyRequest() ;
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) ;
	private:
		bool& m_bResult ;
	};

	class RM_CLASS_API CWolfReadPublicKeyRequest : public CDiagRequestBase
	{
	public:
		CWolfReadPublicKeyRequest( char* publicKey, bool& bResult ) ;
		virtual ~CWolfReadPublicKeyRequest() ;
	protected:
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) ;
	private:
		bool& m_bResult ;
		char*  m_PublicKey ;
	};
}
#endif