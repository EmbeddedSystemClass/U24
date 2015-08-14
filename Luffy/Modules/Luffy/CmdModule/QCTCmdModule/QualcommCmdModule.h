#ifndef __QUALCOMMCMDMODULE_H__
#define __QUALCOMMCMDMODULE_H__

#include "..\RequestUtility\CmdModuleBase.h"
#include "..\RequestUtility\CmdBase.h"
#include "..\..\..\..\lib\Qualcomm\inc\QLib.h"
#include "..\..\..\..\lib\Qualcomm\inc\QLib_Defines.h"
#include "Utility.h"


#pragma warning (disable:4800)


class CQualcommCmdModule : public CCmdModule_Base
{
public:
	// TODO: add your methods here.
	CQualcommCmdModule(UINT Port = -1, bool UseQPST = false);
	virtual ~CQualcommCmdModule(void);

	virtual bool ConnectDevice();
	virtual bool DisConnectDevice();
	virtual bool IsConnected();

	bool WriteNVItem(unsigned short iStartNV, unsigned short iEndNV, unsigned char *pBuffer);
	bool ReadNVItem(unsigned short iStartNV, unsigned short iEndNV, unsigned char *pBuffer);
	bool WriteNVItemByIndex(unsigned short iNV, unsigned char *pBuffer, int iLength, int iIndex);
	bool ReadNVItemByIndex(unsigned short iNV, unsigned char *pBuffer, int iLength, int iIndex);
	
    // QLib
	bool EFS2_SyncWithWait(unsigned char* sFilePath, unsigned long iTimeOut_ms, unsigned char* piExecutionStatus);
	bool EFS2_GoldenCopy();
    bool GetPhoneMode(unsigned long* piMode) ;
    bool SetPhoneMode(int iMode) ;
    bool SetBootMode(unsigned char bFTMmode, unsigned char bReset) ;
    bool WLAN_GEN6_START(int wlanID);
    bool WLAN_GEN6_GET_MAC_ADDR(unsigned char *macAddr);
    bool WLAN_GEN6_SET_MAC_ADDR(unsigned char *macAddr);

protected:
	virtual bool PreExe();
	virtual bool ExeCommand(CCmdPtr &Cmd);
	virtual bool PostExe();

private:
	bool m_UseQPST;
	UINT m_Port;
	static HANDLE m_hResourceContext;
};

class CQualcommCmdContainer : public ICmdBase
{
public:
	CQualcommCmdContainer(BYTE *Cmd, UINT CmdSize, bool HasReturnValue = false, UINT TimeOut = 0);
	virtual ~CQualcommCmdContainer();

	virtual const BYTE *GetCmd() const;
	virtual const BYTE *GetResponse() const;
	virtual const UINT GetCmdSize() const;
	virtual bool HasReturnValue() const;
	virtual UINT GetTimeOut() const;
	virtual void SetResponseBufferSize(UINT size);
    virtual UINT GetResponseSize() const ;

protected:
	BYTE *m_Cmd;
	BYTE m_Response[DIAG_MAX_PACKET_SIZE];
	UINT m_TimeOut;
	UINT m_CmdSize;
	bool m_HasReturnValue;
    short m_ResponseSize ;
};



#endif // End of #ifndef __QUALCOMMCMDMODULE_H__