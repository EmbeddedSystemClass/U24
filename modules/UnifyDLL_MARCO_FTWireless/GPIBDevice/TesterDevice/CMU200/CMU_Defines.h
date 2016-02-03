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

#ifndef __CMU_DEFINES_H__
#define __CMU_DEFINES_H__
 
#define SAD_BASE				96
 
#define _RF_NSIG				1
#define _GSM900MS_NSIG			2
#define _GSM900MS_SIG			3
#define _GSM1800MS_NSIG			4
#define _GSM1800MS_SIG			5
#define _GSM1900MS_NSIG			6
#define _GSM1900MS_SIG			7
#define _AUDIO_NSIG				8
#define _GSM850MS_NSIG		    15
#define _GSM850MS_SIG		    16
#define _WCDMA19UEFDD_SIG		17
#define _WCDMA19UEFDD_NSIG		23
#define _CMUBT					10
#define _CMUBT_SIG				11
#define _CDMA2KCellMS_NSig		25
#define _CDMA2KCellMS_Sig		26
#define _CDMA2KPCSMS_NSig		27
#define _CDMA2KPCSMS_Sig		28
 
 
#define SAD_RF_NSIG					(SAD_BASE + _RF_NSIG)
#define SAD_GSM900MS_NSIG			(SAD_BASE + _GSM900MS_NSIG)
#define SAD_GSM900MS_SIG			(SAD_BASE + _GSM900MS_SIG)
#define SAD_GSM1800MS_NSIG			(SAD_BASE + _GSM1800MS_NSIG)
#define SAD_GSM1800MS_SIG			(SAD_BASE + _GSM1800MS_SIG)
#define SAD_GSM1900MS_NSIG			(SAD_BASE + _GSM1900MS_NSIG)
#define SAD_GSM1900MS_SIG			(SAD_BASE + _GSM1900MS_SIG)
#define SAD_AUDIO_NSIG				(SAD_BASE + _AUDIO_NSIG)
#define SAD_WCDMA19UEFDD_NSIG		(SAD_BASE + _WCDMA19UEFDD_SIG)
#define SAD_WCDMA19UEFDD_SIG		(SAD_BASE + _WCDMA19UEFDD_SIG)
#define SAD_GSM850MS_NSIG			(SAD_BASE + _GSM850MS_NSIG)
#define SAD_GSM850MS_SIG			(SAD_BASE + _GSM850MS_SIG)
#define SAD_BT_SIG					(SAD_BASE + _CMUBT_SIG)
#define SAD_CDMA2KCellMS_NSig		(SAD_BASE + _CDMA2KCellMS_NSig)
#define SAD_CDMA2KCellMS_Sig		(SAD_BASE + _CDMA2KCellMS_Sig)
#define SAD_CDMA2KPCSMS_NSig		(SAD_BASE + _CDMA2KPCSMS_NSig)
#define SAD_CDMA2KPCSMS_Sig			(SAD_BASE + _CDMA2KPCSMS_Sig)
 
 
#define TMO		(int)15
#define EOT		(int)1
#define EOS		(int)0
#define MINATT     (-26.0)
#define MAXATT     (+35.0)
#define STEPATT    (+5.0)
#define FWAITVALUE (100) /* sec */
#define TWAITVALUE (100) /* sec */
 
#define MAP_COMMAND		"SYST:REM:ADDR:SEC %d,\"%s\""
 
#define IDSTR_RF_NSIG				"RF_NSig"
#define IDSTR_GSM900MS_NSIG			"GSM900MS_NSig"
#define IDSTR_GSM900MS_SIG			"GSM900MS_Sig"
#define IDSTR_GSM1800MS_NSIG		"GSM1800MS_NSig"
#define IDSTR_GSM1800MS_SIG			"GSM1800MS_Sig"
#define IDSTR_GSM1900MS_NSIG		"GSM1900MS_NSig"
#define IDSTR_GSM1900MS_SIG			"GSM1900MS_Sig"
#define IDSTR_AUDIO_NSIG			"AUDIO_NSig"
#define IDSTR_WCDMA19UEFDD_SIG		"WCDMA19UEFDD_Sig"
#define IDSTR_WCDMA19UEFDD_NSIG		"WCDMA19UEFDD_NSig"
#define IDSTR_GSM850MS_NSIG			"GSM850MS_NSig"
#define IDSTR_GSM850MS_SIG			"GSM850MS_Sig"
#define IDSTR_BT_SIG				"BLUETOOTH_Sig"
#define IDSTR_CDMA2KCellMS_NSig		"CDMA2KCellMS_NSig"
#define IDSTR_CDMA2KCellMS_Sig		"CDMA2KCellMS_Sig"
#define IDSTR_CDMA2KPCSMS_NSig		"CDMA2KPCSMS_NSig"
#define IDSTR_CDMA2KPCSMS_Sig		"CDMA2KPCSMS_Sig"
 
 
#define GSM_BAND_900								10
#define GSM_BAND_DCS								11
#define GSM_BAND_PCS								12
#define GSM_BAND_850								18
 
#define GSM_BAND_900_to_850							28

#define CDMA2KCell_USC				"USC"
#define CDMA2KPCSMS_NAPC			"NAPC"

 
#endif//__CMU_DEFINES_H__
