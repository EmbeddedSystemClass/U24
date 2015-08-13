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

#ifndef __BAND_INFO__H__
#define __BAND_INFO__H__
 
//#include "../../Public/Inc/CommonUtil.h"
//#include "../../Public/Inc/QLib_Defines.h"
//#include "../Device/CMU200/CMU_Defines.h"
//#include "../Device/CMW500/CMW_Defines.h"
//#include "../../../../Lib/Qualcomm/inc/QLib_Defines.h"
#include <string>
 
//#define GSM_850_BAND_ID 850
//#define GSM_EGSM_BAND_ID 900
//#define GSM_DCS_BAND_ID 1800
//#define GSM_PCS_BAND_ID 1900
 
#define WCDMA_850_BAND_ID 850
#define WCDMA_900_BAND_ID 900
#define WCDMA_AWS_BAND_ID 1700
#define WCDMA_PCS_BAND_ID 1900
#define WCDMA_IMT_BAND_ID 2100

//#define CDMA_BC0_BAND_ID 800
//#define CDMA_BC1_BAND_ID 1900

 
enum CHANNEL_TYPE_ENUM{
	TYPE_LOW_CH,
	TYPE_MIDDLE_CH,
	TYPE_HIGH_CH
};
 
/* 
class CGSMBandInfo
{
public:
	static inline bool CheckVaildBand(const int iBand){
		return (iBand == GSM_850_BAND_ID) || (iBand == GSM_EGSM_BAND_ID) || (iBand == GSM_DCS_BAND_ID) || (iBand == GSM_PCS_BAND_ID);
	}
	static inline bool CheckVaildBand(const String &strBand){
		return CheckVaildBand(CStr::StrToInt(strBand));
	}
	static bool CheckVaildCH(const int iBand, const int iChannel);
	static inline bool CheckVaildCH(const String &strBand, const String &strChannel)
	{
		return CheckVaildCH(CStr::StrToInt(strBand), CStr::StrToInt(strChannel));
	}
 
	static double ArfcnToTXHz(const int iBand, const int iArfcnCH);
	static double ArfcnToRXHz(const int iBand, const int iArfcnCH);
	static inline double ArfcnToTXHz(const String &strBand, const String &strArfcnCH){
		return ArfcnToTXHz(CStr::StrToInt(strBand), CStr::StrToInt(strArfcnCH));
	}
	static inline double ArfcnToRXHz(const String &strBand, const String &strArfcnCH){
		return ArfcnToRXHz(CStr::StrToInt(strBand), CStr::StrToInt(strArfcnCH));
	}
 
	static int GetCMU_SigMode(const int iBand);
	static int GetCMU_NSigMode(const int iBand);
	static FTM_RF_Mode_Enum GetPhoneMode(const int iBand);
	static FTM_GSM_BER_Band_Enum GetPhoneTestMode(const int iBand);
	static int GetChannelType(const int iBand, const int iChannel);
 
	static inline int GetCMU_SigMode(const String& strBand)
	{
		return GetCMU_SigMode(CStr::StrToInt(strBand));
	}
	static inline int GetCMU_NSigMode(const String& strBand)
	{
		return GetCMU_NSigMode(CStr::StrToInt(strBand));
	}
 
	static inline FTM_RF_Mode_Enum GetPhoneMode(const String& strBand)
	{
		return GetPhoneMode(CStr::StrToInt(strBand));
	}
	static inline FTM_GSM_BER_Band_Enum GetPhoneTestMode(const String& strBand)
	{
		return GetPhoneTestMode(CStr::StrToInt(strBand));
	}
 
	static inline int GetChannelType(const String& strBand, const String& strChannel)
	{
		return GetChannelType(CStr::StrToInt(strBand), CStr::StrToInt(strChannel));
	}
 
 
};
*/ 
class CWCDMABandInfo
{
public:
	static inline bool CheckVaildBand(const int iBand){
		return (iBand == WCDMA_850_BAND_ID) || (iBand == WCDMA_900_BAND_ID) || (iBand == WCDMA_AWS_BAND_ID)|| (iBand == WCDMA_PCS_BAND_ID) || (iBand == WCDMA_IMT_BAND_ID);
	}
	static inline bool CheckVaildBand(const std::string &strBand){
		return CheckVaildBand(atoi(strBand.c_str()));
	}
	static bool CheckVaildCH(const int iBand, const int iChannel);
	static inline bool CheckVaildCH(const std::string& strBand, const std::string& strChannel)
	{
		return CheckVaildCH(atoi(strBand.c_str()), atoi(strChannel.c_str()));
	}
	static int GetDLChannel(const int iBand, const int iULChannel);
	static inline int GetDLChannel(const std::string& strBand, const std::string& strULChannel)
	{
		return GetDLChannel(atoi(strBand.c_str()), atoi(strULChannel.c_str()));
	}
	static int GetULChannel(const int iBand, const int iDLChannel);
	static inline int GetULChannel(const std::string& strBand, const std::string& strDLChannel)
	{
		return GetULChannel(atoi(strBand.c_str()), atoi(strDLChannel.c_str()));
	}
	static double ArfcnToHz(const int iBand, const int iArfcnCH);
	static inline double ArfcnToHz(const std::string& strBand, const std::string& strArfcnCH){
		return ArfcnToHz(atoi(strBand.c_str()), atoi(strArfcnCH.c_str()));
	}

	static int GetChannelType(const int iBand, const int iChannel);
	static inline int GetChannelType(const std::string& strBand, const std::string& strChannel)
	{
		return GetChannelType(atoi(strBand.c_str()), atoi(strChannel.c_str()));
	}

	//static int GetCMU_NSigMode(const int iBand);
	//static int GetCMU_SigMode(const int iBand);
	//static FTM_RF_Mode_Enum GetPhoneMode(const int iBand);
	
	/*
	static inline int GetCMU_NSigMode(const String& strBand)
	{
		return GetCMU_NSigMode(CStr::StrToInt(strBand));
	}
	static inline int GetCMU_SigMode(const String& strBand)
	{
		return GetCMU_SigMode(CStr::StrToInt(strBand));
	}
	static inline FTM_RF_Mode_Enum GetPhoneMode(const String& strBand)
	{
		return GetPhoneMode(CStr::StrToInt(strBand));
	}
	*/

 
};


/*
class CCDMABandInfo
{
public:
	static inline bool CheckVaildBand(const int iBand){
		return (iBand == CDMA_BC0_BAND_ID) || (iBand == CDMA_BC1_BAND_ID);
	}
	static inline bool CheckVaildBand(const String &strBand){
		return CheckVaildBand(CStr::StrToInt(strBand));
	}
	static bool CheckVaildCH(const int iBand, const int iChannel);
	static inline bool CheckVaildCH(const String &strBand, const String &strChannel)
	{
		return CheckVaildCH(CStr::StrToInt(strBand), CStr::StrToInt(strChannel));
	}
	static int GetDLChannel(const int iBand, const int iULChannel);
	static inline int GetDLChannel(const String& strBand, const String& strULChannel)
	{
		return GetDLChannel(CStr::StrToInt(strBand), CStr::StrToInt(strULChannel));
	}
	static int GetULChannel(const int iBand, const int iDLChannel);
	static inline int GetULChannel(const String& strBand, const String& strDLChannel)
	{
		return GetULChannel(CStr::StrToInt(strBand), CStr::StrToInt(strDLChannel));
	}
	static double ArfcnToHz(const int iBand, const int iArfcnCH, const int iULorDL);
	static inline double ArfcnToHz(const String &strBand, const String &strArfcnCH, const String &strULorDL){
		return ArfcnToHz(CStr::StrToInt(strBand), CStr::StrToInt(strArfcnCH), CStr::StrToInt(strULorDL));
	}
 
	static int GetCMU_NSigMode(const int iBand);
	static int GetCMU_SigMode(const int iBand);
	static FTM_RF_Mode_Enum GetPhoneMode(const int iBand);
	static int GetChannelType(const int iBand, const int iChannel);
 
	static inline int GetCMU_NSigMode(const String& strBand)
	{
		return GetCMU_NSigMode(CStr::StrToInt(strBand));
	}
	static inline int GetCMU_SigMode(const String& strBand)
	{
		return GetCMU_SigMode(CStr::StrToInt(strBand));
	}
	static inline FTM_RF_Mode_Enum GetPhoneMode(const String& strBand)
	{
		return GetPhoneMode(CStr::StrToInt(strBand));
	}
 
	static inline int GetChannelType(const String& strBand, const String& strChannel)
	{
		return GetChannelType(CStr::StrToInt(strBand), CStr::StrToInt(strChannel));
	}
 
};
 
*/
 
#endif //__BAND_INFO__H__
