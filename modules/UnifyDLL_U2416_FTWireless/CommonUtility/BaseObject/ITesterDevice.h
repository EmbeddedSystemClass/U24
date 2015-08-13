#ifndef _I_TESTER_DEVICE_H_
#define _I_TESTER_DEVICE_H_


#include "IGPIBDevice.h"

class IFunc_Dev;
class IFuncBT_Dev;
class IFuncWLAN_Dev;
class IFuncGPS_Dev;

interface ITesterDevice : public IGPIBDevice
{
RF_DECLARE_DYNAMIC(ITesterDevice)


// Data members
public:
	IFunc_Dev*				m_piFunc_Dev;
	IFuncBT_Dev*			m_piFuncBT;
	IFuncWLAN_Dev*			m_piFuncWLAN;
	IFuncGPS_Dev*			m_piFuncGPS;
	bool m_bInternalReference;
	static int g_iMode;

	static int m_h_RF_NSIG;
	static int m_hWCDMA19UEFDD_NSig;
	static int m_hWCDMA19UEFDD_Sig;
	static int m_h_GSM_NSig;
	static int m_h_DCS_NSig;
	static int m_h_PCS_NSig;
	static int m_h_GSM850MS_NSig;
	static int m_h_GSM900MS_NSig;
	static int m_h_GSM_Sig;
	static int m_h_DCS_Sig;
	static int m_h_PCS_Sig;
	static int m_h_GSM850MS_Sig;
	static int m_h_GSM900MS_Sig;

	static int m_h_CDMACellular_NSig;
	static int m_h_CDMAPCS_NSig;
	static int m_h_CDMACellular_Sig;
	static int m_h_CDMAPCS_Sig;
	static int g_iRFInputPort;
	static int g_iRFOutputPort;

protected:
	
private:


// Member functions
public:
	ITesterDevice();
	virtual ~ITesterDevice();

	static void SetMode(int iMode);
	static void SetRFInputPort(int iInputPort);
	static void SetRFOutputPort(int iOutputPort);
	virtual bool SetInputLoss(int iMode, double dLoss) = 0;
	virtual bool SetOutputLoss(int iMode, double dLoss) = 0;

	virtual bool Initial_CMU200() = 0;

	virtual bool Initial_GSM() = 0;
	virtual bool SetGsmCMU(double dTestFrequency) = 0;
	virtual int CalGsmSetBand(UINT uiMode) = 0; 
	virtual int GSMFetchPower(float *fTestPowerArray) = 0;

	virtual bool Initial_WCDMA() = 0;
	virtual bool SetRFAnalyzerFreq(double dFreq) = 0;
	virtual bool WCDMA_InitMaxPowerMeasurement() = 0;
	virtual bool WCDMA_FetchMaxPower(std::string& strMeasured) = 0;
	virtual int WCDMAFetchPower(float* f_MaxPwr_Result,UINT& iResultNum) = 0;


	virtual bool Initial_CDMA2000() = 0;
	virtual bool SetCDMA2000Band(UINT uiBand) = 0;
	virtual bool SetCDMAChannel(int iChannel) = 0;
	virtual bool FetchCDMA2000NPower(double *dlMeasuredPower) = 0;

	virtual bool RFCDMASetTxInputLevelMaxPower(double dlPwrLvl) = 0;	

protected:
private:
};


#endif // End of #ifndef _I_TESTER_DEVICE_H_
