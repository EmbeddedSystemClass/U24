#include "stdafx.h"
#include "WCDMA_QualComm_Def.h"
#include "../CommonUtility/CommonDefine.h"

int	GetPhoneBandID(int iBand)
{
	if (1==iBand || WCDMA_IMT_BAND_ID == iBand)
		return PHONE_MODE_WCDMA_IMT;
	else if (2 == iBand || WCDMA_PCS_BAND_ID == iBand)
		return PHONE_MODE_WCDMA_1900B;
	else if (5 == iBand || WCDMA_850_BAND_ID == iBand)
		return PHONE_MODE_WCDMA_800;
	else if (8 == iBand || WCDMA_900_BAND_ID == iBand)
		return PHONE_MODE_WCDMA_BC8;

	return -1;
}

int         ConvertFromTxPwrToAGC(double dPwr)
{
	int nAGC=0;
    nAGC = (int)((dPwr-MIN_WCDMA_TX_POWER)*(1024.0/DYNAMIC_RANGE));
    return nAGC;
}

double		ConvertFromAGCToTxPwr(int iAGC)
{
	double dTmp = (double)(DYNAMIC_RANGE/1024.0) * (double)iAGC + (double)MIN_WCDMA_TX_POWER ;
    return dTmp ;
}

int			GetIndexSize(int nBeginIndex,int nEndIndex)
{
	return abs(nEndIndex-nBeginIndex)+1;
}


std::string GetBandString(int iBand)
{
	if (iBand==2100 || iBand ==1)
		return   "UMTSBand1";
	if (iBand==1900 || iBand ==2)
		return   "UMTSBand2";
	if (iBand==900 || iBand ==8)
		return   "UMTSBand8";
	if (iBand==850 || iBand ==5)
		return   "UMTSBand5";

	return NULL;
}

int			GetBandIndex(int nBand)
{
	int nBandIndex=0;
	switch(nBand)
	{
	case WCDMA_IMT_BAND_ID:
		return 0;
	case WCDMA_PCS_BAND_ID:
		return 1;
	case WCDMA_AWS_BAND_ID:
		return 2;
	case WCDMA_900_BAND_ID:
		return 3;
	case WCDMA_850_BAND_ID:
		return 4;
	case 1:
		return 0;
	case 2:
		return 1;
	case 5:
		return 4;
	case 8:
		return 3;
	}
	return -1;
}