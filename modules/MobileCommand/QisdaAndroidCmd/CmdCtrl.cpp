#include "stdafx.h"
#include <fstream>
#include <cstddef> // size_t
#include <ctime>

#include "CmdCtrl.h"
#include "Utility.h"
#include "QisdaAndroidCmdCtrl.h"
#include "CmdType.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CmdCtrl::CmdCtrl()
{
	m_isOpen = false;
	m_isOnceCmd = false;
	m_pCommDrv = NULL;
	//FF:初始化m_farCallBackChar 12.18
	m_farCallBackChar = NULL;
}

CmdCtrl::~CmdCtrl()
{
	if (m_pCommDrv != NULL)
	{
		delete m_pCommDrv;
		m_pCommDrv = NULL;
	}
}

int CmdCtrl::open(int port, int timeout_ms)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	TRACE(_T("%s\n"), __FUNCTION__);
	//如果object是開著的就別去close
	int tryCounts = 0;
	TRACE(_T("m_pCommDrv->open:%d(%d)\n"), port, tryCounts);
	while (m_pCommDrv->open(port, 115200, NOPARITY, ONESTOPBIT, 8) < 0)
	{
		tryCounts++;
		TRACE(_T("m_pCommDrv->open:%d(%d)\n"), port, tryCounts);
		if (tryCounts > 10)   // only retry 10 times
			return HS_TIMEOUT;
		Sleep(port >= QTH_PORT_TCP ? 0 : 10); // USB 0ms, UART 10ms
	}
	if (!m_isOnceCmd)
		m_isOpen = true;

	return HS_OK;
}

int CmdCtrl::close()
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	m_pCommDrv->close();
	if (!m_isOnceCmd)
		m_isOpen = false;

	return HS_OK;
}

int CmdCtrl::ExactReadDevice(int port, int timeout_ms, char* InBuffer, int InSize)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	if (m_pCommDrv->read((unsigned char*)InBuffer, InSize, timeout_ms) != InSize)
		return HS_READ_DATA_FAIL;

	return HS_OK;
}

int CmdCtrl::ExactWriteDevice(int port, int timeout_ms, char* InBuffer, int InSize)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	if (m_pCommDrv->write((unsigned char*)InBuffer, InSize) != InSize)
		return HS_WRITE_DATA_FAIL;

	return HS_OK;
}

// TODO: automatic initial
int CmdCtrl::InitDevice(int port)
{
	if (m_pCommDrv == NULL)
	{
		if (port <= QTH_PORT_COM_MAX)
			m_pCommDrv = new SerialDrv();
		else if (port == QTH_PORT_USB)
			m_pCommDrv = new UsbDrv();
		else if (port >= QTH_PORT_TCP)
			m_pCommDrv = new TcpDrv();
	}
	return HS_OK;
}

int CmdCtrl::FreeDevice(int port)
{
	if (m_pCommDrv != NULL)
	{
		m_pCommDrv->close();
		delete m_pCommDrv;
		m_pCommDrv = NULL;
	}
	return HS_OK;
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value
int CmdCtrl::ReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	m_isOnceCmd = true;
	int ret_code = ExactReceiveCmdOut(port, timeout_ms, (byte*)pkt_cmd, out_buf);
	m_isOnceCmd = false;
	if (ret_code != HS_OK)
	{
		TRACE(_T("%s ExactReceiveCmdOut(%d) fail\r\n"), __FUNCTION__, ret_code);
		TRACER_STREAM(__FUNCTION__ << " ExactReceiveCmdOut port:" << port << " code:" << ret_code << "\r\n");
		return ret_code + ERR_GENERALCMDOUT;
	}
	return HS_OK;
}

int CmdCtrl::GeneralCmd(int port, int timeout_ms, byte* pkt_cmd)
{
	return GeneralCmdInOut(port, timeout_ms, pkt_cmd, NULL, 0, NULL);
}

int CmdCtrl::GeneralCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf)
{
	return GeneralCmdInOut(port, timeout_ms, pkt_cmd, NULL, 0, out_buf);
}

int CmdCtrl::GeneralCmdIn(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size)
{
	return GeneralCmdInOut(port, timeout_ms, pkt_cmd, in_buf, in_size, NULL);
}

int CmdCtrl::GeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	m_isOnceCmd = true;
	int ret_code = HS_FAIL;
	if (!m_isOpen)
	{
		ret_code = open(port, timeout_ms);
		if (ret_code != HS_OK)
			return ERR_OPENDEVICE + ret_code;
	}
	ret_code = ExactGeneralCmdInOut(port, timeout_ms, (byte*)pkt_cmd, in_buf, in_size, out_buf);
	if (ret_code != HS_OK)
	{
		TRACE(_T("%s ExactGeneralCmdInOut(%d) fail\r\n"), __FUNCTION__, ret_code);
		close();
		return ret_code + ERR_GENERALCMDINOUT;
	}
	if (!m_isOpen)
		close();
	m_isOnceCmd = false;

	return HS_OK;
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value
int CmdCtrl::RecvCommand(byte* pkt_cmd, byte* out_buf, int timeout_ms)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	int pkt_result = HS_FAIL;
	byte buffer[PAYLOAD_SIZE_MAX];
	memset(buffer, 0, sizeof(buffer));
	while (1)
	{
		pkt_result = QthGenPacketRecv(DT_PKT_RESULT, pkt_cmd, buffer, NULL, timeout_ms);
		int ret_code = PacketResultCheck(pkt_result, buffer);
		if (ret_code == HS_OK)
		{
			memcpy(out_buf, (buffer + 1), PAYLOAD_SIZE_MAX * sizeof(byte) - 1);
			return HS_OK;
		}
		else if (ret_code != HS_PROGRESS)
		{
			TRACE(_T("%s PacketResultCheck(%d) fail\r\n"), __FUNCTION__, ret_code);
			TRACER_STREAM(__FUNCTION__ << " PacketResultCheck code:" << ret_code << "\r\n");
			return HS_FAIL;
		}
	}
	return HS_FAIL;
}

int CmdCtrl::ExactReceiveCmdOut(int port, int timeout_ms, byte* pkt_cmd, char* out_buf)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	int ret_code = RecvCommand(pkt_cmd, (byte*)out_buf, timeout_ms);
	if (ret_code != HS_OK)
	{
		TRACE(_T("%s RecvCommand(%d) fail\r\n"), __FUNCTION__, ret_code);
		TRACER_STREAM(__FUNCTION__ << " RecvCommand port:" << port << " code:" << ret_code << "\r\n");
		return ret_code;
	}
	return HS_OK;
}

int CmdCtrl::ExactGeneralCmdInOut(int port, int timeout_ms, byte* pkt_cmd, char* in_buf, size_t in_size, char* out_buf)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	byte buffer[PAYLOAD_SIZE_MAX];
	memset(buffer, 0, sizeof(buffer));
	int ret_code = QthGenCommand(pkt_cmd, (byte*)in_buf, in_size, buffer, timeout_ms);
	if (ret_code != HS_OK)
	{
		TRACER_STREAM("QthGenCommand(" << ret_code << ") fail\r\n");
		TRACE(_T("%s QthGenCommand(%d) fail\r\n"), _T(__FUNCTION__), ret_code);
		return ret_code;
	}
	if (out_buf != NULL)
		memcpy(out_buf, buffer, sizeof(buffer));

	return ret_code;
}

int CmdCtrl::PacketResultCheck(int pkt_result, byte* out_buf)
{
	if (pkt_result == HS_PROGRESS)
	{
		return HS_PROGRESS;
	}
	else if (pkt_result != HS_OK)
	{
		TRACER_STREAM(" =>recv packet fail:" << (char*)out_buf << "\r\n");
		return pkt_result;
	}

	if (out_buf[0] == '1')
	{
		return HS_OK;
	}
	else if (out_buf[0] == '0')
	{
		TRACER_STREAM(" =>recv packet fail(0):" << (char*)out_buf << "\r\n");
		return HS_FAIL;
	}
	else
	{
		TRACER_STREAM(" =>recv packet fail(other):" << (char*)out_buf << "\r\n");
		return HS_FAIL;
	}
}

//byte ftd_req[] = {
//	0x42, 0x45, 0x4e, 0x51, // header : BENQ
//    0x10,                   // type   : request
//    0x12, 0x01, 0x00, 0x00, // command: see ftd command list
//    0x00, 0x00, 0x00, 0x00, // payload length = 0
//    0x1b                    // checksum
//};
//byte ftd_ack[] = {
//	0x42, 0x45, 0x4e, 0x51, // header : BENQ
//    0x11,                   // type   : ack
//    0x12, 0x01, 0x00, 0x00, // command: the same with request
//    0x00, 0x00, 0x00, 0x01, // payload length = 1
//	0x1b,                   // payload: request checksum
//	0x00                    // checksum: ack checksum must be 0
//};
//byte ftd_res[] = {
//	0x42, 0x45, 0x4e, 0x51, // header : BENQ
//	0x13,                   // type   : response
//	0x12, 0x01, 0x00, 0x00, // command: the same with request
//	0x00, 0x00, 0x00, 0x09, // payload length = 9
//	0x31, 0x42, 0x4d, 0x41, 0x32, 0x35, 0x30, 0x2d, 0x33, // payload:1BMA250-3G
//	0x47                    // checksum // response payload first byte is PASS(1), FAIL(0)
//};
//super general function,support GeneralCmd, GeneralCmdIn, GeneralCmdOut, GeneralCmdInOut
int CmdCtrl::QthGenCommand(unsigned char* pkt_cmd, unsigned char* payload, size_t payload_size, unsigned char* out_buf, int timeout_ms)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	byte req_checksum;//request packet checksum
	byte ack_req_checksum;//must be the same with req_checksum
	int pkt_result = QthGenPacketSend(DT_PKT_REQUEST, pkt_cmd, payload, payload_size, &req_checksum);
	if (pkt_result != HS_OK)
	{
		TRACER_STREAM("QthGenPacketSend:" << pkt_result << "\r\n");
		TRACE(_T("%s QthGenPacketSend fail(%d)\n"), __FUNCTION__, pkt_result);
		return pkt_result;
	}

	// ack packet
	byte buffer[PAYLOAD_SIZE_MAX];
	memset(buffer, 0, sizeof(buffer));
	pkt_result = QthGenPacketRecv(DT_PKT_ACK, pkt_cmd, buffer, &ack_req_checksum, timeout_ms);
	if (pkt_result != HS_OK)
	{
		TRACER_STREAM("QthGenPacketRecv ack:" << pkt_result << "\r\n");
		TRACE(_T("%s QthGenPacketRecv ack fail(%d)\n"), __FUNCTION__, pkt_result);
		return pkt_result;
	}
	//再來這邊應該要check一下chksum吧
	//double check checksum
	//level 2: check the sender and the receiver
	//剛剛在收封包的時候就檢查過一次了 最底層
	//現在則是檢查 送封包 跟 收封包 的地方 所以YT才會叫做Level2吧
	if (req_checksum != ack_req_checksum)
	{
		TRACER_STREAM("checksum fail\r\n");
		TRACE(_T("%s checksum fail(%x != %x)\r\n"), __FUNCTION__, req_checksum, ack_req_checksum);
		return HS_INVALID_CHECKSUM;
	}

	//// 2008-02-25 Nek Wang: receive result
	int ret_code = HS_FAIL;
	for (;;)
	{
		memset(buffer, 0, sizeof(buffer));
		pkt_result = QthGenPacketRecv(DT_PKT_RESULT, pkt_cmd, buffer, NULL, timeout_ms);
		if (pkt_result != HS_OK)
		{
			TRACER_STREAM("QthGenPacketRecv result:" << pkt_result << "\r\n");
			TRACE(_T("%s QthGenPacketRecv result fail(%d)\n"), __FUNCTION__, pkt_result);
			return pkt_result;
		}
		ret_code = PacketResultCheck(pkt_result, buffer);
		if (ret_code == HS_OK)
		{
			memcpy(out_buf, (buffer + 1), PAYLOAD_SIZE_MAX - 1);
			return ret_code;
		}
		else if (ret_code != HS_PROGRESS)
		{
			//ytt 134029 copy result if send cmd is ok but the cmd result fail
			if (buffer[0] == '0') //result fail
				memcpy(out_buf, (buffer + 1), PAYLOAD_SIZE_MAX - 1);
			return ret_code;
		}
	}
	return ret_code;
}

static inline void endian_swap(size_t& x)
{
	x = (x >> 24) |
	    ((x << 8) & 0x00FF0000) |
	    ((x >> 8) & 0x0000FF00) |
	    (x << 24);
}

int CmdCtrl::QthGenPacketSend(byte type, byte* pkt_cmd, byte* payload, size_t payload_size, byte* checksum_result)
{
	if (type != DT_PKT_REQUEST && type != P21_DATADL_REQUEST && type != P21_DATAUL_REQUEST)
	{
		return HS_FAIL;
	}

	const unsigned char PKT_HEADER[4] = {'B', 'E', 'N', 'Q'};
	size_t endian_swap_payload_size = payload_size;
	endian_swap(endian_swap_payload_size);

	byte packet[PAYLOAD_SIZE_MAX + 64];
	memset(&packet, 0, sizeof(packet));
	//檢查
	//1.packdata是否有比temp長,ex超過設定上限
	//2.注意packetsize是否比packdata大
	memcpy(packet, PKT_HEADER, DT_PKT_HEADER_SIZE);  // packet header -> "BENQ"
	packet[4] = type;              // P21 command type
	memcpy(packet + 5, pkt_cmd, 4);// command or block number
	memcpy(packet + 9, &endian_swap_payload_size, 4);// payload size
	memcpy(packet + 13, payload, payload_size);      // payload data
	int packet_bytes = DT_PKT_HEADER_SIZE + 1 + 4 + 4 + payload_size;

	byte checksum = packet[0];
	for (int j = 1; j < packet_bytes; j++)
	{
		checksum ^= packet[j];
	}
	*checksum_result = checksum;
	packet[packet_bytes] = checksum;
	packet_bytes++; //add chcksum

	int send_blocks = 0;
	int write_bytes = DT_PKT_BLOCK_SIZE;
	int queue_bytes = packet_bytes;
	while (queue_bytes > 0)
	{
		TRACE("%s queue bytes: %d\n", __FUNCTION__, queue_bytes);
		write_bytes = (queue_bytes < write_bytes) ? queue_bytes : DT_PKT_BLOCK_SIZE;
		if (m_pCommDrv->write(&packet[send_blocks * DT_PKT_BLOCK_SIZE], write_bytes) != write_bytes)
		{
			TRACER_STREAM("!" << __FUNCTION__ << ",write:" << write_bytes << "\r\n");
			TRACE(_T("%s write:%d\r\n"), __FUNCTION__, write_bytes);
			return HS_WRITE_DATA_FAIL;
		}
		send_blocks++;
		queue_bytes -= write_bytes;
		Sleep(0);
	}
	return HS_OK;
}

int CmdCtrl::QthGenPacketRecv(byte type, byte* pkt_cmd, byte* out_buf, byte* ack_req_checksum, int timeout_ms)
{
	// 外面:  while (depackage()) 如果回傳是true就一值解封包或
	// 一直接收檔案,直到抓到指令,如果timeout 就直接結束
	// 1.收到封包後解譯,並將結果利用packdata陣列傳出去
	// 1.1 function
	byte read_buf[4] = {0};
	byte packet[PAYLOAD_SIZE_MAX + 64];
	memset(packet, 0, sizeof(packet));
	TRACE("check packet header \"BENQ\" ...\n");
	while (1)
	{
		//嘗試接收第一個byte, 並辨認是否為指定Function的封包header
		//因為m_pCommDrv有timeout機制,所以...不需要自己設定timeout
		//[B]ENQ
		if (m_pCommDrv->read(read_buf, 1, timeout_ms) != 1)
		{
			TRACER_STREAM("QthGenPacketRecv while B, timeout:" << timeout_ms << "\r\n");
			return HS_READ_DATA_FAIL;
		}
		if (read_buf[0] != 'B')
		{
			continue;
		}
		packet[0] = read_buf[0];
		TRACE("[B]"); //[B]ENQ
		//B[E]NQ
		if (m_pCommDrv->read(read_buf, 1, 300) != 1)
		{
			TRACER_STREAM("QthGenPacketRecv while E\r\n");
			return HS_READ_DATA_FAIL;
		}
		if (read_buf[0] != 'E')   //FF: try to read E
		{
			/*2012.5.10. Lion Fix Double 'B' Bug -> B B E N Q*/
			int isSecondB = 0;
			if (read_buf[0] == 'B')   // B[B]E N Q
			{
				isSecondB = 1;
				if (m_pCommDrv->read(read_buf, 1, 300) != 1)
				{
					TRACER_STREAM("QthGenPacketRecv while E\r\n");
					return HS_READ_DATA_FAIL;
				}
				if (read_buf[0] != 'E')   // BB[E]NQ
				{
					continue;
				}
			}
			if (isSecondB == 0)
			{
				continue;
			}
			/*Lion Fix Double 'B' Bug -> B B E N Q*/
		}
		packet[1] = read_buf[0];
		TRACE("[E]"); //B[E]NQ
		//BE[N]Q
		if (m_pCommDrv->read(read_buf, 1, 300) != 1)
		{
			TRACER_STREAM("QthGenPacketRecv while N\r\n");
			return HS_READ_DATA_FAIL;
		}
		if (read_buf[0] != 'N')   //FF: try to read N
		{
			continue;
		}
		packet[2] = read_buf[0];
		TRACE("[N]"); //BE[N]Q
		//BEN[Q]
		if (m_pCommDrv->read(read_buf, 1, 300) != 1)
		{
			TRACER_STREAM("QthGenPacketRecv while Q\r\n");
			return HS_READ_DATA_FAIL;
		}
		if (read_buf[0] != 'Q')   //FF: try to read Q
		{
			continue;
		}
		packet[3] = read_buf[0];
		TRACE("[Q], ok get command type"); //BEN[Q], header ok, get command type

		if (m_pCommDrv->read(read_buf, 1, 300) != 1)
		{
			TRACE(_T("BENQ packet data corruption\n"));
			TRACER_STREAM("QthGenPacketRecv BENQ packet data corruption: type\r\n");
			return HS_READ_DATA_FAIL;
		}
		packet[4] = read_buf[0];
		TRACE(_T(", type: 0x%x\n"), read_buf[0]);

		//判斷是是否要收的cmd或是progress
		//如果是progress的話也要收下封包內容後傳回去
		bool progress_flag = false;
		if (read_buf[0] == DT_PKT_PROGRESS)
		{
			progress_flag = true;
		}
		if (read_buf[0] == type || progress_flag)
		{
			//要收DataSize+1 個,因為最後還有1個checksum
			//FF:Get command or block number (DL or UL function)
			if (m_pCommDrv->read(read_buf, 4, 300) != 4)
			{
				TRACER_STREAM("QthGenPacketRecv BENQ packet data corruption: command\r\n");
				return HS_READ_DATA_FAIL;
			}
			memcpy(packet + 5, read_buf, 4);

			//FF:get payloadsize
			if (m_pCommDrv->read(read_buf, 4, 300) != 4)
			{
				TRACER_STREAM("QthGenPacketRecv BENQ packet data corruption: payloadsize\r\n");
				return HS_READ_DATA_FAIL;
			}
			size_t payload_size = 0;
			memcpy(packet + 9, read_buf, 4);
			memcpy(&payload_size, read_buf, 4);
			endian_swap(payload_size);
			TRACE("%s payload size: %d\n", __FUNCTION__, payload_size);
			int read_bytes = m_pCommDrv->read(packet + 13, payload_size + 1, 5000);
			if (read_bytes != payload_size + 1)
			{
				TRACER_STREAM("!" << __FUNCTION__ << ",nRead:" << read_bytes << "\r\n");
				TRACE("%s packet read size fail(%d!=%d)\n", __FUNCTION__, read_bytes, payload_size);
				return HS_READ_DATA_FAIL;
			}

			byte checksum = packet[0];
			//check whole packet checksum
			for (size_t i = 1; i < 9 + 4 + payload_size; i++)
			{
				checksum ^= packet[i];
			}
			//double check checksum這裡是拿pc tool這邊算出來的checksum來跟DT傳過來的比
			//level 1: check whole packet
			if (checksum != packet[9 + 4 + payload_size])
			{
				TRACER_STREAM("!" << __FUNCTION__ << ",PacketChecksum:" << checksum << "\r\n");
				TRACE("%s packet checksum fail\n", __FUNCTION__);
				return HS_INVALID_CHECKSUM;
			}
			//還要檢查DT傳回來的cmd是否是之前PC tool傳過去的command
			for (int i = 0; i < 4; i++)
			{
				if (packet[i + 5] != pkt_cmd[i])
				{
					TRACER_STREAM("!" << __FUNCTION__ << ",command[" << i << "]:"
					              << packet[i + 5] << "!=" << pkt_cmd[i] << "\r\n");
					TRACE("%s check command fail(%d): %d != %d\n", __FUNCTION__, i, packet[i + 5], pkt_cmd[i]);
					return HS_READ_DATA_FAIL;
				}
			}

			// drop out checksum
			// last packet checksum in index 9+5 if its a ack packet
			if (ack_req_checksum != NULL)
			{
				*ack_req_checksum = packet[9 + 4 + payload_size - 1];
			}
			//copy data to temp, 只有在夾帶raw data時才需要
			memcpy(out_buf, &packet[13], payload_size);
			if (progress_flag)
			{
				TRACE("get \"BENQ\" progress\n");
				return HS_PROGRESS;
			}
			else
			{
				TRACE("get \"BENQ\" command\n");
				return HS_OK;
			}
		} // if (type == true) end
	} // whole pack ok?
	//FFYang 0927

	TRACE("check packet header \"BENQ\" FAIL\n");
	return HS_FAIL;
}

int CmdCtrl::NormalCmdPowerOnToUARTPacketMode(int port, int timeout_ms, int BMS_flag)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	unsigned char pPackData[5] = {'A', 'A', 'A', 'A', 'A'};
	unsigned char PackDataA = 'A';
	int ret_code = 0;
	//如果收到三個z就是成功
	int power_to_uart_packet_mode_counter = 0;
	bool recieve_continuous_flag = false;
	int time_counter = 0;
	int sleep_time = 20;

	//一直收 怕有雜訊 收到沒有 或者是有三個連續的zzz就return
	if (BMS_flag == 1)
	{
		ret_code = EnableDTR(port, timeout_ms);
		if (!m_isOpen)
		{
			ret_code = open(port, timeout_ms);
			if (ret_code != HS_OK)
				return ret_code + ERR_OPENDEVICE;
		}
	}
	else if (!m_isOpen)
	{
		m_isOnceCmd = true;
		ret_code = open(port, timeout_ms);
		if (ret_code != HS_OK)
			return ret_code + ERR_OPENDEVICE;
	}

	// YT 2006-11-10 Modify for bala test
	while (time_counter < 2500)
	{
		// YTT 2006-11-10 move to top for continue control
		time_counter = time_counter + sleep_time;
		Sleep(sleep_time);

		//// send 5 'A'
		if (m_pCommDrv->write(pPackData, 5) != 5)
			return HS_WRITE_DATA_FAIL + ERR_NORMAL_COMMAND;

		// YTT 2006-11-10 clear control
		//skip all dirty data
		//only if get 'Z' or timeout can pass the phase
		//the code is for temp only
		//FF:有收到 檢查為Z 才跳出
		PackDataA = 0;
		while (m_pCommDrv->read(&PackDataA, sizeof(PackDataA), 100) == 1)
			if (PackDataA == 'Z') break;

		//FF:上面的收訊息 timeout 我覺得好像不需要耶...下面就會判斷了
		//FF:而且最上面會有timeout count
		if (PackDataA == 0)
			continue;
		if (PackDataA == 'Z')
		{
			if (recieve_continuous_flag)   //true代表上一次也是A所以累加
				power_to_uart_packet_mode_counter++;
			else   //false代表上一次不是是A所以=1
				power_to_uart_packet_mode_counter = 1;
			// 收到 Z flag 設為 true, 三個 Z 成功
			recieve_continuous_flag = true;
			if (power_to_uart_packet_mode_counter == 3)
			{
				Sleep(500); // sleep 500 ms asked by eric and return
				if (BMS_flag == 1)
				{
					ret_code = DisableDTR(port, timeout_ms);
					if (!m_isOpen)
					{
						close();
						m_isOnceCmd = false;
					}
				}
				else if (!m_isOpen)
				{
					close();
					m_isOnceCmd = false;
				}
				return HS_OK;
			}
		}
		else   // 收到不是A flag設為false
		{
			recieve_continuous_flag = false;
		}
	}
	if (!m_isOpen)
	{
		m_isOnceCmd = false;
		close();
	}

	return ERR_NORMAL_COMMAND;
}

int CmdCtrl::RegisterCallBack(void* farCallBack)
{
	m_farCallBack = (P21_CALLBACK)farCallBack;
	char cBlockOut[20];
	memset(cBlockOut, 0, sizeof(cBlockOut));
	wsprintf(cBlockOut, "%x", (void*)farCallBack);

	memset(cBlockOut, 0, sizeof(cBlockOut));
	wsprintf(cBlockOut, "%x", (void*)m_farCallBack);

	return HS_OK;
}

int CmdCtrl::UnregisterCallBack(void* farCallBack)
{
	m_farCallBack = NULL;
	return HS_OK;
}

int CmdCtrl::RegisterCallBackChar(void* farCallBack)
{
	m_farCallBackChar = (P21_CALLBACKCHAR)farCallBack;
	char cBlockOut[20];
	memset(cBlockOut, 0, sizeof(cBlockOut));
	wsprintf(cBlockOut, "%x", (void*)farCallBack);

	memset(cBlockOut, 0, sizeof(cBlockOut));
	wsprintf(cBlockOut, "%x", (void*)m_farCallBack);

	return HS_OK;
}

int CmdCtrl::UnregisterCallBackChar(void* farCallBack)
{
	m_farCallBackChar = 0;
	return HS_OK;
}

int CmdCtrl::EnableDTR(int port, int TimeOut)
{
	HANDLE hCom;
	if (!m_isOpen)
	{
		m_isOnceCmd = true;
		CString comport;
		comport.Format("COM%d", port);
		hCom = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	{
		hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	}

	DCB dcb;
	if (!GetCommState(hCom, &dcb))
	{
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	if (!SetCommState(hCom, &dcb))
	{
		return HS_SET_DEVICE_FAIL;
	}
	if (!SetupComm(hCom, 1600, 1600))
	{
		return HS_SET_DEVICE_FAIL;
	}

	Sleep(2);
	if (!m_isOpen)
	{
		m_isOnceCmd = false;
		CloseHandle(hCom);
	}

	return HS_OK;
}

int CmdCtrl::DisableDTR(int port, int TimeOut)
{
	HANDLE hCom;
	if (!m_isOpen)
	{
		m_isOnceCmd = true;
		CString comport;
		comport.Format("COM%d", port);
		hCom = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	{
		hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	}
	DCB dcb;
	if (!GetCommState(hCom, &dcb))
	{
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb))
	{
		return HS_SET_DEVICE_FAIL;
	}
	if (!SetupComm(hCom, 1600, 1600))
	{
		return HS_SET_DEVICE_FAIL;
	}

	if (!m_isOpen)
	{
		m_isOnceCmd = false;
		CloseHandle(hCom);
	}

	return HS_OK;
}

int CmdCtrl::ComDtrCtrl(int port, int TimeOut, bool isEnable)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	int ret_code = HS_FAIL;
	if (!m_isOpen)
	{
		ret_code = open(port, TimeOut);
		if (ret_code != HS_OK)
			return ret_code + ERR_OPENDEVICE;
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb))
		return HS_GET_DEVICE_FAIL;
	dcb.fDtrControl = isEnable ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb))
		return HS_SET_DEVICE_FAIL;
	return HS_OK;
}

int CmdCtrl::ComRtsCtrl(int port, int timeout_ms, bool isEnable)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	int ret_code = HS_FAIL;
	if (!m_isOpen)
	{
		ret_code = open(port, timeout_ms);
		if (ret_code != HS_OK)
			return ret_code + ERR_OPENDEVICE;
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb))
		return HS_GET_DEVICE_FAIL;
	dcb.fRtsControl = isEnable ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb))
		return HS_SET_DEVICE_FAIL;
	return HS_OK;
}

int CmdCtrl::ComTxCtrl(int port, int timeout_ms, byte* pBuffer, size_t nLength)
{
	if (m_pCommDrv == NULL) return HS_NOT_INITIALIZE;

	int ret_code = HS_FAIL;
	if (!m_isOpen)
	{
		ret_code = open(port, timeout_ms);
		if (ret_code != HS_OK)
			return ret_code + ERR_OPENDEVICE;
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb))
		return HS_GET_DEVICE_FAIL;
	dcb.BaudRate = BAUD_2400;
	if (!SetCommState(hCom, &dcb))
		return HS_SET_DEVICE_FAIL;
	if (m_pCommDrv->write(pBuffer, static_cast<U32>(nLength)) != nLength)
		return HS_WRITE_DATA_FAIL + ERR_NORMAL_COMMAND;
	return HS_OK;
}
