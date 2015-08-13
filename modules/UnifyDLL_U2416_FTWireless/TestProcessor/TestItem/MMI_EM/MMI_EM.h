/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CMMIBB object.
 * FileName   : CMMIBB.h
 * Version    : $Revision:   1.1  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/04/15
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/04/15     Evonne.Hsieh             Create.
 ********************************************************************************/

#ifndef MMIEM_Proc_H_
#define MMIEM_Proc_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "WinNfc.h"

typedef int (__stdcall * PNfcEstablishContext)(
    IN  DWORD dwScope,
    IN  LPCVOID pvReserved1,
    IN  LPCVOID pvReserved2,
    OUT LPNFCCONTEXT phContext);

typedef int (__stdcall * PNfcConnect)(
    IN      NFCCONTEXT hContext,
    IN      LPCSTR szReader,
    IN      DWORD dwShareMode);

typedef int (__stdcall * PNFC_ConnectCard)(
	IN NFCCONTEXT hContext, 
	IN DWORD dwShareMode, 
	DWORD dwPreferredProtocols);

typedef int (__stdcall * PNfcBuzzerControl)(
    IN NFCCONTEXT hContext,
    IN BYTE bState);

typedef int (__stdcall * PNFC_GetATR)(
	IN NFCCONTEXT hContext, 
	OUT PBYTE pAtr,
	OUT LPDWORD pLength,
	OUT LPDWORD pActiveProtocol);

typedef int (__stdcall * PNFC_SendAPDU)(
    IN NFCCONTEXT hContext,
    IN LPCBYTE pbSendBuffer,
    IN DWORD cbSendLength,
    OUT LPBYTE pbRecvBuffer,
    IN OUT LPDWORD pcbRecvLength);

typedef int (__stdcall * PNFC_DisconnectCard)(
	IN NFCCONTEXT hContext, 
	IN DWORD dwDisposition);

typedef int (__stdcall * PNfcDisconnect)(
    IN      NFCCONTEXT hContext,
    IN      DWORD dwDisposition);

typedef int (__stdcall * PNfcReleaseContext)(
    IN      NFCCONTEXT hContext);

class MMIBBProc : public ITestItem
{
RF_DECLARE_DYNCREATE(MMIBBProc)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_Name;
	std::string m_str_Message;
	std::string m_str_ErrorCode;
	int m_i_offset;
	char m_szFAData[FTD_BUF_SIZE];
	char m_szErrCode[20];
	HMODULE m_hNfcFuncDLL;


// Member functions
public:
	MMIBBProc() {}
	~MMIBBProc() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
   // virtual bool ReadFAData_301(int i_sectorNum, char* p_sz_FAData, int i_size);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MMI_PASS_FAIL_CRITERIA();
	bool Detroit_NFCSWPTest();
	bool NFCOffline0CmTestAuto();
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
	bool GetSensorLog();
	bool Test_NFC_IT100MUR();
	void ShowNfcCardReaderError(NFCCONTEXT ctx, LONG rc);
	void PrintData(PBYTE pbData, DWORD dwLen);
};


#endif // End of #ifndef MMIEM_Proc_H_
