#ifndef __ADBCMDMODULE_H__
#define __ADBCMDMODULE_H__

#include "..\RequestUtility\CmdModuleBase.h"
#include "..\RequestUtility\CmdBase.h"
#include "ADBTypeDef.h"
#include "ADBTypeDef.h"


// This class is exported from the ADBCmdModule.dll
class CADBCmdModule : public CCmdModule_Base
{
public:
    CADBCmdModule(const char *DeviceSerialNum = "-1") ;
	virtual ~CADBCmdModule();
	
	virtual bool ConnectDevice();
	virtual bool DisConnectDevice();
	virtual bool IsConnected();
    
    void ShouldConnectDevice(bool bShould) ;
    void SetExecuteType(ADBModuleExeType exeType) ;
	//virtual bool ExeCommand(CCmdPtr &Cmd);


protected:	
	virtual bool PreExe() ;
	virtual bool ExeCommand(CCmdPtr &Cmd) ;
	virtual bool PostExe() ;
	bool ExeSysCmd(CCmdPtr &Cmd, ADBModuleExeType exeType) ;

public:
	bool ADBRoot();
	bool ADBKillServer();
	bool ADBDevices(char *szDeviceList, size_t BufferSize);

private:
	char *m_DeviceSerialNum;
    bool m_bConnectDevice ;
    ADBModuleExeType m_exeType ;
};

class CADBCmdContainer : public ICmdBase
{
public:
	CADBCmdContainer(BYTE *Cmd, UINT CmdSize, bool HasReturnValue = false, UINT TimeOut = 0);
	virtual ~CADBCmdContainer();

	virtual const BYTE *GetCmd() const;
	virtual const BYTE *GetResponse() const;
	virtual const UINT GetCmdSize() const;
	virtual bool HasReturnValue() const;
	virtual UINT GetTimeOut() const;
	virtual void SetResponseBufferSize(UINT size);
    virtual UINT GetResponseSize() const ;

protected:
	BYTE *m_Cmd;
	BYTE *m_Response;
	UINT m_TimeOut;
	UINT m_CmdSize;
	bool m_HasReturnValue;
};

#endif