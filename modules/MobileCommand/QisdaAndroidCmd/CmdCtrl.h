// CmdCtrl.h: interface for the CmdCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_)
#define AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstddef> // size_t
#include "NeptuneGeneralController.h" // public CNeptuneGeneralController
#include "comdriver.h"
#include "usbdriver.h"
#include "tcpdriver.h"
#include "serialdriver.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

static const int PAYLOAD_SIZE_MAX = 4096;

//CALLBACK FUNCTION FOR PROGRESS
typedef int (*P21_CALLBACK) (int port , int Progress);
typedef int (*P21_CALLBACKCHAR) (int port , char* Data);

class CmdCtrl : public CNeptuneGeneralController
{
public:
	CmdCtrl();
	virtual ~CmdCtrl();
	DISALLOW_COPY_AND_ASSIGN(CmdCtrl);

	virtual int InitDevice(int port);
	virtual int FreeDevice(int port);
	virtual int GeneralCmd(int port, int timeout_ms, byte* pkt_cmd);
	virtual int GeneralCmdOut(  int port, int timeout_ms, byte* pkt_cmd, char* out_buf) ;
	virtual int GeneralCmdIn(   int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size);
	virtual int GeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf);
	//110420 gilpin add for receive serial command such as touch panel pixel and key value
    virtual int ReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf);

private:
	bool m_isOnceCmd;
	bool m_isOpen;
	BaseDrv* m_pCommDrv;

public:
	int open(int port, int timeout_ms);
	int close();
	int ExactReadDevice(int port, int timeout_ms, char* in_buf, int in_size);
	int ExactWriteDevice(int port, int timeout_ms, char* in_buf, int in_size);

private:
	int ExactGeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf);
	//110420 gilpin add for receive serial command such as touch panel pixel and key value
	int ExactReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf) ;
	int PacketResultCheck(int pkt_result, byte* out_buf);

public:
	int NormalCmdPowerOnToUARTPacketMode(int port, int timeout_ms, int DTRFlag);

	virtual int UnregisterCallBack(void* farCallBack);
	virtual int RegisterCallBack(void* farCallBack);
	int UnregisterCallBackChar(void* farCallBack);
	int RegisterCallBackChar(void* farCallBack);

	int EnableDTR(int port, int timeout_ms);
	int DisableDTR(int port, int timeout_ms);

public:
	int ComDtrCtrl(int port, int timeout_ms, bool isEnable);
	int ComRtsCtrl(int port, int timeout_ms, bool isEnable);
	int ComTxCtrl(int port, int timeout_ms, byte* pBuffer, size_t nLength);

private:
	P21_CALLBACK m_farCallBack;
	// 2006-11-15 YTT
	P21_CALLBACKCHAR m_farCallBackChar;

	int QthGenPacketSend(byte type, byte* pkt_cmd, byte* payload, size_t payload_size, byte* checksum_result);
	int QthGenPacketRecv(byte type, byte* pkt_cmd, byte* out_buf, byte* ack_req_checksum, int timeout_ms);
	int QthGenCommand(byte* pkt_cmd, byte* payload, size_t payload_size, byte* out_buf, int timeout_ms);
	//110420 gilpin add for receive serial command such as touch panel pixel and key value
	int RecvCommand(byte* pkt_cmd, byte* out_buf, int timeout_ms);
};

#endif // !defined(AFX_CmdCtrl_H__7C100BB3_1FAE_4BE3_A1C2_531317A625A5__INCLUDED_)
