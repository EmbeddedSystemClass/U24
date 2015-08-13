#ifndef __DLREQUEST__H__
#define __DLREQUEST__H__
#pragma once
#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\Lib\RequestUtility\DownloadCommand.h"
#include "..\..\DownloadDLL\DLcommand.h"
#include <atlstr.h>
#include <stdlib.h>
#include <tchar.h>


#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{	
	
	class RM_CLASS_API CDownloadRequestBase  : public CRequestBase
	{
	public:
		CDownloadRequestBase(const TSTRING& strName,bool bHadReturnvalue = true);
		CDownloadRequestBase(const TSTRING& strName,PotocolType nPotocolType,int	nTimeout = 5000,bool bHadReturnvalue = true);
		virtual ~CDownloadRequestBase();

	public:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual bool GetReceiveData(CBufferPtr& ptBufferReceived); // use for response function execute result
		
	protected:
		bool Tx(const char* szCommand,int nCommandLen, bool bSpeedUp = false);
		bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend, bool bSpeedup = false);
		
		bool GetProcessResult();
		void SetProcessResult(bool bResult);
		int GetLastError();
		void SetLastError(int nError);
		void Safememcopy(void* pBuffer, int nBufferSize,void* pData,int nDataSize);
		
	protected:	
		CBufferPtr m_ptTempBufferReceived;
		bool m_bRunSuccess;
		int m_nErrorCode;
	};
	
	//CDLRAWRequest Request
	class RM_CLASS_API CDLRAWRequest: public CDownloadRequestBase
	{
	public:
		CDLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
		virtual ~CDLRAWRequest();
	};

	//Send dl raw request with returned value
	class RM_CLASS_API CDLRawWithReturnRequest: public CDownloadRequestBase
	{
	public:
		CDLRawWithReturnRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
		virtual ~CDLRawWithReturnRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};



	/***************************************   DMSS   ***************************************/
	/***************** GetExecuteResult by ptRequest->GetReceiveData(ptBuffer)***************/
	class RM_CLASS_API CDMSSNop : public CDownloadRequestBase
	{
	public:
		CDMSSNop();
		virtual ~CDMSSNop();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSParameter: public CDownloadRequestBase
	{
	public:
		CDMSSParameter();
		virtual ~CDMSSParameter();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSWrite32Bit : public CDownloadRequestBase
	{
	public:
		CDMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress);
		virtual ~CDMSSWrite32Bit();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSGo : public CDownloadRequestBase
	{
	public:
		CDMSSGo(WORD nwSegment,WORD nwoffset);
		virtual ~CDMSSGo();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSReset : public CDownloadRequestBase
	{
	public:
		CDMSSReset();
		virtual ~CDMSSReset();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	/***************************  (Original/Speedup) Streaming Download  ****************************/
	/***************** GetExecuteResult by ptRequest->GetReceiveData(ptBuffer)***************/
	class RM_CLASS_API CStreamingDLHello : public CDownloadRequestBase
	{
	public:
		CStreamingDLHello(int nVersion,int nCVersion,int nFeaturebit,bool bSpeedUp = false,DownloadProtocol nDownloadProtocol = QTT_FASTDOWNLOAD);
		virtual ~CStreamingDLHello();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool m_bSpeedUp;
		DownloadProtocol m_nDownloadProtocol;
	};
	

	class RM_CLASS_API CStreamingDLSecurity : public CDownloadRequestBase
	{
	public:
		CStreamingDLSecurity(BYTE nMode, bool bSpeedUp = false);
		virtual ~CStreamingDLSecurity();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CStreamingDLPartition : public CDownloadRequestBase
	{
	public:
		CStreamingDLPartition(BYTE* lpbyData, WORD wnlength/*max=512*/, bool bOverride, BYTE& nStatus, bool bSpeedUp = false);
		virtual ~CStreamingDLPartition();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		BYTE& m_nRespStatus;
	};

	class RM_CLASS_API CStreamingDLOpenMultiImage : public CDownloadRequestBase
	{
	public:
		CStreamingDLOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, bool bSpeedUp = false);
		virtual ~CStreamingDLOpenMultiImage();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CStreamingDLOpenMultiImage7K : public CDownloadRequestBase
	{
	public:
		CStreamingDLOpenMultiImage7K(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, CString Md5FromFile, bool bSpeedUp = false);
		virtual ~CStreamingDLOpenMultiImage7K();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		CString m_Md5FromFile;
	};

	class RM_CLASS_API CStreamingDLIRAMRead : public CDownloadRequestBase
	{
	public:
		CStreamingDLIRAMRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp = false);
		virtual ~CStreamingDLIRAMRead();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	class RM_CLASS_API CStreamingDLeMMCRead : public CDownloadRequestBase
	{
	public:
		CStreamingDLeMMCRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp  = false);
		virtual ~CStreamingDLeMMCRead();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwSector;
	};

	class RM_CLASS_API CStreamingDLStrRead : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrRead(WORD wnLength, DWORD dwAddress, bool bSpeedUp = false);
		virtual ~CStreamingDLStrRead();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
			virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	class RM_CLASS_API CStreamingDLStrWrite : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false);
		virtual ~CStreamingDLStrWrite();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	class RM_CLASS_API CStreamingDLStrBuffer7K : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrBuffer7K(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false);
		virtual ~CStreamingDLStrBuffer7K();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	class RM_CLASS_API CStreamingDLStrBufferWrite7K : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrBufferWrite7K(int nImageSize, CString MD5,bool bSpeedUp = false);
		virtual ~CStreamingDLStrBufferWrite7K();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	//////////////////////////////////////////////////////////////////////////

	class RM_CLASS_API CStreamingDLCloseImage : public CDownloadRequestBase
	{
	public:
		CStreamingDLCloseImage(bool bSpeedUp = false);
		virtual ~CStreamingDLCloseImage();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CStreamingDLCloseImage7K : public CDownloadRequestBase
	{
	public:
		CStreamingDLCloseImage7K(CString Md5FromFile, bool bSpeedUp = false);
		virtual ~CStreamingDLCloseImage7K();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		CString m_Md5FromFile;
	};

	//////////////////////////////////////////////////////////////////////////
	
	class RM_CLASS_API CStreamingDLReset : public CDownloadRequestBase
	{
	public:
		CStreamingDLReset(bool bSpeedUp = false);
		virtual ~CStreamingDLReset();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	/* Alex.Chen Add Start 2011/06/12 */
	class RM_CLASS_API CSetFastBootFlagQCSRequestFor8260 : public CDownloadRequestBase
	{
	public:
		CSetFastBootFlagQCSRequestFor8260(bool& bSuccess, byte comport = 0x77);
		virtual ~CSetFastBootFlagQCSRequestFor8260();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
	/* Alex.Chen Add End 2011/06/12 */

	/* Evonne.Hsieh Add Start 2012/03/30 */
	class RM_CLASS_API CSetFastBootFlagQCSRequestFor8960 : public CDownloadRequestBase
	{
	public:
		CSetFastBootFlagQCSRequestFor8960(bool& bSuccess, byte comport = 0x77);
		virtual ~CSetFastBootFlagQCSRequestFor8960();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
	/* Evonne.Hsieh Add End 2012/03/30 */

	/* Evonne.Hsieh Add Start 2012/02/17 */
	class RM_CLASS_API CSetFastBootFlagQCSRequestFor7227A : public CDownloadRequestBase
	{
	public:
		CSetFastBootFlagQCSRequestFor7227A(CString str_multiDLFlag, bool& bSuccess, byte comport = 0x77);
		virtual ~CSetFastBootFlagQCSRequestFor7227A();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
	/* Evonne.Hsieh Add Start 2012/02/17 */

	/* Lennon.Zhang Add Start 2013/01/25 */
	class RM_CLASS_API CSetFastBootFlagQCSRequestFor8X25 : public CDownloadRequestBase
	{
	public:
		CSetFastBootFlagQCSRequestFor8X25(bool& bSuccess, byte comport = 0x77);
		virtual ~CSetFastBootFlagQCSRequestFor8X25();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
	/* Lennon.Zhang Add End 2013/01/25 */

	class RM_CLASS_API CSendOEMCMD : public CDownloadRequestBase
	{
	public:
		CSendOEMCMD();
		virtual ~CSendOEMCMD();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	/* Macl.Ma Add Start 2012/07/19 */
	class RM_CLASS_API CSendResetCMD : public CDownloadRequestBase
	{
	public:
		CSendResetCMD();
		virtual ~CSendResetCMD();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	/* Alex.Chen Add Start 2011/09/28 */
	class RM_CLASS_API CDDRTest : public CDownloadRequestBase
	{
	public:
		CDDRTest();
		virtual ~CDDRTest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CGetDDRTestResult : public CDownloadRequestBase
	{
	public:
		CGetDDRTestResult();
		virtual ~CGetDDRTestResult();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	/* Alex.Chen Add End 2011/09/28 */

	/* Alex.Chen Add Start 2011/10/13 */
	class RM_CLASS_API CStreamingRead : public CDownloadRequestBase
	{
	public:
		CStreamingRead();
		virtual ~CStreamingRead();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	/* Alex.Chen Add End 2011/10/13 */
}

#endif // __DLREQUEST__H__