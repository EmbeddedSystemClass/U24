#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_


#include <string>
#include <vector>
#include <map>

bool STDStringTokenizer(
	const std::string& strSource,
	const std::string& strDelimiter,
	std::vector<std::string>& vList,
	bool bAppendDelimiter = false
	);

double GSMUplinkChannelToFrequency(int iBand, int iChannel);
double GSMDwonlinkChannelToFrequency(int iBand, int iChannel);

int WCDMA_GetDLChannel(int iBand, int iULChannel);
int WCDMA_GetULChannel(int iBand, int iDLChannel);

double WCDMAUplinkChannelToFrequency(int iBand, int iChannel);
double CDMA2000UplinkChannelToFrequency(int iBand, int iChannel,const int iUL_OR_DL);

bool WCDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName);
bool WCDMA_GetRangeForTxCableLoss(int iBand, int iChannel, std::string& strRange);
bool WCDMA_GetRangeForRxCableLoss(int iBand, int iChannel, std::string& strRange);

bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);
std::string MakeStr(const char* fmt, ...);
bool CheckNoCaseString(char *szFirst, char *szSecond);

#endif // End of #ifndef _COMMON_FUNCTION_H_
