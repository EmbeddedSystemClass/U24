#include "stdafx.h"
#include <fstream>
#include <cstring>
#include "../NeptuneCtrl.h"
#include "GeneralDLLApi.h"
#include "NeptuneDLLApi.h"
#include "P21Type.h"
#include "../Utility.h" // _httoi


using namespace std;

extern CNeptuneCtrlApp theApp;



char* g_DllInfo = "A1.017_070815";
int GeneralDownload(int nPort, int nTimeout, char* DLType, char* DLPath);
int GeneralUpload(int nPort, int nTimeout, char* ULType, char* ULPath);


static int GeneralDownload(int nPort, int nTimeout, char* DLType, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralDownload(nPort, nTimeout, DLType, DLPath);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

//FF: 1030
static int GeneralUpload(int nPort, int nTimeout, char* ULType, char* ULPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralUpload(nPort, nTimeout, ULType, ULPath);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall TILoad2nd(int nPort, int nTimeout, char* TI2ndFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	//Load 2nd
	int nRetCode = 0;
	if (nPort <= QTH_PORT_COM_MAX) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->UARTLoad2nd(nPort, nTimeout, TI2ndFile);
		if (nRetCode != HS_OK) {
			nRetCode = nRetCode + P21_ERR_TILOAD2ND + P21_ERR_TYPE_UART;
		}
	}
	else {
		nRetCode = theApp.m_pCmdCtrl[nPort]->USBLoad2nd(nPort, nTimeout, TI2ndFile);
		if (nRetCode != HS_OK) {
			nRetCode = nRetCode + P21_ERR_TILOAD2ND + P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall TILoadFileToDDR(int nPort, int nTimeout, char* TIDDRFile , int StartAddres )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	//Automatically jump to DDR
	int nRetCode = 0;
	theApp.m_pCmdCtrl[nPort]->m_bP21LPR2PhaseOne = true;
	if (nPort <= QTH_PORT_COM_MAX) { // UART
		nRetCode = theApp.m_pCmdCtrl[nPort]->UARTTILoadFile(nPort, nTimeout, TIDDRFile, StartAddres);
		if (nRetCode != HS_OK) {
			nRetCode = nRetCode + P21_ERR_TILOADDDR + P21_ERR_TYPE_UART;
		}
	}
	else { // USB
		nRetCode = theApp.m_pCmdCtrl[nPort]->USBTILoadFile(nPort, nTimeout, TIDDRFile, StartAddres);
		if (nRetCode != HS_OK) {
			nRetCode = nRetCode + P21_ERR_TILOADDDR + P21_ERR_TYPE_USB;
		}
	}
	theApp.m_pCmdCtrl[nPort]->m_bP21LPR2PhaseOne = false;

	return nRetCode;
}

NEPDLLAPI int __stdcall DDRRun(int nPort, int nTimeout, char* TI2ndFile , char* DDRFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = HS_FAIL;
	if (nPort <= QTH_PORT_COM_MAX) {
		//Load 2nd
		nRetCode = theApp.m_pCmdCtrl[nPort]->UARTLoad2nd(nPort, nTimeout, TI2ndFile);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_TILOAD2ND + P21_ERR_TYPE_UART;
		}
		//Automatically jump to DDR
		nRetCode = theApp.m_pCmdCtrl[nPort]->UARTTILoadFile(nPort, nTimeout, DDRFile, DT_START_ADDR);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_TILOADDDR + P21_ERR_TYPE_UART;
		}
	}
	else { // USB
		//Load 2nd
		nRetCode = theApp.m_pCmdCtrl[nPort]->USBLoad2nd(nPort, nTimeout, TI2ndFile);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_TILOAD2ND + P21_ERR_TYPE_USB;
		}
		//Automatically jump to DDR
		nRetCode = theApp.m_pCmdCtrl[nPort]->USBTILoadFile(nPort, nTimeout, DDRFile, DT_START_ADDR);
		if (nRetCode != HS_OK) {
			return nRetCode + P21_ERR_TILOADDDR + P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}


// 2006-08-31 YTT
//it is difficult to implement USB and UART control in the same object
//because USB and UART implemented with difficult protocol
//maybe we use -> CtrlObject-> Protocol Object ->Communication Object in the future
//Now I plan to use different usb control object/uart control object to control target
//Also
NEPDLLAPI int __stdcall ExactUSBTIJumpAddr(int nPort, int nTimeout, unsigned int StartAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ExactUSBTIJumpAddr(StartAddr);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall ExactUSBTIJumpAddrEx(int nPort, int nTimeout, unsigned int StartAddr, char* workPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ExactUSBTIJumpAddrEx(nPort, nTimeout, StartAddr, workPath);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall ExactReadDevice(int nPort, int nTimeout, char* InBuffer, int InSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ExactReadDevice(nPort, nTimeout, InBuffer, InSize) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall ExactWriteDevice(int nPort, int nTimeout, char* InBuffer, int InSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ExactWriteDevice(nPort, nTimeout, InBuffer, InSize) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall P21ExactOpenDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return OpenDevice(nPort, nTimeout);
}

NEPDLLAPI int __stdcall P21ExactCloseDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return CloseDevice(nPort, nTimeout);
}


NEPDLLAPI int __stdcall P21VibratorOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VIBRATOR,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21VibratorOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VIBRATOR,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21VersionGetDTVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,1,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21FlashIPLDownload(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_IPL,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

NEPDLLAPI int __stdcall P21FlashMSIPLDownload(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_OS0,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

NEPDLLAPI int __stdcall P21FlashEBOOTDownload(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_OS1,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: GetDieID 0912
NEPDLLAPI int __stdcall P21HWGetDieID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,2,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

// 2007-06-11 YTT GetHWVersion by DT according to some hardware inormation
NEPDLLAPI int __stdcall P21HWGetHWVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,4,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

// 2008-01-16 Mark Hsieh
NEPDLLAPI int __stdcall P21SDCardMoveImgToDDR(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_SD,16,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall P21SDCardReadOSVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SD,17,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21SDCardDownloadMSIPL(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	//Move image from SD card to DDR
	char Command[4] = {P21_DT_SD,16,0,0};
	char OutBuffer[256];
	memset(OutBuffer, 0, sizeof(OutBuffer));

	//// Copy image to DDR
	// "04": MSIPL
	// "05": EBOOT
	// "06": Reserved.dio
	char *InBuffer = "04";
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, strlen(InBuffer), OutBuffer);

	//// Copy image from DDR to Flash
	//////////////////////////////////////////////////////////////////////////////
	// "04": MSIPL
	// "05": EBOOT
	// "10": Reserved.dio
	Command[0] = P21_DT_FLASH;
	Command[1] = 4;
	Command[2] = 0;
	Command[3] = 0;
	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 0);
	}
	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, 20000, Command, OutBuffer, strlen(OutBuffer)); //OutBuffer = filesize
	}
	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 1);
	}
	if (nRetCode == HS_OK) {
		nRetCode = P21DLVerifyChecksumFromFlash(nPort, 5000, "04"); // MSIPL
	}
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall P21SDCardDownloadEBOOT(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	//Move image from SD card to DDR
	char Command[4] = {P21_DT_SD,16,0,0};

	char OutBuffer[256];
	memset(OutBuffer, 0, sizeof(OutBuffer));

	// "04": MSIPL
	// "05": EBOOT
	// "06": Reserved.dio
	char *InBuffer = "05";
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, strlen(InBuffer), OutBuffer);

	//Copy image from DDR to Flash
	// "04": MSIPL
	// "05": EBOOT
	// "10": Reserved.dio
	Command[0] = P21_DT_FLASH;
	Command[1] = 5;
	Command[2] = 0;
	Command[3] = 0;

	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 0);
	}
	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, 20000, Command, OutBuffer, strlen(OutBuffer)); //OutBuffer = filesize
	}
	if (nRetCode == HS_OK) {
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 1);
	}
	if (nRetCode == HS_OK) {
		nRetCode = P21DLVerifyChecksumFromFlash(nPort, 5000, "05"); // MSIPL
	}
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall P21SDCardDownloadBDTL0(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	//Move image from SD card to DDR
	char Command[4] = {P21_DT_SD,16,0,0};

	char OutBuffer[256];
	memset(OutBuffer, 0, sizeof(OutBuffer));

	// "04": MSIPL
	// "05": EBOOT
	// "06": Reserved.dio
	// "07": FLASH.os3
	// "08": FLASH.os4
	char *InBuffer = "06";
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, strlen(InBuffer), OutBuffer);
	if (nRetCode == HS_OK) {
		Command[0] = P21_DT_FLASH;
		Command[1] = P21_MENUITEM_BDTL0;
		Command[2] = 0;
		Command[3] = 0;
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 0);
	}
	if (nRetCode == HS_OK) {
		//Copy image from DDR to Flash
		Command[0] = P21_DT_FLASH;
		Command[1] = 52;
		Command[2] = 0;
		Command[3] = 0;
		char szParam[256];
		memset(szParam, 0, sizeof(szParam));
		strcpy(szParam, "0000000000"); //00 means BDLT0; 00000000 means address
		int filesize = _httoi(OutBuffer);
		filesize /= 512;
		char szSize[9];
		memset(szSize, 0, sizeof(szSize));
		IntTo8ASCII(filesize, (unsigned char*)szSize);
		strcat(szParam, szSize); //OutBuffer = filesize
		nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, 20000, Command, szParam, strlen(szParam));
	}
	if (nRetCode == HS_OK) {
		Command[0] = P21_DT_FLASH;
		Command[1] = P21_MENUITEM_BDTL0;
		Command[2] = 0;
		Command[3] = 0;
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 1);
	}
	if (nRetCode == HS_OK) {
		nRetCode = P21DLVerifyChecksumFromFlash(nPort, 5000, "06"); // MSIPL
	}
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall P21SDCardDownloadBDTL1(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	int  nOS3filesize = 0 , nOS4filesize = 0;
	char szOS3Size[9] , szOS4Size[9];

	//Move image (FLASH.os3) from SD card to DDR
	char Command[4] = {P21_DT_SD,16,0,0};
	char OutBuffer[256];
	memset(OutBuffer, 0, sizeof(OutBuffer));

	// "04": MSIPL
	// "05": EBOOT
	// "06": Reserved.dio
	// "07": FLASH.os3
	// "08": FLASH.os4
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, "07", 2, OutBuffer);
	if (nRetCode == HS_OK) {
		Command[0] = P21_DT_FLASH;
		Command[1] = P21_MENUITEM_BDTL1;
		Command[2] = 0;
		Command[3] = 0;
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 0);
	}
	if (nRetCode == HS_OK) {
		//Copy image from DDR to Flash
		Command[0] = P21_DT_FLASH;
		Command[1] = 52;
		Command[2] = 0;
		Command[3] = 0;

		char szOS3Param[256];
		memset(szOS3Param, 0, sizeof(szOS3Param));
		strcpy(szOS3Param, "1000000000"); //10 means BDLT1; 00000000 means address

		nOS3filesize = _httoi(OutBuffer);
		nOS3filesize /= 512;

		memset(szOS3Size, 0, sizeof(szOS3Size));
		IntTo8ASCII(nOS3filesize, (unsigned char*)szOS3Size);
		strcat(szOS3Param, szOS3Size);

		nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, 20000, Command, szOS3Param, strlen(szOS3Param));
	}

	//Move image (FLASH.os4) from SD card to DDR
	Command[0] = P21_DT_SD;
	Command[1] = 16;
	Command[2] = 0;
	Command[3] = 0;
	memset(OutBuffer, 0, sizeof(OutBuffer));

	// "04": MSIPL
	// "05": EBOOT
	// "06": Reserved.dio
	// "07": FLASH.os3
	// "08": FLASH.os4
	nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, "08", 2, OutBuffer);

	if (nRetCode == 1) {
		Command[0] = P21_DT_FLASH;
		Command[1] = P21_MENUITEM_BDTL1;
		Command[2] = 0;
		Command[3] = 0;

		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 0);
	}
	if (nRetCode == 1) {
		//Copy image from DDR to Flash
		Command[0] = P21_DT_FLASH;
		Command[1] = 52;
		Command[2] = 0;
		Command[3] = 0;

		char szOS4Param[19];
		memset(szOS4Param, 0, sizeof(szOS4Param));
		strcpy(szOS4Param, "10"); // 10 means BDTL1
		strcat(szOS4Param, szOS3Size);

		nOS4filesize = _httoi(OutBuffer);
		nOS4filesize /= 512;

		memset(szOS4Size, 0, sizeof(szOS4Size));
		IntTo8ASCII(nOS4filesize, (unsigned char*)szOS4Size);
		strcat(szOS4Param, szOS4Size); //OutBuffer = filesize

		nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, 20000, Command, szOS4Param, strlen(szOS4Param));
	}
	if (nRetCode == HS_OK) {
		memset(OutBuffer, 0, sizeof(OutBuffer));
		IntTo8ASCII( (nOS3filesize + nOS4filesize) * 512 , (unsigned char*)OutBuffer);

		Command[0] = P21_DT_FLASH;
		Command[1] = P21_MENUITEM_BDTL1;
		Command[2] = 0;
		Command[3] = 0;
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_SDCARD , Command, "", OutBuffer, 1);
	}
	if (nRetCode == HS_OK) {
		nRetCode = P21DLVerifyChecksumFromFlash(nPort, 5000, "07");
	}
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

//FF: ShowSDFileInfo 0912
NEPDLLAPI int __stdcall P21SDCardFileInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SD,1,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21SDCardIO1Init(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SD,10,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21SDCardCMD(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SD,11,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDInit(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDRed(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,1,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDGreen(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,2,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDBlue(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,3,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

// 2006-11-09 YTT
// LCDBacklight/SIMPower
NEPDLLAPI int __stdcall P21LCDBlack(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,4,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21LCDWhite(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,5,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21LCDBacklightOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21LCDBacklightOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21ModemSIMPowerOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_MODEM,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21ModemSIMPowerOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_MODEM,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21ModemSIMTest(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_MODEM,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:clock RTC on
NEPDLLAPI int __stdcall P21ClockRTCOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:RTC Off
NEPDLLAPI int __stdcall P21ClockRTCOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:RTC read clock
NEPDLLAPI int __stdcall P21ClockReadClock(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,3,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF:need 20sec to timeout
NEPDLLAPI int __stdcall P21BTConnect(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,5,0,0};
	int nRetCode = HS_FAIL;
	for (int i = 0; i < 3; i++) { // retry 3 times.
		nRetCode = GeneralCmdIn(nPort, 20000, Command, InBuffer, strlen(InBuffer));
		if (nRetCode == HS_OK) {
			break;
		}
	}
	return nRetCode;
}

/////////////////////////////Audio
//FF: 0920
NEPDLLAPI int __stdcall P21AudioHandsetOnMicRev(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHeadsetOnHMicHRev(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHHHFOnMicLS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioAllOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHeadsetDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,5,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioToneToRev(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioToneToHRev(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

// 2007-01-12 YTT
NEPDLLAPI int __stdcall P21AudioHookDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,12,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

///////////////////////////LED
//add by FF 0920
NEPDLLAPI int __stdcall P21LED1On(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}
//FF: 0920
NEPDLLAPI int __stdcall P21LED1Blink(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LED1Off(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}
//FF: 0920
NEPDLLAPI int __stdcall P21LED2On(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LED2Off(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDKeypadLedOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDKeypadLedOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,8,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey1LedOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,9,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey1LedOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,10,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey2LedOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,11,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey2LedOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,12,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

/////////////////////////////////////////ID
//FF: 0920
NEPDLLAPI int __stdcall P21HWGetTritonID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,1,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: 0920
NEPDLLAPI int __stdcall P21HWGetProductID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,3,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

///////////////////////////Read DT
//FF: 0920
NEPDLLAPI int __stdcall P21VersionGetDTDate(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,2,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: 0920
NEPDLLAPI int __stdcall P21VersionGetDTName(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,3,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21VersionGetIPLVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,4,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21VersionGetIPLDate(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,5,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21VersionGetIPLName(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,6,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall P21RegisterCallBack(int nPort, int nTimeout, char* RegCallBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->RegisterCallBack((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}
//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall P21UnregisterCallBack(int nPort, int nTimeout, char* RegCallBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->UnregisterCallBack((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//callback function by YT 061112
NEPDLLAPI int __stdcall RegisterCallBackChar(int nPort, int nTimeout, char* RegCallBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->RegisterCallBackChar((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}
//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall UnregisterCallBackChar(int nPort, int nTimeout, char* RegCallBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->UnregisterCallBackChar((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//FF: 0928
NEPDLLAPI int __stdcall P21FlashFormatAll(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 1016
NEPDLLAPI int __stdcall P21FlashDumpBDTL0(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,41,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: 1017
NEPDLLAPI int __stdcall P21FlashDumpDT(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,36,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: 1017
NEPDLLAPI int __stdcall P21FlashDumpMSIPL(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,38,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: 1017
NEPDLLAPI int __stdcall P21FlashDumpEboot(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,39,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

NEPDLLAPI int __stdcall P21FlashDumpBDTL1(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->FlashDumpBDTL( nPort, nTimeout, InBuffer, 1);

	return nRetCode;
}

NEPDLLAPI int __stdcall P21FlashGetServicePassword(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char szIMEI[MAX_PATH];
	char szDieID[MAX_PATH];
	memset(szIMEI, 0, sizeof(szIMEI));
	memset(szDieID, 0, sizeof(szDieID));

	int nRetCode = HS_FAIL;
	nRetCode = P21FAReadIMEI(nPort, nTimeout, szIMEI);
	if (nRetCode == HS_OK) {
		nRetCode = P21HWGetDieID(nPort, nTimeout, szDieID);
	}
	if (nRetCode == HS_OK) {
		char* szIMEITemp;
		char* szDieIDTemp;
		szIMEITemp	= _strrev(_strdup(szIMEI));
		szDieIDTemp = _strrev(_strdup(szDieID));
		memset(szIMEI, 0, sizeof(szIMEI));
		memset(szDieID, 0, sizeof(szDieID));
		strncpy(szIMEI, szIMEITemp, 4);
		strncpy(szDieID, szDieIDTemp, 5);
		long temp = (atoi(szIMEI) + atoi(szDieID)) * 9578;

		char szPasswordTemp[MAX_PATH];
		memset(szPasswordTemp, 0, sizeof(szPasswordTemp));
		for (int i = 0; i < 6; i++) {
			szPasswordTemp[i] = (char)((temp % 10) + 48);
			temp /= 10;
		}
		memcpy(OutBuffer , szPasswordTemp , sizeof(szPasswordTemp));
	}

	return nRetCode;
}

//FF: 1019
NEPDLLAPI int __stdcall P21EnableDTR(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->EnableDTR(nPort, nTimeout) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//FF: 1019
NEPDLLAPI int __stdcall P21DisableDTR(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->DisableDTR(nPort, nTimeout) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;

}

//FF: 1020
NEPDLLAPI int __stdcall P21FlashDTDownload(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_DT,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//add by FF 1023
//power on mode for UART
//send ASCII 'A' for 5 second
//recieve zzz is success
NEPDLLAPI int __stdcall P21PowerOnToUARTPacketMode(int nPort, int nTimeout, int DTRFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->NormalCmdPowerOnToUARTPacketMode(nPort, nTimeout, DTRFlag);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}
	return nRetCode;
}

//FF: 1025
NEPDLLAPI int __stdcall P21GetNeptuneCtrlDLLVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[6] = {'A', '1', '.', '0', '2', '1'};
	memcpy(OutBuffer, Command, sizeof(Command));
	return HS_OK;
}

//FF: 1025
NEPDLLAPI int __stdcall GetDLLInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	memcpy(OutBuffer, g_DllInfo, sizeof(g_DllInfo));
	return HS_OK;
}

//FF: 1026
NEPDLLAPI int __stdcall P21UARTPacketMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,5,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 1026
NEPDLLAPI int __stdcall P21USBPacketMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 1026
NEPDLLAPI int __stdcall P21GetNeptuneCtrlDLLSubVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	CString cstrVersion;
	CString cstrMainVersion;
	CString cstrVersion2;
	char cSubVer[64];
	memset(cSubVer, 0, sizeof(cSubVer));

	cstrVersion.Format("%s", __DATE__);
	int nRetCode = PreDefMacroToDigitString((char*)(LPCTSTR)cstrVersion, cSubVer, PREDEF_DATE);
	cstrVersion = CString(cSubVer);
	memset(cSubVer, 0, sizeof(cSubVer));

	cstrVersion2.Format("%s", __TIME__);
	nRetCode = PreDefMacroToDigitString((char*)(LPCTSTR)cstrVersion2, cSubVer, PREDEF_TIME);
	cstrVersion = cstrVersion + "." + CString(cSubVer);
	memcpy(OutBuffer, (LPCTSTR)cstrVersion, cstrVersion.GetLength());

	return HS_OK;
}

//FF: change to Engineer mode 1026
NEPDLLAPI int __stdcall P21EngineerMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: change to 301 mode 1026
NEPDLLAPI int __stdcall P21301Mode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: change to uart console mode 1026
NEPDLLAPI int __stdcall P21UARTConsoleMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: change to usb console mode 1026
NEPDLLAPI int __stdcall P21USBConsoleMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: enable LCD log 1026
NEPDLLAPI int __stdcall P21LCDLogEnable(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: enable uart log 1026
NEPDLLAPI int __stdcall P21UARTLogEnable(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHANGEMODE,8,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: Format Mdoc(8M,2M) for LPR2 use only 1030
NEPDLLAPI int __stdcall P21FlashFormatBDTL8M2M(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,13,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: write BTDL0 1030
NEPDLLAPI int __stdcall P21FlashWriteBDTL0(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_OS2,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

int Cut_File(char* filename, char* cFile1_Name, int iFile1_Size, char* cFile2_Name, int iFile2_Size)
{
	CFile srcFile, destFile;
	BOOL bOK = srcFile.Open(filename, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);
	if (!bOK) {
		return HS_FILE_NOT_FOUND;
	}

	const int c_page = 64 * 1024;	// 64 KB
	char buff[c_page];
	DWORD dwRead;
	UINT uWriteBytes;
	for (int i = 1; i <= 2; i++) {
		char szFileName[MAX_PATH];
		memset(szFileName, 0, sizeof(szFileName));
		strcpy(szFileName, (i == 1) ? cFile1_Name : cFile2_Name);

		if ( !destFile.Open(szFileName, CFile::modeWrite | CFile::modeCreate) ) {
			srcFile.Close();
			return HS_FILE_NOT_FOUND;
		}

		uWriteBytes = 0;
		do {
			dwRead = srcFile.Read(buff, c_page);
			if (dwRead > 0) {
				destFile.Write(buff, dwRead);
				uWriteBytes = uWriteBytes + dwRead;
			}
		}
		while (dwRead > 0 && uWriteBytes < static_cast<UINT>(iFile1_Size));
		destFile.Close();
	}
	srcFile.Close();
	return HS_OK;
}

NEPDLLAPI int __stdcall P21FlashWriteBDTL1(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	char cRetMsg[128];
	memset(cRetMsg, 0, sizeof(cRetMsg));
	//FF:get file path
	char cstrBuffer[256];
	memset(cstrBuffer, 0, sizeof(cstrBuffer));

	int nRetCode = 1;
	//// 2008-03-03 Nek Wang: Get File Size
	HANDLE hFile = ::CreateFile(DLPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile);
		TRACER_STREAM("[Port:" << nPort << "] => OS File Path: " << DLPath << "\r\n");
		return HS_FILE_NOT_FOUND;
	}

	int nOsSize = ::GetFileSize(hFile, NULL);
	::CloseHandle(hFile);
	if (nOsSize == 0xFFFFFFFF) {
		TRACER_STREAM("[Port:" << nPort << "] => OS File Path(size): " << DLPath << "\r\n");
		return HS_FILE_NOT_FOUND;
	}
	//// Get File Size

	if (nOsSize <= 60*1024*1024) {
		char Command[4] = {P21_DT_FLASH, P21_MENUITEM_OS3, 0, 0};
		int inRetCode = theApp.m_pCmdCtrl[nPort]->GeneralDownload(nPort, nTimeout, Command, DLPath);
		if (inRetCode != HS_OK) {
			if (nPort <= QTH_PORT_COM_MAX) {
				inRetCode += P21_ERR_TYPE_UART;
			}
			else {
				inRetCode += P21_ERR_TYPE_USB;
			}
		}
		return inRetCode;
	}
	else { // os > 60MB
		char cFileFirstNameTemp[MAX_PATH];
		memset(cFileFirstNameTemp, 0, sizeof(cFileFirstNameTemp));
		char cFileSecNameTemp[MAX_PATH];
		memset(cFileSecNameTemp, 0, sizeof(cFileSecNameTemp));

		TCHAR cFilePath[MAX_PATH];
		memset(cFilePath, 0, sizeof(cFilePath));

		::GetModuleFileName(NULL, cFilePath, MAX_PATH);
		(_tcsrchr(cFilePath, '\\'))[1] = 0;

		sprintf(cFileFirstNameTemp, "%s%d_1.img", cFilePath, nPort);
		sprintf(cFileSecNameTemp, "%s%d_2.img", cFilePath, nPort);

		CString csFirstPath = CString(cFileFirstNameTemp);
		CString csSecPath = CString(cFileSecNameTemp);

		nRetCode = Cut_File(DLPath, (char*)(LPCTSTR)csFirstPath, 60*1024 *1024, (char*)(LPCTSTR)csSecPath, nOsSize - 60*1024*1024);
		//// Write DL INFO (1)
		if (nRetCode == HS_OK) {
			char szFileSize[256];
			memset(szFileSize, 0, sizeof(szFileSize));
			nRetCode = theApp.m_pCmdCtrl[nPort]->DownloadInfoFileSize(cFileFirstNameTemp, szFileSize);
			if (nRetCode != HS_OK) {
				if (::DeleteFile(csFirstPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
					csFirstPath.ReleaseBuffer();
				}
				if (::DeleteFile(csSecPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
					csSecPath.ReleaseBuffer();
				}

				TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
				return nRetCode;
			}

			char Command[4] = {P21_DT_FLASH,P21_MENUITEM_BDTL1,0,0};
			nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, Command, DLPath, szFileSize, 0);
		}
		//// DLOS1 (1)
		Sleep(200);
		if (nRetCode == HS_OK) {
			nRetCode = P21FlashWriteBTDLSectors(nPort, nTimeout, (char*)(LPCTSTR)csFirstPath, "10000000000001E000");
			if (nRetCode != HS_OK) {
				if (::DeleteFile(csFirstPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
					csFirstPath.ReleaseBuffer();
				}
				if (::DeleteFile(csSecPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
					csSecPath.ReleaseBuffer();
				}

				TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
				return nRetCode;
			}
		}
		//// Write DL INFO (2)
		if (nRetCode == HS_OK) {
			char szFileSize[256];
			memset(szFileSize , 0 , sizeof(szFileSize));
			nRetCode = theApp.m_pCmdCtrl[nPort]->DownloadInfoFileSize(cFileSecNameTemp, szFileSize);
			if (nRetCode != HS_OK) {
				if (::DeleteFile(csFirstPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
					csFirstPath.ReleaseBuffer();
				}
				if (::DeleteFile(csSecPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
					csSecPath.ReleaseBuffer();
				}

				TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
				return nRetCode;
			}

			char Command[4] = {P21_DT_FLASH,P21_MENUITEM_BDTL1,0,0};
			nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, Command, DLPath, szFileSize, 0);
		}
		//// DLOS2 (2)
		char cFile2_Size[9];
		memset(cFile2_Size, 0, sizeof(cFile2_Size));
		int iTempSectors = (nOsSize - 60 * 1024 * 1024) / 512;
		nRetCode = IntTo8ASCII(iTempSectors, (unsigned char*)cFile2_Size);

		char cFile2_Parameter[19];
		memset(cFile2_Parameter, 0, sizeof(cFile2_Parameter));
		strncpy(cFile2_Parameter, "100001E000", 10);
		strncpy(cFile2_Parameter + 10, cFile2_Size, strlen(cFile2_Size));
		Sleep(200);
		if (nRetCode == HS_OK) {
			nRetCode = P21FlashWriteBTDLSectors(nPort, nTimeout, (char*)(LPCTSTR)csSecPath, cFile2_Parameter);
			if (nRetCode != HS_OK) {
				if (::DeleteFile(csFirstPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
					csFirstPath.ReleaseBuffer();
				}
				if (::DeleteFile(csSecPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
					csSecPath.ReleaseBuffer();
				}

				TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
				return nRetCode;
			}
		}
		//// Write DL INFO FINISH (DL OK)
		if (nRetCode == HS_OK) {
			char szFileSize[256];
			memset(szFileSize , 0 , sizeof(szFileSize));
			nRetCode = theApp.m_pCmdCtrl[nPort]->DownloadInfoFileSize(DLPath, szFileSize);
			if (nRetCode != HS_OK) {
				if (::DeleteFile(csFirstPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
					csFirstPath.ReleaseBuffer();
				}
				if (::DeleteFile(csSecPath) == FALSE) {
					TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
					csSecPath.ReleaseBuffer();
				}

				TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
				return nRetCode;
			}

			char Command[4] = {P21_DT_FLASH,P21_MENUITEM_BDTL1,0,0};
			nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, Command, DLPath, szFileSize, 1);
		}
		if (::DeleteFile(csFirstPath) == FALSE) {
			TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csFirstPath.GetBuffer() << "\r\n");
			csFirstPath.ReleaseBuffer();
			TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
			return HS_FILE_NOT_FOUND;
		}
		if (::DeleteFile(csSecPath) == FALSE) {
			TRACER_STREAM("[Port:" << nPort << "] => Delete Fail: " << csSecPath.GetBuffer() << "\r\n");
			csSecPath.ReleaseBuffer();
			TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
			return HS_FILE_NOT_FOUND;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

//FF: write BTDL2 1226
NEPDLLAPI int __stdcall P21FlashWriteBDTL2(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,P21_MENUITEM_OS5,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: run necleus os 1030
NEPDLLAPI int __stdcall P21OSRunNecleusOS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,5,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: Wlan Power On 1030
NEPDLLAPI int __stdcall P21WLANPowerOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: Wlan Power Off 1030
NEPDLLAPI int __stdcall P21WLANPowerOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: setWlanTestMode 1030
NEPDLLAPI int __stdcall P21WLANSetWlanTestMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: Tx Local frequency 1030
NEPDLLAPI int __stdcall P21WLANSetCmdLocalFrequecy(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,4,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Tx Output power 1030
NEPDLLAPI int __stdcall P21WLANSetCmdOutputPower(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,5,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Write MAC Address 1030
NEPDLLAPI int __stdcall P21WLANSetCmdMACAddrToEEProm(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,7,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Read MAC Address 1030
NEPDLLAPI int __stdcall P21WLANGetCmdMACAddrFromEEProm(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,8,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Get Wanla Chip ID 1030
NEPDLLAPI int __stdcall P21WLANGetMTKChipID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_WLAN,9,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Get Rssi 1030
NEPDLLAPI int __stdcall P21WLANGetCmdPacketRxStatus(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_WLAN,6,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//////////////////camera
//FF: camera power On 1106
NEPDLLAPI int __stdcall P21CameraPowerOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CAMERA,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: camera power Off 1106
NEPDLLAPI int __stdcall P21CameraPowerOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CAMERA,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: Read MDoc ID 1106
NEPDLLAPI int __stdcall P21FlashReadMDocID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,46,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

/////////SDRAM
//FF: SDRAM read write full 1106
NEPDLLAPI int __stdcall P21SDRAMReadWriteFull(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SDRAM,1,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: SDRAM read write partial 1106
NEPDLLAPI int __stdcall P21SDRAMReadWritePartial(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SDRAM,2,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}


/////////batery
//FF: ADCIN 1 1106
NEPDLLAPI int __stdcall P21HWADCIN1(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,1,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 2 1106
NEPDLLAPI int __stdcall P21HWADCIN2(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,2,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 3 1106
NEPDLLAPI int __stdcall P21HWADCIN3(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,3,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}


//FF: ADCIN 4 1106
NEPDLLAPI int __stdcall P21HWADCIN4(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,4,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 5 1106
NEPDLLAPI int __stdcall P21HWADCIN5(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,5,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 6 1106
NEPDLLAPI int __stdcall P21HWADCIN6(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,6,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 7 1106
NEPDLLAPI int __stdcall P21HWADCIN7(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,7,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 8 1106
NEPDLLAPI int __stdcall P21HWADCIN8(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,8,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 9 1106
NEPDLLAPI int __stdcall P21HWADCIN9(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,9,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 10 1106
NEPDLLAPI int __stdcall P21HWADCIN10(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,10,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: ADCIN 11 1106
NEPDLLAPI int __stdcall P21HWADCIN11(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,11,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//OS related
//FF: P21 power off 0109 modify
NEPDLLAPI int __stdcall P21OSPowerOff(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,3,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: warch dog reset 0109 modify
NEPDLLAPI int __stdcall P21OSWatchDogReset(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,4,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: write factory area 1106
NEPDLLAPI int __stdcall P21FAWriteFactoryArea(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,34,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: read factory area 1106
NEPDLLAPI int __stdcall P21FAReadFactoryArea(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,33,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//YTT: write LocalSetting
NEPDLLAPI int __stdcall P21FAWriteCountryCode(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,52,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//YTT: read LocalSetting 1106
NEPDLLAPI int __stdcall P21FAReadCountryCode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	memset(OutBuffer, 0, sizeof(OutBuffer));
	char Command[4] = {P21_DT_FACTORYAREA,51,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//for  P21FAWriteMEPDSimLockFile
int load_file_to_memory(const char * filename, char** result) //BenQ Mark Hsieh + Dec.22.2006
{
	unsigned int size = 0;
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		*result = NULL;
		return -1; // -1 means file opening fail
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*result = (char*)malloc(size + 1);
	if (size != fread(*result, sizeof(char), size, fp)) {
		free(*result);
		return -2; // -2 means file reading fail
	}
	fclose(fp);
	(*result)[size] = '\0';
	return size;
}

//for  P21FAWriteMEPDSimLockFile
int write_file_from_memory(const char * filename, char* buf, int size)
{
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		return -1; // -1 means file opening fail
	}

	int nByteWritten = (int)fwrite(buf, sizeof(char), size, fp);
	fclose(fp);

	return nByteWritten;
}

NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockFile(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	TCHAR dir[MAX_PATH];
	memset(dir, 0, sizeof(dir));
	GetModuleFileName(NULL, dir, MAX_PATH);
	(_tcsrchr(dir, '\\'))[1] = 0;
	sprintf(dir, "%stmpFA_%d.bin", dir, nPort);

	TRACER_STREAM("[Port:" << nPort << "] => FA File Path: " << dir << "\r\n");

	int ret = P21FlashDumpFA(nPort, nTimeout, dir);
	if (ret == 1) {
		char *FA_content = NULL;
		int FA_size = load_file_to_memory(dir, &FA_content);
		if ((FA_size != FA_SIZE_2G_FLASH) && (FA_size != FA_SIZE_1G_FLASH)) {
			//printf("Error: FA size is not correct!\n");
			free(FA_content);
			return HS_INVALID_SIZE;
		}

		//Load MEPD file into memory
		char *MEPD_content = NULL;
		int MEPD_size = 0;

		MEPD_size = load_file_to_memory(InBuffer, &MEPD_content);

		if (MEPD_size < 0) {
			//printf("Fail to load MEPD file!\n");
			free(FA_content);
			return HS_FILE_NOT_FOUND;
		}

		//Write MEPD to FA
		for (int i = 0; i < MEPD_size; i++) {
			FA_content[i + MEPD_OFFSET] = MEPD_content[i];
		}

		write_file_from_memory(dir, FA_content, FA_size);
		free(FA_content);
		free(MEPD_content);

		ret = P21FlashWriteFA(nPort, nTimeout, dir);
		if (ret == 1) {
			DeleteFile(dir);
			//	printf("Write MEPD to FA OK!\n");
		}
		else {
			DeleteFile(dir);
			//printf("Fail to write FA via DT!\n");
			return ret;
		}
	}
	else {
		//printf("Fail to dump FA!\n");
		return ret;
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << ret << "\r\n");
	return 1;
}

NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockDataEX(int nPort, int nTimeout, char* InBuffer, char *MEPDFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = HS_OK;
	//// Write DL INFO (1)
	if (nRetCode == HS_OK) {
		//// Not real Send to DT, bad code
		char Command[4] = {P21_DT_FACTORYAREA,P21_MENUITEM_MEPD,0,0};
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, Command, MEPDFlag, "00000800" , 0);
	}
	//// DL MEPD
	if (nRetCode == HS_OK) {
		TCHAR dir[MAX_PATH];
		memset(dir, 0, sizeof(dir));
		GetModuleFileName(NULL, dir, MAX_PATH);
		(_tcsrchr(dir, '\\'))[1] = 0;
		sprintf(dir, "%stmpFA_%d.bin", dir, nPort);

		TRACER_STREAM("[Port:" << nPort << "] => FA File Path: " << dir << "\r\n");

		nRetCode = P21FlashDumpFA(nPort, nTimeout, dir);
		if (nRetCode == HS_OK) {
			char* FA_content = NULL;
			int FA_size = load_file_to_memory(dir, &FA_content);
			if ((FA_size != FA_SIZE_2G_FLASH) && (FA_size != FA_SIZE_1G_FLASH)) {
				DeleteFile(dir);
				free(FA_content);
				return HS_INVALID_SIZE;
			}
			for (int i = 0; i < 2048; i++) {
				FA_content[i + MEPD_OFFSET] = InBuffer[i];
			}
			int nWrittenByte = write_file_from_memory(dir, FA_content, FA_size);
			if (nWrittenByte != FA_size) {
				TRACER_STREAM("[Port:" << nPort << "] => Wrong File Size : " << nWrittenByte << "\r\n");

				DeleteFile(dir);
				free(FA_content);
				return HS_INVALID_SIZE;
			}

			free(FA_content);

			nRetCode = P21FlashWriteFA(nPort, nTimeout, dir);
			DeleteFile(dir);
		}
	}
	//// Write DL INFO (finish)
	if (nRetCode == 1) {
		char szFileSize[256];
		memset(szFileSize, 0, sizeof(szFileSize));
		IntTo8ASCII((int)strlen(InBuffer) , (unsigned char*)szFileSize);

		//// Not real Send to DT, bad code
		char Command[4] = {P21_DT_FACTORYAREA,P21_MENUITEM_MEPD,0,0};
		nRetCode = theApp.m_pCmdCtrl[nPort]->WriteDownloadInfo(nPort, nTimeout, P21_DL_METHOD_USB, Command, MEPDFlag, "00000800" , 1);
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << nRetCode << "\r\n");
	return nRetCode;
}

//input is a buffer but not a file
NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockData(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}

	TCHAR dir[MAX_PATH];
	memset(dir, 0, sizeof(dir));
	GetModuleFileName(NULL, dir, MAX_PATH);
	(_tcsrchr(dir, '\\'))[1] = 0;
	sprintf(dir, "%stmpFA_%d.bin", dir, nPort);

	TRACER_STREAM("[Port:" << nPort << "] => FA File Path: " << dir << "\r\n");

	int ret = P21FlashDumpFA(nPort, nTimeout, dir);
	if (ret == HS_OK) {
		char *FA_content = NULL;
		int FA_size = load_file_to_memory(dir, &FA_content);
		if ((FA_size != FA_SIZE_2G_FLASH) && (FA_size != FA_SIZE_1G_FLASH)) {
			free(FA_content);
			return HS_INVALID_SIZE;
		}

		for (int i = 0; i < 2048; i++) {
			FA_content[i + MEPD_OFFSET] = InBuffer[i];
		}

		write_file_from_memory(dir, FA_content, FA_size);
		free(FA_content);

		ret = P21FlashWriteFA(nPort, nTimeout, dir);
		DeleteFile(dir);
		if (ret != HS_OK) {
			return ret;
		}
	}
	else {
		return ret;
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode: " << ret << "\r\n");
	return HS_OK;
}

//FF: erase necleus GSMFFS 1107
NEPDLLAPI int __stdcall P21OSEraseNucleusGSMFFS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: add by YT
NEPDLLAPI int __stdcall USB2ndSendCommand(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->USB2ndSendCommand(nPort, nTimeout, InBuffer) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}
	return nRetCode;
}

//FF: read N GSMFFS
NEPDLLAPI int __stdcall P21FlashDumpNGSMFFS(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,47,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: read W GSMFFS
NEPDLLAPI int __stdcall P21FlashDumpWGSMFFS(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,48,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

// 2006-11-16 YTT
//to dump Eboot config
NEPDLLAPI int __stdcall P21FlashDumpEbootConfig(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,40,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: write CFG 1122
NEPDLLAPI int __stdcall P21FlashWriteCFG(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,6,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: Format Mdoc(5M,60M) 1226
NEPDLLAPI int __stdcall P21FlashFormatBDTL5M60M(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,8,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: write N GSMFFS 1122
NEPDLLAPI int __stdcall P21FlashWriteNGSMFFS(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,49,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: write W GSMFFS 1122
NEPDLLAPI int __stdcall P21FlashWriteWGSMFFS(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,50,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: write IMEI 1205
NEPDLLAPI int __stdcall P21FAWriteIMEI(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,38,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Read IMEI 1205
NEPDLLAPI int __stdcall P21FAReadIMEI(int nPort, int nTimeout, char* cImei)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	TRACER_STREAM("[Port:" << nPort << "] => IMEI: " << cImei << "\r\n");

	char Command[4] = {P21_DT_FACTORYAREA,37,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, cImei);
}

// Mark Hsieh 2007.Dec.4th BEGIN
NEPDLLAPI int __stdcall P21FAVBATCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,60,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}
// Mark Hsieh 2007.Dec.4th END

NEPDLLAPI int __stdcall P21FAWriteVBATOffset(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,50,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21FAReadVBATOffset(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,49,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

// 2007-06-11 YTT Write IMEI NetworkLock Key to OTP
NEPDLLAPI int __stdcall P21FAWriteIMEIAndNLockKey(int nPort, int nTimeout, char* InBuffer, char * cLockKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,42,0,0};
	char cInBuffer[32];
	memset(cInBuffer, 0, sizeof(cInBuffer));
	memcpy(cInBuffer, InBuffer, 16); //16 for imei and 4 for network key
	memcpy(cInBuffer + 16, cLockKey, 4); //repeat copy network lock key to byte 21-24
	memcpy(cInBuffer + 20, cLockKey, 4); //repeat copy network lock key to byte 21-24
	return GeneralCmdIn(nPort, nTimeout, Command, cInBuffer, strlen(InBuffer));
}

// 2007-06-11 YTT Read IMEI NetworkLock Key to OTP
NEPDLLAPI int __stdcall P21FAReadIMEIAndNLockKey(int nPort, int nTimeout, char* cImei, char* cLockKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,41,0,0};

	char cOutBuffer[256];
	memset(cOutBuffer, 0, sizeof(cOutBuffer));

	int nRetCode = GeneralCmdOut(nPort, nTimeout, Command, cOutBuffer);

	memcpy(cImei, cOutBuffer, 16);
	memcpy(cLockKey, cOutBuffer + 16, 4);
	//check if key 0-3  equals to key 4-7
	for (int i = 0; i < 4; i++) {
		if (cOutBuffer[16+i] != cOutBuffer[20+i] ) {
			return nRetCode + 1 ;//for special check
		}
	}

	return nRetCode;
}

//FF: get camera pic 1221
NEPDLLAPI int __stdcall P21CameraQQVGA(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CAMERA,3,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: LCD Off 1221
NEPDLLAPI int __stdcall P21LCDOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD show yellow 1221
NEPDLLAPI int __stdcall P21LCDYellow(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,6,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD show Magenta 1221
NEPDLLAPI int __stdcall P21LCDMagenta(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,7,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD show Cyan 1221
NEPDLLAPI int __stdcall P21LCDCyan(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,8,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD backlight test 1221
NEPDLLAPI int __stdcall P21LCDBacklightTest(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern1(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,1,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern2(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,2,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern3(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,3,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern4(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,4,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern5(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,5,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern6(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,6,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern7(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,7,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern8(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,8,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern9(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,9,0};
	return GeneralCmd(nPort, nTimeout, Command);
}
//FF: LCD ID 1221
NEPDLLAPI int __stdcall P21LCDGetID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LCD,9,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Audio Tone to LS 1221
NEPDLLAPI int __stdcall P21AudioToneToLS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,8,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}
//FF: get sim ID 1221
NEPDLLAPI int __stdcall P21ModemGetSIMInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_MODEM,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:BT on 1221
NEPDLLAPI int __stdcall P21BTOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:BT off 1221
NEPDLLAPI int __stdcall P21BTOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,2,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:BT Test 1221
NEPDLLAPI int __stdcall P21BTTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,3,0,0};
	return GeneralCmdOut(nPort, 20000, Command, OutBuffer);
}

//FF:BT relay mode 1221
NEPDLLAPI int __stdcall P21BTRelayMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:BT disconnect 1221
NEPDLLAPI int __stdcall P21BTDisconnect(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:BT audio on 1221
NEPDLLAPI int __stdcall P21BTAudioOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: get BT ID 1221
NEPDLLAPI int __stdcall P21BTGetID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,9,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21BTRFTest(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_BT,12,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21BTFMGetRSSILevel(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_BT,13,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//FF: SDRAM line test 1221
NEPDLLAPI int __stdcall P21SDRAMLineTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SDRAM,3,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: SDRAM ID 1221
NEPDLLAPI int __stdcall P21SDRAMGetID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_SDRAM,9,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Write clock 1221
NEPDLLAPI int __stdcall P21ClockWriteClock(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,4,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, InBuffer);
}

//FF:RTC Read alarm
NEPDLLAPI int __stdcall P21ClockReadAlarm(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,5,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Write alarm 1221
NEPDLLAPI int __stdcall P21ClockWriteAlarm(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,6,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF:clock RTC on 1221
NEPDLLAPI int __stdcall P21ClockRTCAlarmPowerOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CLOCK,7,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:Charger CV On 1221
NEPDLLAPI int __stdcall P21ChargerCVOn(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,1,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF:Charger CC On 1221
NEPDLLAPI int __stdcall P21ChargerCCOn(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,2,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF:Charger off 1221
NEPDLLAPI int __stdcall P21ChargerOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,3,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:Charge BKP battery On 1221
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,4,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:Charge BKP battery Off 1221
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,5,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF:Charger status
NEPDLLAPI int __stdcall P21ChargerStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CHARGER,6,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF:Read OS version 1221
NEPDLLAPI int __stdcall P21VersionOSVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_VERSION,4,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: FA download 1226
NEPDLLAPI int __stdcall P21FlashWriteFA(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,7,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: read MDOC partition info
NEPDLLAPI int __stdcall P21FlashMDOCInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,9,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: key pad 1227
NEPDLLAPI int __stdcall P21KeypadPress(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_KEYPAD,2,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: key pad 1227
NEPDLLAPI int __stdcall P21KeypadReadMatrix(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_KEYPAD,3,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: save necleus GSMFFS 0104
NEPDLLAPI int __stdcall P21OSNucleusSaveGSMFFS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,6,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: jump ot OS 0109
//FF: DT load OS
NEPDLLAPI int __stdcall P21OSJumpToOS(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,1,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: 0131 dump FA
NEPDLLAPI int __stdcall P21FlashDumpFA(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,37,0,0};
	return GeneralUpload(nPort, nTimeout, Command, OutBuffer);
}

//FF: dump BDTL2
NEPDLLAPI int __stdcall P21FlashDumpBDTL2(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->FlashDumpBDTL( nPort, nTimeout, InBuffer, 2);

	return nRetCode;
}

//FF: dump BTDL sectors 0207
//FF: sector parameter assign by PC
NEPDLLAPI int __stdcall P21FlashDumpBDTLSectors(int nPort, int nTimeout, char* DLPath, char* parameters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_FLASH,51,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->BDTLSectorUpload(nPort, nTimeout, Command, DLPath, parameters);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//FF: write BTDL sectors 0207
//FF: sector parameter assign by PC
NEPDLLAPI int __stdcall P21FlashWriteBTDLSectors(int nPort, int nTimeout, char* DLPath, char* parameters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_FLASH,52,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->BDTLSectorDownload(nPort, nTimeout, Command, DLPath, parameters) ;
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

//FF: camera preview 0213
NEPDLLAPI int __stdcall P21CameraPreview(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CAMERA,4,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: camera VGA 0213
NEPDLLAPI int __stdcall P21CameraVGA(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_CAMERA,5,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: write BT address to FA 0215
NEPDLLAPI int __stdcall P21FAWriteBTAddress(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,40,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: read BT address from FA 0215
NEPDLLAPI int __stdcall P21FAReadBTAddress(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,39,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Audio Volume Up 0305
NEPDLLAPI int __stdcall P21AudioVULPG(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,13,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Audio Volume Down  0305
NEPDLLAPI int __stdcall P21AudioVDLPG(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_AUDIO,14,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}
//FF: dump IPL 0305
NEPDLLAPI int __stdcall P21FlashDumpIPL(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,35,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: dump BTDL3 0305
NEPDLLAPI int __stdcall P21FlashDumpBDTL3(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,54,0,0};
	return GeneralUpload(nPort, nTimeout, Command, InBuffer);
}

//FF: write BTDL3 0305
NEPDLLAPI int __stdcall P21FlashWriteBDTL3(int nPort, int nTimeout, char* DLPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,53,0,0};
	return GeneralDownload(nPort, nTimeout, Command, DLPath);
}

//FF: write 301 flag 0308
NEPDLLAPI int __stdcall P21FAWrite301Flag(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_FACTORYAREA,44,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: read 301 flag 0308
NEPDLLAPI int __stdcall P21FARead301Flag(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,43,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

//FF: Run PLDR 0313
//FF:DT set a flag to DDR, then load PLDR
NEPDLLAPI int __stdcall P21OSRunPLDR(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_RUNOS,9,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: write IMEI temp 0322
//FF: write IMEI to BDTL0
NEPDLLAPI int __stdcall P21FAWriteIMEITemp(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,46,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: Read IMEI temp 0322
//FF: Read IMEI from BDTL0
NEPDLLAPI int __stdcall P21FAReadIMEITemp(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,45,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21FAWriteVersionInfo(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,36,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21FAReadVersionInfo(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_FACTORYAREA,35,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall P21FlashFormatAllPCAssign(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,44,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF: format BDTL by PC assign parameter 0322
NEPDLLAPI int __stdcall P21FlashFormatBDTLPCAssign(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,45,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

//FF:0327
NEPDLLAPI int __stdcall P21Sidekey1LEDBlink(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_LED,13,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: copy clibration data from BDTL0 to BDK1:Bin3 (FA)
//FF: but DT will not check that calibration data is on BDTL0 or not
NEPDLLAPI int __stdcall P21FABackupGSMFFSToFA(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,47,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: copy clibration data from BDK1:Bin3 (FA) to BDTL0
//FF: but DT will not check that calibration data is on FA or not
NEPDLLAPI int __stdcall P21FARestoreGSMFFSFromFA(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FACTORYAREA,48,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//FF: check protect 0426
//FF:check protect statuse of flash
NEPDLLAPI int __stdcall P21FlashCheckProtect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,60,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall SetUIInfo(int nPort , char* UIInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return theApp.m_pCmdCtrl[nPort]->SetUIInfo(nPort , UIInfo);
}

NEPDLLAPI int __stdcall P21DLCheckServicePasswd(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_DOWNLOAD,35,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21DLVerifyChecksumFromFlash(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_DOWNLOAD,37,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21I2CTest(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_HARDWARE,5,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}
NEPDLLAPI int __stdcall P21DDRDump(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->DDRDump(nPort, nTimeout);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall P21FAGetMEPDUnlockCode(int nPort, int nTimeout, char* szUnlockCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char szIMEI[MAX_PATH];
	char szFSN[MAX_PATH];
	memset(szIMEI, 0, sizeof(szIMEI));
	memset(szFSN, 0, sizeof(szFSN));

	int nRetCode = HS_FAIL;
	nRetCode = P21FAReadIMEI(nPort, nTimeout, szIMEI);
	if (nRetCode != HS_OK) {
		TRACER_STREAM("P21FAReadIMEI() fail!\r\n");
		return nRetCode;
	}
	nRetCode = P21HWGetDieID(nPort, nTimeout, szFSN);
	if (nRetCode != HS_OK) {
		TRACER_STREAM("P21HWGetDieID() fail!\r\n");
		return nRetCode;
	}

	TRACER_STREAM("IMEI: " << szIMEI << "\r\n");
	TRACER_STREAM("DieID: " << szFSN << "\r\n");

	const unsigned int MAX_LEN_MEPD = 2048; // 2k
	const unsigned int OFFSET_MEPD  = 5 * 1024; // 5k
	char mepd[MAX_LEN_MEPD];

	char* raw_fa = NULL;
	char path_fa_dmp[MAX_PATH];
	memset(path_fa_dmp, 0, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, path_fa_dmp);
	strcat(path_fa_dmp, "\\");
	strcat(path_fa_dmp, szIMEI);
	strcat(path_fa_dmp, "_fa.dump");

	nRetCode = P21FlashDumpFA(nPort, nTimeout, path_fa_dmp);
	if (nRetCode != HS_OK) {
		TRACER_STREAM("P21FlashDumpFA() fail!\r\n");
		return nRetCode;
	}
	int length_fa = load_file_to_memory(path_fa_dmp, &raw_fa);
	if (length_fa < 0) {
		TRACER_STREAM("load_file_to_memory() fail!\r\n");
		return HS_PC_LOAD_FILE_FAIL;
	}

	memset(mepd, 0, MAX_LEN_MEPD);
	memcpy(mepd, raw_fa + OFFSET_MEPD, MAX_LEN_MEPD);
	delete raw_fa;

	unsigned char code[9];
	memset(code, 0, 9);

	// 2010-06-21 Evonne: load SimEncoder.dll
	typedef UINT (__stdcall *LPDLLFUNC)(unsigned char *, const unsigned char *, unsigned int, unsigned int, char *, char *);
	HMODULE m_hmLib = ::LoadLibrary("SimEncoder.dll");
	LPDLLFUNC lpfnDllFunc = NULL;
	if(m_hmLib) {
		lpfnDllFunc = (LPDLLFUNC)::GetProcAddress(m_hmLib, "GetUnlockCodeFromMEPD");

		if (!lpfnDllFunc) {
			AfxMessageBox("Function not found in DLL");
			FreeLibrary(m_hmLib);
			return HS_FAIL;
		}
		lpfnDllFunc(code, (const unsigned char*)mepd, MAX_LEN_MEPD, 0, szFSN, szIMEI);

	} else {
		CString msg = _T("Load: SimEncoder.dll Fail!");
		AfxMessageBox(msg);
	}

	for (int i = 0; i < 8; i++) {
		if (!isdigit(code[i])) {
			TRACER_STREAM("Invalid unlock code is retrieved!\r\n");
			return HS_FAIL;
		}
	}
	strcpy(szUnlockCode, (const char*)code);

	// 2010-06-21 Evonne: free library
	if (m_hmLib) {
		::FreeLibrary(m_hmLib);
		m_hmLib = NULL;
	}

	TRACER_STREAM("Unlock Code: " << szUnlockCode << "\r\n");
	return HS_OK;
}

NEPDLLAPI int __stdcall GetMEPDUnlockCode(unsigned char *szUnlockCode, const unsigned char *mepd, unsigned int length, unsigned int easyDecode, char *FSN, char *IMEI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	// 2010-06-21 Evonne: load SimEncoder.dll
	typedef UINT (__stdcall *LPDLLFUNC)(unsigned char *, const unsigned char *, unsigned int, unsigned int, char *, char *);
	LPDLLFUNC lpfnDllFunc = NULL;
	HMODULE m_hmLib = ::LoadLibrary("SimEncoder.dll");

	if(m_hmLib) {
		lpfnDllFunc = (LPDLLFUNC)::GetProcAddress(m_hmLib, "GetUnlockCodeFromMEPD");

		if (!lpfnDllFunc) {
			AfxMessageBox("Function not found in DLL");
			FreeLibrary(m_hmLib);
			return HS_FAIL;
		}
		lpfnDllFunc(szUnlockCode, mepd, length, 0, FSN, IMEI);

	} else {
		CString msg = _T("Load: SimEncoder.dll Fail!");
		AfxMessageBox(msg);
	}

	for (int i = 0; i < 8; i++) {
		if (!isdigit(szUnlockCode[i])) {
			TRACER_STREAM("UnlockCode digit[" << i << "] is not a number!\r\n");
			return HS_FAIL;
		}
	}

	// 2010-06-21 Evonne: free library
	if (m_hmLib) {
		::FreeLibrary(m_hmLib);
		m_hmLib = NULL;
	}

	return HS_OK;
}

NEPDLLAPI int __stdcall P21FlashExit(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_FLASH,64,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

//// Just Test
NEPDLLAPI int __stdcall P21GenCMDOutTEST(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_DOWNLOAD,4,0,0};
	return GeneralCmdOut(nPort, nTimeout, Command, OutBuffer);
}

NEPDLLAPI int __stdcall P21GenCMDInTEST(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_DOWNLOAD,4,0,0};
	return GeneralCmdIn(nPort, nTimeout, Command, InBuffer, strlen(InBuffer));
}

NEPDLLAPI int __stdcall P21GenCMDTEST(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char Command[4] = {P21_DT_DOWNLOAD,61,0,0};
	return GeneralCmd(nPort, nTimeout, Command);
}

NEPDLLAPI int __stdcall P21GenCMDInOutTEST(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	char Command[4] = {P21_DT_DOWNLOAD,4,0,0};
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, Command, InBuffer, i_InLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}
