#ifndef _WCDMA_QUAL_COMM_DEF_H_
#define _WCDMA_QUAL_COMM_DEF_H_

#include "QLib.h"
#include "QLib_Defines.h"
#include <string>
// RTR6285A
// {
#define WCDMA_PA_RANGE_NUM				4
#define MAX_WCDMA_TX_AGC_PDM			255
#define MIN_WCDMA_TX_AGC_PDM			0
#define DYNAMIC_RANGE					85.3
#define MIN_WCDMA_TX_POWER				-57.3
#define MAX_HDET_VS_AGC					255
#define	WCDMA_LNA_GAIN_STATE			4
// }

#define WCDMA_DVGA_CAL_ACTION			100
#define WCDMA_LNA1_CAL_ACTION			1
#define WCDMA_LNA2_CAL_ACTION			2
#define WCDMA_LNA3_CAL_ACTION			3
#define WCDMA_LNA4_CAL_ACTION			4
#define WCDMA_NO_CAL_ACTION				255
#define WCDMA_TX_CAL_COMP_VS_FREQ_0		0
#define WCDMA_TX_CAL_COMP_VS_FREQ_1		1
#define WCDMA_TX_CAL_COMP_VS_FREQ_2		2
#define WCDMA_TX_CAL_COMP_VS_FREQ_3		3
#define WCDMA_TX_CAL_LIM_VS_FREQ		100



int			GetPhoneBandID(int iBand);
int			ConvertFromTxPwrToAGC(double dPwr);
double		ConvertFromAGCToTxPwr(int iAGC);
int			GetIndexSize(int nBeginIndex,int nEndIndex);
std::string GetBandString(int iBand);
int			GetBandIndex(int nBand);

#endif // End of #ifndef _I_TEST_ITEM_H_