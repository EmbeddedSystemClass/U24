#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_


#include <string>
#include <vector>
#include <map>
#include <cctype>
#include "CommonDefine.h"


bool STDStringTokenizer(const std::string& strSource,
	const std::string& strDelimiter,
	std::vector<std::string>& vList,
	bool bAppendDelimiter = false);
bool ReSplitVector(const std::vector<std::string>& vSource,
	std::vector<std::string>& vResult,
	int iCollectItemNumber);
bool RemoveFrontSpace(const std::string& strOriginal, std::string& strDest);
bool QueryTestItemIsEnable(const std::vector<std::string>& vTestList, int iTestItemID);
bool IsDigit(std::string str);
std::string MakeSTDstring(const char* fmt, ...);
std::string MakeStr(const char* fmt, ...);
bool COMDeviceControl(char* szDeviceName, int iBaudRate, char* szCommand, char* szErrMsg);


// GSM functions
double GSM_ULChannelToFrequency(int iBand, int iChannel);
double GSMDwonlinkChannelToFrequency(int iBand, int iChannel);
bool GSM_GetBandNameForCableLoss(int iBand, std::string& strBandName);
bool GSM_GetRangeForTxRxCableLoss(int iBand, int iChannel, std::string& strRange);
bool EDGE_IsMultiSlot(const std::string& strSlotList, int& iMultiAmount, bool& bIsMultiSlot);


// WCDMA functions
int WCDMA_GetDLChannel(int iBand, int iULChannel);
int WCDMA_GetULChannel(int iBand, int iDLChannel);
double WCDMAUplinkChannelToFrequency(int iBand, int iChannel);
double WCDMAChannelToFrequency(int iBand,int iChannel);
bool WCDMACheckValidChannel(int iBand,int iChannel);
bool WCDMACheckValidBand(int iBand);
bool WCDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName);
bool WCDMA_GetRangeForTxCableLoss(int iBand, int iChannel, std::string& strRange);
bool WCDMA_GetRangeForRxCableLoss(int iBand, int iChannel, std::string& strRange);


// WiFi functions
double WiFi_ChannelToFrequency(int iChannel);

// Auto detect device functions
bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);
///////////////////////////////////////////////////////////////////////////////////////////
//
//	CRangePair
//
///////////////////////////////////////////////////////////////////////////////////////////

//class CRangePair
//{
//public:
//	void SetRange(double dlFirst, double dlSecond)
//	{
//		if (dlFirst >= dlSecond)
//		{
//			m_dlMax = dlFirst;
//			m_dlMin = dlSecond;
//		}
//		else
//		{
//			m_dlMax = dlSecond;
//			m_dlMin = dlFirst;
//		}
//	}
//
//	bool InRange(double dlValue)
//	{
//		if (dlValue > m_dlMax || dlValue < m_dlMin)
//			return false;
//
//		return true;
//	}
//
//	inline double GetMax() { return m_dlMax; }
//	inline double GetMin() { return m_dlMin; }
//
//private:
//	double m_dlMax;
//	double m_dlMin;
//};


// LTE functions //--------------------------------------------------------------------
bool LTE_GetPhoneRFModeByBand(int iBand, short& nRFMode);
bool LTE_GetPhoneNVModeByBand(int iBand, short& nNVMode);
bool LTE_GetBSTxChannelByMSTxChannel(int iBand, unsigned int iMSTxChannel, unsigned int& iBSTxChannel);
bool LTE_GetMSTxChannelByBSTxChannel(int iBand, unsigned int iBSTxChannel, unsigned int& iMSTxChannel);
bool LTE_GetBandFrequency(int iBand,
                          unsigned int& iBSTxChannelLower, unsigned int& iBSTxChannelUpper,
                          unsigned int& iMSTxChannelLower, unsigned int& iMSTxChannelUpper,
                          unsigned int& iBSTxFreqLower,    unsigned int& iBSTxFreqUpper,
                          unsigned int& iMSTxFreqLower,    unsigned int& iMSTxFreqUpper);
bool LTE_MSTxChannelToFrequency(int iBand, unsigned int iMSTxChannel, unsigned int& uiFreq_Hz);
bool LTE_BSTxChannelToFrequency(int iBand, unsigned int iBSTxChannel, unsigned int& uiFreq_Hz, bool bIsCal = false);
bool LTE_GetBandNameForCableLoss(int iBand, std::string& strBandName);
bool LTE_GetRangeForMSTxCableLoss(int iBand, unsigned int iMSTxChannel, std::string& strRange);
bool LTE_GetRangeForBSTxCableLoss(int iBand, unsigned int iBSTxChannel, std::string& strRange);
bool LTE_CalculateMSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss);
bool LTE_CalculateBSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss);
bool LTE_GetBandForCMW(int iBand, std::string& strBand);

#endif // End of #ifndef _COMMON_FUNCTION_H_
