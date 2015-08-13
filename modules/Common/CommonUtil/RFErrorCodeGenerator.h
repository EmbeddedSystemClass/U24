#ifndef _RF_ERROR_CODE_GENERATOR_H_
#define _RF_ERROR_CODE_GENERATOR_H_

#include <string>
#include <vector>
#include <map>
#include <cctype>


//////////////////////////////////////////////////////////////////////////
//  RF Error Code Generator related PreDefine
//   - Follow RF_ErrorCode_Rule_*.xls, from QCS.
//////////////////////////////////////////////////////////////////////////

// Technology - 2G
#define     _RFECDef_Tech_GSM           "G"
#define     _RFECDef_Tech_EDGE          "E"
#define     _RFECDef_Tech_CDMA          "C"

// Technology - 3G
#define     _RFECDef_Tech_WCDMA         "W"
#define     _RFECDef_Tech_DCHSDPA       "D"
#define     _RFECDef_Tech_1xEVDO        "V"
//#define     _RFECDef_Tech_TDSCDMA     "T"     // China 3G Standard

// Technology - 4G
#define     _RFECDef_Tech_LTE           "L"

// Mode
#define     _RFECDef_Mode_TestNoPower   "0"
#define     _RFECDef_Mode_TestInHPower  "H"
#define     _RFECDef_Mode_TestInLPower  "L"
#define     _RFECDef_Mode_Cal           "3"


// Test Item - GSM Calibration
#define     _RFECDef_Item_GSMCal_InitCal                "GK00"
#define     _RFECDef_Item_GSMCal_PA_DAC_Search          "GK01"
#define     _RFECDef_Item_GSMCal_KVCO                   "GK02"
#define     _RFECDef_Item_GSMCal_CarrierSuppressionCal  "GK03"
#define     _RFECDef_Item_GSMCal_PolarCalibration       "GK04"
#define     _RFECDef_Item_GSMCal_RxFreqCompensation     "GK05"
#define     _RFECDef_Item_GSMCal_TimingDelay            "GK06"
#define     _RFECDef_Item_GSMCal_AFCPhasing             "GK07"
#define     _RFECDef_Item_GSMCal_TxRxPhasing            "GK08"
#define     _RFECDef_Item_GSMCal_PrePhasingTask         "GK09"
#define     _RFECDef_Item_GSMCal_PostPhasingTask        "GK10"
#define     _RFECDef_Item_GSMCal_CS_Idc                 "GK11"
#define     _RFECDef_Item_GSMCal_CS_Qdc                 "GK12"
#define     _RFECDef_Item_GSMCal_LinearCalibration      "GK13"
#define     _RFECDef_Item_GSMCal_EndCal                 "GK99"

// Test Item - GSM Test
#define     _RFECDef_Item_GSMTest_InitTest              "GT00"
#define     _RFECDef_Item_GSMTest_StartCall             "GT01"
#define     _RFECDef_Item_GSMTest_MaxPower              "GT02"
#define     _RFECDef_Item_GSMTest_PVT                   "GT03"
#define     _RFECDef_Item_GSMTest_PeakPhase             "GT04"
#define     _RFECDef_Item_GSMTest_RMSPhase              "GT05"
#define     _RFECDef_Item_GSMTest_FreqError             "GT06"
#define     _RFECDef_Item_GSMTest_Switching_ORFS        "GT07"
#define     _RFECDef_Item_GSMTest_MOD_ORFS              "GT08"
#define     _RFECDef_Item_GSMTest_RxLev                 "GT09"
#define     _RFECDef_Item_GSMTest_TxCurrent             "GT10"
#define     _RFECDef_Item_GSMTest_RxBER                 "GT11"
#define     _RFECDef_Item_GSMTest_HandOver              "GT98"
#define     _RFECDef_Item_GSMTest_EndTest               "GT99"

// Test Item - EDGE Test
#define     _RFECDef_Item_EDGETest_InitTest             "ET00"
#define     _RFECDef_Item_EDGETest_StartCall            "ET01"
#define     _RFECDef_Item_EDGETest_MaxPower             "ET02"
#define     _RFECDef_Item_EDGETest_EVM_RMS              "ET03"
#define     _RFECDef_Item_EDGETest_EVM95                "ET04"
#define     _RFECDef_Item_EDGETest_FreqError            "ET05"
#define     _RFECDef_Item_EDGETest_Switching_ORFS    	"ET06"
#define     _RFECDef_Item_EDGETest_MOD_ORFS             "ET07"
#define     _RFECDef_Item_EDGETest_PVT                  "ET08"
#define     _RFECDef_Item_EDGETest_EVM_Peak             "ET09"
#define     _RFECDef_Item_EDGETest_HandOver             "ET98"
#define     _RFECDef_Item_EDGETest_EndTest              "ET99"

// Test Item - WCDMA Calibration
#define     _RFECDef_Item_WCDMACal_InitCal              "WK00"
#define     _RFECDef_Item_WCDMACal_ThermCal             "WK01"
#define     _RFECDef_Item_WCDMACal_DoDCAutoCal          "WK02"
#define     _RFECDef_Item_WCDMACal_TXLin                "WK03"
#define     _RFECDef_Item_WCDMACal_TxRxFreqComp         "WK04"
#define     _RFECDef_Item_WCDMACal_XOCoarseCal          "WK06"
#define     _RFECDef_Item_WCDMACal_XOFineCal            "WK07"
#define     _RFECDef_Item_WCDMACal_EndCal               "WK99"

// Test Item - WCDMA Test
#define     _RFECDef_Item_WCDMATest_InitTest            "WT00"
#define     _RFECDef_Item_WCDMATest_StartCall           "WT01"
#define     _RFECDef_Item_WCDMATest_MaxPower            "WT02"
#define     _RFECDef_Item_WCDMATest_MinPower            "WT03"
#define     _RFECDef_Item_WCDMATest_EVM                 "WT04"
#define     _RFECDef_Item_WCDMATest_FreqError           "WT05"
#define     _RFECDef_Item_WCDMATest_TransTimeError      "WT06"
#define     _RFECDef_Item_WCDMATest_EMASK               "WT07"
#define     _RFECDef_Item_WCDMATest_OBW                 "WT10"
#define     _RFECDef_Item_WCDMATest_BER                 "WT11"
#define     _RFECDef_Item_WCDMATest_RxLev               "WT12"
#define     _RFECDef_Item_WCDMATest_ILPC_E              "WT14"
#define     _RFECDef_Item_WCDMATest_ILPC_F              "WT15"
#define     _RFECDef_Item_WCDMATest_ACLR_n10            "WT20"
#define     _RFECDef_Item_WCDMATest_ACLR_n5             "WT21"
#define     _RFECDef_Item_WCDMATest_ACLR_5              "WT22"
#define     _RFECDef_Item_WCDMATest_ACLR_10             "WT23"
#define     _RFECDef_Item_WCDMATest_TxCurrent           "WT91"
#define     _RFECDef_Item_WCDMATest_HandOver            "WT98"
#define     _RFECDef_Item_WCDMATest_EndTest             "WT99"

// Test Item - CDMA Calibration
#define     _RFECDef_Item_CDMACal_InitCal               "CK00"
#define     _RFECDef_Item_CDMACal_TXLin                 "CK01"
#define     _RFECDef_Item_CDMACal_TxRxFreqComp          "CK02"
#define     _RFECDef_Item_CDMACal_RxIntelliceiverCal    "CK03"
#define     _RFECDef_Item_CDMACal_XOCoarseCal           "CK04"
#define     _RFECDef_Item_CDMACal_XOFineCal             "CK05"
#define     _RFECDef_Item_CDMACal_EndCal                "CK99"

// Test Item - CDMA Test
#define     _RFECDef_Item_CDMATest_InitTest             "CT00"
#define     _RFECDef_Item_CDMATest_StartCall            "CT01"
#define     _RFECDef_Item_CDMATest_WaveformQuality      "CT02"
#define     _RFECDef_Item_CDMATest_FreqError            "CT03"
#define     _RFECDef_Item_CDMATest_TimeError            "CT04"
#define     _RFECDef_Item_CDMATest_CarrierFeedthrough   "CT05"
#define     _RFECDef_Item_CDMATest_PhaseError           "CT06"
#define     _RFECDef_Item_CDMATest_MagnitudeError       "CT07"
#define     _RFECDef_Item_CDMATest_EVM                  "CT08"
#define     _RFECDef_Item_CDMATest_TxCurrent            "CT09"
#define     _RFECDef_Item_CDMATest_CloseLoopPowerCtrl   "CT10"
#define     _RFECDef_Item_CDMATest_CDPower              "CT11"
#define     _RFECDef_Item_CDMATest_TimeTolerance        "CT12"
#define     _RFECDef_Item_CDMATest_PhaseTolerance       "CT13"
#define     _RFECDef_Item_CDMATest_MinPower             "CT14"
#define     _RFECDef_Item_CDMATest_MaxPower             "CT15"
#define     _RFECDef_Item_CDMATest_RxSensitivity        "CT16"
#define     _RFECDef_Item_CDMATest_RxDynamicRange       "CT17"
#define     _RFECDef_Item_CDMATest_CSE_n2               "CT18"
#define     _RFECDef_Item_CDMATest_CSE_2                "CT19"
#define     _RFECDef_Item_CDMATest_CSE_n4               "CT20"
#define     _RFECDef_Item_CDMATest_CSE_4                "CT21"
#define     _RFECDef_Item_CDMATest_Handover             "CT98"
#define     _RFECDef_Item_CDMATest_EndTest              "CT99"

// Test Item - EVDO Test
#define     _RFECDef_Item_EVDOTest_InitTest             "VT00"
#define     _RFECDef_Item_EVDOTest_StartCall            "VT01"
#define     _RFECDef_Item_EVDOTest_WaveformQuality      "VT02"
#define     _RFECDef_Item_EVDOTest_FreqError            "VT03"
#define     _RFECDef_Item_EVDOTest_TimeError            "VT04"
#define     _RFECDef_Item_EVDOTest_CarrierFeedthrough   "VT05"
#define     _RFECDef_Item_EVDOTest_PhaseError           "VT06"
#define     _RFECDef_Item_EVDOTest_MagnitudeError       "VT07"
#define     _RFECDef_Item_EVDOTest_EVM                  "VT08"
#define     _RFECDef_Item_EVDOTest_TxCurrent            "VT09"
#define     _RFECDef_Item_EVDOTest_CloseLoopPowerCtrl   "VT10"
#define     _RFECDef_Item_EVDOTest_CDPower              "VT11"
#define     _RFECDef_Item_EVDOTest_TimeTolerance        "VT12"
#define     _RFECDef_Item_EVDOTest_PhaseTolerance       "VT13"
#define     _RFECDef_Item_EVDOTest_MinPower             "VT14"
#define     _RFECDef_Item_EVDOTest_MaxPower             "VT15"
#define     _RFECDef_Item_EVDOTest_RxSensitivity        "VT16"
#define     _RFECDef_Item_EVDOTest_RxDynamicRange       "VT17"
#define     _RFECDef_Item_EVDOTest_CSE                  "VT18"
#define     _RFECDef_Item_EVDOTest_Handover             "VT98"
#define     _RFECDef_Item_EVDOTest_EndTest              "VT99"

// Test Item - LTE Calibration
#define     _RFECDef_Item_LTECal_InitCal                "LK00"
#define     _RFECDef_Item_LTECal_TXLin                  "LK03"
#define     _RFECDef_Item_LTECal_TxRxFreqComp           "LK04"
#define     _RFECDef_Item_LTECal_DRxCal                 "LK10"
#define     _RFECDef_Item_LTECal_EndCal                 "LK99"

// Test Item - LTE Test
#define     _RFECDef_Item_LTETest_InitTest              "LT00"
#define     _RFECDef_Item_LTETest_StartCall             "LT01"
#define     _RFECDef_Item_LTETest_MaxPower              "LT02"
#define     _RFECDef_Item_LTETest_MinPower              "LT03"
#define     _RFECDef_Item_LTETest_FreqErr               "LT04"
#define     _RFECDef_Item_LTETest_IQoffset              "LT05"
#define     _RFECDef_Item_LTETest_EVM                   "LT06"
#define     _RFECDef_Item_LTETest_SEM                   "LT07"
#define     _RFECDef_Item_LTETest_OBW                   "LT08"
#define     _RFECDef_Item_LTETest_ACLR                  "LT09"
#define     _RFECDef_Item_LTETest_BLER                  "LT10"
#define     _RFECDef_Item_LTETest_RxLevel               "LT11"
#define     _RFECDef_Item_LTETest_RxSensitivity         "LT12"
#define     _RFECDef_Item_LTETest_NaxInputLevel         "LT13"
#define     _RFECDef_Item_LTETest_IEM                   "LT14"
#define     _RFECDef_Item_LTETest_ESFL                  "LT15"
#define     _RFECDef_Item_LTETest_TransPower            "LT16"
#define     _RFECDef_Item_LTETest_DRx_BLER              "LT30"
#define     _RFECDef_Item_LTETest_DRx_RxSensitivity     "LT31"
#define     _RFECDef_Item_LTETest_DRx_MaxInputLevel     "LT32"
#define     _RFECDef_Item_LTETest_DRx_MinRxSens         "LT33"
#define     _RFECDef_Item_LTETest_Handover              "LT98"
#define     _RFECDef_Item_LTETest_EndTest               "LT99"


//////////////////////////////////////////////////////////////////////////
//  RF Error Code Generator related functions
//////////////////////////////////////////////////////////////////////////
bool GenRFErrorCode(std::string strTech, int iBand, std::string strChannelRange, std::string strMode, std::string strItem, std::string strBasicErrCode, std::string& strErrorCode, std::string& strErrMsg) ;

bool RFErrorCode_CheckTech(std::string strTech) ;
bool RFErrorCode_GetBand(std::string strTech, int iBand, std::string &strBand) ;
bool RFErrorCode_GheckChannelRange(std::string& strChannelRange) ;
bool RFErrorCode_CheckModeAndItem(std::string strTech, std::string& strMode, std::string& strItem, bool bIsDeviceMode) ;


#endif // End of #ifndef _RF_ERROR_CODE_GENERATOR_H_
