/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *   This is a part of the Qisda RF Calibration and Test library.
 *
 *   This document contains propriety information, and except with written
 *   permission of Qisda Corp., such information shall not be 
 *   published, or disclosed to others, or used for any purpose, and the   
 *   document shall not be duplicated in whole or in part.  
 *
 *   Copyright (C) 2009 Qisda Corporation
 *   All Rights Reserved.
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */

#include "stdafx.h"
#include "CBandInfo.h"
/* 
bool CGSMBandInfo::CheckVaildCH(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			if (iChannel < 128 || iChannel > 251)
			{return false;}
			else
				return true;
			break;
		case GSM_EGSM_BAND_ID:
			if ((iChannel <= 124 && iChannel >= 0) || (iChannel <= 1023 && iChannel >= 975))
			{return true;}
			else
				return false;
			break;
		case GSM_DCS_BAND_ID:
			if (iChannel < 512 || iChannel > 885)
			{return false;}
			else
				return true;
			break;
		case GSM_PCS_BAND_ID:
			if (iChannel < 512 || iChannel > 810)
			{return false;}
			else
				return true;
		//Have Checked Band, no need default
	}
}
 
double CGSMBandInfo::ArfcnToTXHz(const int iBand, const int iArfcnCH)
{
	ASSERT(CheckVaildCH(iBand, iArfcnCH));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return 824.2+0.2*(iArfcnCH-128);
		case GSM_EGSM_BAND_ID:
			return (iArfcnCH >= 975)?(890 + 0.2 * (iArfcnCH-1024)):(890 + 0.2 * (iArfcnCH));
		case GSM_DCS_BAND_ID:
			return 1710.2 + 0.2 * (iArfcnCH - 512);
		case GSM_PCS_BAND_ID:
			return 1850.2 + 0.2 * (iArfcnCH - 512);
	}
}
double CGSMBandInfo::ArfcnToRXHz(const int iBand, const int iArfcnCH)
{
	ASSERT(CheckVaildCH(iBand, iArfcnCH));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return 869.2+0.2*(iArfcnCH-128);
		case GSM_EGSM_BAND_ID:
			return (iArfcnCH >= 975)?(935 + 0.2 * (iArfcnCH-1024)):(935 + 0.2 * (iArfcnCH));
		case GSM_DCS_BAND_ID:
			return 1805.2 + 0.2 * (iArfcnCH - 512);
		case GSM_PCS_BAND_ID:
			return 1930.2 + 0.2 * (iArfcnCH - 512);
	}
}
 
int CGSMBandInfo::GetCMU_SigMode(const int iBand){
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return _GSM850MS_SIG;
		case GSM_EGSM_BAND_ID:
			return _GSM900MS_SIG;
		case GSM_DCS_BAND_ID:
			return _GSM1800MS_SIG;
		case GSM_PCS_BAND_ID:
			return _GSM1900MS_SIG;
	}
 
}
int CGSMBandInfo::GetCMU_NSigMode(const int iBand){
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return _GSM850MS_NSIG;
		case GSM_EGSM_BAND_ID:
			return _GSM900MS_NSIG;
		case GSM_DCS_BAND_ID:
			return _GSM1800MS_NSIG;
		case GSM_PCS_BAND_ID:
			return _GSM1900MS_NSIG;
	}
}
 
 
FTM_RF_Mode_Enum CGSMBandInfo::GetPhoneMode(const int iBand)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return PHONE_MODE_GSM_850;
		case GSM_EGSM_BAND_ID:
			return PHONE_MODE_GSM_900;
		case GSM_DCS_BAND_ID:
			return PHONE_MODE_GSM_1800;
		case GSM_PCS_BAND_ID:
			return PHONE_MODE_GSM_1900;
	}
 
}
FTM_GSM_BER_Band_Enum CGSMBandInfo::GetPhoneTestMode(const int iBand)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			return FTM_GSM_BER_CELL_850;
		case GSM_EGSM_BAND_ID:
			return FTM_GSM_BER_EGSM_900;
		case GSM_DCS_BAND_ID:
			return FTM_GSM_BER_DCS_1800;
		case GSM_PCS_BAND_ID:
			return FTM_GSM_BER_PCS_1900;
	}
 
}
int CGSMBandInfo::GetChannelType(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildCH(iBand, iChannel));
	switch(iBand)
	{
		case GSM_850_BAND_ID:
			if ( iChannel < 170)
			{ return TYPE_LOW_CH;	}
			else if ( iChannel < 211){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
 
		case GSM_EGSM_BAND_ID:
			if ( iChannel <= 1023 && iChannel >=975)
			{ return TYPE_LOW_CH;	}
			else if ( iChannel <= 62 && iChannel >= 0){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case GSM_DCS_BAND_ID:
			if ( iChannel < 637)
			{ return TYPE_LOW_CH;	}
			else if ( iChannel < 762){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case GSM_PCS_BAND_ID:
			if ( iChannel < 612)
			{ return TYPE_LOW_CH;	}
			else if ( iChannel < 711){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
	}
 
}
*/
 
bool CWCDMABandInfo::CheckVaildCH(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case WCDMA_850_BAND_ID:
			if ((iChannel <= 4233 && iChannel >= 4132) || (iChannel <= 4458 && iChannel >= 4357)|| (iChannel==1007) || (iChannel==1012) || (iChannel==1032) || (iChannel==1037)||
				(iChannel==1062) || (iChannel==1087) || (iChannel==782) || (iChannel==787) || (iChannel==807) || (iChannel==812)||
				(iChannel==837) || (iChannel==862))
			{return true;}
			else
				return false;
			break;
		case WCDMA_900_BAND_ID:
			if ((iChannel <= 2863 && iChannel >= 2712) || (iChannel <= 3088 && iChannel >= 2937))
			{return true;}
			else
				return false;
			break;
		case WCDMA_AWS_BAND_ID:
			if ((iChannel <= 1513 && iChannel >= 1312) || (iChannel <= 1738 && iChannel >= 1537))
			{return true;}
			else
				return false;
			break;
		case WCDMA_PCS_BAND_ID:
			if ((iChannel <= 9538 && iChannel >= 9262) || (iChannel <= 9938 && iChannel >= 9662) || (iChannel==412) || (iChannel==437) || (iChannel==462) || (iChannel==487)||
				(iChannel==512) || (iChannel==537) || (iChannel==562) || (iChannel==587)||
				(iChannel==612) || (iChannel==637) || (iChannel==662) || (iChannel==687) || (iChannel== 12) || (iChannel== 37) || (iChannel== 62) || (iChannel== 87)||
				(iChannel==112) || (iChannel==137) || (iChannel==162) || (iChannel==187)||
				(iChannel==212) || (iChannel==237) || (iChannel==262) || (iChannel==287))
			{return true;}
			else
				return false;
			break;
		case WCDMA_IMT_BAND_ID:
			if ((iChannel <= 9888 && iChannel >= 9612) || (iChannel <= 10838 && iChannel >= 10562))
			{return true;}
			else
				return false;
			break;
			//Have Checked Band, no need default
	}

	return false;
}
 
 
int CWCDMABandInfo::GetDLChannel(const int iBand, const int iULChannel)
{
	ASSERT(CheckVaildCH(iBand, iULChannel));
	switch(iBand)
	{
		case WCDMA_850_BAND_ID:
			return iULChannel + 225;
		case WCDMA_900_BAND_ID:
			return iULChannel + 225;
		case WCDMA_AWS_BAND_ID:
			return iULChannel + 225;
		case WCDMA_PCS_BAND_ID:
			return iULChannel + 400;
		case WCDMA_IMT_BAND_ID:
			return iULChannel + 950;
	}
 
	ASSERT(0);
	return 0;
}
int CWCDMABandInfo::GetULChannel(const int iBand, const int iDLChannel)
{
	ASSERT(CheckVaildCH(iBand, iDLChannel));
	switch(iBand)
	{
	case WCDMA_850_BAND_ID:
		return iDLChannel - 225;
	case WCDMA_900_BAND_ID:
		return iDLChannel - 225;
	case WCDMA_AWS_BAND_ID:
		return iDLChannel - 225;
	case WCDMA_PCS_BAND_ID:
		return iDLChannel - 400;
	case WCDMA_IMT_BAND_ID:
		return iDLChannel - 950;
	}

	return -1;
}
 
double CWCDMABandInfo::ArfcnToHz(const int iBand, const int iArfcnCH)
{
	ASSERT(CheckVaildCH(iBand, iArfcnCH));
	switch(iBand)
	{
		case WCDMA_850_BAND_ID:
			if (iArfcnCH>=4132)
			{
				return 0.2 * (iArfcnCH);
			}else{
				return 670.1 + 0.2 * (iArfcnCH);
			}
 
		case WCDMA_900_BAND_ID:
			return 0.2 * (iArfcnCH) + 340;
		case WCDMA_AWS_BAND_ID:  /* ignore additional channels that are not in CalChannelList */
			if (iArfcnCH <= 1513 && iArfcnCH >= 1312)
			{
				return 1450 + 0.2 * (iArfcnCH);
			}else{ /* DL --> 1537 to 1738 */
				return 1805 + 0.2 * (iArfcnCH);
			}

		case WCDMA_PCS_BAND_ID:
			if (iArfcnCH>=9262)
			{
				return 0.2 * (iArfcnCH);
			}else{
				return 1850.1 + 0.2 * (iArfcnCH);
			}
 
		case WCDMA_IMT_BAND_ID:
			return 0.2 * (iArfcnCH);
	}
 
	ASSERT(0);
	return 0; 
}

/*
int CWCDMABandInfo::GetCMU_NSigMode(const int iBand)
{
	return _WCDMA19UEFDD_NSIG;
}
FTM_RF_Mode_Enum CWCDMABandInfo::GetPhoneMode(const int iBand)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case WCDMA_850_BAND_ID:
			return PHONE_MODE_WCDMA_800;
		case WCDMA_900_BAND_ID:
			return PHONE_MODE_WCDMA_BC8;
		case WCDMA_AWS_BAND_ID:
			return PHONE_MODE_WCDMA_BC4;
		case WCDMA_PCS_BAND_ID:
			return PHONE_MODE_WCDMA_1900B;
		case WCDMA_IMT_BAND_ID:
			return PHONE_MODE_WCDMA_IMT;
	}
}
int CWCDMABandInfo::GetCMU_SigMode(const int iBand)
{
	return _WCDMA19UEFDD_SIG;
}
*/ 
 
//Have to consider 850 1900 some unique band
int CWCDMABandInfo::GetChannelType(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildCH(iBand, iChannel));
	switch(iBand)
	{
		case WCDMA_850_BAND_ID:
			if ( iChannel < 4166 || (iChannel < 4391 && iChannel >= 4357))
			{ return TYPE_LOW_CH;	}
			else if (( iChannel >= 4166 && iChannel< 4199 ) || ( iChannel >= 4391 && iChannel < 4424)){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case WCDMA_900_BAND_ID:
			if ( iChannel < 2762 || (iChannel < 2987 && iChannel >= 2937))
			{ return TYPE_LOW_CH;	}
			else if (( iChannel >= 2762 && iChannel< 2813 ) || ( iChannel >= 2987 && iChannel < 3038)){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case WCDMA_AWS_BAND_ID:
			if ( iChannel < 1379 || (iChannel < 1604 && iChannel >= 1537))
			{ return TYPE_LOW_CH;	}
			else if (( iChannel >= 1379 && iChannel< 1443 ) || ( iChannel >= 1604 && iChannel < 1668)){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case WCDMA_PCS_BAND_ID:
			if ( iChannel < 9355 || (iChannel < 9755 && iChannel >= 9662))
			{ return TYPE_LOW_CH;	}
			else if (( iChannel >= 9355 && iChannel< 9445 ) || ( iChannel >= 9755 && iChannel < 9845)){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
		case WCDMA_IMT_BAND_ID:
			if ( iChannel < 9704 || (iChannel < 10654 && iChannel >= 10562))
			{ return TYPE_LOW_CH;	}
			else if (( iChannel >= 9704 && iChannel< 9796 ) || ( iChannel >= 10654 && iChannel < 10746)){
				return TYPE_MIDDLE_CH;
			}else{
				return TYPE_HIGH_CH;
			}
	}
 
	ASSERT(0);

	return -1 ;
}

/*
//CDMA Band Info
FTM_RF_Mode_Enum CCDMABandInfo::GetPhoneMode(const int iBand)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case CDMA_BC0_BAND_ID:
			return PHONE_MODE_CDMA_800;
		case CDMA_BC1_BAND_ID:
			return PHONE_MODE_CDMA_1900;
	}
}

bool CCDMABandInfo::CheckVaildCH(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildBand(iBand));
	switch(iBand)
	{
		case CDMA_BC0_BAND_ID:
			if ((iChannel <= 799 && iChannel >= 1) || (iChannel <= 1023 && iChannel >= 991)){
				return true;}
			else
				return false;
			break;
		case CDMA_BC1_BAND_ID:
			if ((iChannel <= 1199 && iChannel >= 0) )
			{return true;}
			else
				return false;
			break;
	}
}

double CCDMABandInfo::ArfcnToHz(const int iBand, const int iArfcnCH, const int iULorDL) //UL:0, DL:1
{
	ASSERT(CheckVaildCH(iBand, iArfcnCH));
	switch(iBand)
	{
		case CDMA_BC0_BAND_ID:
			if (iArfcnCH<=799)
			{
				return 825.0 + 0.03 * (iArfcnCH) + iULorDL*45;
			}else{
				return 825.0 + 0.03 * (iArfcnCH - 1023) + iULorDL*45;
			}
			break;
		case CDMA_BC1_BAND_ID:		
			return 1850.0 + 0.05 * (iArfcnCH) + iULorDL*80;
			break;
	}
 
	ASSERT(0);
	return 0; 
}

int CCDMABandInfo::GetChannelType(const int iBand, const int iChannel)
{
	ASSERT(CheckVaildCH(iBand, iChannel));
	switch(iBand)
	{
	case CDMA_BC0_BAND_ID:  // 1~799  991~1023
		if ((iChannel <= 1023 && iChannel >= 991) || ( iChannel >= 1 && iChannel< 200 )){ 
			return TYPE_LOW_CH;	
		}else if (( iChannel >= 200 && iChannel< 500 )){
			return TYPE_MIDDLE_CH;
		}else{
			return TYPE_HIGH_CH;
		}
	case CDMA_BC1_BAND_ID:  //0~1199
		if ( (iChannel < 300 && iChannel >= 0))
		{ return TYPE_LOW_CH;	}
		else if (( iChannel >= 300 && iChannel< 800 )){
			return TYPE_MIDDLE_CH;
		}else{
			return TYPE_HIGH_CH;
		}
	}
	ASSERT(0);
}

int CCDMABandInfo::GetDLChannel(const int iBand, const int iULChannel)
{
	ASSERT(CheckVaildCH(iBand, iULChannel));
	return iULChannel;
}
int CCDMABandInfo::GetULChannel(const int iBand, const int iDLChannel)
{
	ASSERT(CheckVaildCH(iBand, iDLChannel));
	return iDLChannel;
	
}
*/
