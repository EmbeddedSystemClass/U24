// CmdCtrl.cpp: implementation of the CmdCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <fstream>
#include <cstddef> // size_t

#include "CmdCtrl.h"
#include "Utility.h"
#include "NeptuneCtrl.h"
#include "CmdType.h"
#include "API/NeptuneDLLApi.h" // 2008-01-18 Nek: For Write Download Info
#include "API/P21Type.h"

//#define SERV_PORT 9599

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
	m_isGeneralCmd = false;
	m_bIfUSBCmd = false;
	m_pCommDrv = NULL;
	m_bP21LPR2PhaseOne = false;
	//FF:初始化m_farCallBackChar 12.18
	m_farCallBackChar = NULL;
	memset(m_UIInfo, 0, sizeof(m_UIInfo));
	strncpy(m_UIInfo, "DEFA00", 6);
}

CmdCtrl::~CmdCtrl()
{
	if (m_pCommDrv != NULL) {
		delete m_pCommDrv;
		m_pCommDrv = NULL;
	}
}

int CmdCtrl::open(int nPort, int nTimeout)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int nCount = 0;
	/*110610 Gilpin.Xi Add for test Socket connect Start*/
	int nRetCode = HS_FAIL;
	//CString cstrMessage;
	/*110610 Gilpin.Xi Add for test Socket connect End*/
	//如果object是開著的就別去close
	while ((nRetCode = m_pCommDrv->open(nPort, 115200, NOPARITY, ONESTOPBIT, 8)) != 0) {
		/*110610 Gilpin.Xi Add for test Socket connect Start*/
		//cstrMessage.Format("%d", nRetCode);
		//::AfxMessageBox(cstrMessage);
		/*110610 Gilpin.Xi Add for test Socket connect End*/
		nCount++;
		if (nCount > nTimeout / 100) {
			return HS_TIMEOUT;
		}
		Sleep(11);
	}
	if (m_isGeneralCmd) {
		m_isOpen = true;
	}
	return HS_OK;
}

int CmdCtrl::close(int nPort, int nTimeout)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_pCommDrv->close();
	if (m_isGeneralCmd) {
		m_isOpen = false;
	}

	return HS_OK;
}

int CmdCtrl::ExactReadDevice(int nPort, int nTimeout, char* InBuffer, int InSize)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	if (m_pCommDrv->read((unsigned char *)InBuffer, InSize, nTimeout) != InSize) {
		return HS_READ_DATA_FAIL;
	}
	return HS_OK;
}

int CmdCtrl::ExactWriteDevice(int nPort, int nTimeout, char* InBuffer, int InSize)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	if (m_pCommDrv->write((unsigned char *)InBuffer, InSize) != InSize) {
		return HS_WRITE_DATA_FAIL;
	}
	return HS_OK;
}

// TODO: automatic initial
int CmdCtrl::InitDevice(int nPort)
{
	if (m_pCommDrv == NULL) {
		if (nPort <= QTH_PORT_COM_MAX) {
			m_pCommDrv = new SerialDrv();
		}
		else if(nPort == QTH_PORT_USB) {
			m_pCommDrv = new UsbDrv();
		}
		else if(nPort == QTH_PORT_TCP) {
			m_pCommDrv = new TcpDrv();
		}
	}
	return HS_OK;
}

int CmdCtrl::FreeDevice(int nPort)
{
	/* 2011/05/23 Alex.Chen Modify Start */
	//if (m_pCommDrv != NULL) {
	//	m_pCommDrv->close();
	//}
	//delete m_pCommDrv;
	//m_pCommDrv = NULL;
	if (m_pCommDrv != NULL) {
		delete m_pCommDrv;
		m_pCommDrv = NULL;
	}
	/* 2011/05/23 Alex.Chen Modify End */

	return HS_OK;
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value 
int CmdCtrl::ReceiveCmdOut(int nPort, int nTimeout, char* CtrlBuffer, char* OutBuffer)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	int nRetCode = HS_FAIL;
	/*110610 Gilpin.Xi delete for not open socket while receive touch point Start*/
	//if (!m_isOpen) {
	//	nRetCode = open(nPort, nTimeout);
	//	if (nRetCode != HS_OK) {
	//		return P21_ERR_OPENDEVICE + nRetCode;
	//	}
	//}
	/*110610 Gilpin.Xi delete for not open socket while receive touch point End*/
	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	cGenPayload[0] = CtrlBuffer[0];
	cGenPayload[1] = CtrlBuffer[1];
	cGenPayload[2] = CtrlBuffer[2];
	cGenPayload[3] = CtrlBuffer[3];
	nRetCode = ExactReceiveCmdOut(nPort, nTimeout, CtrlBuffer, OutBuffer);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactReceiveCmdOut(%d) fail\r\n"), __FUNCTION__, nRetCode);
		/*110610 Gilpin.Xi delete for not close socket while receive touch point Start*/
		//close(nPort, nTimeout);
		/*110610 Gilpin.Xi delete for not close socket while receive touch point End*/
		return nRetCode + P21_ERR_GENERALCMDOUT; // FIXME
	}
	/*110610 Gilpin.Xi delete for not close socket while receive touch point Start*/
	//if (m_isOpen) {
	//	close(nPort, nTimeout);
	//}
	/*110610 Gilpin.Xi delete for not close socket while receive touch point End*/
	m_isGeneralCmd = false;

	return HS_OK;
}

int CmdCtrl::GeneralCmd(int nPort, int nTimeout, char* CtrlCmd)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	//Open Device
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}
	nRetCode = ExactGeneralCmd(nPort, nTimeout, CtrlCmd);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactGeneralCmd(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_GENERALCMD;
	}
	if (m_isOpen) {
		close(nPort, nTimeout);
	}
	m_isGeneralCmd = false;

	return HS_OK;
}

int CmdCtrl::GeneralCmdOut(int nPort, int nTimeout, char* CtrlBuffer, char* OutBuffer)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return P21_ERR_OPENDEVICE + nRetCode;
		}
	}
	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	cGenPayload[0] = CtrlBuffer[0];
	cGenPayload[1] = CtrlBuffer[1];
	cGenPayload[2] = CtrlBuffer[2];
	cGenPayload[3] = CtrlBuffer[3];
	nRetCode = ExactGeneralCmdOut(nPort, nTimeout, CtrlBuffer, OutBuffer);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactGeneralCmdOut(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);

		if(nRetCode == HS_EMPTY)
			return HS_EMPTY;
		else
		return nRetCode + P21_ERR_GENERALCMDOUT; // FIXME
	}
	if (m_isOpen) {
		close(nPort, nTimeout);
	}
	m_isGeneralCmd = false;

	return HS_OK;
}

int CmdCtrl::GeneralCmdIn(int nPort, int nTimeout, char* CtrlBuffer, char* InBuffer, size_t nInLength)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	//Open Device
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return P21_ERR_OPENDEVICE + nRetCode; // composited command, should be
		}
	}

	nRetCode = ExactGeneralCmdIn(nPort, nTimeout, CtrlBuffer, InBuffer, nInLength);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactGeneralCmdIn(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_GENERALCMDIN; // FIXME: too many exit way
	}

	if (m_isOpen) {
		close(nPort, nTimeout);
	}
	m_isGeneralCmd = false;

	return HS_OK;
}

int CmdCtrl::GeneralCmdInOut(int nPort, int nTimeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	int nRetCode = HS_FAIL;

	/***********110608 gilpin add for socket connect one test item once start************/
	//if((CtrlBuffer[0] == NV_DT_TOUCH)&&(CtrlBuffer[0] == 1)&&(InBuffer[0] == '0'))
	//	m_isOpen = true;
	/***********110608 gilpin add for socket connect one test item once end************/

	if (!m_isOpen) {
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return P21_ERR_OPENDEVICE + nRetCode;
		}
	}

	/***********110608 gilpin add for socket connect one test item once start************/
	if(((CtrlBuffer[0] == NV_DT_TOUCH)&&(CtrlBuffer[1] == 1)&&(InBuffer[0] == '1'))
		||((CtrlBuffer[0] == NV_DT_KEYPAD)&&(CtrlBuffer[1] == 1)&&(InBuffer[0] == '1')))	
	{
		m_isOpen = false;
	}
	/***********110608 gilpin add for socket connect one test item once end************/

	nRetCode = ExactGeneralCmdInOut(nPort, nTimeout, CtrlBuffer, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactGeneralCmdInOut(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_GENERALCMDINOUT;
	}

	if (m_isOpen) {
		Sleep(11);
		close(nPort, nTimeout);
	}
	m_isGeneralCmd = false;

	return HS_OK;
}

int CmdCtrl::GeneralDownload(int nPort, int nTimeout, char* DLType, char* WorkBuffer)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	char szFileSize[256];
	memset(szFileSize, 0, sizeof(szFileSize));
	int nRetCode = HS_FAIL;
	nRetCode = DownloadInfoFileSize(WorkBuffer, szFileSize);
	if (nRetCode == HS_OK) {
		nRetCode = WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, DLType, WorkBuffer, szFileSize, 0);
	}
	if (nRetCode == HS_OK) {
		if (!m_isOpen) {
			m_isGeneralCmd = true;
			nRetCode = open(nPort, nTimeout);
			if (nRetCode != HS_OK) {
				return nRetCode + P21_ERR_OPENDEVICE;
			}
		}
		Sleep(11);
		nRetCode = ExactUSBGenDownloadCmd(nPort, nTimeout, DLType, WorkBuffer);
		if (nRetCode != HS_OK) {
			close(nPort, nTimeout);
			return nRetCode;//composited error control has added in Exact_USBGenDownloadCmd
		}
		if (!m_isOpen) {
			Sleep(11);
			m_isGeneralCmd = false;
			close(nPort, nTimeout);
		}
	}
	if (nRetCode == HS_OK) {
		char szFileSize[256];
		memset(szFileSize, 0, sizeof(szFileSize));
		nRetCode = DownloadInfoFileSize(WorkBuffer, szFileSize);
	}
	if (nRetCode == HS_OK) {
		nRetCode = WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, DLType, WorkBuffer, szFileSize, 1);
	}

	return nRetCode;
}

//The work buffer would be a data buffer or a file path . It just depends on implementation.
int CmdCtrl::GeneralUpload(int nPort, int nTimeout, char* CtrlBuffer, char* WorkBuffer)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}
	nRetCode = ExactUSBGenUploadCmd(nPort, nTimeout, CtrlBuffer, WorkBuffer);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactUSBGenUploadCmd(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_USBGENUPLOADCMD;
	}

	if (!m_isOpen) {
		Sleep(11);
		m_isGeneralCmd = false;
		close(nPort, nTimeout);
	}

	return HS_OK;
}

int CmdCtrl::USBLoad2nd(int nPort, int nTimeout, char* ndFilePath)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char cFile_name[255];
	memset(cFile_name, 0, sizeof(cFile_name));
	Dl_Filehandler file;
	T_FILE_HDR *file_hdr;
	U8 *File_Data;
	DWORD size = 0;
	U8 boot_message[4];
	U8 size_command[4];
	U8 General_Message[4];
	U8 ASIC_ID[ASIC_SIZE_USB];

	boot_message[0] = 0x02;
	boot_message[1] = 0x00;
	boot_message[2] = 0x03;
	boot_message[3] = 0xF0;

	//FF:不能傳null進去,不然會掛點
	if (file.read_file(ndFilePath, &file_hdr) == FAULT) {
		file.free_file_mem(file_hdr);
		return HS_FILE_NOT_FOUND;
	}

	File_Data = (file_hdr->section_hdr[0]).data_ptr;
	size = (file_hdr->section_hdr[0]).sct_size;
	size_command [0] = (byte)((size & 0xFF));
	size_command [1] = (byte)((size & 0xFF00) >> 8);
	size_command [2] = (byte)((size & 0xFF0000) >> 16);
	size_command [3] = (byte)(size >> 24);
	//FF:保留 有可能會用到 1221
	//Try 10 times
	int Count = 0;
	while (m_pCommDrv->open(nPort, 115200, EVENPARITY, ONESTOPBIT, 8) < 0) {
		Count++;
		if (Count > nTimeout / 100) {
			return HS_TIMEOUT;
		}
		Sleep(11);
	}

	// TODO: set timeout here
	memset(ASIC_ID, 0, sizeof(ASIC_ID));
	if (m_pCommDrv->read(ASIC_ID, ASIC_SIZE_USB, 20000) != ASIC_SIZE_USB) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_READ_DATA_FAIL;
	}

	// Send the Flash Command
	if (m_pCommDrv->write(boot_message, sizeof(boot_message)) != sizeof(boot_message)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	if (m_pCommDrv->write(size_command, sizeof(size_command)) != sizeof(size_command)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	if (m_pCommDrv->write(File_Data, size) != (S32)size) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	//because the target change its status, we should close the device to reset USB
	m_pCommDrv->close();

	//-------------------Phase 2-in 2nd Menu ------------------------------------------------------------
	Count = 0;
	while (m_pCommDrv->open(nPort, 115200,/*EVENPARITY*/NOPARITY, ONESTOPBIT, 8) < 0) {
		Sleep(11);
		Count++;
		if (Count > nTimeout / 100) {
			return HS_TIMEOUT;
		}
	}

	//FF:GET Menu Descriptoion for USB
	General_Message[0] = 0x00;
	General_Message[1] = 0xFF;
	General_Message[2] = 0XFF;
	General_Message[3] = 0XFF;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}

	//FF: add by YT ->Remove all dirty data (menu information)
	while (m_pCommDrv->read(General_Message, 1, 500) == 1) {
		General_Message[1] = 0;
	}
	//Send 2 as Load Program
	Sleep(1000);
	General_Message[0] = 0xFF;
	General_Message[1] = 0x00;
	General_Message[2] = 0x00;
	General_Message[3] = 0x00;
	m_pCommDrv->write((unsigned char *)General_Message, 4);

	//GET Menu Descriptoion for USB
	General_Message[0] = 0x00;
	General_Message[1] = 0xFF;
	General_Message[2] = 0XFF;
	General_Message[3] = 0XFF;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}

	//Remove all dirty data (menu information)
	while (m_pCommDrv->read(General_Message, 1, 500) == 1) {
		General_Message[1] = 0;
		if (!(m_farCallBackChar == NULL)) {
			m_farCallBackChar(nPort, (char*)General_Message);
		}
	}
	m_pCommDrv->close();
	file.free_file_mem(file_hdr);

	return HS_OK;
}

// 2006-11-15 YTT
//The Command is based on menu item selection
//EXAMPLE SEND "1" to choose the first meni item
int CmdCtrl::USB2ndSendCommand(int nPort, int nTimeout, char* MenuCommand )
{
	//Try 10 times,注意Parity bit
	int Count = 0;
	while (m_pCommDrv->open(nPort, 115200, NOPARITY, ONESTOPBIT, 8) < 0) {
		Sleep(11);
		Count++;
		if (Count > nTimeout / 100) {
			return HS_TIMEOUT;
		}
	}

	//Get command
	U8 General_Message[4];
	General_Message[0] = 0xFF;
	General_Message[1] = 0x00;
	General_Message[2] = 0x00;
	General_Message[3] = 0x00;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		m_pCommDrv->close();
		return HS_WRITE_DL_PROTOCOL_FAIL;
	}

	General_Message[0] = MenuCommand[0];
	General_Message[1] = 0;
	if (m_pCommDrv->write(General_Message, 1) != 1) {
		m_pCommDrv->close();
		return HS_WRITE_DL_PROTOCOL_FAIL;
	}

	Sleep(11);
	//理論上menu description 馬上出來
	//所以只等一秒
	U8 MsgBuffer[128];
	memset(MsgBuffer, 0, sizeof(MsgBuffer));
	while (m_pCommDrv->read(MsgBuffer, 1, 5000) == 1) {
		if (m_farCallBackChar != NULL) {
			m_farCallBackChar(nPort, (char*)MsgBuffer); //送出menu message
		}
	}
	m_pCommDrv->close();

	return HS_OK;
}


//應該要這樣GeneralCmd 最高級應該能自動open close
//但中間一集如P21GeneralCmd則不需要
//鑑於2ND OPEN的特殊性,應該把它包在SPECIAL CASE中
//意即Load2nd,LoadFile都要能自我開合
int CmdCtrl::USBTILoadFile(int nPort, int nTimeout, char* DDRFileName, unsigned int StartAddr   )
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	Dl_Filehandler file2;
	T_FILE_HDR *file_hdr2;
	U8 *File_Data2;
	U8 Size_Command[4];
	U8 General_Message[4];
	int nRetCode;
	DWORD FileSize = 0;
	//=============================================================================
	//level-2 for flash writer
	//-----------------------------------------------------------------------
	nRetCode = file2.read_file(DDRFileName, &file_hdr2);
	if (nRetCode == FAULT) {
		file2.free_file_mem(file_hdr2);
		return HS_FILE_NOT_FOUND;
	}

	//Try 10 times,注意Parity bit
	int Count = 0;
	while (m_pCommDrv->open(nPort, 115200, NOPARITY, ONESTOPBIT, 8) < 0) {
		Sleep(11);
		Count++;
		if (Count > nTimeout / 100) {
			return HS_TIMEOUT;
		}
	}

	//注意實際要傳時得沏,每個block 大小為0x6000
	File_Data2 = (file_hdr2->section_hdr[0]).data_ptr;
	FileSize = (file_hdr2->section_hdr[0]).sct_size;
	//Create the size command (part of OMAP protocol!)
	// NC - must be little endian for UART - doesn't matter for USB in current devices.
	// but it could matter for future devices.
	//Default Paacket Size 0x00006000 (24 kbytes)
	Size_Command [0] = 0x00;
	Size_Command [1] = 0x60;
	Size_Command [2] = 0x00;
	Size_Command [3] = 0x00;
	DWORD FinalSize = (1024 * 24);
	DWORD InitAddr = StartAddr;

	int BlockNum = (FileSize / (1024 * 24));
	if ((FileSize % (1024*24)) != 0) {
		BlockNum++;
	}

	DWORD NewAddr = InitAddr;
	for (int i = 0; i < BlockNum; i++) {
		//FF: SET ADDRESS
		General_Message[0] = 0x08;
		General_Message[1] = 0xff;
		General_Message[2] = 0xaa;
		General_Message[3] = 0x55;
		if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
			m_pCommDrv->close();
			file2.free_file_mem(file_hdr2);
			return HS_WRITE_DL_PROTOCOL_FAIL;
		}

		//-------------------------------------------------------------------
		//FF: physical Address for each data
		NewAddr = InitAddr + (i * 1024 * 24);
		General_Message[0] = (byte) ((NewAddr & 0xFF));
		General_Message[1] = (byte) ((NewAddr & 0xFF00) >> 8);
		General_Message[2] = (byte) ((NewAddr & 0xFF0000) >> 16);
		General_Message[3] = (byte) (NewAddr >> 24);
		if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
			m_pCommDrv->close();
			file2.free_file_mem(file_hdr2);
			return HS_WRITE_DL_PROTOCOL_FAIL;
		}

		//-------------------------------------------------------------------
		//Write Size Command
		//FF:SET LENGTH
		General_Message[0] = 0x02;
		General_Message[1] = 0x55;
		General_Message[2] = 0xff;
		General_Message[3] = 0xaa;
		if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
			m_pCommDrv->close();
			file2.free_file_mem(file_hdr2);
			return HS_WRITE_DL_PROTOCOL_FAIL;
		}

		//If Final Packet
		if (i == BlockNum - 1) {
			FinalSize = FileSize % (1024 * 24);
			Size_Command[0] = (byte) ((FinalSize & 0xFF));
			Size_Command[1] = (byte) ((FinalSize & 0xFF00) >> 8);
			Size_Command[2] = (byte) ((FinalSize & 0xFF0000) >> 16);
			Size_Command[3] = (byte) (FinalSize >> 24);
		}
		//FF:data length
		if (m_pCommDrv->write(Size_Command, sizeof(Size_Command)) != sizeof(Size_Command)) {
			m_pCommDrv->close();
			file2.free_file_mem(file_hdr2);
			return HS_WRITE_DL_PROTOCOL_FAIL;
		}

		//FF: Send Data
		General_Message[0] = 0x01;
		General_Message[1] = 0x55;
		General_Message[2] = 0xaa;
		General_Message[3] = 0xff;
		if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
			m_pCommDrv->close();
			file2.free_file_mem(file_hdr2);
			return HS_WRITE_DL_PROTOCOL_FAIL;
		}

		//------------------------------------------------------------------------
		//Write Data
		//2006-12-04 YTT: WRITE 12 TIME
		int PartialDataSize = FinalSize;
		int PartialBlockSize = 2048;
		//FF:設定好要送data後 實際送data的地方拆成2k送 送12次
		for (int j = 0; j < 12; j++) {
			if (PartialDataSize <= 0) {
				break;
			}
			if (PartialDataSize < PartialBlockSize) {
				PartialBlockSize = PartialDataSize;
			}
			if (m_pCommDrv->write(File_Data2 + j*2048, PartialBlockSize) != PartialBlockSize) {
				m_pCommDrv->close();
				file2.free_file_mem(file_hdr2);

				return HS_WRITE_DL_DATA_FAIL;
			}
			PartialDataSize -= 2048;
		}

		if (m_farCallBackChar != NULL && (i % 10) == 0) {
			m_farCallBackChar(nPort, "*");
		}
		//Point to next sector of file handler
		File_Data2 += (1024 * 24);
	}//End of send data loop

	//Jump to DDR RAM Start Address
	nRetCode = ExactUSBTIJumpAddr(DT_START_ADDR);
	if (nRetCode < 1) {
		m_pCommDrv->close();
		//ISBTIJump can not return meaningful error code
		return nRetCode;
	}

	m_pCommDrv->close();
	file2.free_file_mem(file_hdr2);
	Sleep(50);

	return HS_OK;
}

int CmdCtrl::ExactUSBTIJumpAddr(unsigned int StartAddr)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char General_Message[4];
	memset(General_Message, 0, sizeof(General_Message));
	General_Message[0] = 0x08;
	General_Message[1] = 0xff;
	General_Message[2] = 0xaa;
	General_Message[3] = 0x55;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	//modify address here
	//please assign StartAddress here !!!!
	General_Message[0] = 0x00;
	General_Message[1] = 0x00;
	General_Message[2] = 0x00;
	General_Message[3] = 0x10;
	//Jump TO Address
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	General_Message[0] = 0x20;
	General_Message[1] = 0xaa;
	General_Message[2] = 0x55;
	General_Message[3] = 0xFF;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	return HS_OK;
}

int CmdCtrl::ExactUSBTIJumpAddrEx(int nPort, int nTimeout, unsigned int StartAddr, char* workPath )
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char General_Message[4];
	memset(General_Message, 0, sizeof(General_Message));

	//FF: SET ADDRESS
	General_Message[0] = 0x08;
	General_Message[1] = 0xff;
	General_Message[2] = 0xaa;
	General_Message[3] = 0x55;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	//FF: physical address
	General_Message[0] = 0x00;
	General_Message[1] = 0x00;
	General_Message[2] = 0x00;
	General_Message[3] = 0x10;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	//FF: Run 應該是設定好起始的位址後 jump的命令
	General_Message[0] = 0x20;
	General_Message[1] = 0xaa;
	General_Message[2] = 0x55;
	General_Message[3] = 0xFF;
	if (m_pCommDrv->write(General_Message, sizeof(General_Message)) != sizeof(General_Message)) {
		return HS_WRITE_DATA_FAIL;
	}

	//之後while loop等回應
	memset(General_Message, 0, sizeof(General_Message));
	General_Message[1] = 0;
	int iFW_finish = 0;
	//open file
	int iSaveData = 0;
	char buffer[2*1024];
	memset(buffer, 0, sizeof(buffer));
	while (m_pCommDrv->read(General_Message, 1, nTimeout) == 1) {
		if ((General_Message[0] == 'A') && (iSaveData == 0)) {
			iSaveData = 1;
			continue;
		}
		if (iSaveData == 1) {
			iSaveData = (General_Message[0] == 'C') ? 2 : 0;
			continue;
		}
		if (iSaveData == 2) {
			iSaveData = (General_Message[0] == 'U') ? 3 : 0;
			continue;
		}
		if (iSaveData == 3) {
			iSaveData = (General_Message[0] == 'X') ? 4 : 0;
			continue;
		}
		if (iSaveData == 4) {
			if ((General_Message[0] == 'Z')) {
				if (m_pCommDrv->read(General_Message, 4, nTimeout) == 4) {
					iSaveData = 5;
				}
				else {
					return HS_FAIL;
				}

				char *cKData[MAX_PATH];
				memset(cKData, 0, sizeof(cKData));
				strcpy((char*)cKData, workPath);
				strcat((char*)cKData, "\\P21WCalibrationDataBackup.dat");
				ofstream fout((char*)cKData, ios::binary);
				for (int i = 0; i < 80; i++) {
					if (m_pCommDrv->read((unsigned char *)buffer, 1024, nTimeout) == 1024) {
						fout.write(buffer, 1024);
					}
					else {
						fout.close();
						return HS_FAIL;
					}
				}

				//read B???? and file sieze (4 byte), total 9 byte
				if (m_pCommDrv->read((unsigned char *)buffer, 9, nTimeout) != 9) {
					fout.close();
					return HS_FAIL;
				}

				memset(General_Message, 0, sizeof(General_Message));
				fout.close();
			}
			else {
				iSaveData = 0;
			}
			continue;
		}//end of iSaveData==4


		if (!(m_farCallBackChar == NULL)) {
			//m_farCallBack(nPort,atoi((const char*)pPackData[0]));
			m_farCallBackChar(nPort, (char*)General_Message);
		}
		if (iFW_finish == 0) {
			if (General_Message[0] == 'a') {
				iFW_finish = 1;
			}
			continue;
		}
		if (iFW_finish == 1) {
			iFW_finish = (General_Message[0] == 'd') ? 2 : 0;
			continue;
		}
		if (iFW_finish == 2) {
			iFW_finish = (General_Message[0] == 'a') ? 3 : 0;
			continue;
		}
		if (iFW_finish == 3) {
			iFW_finish = (General_Message[0] == ':') ? 4 : 0;
			continue;
		}
		if (iFW_finish == 4) {
			if (General_Message[0] == '1') {
				return HS_OK;
			}
			else {
				iFW_finish = 0;
			}
			continue;
		}
	}

	return HS_FAIL;
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value 
int CmdCtrl::RecvCommand(int PayloadSize, unsigned char* Command, unsigned char* PayloadData, unsigned char* OutputData, int nPort)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char pPackData[PAYLOAD_SIZE];
	unsigned char cGenPayload[PAYLOAD_SIZE]; // FIXME buffer size
	cGenPayload[0] = Command[0];
	cGenPayload[1] = Command[1];
	cGenPayload[2] = Command[2];
	cGenPayload[3] = Command[3];
	//FF:如果有參數的話copy 端看傳進來的Payloadsize是多少
	memcpy(cGenPayload + 4, PayloadData, PayloadSize);

	//// 2008-02-25 Nek Wang: receive result
	memset(pPackData, 0, sizeof(pPackData));
	// TODO: check recevie buffer
	while (1) {
		/*110610 Gilpin.Xi delete for touch speed Start*/
		//Sleep(50);
		/*110610 Gilpin.Xi delete for touch speed End*/
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
		int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			memcpy(OutputData, (pPackData + 1), PAYLOAD_SIZE*sizeof(unsigned char)); // FIXME buffer size
			return HS_OK;
		}
		else if (nRetCode != HS_PROGRESS) {
			return HS_FAIL;
		}
	}
	return HS_FAIL;
}

int CmdCtrl::ExactReceiveCmdOut(int nPort, int nTimeout, char* CtrlCmd, char* OutBuffer)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));

	//Open Device
	cGenPayload[0] = CtrlCmd[0];
	cGenPayload[1] = CtrlCmd[1];
	cGenPayload[2] = CtrlCmd[2];
	cGenPayload[3] = CtrlCmd[3];
	int PayloadSize = 0;
	if (nPort >= QTH_PORT_USB) {
		m_bIfUSBCmd = true;
	}
	int nRetCode = RecvCommand(PayloadSize, cGenPayload, cGenPayload + 4, (unsigned char *)OutBuffer, nPort);
	if (m_bIfUSBCmd) {
		m_bIfUSBCmd = false;
	}

	if (nRetCode != HS_OK) {
		TRACE(_T("%s RecvCommand(%d) fail\r\n"), __FUNCTION__, nRetCode);
		return nRetCode;
	}

	return HS_OK;
}

//super general function,support GeneralCmd, GeneralCmdIn, GeneralCmdOut, GeneralCmdInOut
int CmdCtrl::QthGenCommand(int PayloadSize, unsigned char * Command, unsigned char * PayloadData, unsigned char * OutputData, int nPort)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;//用來取得算出來的Checksum
	unsigned char cGenPayload[PAYLOAD_SIZE]; // FIXME buffer size
	cGenPayload[0] = Command[0];
	cGenPayload[1] = Command[1];
	cGenPayload[2] = Command[2];
	cGenPayload[3] = Command[3];
	//FF:如果有參數的話copy 端看傳進來的Payloadsize是多少
	memcpy(cGenPayload + 4, PayloadData, PayloadSize);

	if ((!m_bIfUSBCmd) && (m_isOpen)) {
		Sleep(2);
	}
	else if ((!m_bIfUSBCmd) && (!m_isOpen)) {
		Sleep(11);
	}
	else {
		Sleep(20);
	}

	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	if (SendResult < HS_OK) {
		return SendResult;
	}
	if ((!m_bIfUSBCmd) && (m_isOpen)) {
		Sleep(2);
	}
	else if ((!m_bIfUSBCmd) && (!m_isOpen)) {
		Sleep(11);
	}
	else {
		Sleep(20);
	}
	//此處FileSize此時已經變成sector了,沒用到

	unsigned char pPackData[PAYLOAD_SIZE];
	memset(pPackData, 0, sizeof(pPackData));
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		return ReceiveResult;
	}
	//再來這邊應該要check一下chksum吧
	//double check checksum
	//level 2: check the sender and the receiver
	//剛剛在收封包的時候就檢查過一次了 最底層
	//現在則是檢查 送封包 跟 收封包 的地方 所以YT才會叫做Level2吧
	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		return HS_INVALID_CHECKSUM;
	}

	//// 2008-02-25 Nek Wang: receive result
	memset(pPackData, 0, sizeof(pPackData));
	// TODO: check recevie buffer
	while (1) {
		Sleep(50);
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
		int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			memcpy(OutputData, (pPackData + 1), PAYLOAD_SIZE*sizeof(unsigned char)); // FIXME buffer size
			return HS_OK;
		}
		else if(nRetCode == HS_EMPTY)
		{
			return HS_EMPTY;
		}
		else if (nRetCode != HS_PROGRESS) {
			
			if(Command[0] == 0x1A && Command[1] == 0x01 && Command[2] == 0x00 && Command[3] == 0x00)
			{
				memcpy(OutputData, (pPackData + 1), 1);				

				if(OutputData[0] == 2)
					return HS_OK;
			}
			//add by Tom.Li
			else if(Command[0] == 0x09 && Command[1] == 0x01 && Command[2] == 0x00 && Command[3] == 0x00)
			{
				memcpy(OutputData, (pPackData + 1), 1);	
				OutputData[1]=0;
			}
			else
			{
				memcpy(OutputData, (pPackData + 1), PAYLOAD_SIZE*sizeof(unsigned char));
			}


			return HS_FAIL;
		}
	}
	return HS_FAIL;
}

//Exact GeneralCmd is used to send GeneralCmd so that It's not necessary to
//handle the output buffer "pPackData"
int CmdCtrl::ExactGeneralCmd(int nPort, int nTimeout, char* CtrlCmd)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	unsigned char pPackData[PAYLOAD_SIZE]; // FIXME buffer size
	memset(pPackData, 0, sizeof(pPackData));

	//Open Device
	cGenPayload[0] = CtrlCmd[0];
	cGenPayload[1] = CtrlCmd[1];
	cGenPayload[2] = CtrlCmd[2];
	cGenPayload[3] = CtrlCmd[3];

	if (nPort >= QTH_PORT_USB) {
		m_bIfUSBCmd = true;
	}
	int PayloadSize = 0;
	int nRetCode = QthGenCommand(PayloadSize, cGenPayload, cGenPayload + 4, pPackData, nPort);
	if (m_bIfUSBCmd) {
		m_bIfUSBCmd = false;
	}

	if (nRetCode != HS_OK) {
		TRACE(_T("%s(%d)\n"), __FUNCTION__, nRetCode);
		return nRetCode;
	}
	return HS_OK;
}

//Exact GeneralCmd is used to send GeneralCmd so that It's not necessary to
//handle the output buffer "pPackData"
int CmdCtrl::ExactGeneralCmdOut(int nPort, int nTimeout, char* CtrlCmd, char* OutBuffer)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));

	//Open Device
	cGenPayload[0] = CtrlCmd[0];
	cGenPayload[1] = CtrlCmd[1];
	cGenPayload[2] = CtrlCmd[2];
	cGenPayload[3] = CtrlCmd[3];
	int PayloadSize = 0;
	if (nPort >= QTH_PORT_USB) {
		m_bIfUSBCmd = true;
	}
	int nRetCode = QthGenCommand(PayloadSize, cGenPayload, cGenPayload + 4, (unsigned char *)OutBuffer, nPort);
	if (m_bIfUSBCmd) {
		m_bIfUSBCmd = false;
	}

	if (nRetCode != HS_OK) {
		TRACE(_T("%s QthGenCommand(%d) fail\r\n"), __FUNCTION__, nRetCode);
		return nRetCode;
	}

	return HS_OK;
}

int CmdCtrl::QthGenPacketSend(int Type, int PayLoadSize, unsigned char * pPackData, unsigned char * OChecksum, int nPort)
{
	int TotalDataSize; //Total要傳的DataSize自己算
	unsigned char PacketHeader[4] = {'B','E','N','Q'};

	//注意這和P31 PROTOCOL ENDIAN不同
	//因為command與data共用一個參數pPackData
	//所以實際的payload size還得扣掉4 byte(for command)
	//但在傳DATA DL的時候因為傳進來的PAYLOADSIZE就是DATA SIZE
	//所以就不用
	//以後要記得改好,目前狀況是會傳進 4(COMMAND)+PAYLOAD(DATA)
	//個DATA,但是PAYLOADSIZE名稱卻沒有正確反映此狀況
	// 2006-08-23 已修正: Payloadsize = sizeof(pPackData)-4
	unsigned char FileSize[4];
	FileSize[0] = (unsigned char)(PayLoadSize >> 24);
	FileSize[1] = (unsigned char)(PayLoadSize >> 16);
	FileSize[2] = (unsigned char)(PayLoadSize >> 8);
	FileSize[3] = (unsigned char)PayLoadSize;

	unsigned char temp[9000];
	memset(&temp, 0, sizeof(temp));
	unsigned char Checksum;
	//檢查
	//1.packdata是否有比temp長,ex超過設定上限
	//2.注意packetsize是否比packdata大
	if (Type == DT_PKT_REQUEST || Type == P21_DATADL_REQUEST || Type == P21_DATAUL_REQUEST) {
		memcpy(temp, PacketHeader, DT_PKT_HEADER_SIZE); // P21 packet header -> "BENQ"
		temp[4] = (unsigned char)Type;  // P21 Command Type
		memcpy(temp + 5, pPackData, 4); // command or block number
		memcpy(temp + 9, FileSize, 4);  // payload size
		memcpy(temp + 13, pPackData + 4, PayLoadSize); // payload data
		TotalDataSize = DT_PKT_HEADER_SIZE + 1 + 4 + 4 + PayLoadSize;

		//Calculate Checksum
		Checksum = temp[0];
		for (int j = 1; j < TotalDataSize; j++) {
			Checksum ^= temp[j];
		}
		temp[TotalDataSize] = Checksum;
		*OChecksum = Checksum;
		TotalDataSize++; //add chcksum

		int nDataSize = TotalDataSize;
		int nPktSend = 0;
		int nByteWrite = DT_PKT_BLOCK_SIZE;
		while (nDataSize > 0) {
			TRACE("%s PartialDataSize: %d\n", __FUNCTION__, nDataSize);
			nByteWrite = (nDataSize < nByteWrite) ? nDataSize : DT_PKT_BLOCK_SIZE;
			if (m_pCommDrv->write(&temp[nPktSend*DT_PKT_BLOCK_SIZE], nByteWrite) != nByteWrite) {
				return HS_WRITE_DATA_FAIL;
			}
			nPktSend++;
			nDataSize -= nByteWrite;

			/* Alex.Chen Modify Start 2011/05/30 */
			//Sleep(300);
			/* Alex.Chen Modify End 2011/05/30 */
		}
		return HS_OK;
	}// if Type is DT_PKT_REQUEST, P21_DATADL_REQUEST, or P21_DATAUL_REQUEST

	return HS_FAIL;
}

// TODO: add timeout here
int CmdCtrl::QthGenPacketRecv(int type, unsigned char * PayloadCommand, unsigned char * pReceiveData, unsigned char * NChecksum)
{
	TRACER_STREAM(" => QthGenPacketRecv, type: " << type << "\r\n");
	// 外面:  while (depackage()) 如果回傳是true就一值解封包或
	// 一直接收檔案,直到抓到指令,如果timeout 就直接結束
	// 1.收到封包後解譯,並將結果利用packdata陣列傳出去
	// 1.1 function
	int nTimeout = 10000;
	unsigned char data[4];
	int PayloadSize = 0;
	data[0] = 0;

	/* 2011/05/23 Alex.Chen Add Start */
	clock_t start;
	clock_t finish;
	/* 2011/05/23 Alex.Chen Add End */

	// 2006-11-09 YTT special control for BTConnect
	if ((PayloadCommand[0] == QC_DT_SIM && PayloadCommand[1] == 6) || // cmd: 6, 6
		(PayloadCommand[0] == QC_DT_BT  && PayloadCommand[1] == 1) || // cmd: 7, 1
		(PayloadCommand[0] == P21_DT_BT     && PayloadCommand[1] == 5) || // cmd: 7, 5
		(PayloadCommand[0] == P21_DT_CAMERA && PayloadCommand[1] == 3) || // cmd: 2, 3
		(PayloadCommand[0] == P21_DT_CAMERA && PayloadCommand[1] == 4) || // cmd: 2, 4
		(PayloadCommand[0] == P21_DT_CAMERA && PayloadCommand[1] == 5) || // cmd: 2, 5
		(PayloadCommand[0] == P21_DT_FLASH) || // cmd: 8, x
		(PayloadCommand[0] == NV_DT_TOUCH && PayloadCommand[1] == 4) //	NV_DT_TOUCH, cmd: 4
		) {
		nTimeout = 20000;
	}
	// 2009-04-06 Mick: WLAN Tx Max Power need at least 20 sec
	if (PayloadCommand[0] == QC_DT_WLAN && PayloadCommand[1] == 1) { // cmd: 18, 1
		nTimeout = 25000;
	}
	// 2013-08-15 Macl: P sensor cal need 30 sec at maximum
	if (PayloadCommand[0] == NV_DT_SENSOR && PayloadCommand[1] == 11) { // cmd: 4, 11
		nTimeout = 30 * CLOCKS_PER_SEC;
	}
	// 2013-08-15 Macl: Camera need 20 sec at maximum
	if (PayloadCommand[0] == NV_DT_CAMERA && PayloadCommand[1] == 2) { // cmd: 4, 11
		nTimeout = 20 * CLOCKS_PER_SEC;
	}

	unsigned char temp[5000];
	memset(temp, 0, sizeof(temp));

	/* 2011/05/23 Alex.Chen Add Start */
	start = clock(); // Count start time
	/* 2011/05/23 Alex.Chen Add End */

	while (1) {
		//嘗試接收第一個byte, 並辨認是否為指定Function的封包header
		//因為m_pCommDrv有timeout機制,所以...不需要自己設定timeout
		//FF: try to read B
		TRACE("[B]"); //[B]ENQ
		if (m_pCommDrv->read(data, 1, nTimeout) != 1) {
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'B') {

			/* 2011/05/23 Alex.Chen Add Start */
			finish = clock();
			if((finish-start) > nTimeout) {
				break;
			}
			/* 2011/05/23 Alex.Chen Add End */

			continue;
		}

		//FF: try to read E
		temp[0] = data[0];
		TRACE("[E]"); //B[E]NQ
		if (m_pCommDrv->read(data, 1, 300) != 1) {
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'E') {

			/* 2011/05/23 Alex.Chen Add Start */
			finish = clock();
			if((finish-start) > nTimeout) {
				break;
			}
			/* 2011/05/23 Alex.Chen Add End */

			continue;
		}

		//FF: try to read N
		temp[1] = data[0];
		TRACE("[N]"); //BE[N]Q
		if (m_pCommDrv->read(data, 1, 300) != 1) {
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'N') {

			/* 2011/05/23 Alex.Chen Add Start */
			finish = clock();
			if((finish-start) > nTimeout) {
				break;
			}
			/* 2011/05/23 Alex.Chen Add End */

			continue;
		}

		//FF: try to read Q
		temp[2] = data[0];
		TRACE("[Q]"); //BEN[Q]
		if (m_pCommDrv->read(data, 1, 300) != 1) {
			return HS_READ_DATA_FAIL;
		}
		if (data[0] != 'Q') {

			/* 2011/05/23 Alex.Chen Add Start */
			finish = clock();
			if((finish-start) > nTimeout) {
				break;
			}
			/* 2011/05/23 Alex.Chen Add End */

			continue;
		}

		temp[3] = data[0];
		//get command type
		TRACE(", get pc control command\n");

		if (m_pCommDrv->read(data, 1, 300) != 1) {
			return HS_READ_DATA_FAIL;
		}
		temp[4] = data[0];
		TRACE(_T("cmd type: 0x%x\n"), data[0]);
		//===========================
		//判斷是是否要收的cmd或是progress
		//如果是progress的話也要收下封包內容後傳回去
		bool progress_flag = false;
		if (data[0] == DT_PKT_PROGRESS) {
			progress_flag = true;
		}
		if (data[0] == type || progress_flag) {
			//要收DataSize+1 個,因為最後還有1個checksum
			//FF:Get command or block number (DL or UL function)
			if (m_pCommDrv->read(data, 4, 1000) != 4) {
				return HS_READ_DATA_FAIL;
			}
			memcpy(temp + 5, data, 4);

			//FF:get payloadsize
			if (m_pCommDrv->read(data, 4, 1000) != 4) {
				return HS_READ_DATA_FAIL;
			}
			memcpy(temp + 9, data, 4);

			PayloadSize = data[3] + data[2] * 16 * 16 + data[1] * 16 * 16 * 16 + data[0] * 16 * 16 * 16 * 16;
			TRACE("%s payload size: %d\n", __FUNCTION__, PayloadSize);
			if (m_pCommDrv->read(temp + 13, PayloadSize + 1, 3000) != PayloadSize + 1) {
				return HS_READ_DATA_FAIL;
			}

			unsigned char PacketChecksum = temp[0];
			//check whole packet checksum
			for (int i = 1; i < 9 + 4 + PayloadSize; i++) {
				PacketChecksum ^= temp[i];
			}
			//double check checksum這裡是拿pc tool這邊算出來的checksum來跟DT傳過來的比
			//level 1: check whole packet
			if (PacketChecksum != temp[9 + 4 + PayloadSize]) {
				TRACE("%s packet checksum fail\n", __FUNCTION__);
				return HS_INVALID_CHECKSUM;
			}
			//還要檢查DT傳回來的cmd是否是之前PC tool傳過去的command
			for (int i = 0; i < 4; i++) {
				if (temp[i + 5] != PayloadCommand[i]) {
					TRACE("%s check command fail(%d): %d != %d\n", __FUNCTION__, i, temp[i + 5], PayloadCommand[i]);
					return HS_READ_DATA_FAIL;
				}
			}

			// drop out checksum
			// last packet checksum in index 9+5 if its a ack packet
			*NChecksum = temp[9 + 4 + PayloadSize - 1];
			//copy data to temp, 只有在夾帶raw data時才需要
			memcpy(pReceiveData, &temp[13], PayloadSize);
			if (progress_flag) {
				return HS_PROGRESS;
			}
			else {
				return HS_OK;
			}
		} // if (type == true) end
	} // whole pack ok?
	//FFYang 0927

	return HS_FAIL;
}

// TODO: too many exit point in this function.
int CmdCtrl::ExactUSBGenDownloadCmd(int nPort, int nTimeout, char* DLFileType, char* FileName)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;
	//type and block num(4)  Data(512)
	unsigned char cGenPayload[16384];
	unsigned char pPackData[256];
	int PayloadSize = 0;

	//FF: 這應該可以獨立出來了 =.= 1021判斷是否從SD 讀檔
	if (DLFileType[0] == 1 && DLFileType[1] == 2) {
		int PayloadSize = (int)strlen(FileName);
		memset(cGenPayload, 0, sizeof(cGenPayload));
		memset(pPackData, 0, sizeof(pPackData));
		cGenPayload[0] = DLFileType[0];//1
		cGenPayload[1] = DLFileType[1];//2
		cGenPayload[2] = DLFileType[2];//0
		cGenPayload[3] = DLFileType[3];//0
		for (size_t i = 0; i < strlen(FileName); i++) {
			cGenPayload[4+i] = FileName[i];
		}

		int SendResult = 0;
		SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
		Sleep(5);
		//此處FileSize此時已經變成sector了,沒用到
		ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
		if (ReceiveResult != HS_OK) {
			TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
			return ReceiveResult;
		}
		//double check checksum
		//level 2: check the sender and the receiver
		//剛剛在收封包的時候就檢查過一次了 最底層
		//現在則是檢查 送封包   跟收封包的地方 所以YT才會叫做Level2吧
		if (OldChecksum != NewChecksum) {
			TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
			return HS_INVALID_CHECKSUM;
		}

		//// 2008-02-25 Nek Wang: clear progress
		while (1) {
			Sleep(20);
			ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);

			int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
			if (nRetCode == HS_OK) {
				return HS_OK;
			}
			else if (nRetCode != HS_PROGRESS) {
				return HS_FAIL;
			}
		}
		return HS_FAIL;
	}
	else {
		Dl_Filehandler file2;
		T_FILE_HDR *file_hdr2;
		U8 *File_Data2;
		int nRetCode = 0;
		DWORD FileSize = 0;
		nRetCode = file2.read_file(FileName, &file_hdr2);
		if (nRetCode == FAULT) {
			file2.free_file_mem(file_hdr2);
			return HS_FILE_NOT_FOUND;
		}
		//注意實際要傳時得沏,每個block 大小為0x6000
		File_Data2 = (file_hdr2->section_hdr[0]).data_ptr;
		FileSize = (file_hdr2->section_hdr[0]).sct_size;
		//Create the size command (part of OMAP protocol!)
		// NC - must be little endian for UART - doesn't matter for USB in current devices.
		// but it could matter for future devices.
		//----------------------------------------------------------------
		//Initial buffer command as a kind of GeneralCmdIn() with P21GeneralCmd
		//Default BlockSize

		DWORD dwBlockSize = DT_USB_BLOCK_SIZE;
		//以後下面的code 的block size要改成動態的
		//default是8192
		//for General Download
		//16-33 Init Buffer Size 512
		cGenPayload[0] = 16;
		cGenPayload[1] = 33;
		cGenPayload[2] = 0;
		cGenPayload[3] = 0;
		//total initial buffer  1024

		//其實何必麻煩,用湊整數的就好啦
		cGenPayload[4]  = (unsigned char)((0xFF000000 & FileSize) >> 24);
		cGenPayload[6]  = (unsigned char)((0x00FF0000 & FileSize) >> 16);
		cGenPayload[8]  = (unsigned char)((0x0000FF00 & FileSize) >> 8);
		cGenPayload[10] = (unsigned char)( 0x000000FF & FileSize);
		//transfer 4byte hex to 8 byte ascii
		for (int i = 0; i < 4; i++) {
			cGenPayload[5+2*i] = cGenPayload[4+2*i] % 16;
			if (cGenPayload[5+2*i] < 10) {
				cGenPayload[5+2*i] = '0' + cGenPayload[5+2*i];
			}
			else {
				cGenPayload[5+2*i] = 'A' + cGenPayload[5+2*i] - 10;
			}
			cGenPayload[4+2*i] = cGenPayload[4+2*i] / 16;
			if (cGenPayload[4+2*i] < 10) {
				cGenPayload[4+2*i] = '0' + cGenPayload[4+2*i];
			}
			else {
				cGenPayload[4+2*i] = 'A' + cGenPayload[4+2*i] - 10;
			}
		}

		//注意init buffer應該要改成P21GeneralCmd才對
		//寫死的,注意要改掉
		//block size  2048
		cGenPayload[12] = '0';
		cGenPayload[13] = '0';
		cGenPayload[14] = '0';
		cGenPayload[15] = '0';
		cGenPayload[16] = '0';
		cGenPayload[17] = 8 + '0';
		cGenPayload[18] = '0';
		cGenPayload[19] = '0';
		PayloadSize = 16;
		memset(pPackData, 0, sizeof(pPackData));

		SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
		if (SendResult < HS_OK) {
			file2.free_file_mem(file_hdr2);
			return SendResult;
		}

		ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
		if (ReceiveResult != 1) {
			file2.free_file_mem(file_hdr2);
			return ReceiveResult;
		}
		if (OldChecksum != NewChecksum) {
			file2.free_file_mem(file_hdr2);
			return HS_INVALID_CHECKSUM;
		}

		Sleep(20);
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
		nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode !=  HS_OK) {
			/////////////////////////////////////////////////////////////////////
			TRACER_STREAM("[Port " << nPort << "] => DT_PKT_RESULT: FAIL - " << (char*)cGenPayload << "\r\n");
			/////////////////////////////////////////////////////////////////////
			file2.free_file_mem(file_hdr2);
			return nRetCode;
		}

		unsigned char cChecksum[9];
		memset(cChecksum, 0, sizeof(cChecksum));
		for (DWORD i = 0; i < FileSize; i++) {
			switch (i % 4) {
			case 0:
				cChecksum[0] = cChecksum[0] ^ File_Data2[i];
				break;
			case 1:
				cChecksum[2] = cChecksum[2] ^ File_Data2[i];
				break;
			case 2:
				cChecksum[4] = cChecksum[4] ^ File_Data2[i];
				break;
			case 3:
				cChecksum[6] = cChecksum[6] ^ File_Data2[i];
				break;
			default:
				break;
			}
		}
		for (int i = 0; i < 4; i++) {
			cChecksum[1+2*i] = cChecksum[2*i] % 16;
			if (cChecksum[1+2*i] < 10) {
				cChecksum[1+2*i] = '0' + cChecksum[1+2*i];
			}
			else {
				cChecksum[1+2*i] = 'A' + cChecksum[1+2*i] - 10;
			}
			cChecksum[2*i] = cChecksum[2*i] / 16;
			if (cChecksum[2*i] < 10) {
				cChecksum[2*i] = '0' + cChecksum[2*i];
			}
			else {
				cChecksum[2*i] = 'A' + cChecksum[2*i] - 10;
			}

		}

		int FinalSize = 0;
		PayloadSize = dwBlockSize;
		int iBlockNum = (FileSize / dwBlockSize);
		if ((FileSize % dwBlockSize) != 0) {
			iBlockNum++;
		}
		int nRetCoderansfer_Count = 0;
		FinalSize = dwBlockSize;

		int iCount = 0;
		while (1) {
			if (iCount >= iBlockNum) {
				break;
			}
			//FF: 清空buffer Send Data DL/Receive Data Ack
			memset(cGenPayload, 0, sizeof(cGenPayload));

			//FF:block number
			cGenPayload[0] = (unsigned char)(iCount >> 24);
			cGenPayload[1] = (unsigned char)(iCount >> 16);
			cGenPayload[2] = (unsigned char)(iCount >> 8);
			cGenPayload[3] = (unsigned char)iCount;

			//最後一個block 就塞滿0
			if (iCount == iBlockNum - 1) {
				FinalSize = FileSize % (dwBlockSize);
				if (FinalSize == 0) {
					FinalSize = dwBlockSize;
				}
			}

			//copy data to buffer
			memcpy(cGenPayload + 4, File_Data2, FinalSize);
			SendResult = QthGenPacketSend(P21_DATADL_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
			if (SendResult < 1) {
				/////////////////////////////////////////////////////////////////////
				TRACER_STREAM("[Port " << nPort << "] => P21_DATADL_REQUEST: FAIL - " << (char*)cGenPayload << "\r\n");
				/////////////////////////////////////////////////////////////////////
				file2.free_file_mem(file_hdr2);
				return SendResult;
			}

			memset(pPackData, 0, sizeof(pPackData));
			ReceiveResult = QthGenPacketRecv(P21_DATADL_ACK, cGenPayload, pPackData, &NewChecksum);
			if (ReceiveResult != 1) {
				/////////////////////////////////////////////////////////////////////
				TRACER_STREAM("[Port " << nPort << "] => P21_DATADL_ACK: FAIL - " << (char*)pPackData << "\r\n");
				/////////////////////////////////////////////////////////////////////
				nRetCoderansfer_Count++;
				if (nRetCoderansfer_Count < 4) {
					continue;
				}
				file2.free_file_mem(file_hdr2);
				return ReceiveResult;
			}
			if (OldChecksum != NewChecksum) {
				TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
				file2.free_file_mem(file_hdr2);
				return HS_INVALID_CHECKSUM;
			}

			int nProgress = ((iCount + 1) * 100 / iBlockNum) / 2;
			char cProgress[5];
			if (m_farCallBackChar != NULL) {
				if ((((iCount + 1)*10) % (iBlockNum / 10)) == 0 ) {
					memset(cProgress, 0, sizeof(cProgress));
					wsprintf(cProgress, "%d ", nProgress);
					m_farCallBackChar(nPort, cProgress);
				}
			}

			nRetCoderansfer_Count = 0;
			File_Data2 += PayloadSize;
			iCount++;
		}//sending data for loop end

		//FF:要清成0 不然會影響下依次的code
		nRetCoderansfer_Count = 0;
		//下16-44取得checksum

		memset(pPackData, 0, sizeof(pPackData));
		memset(cGenPayload, 0, sizeof(cGenPayload));
		cGenPayload[0] = 16;
		cGenPayload[1] = 34;
		cGenPayload[2] = 0;
		cGenPayload[3] = 0;
		cGenPayload[4] = (unsigned char)( (0xFF000000 & FileSize) >> 24);
		cGenPayload[6] = (unsigned char)( (0x00FF0000 & FileSize) >> 16);
		cGenPayload[8] = (unsigned char)( (0x0000FF00 & FileSize) >> 8);
		cGenPayload[10] = (unsigned char) (0x000000FF & FileSize);
		//transfer 4byte hex to 8 byte ascii
		for (int i = 0; i < 4; i++) {
			cGenPayload[5+2*i] = cGenPayload[4+2*i] % 16;
			if (cGenPayload[5+2*i] < 10) {
				cGenPayload[5+2*i] = '0' + cGenPayload[5+2*i];
			}
			else {
				cGenPayload[5+2*i] = 'A' + cGenPayload[5+2*i] - 10;
			}
			cGenPayload[4+2*i] = cGenPayload[4+2*i] / 16;
			if (cGenPayload[4+2*i] < 10) {
				cGenPayload[4+2*i] = '0' + cGenPayload[4+2*i];
			}
			else {
				cGenPayload[4+2*i] = 'A' + cGenPayload[4+2*i] - 10;
			}
		}
		PayloadSize = 8;

		//下16-44時
		//收progress跟checksum的情況  這是因為檔案太大的時候
		//DT算checksum要比較久的時間 所以read result的時候會timeout
		//但要是timeout 設太長會影響到整體的速度
		//(也就是說一個cmd fail都會等到10s)
		//所以就收progress解決
		/////////////////////////
		//接下來 直接送 一直收 然後結束 不要用QthGenCommand

		memset(pPackData, 0, sizeof(pPackData));
		int SendResult = 0;
		SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
		ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
		if (ReceiveResult != HS_OK) {
			TRACER_STREAM("[Port " << nPort << "] => DT_PKT_ACK: FAIL - " << (char*)pPackData << "\r\n");

			file2.free_file_mem(file_hdr2);
			return ReceiveResult;
		}

		if (OldChecksum != NewChecksum) {
			TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
			file2.free_file_mem(file_hdr2);
			return HS_INVALID_CHECKSUM;
		}
		//// 2008-02-25 Nek Wang: clear progress
		for (;;) {
			Sleep(20);
			ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
			nRetCode = PacketResultCheck(ReceiveResult, pPackData);
			if (nRetCode == HS_OK) {
				break;
			}
			else if (nRetCode != HS_PROGRESS) {
				file2.free_file_mem(file_hdr2);
				return HS_FAIL;
			}
		}

		unsigned char temp_pPackData[256];
		memset(temp_pPackData, 0, sizeof(temp_pPackData));
		memcpy(temp_pPackData, &(pPackData[1]), 254);
		if (CString(temp_pPackData).Compare(CString(cChecksum)) != 0) {
			TRACE(_T("%s checksum fail\r\n"), __FUNCTION__);
			file2.free_file_mem(file_hdr2);
			return HS_INVALID_CHECKSUM;
		}
		memset(pPackData, 0, sizeof(pPackData));
		memset(cGenPayload, 0, sizeof(cGenPayload));
		memset(pPackData, 0, sizeof(pPackData));
		memset(cGenPayload, 0, sizeof(cGenPayload));
		cGenPayload[0] = DLFileType[0];
		cGenPayload[1] = DLFileType[1];
		cGenPayload[2] = DLFileType[2];
		cGenPayload[3] = DLFileType[3];
		cGenPayload[4] = (unsigned char)( (0xFF000000 & FileSize) >> 24);
		cGenPayload[6] = (unsigned char)( (0x00FF0000 & FileSize) >> 16);
		cGenPayload[8] = (unsigned char)( (0x0000FF00 & FileSize) >> 8);
		cGenPayload[10] = (unsigned char) (0x000000FF & FileSize);
		//transfer 4byte hex to 8 byte ascii
		for (int i = 0; i < 4; i++) {
			cGenPayload[5+2*i] = cGenPayload[4+2*i] % 16;
			if (cGenPayload[5+2*i] < 10) {
				cGenPayload[5+2*i] = '0' + cGenPayload[5+2*i];
			}
			else {
				cGenPayload[5+2*i] = 'A' + cGenPayload[5+2*i] - 10;
			}
			cGenPayload[4+2*i] = cGenPayload[4+2*i] / 16;
			if (cGenPayload[4+2*i] < 10) {
				cGenPayload[4+2*i] = '0' + cGenPayload[4+2*i];
			}
			else {
				cGenPayload[4+2*i] = 'A' + cGenPayload[4+2*i] - 10;
			}
		}
		//Verify Checksum
		PayloadSize = 8;

		memset(pPackData, 0, sizeof(pPackData));
		SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
		ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
		if (ReceiveResult != HS_OK) {
			TRACER_STREAM("[Port " << nPort << "] => DT_PKT_ACK: Fail\r\n");

			file2.free_file_mem(file_hdr2);
			return ReceiveResult;
		}

		if (OldChecksum != NewChecksum) {
			TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
			file2.free_file_mem(file_hdr2);
			return HS_INVALID_CHECKSUM;
		}

		//// 2008-02-25 Nek Wang: receive progress.
		for (;;) {
			Sleep(20);
			ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
			int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
			if (nRetCode == HS_OK) {
				if (m_farCallBackChar != NULL) {
					m_farCallBackChar(nPort, "100 ");
				}
				file2.free_file_mem(file_hdr2);
				return HS_OK;
			}
			else if (nRetCode == HS_PROGRESS) {
				char c2Progress[5];
				if (m_farCallBackChar != NULL) {
					memset(c2Progress, 0, sizeof(c2Progress));
					int nPrg = 50 + (int)pPackData[0] / 2;
					wsprintf(c2Progress, "%d ", nPrg);
					m_farCallBackChar(nPort, c2Progress);
				}
			}
			else {
				file2.free_file_mem(file_hdr2);
				return HS_FAIL;
			}
		}
		file2.free_file_mem(file_hdr2);
	}
	return HS_FAIL;
} //! ExactUSBGenDownloadCmd

/*-----------------------------------------------------------------------------
| Function    : UARTLoad2nd()
+------------------------------------------------------------------------------
| Description : Load 2nd file to DDR and jump to 2nd
|
| Parameters  : nPort=com port, nTimeout...->is timeout, ndFilePath= 2ns file path
|
| Returns     : int, 1 represent success
+----------------------------------------------------------------------------*/
int CmdCtrl::UARTLoad2nd(int nPort, int nTimeout, char* ndFilePath)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char cFile_name[255];
	memset(cFile_name, 0, sizeof(cFile_name));
	Dl_Filehandler file;
	T_FILE_HDR *file_hdr;
	U8 *File_Data;
	DWORD size = 0;

	U8 ASIC_ID[ASIC_SIZE_UART];
	U8 boot_message[4];
	U8 size_command[4];
	U8 general_message[4];
	boot_message[0] = 0x02;
	boot_message[1] = 0x00;
	boot_message[2] = 0x03;
	boot_message[3] = 0xF0;

	if (file.read_file(ndFilePath, &file_hdr) == FAULT) {
		return HS_FILE_NOT_FOUND;
	}
	File_Data = (file_hdr->section_hdr[0]).data_ptr;
	size = (file_hdr->section_hdr[0]).sct_size;
	size_command [0] = (byte)((size & 0xFF));
	size_command [1] = (byte)((size & 0xFF00) >> 8);
	size_command [2] = (byte)((size & 0xFF0000) >> 16);
	size_command [3] = (byte)(size >> 24);
	//open device
	int Count = 0;
	while (m_pCommDrv->open(nPort, 115200, EVENPARITY, ONESTOPBIT, 8) < 0) {
		Count++;
		if (Count > nTimeout / 100) {
			return HS_OPEN_DEVICE_FAIL;
		}
		Sleep(11);
	}

	//read ASIC
	memset(ASIC_ID, 0, sizeof(ASIC_ID));
	if (m_pCommDrv->read(ASIC_ID, ASIC_SIZE_UART, 20000) != ASIC_SIZE_UART) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_READ_DATA_FAIL;
	}
	// Send the Flash Command
	if (m_pCommDrv->write(boot_message, sizeof(boot_message)) != sizeof(boot_message)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}

	//send size command
	if (m_pCommDrv->write(size_command, sizeof(size_command)) != sizeof(size_command)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	//write 2nd
	if (m_pCommDrv->write(File_Data, size) != (S32)size) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}

	//ECHO COMMAND
	general_message[0] = 0x80;
	general_message[1] = 0x55;
	general_message[2] = 0x55;
	general_message[3] = 0xFF;
	//do echo test
	if (m_pCommDrv->write(general_message, sizeof(general_message)) != sizeof(general_message)) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	Sleep(300);
	while (m_pCommDrv->read(general_message, 1, 500) == 1) {
		general_message[1] = 0;
	}
	//because the target change its status, we should close the device to reset UART
	m_pCommDrv->close();

	//-------------------Phase 2-in 2nd Menu ------------------------------------------------------------
	//Open Connect
	Count = 0;
	while (m_pCommDrv->open(nPort, 115200, NOPARITY, ONESTOPBIT, 8) < 0) {
		Count++;
		if (Count > nTimeout / 100) {
			return HS_OPEN_DEVICE_FAIL;
		}
		Sleep(11);
	}
	Sleep(300);
	//RunProgram (jump to 2nd)
	general_message[1] = 0;
	general_message[0] = 'A';
	if (m_pCommDrv->write(general_message, 1) != 1) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}

	while (m_pCommDrv->read(general_message, 1, 1000) == 1); // clean buffer
	file.free_file_mem(file_hdr);
	//FF:因為UART TiLoadFile沒有open device的動作所以這行留著 1221
	//	m_pCommDrv->close();

	return 1;
}

/*-----------------------------------------------------------------------------
| Function    : UARTTILoadFile()
+------------------------------------------------------------------------------
| Description : Load DT file to DDR and jump to DT
|
| Parameters  : nPort=com port, nTimeout...->is timeout, DDRFileName= DT file path, StartAddr=DT start address
|
| Returns     : int, 1 represent success
+----------------------------------------------------------------------------*/
int CmdCtrl::UARTTILoadFile(int nPort, int nTimeout, char* DDRFileName, unsigned int StartAddr)
{
	unsigned char cFile_name[255];
	memset(cFile_name, 0, sizeof(cFile_name));
	Dl_Filehandler file;
	T_FILE_HDR *file_hdr;
	U8 *File_Data;
	int nRetCode;
	DWORD size = 0;

	//read DT file
	nRetCode = file.read_file(DDRFileName, &file_hdr);
	if (nRetCode == FAULT) {
		return HS_FILE_NOT_FOUND;
	}
	File_Data = (file_hdr->section_hdr[0]).data_ptr;
	size = (file_hdr->section_hdr[0]).sct_size;

	//send 2 to 2nd, prepare to load DT
	U8 general_message[4];
	general_message[1] = 0;
	general_message[2] = 0;
	general_message[3] = 0;
	if (m_pCommDrv->write(general_message, 1) != 1) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return HS_WRITE_DATA_FAIL;
	}
	Sleep(300);
	while (m_pCommDrv->read(general_message, 1, 1000) == 1); //clean buffer

	//downalod
	int ReceiveResult = UARTPutHugeData(m_pCommDrv, 1, File_Data, size, 10);
	if (ReceiveResult < HS_OK) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return ReceiveResult;
	}

	//get downlaod result packets
	if (m_pCommDrv->read(general_message, 3, 10000) != 3) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return -6;
	}
	//check result
	if (general_message[0] != 0x11 &&  general_message[1] == 0x01) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return -6;
	}

	Sleep(500);
	while (m_pCommDrv->read(general_message, 1, 1000) == 1); // clean buffer

	general_message[0] = '9';
	general_message[1] = 0;
	general_message[2] = 0;
	general_message[3] = 0;
	if (m_pCommDrv->write(general_message, 1) != 1) {
		m_pCommDrv->close();
		file.free_file_mem(file_hdr);
		return -1;
	}

	Sleep(300);
	while (m_pCommDrv->read(general_message, 1, 1000) == 1); // clean buffer
	m_pCommDrv->close();
	file.free_file_mem(file_hdr);

	return HS_OK;
}
/*-----------------------------------------------------------------------------
| Function    : UARTPutHugeData()
+------------------------------------------------------------------------------
| Description : Use in UARTTILoadFIle(), i guess UART protocol is different to USB, so YT implement this
|
| Parameters  : *pCom=UART obj, PortNum=com port, pPackData=DT image, Length=image length, TimeOut= timeout
|
| Returns     : int, 1 represent success
+----------------------------------------------------------------------------*/
int CmdCtrl::UARTPutHugeData(BaseDrv *pCom, int PortNum, unsigned char * pPackData, int Length, int TimeOut)
{
	unsigned char FileSize[10];
	unsigned char OldChecksum;
	unsigned char NewChecksum;
	int SendResult;
	int ReceiveResult;
	int LastPacketNum = 0;
	int TotalFileSize = 0;
	int LastPacketSize = 0;
	int ErrorCount = 0;
	//由此開始,傳送大量資料給target端
	//先傳送size packet,再接收size packet 的return packet
	int tsize = Length;
	FileSize[0] = (unsigned char)tsize;
	FileSize[1] = (unsigned char)(tsize >> 8);
	FileSize[2] = (unsigned char)(tsize >> 16);
	FileSize[3] = (unsigned char)(tsize >> 24);

	SendResult = UARTGenPacketSend(pCom, 0x00, FileSize, pPackData, &OldChecksum, PACKET_SIZE_GENCMD);
	if (SendResult < HS_OK) {
		return SendResult;
	}
	Sleep(1000);
	//此處FileSize此時已經變成sector了,沒用到
	ReceiveResult = UARTGenPacketRecv(pCom, 0x00, NULL, pPackData, &NewChecksum, 3);
	if (ReceiveResult != HS_OK) {
		return ReceiveResult;
	}
	//再來這邊應該要check ㄧ下chksum吧
	if (OldChecksum != NewChecksum) {
		return HS_INVALID_CHECKSUM;
	}

	//再來傳送data packet
	//此處的寫法要注意一下,最好能自動切
	//根據filesize算出封包數,記下last data size並將
	// 陣列中的data一一copy到
	TotalFileSize +=  (unsigned int)FileSize[0];
	TotalFileSize +=  (unsigned int)FileSize[1] << 8;
	TotalFileSize +=  (unsigned int)FileSize[2] << 16;
	TotalFileSize +=  (unsigned int)FileSize[3] << 24;
	LastPacketSize = TotalFileSize % SECTOR_SIZE;
	if (LastPacketSize == 0) LastPacketSize = SECTOR_SIZE;
	LastPacketNum  = (TotalFileSize - 1) / SECTOR_SIZE;//number=1時實際代表有2個packet
	Sleep(11);

	//注意,切割封包應該在外面就作完了,裡面應該只允許單獨的封包
	//來送出,所以傳進來的應該是secotr number(for
	//之前的send package是指送一個封包,所以如果把定義改一下
	// unsigned char CheckSum;
	// int j;
	int DataSize;
	unsigned char SectorNum[4];
	for (int i = 0; i <= LastPacketNum;) {
		SectorNum[0] = (unsigned char)i;
		SectorNum[1] = (unsigned char)(i >> 8);
		SectorNum[2] = (unsigned char)(i >> 16);
		SectorNum[3] = (unsigned char)(i >> 24);
		DataSize = (i == LastPacketNum) ? LastPacketSize : SECTOR_SIZE; //for header and crc
		SendResult = UARTGenPacketSend(pCom, 0x10, SectorNum, pPackData + SECTOR_SIZE * i, &OldChecksum, DataSize);
		if (SendResult < HS_OK) {
			return SendResult;
		}
		ReceiveResult = UARTGenPacketRecv(pCom, 0x10, NULL, pPackData, &NewChecksum, 3);
		if (ReceiveResult == 0) {
			ErrorCount++;
			Sleep(5000);
			continue;
		}

		if (ErrorCount > 2) { //!!! timeout仍然繼續做
			CString strOutDrive2;
			strOutDrive2.Format("Sending packet %d  of %d  Fail\r\n", i, LastPacketNum);
			AfxMessageBox(strOutDrive2);
			return ReceiveResult;
		}
		if (OldChecksum == NewChecksum) {
			i++;
		}
		else {
			ErrorCount++;
		}
	} // transfered all data packet

	return HS_OK;
}

/*-----------------------------------------------------------------------------
| Function    : UARTGenPacketRecv()
+------------------------------------------------------------------------------
| Description : Use in UARTPutHugeData(), to send data
|
| Parameters  : *pCom=UART obj, 其他...??? 等我比較完再寫
|
| Returns     : int, 1 represent success
+----------------------------------------------------------------------------*/
int CmdCtrl::UARTGenPacketRecv(BaseDrv* pCom, int Function, unsigned char * SectorNumber, unsigned char * pReceiveData, unsigned char * NChecksum, int DataSize)
{
	//外面:while (depackage())如果回傳是true就一值解封包或
	//一直接收檔案,直到抓到指令,如果timeout 就直接結束
	//1.收到封包後解譯,並將結果利用packdata陣列傳出去
	int TimeOutCount = 0;
	unsigned char data[4] = {0};
	unsigned char temp[5000];
	memset(&temp, 0, sizeof(temp));
	while (1) {
		//嘗試接收第一個byte,並辨認是否為指定Function的封包header
		//部會立即返回
		// TODO: set timeout
		if (pCom->read(data, 1, 2000) != 1) {
			return HS_READ_DATA_FAIL;
		}

		TimeOutCount = 0;
		temp[0] = data[0];
		//===========================
		//本來是 temp[0]== Function
		//但為了能同時接收0x00 0x01 function packet
		//只好改成  temp[0]<2 以符合
		if (temp[0] == Function) {
			//要收DataSize+1 個,因為最後還有1個checksum
			for (int i = 1; i < DataSize; i++) {
				if (pCom->read(data, 1, 1000) != 1) {
					return HS_READ_DATA_FAIL;
				}
				TimeOutCount = 0;
				temp[i] = data[0];

				//這裡只有把Receive的checksum丟出去
				//而沒有check整個data的checksum
				//在RX地方出去後有CHECK,但tx則否
				if (i == DataSize - 1) {
					if (DataSize == PACKET_SIZE_RETURN) {
						*NChecksum = temp[1];
						return HS_OK;
					}
					else {
						*NChecksum = temp[i];
						memcpy(pReceiveData, temp, DataSize);
					}
					return HS_OK;
				}

			}  //for end
		}//if (order == true) end
	}  //whole pack ok?

	return HS_FAIL;
}

/*-----------------------------------------------------------------------------
| Function    : UARTGenPacketSend()
+------------------------------------------------------------------------------
| Description : Use in UARTPutHugeData(), to send data
|
| Parameters  : *pCom=UART obj, 其他...??? 等我比較完再寫
|
| Returns     : int, 1 represent success
+----------------------------------------------------------------------------*/
int CmdCtrl::UARTGenPacketSend(BaseDrv *pCom, int Function, unsigned char * FileSize, unsigned char * pPackData, unsigned char * OChecksum, int DataSize)
{
	unsigned char temp[5000];
	memset(&temp, 0, sizeof(temp));
	//檢查
	//1.packdata是否有比temp長,ex超過設定上限
	//2.注意packetsize是否比packdata大
	if (DataSize == PACKET_SIZE_RETURN) {
		//return or answer
		temp[0] = Function;
		temp[1] = pPackData[0];
		temp[2] = temp[0] ^ temp[1];
		if (pCom->write(temp, PACKET_SIZE_RETURN) != PACKET_SIZE_RETURN) {
			return HS_WRITE_DATA_FAIL;
		}
		return HS_OK;
	}

	//應該要改成CHECK SIZE PACKET的FORMAT才對
	char CheckSum;
	if ((Function == 0x00) && (DataSize != 3)) {
		//即Size Packet
		temp[0] = Function;
		for (int i = 0; i < 4; i++) {
			temp[i+1] = FileSize[i];
		}
		CheckSum = temp[0];
		int j;
		for (j = 1;j < 5;j++) {
			CheckSum ^= temp[j];
		}
		temp[j] = CheckSum;
		*OChecksum =  CheckSum;
		if (pCom->write(temp, PACKET_SIZE_GENCMD) != PACKET_SIZE_GENCMD) {
			return HS_WRITE_DATA_FAIL;
		}

		//以下的大量傳輸是給XXX用的
	}
	else if ((Function == 0x10) && (DataSize != 3)) {
		temp[0] = Function;
		for (int k = 0;k < 4;k++) {
			//此處的FileSize是secotr number
			temp[k+1] = FileSize[k];
		}
		int m;
		memcpy(temp + 5, pPackData, DataSize);
		CheckSum = temp[0];
		for (m = 1;m < DataSize + 5;m++) {
			CheckSum ^= temp[m];
		}
		temp[m] = CheckSum;
		*OChecksum =  CheckSum;
		if (pCom->write(temp, PACKET_SIZE_GENCMD + DataSize) != (PACKET_SIZE_GENCMD + DataSize)) {
			return HS_WRITE_DATA_FAIL;
		}
	} //if data packet

	return HS_OK;
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

int CmdCtrl::ExactGeneralCmdIn(int nPort, int nTimeout,
							   char* CtrlCmd, char* InBuffer, size_t nInLength)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	//所以Exact_GeneralCmdIn要用傳進來的InBuffer的size大小
	int nPayloadSize = static_cast<int>(nInLength);//size = 4
	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	unsigned char pPackData[PAYLOAD_SIZE];
	memset(pPackData, 0, sizeof(pPackData));

	//Open Device
	cGenPayload[0] = CtrlCmd[0];
	cGenPayload[1] = CtrlCmd[1];
	cGenPayload[2] = CtrlCmd[2];
	cGenPayload[3] = CtrlCmd[3];

	//FFYang
	//cGenPayload 應該是要加參數的地方 0926
	//第三個參數改成InBuffer 要轉個型

	if (nPort >= QTH_PORT_USB) {
		m_bIfUSBCmd = true;
	}
	int nRetCode = QthGenCommand(nPayloadSize, cGenPayload, (unsigned char *)InBuffer, pPackData, nPort);
	if (m_bIfUSBCmd) {
		m_bIfUSBCmd = false;
	}
	if (nRetCode != HS_OK) {
		TRACE(_T("%s QthGenCommand(%d) fail\r\n"), __FUNCTION__, nRetCode);
		return nRetCode;
	}

	return HS_OK;
}
//============================================================================//
//Add by FF 1016
int CmdCtrl::ExactUSBGenUploadCmd(int nPort, int nTimeout, char* CtrlBuffer, char* WorkBuffer)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;//用來取得算出來的Checksum
	unsigned char cGenPayload[4096];
	int iBlockNum = 0;
	//存傳回結果
	unsigned char pPackData[9000];
	int PayloadSize = 0;//size =
	memset(pPackData, 0, sizeof(pPackData));
	memset(cGenPayload, 0, sizeof(cGenPayload));
	//-------------Initial ULBuffer Command Start

	//init buffer,block size是gobal varible,default是8192
	//如果不是8192就要先下16-33去改
	//現在這階段很尷尬 應該都要用8192
	//但是目前的F1.002這個版本的block size是2048
	//////////////////////////
	//以後下面的code的block size要改成動態的,default是2048
	//for General Download
	//16-33 Init Buffer Size 512
	cGenPayload[0] = 16;
	cGenPayload[1] = 33;
	cGenPayload[2] = 0;
	cGenPayload[3] = 0;
	//total initial buffer 1024
	cGenPayload[4] = '0';
	cGenPayload[5] = '0';
	cGenPayload[6] = '0';
	cGenPayload[7] = '0';
	cGenPayload[8] = '0';
	cGenPayload[9] = '0';
	cGenPayload[10] = '0';
	cGenPayload[11] = '0';
	//注意init buffer應該要改成P21GeneralCmd才對
	//寫死的,注意要改掉
	cGenPayload[12] = '0';
	cGenPayload[13] = '0';
	cGenPayload[14] = '0';
	cGenPayload[15] = '0';
	////////////////////////////////
	cGenPayload[16] = '0';
	cGenPayload[17] = 8 + '0';
	////////////////////////////////
	cGenPayload[18] = '0';
	cGenPayload[19] = '0';
	//對於DL來說PayloadSize一定要16byte
	//這是protocol不能改
	PayloadSize = 16;

	//cGenPayload的前四個bit command,後面的才是data
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	if (SendResult != HS_OK) {
		TRACE(_T("%s QthGenPacketSend(%d) fail\r\n"), __FUNCTION__, SendResult);
		return SendResult;
	}
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		return ReceiveResult;
	}

	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		return HS_INVALID_CHECKSUM;
	}
	Sleep(20);
	ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
	int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
	if (nRetCode !=  HS_OK) {
		return nRetCode;
	}

	//-------------Initial Buffer Command End
	//PayloadSize = strlen(WorkBuffer);
	PayloadSize = 4;
	cGenPayload[0] = CtrlBuffer[0];
	cGenPayload[1] = CtrlBuffer[1];
	cGenPayload[2] = CtrlBuffer[2];
	cGenPayload[3] = CtrlBuffer[3];
	cGenPayload[4] = '2';
	cGenPayload[5] = '0';
	cGenPayload[6] = '4';
	cGenPayload[7] = '8';

	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		return ReceiveResult;
	}
	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		return HS_INVALID_CHECKSUM;
	}

	//// 2008-02-25 Nek Wang: clear progress
	for (;;) {
		Sleep(20);
		memset(pPackData, 0, sizeof(pPackData));
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
		nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			int ij = -10;
			int sum = 0;
			// TODO: check here, it may use sscanf %x instead of the following
			for (int i = 8; i > 0; i--) {
				AsciiToBinary(pPackData[i], &ij);
				sum = sum + ij * intergePower(16, (8 - i));
			}
			if (sum % 2048 != 0) {
				iBlockNum = (sum / 2048) + 1;
			}
			else {
				iBlockNum = (sum / 2048);
			}
			break;
		}
		else if (nRetCode != HS_PROGRESS) {
			return HS_FAIL;
		}
	}
	//-------------Send Command End

	//open binary file to write
	//FF:if camera get frame 則利用外面傳進來的檔名和路徑去存檔
	ofstream ofile;
	//FF:camera function的buffer
	unsigned char * bPicDataBuffer = NULL;
	//FF:加一個buffer buffer滿了在進行寫入的動作 or 收失敗了就進行寫入的動作
	unsigned char * cDataBuffer = NULL;

	if (CtrlBuffer[0] == P21_DT_CAMERA) {
		if (CtrlBuffer[1] == 3) {
			bPicDataBuffer = new unsigned char[160*120*2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+2048];
			memset(bPicDataBuffer, 0, sizeof(160*120*2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2048));
		}
		else if (CtrlBuffer[1] == 4) {
			bPicDataBuffer = new unsigned char[240*180*2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+2048];
			memset(bPicDataBuffer, 0, sizeof(240*180*2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2048));
		}
		else if (CtrlBuffer[1] == 5) {
			bPicDataBuffer = new unsigned char[640*480*2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+2048];
			memset(bPicDataBuffer, 0, sizeof(640*480*2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2048));
		}
	}
	else {
		ofile.open(WorkBuffer, ios::binary);
		cDataBuffer = new unsigned char[1024*1024*2];
		memset(cDataBuffer, 0, sizeof(cDataBuffer));
	}

	//receive data
	memset(cGenPayload, 0, sizeof(cGenPayload));
	int index = 0;
	int retransfer = 0;
	while (index < iBlockNum) {
		memset(cGenPayload, 0, sizeof(cGenPayload));
		memset(pPackData, 0, sizeof(pPackData));
		cGenPayload[0] = (unsigned char)(index >> 24);
		cGenPayload[1] = (unsigned char)(index >> 16);
		cGenPayload[2] = (unsigned char)(index >> 8);
		cGenPayload[3] = (unsigned char)index;
		SendResult = QthGenPacketSend(P21_DATAUL_REQUEST, 0, cGenPayload, &OldChecksum, nPort);

		//先收掉重複的封包 如果有的話
		if (retransfer > 0) {
			for (int i = 0; i < retransfer; i++) {
				ReceiveResult = QthGenPacketRecv(P21_DATAUL_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
				if (ReceiveResult == 1) {
					retransfer--;
				}
				else {
					retransfer++;
					if (retransfer < 5) {
						continue;
					}
					//FF:delete buffer
					if (NULL != cDataBuffer) {
						delete[] cDataBuffer;
					}
					if (NULL != bPicDataBuffer) {
						delete[] bPicDataBuffer;
					}
					ofile.close();
					return ReceiveResult;
				}
			}
		}

		//收要存檔的封包
		ReceiveResult = QthGenPacketRecv(P21_DATAUL_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
		if (ReceiveResult == 1) {

			// 2007-07-17 YTT: 每收2mb就寫入
			if ((index != 0) && ((index % 1024) == 0)) {
				//FF:只要不是camera function 如果buffer滿了 寫入
				if (!(CtrlBuffer[0] == P21_DT_CAMERA)) {
					ofile.write(reinterpret_cast<char*>(cDataBuffer), 1024*1024*2);
				}
			}
			//FF:一定要把data放入buffer
			if (CtrlBuffer[0] == P21_DT_CAMERA) {
				memcpy(bPicDataBuffer + ((index % 1024)*2048), pPackData, 2048);
			}
			else {
				memcpy(cDataBuffer + ((index % 1024)*2048), pPackData, 2048);
			}
		}
		else {
			retransfer++;
			if (retransfer < 5) {
				continue;
			}
			//FF:delete buffer
			if (NULL != cDataBuffer) {
				delete[] cDataBuffer;
			}
			if (NULL != bPicDataBuffer) {
				delete[] bPicDataBuffer;
			}
			ofile.close();
			return ReceiveResult;
		}
		index++;
		retransfer = 0;
	} //for loop end
	//FF:index不一定都剛好整除 所以外面要寫剩餘的檔案
	if ((index % 1024) != 0) {
		if (CtrlBuffer[0] != P21_DT_CAMERA) {
			ofile.write(reinterpret_cast<char*>(cDataBuffer), (index % 1024)*2048);
		}
	}

	//FF:若index剛好整除1024 裡面不會寫
	//FF:所以外面要寫最後一個cDataBuffer的size大小
	if (((index % 1024) == 0) && (index != 0)) {
		if (CtrlBuffer[0] != P21_DT_CAMERA) {
			ofile.write(reinterpret_cast<char*>(cDataBuffer), 1024*1024*2);
		}
	}

	if (CtrlBuffer[0] == P21_DT_CAMERA) {
		if (CtrlBuffer[1] == 3) {
			//FF:資料前後交換
			unsigned char * cOutput = new unsigned char[160*120*3];
			ConvertDTImageData(160, 120, bPicDataBuffer, cOutput);
			SaveBMP(160, 120, cOutput, WorkBuffer);
			delete[] cOutput;
		}
		else if (CtrlBuffer[1] == 4) {
			unsigned char * cOutput = new unsigned char[240*180*3];
			ConvertDTImageData(240, 180, bPicDataBuffer, cOutput);
			SaveBMP(240, 180, cOutput, WorkBuffer);
			delete[] cOutput;
		}
		else if (CtrlBuffer[1] == 5) {
			unsigned char * cOutput = new unsigned char[640*480*3];
			ConvertDTImageData(640, 480, bPicDataBuffer, cOutput);
			SaveBMP(640, 480, cOutput, WorkBuffer);
			delete[] cOutput;
		}
	}
	//FF:delete buffer
	if (NULL != cDataBuffer) {
		delete[] cDataBuffer;
	}
	if (NULL != bPicDataBuffer) {
		delete[] bPicDataBuffer;
	}
	ofile.close();

	return HS_OK;
}

int CmdCtrl::EnableDTR(int nPort, int TimeOut)
{
	HANDLE hCom;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		CString comport;
		comport.Format("COM%d", nPort);
		hCom = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else {
		hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	}

	DCB dcb;
	if (!GetCommState(hCom, &dcb)) {
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	if (!SetCommState(hCom, &dcb)) {
		return HS_SET_DEVICE_FAIL;
	}
	if (!SetupComm(hCom, 1600, 1600)) {
		return HS_SET_DEVICE_FAIL;
	}

	Sleep(2);
	if (!m_isOpen) {
		m_isGeneralCmd = false;
		CloseHandle(hCom);
	}

	return HS_OK;
}

int CmdCtrl::DisableDTR(int nPort, int TimeOut)
{
	HANDLE hCom;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		CString comport;
		comport.Format("COM%d", nPort);
		hCom = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else {
		hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	}
	DCB dcb;
	if (!GetCommState(hCom, &dcb)) {
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb)) {
		return HS_SET_DEVICE_FAIL;
	}
	if (!SetupComm(hCom, 1600, 1600)) {
		return HS_SET_DEVICE_FAIL;
	}

	if (!m_isOpen) {
		m_isGeneralCmd = false;
		CloseHandle(hCom);
	}

	return HS_OK;
}

int CmdCtrl::ComDtrCtrl(int nPort, int TimeOut, bool isEnable)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, TimeOut);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb)) {
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fDtrControl = isEnable ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb)) {
		return HS_SET_DEVICE_FAIL;
	}
	return HS_OK;
}

int CmdCtrl::ComRtsCtrl(int nPort, int TimeOut, bool isEnable)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, TimeOut);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb)) {
		return HS_GET_DEVICE_FAIL;
	}
	dcb.fRtsControl = isEnable ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
	if (!SetCommState(hCom, &dcb)) {
		return HS_SET_DEVICE_FAIL;
	}
	return HS_OK;
}

int CmdCtrl::ComTxCtrl(int nPort, int TimeOut, byte* pBuffer, size_t nLength)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, TimeOut);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}
	HANDLE hCom = ((SerialDrv*)m_pCommDrv)->getHandle();
	DCB dcb;
	if (!GetCommState(hCom, &dcb)) {
		return HS_GET_DEVICE_FAIL;
	}
	dcb.BaudRate = BAUD_2400;
	if (!SetCommState(hCom, &dcb)) {
		return HS_SET_DEVICE_FAIL;
	}
	if (m_pCommDrv->write(pBuffer, static_cast<U32>(nLength)) != nLength) {
		return HS_WRITE_DATA_FAIL + P21_ERR_NORMAL_COMMAND;
	}
	return HS_OK;
}

int CmdCtrl::NormalCmdPowerOnToUARTPacketMode(int nPort, int TimeOut, int BMS_flag)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	unsigned char pPackData[5] = {'A', 'A', 'A', 'A', 'A'};
	unsigned char PackDataA = 'A';
	int nRetCode = 0;
	//如果收到三個z就是成功
	int power_to_uart_packet_mode_counter = 0;
	bool recieve_continuous_flag = false;
	int time_counter = 0;
	int sleep_time = 20;

	//一直收 怕有雜訊 收到沒有 或者是有三個連續的zzz就return
	if (BMS_flag == 1) {
		nRetCode = EnableDTR(nPort, TimeOut);
		if (!m_isOpen) {
			nRetCode = open(nPort, TimeOut);
			if (nRetCode != HS_OK) {
				return nRetCode + P21_ERR_OPENDEVICE;
			}
		}
	}
	else if (!m_isOpen) {
		m_isGeneralCmd = true;
		nRetCode = open(nPort, TimeOut);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}

	// YT 2006-11-10 Modify for bala test
	while (time_counter < 2500) {
		// YTT 2006-11-10 move to top for continue control
		time_counter = time_counter + sleep_time;
		Sleep(sleep_time);

		//// send 5 'A'
		if (m_pCommDrv->write(pPackData, 5) != 5) {
			return HS_WRITE_DATA_FAIL + P21_ERR_NORMAL_COMMAND;
		}

		// YTT 2006-11-10 clear control
		//skip all dirty data
		//only if get 'Z' or timeout can pass the phase
		//the code is for temp only
		//FF:有收到 檢查為Z 才跳出
		PackDataA = 0;
		while (m_pCommDrv->read(&PackDataA, sizeof(PackDataA), 100) == 1) {
			if (PackDataA == 'Z') {
				break;
			}
		}

		//FF:上面的收訊息 timeout 我覺得好像不需要耶...下面就會判斷了
		//FF:而且最上面會有timeout count
		if (PackDataA == 0) {
			continue;
		}
		if (PackDataA == 'Z') {
			if (recieve_continuous_flag) { //true代表上一次也是A所以累加
				power_to_uart_packet_mode_counter++;
			}
			else { //false代表上一次不是是A所以=1
				power_to_uart_packet_mode_counter = 1;
			}
			// 收到 Z flag 設為 true, 三個 Z 成功
			recieve_continuous_flag = true;
			if (power_to_uart_packet_mode_counter == 3) {
				Sleep(500); // sleep 500 ms asked by eric and return
				if (BMS_flag == 1) {
					nRetCode = DisableDTR(nPort, TimeOut);
					if (!m_isOpen) {
						close(nPort, TimeOut);
						m_isGeneralCmd = false;
					}
				}
				else if (!m_isOpen) {
					close(nPort, TimeOut);
					m_isGeneralCmd = false;
				}

				return HS_OK;
			}
		}
		else { // 收到不是A flag設為false
			recieve_continuous_flag = false;
		}
	}
	if (!m_isOpen) {
		m_isGeneralCmd = false;
		close(nPort, TimeOut);
	}

	return P21_ERR_NORMAL_COMMAND;
}

int CmdCtrl::ExactGeneralCmdInOut(int nPort, int nTimeout, char* CtrlBuffer, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	Sleep(11);
	//所以Exact_GeneralCmdIn要用傳進來的InBuffer的size
	/* 111020 Gilpin modify start(add in buffer size parameter transfer) */
	int PayloadSize = (int)i_InLength;
	//int PayloadSize = (int)strlen(InBuffer);
	/* 111020 Gilpin modify end(add in buffer size parameter transfer) */

	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	unsigned char pPackData[PAYLOAD_SIZE];
	memset(pPackData, 0, sizeof(pPackData));
	cGenPayload[0] = CtrlBuffer[0];
	cGenPayload[1] = CtrlBuffer[1];
	cGenPayload[2] = CtrlBuffer[2];
	cGenPayload[3] = CtrlBuffer[3];

	//FFYang
	//cGenPayload 應該是要加參數的地方 0926
	//第三個參數改成InBuffer 要轉個型
	if (nPort >= QTH_PORT_USB) {
		m_bIfUSBCmd = true;
	}
	int nRetCode = QthGenCommand(PayloadSize, cGenPayload, (unsigned char *)InBuffer, pPackData, nPort);
	if (m_bIfUSBCmd) {
		m_bIfUSBCmd = false;
	}
	//萬一傳回值比較大怎麼辦...??一定會錯
	//所以還是有兩個參數會比較好
	memset(OutBuffer, 0, sizeof(OutBuffer));
	memcpy(OutBuffer, pPackData, PAYLOAD_SIZE);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s QthGenCommand(%d) fail\r\n"), _T(__FUNCTION__), nRetCode);
		return nRetCode;
	}

	return HS_OK;
}

long CmdCtrl::ConvertImageToBmpFile(S_Image udtImage, byte* pbytSource, byte* pbytOutput, int iRow, int iColum)
{
	long lReturnLen = 0;
	BITMAPFILEHEADER bmFileHeader;
	BITMAPINFOHEADER bmInfoHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmInfoHeader, 0, sizeof(BITMAPINFOHEADER));

	long lSize = udtImage.lSize;
	byte* pbytBuffer2;
	pbytBuffer2	= new byte[lSize * 3];
	long lBufSize = lSize;
	int iBmpWidth = 0;

	sprintf((char*)&(bmFileHeader.bfType), "BM");
	bmFileHeader.bfSize = lBufSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmInfoHeader.biHeight = udtImage.lResolutionY;
	bmInfoHeader.biWidth = udtImage.lResolutionX;
	bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfoHeader.biPlanes = 1;
	bmInfoHeader.biBitCount = 24;
	bmInfoHeader.biCompression = BI_RGB;

	long lOffSet = 0;
	memcpy(pbytOutput + lOffSet, &bmFileHeader, sizeof(BITMAPFILEHEADER));
	lOffSet += sizeof(BITMAPFILEHEADER);
	memcpy(pbytOutput + lOffSet, &bmInfoHeader, sizeof(BITMAPINFOHEADER));
	lOffSet += sizeof(BITMAPINFOHEADER);
	memcpy(pbytOutput + lOffSet, pbytSource, lBufSize);

	delete[] pbytBuffer2;

	lReturnLen = lBufSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	return	lReturnLen;
}

BOOL CmdCtrl::SaveBMP(int iRow, int iColum, unsigned char * SourceData, char* FileName)
{
	CHAR szCurDir[_MAX_DIR] = "\0";
	GetCurrentDirectory(_MAX_DIR, szCurDir);
	CString cstrPathName;

	byte *GetOutputData;
	GetOutputData = new byte[iRow*iColum*3*2+2048];

	S_Image Imageresq;
	Imageresq.lResolutionX = iRow;
	Imageresq.lResolutionY = iColum;
	Imageresq.lSize = iRow * iColum * 3;

	long lBuffer;
	lBuffer	= ConvertImageToBmpFile(Imageresq, SourceData, GetOutputData, iRow, iColum);

	ofstream ofile;
	ofile.open(FileName, ios::binary);
	ofile.write(reinterpret_cast<char*>(GetOutputData), iRow*iColum*3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	ofile.close();
	delete[] GetOutputData;

	return TRUE;
}

int CmdCtrl::ConvertDTImageData(int iRow, int iColum, unsigned char * source, unsigned char * output)
{
	char* cSwap3 = new char[iRow*iColum*2];
	char* cSwap2 = new char[iRow*iColum*2];

	int j = iRow * iColum * 2 - 1;
	int i = 0;
	while (i < iRow*iColum*2) {
		cSwap3[i] = source[j-1];
		cSwap3[i+1] = source[j];
		i = i + 2;
		j = j - 2;
	}

	j = 0;
	i = 0;
	while (j < iRow*iColum*2) {
		while (i < iRow*2) {
			cSwap2[j+i] = cSwap3[j+iRow*2-i-2];
			cSwap2[j+i+1] = cSwap3[j+iRow*2-i-1];
			i = i + 2;
		}
		i = 0;
		j = j + iRow * 2;
	}

	size_t nSize = iRow * iColum * 2;
	Rgb565ToRgb(output, (byte*)cSwap2, &nSize);

	delete[] cSwap3;
	delete[] cSwap2;

	return 1;
}

int CmdCtrl::BDTLSectorDownload(int nPort, int nTimeout, char* DLType, char* FilePath, char* parameter)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}

	}
	nRetCode = ExactBDTLSectorDownloadCmd(nPort, nTimeout, DLType, FilePath, parameter);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactBDTLSectorDownloadCmd(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode;
	}
	if (!m_isOpen) {
		Sleep(11);
		m_isGeneralCmd = false;
		close(nPort, nTimeout);
	}

	return HS_OK;
}

int CmdCtrl::ExactBDTLSectorDownloadCmd(int nPort, int nTimeout, char* CmdNumber, char* FilePath, char* parameter)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	Sleep(200);
	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;//用來取得算出來的Checksum
	//type and block num(4)  Data(512)
	unsigned char cGenPayload[16384];
	unsigned char pPackData[256];
	int PayloadSize = 0;

	Dl_Filehandler file2;
	T_FILE_HDR *file_hdr2;
	U8 *File_Data2;

	int nRetCode = 0;
	DWORD FileSize = 0;
	//=============================================================================
	//level-2 for flash writer
	//-----------------------------------------------------------------------
	nRetCode = file2.read_file(FilePath, &file_hdr2);
	if (nRetCode == FAULT) {
		file2.free_file_mem(file_hdr2);
		return HS_FILE_NOT_FOUND;
	}
	//注意實際要傳時得沏,每個block 大小為0x6000
	File_Data2 = (file_hdr2->section_hdr[0]).data_ptr;
	FileSize = (file_hdr2->section_hdr[0]).sct_size;
	//Create the size command (part of OMAP protocol!)
	// NC - must be little endian for UART - doesn't matter for USB in current devices.
	// but it could matter for future devices.
	//----------------------------------------------------------------
	//Initial buffer command as a kind of GeneralCmdIn() with P21GeneralCmd
	//Default BlockSize

	DWORD dwBlockSize = DT_USB_BLOCK_SIZE;
	//以後下面的code 的block size要改成動態的
	//default是8192
	//for General Download
	//16-33 Init Buffer Size 512
	cGenPayload[0] = 16;
	cGenPayload[1] = 33;
	cGenPayload[2] = 0;
	cGenPayload[3] = 0;
	//total initial buffer  1024
	//其實何必麻煩,用湊整數的就好啦
	cGenPayload[4] = (unsigned char)( (0xFF000000 & FileSize) >> 24);
	cGenPayload[6] = (unsigned char)( (0x00FF0000 & FileSize) >> 16);
	cGenPayload[8] = (unsigned char)( (0x0000FF00 & FileSize) >> 8);
	cGenPayload[10] = (unsigned char) (0x000000FF & FileSize);
	//transfer 4byte hex to 8 byte ascii

	size_t i = 0; // avoiding for loop scope bug in VC6.
	for (i = 0; i < 4; i++) {
		cGenPayload[5+2*i] = cGenPayload[4+2*i] % 16;
		if (cGenPayload[5+2*i] < 10) {
			cGenPayload[5+2*i] = '0' + cGenPayload[5+2*i];
		}
		else {
			cGenPayload[5+2*i] = 'A' + cGenPayload[5+2*i] - 10;
		}
		cGenPayload[4+2*i] = cGenPayload[4+2*i] / 16;
		if (cGenPayload[4+2*i] < 10) {
			cGenPayload[4+2*i] = '0' + cGenPayload[4+2*i];
		}
		else {
			cGenPayload[4+2*i] = 'A' + cGenPayload[4+2*i] - 10;
		}
	}

	//注意init buffer應該要改成P21GeneralCmd才對
	//寫死的,注意要改掉
	//block size  2048
	cGenPayload[12] = '0';
	cGenPayload[13] = '0';
	cGenPayload[14] = '0';
	cGenPayload[15] = '0';
	////////////////////////////////
	cGenPayload[16] = '0';
	cGenPayload[17] = 8 + '0';
	//cGenPayload[16] =2+'0';
	//cGenPayload[17] ='0';
	////////////////////////////////
	cGenPayload[18] = '0';
	cGenPayload[19] = '0';
	//對於DL 來說 PayloadSize一定要16byte
	//這是protocol 不能改
	PayloadSize = 16;
	//cGenPayload的前四個bit command,後面的才是data
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	if (SendResult < HS_OK) {
		TRACE(_T("%s QthGenPacketSend(%d) fail\r\n"), __FUNCTION__, SendResult);
		file2.free_file_mem(file_hdr2);
		return SendResult;
	}
	Sleep(2);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		file2.free_file_mem(file_hdr2);
		return ReceiveResult;
	}
	//再來這邊應該要check ㄧ下chksum吧
	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		file2.free_file_mem(file_hdr2);
		return HS_INVALID_CHECKSUM;
	}
	Sleep(20);
	ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
	nRetCode = PacketResultCheck(ReceiveResult, pPackData);
	if (nRetCode !=  HS_OK) {
		file2.free_file_mem(file_hdr2);
		return nRetCode;
	}

	//-------------Initial Buffer Command End
	//----------------------------------------------
	//calculate checksum
	unsigned char cChecksum[9];
	memset(cChecksum, 0, sizeof(cChecksum));
	for (i = 0; i < FileSize; i++) {
		switch (i % 4) {
		case 0:
			cChecksum[0] = cChecksum[0] ^ File_Data2[i];
			break;
		case 1:
			cChecksum[2] = cChecksum[2] ^ File_Data2[i];
			break;
		case 2:
			cChecksum[4] = cChecksum[4] ^ File_Data2[i];
			break;
		case 3:
			cChecksum[6] = cChecksum[6] ^ File_Data2[i];
			break;
		default:
			break;
		}
	}
	for (i = 0; i < 4; i++) {
		cChecksum[1+2*i] = cChecksum[2*i] % 16;
		if (cChecksum[1+2*i] < 10) {
			cChecksum[1+2*i] = '0' + cChecksum[1+2*i];
		}
		else {
			cChecksum[1+2*i] = 'A' + cChecksum[1+2*i] - 10;
		}
		cChecksum[2*i] = cChecksum[2*i] / 16;
		if (cChecksum[2*i] < 10) {
			cChecksum[2*i] = '0' + cChecksum[2*i];
		}
		else {
			cChecksum[2*i] = 'A' + cChecksum[2*i] - 10;
		}
	}

	//send data
	int FinalSize = 0;
	PayloadSize = dwBlockSize;
	int iBlockNum = (FileSize / dwBlockSize);
	if ((FileSize % dwBlockSize) != 0) {
		iBlockNum++;
	}
	int nRetCoderansfer_Count = 0;
	FinalSize = dwBlockSize;
	int iCount = 0;

	while (1) {
		if (iCount >= iBlockNum) {
			break;
		}
		//FF: 清空buffer Send Data DL/Receive Data Ack
		memset(cGenPayload, 0, sizeof(cGenPayload));
		//FF:block number
		cGenPayload[0] = (unsigned char)(iCount >> 24);
		cGenPayload[1] = (unsigned char)(iCount >> 16);
		cGenPayload[2] = (unsigned char)(iCount >> 8);
		cGenPayload[3] = (unsigned char)iCount;
		//最後一個block 就塞滿0
		if (iCount == iBlockNum - 1) {
			FinalSize = FileSize % (dwBlockSize);
			//				 cGenPayload[0] =(unsigned char)(iCount>>24);
			//				 cGenPayload[1] =(unsigned char)(iCount>>16);
			//				 cGenPayload[2] =(unsigned char)(iCount>>8);
			//				 cGenPayload[3] =(unsigned char)iCount;

			// 2006-09-22 YTT
			// bug fixed: Checksum Fail when (file size % blocksize ==0)
			//整除少送一個封包
			if (FinalSize == 0) {
				FinalSize = dwBlockSize;
			}
		}
		//COPY Data To Buffer
		memcpy(cGenPayload + 4 , File_Data2 , FinalSize);
		SendResult = QthGenPacketSend(P21_DATADL_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
		if (SendResult != HS_OK) {
			TRACE(_T("%s QthGenPacketSend(%d) fail\r\n"), __FUNCTION__, SendResult);
			file2.free_file_mem(file_hdr2);
			return SendResult;
		}
		memset(pPackData, 0, sizeof(pPackData));
		ReceiveResult = QthGenPacketRecv(P21_DATADL_ACK, cGenPayload, pPackData, &NewChecksum);
		if (ReceiveResult != HS_OK) {
			nRetCoderansfer_Count++;
			if (nRetCoderansfer_Count < 4) {
				continue;
			}
			file2.free_file_mem(file_hdr2);
			return ReceiveResult;
		}

		if (OldChecksum != NewChecksum) {
			TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
			file2.free_file_mem(file_hdr2);
			return HS_INVALID_CHECKSUM;
		}

		//FF:如果要傳回progress就在這邊吧 1221
		//			 if(m_farCallBack)
		//			 {
		//m_farCallBack(nPort,(i*100)/iBlockNum);
		//           }
		//	 }
		//-------------------------
		//move data pointer here

		/*		if (m_farCallBackChar != NULL && (iCount % 800) == 0) {
				    m_farCallBackChar(nPort,"*");
				}
		*/
		int nProgress = ((iCount + 1) * 100 / iBlockNum) / 2;
		char cProgress[5];
		if (m_farCallBackChar != NULL) {
			//if( ( ((iCount + 1) * 100) % iBlockNum) == 0 )
			if (( ((iCount + 1) * 10) % (iBlockNum / 10)) == 0 ) {
				memset(cProgress, 0, sizeof(cProgress));
				wsprintf(cProgress, "%d ", nProgress);
				//if(CmdNumber[0]==8 && CmdNumber[1]==7 )
				m_farCallBackChar(nPort, cProgress);
			}
		}
		nRetCoderansfer_Count = 0;
		File_Data2 += PayloadSize;
		iCount++;
	}//sending data for loop end

	//FF:要清成0 不然會影響下依次的code
	nRetCoderansfer_Count = 0;
	//下16-44取得checksum
	memset(pPackData, 0, sizeof(pPackData));
	memset(cGenPayload, 0, sizeof(cGenPayload));
	cGenPayload[0] = 16;
	cGenPayload[1] = 34;
	cGenPayload[2] = 0;
	cGenPayload[3] = 0;
	cGenPayload[4] = (unsigned char)( (0xFF000000 & FileSize) >> 24);
	cGenPayload[6] = (unsigned char)( (0x00FF0000 & FileSize) >> 16);
	cGenPayload[8] = (unsigned char)( (0x0000FF00 & FileSize) >> 8);
	cGenPayload[10] = (unsigned char) (0x000000FF & FileSize);
	//transfer 4byte hex to 8 byte ascii
	for (i = 0; i < 4; i++) {
		cGenPayload[5+2*i] = cGenPayload[4+2*i] % 16;
		if (cGenPayload[5+2*i] < 10) {
			cGenPayload[5+2*i] = '0' + cGenPayload[5+2*i];
		}
		else {
			cGenPayload[5+2*i] = 'A' + cGenPayload[5+2*i] - 10;
		}
		cGenPayload[4+2*i] = cGenPayload[4+2*i] / 16;
		if (cGenPayload[4+2*i] < 10) {
			cGenPayload[4+2*i] = '0' + cGenPayload[4+2*i];
		}
		else {
			cGenPayload[4+2*i] = 'A' + cGenPayload[4+2*i] - 10;
		}
	}
	PayloadSize = 8;

	//下16-44時
	//收progress跟checksum的情況  這是因為檔案太大的時候
	//DT算checksum要比較久的時間 所以read result的時候會timeout
	//但要是timeout 設太長會影響到整體的速度
	//(也就是說一個cmd fail都會等到10s)
	//所以就收progress解決
	/////////////////////////
	//接下來 直接送 一直收 然後結束 不要用QthGenCommand
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = 0;
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	Sleep(2);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		file2.free_file_mem(file_hdr2);
		return ReceiveResult;
	}

	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		file2.free_file_mem(file_hdr2);
		return HS_INVALID_CHECKSUM;
	}

	//// 2008-02-25 Nek Wang: clear progress
	while (1) {
		Sleep(20);
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
		nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			break;
		}
		else if (nRetCode != HS_PROGRESS) {
			file2.free_file_mem(file_hdr2);
			return HS_FAIL;
		}
	}

	unsigned char temp_pPackData[256];
	memset(temp_pPackData, 0, sizeof(temp_pPackData));
	memcpy(temp_pPackData, &(pPackData[1]), 254);
	if (CString(temp_pPackData).Compare(CString(cChecksum)) != 0) {
		TRACE(_T("%s checksum fail"), __FUNCTION__);
		file2.free_file_mem(file_hdr2);
		return HS_INVALID_CHECKSUM;
	}

	//下08-xx時
	//收progress跟result的情況  這是因為檔案太大的時候
	//DT要write file to flash比較久的時間
	//所以read result的時候會timeout
	//但要是timeout 設太長會影響到整體的速度
	//(也就是說一個cmd fail都會等到10s)
	//所以就收progress解決
	/////////////////////////
	//接下來 直接送 一直收 然後結束 不要用QthGenCommand

	memset(pPackData, 0, sizeof(pPackData));
	memset(cGenPayload, 0, sizeof(cGenPayload));
	memset(pPackData, 0, sizeof(pPackData));
	memset(cGenPayload, 0, sizeof(cGenPayload));
	cGenPayload[0] = CmdNumber[0];
	cGenPayload[1] = CmdNumber[1];
	cGenPayload[2] = CmdNumber[2];
	cGenPayload[3] = CmdNumber[3];
	PayloadSize = (int)strlen(parameter);
	strncpy((char*)cGenPayload + 4, parameter, strlen(parameter));
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);

	Sleep(2);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		file2.free_file_mem(file_hdr2);
		return ReceiveResult;
	}
	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		file2.free_file_mem(file_hdr2);
		return HS_INVALID_CHECKSUM;
	}

	//// 20008-02-25 Nek Wang: receive progress
	while (1) {
		Sleep(20);
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
		int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			if (m_farCallBackChar != NULL) {
				m_farCallBackChar(nPort, "100 ");
			}
			file2.free_file_mem(file_hdr2);
			return HS_OK;
		}
		else if (nRetCode == HS_PROGRESS) {
			char c2Progress[5];
			if (m_farCallBackChar != NULL) {
				memset(c2Progress, 0, sizeof(c2Progress));
				int nPrg = 50 + (int)pPackData[0] / 2;
				wsprintf(c2Progress, "%d ", nPrg);
				m_farCallBackChar(nPort, c2Progress);
			}
		}
		else {
			file2.free_file_mem(file_hdr2);
			return HS_FAIL;
		}
	}
	file2.free_file_mem(file_hdr2);
	return HS_FAIL;
}

int CmdCtrl::BDTLSectorUpload(int nPort, int nTimeout, char* CmdNumber, char* FilePath, char* parameter)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}

	nRetCode = ExactBDTLSectorUploadCmd(nPort, nTimeout, CmdNumber, FilePath, parameter);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s ExactBDTLSectorUploadCmd(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_USBGENUPLOADCMD;
	}

	if (!m_isOpen) {
		m_isGeneralCmd = false;
		close(nPort, nTimeout);
	}

	return HS_OK;
}

int CmdCtrl::ExactBDTLSectorUploadCmd(int nPort, int nTimeout, char* CmdNumber, char* FilePath, char* parameter)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}
	Sleep(11);
	int SendResult = 0;
	int ReceiveResult = 0;
	unsigned char NewChecksum;
	unsigned char OldChecksum;//用來取得算出來的Checksum
	unsigned char cGenPayload[4096];
	int iBlockNum = 0;
	//存傳回結果
	unsigned char pPackData[9000];
	int PayloadSize = 0;//size =
	memset(pPackData, 0, sizeof(pPackData));
	memset(cGenPayload, 0, sizeof(cGenPayload));
	//-------------Initial ULBuffer Command Start

	//init buffer
	//block size是gobal varible
	//default 是8192
	//如果不是8192就要先下16-33去改
	//現在這階段很尷尬 應該都要用8192
	//但是目前的F1.002這個版本的block size是2048
	//////////////////////////
	//以後下面的code 的block size要改成動態的
	//default是2048
	//for General Download
	//16-33 Init Buffer Size 512
	cGenPayload[0] = 16;
	cGenPayload[1] = 33;
	cGenPayload[2] = 0;
	cGenPayload[3] = 0;
	//total initial buffer  1024

	cGenPayload[4] = '0';
	cGenPayload[5] = '0';
	cGenPayload[6] = '0';
	cGenPayload[7] = '0';
	cGenPayload[8] = '0';
	cGenPayload[9] = '0';
	cGenPayload[10] = '0';
	cGenPayload[11] = '0';
	//注意init buffer應該要改成P21GeneralCmd才對
	//寫死的,注意要改掉
	cGenPayload[12] = '0';
	cGenPayload[13] = '0';
	cGenPayload[14] = '0';
	cGenPayload[15] = '0';
	////////////////////////////////
	cGenPayload[16] = '0';
	cGenPayload[17] = 8 + '0';
	//cGenPayload[16] =2+'0';
	//cGenPayload[17] ='0';
	////////////////////////////////
	cGenPayload[18] = '0';
	cGenPayload[19] = '0';
	//對於DL 來說 PayloadSize一定要16byte
	//這是protocol 不能改
	PayloadSize = 16;

	//cGenPayload的前四個bit command,後面的才是data
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	if (SendResult < HS_OK) {
		TRACE(_T("%s QthGenPacketSend(%d) fail\r\n"), __FUNCTION__, SendResult);
		return SendResult;
	}
	Sleep(2);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		return ReceiveResult;
	}

	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		return HS_INVALID_CHECKSUM;
	}
	Sleep(20);
	ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);

	int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
	if (nRetCode != HS_OK) {
		return nRetCode;
	}
	//-------------Initial Buffer Command End

	cGenPayload[0] = CmdNumber[0];
	cGenPayload[1] = CmdNumber[1];
	cGenPayload[2] = CmdNumber[2];
	cGenPayload[3] = CmdNumber[3];
	PayloadSize = 4;
	PayloadSize = (int)strlen(parameter);
	strncpy((char*)cGenPayload + 4, parameter, strlen(parameter));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	Sleep(2);
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		TRACE(_T("%s QthGenPacketRecv(%d) fail\r\n"), __FUNCTION__, ReceiveResult);
		return ReceiveResult;
	}
	if (OldChecksum != NewChecksum) {
		TRACE(_T("%s:%s checksum fail(%x != %x)\r\n"), __FUNCTION__, __LINE__, OldChecksum, NewChecksum);
		return HS_INVALID_CHECKSUM;
	}
	//// 2008-02-25 Nek Wang: clear progress
	while (1) {
		Sleep(20);
		memset(pPackData, 0, sizeof(pPackData));
		ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
		int nRetCode = PacketResultCheck(ReceiveResult, pPackData);
		if (nRetCode == HS_OK) {
			int ij = -10;
			int sum = 0;
			for (int i = 17; i > 9; i--) {
				AsciiToBinary(parameter[i], &ij);
				sum = sum + ij * intergePower(16, (17 - i));
			}
			sum *= 512;
			if (sum % 2048 != 0) {
				iBlockNum = (sum / 2048) + 1;
			}
			else {
				iBlockNum = (sum / 2048);
			}
			break;
		}
		else if (nRetCode != HS_PROGRESS) {
			return HS_FAIL;
		}
	}
	//-------------Send Command End

	//open binary file to write
	//FF:if camera get frame 則利用外面傳進來的檔名和路徑去存檔
	ofstream ofile;
	//FF:camera function的buffer
	unsigned char *bPicDataBuffer = NULL;
	//FF:加一個buffer buffer滿了在進行寫入的動作 or 收失敗了就進行寫入的動作
	unsigned char *cDataBuffer = NULL;
	ofile.open(FilePath, ios::binary | ios::app);
	cDataBuffer = new unsigned char[1024*1024*2];
	memset(cDataBuffer, 0, sizeof(cDataBuffer));

	//receive data
	memset(cGenPayload, 0, sizeof(cGenPayload));
	//////////////recieveing data
	int index = 0;
	int retransfer = 0;
	while (index < iBlockNum) {
		memset(cGenPayload, 0, sizeof(cGenPayload));
		memset(pPackData, 0, sizeof(pPackData));
		cGenPayload[0] = (unsigned char)(index >> 24);
		cGenPayload[1] = (unsigned char)(index >> 16);
		cGenPayload[2] = (unsigned char)(index >> 8);
		cGenPayload[3] = (unsigned char)index;
		SendResult = QthGenPacketSend(P21_DATAUL_REQUEST, 0, cGenPayload, &OldChecksum, nPort);
		//先收掉重複的封包 如果有的話

		if (retransfer > 0) {
			for (int i = 0; i < retransfer; i++) {
				ReceiveResult = QthGenPacketRecv(P21_DATAUL_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
				if (ReceiveResult == 1) {
					retransfer--;
				}
				else {
					retransfer++;
					if (retransfer < 5) {
						continue;
					}
					//FF:delete buffer
					if (NULL != cDataBuffer) {
						delete[] cDataBuffer;
					}
					if (NULL != bPicDataBuffer) {
						delete[] bPicDataBuffer;
					}
					ofile.close();
					return ReceiveResult;
				}
			}
		}
		//收要存檔的封包
		ReceiveResult = QthGenPacketRecv(P21_DATAUL_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
		if (ReceiveResult == HS_OK) {
			//write file
			if ((index != 0) && ((index % 1024) == 0)) {
				//FF:只要不是camera function 如果buffer滿了 寫入
				if (!(CmdNumber[0] == P21_DT_CAMERA)) {
					ofile.write(reinterpret_cast<char*>(cDataBuffer), 1024*1024*2);
				}
			}
			//FF:一定要把data放入buffer
			if (CmdNumber[0] == P21_DT_CAMERA) {
				memcpy(bPicDataBuffer + ((index % 1024)*2048), pPackData, 2048);
			}
			else {
				memcpy(cDataBuffer + ((index % 1024)*2048), pPackData, 2048);
			}
		}
		else {
			retransfer++;
			if (retransfer < 5) {
				continue;
			}
			//FF:delete buffer
			if (NULL != cDataBuffer) {
				delete[] cDataBuffer;
			}
			if (NULL != bPicDataBuffer) {
				delete[] bPicDataBuffer;
			}
			ofile.close();
			return ReceiveResult;
		}
		index++;
		retransfer = 0;
	}//for loop end
	//FF:index不一定都剛好整除 所以外面要寫剩餘的檔案
	if ((index % 1024) != 0) {
		ofile.write(reinterpret_cast<char*>(cDataBuffer), (index % 1024)*2048);
	}

	//FF:若index剛好整除1024 裡面不會寫
	//FF:所以外面要寫最後一個cDataBuffer的size大小
	if ((index % 1024) == 0 && index != 0) {
		ofile.write(reinterpret_cast<char*>(cDataBuffer), 1024*1024*2);
	}

	//FF:delete buffer
	if (NULL != cDataBuffer) {
		delete[] cDataBuffer;
	}
	if (NULL != bPicDataBuffer) {
		delete[] bPicDataBuffer;
	}
	ofile.close();

	return HS_OK;
}

int CmdCtrl::SetUIInfo(int nPort, char* UIInfo)
{
	strncpy(m_UIInfo, UIInfo, 6);
	return HS_OK;
}

//0x30 0x30 ->0x00  1b to 2b
int CmdCtrl::AsciiToHex(char* input, int Size)
{
	unsigned char cTemp[256];
	unsigned char a;
	unsigned char b;
	memset(cTemp, 0, sizeof(cTemp));
	for (int i = 0; i < Size / 2; i++) {
		if ((input[2*i] >= '0') && (input[2*i] <= '9')) {
			a = input[2*i] - '0';
		}
		else if ((input[2*i] >= 'A') && (input[2*i] <= 'F')) {
			a = input[2*i] - 'A' + 10;
		}
		else {
			return 0;
		}

		if ((input[2*i+1] >= '0') && (input[2*i+1] <= '9')) {
			b = input[2*i+1] - '0';
		}
		else if ((input[2*i+1] >= 'A') && (input[2*i+1] <= 'F')) {
			b = input[2*i+1] -  'A' + 10;
		}
		else {
			return 0;
		}
		cTemp[i] = (a<<4) + b;
	}
	memset(input, 0, sizeof(char)*Size);
	strncpy(input, (char*)cTemp, Size / 2);

	return 1;
}

// 0x00 -> 0x30 0x30 (2b to 1b)
int CmdCtrl::HexToAscii(char* Input, int Size)
{
	unsigned char cWorkBuffer[256];
	memset(cWorkBuffer, 0, sizeof(cWorkBuffer));
	for (int i = 0; i < Size; i++) {
		cWorkBuffer[2*i] = Input [i];
	}

	//transfer 4byte hex to 8 byte ascii
	for (int x = 0;x < Size;x++) {
		cWorkBuffer[1+2*x] = cWorkBuffer[2*x] % 16;
		if (cWorkBuffer[1+2*x] < 10) {
			cWorkBuffer[1+2*x] = '0' + cWorkBuffer[1+2*x];
		}
		else {
			cWorkBuffer[1+2*x] = 'A' + cWorkBuffer[1+2*x] - 10;
		}
		cWorkBuffer[2*x] = cWorkBuffer[2*x] / 16;
		if (cWorkBuffer[2*x] < 10) {
			cWorkBuffer[2*x] = '0' + cWorkBuffer[+2*x];
		}
		else {
			cWorkBuffer[2*x] = 'A' + cWorkBuffer[2*x] - 10;
		}
	}
	strncpy(Input, (char*)cWorkBuffer, strlen((char*)cWorkBuffer));

	return 1;
}

/**
 * 2008-02-18 Mick: get version string from file.
 *
 * @param filepath path to file.
 * @param buffer   buffer to load file.
 * @param length   the max length of str.
 * @retval -1      INVALID_HANDLE_VALUE
 * @retval 0       read nothing.
 * @retval 1       read OK.
 */
int CmdCtrl::GetFile2Buffer(const char* filepath, char* buffer, int length)
{
	HANDLE hFile =
	    CreateFile(filepath,
	               GENERIC_READ | GENERIC_WRITE, // open for writing
	               0,                            // do not share
	               NULL,                         // no security
	               OPEN_EXISTING,                //
	               FILE_ATTRIBUTE_NORMAL,        // normal file
	               NULL);                        // no attr. template

	int return_code = -1;
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD nMaxRead = GetFileSize(hFile, NULL);
		if (nMaxRead > static_cast<DWORD>(length)) {
			nMaxRead = length;
		}
		DWORD nbytesRead = 0;
		BOOL isOk = ::ReadFile(hFile, buffer, nMaxRead, &nbytesRead, NULL);
		return_code = (isOk && nbytesRead) ? 1 : 0;
		CloseHandle(hFile);
	}
	return return_code;
}

int CmdCtrl::GetExternalOSVer(char* InputFilePath, char* cOSVer, int iOSVerlen)
{
	CString cstrWorkPath = CString(InputFilePath);
	int nFileNameLength = cstrWorkPath.ReverseFind('\\') + 1;
	cstrWorkPath = cstrWorkPath.Left(nFileNameLength);

	CString cstrCheckFile = cstrWorkPath + CString("P213RDDownloadToolOSVersion");
	int nRetCode = this->GetFile2Buffer((LPCTSTR)cstrCheckFile, cOSVer, iOSVerlen);
	if (nRetCode != -1)
		return nRetCode;

	cstrCheckFile = cstrWorkPath + CString("OSImageVersion.txt");
	nRetCode = this->GetFile2Buffer((LPCTSTR)cstrCheckFile, cOSVer, iOSVerlen);
	if (nRetCode != -1)
		return nRetCode;

	nRetCode = this->GetFile2Buffer("OSImageVersion.txt", cOSVer, iOSVerlen);
	if (nRetCode != -1)
		return nRetCode;

	return 0;
}

//// flag = 0 is temp, flag = 1 is finish
CString CmdCtrl::GetExternalVersion(char* WorkBuffer, CString cstrTempFileName, int flag)
{
	char cExtOSVer[128];
	memset(cExtOSVer, 0, sizeof(cExtOSVer));
	if (flag == 0) {
		cstrTempFileName = "FAIL!!";
	}
	else {
		if (GetExternalOSVer(WorkBuffer, cExtOSVer, 128) == 1) {
			cExtOSVer[127] = '\0';
			cstrTempFileName = CString(cExtOSVer);
		}
	}

	cstrTempFileName.Replace("\r\n", "");
	//// Month and Day
	struct tm *T;
	time_t t;
	time(&t);
	T = localtime(&t);

	char cMonthDay[256];
	memset(cMonthDay, '0', sizeof(cMonthDay));
	sprintf(cMonthDay, "%2d%2d", T->tm_mon + 1, T->tm_mday);

	size_t i = 0; // avoiding for loop scope bug in VC6.
	for (i = 0; i < strlen(cMonthDay); i++) {
		if (cMonthDay[i] == ' ')
			cMonthDay[i] = '0';
	}
	//// Month and Day

	// 301 mode length is 20
	for (i = strlen(cMonthDay) + cstrTempFileName.GetLength() + 1; i <= 20; i++) {
		cstrTempFileName = cstrTempFileName + " ";
	}
	cstrTempFileName = cstrTempFileName + cMonthDay;

	return cstrTempFileName;
}

int CmdCtrl::DownloadInfoFileSize(char* FileName, char *szFileSize)
{
	TRACER_FUNCTION_IN();

	//// 2008-03-03 Nek Wang: Get File Size
	HANDLE hFile;
	DWORD FileSize;
	hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile);
		return HS_FILE_NOT_FOUND;
	}
	FileSize = ::GetFileSize(hFile, NULL);
	if (FileSize == 0xFFFFFFFF) {
		::CloseHandle(hFile);
		return HS_FILE_NOT_FOUND;
	}
	::CloseHandle(hFile);
	//// Get File Size

	unsigned char cGenPayload[12];
	memset(cGenPayload, 0, sizeof(cGenPayload));
	cGenPayload[4]  = (unsigned char)((0xFF000000 & FileSize) >> 24);
	cGenPayload[6]  = (unsigned char)((0x00FF0000 & FileSize) >> 16);
	cGenPayload[8]  = (unsigned char)((0x0000FF00 & FileSize) >> 8);
	cGenPayload[10] = (unsigned char)(0x000000FF & FileSize);
	//transfer 4byte hex to 8 byte ascii
	for (int i = 0; i < 4; i++) {
		cGenPayload[5 + 2*i] = cGenPayload[ 4 + 2*i] % 16;
		if (cGenPayload[5 + 2*i] < 10) {
			cGenPayload[5 + 2*i] = '0' + cGenPayload[5 + 2*i];
		}
		else {
			cGenPayload[5 + 2*i] = 'A' + cGenPayload[5 + 2*i] - 10;
		}
		cGenPayload[4 + 2*i] = cGenPayload[4 + 2*i] / 16;
		if (cGenPayload[4 + 2*i] < 10) {
			cGenPayload[4 + 2*i] = '0' + cGenPayload[4 + 2*i];
		}
		else {
			cGenPayload[4 + 2*i] = 'A' + cGenPayload[4 + 2*i] - 10;
		}
	}
	memset(szFileSize, 0, sizeof(szFileSize));
	strncpy(szFileSize, (char*)(cGenPayload + 4), 8);

	return HS_OK;
}

int CmdCtrl::WriteDownloadInfo(int nPort, int nTimeout, char *DLMethod, char* DLType, char* DLFilename, char *DLFileSize, int DLFlag)
{
	TRACER_FUNCTION_IN();

	char szDLInfoBuf[256];
	memset(szDLInfoBuf, 0, sizeof(szDLInfoBuf));
	char szDLType[3];
	memset(szDLType, 0, sizeof(szDLType));

	if (DLType[0] == P21_DT_FLASH) {
		switch (DLType[1]) {
		case P21_MENUITEM_IPL:
			strncpy(szDLType, P21_TYPE_IPL_FILE, 2);
			break;
		case P21_MENUITEM_DT:
			strncpy(szDLType, P21_TYPE_DT_FILE, 2);
			break;
		case P21_MENUITEM_MSIPL:
			strncpy(szDLType, P21_TYPE_MSIPL_FILE, 2);
			break;
		case P21_MENUITEM_EBOOT:
			strncpy(szDLType, P21_TYPE_EBOOT_FILE, 2);
			break;
		case P21_MENUITEM_BDTL0:
			strncpy(szDLType, P21_TYPE_BDTL0_FILE, 2);
			break;
		case P21_MENUITEM_BDTL1:
			strncpy(szDLType, P21_TYPE_BDTL1_FILE, 2);
			break;
		default:
			return HS_OK; //skip other DL
		}
	}
	if (DLType[0] == P21_DT_FACTORYAREA) {
		switch (DLType[1]) {
		case P21_MENUITEM_MEPD:
			strncpy(szDLType, P21_TYPE_MEPD_FILE, 2);
			break;
		default:
			return HS_OK;		//skip other DL
		}
	}
	memset(szDLInfoBuf, '0', sizeof(szDLInfoBuf));
	//// DL Method "03" = USB, "04" = SD CARD
	strncpy(szDLInfoBuf, DLMethod, 2);

	//// DL Type
	strncpy(szDLInfoBuf + 2, szDLType, 2);

	//// DL Version
	CString filename(DLFilename);
	filename = filename.Right(filename.GetLength() - filename.ReverseFind('\\') - 1);

	char szVersion[256];
	memset(szVersion, 0, sizeof(szVersion));
	if (DLType[0] == P21_DT_FACTORYAREA && DLType[1] == P21_MENUITEM_MEPD) {
		if (DLFlag == 1) {
			filename = DLFilename; //// Download MEPD
		}
		else {
			filename = GetExternalVersion(DLFilename, filename, DLFlag);
		}
	}
	else if (strcmp(DLMethod, P21_DL_METHOD_USB) != 0) { //// USB Download
		filename= GetExternalVersion(DLFilename, filename, DLFlag);
	}
	else if (strcmp(DLMethod, P21_DL_METHOD_SDCARD) != 0) {//// SD Download
		if (DLFlag == 1) {
			int nRetCode = P21SDCardReadOSVersion(nPort, nTimeout, szVersion);
			if (nRetCode != 1) {
				return nRetCode;
			}
			filename = szVersion;
		}
		else {
			filename = GetExternalVersion(DLFilename, filename, DLFlag);
		}
	}

	memset(szVersion, 0, sizeof(szVersion));
	strncpy(szVersion, (LPCTSTR)filename, filename.GetLength());
	HexToAscii(szVersion, filename.GetLength());
	strncpy(szDLInfoBuf + 4, szVersion, strlen(szVersion) > 60 ? 60 : strlen(szVersion));

	//// DL User Info
	char szUserInfo[256];
	memset(szUserInfo, 0, sizeof(szUserInfo));
	strncpy(szUserInfo, m_UIInfo, 6);
	HexToAscii(szUserInfo, 6);
	strncpy(szDLInfoBuf + 64, szUserInfo, strlen(szUserInfo) > 12 ? 12 : strlen(szUserInfo));

	//// DL File Size
	strncpy(szDLInfoBuf + 76, DLFileSize, strlen(szUserInfo) > 8 ? 8 : strlen(DLFileSize));
	szDLInfoBuf[128] = 0;
	int nRetCode = P21FAWriteVersionInfo(nPort, nTimeout, szDLInfoBuf);

	TRACER_STREAM("[Port " << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

int CmdCtrl::DDRDump(int nPort, int nTimeout)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		m_isGeneralCmd = true;
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_OPENDEVICE;
		}
	}

	unsigned char cGenPayload[4096];
	unsigned char pPackData[9000];
	unsigned char NewChecksum;
	unsigned char OldChecksum;
	int SendResult = 0;
	int ReceiveResult = 0;
	cGenPayload[0] = 16;
	cGenPayload[1] = 33;
	cGenPayload[2] = 0;
	cGenPayload[3] = 0;
	cGenPayload[4] = '0';
	cGenPayload[5] = '0';
	cGenPayload[6] = '0';
	cGenPayload[7] = '0';
	cGenPayload[8] = '0';
	cGenPayload[9] = '0';
	cGenPayload[10] = '0';
	cGenPayload[11] = '0';
	cGenPayload[12] = '0';
	cGenPayload[13] = '0';
	cGenPayload[14] = '0';
	cGenPayload[15] = '0';
	cGenPayload[16] = '0';
	cGenPayload[17] = 8 + '0';
	cGenPayload[18] = '0';
	cGenPayload[19] = '0';
	int PayloadSize = 16;
	memset(pPackData, 0, sizeof(pPackData));
	SendResult = QthGenPacketSend(DT_PKT_REQUEST, PayloadSize, cGenPayload, &OldChecksum, nPort);
	if (SendResult != HS_OK) {
		TRACE(_T("%s QthGenPacketSend(%d) fail\r\n"), __FUNCTION__, SendResult);
		return SendResult;
	}
	ReceiveResult = QthGenPacketRecv(DT_PKT_ACK, cGenPayload, pPackData, &NewChecksum);
	if (ReceiveResult != HS_OK) {
		return ReceiveResult;
	}
	if (OldChecksum != NewChecksum) {
		return HS_INVALID_CHECKSUM;
	}
	Sleep(20);
	ReceiveResult = QthGenPacketRecv(DT_PKT_RESULT, cGenPayload, pPackData, &NewChecksum);
	nRetCode = PacketResultCheck(ReceiveResult, pPackData);
	if (nRetCode !=  HS_OK) {
		return nRetCode;
	}

	ofstream fouts[5];
	fouts[0].open("QLOG.txt", ios::binary);
	fouts[1].open("KLOG.txt", ios::binary);
	fouts[2].open("ELOG.txt", ios::binary);
	fouts[3].open("MODEMDATA.txt", ios::binary);
	fouts[4].open("RESERVED.txt", ios::binary);

	int LogStartIndex[5];
	LogStartIndex[0] = 0x728000 / 2048;
	LogStartIndex[1] = 0x7A8000 / 2048;
	LogStartIndex[2] = 0x7C4000 / 2048;
	LogStartIndex[3] = 0x7C9000 / 2048;
	LogStartIndex[4] = 0x7CF000 / 2048;
	int LogEndIndex[5];
	LogEndIndex[0] = LogStartIndex[0] + 0x80000 / 2048;
	LogEndIndex[1] = LogStartIndex[1] + 0x1c000 / 2048;
	LogEndIndex[2] = LogStartIndex[2] + 0x4000 / 2048;
	LogEndIndex[3] = LogStartIndex[3] + 0x6000 / 2048;
	LogEndIndex[4] = LogStartIndex[4] + 0x80000 / 2048;

	//receive data
	int index = 0;
	while (index < LogEndIndex[4]) {
		memset(cGenPayload, 0, sizeof(cGenPayload));
		memset(pPackData, 0, sizeof(pPackData));
		cGenPayload[0] = (unsigned char)(index >> 24);
		cGenPayload[1] = (unsigned char)(index >> 16);
		cGenPayload[2] = (unsigned char)(index >> 8);
		cGenPayload[3] = (unsigned char)index;
		SendResult = QthGenPacketSend(P21_DATAUL_REQUEST, 0, cGenPayload, &OldChecksum, nPort);
		if (SendResult < HS_OK) {
			return SendResult;
		}
		ReceiveResult = QthGenPacketRecv(P21_DATAUL_ACK, cGenPayload, (unsigned char *)pPackData, &NewChecksum);
		if (ReceiveResult == HS_OK) {
			for (int i = 0; i < 5; i++) {
				if (LogStartIndex[i] <= index && index < LogEndIndex[i]) {
					fouts[i].write(reinterpret_cast<char*>(pPackData), 2048);
				}
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				fouts[i].close();
			}
			return ReceiveResult;
		}
		index++;
	}
	for (int i = 0; i < 5; i++) {
		fouts[i].close();
	}

	if (nRetCode != HS_OK) {
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_USBGENUPLOADCMD; // FIXME
	}

	if (!m_isOpen) {
		m_isGeneralCmd = false;
		close(nPort, nTimeout);
	}

	return HS_OK;
}

int CmdCtrl::PacketResultCheck(int iPacketResult, unsigned char * pPackData)
{
	if (iPacketResult == HS_PROGRESS) {
		return HS_PROGRESS;
	}
	else if (iPacketResult != HS_OK) {
		TRACER_STREAM(" => Receive packet fail: " << (char*)pPackData << "\r\n");
		return iPacketResult;
	}

	if (pPackData[0] == '1') {
		return HS_OK;
	}
	else if (pPackData[0] == '0') {
		if(pPackData[1] == 0x02)
		{
			TRACER_STREAM(" => Receive packet fail(0), errorcode: " << *(pPackData+1) << "\r\n");
			return HS_EMPTY;	
		}
		else
		{
			TRACER_STREAM(" => Receive packet fail(0), errorcode: " << *(pPackData+1) << "\r\n");
			return HS_FAIL;
		}
	}
	else {
		TRACER_STREAM(" => Receive packet fail(other), errorcode:" << *(pPackData+1) << "\r\n");
		return HS_FAIL;
	}
}

int CmdCtrl::FlashDumpBDTL( int nPort, int nTimeout, char* InBuffer, int BDTL)
{
	::DeleteFile(InBuffer);

	int BDTL_BlockLen = 0;
	char szServicePassword[MAX_PATH];
	memset(szServicePassword, 0, sizeof(szServicePassword));

	int nRetCode = HS_FAIL;
	nRetCode = P21FlashGetServicePassword(nPort, nTimeout, szServicePassword);
	if (nRetCode == HS_OK) {
		nRetCode = P21DLCheckServicePasswd(nPort, nTimeout, szServicePassword);
	}
	if (nRetCode == HS_OK) {
		char szMDOCInfo[MAX_PATH];
		memset(szMDOCInfo, 0, sizeof(szMDOCInfo));
		nRetCode = P21FlashMDOCInfo(nPort, nTimeout, szMDOCInfo);
		if (nRetCode == HS_OK) {
			char* cBDTL;
			if (BDTL == 0) {
				cBDTL = strstr(szMDOCInfo, "BDT0");
			}
			else if (BDTL == 1) {
				cBDTL = strstr(szMDOCInfo, "BDT1");
			}
			else {
				cBDTL = strstr(szMDOCInfo, "BDT2");
			}

			int ij = 0, sum = 0;
			for (int i = 11; i > 3; i--) {
				AsciiToBinary(cBDTL[i], &ij);
				sum = sum + ij * intergePower(16, (11 - i));
			}
			BDTL_BlockLen = sum / 512;
		}
	}
	if (nRetCode == HS_OK) {
		int BlockIndex = 0;
		int BlockDataLen = 0;
		const int nSize = 0x14000;	//// 40MB
		do { //////////////////////////////////////////////////////////////////////
			BlockDataLen = (BDTL_BlockLen > nSize) ? nSize : BDTL_BlockLen;
			if (nRetCode == HS_OK) {
				char szFileParameter[19];
				memset(szFileParameter, 0, sizeof(szFileParameter));
				char szBlockIndex[9];
				memset(szBlockIndex, 0, sizeof(szBlockIndex));
				char szBlockDataLen[9];
				memset(szBlockDataLen, 0, sizeof(szBlockDataLen));

				IntTo8ASCII(BlockIndex, (unsigned char *)szBlockIndex);
				IntTo8ASCII(BlockDataLen, (unsigned char *)szBlockDataLen);
				if (BDTL == 0) {
					strncpy(szFileParameter, "00", 2);
				}
				else if (BDTL == 1) {
					strncpy(szFileParameter, "10", 2);
				}
				else {
					strncpy(szFileParameter, "20", 2);
				}
				strncpy(szFileParameter + 2, szBlockIndex  , 8);
				strncpy(szFileParameter + 10, szBlockDataLen, 8);
				nRetCode = P21FlashDumpBDTLSectors(nPort, nTimeout, InBuffer, szFileParameter);
			}
			BlockIndex = BlockIndex + nSize;
			BDTL_BlockLen = BDTL_BlockLen - nSize;
		} //////////////////////////////////////////////////////////////////////
		while (BDTL_BlockLen > 0);
	}
	return nRetCode;
}

int CmdCtrl::MTKGenCommand(int nBodySize, unsigned char* ReqBody,int nPeerBufferSize,unsigned char* PeerBuffer,unsigned char* OutConfirm, unsigned char * OutPeerData)
{
	TRACER_FUNCTION_IN();
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	int SendResult = 0;
	int ReceiveResult = 0;

	int nRetCode = HS_FAIL;
	
	/*unsigned char cGenPayload[PAYLOAD_SIZE];
	memcpy(cGenPayload, command, 4);
	memcpy(cGenPayload+4, payload, nSize);*/

	nRetCode = MTKGenPacketSend(MTK_REQ_SEND_AP, nBodySize, ReqBody, nPeerBufferSize,PeerBuffer);
	if (nRetCode != HS_OK) {
		TRACER_STREAM("MTKGenPacketSend:" << ReceiveResult << "\r\n");
		TRACE(_T("%s MTKGenPacketSend fail(%d)\n"), __FUNCTION__, SendResult);
		return nRetCode;
	}

//	Sleep(12);

	unsigned char pConfirm[PAYLOAD_SIZE];
	unsigned char pPeerData[PAYLOAD_SIZE];
	memset(pConfirm, 0, sizeof(pConfirm));
	memset(pPeerData, 0, sizeof(pPeerData));

	// TODO: check receive buffer
//	Sleep(50);
	nRetCode = MTKGenPacketRecv(ReqBody, pConfirm, pPeerData,500);
	if (nRetCode != HS_OK) 
	{
		TRACER_STREAM("QthGenPacketRecv result:" << ReceiveResult << "\r\n");
		TRACE(_T("%s QthGenPacketRecv result fail(%d)\n"), __FUNCTION__, ReceiveResult);
	}
	else
	{
		memcpy(OutConfirm, pConfirm, PAYLOAD_SIZE*sizeof(unsigned char));
		memcpy(OutPeerData, pPeerData, PAYLOAD_SIZE*sizeof(unsigned char));
	}
	
	return nRetCode;
}

int CmdCtrl::MTKGenPacketSend(int Type,int nBodySize, BYTE* ReqBody,int nPeerBufferSize,BYTE* PeerBuffer)
{
	//TRACER_FUNCTION_IN();
	int TotalDataSize;
	unsigned char PacketHeader[4];
	PacketHeader[0] = 0x55;
	PacketHeader[1] = (unsigned char)((nBodySize+nPeerBufferSize+4)>>8);
	PacketHeader[2] = (unsigned char)(nBodySize+nPeerBufferSize+4);

	if (Type == MTK_REQ_SEND_AP)
	{
		PacketHeader[3] = 0x66; 
	}
	else if (Type == MTK_REQ_SEND_MODEM_1_2)
	{
		PacketHeader[3] = 0x64; 
	}
	else if (Type == MTK_REQ_SEND_MODEM_2)
	{
		PacketHeader[3] = 0xA0; 
	}
	else
	{
        return HS_FAIL;
	}

	unsigned char temp[9000];
	memset(&temp, 0, sizeof(temp));
	unsigned char Checksum;
	
	memcpy(temp, PacketHeader, DT_PKT_HEADER_SIZE);
	temp[4] = (unsigned char)(nBodySize);
	temp[5] = (unsigned char)(nBodySize>>8);
	
	temp[6] = (unsigned char)(nPeerBufferSize);
	temp[7] = (unsigned char)(nPeerBufferSize>>8);

	memcpy(temp + 8, ReqBody, nBodySize); // command or block number
	memcpy(temp + 8 + nBodySize, PeerBuffer, nPeerBufferSize);  // payload size
	TotalDataSize = DT_PKT_HEADER_SIZE + 4 + nBodySize + nPeerBufferSize;

	//Calculate Checksum
	Checksum = temp[0];
	for (int j = 1; j < TotalDataSize; j++) {
		Checksum ^= temp[j];
	}
	temp[TotalDataSize] = Checksum;
	//*OChecksum = Checksum;
	TotalDataSize++; //add chcksum

	int nDataSize = TotalDataSize;
	int nPktSend = 0;
	int nByteWrite = DT_PKT_BLOCK_SIZE;
	while (nDataSize > 0) {
		TRACE("%s PartialDataSize: %d\n", __FUNCTION__, nDataSize);
		nByteWrite = (nDataSize < nByteWrite) ? nDataSize : DT_PKT_BLOCK_SIZE;
		if (m_pCommDrv->write(&temp[nPktSend*DT_PKT_BLOCK_SIZE], nByteWrite) != nByteWrite) {
			TRACER_STREAM("!" << __FUNCTION__ << ",write:" << nByteWrite << "\r\n");
			TRACE(_T("%s fail, code:%d\r\n"), __FUNCTION__, nByteWrite);
			return HS_WRITE_DATA_FAIL;
		}
		nPktSend++;
		nDataSize -= nByteWrite;
	//	Sleep(300);
	}
	return HS_OK;
}

int CmdCtrl::MTKGenPacketRecv(BYTE* PayloadCommand, BYTE* pConfirmData, BYTE* pPeerData, int nTimeout)
{
	//TRACER_FUNCTION_IN();
	
	int nPayloadSize = 0;
	unsigned char data[4096] = {0};
	unsigned char temp[4096] = {0};

	int nTotalSize = 0;
	int nLocalSize = 0;
	int nPeerSize = 0;

	TRACE("check frame header ...\n");
		
	//start flag 0x55
	if (m_pCommDrv->read(data, 1, nTimeout) != 1) {
		TRACER_STREAM("QthGenPacketRecv while start flag, timeout:" << nTimeout << "\r\n");
		return HS_READ_DATA_FAIL;
	}
	if (data[0] != 0x55) {
		TRACE("check packet header FAIL\n");
		return HS_FAIL;
	}
	temp[0] = data[0];

	//total length exclude header(4bytes) and checksum(1byte)
	if (m_pCommDrv->read(data, 2, nTimeout) != 2) {
		TRACER_STREAM("QthGenPacketRecv while get packet length \r\n");
		return HS_READ_DATA_FAIL;
	}
	nTotalSize = (int)(data[0]<<8) +(int)(data[1]);
	memcpy(temp + 1, data, 2);

	//confirm type 0x60
	if (m_pCommDrv->read(data, 1, nTimeout) != 1) {
		TRACER_STREAM("QthGenPacketRecv while confirm type\r\n");
		return HS_READ_DATA_FAIL;
	}
	if (data[0] != 0x60) { 
		TRACE("check packet header FAIL\n");
		return HS_FAIL;
	}
	temp[3] = data[0];

	//local length, confirm length = token(2bytes) + confID(2bytes) +confirm body
	if (m_pCommDrv->read(data, 2, nTimeout) != 2) {
		TRACER_STREAM("QthGenPacketRecv local length \r\n");
		return HS_READ_DATA_FAIL;
	}
	nLocalSize = (int)(data[1]<<8) +(int)(data[0]);
	memcpy(temp + 4, data, 2);

	//peer length = peer header(8bytes) + peer data
	if (m_pCommDrv->read(data, 2, nTimeout) != 2) {
		TRACER_STREAM("QthGenPacketRecv peer length \r\n");
		return HS_READ_DATA_FAIL;
	}
	nPeerSize = (int)(data[1]<<8) +(int)(data[0]);
	memcpy(temp + 6, data, 2);

	//confirm = token(2bytes) + confID(2bytes) +confirm body 
	if (m_pCommDrv->read(data, nLocalSize, nTimeout) != nLocalSize) {
		TRACER_STREAM("QthGenPacketRecv confirm body \r\n");
		return HS_READ_DATA_FAIL;
	}

	memcpy(temp + 8, data, nLocalSize);

	//check token(2bytes)
	if (temp[8]!=PayloadCommand[0] || temp[9]!=PayloadCommand[1])
	{
		TRACER_STREAM("QthGenPacketRecv check token fail \r\n");
		return HS_READ_DATA_FAIL;
	}

	//check confirm id (2bytes) =  request id +1
	int nConfirmID = (int)(temp[11]<<8) +(int)(temp[10]);
	int nReqID = (int)(PayloadCommand[3]<<8) +(int)(PayloadCommand[2]);

	if (nConfirmID != nReqID + 1)
	{
		TRACER_STREAM("QthGenPacketRecv check confirm id fail \r\n");
		return HS_READ_DATA_FAIL;
	}

	//confirm data = confirm - token(2bytes) - confID(2bytes)
	memcpy(pConfirmData, &temp[11], nLocalSize-4);

	//peer buffer = peer header(8bytes) + peer data
	if (m_pCommDrv->read(data, nPeerSize, nTimeout) != nPeerSize) {
		TRACER_STREAM("QthGenPacketRecv peer \r\n");
		return HS_READ_DATA_FAIL;
	}
	memcpy(temp + 8 + nLocalSize, data, nPeerSize);

	//peer data
	memcpy(pPeerData, &temp[8+nLocalSize +8], nPeerSize-8);

	//check size
	if (nTotalSize != nLocalSize + nPeerSize + 4 + 1)//
	{
		TRACE("%s packet size: %d\n", __FUNCTION__, nTotalSize);
		TRACE("%s local size: %d\n", __FUNCTION__, nLocalSize);
		TRACE("%s peer size: %d\n", __FUNCTION__, nPeerSize);
		return HS_READ_DATA_FAIL;
	}

	//nPayloadSize = data[3] + data[2] * 16 * 16 + data[1] * 16 * 16 * 16 + data[0] * 16 * 16 * 16 * 16;

	//checksum
	if (m_pCommDrv->read(data, 1, nTimeout) != 1) {
		TRACER_STREAM("QthGenPacketRecv checksum\r\n");
		return HS_READ_DATA_FAIL;
	}

	unsigned char PacketChecksum = temp[0];
	//check whole packet checksum
	for (int i = 1; i < 4 + nTotalSize; i++) {
		PacketChecksum ^= temp[i];
	}

	//level 1: check whole packet
	if (PacketChecksum != data[0]) {
		TRACER_STREAM("!" << __FUNCTION__ << ",PacketChecksum:" << PacketChecksum << "\r\n");
		TRACE("%s packet checksum fail\n", __FUNCTION__);
		return HS_INVALID_CHECKSUM;
	}

	return HS_OK;

}

int CmdCtrl::MTKGeneralCmdInOut(int nPort, int nTimeout,int nBodySize, BYTE* ReqBody,int nPeerBufferSize,BYTE* PeerBuffer,BYTE* OutConfirm, BYTE * OutPeerData)
{
	if (m_pCommDrv == NULL) {
		return HS_NOT_INITIALIZE;
	}

	m_isGeneralCmd = true;
	int nRetCode = HS_FAIL;
	if (!m_isOpen) {
		nRetCode = open(nPort, nTimeout);
		if (nRetCode != HS_OK) {
			return P21_ERR_OPENDEVICE + nRetCode;
		}
	}
	nRetCode = MTKGenCommand(nBodySize, ReqBody,nPeerBufferSize, PeerBuffer, OutConfirm,OutPeerData);
	if (nRetCode != HS_OK) {
		TRACE(_T("%s MTKGeneralCmdInOut(%d) fail\r\n"), __FUNCTION__, nRetCode);
		close(nPort, nTimeout);
		return nRetCode + P21_ERR_GENERALCMDINOUT;
	}

	if (m_isOpen) {
		close(nPort, nTimeout);
	}
	m_isGeneralCmd = false;

	return HS_OK;
}
