#ifndef _I_PHONE_H_
#define _I_PHONE_H_


#include "../CommonDefine.h"
#include "CToolBaseObject.h"
#include "../../../../Lib/Qualcomm/inc/QLib.h"
#include "../../../../Lib/Qualcomm/inc/QLib_Defines.h"


interface IPhone : public CToolBaseObject
{
RF_DECLARE_DYNAMIC(IPhone)


// Data members
public:
	std::string m_str_comPort;
	std::string m_str_diagCmdFlag;
	std::string m_strSWversion;
	std::string m_strVariant;
	std::string m_strDeviceName;

	std::vector<std::string> m_devices;

protected:
private:


// Member functions
public:
	IPhone() {}
	virtual ~IPhone() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;

	// QMSL functions
	virtual bool Initial_QMSL() = 0;
	virtual bool IsConnected_QMSL() = 0;
	virtual bool Disconnect_QMSL() = 0;

	//For USB4702 Control Board functions
	virtual bool InitControlBoard(){return true;}
	virtual bool USB4702_Close(){return true;}
	virtual bool USB4702_SetAOVol(int nAddress, int iChannel, char *voltage){return true;}
	virtual bool USB4702_GetADCVol(int nAddress, int iChannel, char *OutValue){return true;}
	virtual bool USB4702_SetDOBit(int nAddress, int ibit){return true;}
	virtual bool USB4702_ClearDOBit(int nAddress, int ibit){return true;}

	// Phone functions
	virtual bool Initial() = 0;
	virtual bool IsConnected() = 0;
	virtual bool IsConnectedMulti() = 0;
	virtual bool IsMobileInFTDMode(int nPort, int nTimeout) = 0;
	virtual bool MobileAutoFTDMode(int nPort, int nTimeout) = 0;
	virtual bool Disconnect() = 0;

	virtual bool ExecAdbCommand(CString str_command, char* pszOutput, char* p_sz_ErrorMsg) = 0;
	virtual bool ExecFastbootCommand(CString str_command, char* pszOutput, char* p_sz_ErrorMsg) = 0;
	virtual int IsConnectedMode( int iType ) = 0;

	//Device
	virtual bool InitDevice(int nPort, int nTimeout) = 0;
	virtual bool FreeDevice(int nPort, int nTimeout) = 0;
	virtual bool SearchPortUSB(const UINT nTimeout) = 0;

	//Receive Data
	virtual bool ExactOpenDevice(int nComport, int nTimeout) {return true;}
	virtual bool ExactCloseDevice(int nComport, int nTimeout) {return true;}
	virtual bool ReceiveCommandData(int nPort, int nTimeout, char* cIDCommand, char* OutBuffer) = 0;

	//CAMERA
	virtual bool CAMERA_5M_Test(int nComport, int nTimeout, int FocusIndex, char* Outbuffer) {return true;}
	//LCD
	virtual bool Display_LCD_RGB(int nComport, int nTimeout, char* sz_displayColor, char* Outbuffer) {return true;}
	//Touch
	virtual bool TouchPanelTest(int nComport, int nTimeout, bool szTestFlag, char* PointCoord) {return true;}
	virtual bool TouchCapkeyTest(int nComport,  int nTimeout, bool szTestFlag, char* PointCoord) {return true;}
	virtual bool TOUCHversion(int nPort, int nTimeout, char* pszInput, char* pszOutput) {return true;}

	//BATTERY
	virtual bool CheckDFIVersion(int nComport, int nTimeout, char* p_sz_Outbuffer){return true;}

	//ID
	virtual bool ReadGaugeID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadChargerID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadBatteryID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadCpuID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadGSensorID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadECompassID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadGyroID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadCapKeyID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadTouchID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadLSensorID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadPSensorID(int nComport, int nTimeout, char* pszId) {return true;}
	virtual bool ReadTouchModel(int nComport, int nTimeout, char* pszModel) {return true;}





	//LED
	virtual bool LED_BacklightOnOff(int nComport, int nTimeout, char* InBuffer, char* OutBuffer) {return true;}

	//NV
	virtual bool ReadNVItem(int nComport, int nTimeout, int NVID, char* p_sz_nvData) {return true;}
	virtual bool WriteNVItem(int nComport, int nTimeout, int NVID, char* pszInputData, size_t i_InputSize, char* p_sz_nvStatus) {return true;}

	//Wireless Diag
	virtual bool SetPARange_Diag(unsigned short unSlot, unsigned short unPARange) = 0;
	virtual bool SetRGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType) = 0;
	virtual BOOL SetBand(int Band) = 0;
	virtual int SetRFChannel(unsigned short iChannel) = 0;
	virtual BOOL PhoneIsConnected(void)=0;
	virtual BOOL ConnectThePhone(void)= 0;
	virtual BOOL ConnectServer()= 0;
	virtual void DisconnectServer(void)= 0;
	virtual void SetCOMPort(unsigned int iComPort)= 0;
	virtual BOOL DisconnectThePhone(void)= 0;
	virtual BOOL SetPhoneBand(FTM_RF_Mode_Enum phoneBand) = 0;
	virtual int SetTxOnOff(int On_Off)= 0;
	virtual bool ReadWCDMANV(unsigned short unIndex, unsigned char* pData, int iSize) = 0;
	virtual bool GetWCDMAPhoneMode(short nMode) = 0;
	virtual int SetPhoneID(short iModeId) = 0;
	virtual bool SetWCDMAWave(unsigned char iOnOff) = 0;
	virtual bool SetPARange(unsigned short unRange) = 0;
	virtual bool SetPDM(unsigned short unPDM) = 0;
	virtual int ReadGSMNV(unsigned short iItemID, unsigned char* piItemData, int iLength) = 0;
	virtual int IsFTM_Mode(unsigned char* pbIsFTMMode) = 0;
	virtual int ChangeToFTMMode(unsigned char bFTMmode) = 0;

	virtual int CDMA2000_CW_Waveform(unsigned char bSelectCW) = 0;
	virtual int SetCommandCode(short iCommandCode) = 0;

	virtual bool FTDCmdCtrl(int nPort, int nTimeout, char* p_sz_CmdList, char* pszInput, char* pszOutput) = 0;

	//01-Battery
	virtual bool FTD_BATTcoincellVoltage(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BATTcoincellChargeOn(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BATTrtcTime(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	//02-Power
	virtual bool FTD_PWRgaugeID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRgaugeVoltage(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRgaugeCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRgaugeTemperature(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRchargerID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRchargerOn(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRbattCapacity(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_PWRWriteRsenseData(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 13
	virtual bool FTD_PWRAverageGaugeCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 14

	virtual bool FTD_PWRRFBandIDTemperature(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 11
	virtual bool FTD_WriteMaxChargeCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 15
	virtual bool FTD_ReadPowerMaxCurrent(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 16

	virtual bool FTD_ReadBuckID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 18
	virtual bool FTD_ReadBuckfeature(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;// cmd: 19
	

	//03-Memory
	virtual bool FTD_MCPflashID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_MCPddrID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_MCPddrSize(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_MCPemmcsize(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0; //8
	virtual bool FTD_MCPemmcFWversion(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0; //9
	virtual bool FTD_MCPddrmr8(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0; //10 
	virtual bool FTD_MCPddrmr6(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0; //11 


	//04-LCM
	virtual bool FTD_TouchSelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_TouchModel(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_OLEDTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_BacklightDriver(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	//05-Audio
	virtual bool FTD_AUpathOut(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUpathSwitch(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUplaymp3(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUrecordWAV(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUplayWAV(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUloopbackTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUdetectHeadset(int nPort, int nTimeout, char* pszOutput) = 0;
	virtual bool FTD_AUButtonCheck(int nPort, int nTimeout, char* pszOutput) = 0;
	virtual bool FTD_AUinitADC(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUADCCali(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUheadsetInfo(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AUButtonCount(int nPort, int nTimeout, char* pszOutput) = 0;

	//06-Camera
	virtual bool FTD_CAMReadID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_CAMshot(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_CAMstart(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_CAMend(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_CAMFlashLEDID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:6
	virtual bool FTD_CAM_OTP_AWB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:7
	virtual bool FTD_CAM_OTP_AFMacro(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:8
	virtual bool FTD_CAM_OTP_AFInfinity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:9
	virtual bool FTD_CAM_OTP_FrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:10
	virtual bool FTD_CAM_OTP_WriteFrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:11
	virtual bool FTD_CAM_Read_Bridge_ID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:12
	

	//07-Keypad
	virtual bool FTD_KeyTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	//08-SIM
	virtual bool FTD_SIMtest(int nPort, int nTimeout, char* pszOutput) = 0;
	virtual bool FTD_SIMdetect(int nPort, int nTimeout, char* pszOutput) = 0;

    virtual bool FTD_USBTest(int nPort, int nTimeout, char* pszOutput) = 0;

	//10-Vibrator
	virtual bool FTD_VibRun(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	// 13-BT
	virtual bool FTD_BTid(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BTTestMode(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BTTXPower(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_BTRXrssi(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BTSelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BTLteTx(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BTLteRx(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BT_INQUIRY(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_2_4GPowerMode(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_2_4GConsumer(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;


	//14-WLAN
	virtual bool FTD_WLAN_Mode(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_WLAN_TXPower(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_WLAN_ScanAP(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_WLAN_selfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_WLAN_MODULE_Test(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	

	//15-GPS
	virtual bool FTD_GPSValueCN0(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	//16-FMR
	virtual bool FTD_FMRrssi(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FMR_SelfTest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FMRModuleOn(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	//17-NFC
	virtual bool FTD_NFCid(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_NFCFWVersion(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_NFCAntennaSelftest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_NFCSwpSelftest(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	//18-Sensor
	virtual bool FTD_SNSRgsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRgsensorAcc(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRgsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRecompassID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_SNSRlsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRlsensorValue(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRlsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRpsensorID(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRpsensorValue(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRpsensorCalib(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRloadFALPkdata(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_SNSREcompassSelfTest(int nPort, int nTimeout,char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRGyroscopeTest(int nPort, int nTimeout,char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRGyroscopeSelfTest(int nPort, int nTimeout,char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_SNSRgsensorSelfTest(int nPort, int nTimeout,char* p_sz_Input, char* pszOutput) = 0 ;

	//19-LED
	virtual bool FTD_LEDid(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_CAMFlashLED(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_LEDon(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	//20-SD
	virtual bool FTD_SDdetect(int nPort, int nTimeout, char* pszOutput) = 0;
	virtual bool FTD_SDdetectPin(int nPort, int nTimeout, char* pszOutput) = 0;
	//21-ID
	virtual bool FTD_CPUid(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_BoardADC(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_AntennaIC(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;//13

	virtual bool FTD_PMid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		// 6 FTD_Audioid

	virtual bool FTD_SOCversion(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		// 7 FTD_PMICid

	virtual bool FTD_PCBVerMPP3(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		// 9 FTD_PCBVerMPP3 ytt
	virtual bool FTD_PCBVerMPP2(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		// 10 FTD_PCBVerMPP2 ytt
	
	virtual bool FTD_PMIid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;	//11 FTD_PMIid ytt
	virtual bool FTD_RFASMid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		//12 FTD_RFASMid
	virtual bool FTD_Boardid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		//14 FTD_Boardid
	virtual bool FTD_SKUid(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;		//15 FTD_SKUid
	//virtual bool FTD_SOCversion(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;	//16 FTD_SOCversion
	virtual bool FTD_SOCfeature(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output)= 0;	//17 FTD_SOCfeature

	//22-SmartCover
	virtual bool Sapporo_SCoverDetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:1
	virtual bool Sapporo_SCoverInterrupt(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:2
	virtual bool Sapporo_SCoverEnable3V(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:3
	virtual bool Sapporo_SCoverCheckI2C(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:4
	virtual bool Sapporo_SCoverSwitch5V(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:5

	//90-OS
	virtual bool FTD_OSpwr2home(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_OSgetprop(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_OSmodeSleep(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	//91-version
	virtual bool FTD_VERftd(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_VERchange(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	//92-factory
	virtual bool FTD_FAC_FARead(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FAC_FAWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FAC_Picasso(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_FAC_OTPRead(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FAC_OTPWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FAC_OTPEnable(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;

	virtual bool FTD_FAC_CFGRead(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;
	virtual bool FTD_FAC_CFGWrite(int nPort, int nTimeout, char* pszInput, char* pszOutput) = 0;


	//93-misc
	virtual bool Sapporo_ftdStartEnd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0;//cmd:1
	virtual bool FTD_Get_GPIO_Value(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0 ;//cmd:3
	virtual bool FTD_Check_APQ_UART(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) = 0 ;//cmd:3
	
protected:
private:
};


#endif // End of #ifndef _I_PHONE_H_

