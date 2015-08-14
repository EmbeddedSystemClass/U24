#ifndef __FTDCMDMODULE_H__
#define __FTDCMDMODULE_H__

#include "..\RequestUtility\CmdModuleBase.h"
#include "..\RequestUtility\CmdBase.h"

#include "tcpdriver.h"
#include "CmdType.h"
#include "UtilityForFTD.h"

static const int PAYLOAD_SIZE = 4096;
static const int ERRMSG_SIZE = 255;

class CFTDCmdModule : public CCmdModule_Base
{
public:
	CFTDCmdModule(UINT Port = 2000);
	virtual ~CFTDCmdModule();

	virtual bool ConnectDevice();
	virtual bool DisConnectDevice();
	virtual bool IsConnected();

	void		SetExtraCmd(char *szExtraCmd) ;
	void        SetExtraCmd(CString szExtraCmd) ;
	const UINT	GetExtraCmdSize() const;
	char		*GetExtraCmd();
	char		*GetErrResponse();
	UINT		m_Timeout;

	bool Open();
	bool Close();

protected:
	virtual bool PreExe();
	virtual bool ExeCommand(CCmdPtr &Cmd);
	virtual bool PostExe();

	
	bool EnterFTDMode();
	bool VerifyFTDMode();

	bool ExactGeneralCmdInOut(int nPort, int nTimeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer, char* szErrMsg);
	UINT GenPacketSend(int iType, int PayLoadSize, unsigned char * pPackData, unsigned char * OChecksum, int nPort, char* szErrMsg);
	UINT GenPacketRecv(int iType, unsigned char * PayloadCommand, unsigned char * pReceiveData, unsigned char * NChecksum, int nTimeout, char* szErrMsg);
	UINT PacketResultCheck(int iPacketResult, unsigned char * pPackData, char* szErrMsg);
	
private:
	UINT m_Port;
	
	char* m_ExtraCmd;
	char* m_szErrMsg;
	UINT m_ExtraCmdSize;
public:
	BaseDrv *m_Device;
	
};

class CFTDCmdContainer : public ICmdBase
{
public:
	CFTDCmdContainer(char *Cmd, UINT CmdSize, bool HasReturnValue = false, UINT TimeOut = 0);
	virtual ~CFTDCmdContainer();

	virtual const BYTE *GetCmd() const;
	virtual const BYTE *GetResponse() const;
	virtual const UINT GetCmdSize() const;
	virtual bool HasReturnValue() const;
	virtual UINT GetTimeOut() const;
    virtual void SetResponseBufferSize(UINT size);
    virtual UINT GetResponseSize() const ;
	
protected:
	char *m_Cmd;
	char *m_Response;
	UINT m_TimeOut;
	UINT m_CmdSize;
	bool m_HasReturnValue;
};

#endif