#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_


#include <string>
#include <vector>
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


///////////////////////////////////////////////////////////////////////////////////////////
//
//	CRangePair
//
///////////////////////////////////////////////////////////////////////////////////////////

class CRangePair
{
public:
	void SetRange(double dlFirst, double dlSecond)
	{
		if (dlFirst >= dlSecond)
		{
			m_dlMax = dlFirst;
			m_dlMin = dlSecond;
		}
		else
		{
			m_dlMax = dlSecond;
			m_dlMin = dlFirst;
		}
	}

	bool InRange(double dlValue)
	{
		if (dlValue > m_dlMax || dlValue < m_dlMin)
			return false;

		return true;
	}

	inline double GetMax() { return m_dlMax; }
	inline double GetMin() { return m_dlMin; }

private:
	double m_dlMax;
	double m_dlMin;
};


#endif // End of #ifndef _COMMON_FUNCTION_H_
