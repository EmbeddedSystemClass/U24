/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of Mobile Phone operate object.
 * FileName   : CPhone.h
 * Version    : $Revision:   1.19  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/16
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2011/03/16      Gilpin.Xi						Create.
 *  2    2011/07/22      Gilpin.Xi      Modify for FTD command document update
 ********************************************************************************/

#ifndef _C__PHONE_H_
#define _C__PHONE_H_

#include "FTD_Command_API.h"
#include "../CommonUtility/BaseObject/IPhone.h"
#include "../CommonUtility/ErrorCode.h"
#include "../CommonUtility/EnumFiles.h"
#include "../CommonUtility/BaseObject/ITesterDevice.h"
#include "../../../Lib/Qualcomm/inc/QLib.h"
#include "../../../Lib/Qualcomm/inc/QLib_Defines.h"
#pragma comment(lib, "QMSL_MSVC9R")

#ifndef ASYNC
#define ASYNC 0x01
#endif

#define CPHONE_FTD_CMD_IN_OUT_CREATE(cphone_func_name, dll_api_name)                  \
bool CPhone::cphone_func_name(int nPort, int nTimeout, char* szInput, char* szOutput) \
{                                                                                     \
	return FTDCmdCtrl(nPort, nTimeout, dll_api_name, szInput, szOutput);              \
}

class CPhone : public IPhone
{
RF_DECLARE_DYNCREATE(CPhone)


// Data members
public:
	HANDLE m_hPhone;
	unsigned short m_nFtdPort;
	bool m_bUseQPST;
	qlib_library_mode m_qLibMode;
	bool m_IsHandshakeconnect;
	BOOL m_bLogging;
	bool m_bConnected;

protected:
private:
	HMODULE		m_hFuncBoardDLL;
	HMODULE		m_hFTDCmdDLL;
	PROCESS_INFORMATION processInfo;

// Member functions
public:
	CPhone();
	virtual ~CPhone();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	// QMSL functions
	virtual bool Initial_QMSL();
	virtual bool IsConnected_QMSL();
	virtual bool Disconnect_QMSL();

	// USB4702 Control Board functions
	virtual bool InitControlBoard();
	virtual bool USB4702_Close();
	virtual bool USB4702_SetAOVol(int nAddress, int iChannel, char *voltage);
	virtual bool USB4702_GetADCVol(int nAddress, int iChannel, char *OutValue);
	virtual bool USB4702_SetDOBit(int nAddress, int ibit);
	virtual bool USB4702_ClearDOBit(int nAddress, int ibit);

	// Phone functions
	virtual bool Initial();
	virtual bool IsConnected();
	virtual bool IsConnectedMulti();
	virtual bool IsMobileInFTDMode(int nPort, int nTimeout);
	virtual bool MobileAutoFTDMode(int nPort, int nTimeout);
	virtual bool Disconnect();

	virtual bool ExecAdbCommand(CString str_command, char* szOutput, char* szErrorMsg);
	virtual bool ExecFastbootCommand(CString str_command, char* szOutput, char* szErrorMsg);
	virtual int  IsConnectedMode( int iType );

	//Device
	virtual bool InitDevice(int nPort, int nTimeout);
	virtual bool FreeDevice(int nPort, int nTimeout);
	virtual bool SearchPortUSB(const UINT nTimeout);

	//Receive Data
	virtual bool ExactOpenDevice(int nPort, int nTimeout);
	virtual bool ExactCloseDevice(int nPort, int nTimeout);
	virtual bool ReceiveCommandData(int nPort, int nTimeout, char* cIDCommand, char* szOutput);

	//CAMERA
	virtual bool CAMERA_5M_Test(int nPort, int nTimeout, int index, char* szOutput);
	//LCD
	virtual bool Display_LCD_RGB(int nPort, int nTimeout, char* sz_displayColor, char* szOutput);
	//Touch
	// szOutput: point coord
	virtual bool TouchPanelTest(int nPort, int nTimeout, bool testFlag, char* szOutput);
	// szOutput: point coord
	virtual bool TouchCapkeyTest(int nPort, int nTimeout, bool testFlag, char* szOutput);
	virtual bool TOUCHversion(int nPort, int nTimeout, char* szInput, char* szOutput);

	//BATTERY
	virtual bool CheckDFIVersion(int nPort, int nTimeout, char* szOutput);

	//ID Test
	virtual bool ReadGaugeID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadChargerID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadBatteryID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadCpuID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadGSensorID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadECompassID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadGyroID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadCapKeyID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadTouchID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadLSensorID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadPSensorID(int nPort, int nTimeout, char* szOutput);
	virtual bool ReadTouchModel(int nPort, int nTimeout, char* szModel);


	//LED
	virtual bool LED_BacklightOnOff(int nPort, int nTimeout, char* szInput, char* szOutput);

	//NV
	virtual bool ReadNVItem(int nPort, int nTimeout, int nvID, char* nvData);
	virtual bool WriteNVItem(int nPort, int nTimeout, int nvID, char* szInputData, size_t nInputSize, char* nvStatus);

	// QMSL phone functions
	bool SetPARange_Diag(unsigned short unSlot, unsigned short unPARange);
	bool SetRGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType);
	BOOL SetBand(int Band);
	int SetRFChannel(unsigned short iChannel);
	BOOL PhoneIsConnected(void);
	BOOL ConnectThePhone(void);
	//int  GetPhoneMode(unsigned long *piPhoneMode);
	BOOL ConnectServer();
	void DisconnectServer(void);
	void SetCOMPort(unsigned int iComPort);
	BOOL DisconnectThePhone(void);
	int FTM_FlushRX_Buffer();
	BOOL SetPhoneBand(FTM_RF_Mode_Enum phoneBand);
	BOOL SetBand(FTM_RF_Mode_Enum Band );
	int SetTxOnOff(int On_Off);
	bool ReadWCDMANV(unsigned short unIndex, unsigned char* pData, int iSize);
	int SetPhoneID(short iModeId);
	bool GetWCDMAPhoneMode(short nMode);
	bool SetWCDMAWave(unsigned char iOnOff);
	bool SetPARange(unsigned short unRange);
	bool SetPDM(unsigned short unPDM);
	int ReadGSMNV(unsigned short iItemID, unsigned char* piItemData, int iLength);
	int IsFTM_Mode(unsigned char* pbIsFTMMode);
	int ChangeToFTMMode(unsigned char bFTMmode);
	int CDMA2000_CW_Waveform(unsigned char bSelectCW);
	int SetCommandCode(short iCommandCode);

	bool FTDCmdCtrl(int nPort, int nTimeout, char* szFuncName, char* pszInput, char* pszOutput);

	// 01-Battery
	bool FTD_BATTcoincellVoltage(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BATTcoincellChargeOn(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BATTrtcTime(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	// 02-Power
	bool FTD_PWRgaugeID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRgaugeVoltage(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRgaugeCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRgaugeTemperature(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRchargerID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRchargerOn(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRbattCapacity(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_PWRWriteRsenseData(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);// cmd: 13
	bool FTD_PWRAverageGaugeCurrent(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);// cmd: 14

	bool FTD_PWRRFBandIDTemperature(int nPort, int nTimeout, char* pszInput, char* pszOutput);// cmd: 11

	bool FTD_WriteMaxChargeCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput);// cmd: 15

	bool FTD_ReadPowerMaxCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput);// cmd: 16

	bool FTD_ReadBuckID(int nPort, int nTimeout, char* pszInput, char* pszOutput);// cmd: 18
	bool FTD_ReadBuckfeature(int nPort, int nTimeout, char* pszInput, char* pszOutput);// cmd: 19
	
	// 03-Memory
	bool FTD_MCPflashID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPddrID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPddrSize(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPCsdInfo(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_MCPemmcsize(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPemmcFWversion(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPddrmr8(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_MCPddrmr6(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	// 04-LCM
	bool FTD_TouchSelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_TouchModel(int nPort, int nTimeout, char* pszInput, char* pszOutput);//cmd:10
	bool FTD_OLEDTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);//cmd:14
	bool FTD_BacklightDriver(int nPort, int nTimeout, char* pszInput, char* pszOutput);//cmd:14
	
	// 05-Audio
	bool FTD_AUpathOut(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUpathSwitch(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUplaymp3(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUrecordWAV(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUplayWAV(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUloopbackTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUdetectHeadset(int nPort, int nTimeout, char* pszOutput);
	bool FTD_AUButtonCheck(int nPort, int nTimeout, char* pszOutput);
	bool FTD_AUinitADC(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUADCCali(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUheadsetInfo(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_AUButtonCount(int nPort, int nTimeout, char* pszOutput);
	// 06-Camera
	bool FTD_CAMReadID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_CAMshot(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_CAMstart(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_CAMend(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_CAMFlashLEDID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
	bool FTD_CAM_OTP_AWB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:7
	bool FTD_CAM_OTP_AFMacro(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8
	bool FTD_CAM_OTP_AFInfinity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
	bool FTD_CAM_OTP_FrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:10
	bool FTD_CAM_OTP_WriteFrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:11
	bool FTD_CAM_Read_Bridge_ID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:12
	

	// 07-Keypad
	bool FTD_KeyTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	// 08-SIM
	bool FTD_SIMtest(int nPort, int nTimeout, char* pszOutput);
	bool FTD_SIMdetect(int nPort, int nTimeout, char* pszOutput);
    //10-usb YTT
	bool FTD_USBTest(int nPort, int nTimeout, char* pszOutput);
	//10-Vibrator
	bool FTD_VibRun(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	//13-BT
	bool FTD_BTid(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BTTestMode(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BTTXPower(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_BTRXrssi(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BTSelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BTLteTx(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_BTLteRx(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_2_4GTxPower(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_2_4GPowerMode(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_2_4GConsumer(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	//14-WLAN
	bool FTD_WLAN_Mode(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_WLAN_TXPower(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_WLAN_ScanAP(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_WLAN_selfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	
	//15-GPS
	bool FTD_GPSValueCN0(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	//16-FMR
	bool FTD_FMRrssi(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FMR_SelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_FMRModuleOn(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	//17-NFC
	bool FTD_NFCid(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_NFCFWVersion(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_NFCAntennaSelftest(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_NFCSwpSelftest(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	//18-Sensor
	bool FTD_SNSRgsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRgsensorAcc(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRgsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRecompassID(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_SNSRlsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRlsensorValue(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRlsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRpsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRpsensorValue(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRpsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_SNSRloadFALPkdata(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_SNSREcompassSelfTest(int nPort, int nTimeout,char* p_sz_Input, char* pszOutput);
	bool FTD_SNSRGyroscopeTest(int nPort, int nTimeout,char* p_sz_Input, char* pszOutput);
	bool FTD_SNSRGyroscopeSelfTest(int nPort, int nTimeout,char* p_sz_Input, char* pszOutput);


	bool FTD_SNSRgsensorSelfTest(int nPort, int nTimeout,char* p_sz_Input, char* pszOutput);


	// 19-LED
	bool FTD_LEDid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);
	bool FTD_CAMFlashLED(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);
	bool FTD_LEDon(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);

	// 20-SD
	bool FTD_SDdetect(int nPort, int nTimeout, char* p_sz_Output);
	bool FTD_SDdetectPin(int nPort, int nTimeout, char* p_sz_Output);

	// 21-ID
	bool FTD_CPUid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);
	bool FTD_BoardADC(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);
	bool FTD_AntennaIC(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);//13 FTD_AntennaIC


	bool FTD_Audioid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		// 6 FTD_Audioid
	bool FTD_PMICid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		// 7 FTD_PMICid


	
	bool FTD_RFPAQFEid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);	//11 FTD_RFPAQFEid
	bool FTD_RFASMid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		//12 FTD_RFASMid
	bool FTD_Boardid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		//14 FTD_Boardid
	bool FTD_SKUid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		//15 FTD_SKUid
	bool FTD_SOCversion(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);	//16 FTD_SOCversion
	bool FTD_SOCfeature(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);	//17 FTD_SOCfeature

	bool FTD_PMid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		// 6 FTD_Audioid
	bool FTD_PCBVerMPP3(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		// 9 FTD_PCBVerMPP3 ytt
	bool FTD_PCBVerMPP2(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);		// 10 FTD_PCBVerMPP2 ytt
	bool FTD_PMIid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);	//11 FTD_PMIid ytt


	// 22-ID
	bool Sapporo_SCoverDetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
	bool Sapporo_SCoverInterrupt(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
	bool Sapporo_SCoverEnable3V(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
	bool Sapporo_SCoverCheckI2C(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
	bool Sapporo_SCoverSwitch5V(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5

	// 90-OS
	bool FTD_OSpwr2home(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_OSgetprop(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_OSmodeSleep(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	// 91-version
	bool FTD_VERftd(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_VERchange(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	// 92-factory
	bool FTD_FAC_FARead(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FAC_FAWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FAC_Picasso(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_FAC_OTPRead(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FAC_OTPWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FAC_OTPEnable(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	bool FTD_FAC_CFGRead(int nPort, int nTimeout, char* pszInput, char* pszOutput);
	bool FTD_FAC_CFGWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	//93-misc
	bool Sapporo_ftdStartEnd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
	bool FTD_Get_GPIO_Value(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3

protected:
private:
	bool LoadMobileCommandDLL();
	bool LoadControlBoardDLL();		//USB4702
};


#endif // End of #ifndef _C__PHONE_H_
