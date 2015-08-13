#ifndef _C_CMU200_H_
#define _C_CMU200_H_


#include "../../../CommonUtility/BaseObject/ITesterDevice.h"
#include "CFuncDev_CMU.h"
#include "CFuncBT_CMU.h"
#include "CFuncWLAN_CMU.h"
#include "CFuncGPS_CMU.h"
#include "../../../../../Lib/Qualcomm/inc/QLib_Defines.h"

#define READ_BUFF_SIZE 8196
#define CMD_ARRAY_SIZE      2048 //new
#define MAX_RESULT_BUFFER_SIZE 4096 //new add by Jackiesen
#define	ARRAY_SIZE	  32767


class CCMU200 : public ITesterDevice
{
RF_DECLARE_DYNCREATE(CCMU200)


// Data members
public:
	virtual bool SetInputLoss(int iMode, double dLoss);
	virtual bool SetOutputLoss(int iMode, double dLoss);
	virtual bool SetInputPort(int iMode);
	virtual bool SetOutputPort(int iMode);

	virtual bool Initial_CMU200();
	virtual bool GetFloatResultFromString(const String& strSource,float * fResult,UINT& iResultNum);


	/*WCDMA Function*/
	virtual bool Initial_WCDMA();
	virtual bool WCDMA_InitMaxPowerMeasurement();
	virtual bool SetRFAnalyzerFreq(double dFreq);
	virtual bool WCDMA_FetchMaxPower(std::string& strMeasured);
	virtual int WCDMAFetchPower(float* f_MaxPwr_Result,UINT& iResultNum);

	/*GSM Function*/
	virtual bool Initial_GSM();
	virtual int CalGsmSetBand(UINT uiMode);
	virtual bool SetGsmCMU(double dTestFrequency);
	virtual int GSMFetchPower(float *fTestPowerArray);

	/*CDMA2000 Function*/
	virtual bool Initial_CDMA2000();
	virtual bool FetchCDMA2000NPower(double *dlMeasuredPower);
	virtual bool SetCDMA2000Band(UINT uiBand);
	virtual bool SetCDMAChannel(int iChannel);
	virtual bool RFCDMASetTxInputLevelMaxPower(double dlPwrLvl);	

protected:
private:


// Member functions
public:
	CCMU200();
	virtual ~CCMU200();

protected:
private:
};


#endif // End of #ifndef _C_CMU200_H_
