#include <vector>
#include "..\CommonUtility\BaseObject\IPhone.h"

#define			WCDMA_BAND_TYPE_NUM					5
#define			NV_WCDMA_TX_LIN_MASTER_ENH_SIZ		32
#define			NV_WCDMA_TX_LIN_MASTER_SIZ			32
#define			NV_WCDMA_EXP_HDET_VS_AGC_SIZ		16
#define			NV_WCDMA_FREQ_TABLE_SIZ				16
#define			NV_WCDMA_LIM_VS_FREQ_SIZ			16

#define GSM_850_BAND_ID 850
#define GSM_EGSM_BAND_ID 900
#define GSM_DCS_BAND_ID 1800
#define GSM_PCS_BAND_ID 1900

#define GSM_BAND_TYPE_NUM					4

//Carrier Suppression
#define NV_GSM_DCS_CARRIER_SUPPRESSION					4871
#define NV_GSM_1900_CARRIER_SUPPRESSION					4872

//for polar nv item emu
#define NV_GSM_AMAM_MASTER_TBL_SEG1_F1_I                2521
#define NV_DCS_AMAM_MASTER_TBL_SEG1_F1_I                2529
#define NV_GSM_850_AMAM_MASTER_TBL_SEG1__F1_I			2537
#define NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F1_I           2545

#define NV_GSM_AMPM_MASTER_TBL_SEG1_F1_I                2585
#define NV_DCS_AMPM_MASTER_TBL_SEG1_F1_I                2593
#define NV_GSM_850_AMPM_MASTER_TBL_SEG1__F1_I			2601
#define NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F1_I           2609

#define NV_GSM_AMAM_MASTER_TBL_SEG1_F2_I                2553
#define NV_DCS_AMAM_MASTER_TBL_SEG1_F2_I                2561
#define NV_GSM_850_AMAM_MASTER_TBL_SEG1__F2_I			2569
#define NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F2_I           2577

#define NV_GSM_AMPM_MASTER_TBL_SEG1_F2_I                2617
#define NV_DCS_AMPM_MASTER_TBL_SEG1_F2_I                2625
#define NV_GSM_850_AMPM_MASTER_TBL_SEG1__F2_I			2633
#define NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F2_I           2641
//added by strong for supporting F3 
#define NV_GSM_AMAM_MASTER_TBL_SEG1_F3_I                3078
#define NV_DCS_AMAM_MASTER_TBL_SEG1_F3_I                3086
#define NV_GSM_850_AMAM_MASTER_TBL_SEG1__F3_I           3094
#define NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F3_I           3102

#define NV_GSM_AMPM_MASTER_TBL_SEG1_F3_I                3110
#define NV_DCS_AMPM_MASTER_TBL_SEG1_F3_I                3118
#define NV_GSM_850_AMPM_MASTER_TBL_SEG1__F3_I           3126
#define NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F3_I           3134

//Timing delay
#define NV_GSM_POLAR_PATH_DELAY_I                       2770
#define NV_850_POLAR_PATH_DELAY_I						2772
#define NV_DCS_POLAR_PATH_DELAY_I                       2771
#define NV_GSM_1900_POLAR_PATH_DELAY_I                  2773

#define NV_GSM_AMAM_DYNAMIC_RANGE		                2743
#define NV_DCS_AMAM_DYNAMIC_RANGE				        2745
#define NV_GSM_850_AMAM_DYNAMIC_RANGE				    2747
#define NV_GSM_1900_AMAM_DYNAMIC_RANGE			        2749

/* AMAM Master Table Size */
#define RF_AMAM_MASTER_TBL_SIZ 512 

/* AMPM Master Table Size */
#define RF_AMPM_MASTER_TBL_SIZ 256 
#define NV_DCS_CAL_ARFCN_SIZ   16

typedef unsigned short      UINT16, *PUINT16;
typedef signed char         INT8, *PINT8;


class CQualComm_RFNVItems
{
public:
	CQualComm_RFNVItems();
	~CQualComm_RFNVItems();
	
	void	SetAPT_Caled(bool bActed);
	bool	HasAPTCaled();
	bool	Initiate();

private:
	bool	m_bAPTActed;
	//RaphKan
	UINT16	m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_ENH_SIZ];
    UINT16	m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];	
	UINT16	m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT16	m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT16	m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];

	UINT16	m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_ENH_SIZ];
	UINT16  m_uiNV_WCDMA_TX_PDM_0_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT16	m_uiNV_WCDMA_TX_PDM_1_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT16	m_uiNV_WCDMA_TX_PDM_2_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT16	m_uiNV_WCDMA_TX_PDM_3_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	UINT8	m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[WCDMA_BAND_TYPE_NUM][NV_WCDMA_EXP_HDET_VS_AGC_SIZ];

	INT8	m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];

	UINT8	m_uiNV_WCDMA_HDET_OFF_I_NEW[WCDMA_BAND_TYPE_NUM];
	UINT8	m_uiNV_WCDMA_HDET_SPN_I_NEW[WCDMA_BAND_TYPE_NUM];
	UINT16	m_uiHDET[WCDMA_BAND_TYPE_NUM][NV_WCDMA_TX_LIN_MASTER_SIZ];
	INT16	m_iNV_WCDMA_LIM_VS_FREQ_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT16	m_iNV_WCDMA_VGA_GAIN_OFFSET_I[WCDMA_BAND_TYPE_NUM];
	INT8	m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];

	UINT16	m_uiNV_WCDMA_LNA_OFFSET_I[WCDMA_BAND_TYPE_NUM];
	UINT16	m_uiNV_WCDMA_LNA_OFFSET_2_I[WCDMA_BAND_TYPE_NUM];
	UINT16	m_uiNV_WCDMA_LNA_OFFSET_3_I[WCDMA_BAND_TYPE_NUM];
	UINT16	m_uiNV_WCDMA_LNA_OFFSET_4_I[WCDMA_BAND_TYPE_NUM];

	INT8	m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ];
	INT8	m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[WCDMA_BAND_TYPE_NUM][NV_WCDMA_FREQ_TABLE_SIZ]; 
	//Therm cal, first is max
	UINT8	m_uiNV_THERM_MAX_MIN[2];

	// IM2
	INT8	m_iNV_WCDMA_IM2_I_VALUE_I[WCDMA_BAND_TYPE_NUM] ;
	INT8	m_iNV_WCDMA_IM2_Q_VALUE_I[WCDMA_BAND_TYPE_NUM] ;
	INT8	m_iNV_WCDMA_IM2_GM_VALUE_I[WCDMA_BAND_TYPE_NUM] ;

	//Carrier Suppression
	INT16 m_uiNV_IDC_QDC[2][2];

	//Tx Polar NV Items
	UINT16 m_uiNV_GSM_AMAM_MASTER_TBL_F1[GSM_BAND_TYPE_NUM][RF_AMAM_MASTER_TBL_SIZ]; 
	UINT16 m_uiNV_GSM_AMAM_MASTER_TBL_F2[GSM_BAND_TYPE_NUM][RF_AMAM_MASTER_TBL_SIZ]; 
	UINT16 m_uiNV_GSM_AMAM_MASTER_TBL_F3[GSM_BAND_TYPE_NUM][RF_AMAM_MASTER_TBL_SIZ];

	UINT32 m_iNV_GSM_AMPM_MASTER_TBL_F1[GSM_BAND_TYPE_NUM][RF_AMPM_MASTER_TBL_SIZ];
	UINT32 m_iNV_GSM_AMPM_MASTER_TBL_F2[GSM_BAND_TYPE_NUM][RF_AMPM_MASTER_TBL_SIZ];
	UINT32 m_iNV_GSM_AMPM_MASTER_TBL_F3[GSM_BAND_TYPE_NUM][RF_AMPM_MASTER_TBL_SIZ];

	//Timing Delay search
	UINT16 m_uiNV_GSM_POLAR_PATH_DELAY[GSM_BAND_TYPE_NUM];

	INT32 m_iNV_DYNAMIC_RANGE[GSM_BAND_TYPE_NUM][2];

	//PIPhone m_pcPhone;

public:
	bool	Set_NV_WCDMA_TX_LIN_MASTER_0_ENH_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_LIN_MASTER_0_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_LIN_MASTER_1_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_LIN_MASTER_2_I_NEW(PBYTE pByte,size_t stBand,size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_LIN_MASTER_3_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	
	bool	Set_NV_WCDMA_TX_PDM_0_ENH_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_PDM_0_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_PDM_1_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_PDM_2_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_TX_PDM_3_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);

	bool	Set_NV_WCDMA_EXP_HDET_VS_AGC_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);
	bool	Set_NV_WCDMA_HDET_OFF_I_NEW(PBYTE pByte,size_t stBand ,size_t stSize);
	bool	Set_NV_WCDMA_HDET_SPN_I_NEW(PBYTE pByte, size_t stBand,size_t stSize);

	bool	Get_NV_WCDMA_TX_LIN_MASTER_0_ENH_I_NEW(PBYTE pByte, size_t stBand, size_t stStartIndex, size_t& stLen);
	bool	Get_NV_WCDMA_TX_LIN_MASTER_0_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_LIN_MASTER_1_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_LIN_MASTER_2_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_LIN_MASTER_3_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);

	bool	Get_NV_WCDMA_TX_PDM_0_ENH_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_PDM_0_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_PDM_1_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_PDM_2_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Get_NV_WCDMA_TX_PDM_3_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);

	bool	Get_NV_WCDMA_EXP_HDET_VS_AGC_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Get_NV_WCDMA_HDET_OFF_I_NEW(PBYTE pByte,size_t stBand,size_t& stSize);
	bool	Get_NV_WCDMA_HDET_SPN_I_NEW(PBYTE pByte, size_t stBand,size_t& stSize);

	bool	Get_NV_WCDMA_TX_COMP_VS_FREQ_0_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_TX_COMP_VS_FREQ_0_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_TX_COMP_VS_FREQ_1_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_TX_COMP_VS_FREQ_1_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_TX_COMP_VS_FREQ_2_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_TX_COMP_VS_FREQ_2_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_TX_COMP_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_TX_COMP_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_HDET(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen);
	bool	Set_HDET(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_LIM_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_LIM_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_VGA_GAIN_OFFSET_I(PBYTE pByte, size_t stBand, size_t& stSize);
	bool	Set_NV_WCDMA_VGA_GAIN_OFFSET_I(PBYTE pByte, size_t stBand, size_t stSize);

	bool	Get_NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex,  size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_I(PBYTE pByte, size_t stBand, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_I(PBYTE pByte, size_t stBand, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_2_I(PBYTE pByte, size_t stBand, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_2_I(PBYTE pByte, size_t stBand, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_3_I(PBYTE pByte, size_t stBand, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_3_I(PBYTE pByte, size_t stBand, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_4_I(PBYTE pByte, size_t stBand, size_t& stSize); 
	bool	Set_NV_WCDMA_LNA_OFFSET_4_I(PBYTE pByte, size_t stBand, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I(PBYTE pByte, size_t stBand,size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I(PBYTE pByte, size_t stBand,size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize); 
	bool	Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize);

	bool	Get_NV_THERM_MAX(PBYTE pByte, size_t& stSize);
	bool	Set_NV_THERM_MAX(PBYTE pByte, size_t stSize);
	bool	Get_NV_THERM_MIN(PBYTE pByte, size_t& stSize);
	bool	Set_NV_THERM_MIN(PBYTE pByte, size_t stSize);

	bool	Get_NV_WCDMA_IM2_I_VALUE_I (PBYTE pByte, int stBand, size_t& stSize) ;
	bool	Set_NV_WCDMA_IM2_I_VALUE_I (PBYTE pByte, int stBand, size_t stSize) ;
	bool	Get_NV_WCDMA_IM2_Q_VALUE_I (PBYTE pByte, int stBand, size_t& stSize) ;
	bool	Set_NV_WCDMA_IM2_Q_VALUE_I (PBYTE pByte, int stBand, size_t stSize) ;
	bool	Get_NV_WCDMA_IM2_GM_VALUE_I (PBYTE pByte, int stBand, size_t& stSize) ;
	bool	Set_NV_WCDMA_IM2_GM_VALUE_I (PBYTE pByte, int stBand, size_t stSize) ;

	bool	Get_NV_GSM_CARRIER_SUPPRESSION_Idc_Qdc(PBYTE pByte, int stBand, size_t& stSize);

	//bool	Get_NV_GSM_CARRIER_SUPPRESSION_Qdc(PBYTE pByte, int stBand, size_t& stSize);
	bool	Set_NV_GSM_CARRIER_SUPPRESSION_Qdc(PBYTE pByte, int stBand, size_t stSize);

	//bool	Get_NV_GSM_CARRIER_SUPPRESSION_Idc(PBYTE pByte, int stBand, size_t& stSize);
	bool	Set_NV_GSM_CARRIER_SUPPRESSION_Idc(PBYTE pByte, int stBand, size_t stSize);

	//GSM Tx Polar NV Items
	bool	Get_NV_GSM_AMAM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMAM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t stSize);
	bool	Get_NV_GSM_AMAM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMAM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t stSize); 
	bool	Get_NV_GSM_AMAM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMAM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t stSize); 

	bool	Get_NV_GSM_AMPM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMPM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t stSize); 
	bool	Get_NV_GSM_AMPM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMPM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t stSize); 
	bool	Get_NV_GSM_AMPM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize);
	bool	Set_NV_GSM_AMPM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t stSize); 

	bool	Get_GSM_NV_POLAR_PATH_DELAY_I(PBYTE pByte, int stBand, size_t& stSize);
	bool	Set_GSM_NV_POLAR_PATH_DELAY_I(PBYTE pByte, int stBand, size_t stSize);

	bool	Get_NV_GSM_DYNAMIC_RANGE(PBYTE pByte, int stBand, size_t& stSize);
	bool	Set_NV_GSM_DYNAMIC_RANGE_MAX(PBYTE pByte, int stBand, size_t stSize);
	bool	Set_NV_GSM_DYNAMIC_RANGE_MIN(PBYTE pByte, int stBand, size_t stSize);

	/*
	bool	Write_NV_GSM_TX_V2(std::vector<int> vChannel,int iBandID);

	bool	Write_NV_GSM_AMAM_MASTER_TBL_F1(int iBandID);
	bool	Write_NV_GSM_AMAM_MASTER_TBL_F2(int iBandID);
	bool	Write_NV_GSM_AMAM_MASTER_TBL_F3(int iBandID);
	bool	Write_NV_GSM_AMPM_MASTER_TBL_F1(int iBandID);
	bool	Write_NV_GSM_AMPM_MASTER_TBL_F2(int iBandID);
	bool	Write_NV_GSM_AMPM_MASTER_TBL_F3(int iBandID);

	bool	Write_NV_GSM_CARRIER_SUPPRESSION_Idc_Qdc(int iBandID);
	bool	Write_GSM_NV_POLAR_PATH_DELAY_I(int iBandID);
	*/

};
