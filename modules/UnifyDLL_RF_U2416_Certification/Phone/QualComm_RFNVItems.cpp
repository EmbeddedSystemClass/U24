#include "stdafx.h"
#include "QualComm_RFNVItems.h"


CQualComm_RFNVItems::CQualComm_RFNVItems()
{
	m_bAPTActed=true;
}

CQualComm_RFNVItems::~CQualComm_RFNVItems(){}

void CQualComm_RFNVItems::SetAPT_Caled(bool bActed)
{
	m_bAPTActed=bActed;
}

bool CQualComm_RFNVItems::HasAPTCaled()
{
	return	m_bAPTActed ;
}

bool CQualComm_RFNVItems::Initiate()
{
	memset(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW));
    memset(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW));
	memset(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW));
	memset(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW));
	memset(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW));
	
	memset(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW));
	memset(m_uiNV_WCDMA_TX_PDM_0_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW));
	memset(m_uiNV_WCDMA_TX_PDM_1_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW));
	memset(m_uiNV_WCDMA_TX_PDM_2_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW));
	memset(m_uiNV_WCDMA_TX_PDM_3_I_NEW,0,sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW));
	
	memset(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW,0,sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW));
	
	memset(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I,0,sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I));
	memset(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I,0,sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I));
	memset(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I,0,sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I));
	memset(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I,0,sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I));
	
	memset(m_uiNV_WCDMA_HDET_OFF_I_NEW,0,sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW));
	memset(m_uiNV_WCDMA_HDET_SPN_I_NEW,0,sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW));
	
	memset(m_uiHDET,0,sizeof(m_uiHDET));
	memset(m_iNV_WCDMA_LIM_VS_FREQ_I,0,sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I));
	
	memset(m_iNV_WCDMA_VGA_GAIN_OFFSET_I,0,sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I));
	memset(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I,0,sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I));
	
	memset(m_uiNV_WCDMA_LNA_OFFSET_I,0,sizeof(m_uiNV_WCDMA_LNA_OFFSET_I));
	memset(m_uiNV_WCDMA_LNA_OFFSET_2_I,0,sizeof(m_uiNV_WCDMA_LNA_OFFSET_2_I));
	memset(m_uiNV_WCDMA_LNA_OFFSET_3_I,0,sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I));
	memset(m_uiNV_WCDMA_LNA_OFFSET_4_I,0,sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I));
	
	memset(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I,0,sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I));
	memset(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I,0,sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I));
	memset(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I,0,sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I));
	memset(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I,0,sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I));
	memset(m_uiNV_THERM_MAX_MIN,0,sizeof(m_uiNV_THERM_MAX_MIN));

	//Tx Polar NV Items
	memset(&m_uiNV_GSM_AMAM_MASTER_TBL_F1, 0, sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F1));
	memset(&m_uiNV_GSM_AMAM_MASTER_TBL_F2, 0, sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F2));
	memset(&m_uiNV_GSM_AMAM_MASTER_TBL_F3, 0, sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F3));

	memset(&m_iNV_GSM_AMPM_MASTER_TBL_F1, 0, sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F1));
	memset(&m_iNV_GSM_AMPM_MASTER_TBL_F2, 0, sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F2));
	memset(&m_iNV_GSM_AMPM_MASTER_TBL_F3, 0, sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F3));

	//Timing Delay search
	memset(&m_uiNV_GSM_POLAR_PATH_DELAY, 0, sizeof(m_uiNV_GSM_POLAR_PATH_DELAY));

	//Tx CS NV Items
	memset(&m_uiNV_IDC_QDC, 0, sizeof(m_uiNV_IDC_QDC));

	// IM2
	memset(&m_iNV_WCDMA_IM2_I_VALUE_I,	0, sizeof(m_iNV_WCDMA_IM2_I_VALUE_I) ) ;
	memset(&m_iNV_WCDMA_IM2_Q_VALUE_I,	0, sizeof(m_iNV_WCDMA_IM2_Q_VALUE_I) ) ;
	memset(&m_iNV_WCDMA_IM2_GM_VALUE_I, 0, sizeof(m_iNV_WCDMA_IM2_GM_VALUE_I)) ;

	return true;
}



bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_LIN_MASTER_0_ENH_I_NEW(PBYTE pByte, size_t stBand, size_t stStartIndex, size_t& stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_LIN_MASTER_0_ENH_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_ENH_0_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_LIN_MASTER_0_ENH_I_NEW(int iBandID)
{
	DWORD dwNVItemID_LIN_MASTER = 0;
	WORD wStatus = 0;
	int nBandIndex=0;
	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_TX_LIN_MASTER_0_ENH_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_1900_TX_LIN_MASTER_0_ENH_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_BC4_TX_LIN_MASTER_0_ENH_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_800_TX_LIN_MASTER_0_ENH_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_900_TX_LIN_MASTER_0_ENH_I;
		nBandIndex=3;
		break;
	default:
		return false;
	}

	return true;
}
*/

bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_LIN_MASTER_0_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][0]));
	return true;
}

bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_LIN_MASTER_0_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][0])*stLen;

	memcpy(pByte, &m_uiNV_WCDMA_TX_LIN_MASTER_0_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_LIN_MASTER_0_I_NEW(int iBandID)
{
	DWORD dwNVItemID_LIN_MASTER = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_TX_LIN_MASTER_0_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_1900_TX_LIN_MASTER_0_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_BC4_TX_LIN_MASTER_0_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_800_TX_LIN_MASTER_0_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_900_TX_LIN_MASTER_0_I;
		nBandIndex=3;
		break;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_LIN_MASTER_1_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][0]));
	return true;
}

bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_LIN_MASTER_1_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][0])*stLen;

	memcpy(pByte, &m_uiNV_WCDMA_TX_LIN_MASTER_1_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_LIN_MASTER_1_I_NEW(int iBandID)
{
	DWORD dwNVItemID_LIN_MASTER = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_TX_LIN_MASTER_1_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_1900_TX_LIN_MASTER_1_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_BC4_TX_LIN_MASTER_1_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_800_TX_LIN_MASTER_1_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_900_TX_LIN_MASTER_1_I;
		nBandIndex=3;
		break;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_LIN_MASTER_2_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][0])*stLen;

	memcpy(pByte, &m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_LIN_MASTER_2_I_NEW(PBYTE pByte,size_t stBand,size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_2_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_LIN_MASTER_2_I_NEW(int iBandID)
{
	DWORD dwNVItemID_LIN_MASTER = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_TX_LIN_MASTER_2_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_1900_TX_LIN_MASTER_2_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_BC4_TX_LIN_MASTER_2_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_800_TX_LIN_MASTER_2_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_900_TX_LIN_MASTER_2_I;
		nBandIndex=3;
		break;
	}
	return true;
}

*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_LIN_MASTER_3_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][stStartIndex], stSize);
	return true;
} 
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_LIN_MASTER_3_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_LIN_MASTER_3_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_LIN_MASTER_3_I_NEW(int iBandID)
{
	DWORD dwNVItemID_LIN_MASTER = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_TX_LIN_MASTER_3_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_1900_TX_LIN_MASTER_3_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_BC4_TX_LIN_MASTER_3_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_800_TX_LIN_MASTER_3_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIN_MASTER = NV_WCDMA_900_TX_LIN_MASTER_3_I;
		nBandIndex=3;
		break;
	}
	
	return true;
}

*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_PDM_0_ENH_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_PDM_0_ENH_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
    stSize=stLen*sizeof(m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][0]);
	memcpy(&m_uiNV_WCDMA_TX_PDM_ENH_0_I_NEW[stBand][stIndex], pByte, stSize);
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_PDM_0_ENH_I_NEW(int iBandID)
{
	DWORD dwNVItemID_TX_PDM = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_TX_PDM_LIN_0_ENH_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_1900_TX_PDM_LIN_0_ENH_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_BC4_TX_PDM_LIN_0_ENH_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_800_TX_PDM_LIN_0_ENH_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_900_TX_PDM_LIN_0_ENH_I;
		nBandIndex=3;
		break;
	}

	return true;
}

*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_PDM_0_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_PDM_0_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_PDM_0_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_PDM_0_I_NEW(int iBandID)
{
	DWORD dwNVItemID_TX_PDM = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_TX_PDM_LIN_1_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_1900_TX_PDM_LIN_1_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_BC4_TX_PDM_LIN_1_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_800_TX_PDM_LIN_1_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_900_TX_PDM_LIN_1_I;
		nBandIndex=3;
		break;

	}
	
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_PDM_1_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_PDM_1_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_PDM_1_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_PDM_1_I_NEW(int iBandID)
{
	DWORD dwNVItemID_TX_PDM = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_TX_PDM_LIN_1_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_1900_TX_PDM_LIN_1_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_BC4_TX_PDM_LIN_1_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_800_TX_PDM_LIN_1_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_900_TX_PDM_LIN_1_I;
		nBandIndex=3;
		break;

	}
	
	return true;
}

*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_PDM_2_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_PDM_2_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_PDM_2_I_NEW[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_PDM_2_I_NEW(int iBandID)
{
	DWORD dwNVItemID_TX_PDM = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_TX_PDM_LIN_2_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_1900_TX_PDM_LIN_2_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_BC4_TX_PDM_LIN_2_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_800_TX_PDM_LIN_2_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_PDM = NV_WCDMA_900_TX_PDM_LIN_2_I;
		nBandIndex=3;
		break;
	}
	return true;
}

*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_PDM_3_I_NEW(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_PDM_3_I_NEW(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_TX_PDM_3_I_NEW[stBand][0]));
	return true;
}

bool CQualComm_RFNVItems::Get_NV_WCDMA_EXP_HDET_VS_AGC_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][0])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_EXP_HDET_VS_AGC_I_NEW(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][stIndex], pByte, stLen*sizeof(m_uiNV_WCDMA_EXP_HDET_VS_AGC_I_NEW[stBand][0]));
	return true;
}

/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_EXP_HDET_VS_AGC_V2_I_NEW(int iBandID)
{
	DWORD dwNVItemID_HDET_AGC = 0;
	WORD wStatus = 0;
	int nBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_HDET_AGC = NV_WCDMA_EXP_HDET_VS_AGC_V2_I;
		nBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_HDET_AGC = NV_WCDMA_1900_EXP_HDET_VS_AGC_V2_I;
		nBandIndex=1;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_HDET_AGC = NV_WCDMA_BC4_EXP_HDET_VS_AGC_V2_I;
		nBandIndex=2;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_HDET_AGC = NV_WCDMA_800_EXP_HDET_VS_AGC_V2_I;
		nBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_HDET_AGC = NV_WCDMA_900_EXP_HDET_VS_AGC_V2_I;
		nBandIndex=3;
		break;
	}

	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_HDET_OFF_I_NEW(PBYTE pByte,size_t stBand,size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;
	stSize=stLen*sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand]);
	memcpy(pByte, &m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_HDET_OFF_I_NEW(PBYTE pByte,size_t stBand ,size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand]);
	if(stSize>stArrayLen)
		stSize=stArrayLen;

	memcpy(&(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand]), pByte, stSize*sizeof(m_uiNV_WCDMA_HDET_OFF_I_NEW[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_HDET_OFF_V2_I_NEW(int iBandID)
{
	DWORD dwNV_WCDMA_HDET_OFF = 0;
	WORD wStatus = 0;
	size_t stBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		//dwNV_WCDMA_HDET_OFF= NV_WCDMA_HDET_OFF_I;
		dwNV_WCDMA_HDET_OFF= NV_WCDMA_HDET_OFF_V2_I;
		stBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_HDET_OFF = NV_WCDMA_1900_HDET_OFF_V2_I;
		stBandIndex=1;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_HDET_OFF = NV_WCDMA_800_HDET_OFF_V2_I;
		stBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_HDET_OFF = NV_WCDMA_900_HDET_OFF_V2_I;
		stBandIndex=3;
		break;
	}
	return true;
}
*/


bool CQualComm_RFNVItems::Get_NV_WCDMA_HDET_SPN_I_NEW(PBYTE pByte, size_t stBand,size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;
	stSize=stLen*sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand]);
	memcpy(pByte, &m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_HDET_SPN_I_NEW(PBYTE pByte, size_t stBand,size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand])/sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand]);
	if(stSize>stArrayLen)
		stSize=stArrayLen;

	memcpy(&(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand]), pByte, stSize*sizeof(m_uiNV_WCDMA_HDET_SPN_I_NEW[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_HDET_SPN_V2_I_NEW(int iBandID)
{
	DWORD dwNV_WCDMA_HDET_SPN = 0;
	WORD wStatus = 0;
	size_t stBandIndex=0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		//dwNV_WCDMA_HDET_SPN= NV_WCDMA_HDET_SPN_I;
		dwNV_WCDMA_HDET_SPN= NV_WCDMA_HDET_SPN_V2_I;
		stBandIndex=0;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_HDET_SPN = NV_WCDMA_1900_HDET_SPN_V2_I;
		stBandIndex=1;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_HDET_SPN = NV_WCDMA_800_HDET_SPN_V2_I;
		stBandIndex=4;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_HDET_SPN = NV_WCDMA_900_HDET_SPN_V2_I;
		stBandIndex=3;
		break;
	}
	return true;
}
*/


bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_COMP_VS_FREQ_0_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][stIndex], stSize);
	return true;

}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_COMP_VS_FREQ_0_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I[stBand][0]));
	return true;

}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_COMP_VS_FREQ_0_I(int iBandID)
{
	DWORD dwNVItemID_TX_COMP= 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_TX_COMP_VS_FREQ_0_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_1900_TX_COMP_VS_FREQ_0_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_BC4_TX_COMP_VS_FREQ_0_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_800_TX_COMP_VS_FREQ_0_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_900_TX_COMP_VS_FREQ_0_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_TX_COMP, (unsigned char* )&m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I, sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_0_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_COMP_VS_FREQ_1_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_COMP_VS_FREQ_1_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I[stBand][0]));
	return true;

}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_COMP_VS_FREQ_1_I(int iBandID)
{
	DWORD dwNVItemID_TX_COMP = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_TX_COMP_VS_FREQ_1_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_1900_TX_COMP_VS_FREQ_1_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_BC4_TX_COMP_VS_FREQ_1_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_800_TX_COMP_VS_FREQ_1_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_900_TX_COMP_VS_FREQ_1_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_TX_COMP, (unsigned char* )&m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I, sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_1_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_COMP_VS_FREQ_2_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_COMP_VS_FREQ_2_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I[stBand][0]));
	return true;

}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_COMP_VS_FREQ_2_I(int iBandID)
{
	DWORD dwNVItemID_TX_COMP = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_TX_COMP_VS_FREQ_2_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_1900_TX_COMP_VS_FREQ_2_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_BC4_TX_COMP_VS_FREQ_2_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_800_TX_COMP_VS_FREQ_2_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_900_TX_COMP_VS_FREQ_2_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_TX_COMP, (unsigned char* )&m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I, sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_2_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_TX_COMP_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_TX_COMP_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand])/sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_TX_COMP_VS_FREQ_3_I(int iBandID)
{
	DWORD dwNVItemID_TX_COMP = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_TX_COMP_VS_FREQ_3_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_1900_TX_COMP_VS_FREQ_3_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_BC4_TX_COMP_VS_FREQ_3_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_800_TX_COMP_VS_FREQ_3_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_TX_COMP = NV_WCDMA_900_TX_COMP_VS_FREQ_3_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_TX_COMP, (unsigned char* )&m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I, sizeof(m_iNV_WCDMA_TX_COMP_VS_FREQ_3_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_HDET(PBYTE pByte,size_t stBand, size_t stStartIndex, size_t stLen)
{
	size_t stArrayLen=sizeof(m_uiHDET[stBand])/sizeof(m_uiHDET[stBand][0]);
	if(stLen>(stArrayLen-1)-stStartIndex+1)
		stLen=(stArrayLen-1)-stStartIndex+1;

	size_t stSize = sizeof(m_uiHDET[stBand][0])*stLen;

	memcpy(pByte, &m_uiHDET[stBand][stStartIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_HDET(PBYTE pByte,size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiHDET[stBand])/sizeof(m_uiHDET[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_uiHDET[stBand][stIndex], pByte, stLen*sizeof(m_uiHDET[stBand][0]));
	return true;
}


bool CQualComm_RFNVItems::Get_NV_WCDMA_LIM_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LIM_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I[stBand][0]));
	return true;

}
/*
bool CRFNVItems::Write_NV_WCDMA_LIM_VS_FREQ_I(int iBandID)
{
	DWORD dwNVItemID_LIM_FREQ = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LIM_FREQ = NV_WCDMA_TX_LIM_VS_FREQ_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LIM_FREQ = NV_WCDMA_1900_TX_LIM_VS_FREQ_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LIM_FREQ = NV_WCDMA_BC4_TX_LIM_VS_FREQ_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LIM_FREQ = NV_WCDMA_800_TX_LIM_VS_FREQ_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LIM_FREQ = NV_WCDMA_900_TX_LIM_VS_FREQ_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_LIM_FREQ, (unsigned char* )&m_iNV_WCDMA_LIM_VS_FREQ_I, sizeof(m_iNV_WCDMA_LIM_VS_FREQ_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/


bool CQualComm_RFNVItems::Get_NV_WCDMA_VGA_GAIN_OFFSET_I(PBYTE pByte, size_t stBand, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand])/sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	stSize = sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_VGA_GAIN_OFFSET_I(PBYTE pByte, size_t stBand, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand])/sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	memcpy(&m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand], pByte, stLen*sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I[stBand]));
	return true;
}
/*
bool CRFNVItems::Write_NV_WCDMA_VGA_GAIN_OFFSET_I(int iBandID)
{
	DWORD dwNVItemID_DVGA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_VGA_GAIN_OFFSET_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_1900_VGA_GAIN_OFFSET_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_BC4_VGA_GAIN_OFFSET_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_800_VGA_GAIN_OFFSET_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_900_VGA_GAIN_OFFSET_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_DVGA_Offset, (unsigned char* )&m_iNV_WCDMA_VGA_GAIN_OFFSET_I, sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	stSize = sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][0])*stLen;
	memcpy(pByte, &m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex,  size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I[stBand][0]));
	return true;
}
/*
bool CRFNVItems::Write_NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I(int iBandID)
{
	DWORD dwNVItemID_DVGA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_1900_VGA_GAIN_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_BC4_VGA_GAIN_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_800_VGA_GAIN_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_DVGA_Offset = NV_WCDMA_900_VGA_GAIN_OFFSET_VS_FREQ_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_DVGA_Offset, (unsigned char* )&m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I, sizeof(m_iNV_WCDMA_VGA_GAIN_OFFSET_VS_FREQ_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_I(PBYTE pByte, size_t stBand, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	stSize = sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_LNA_OFFSET_I[stBand], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_I(PBYTE pByte, size_t stBand, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;
	memcpy(&m_uiNV_WCDMA_LNA_OFFSET_I[stBand], pByte, stLen*sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_I(int iBandID)
{
	DWORD dwNV_WCDMA_LNA_OFFSET = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET= NV_WCDMA_LNA_RANGE_OFFSET_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_1900_LNA_RANGE_OFFSET_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_BC4_LNA_RANGE_OFFSET_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_800_LNA_RANGE_OFFSET_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_900_LNA_RANGE_OFFSET_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNV_WCDMA_LNA_OFFSET, (unsigned char* )&m_uiNV_WCDMA_LNA_OFFSET_I, sizeof(m_uiNV_WCDMA_LNA_OFFSET_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;

}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_2_I(PBYTE pByte, size_t stBand, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_2_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_2_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	stSize = sizeof(m_uiNV_WCDMA_LNA_OFFSET_2_I[stBand])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_LNA_OFFSET_2_I[stBand], stSize);
	return true;

}

bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_2_I(PBYTE pByte, size_t stBand, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;
	memcpy(&m_uiNV_WCDMA_LNA_OFFSET_2_I[stBand], pByte, stLen*sizeof(m_uiNV_WCDMA_LNA_OFFSET_I[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_2_I(int iBandID)
{
	DWORD dwNV_WCDMA_LNA_OFFSET = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET= NV_WCDMA_LNA_RANGE_OFFSET_2_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_1900_LNA_RANGE_OFFSET_2_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_BC4_LNA_RANGE_OFFSET_2_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_800_LNA_RANGE_OFFSET_2_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_900_LNA_RANGE_OFFSET_2_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNV_WCDMA_LNA_OFFSET, (unsigned char* )&m_uiNV_WCDMA_LNA_OFFSET_2_I, sizeof(m_uiNV_WCDMA_LNA_OFFSET_2_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_3_I(PBYTE pByte, size_t stBand, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	stSize = sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand])*stLen;
	memcpy(pByte, &m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_3_I(PBYTE pByte, size_t stBand, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	memcpy(&m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand], pByte, stLen*sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_3_I(int iBandID)
{
	DWORD dwNV_WCDMA_LNA_OFFSET = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET= NV_WCDMA_LNA_RANGE_OFFSET_3_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_1900_LNA_RANGE_OFFSET_3_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_BC4_LNA_RANGE_OFFSET_3_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_800_LNA_RANGE_OFFSET_3_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_900_LNA_RANGE_OFFSET_3_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNV_WCDMA_LNA_OFFSET, (unsigned char* )&m_uiNV_WCDMA_LNA_OFFSET_3_I, sizeof(m_uiNV_WCDMA_LNA_OFFSET_3_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_4_I(PBYTE pByte, size_t stBand, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;

	stSize = sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand])*stLen;

	memcpy(pByte, &m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_4_I(PBYTE pByte, size_t stBand, size_t stSize)
{
	size_t stArrayLen=sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand])/sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand]);
	size_t stLen=stSize;
	if(stLen>stArrayLen)
		stLen=stArrayLen;
	memcpy(&m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand], pByte, stLen*sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I[stBand]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_4_I(int iBandID)
{
	DWORD dwNV_WCDMA_LNA_OFFSET = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET= NV_WCDMA_LNA_RANGE_OFFSET_4_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_1900_LNA_RANGE_OFFSET_4_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_BC4_LNA_RANGE_OFFSET_4_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_800_LNA_RANGE_OFFSET_4_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNV_WCDMA_LNA_OFFSET = NV_WCDMA_900_LNA_RANGE_OFFSET_4_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNV_WCDMA_LNA_OFFSET, (unsigned char* )&m_uiNV_WCDMA_LNA_OFFSET_4_I, sizeof(m_uiNV_WCDMA_LNA_OFFSET_4_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	stSize=stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][0]);
	memcpy(pByte, &m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	memcpy(&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_VS_FREQ_I(int iBandID)
{
	DWORD dwNVItemID_LNA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_LNA_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_1900_LNA_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_BC4_LNA_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_800_LNA_OFFSET_VS_FREQ_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_900_LNA_OFFSET_VS_FREQ_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_LNA_Offset, (unsigned char* )&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I, sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I(PBYTE pByte, size_t stBand,size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	stSize=stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][0]);
	memcpy(pByte, &m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][stIndex], stSize);
	return true;
}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I(PBYTE pByte, size_t stBand,size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	memcpy(&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I(int iBandID)
{
	DWORD dwNVItemID_LNA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_LNA_OFFSET_VS_FREQ_2_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_1900_LNA_OFFSET_VS_FREQ_2_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_BC4_LNA_OFFSET_VS_FREQ_2_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_800_LNA_OFFSET_VS_FREQ_2_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_900_LNA_OFFSET_VS_FREQ_2_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_LNA_Offset, (unsigned char* )&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I, sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_2_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	stSize=stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][0]);
	memcpy(pByte, &m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][stIndex], stSize);
	return true;

}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I[stBand][0]));
	return true;
}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I(int iBandID)
{
	DWORD dwNVItemID_LNA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_LNA_OFFSET_VS_FREQ_3_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_1900_LNA_OFFSET_VS_FREQ_3_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_BC4_LNA_OFFSET_VS_FREQ_3_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_800_LNA_OFFSET_VS_FREQ_3_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_900_LNA_OFFSET_VS_FREQ_3_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_LNA_Offset, (unsigned char* )&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I, sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_3_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/
bool CQualComm_RFNVItems::Get_NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t& stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;
	stSize=stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][0]);
	memcpy(pByte, &m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][stIndex], stSize);
	return true;

}
bool CQualComm_RFNVItems::Set_NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I(PBYTE pByte, size_t stBand, size_t stIndex, size_t stSize)
{
	size_t stArrayLen=sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand])/sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][0]);
	size_t stLen=stSize;
	if(stLen>(stArrayLen-1)-stIndex+1)
		stLen=(stArrayLen-1)-stIndex+1;

	memcpy(&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][stIndex], pByte, stLen*sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I[stBand][0]));
	return true;

}
/*
bool CQualComm_RFNVItems::Write_NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I(int iBandID)
{
	DWORD dwNVItemID_LNA_Offset = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case WCDMA_IMT_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_LNA_OFFSET_VS_FREQ_4_I;
		break;
	case WCDMA_PCS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_1900_LNA_OFFSET_VS_FREQ_4_I;
		break;
	case WCDMA_AWS_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_BC4_LNA_OFFSET_VS_FREQ_4_I;
		break;
	case WCDMA_850_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_800_LNA_OFFSET_VS_FREQ_4_I;
		break;
	case WCDMA_900_BAND_ID:
		dwNVItemID_LNA_Offset = NV_WCDMA_900_LNA_OFFSET_VS_FREQ_4_I;
		break;
	default:
		ASSERT_MSG(false, _T("Wrong Band ID"))
	}
	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNVItemID_LNA_Offset, (unsigned char* )&m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I, sizeof(m_iNV_WCDMA_LNA_OFFSET_VS_FREQ_4_I), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_THERM_MAX(PBYTE pByte, size_t& stSize)
{
	size_t stArrayLen=1;
	if(stSize>stArrayLen)
		stSize=stArrayLen;

	memcpy(pByte, &m_uiNV_THERM_MAX_MIN[1], stSize*sizeof(m_uiNV_THERM_MAX_MIN[0]));
	return true;

}
bool CQualComm_RFNVItems::Set_NV_THERM_MAX(PBYTE pByte, size_t stSize)
{
	size_t stArrayLen=1;
	if(stSize>stArrayLen)
		stSize=stArrayLen;

	memcpy(&m_uiNV_THERM_MAX_MIN[1], pByte, stSize*sizeof(m_uiNV_THERM_MAX_MIN[0]));
	return true;
}

bool CQualComm_RFNVItems::Get_NV_THERM_MIN(PBYTE pByte, size_t& stSize)
{
	size_t stArrayLen=1;
	if(stSize>stArrayLen)
		stSize=stArrayLen;

	stSize = sizeof(m_uiNV_THERM_MAX_MIN[0]);
	memcpy(pByte, &m_uiNV_THERM_MAX_MIN[0], stSize);
	return true;

}

bool CQualComm_RFNVItems::Set_NV_THERM_MIN(PBYTE pByte, size_t stSize)
{
	size_t stArrayLen=1;
	if(stSize>stArrayLen)
		stSize=stArrayLen;
	stSize = sizeof(m_uiNV_THERM_MAX_MIN[0]);
	memcpy(&m_uiNV_THERM_MAX_MIN[0], pByte, stSize);
	return true;
}
 /*
bool CRFNVItems::Write_NV_ENH_THERM_MAX_MIN()
{
	DWORD dwNV_ENH_THERM_MAX_MIN = NV_ENH_THERM_I;
	WORD wStatus = 0;

	if(m_pcPhone->WriteRFNVItemInBatchMode(dwNV_ENH_THERM_MAX_MIN, (unsigned char* )&m_iNV_ENH_THERM_MAX_MIN, sizeof(m_iNV_ENH_THERM_MAX_MIN), &wStatus)!=QLIB_FUNCTION_SUCCESSFUL)
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_WCDMA_IM2_I_VALUE_I (PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_I_VALUE_I[0]) ;
	memcpy(pByte, &m_iNV_WCDMA_IM2_I_VALUE_I[stBand], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_IM2_I_VALUE_I (PBYTE pByte, int stBand, size_t stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_I_VALUE_I[0]) ;
	memcpy(&m_iNV_WCDMA_IM2_I_VALUE_I[stBand], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_WCDMA_IM2_Q_VALUE_I (PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_Q_VALUE_I[0]) ;
	memcpy(pByte, &m_iNV_WCDMA_IM2_Q_VALUE_I[stBand], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_IM2_Q_VALUE_I (PBYTE pByte, int stBand, size_t stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_Q_VALUE_I[0]) ;
	memcpy(&m_iNV_WCDMA_IM2_Q_VALUE_I[stBand], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_WCDMA_IM2_GM_VALUE_I (PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_GM_VALUE_I[0]) ;
	memcpy(pByte, &m_iNV_WCDMA_IM2_GM_VALUE_I[stBand], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_WCDMA_IM2_GM_VALUE_I (PBYTE pByte, int stBand, size_t stSize)
{
	stSize = sizeof(m_iNV_WCDMA_IM2_GM_VALUE_I[0]) ;
	memcpy(&m_iNV_WCDMA_IM2_GM_VALUE_I[stBand], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_CARRIER_SUPPRESSION_Idc_Qdc(PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_uiNV_IDC_QDC[stBand]);
	memcpy(pByte, &m_uiNV_IDC_QDC[stBand][0], stSize);
	return true;

}

/*
bool CQualComm_RFNVItems::Get_NV_GSM_CARRIER_SUPPRESSION_Qdc(PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_uiNV_IDC_QDC[stBand][0]);
	memcpy(pByte, &m_uiNV_IDC_QDC[stBand][0], stSize);
	return true;

}
*/

bool CQualComm_RFNVItems::Set_NV_GSM_CARRIER_SUPPRESSION_Qdc(PBYTE pByte, int stBand, size_t stSize)
{
	ASSERT(stSize == sizeof(m_uiNV_IDC_QDC[stBand][0]));
	memcpy(&m_uiNV_IDC_QDC[stBand][0], pByte, stSize);
	return true;
}

/*
bool CQualComm_RFNVItems::Get_NV_GSM_CARRIER_SUPPRESSION_Idc(PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_uiNV_IDC_QDC[stBand][1]);
	memcpy(pByte, &m_uiNV_IDC_QDC[stBand][1], stSize);
	return true;

}
*/

bool CQualComm_RFNVItems::Set_NV_GSM_CARRIER_SUPPRESSION_Idc(PBYTE pByte, int stBand, size_t stSize)
{
	ASSERT(stSize == sizeof(m_uiNV_IDC_QDC[stBand][1]));
	memcpy(&m_uiNV_IDC_QDC[stBand][1], pByte, stSize);
	return true;
}

/*
bool CQualComm_RFNVItems::Write_NV_GSM_CARRIER_SUPPRESSION_Idc_Qdc(int iBandID)
{
	DWORD dwNV_GSM_CARRIER_SUPPRESSION = 0;
	WORD wStatus = 0;

	switch(iBandID)
	{
	case GSM_DCS_BAND_ID:
		dwNV_GSM_CARRIER_SUPPRESSION= NV_GSM_DCS_CARRIER_SUPPRESSION;
		break;
	case GSM_PCS_BAND_ID:
		dwNV_GSM_CARRIER_SUPPRESSION = NV_GSM_1900_CARRIER_SUPPRESSION;
		break;
	}
	if(!m_pIPhone->QMSL_DIAG_NV_WRITE_F(dwNV_GSM_CARRIER_SUPPRESSION, (unsigned char* )&m_uiNV_IDC_QDC, sizeof(m_uiNV_IDC_QDC), &wStatus))
	{
		return false;
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_NV_GSM_AMAM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F1[stBand][0]);
	stSize = stSize*RF_AMAM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_uiNV_GSM_AMAM_MASTER_TBL_F1[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMAM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F1[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_uiNV_GSM_AMAM_MASTER_TBL_F1[stBand][stIndex], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_AMAM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F2[stBand][0]);
	stSize = stSize*RF_AMAM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_uiNV_GSM_AMAM_MASTER_TBL_F2[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMAM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F2[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_uiNV_GSM_AMAM_MASTER_TBL_F2[stBand][stIndex], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_AMAM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F3[stBand][0]);
	stSize = stSize*RF_AMAM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_uiNV_GSM_AMAM_MASTER_TBL_F3[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMAM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F3[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_uiNV_GSM_AMAM_MASTER_TBL_F3[stBand][stIndex], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_AMPM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F1[stBand][0]);
	stSize = stSize*RF_AMPM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_iNV_GSM_AMPM_MASTER_TBL_F1[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMPM_MASTER_TBL_F1(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F1[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_iNV_GSM_AMPM_MASTER_TBL_F1[stBand][stIndex], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_AMPM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F2[stBand][0]);
	stSize = stSize*RF_AMPM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_iNV_GSM_AMPM_MASTER_TBL_F2[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMPM_MASTER_TBL_F2(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F2[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_iNV_GSM_AMPM_MASTER_TBL_F2[stBand][stIndex], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_AMPM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t& stSize)
{
	stSize = sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F3[stBand][0]);
	stSize = stSize*RF_AMPM_MASTER_TBL_SIZ/8;
	memcpy(pByte, &m_iNV_GSM_AMPM_MASTER_TBL_F3[stBand][stIndex], stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_AMPM_MASTER_TBL_F3(PBYTE pByte, int stBand, size_t stIndex, size_t stSize)
{
	size_t stTemp = stSize;
	stTemp %= sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F3[stBand][0]);
	ASSERT(stTemp == 0);
	memcpy(&m_iNV_GSM_AMPM_MASTER_TBL_F3[stBand][stIndex], pByte, stSize);
	return true;
}

/*
bool CQualComm_RFNVItems::Write_NV_GSM_TX_V2(std::vector<int> vChannel,int iBandID)
{
	bool bRet = false;
	for(int i=0;i<vChannel.size();i++)
	{
		if(vChannel[i]==-1)
			continue;

		switch(i)
		{
		case 0:
			if(!Write_NV_GSM_AMAM_MASTER_TBL_F1(iBandID)){return bRet;}
			if(!Write_NV_GSM_AMPM_MASTER_TBL_F1(iBandID)){return bRet;}
			break;
		case 1:
			if(!Write_NV_GSM_AMAM_MASTER_TBL_F2(iBandID)){return bRet;}
			if(!Write_NV_GSM_AMPM_MASTER_TBL_F2(iBandID)){return bRet;}
			break;

		case 2:
			if(!Write_NV_GSM_AMAM_MASTER_TBL_F3(iBandID)){return bRet;}
			if(!Write_NV_GSM_AMPM_MASTER_TBL_F3(iBandID)){return bRet;}
			break;
		default: 
			ASSERT(0);
			return false;
		}
	}
 
	return true;
 
}
*/


/*
bool CQualComm_RFNVItems::Write_NV_GSM_AMAM_MASTER_TBL_F1(int iBandID)
{
	unsigned short unNV_GSM_AMAM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case 1800:
		unNV_GSM_AMAM_MASTER_TBL= NV_DCS_AMAM_MASTER_TBL_SEG1_F1_I;
		break;
	case 1900:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F1_I;
		break;
	case 850:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_850_AMAM_MASTER_TBL_SEG1__F1_I;
		break;
	case 900:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_AMAM_MASTER_TBL_SEG1_F1_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMAM_MASTER_TBL + i, (unsigned char* )&m_uiNV_GSM_AMAM_MASTER_TBL_F1 + i * 64 *sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F1[0]), sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F1[0])*64, &wStatus))
		{
			return false;
		}
	}
	return true;
}

bool CQualComm_RFNVItems::Write_NV_GSM_AMAM_MASTER_TBL_F2(int iBandID)
{
	unsigned short unNV_GSM_AMAM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case GSM_DCS_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL= NV_DCS_AMAM_MASTER_TBL_SEG1_F2_I;
		break;
	case GSM_PCS_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F2_I;
		break;
	case GSM_850_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_850_AMAM_MASTER_TBL_SEG1__F2_I;
		break;
	case GSM_EGSM_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_AMAM_MASTER_TBL_SEG1_F2_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMAM_MASTER_TBL + i, (unsigned char* )&m_uiNV_GSM_AMAM_MASTER_TBL_F2 + i * 64 *sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F2[0]), sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F2[0])*64, &wStatus))
		{
			return false;
		}
	}
	return true;
}
bool CQualComm_RFNVItems::Write_NV_GSM_AMAM_MASTER_TBL_F3(int iBandID)
{
	unsigned short unNV_GSM_AMAM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case GSM_DCS_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL= NV_DCS_AMAM_MASTER_TBL_SEG1_F3_I;
		break;
	case GSM_PCS_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_1900_AMAM_MASTER_TBL_SEG1_F3_I;
		break;
	case GSM_850_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_850_AMAM_MASTER_TBL_SEG1__F3_I;
		break;
	case GSM_EGSM_BAND_ID:
		unNV_GSM_AMAM_MASTER_TBL = NV_GSM_AMAM_MASTER_TBL_SEG1_F3_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMAM_MASTER_TBL + i, (unsigned char* )&m_uiNV_GSM_AMAM_MASTER_TBL_F3 + i * 64 *sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F3[0]), sizeof(m_uiNV_GSM_AMAM_MASTER_TBL_F3[0])*64, &wStatus))
		{
			return false;
		}
	}
	return true;
}

bool CQualComm_RFNVItems::Write_NV_GSM_AMPM_MASTER_TBL_F1(int iBandID)
{
	unsigned short unNV_GSM_AMPM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case 1800:
		unNV_GSM_AMPM_MASTER_TBL= NV_DCS_AMPM_MASTER_TBL_SEG1_F1_I;
		break;
	case 1900:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F1_I;
		break;
	case 850:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_850_AMPM_MASTER_TBL_SEG1__F1_I;
		break;
	case 900:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_AMPM_MASTER_TBL_SEG1_F1_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMPM_MASTER_TBL + i, (unsigned char* )&m_iNV_GSM_AMPM_MASTER_TBL_F1 + i * 32 *sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F1[0]), sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F1[0])*32, &wStatus))
		{
			return false;
		}
	}
	return true;
}

bool CQualComm_RFNVItems::Write_NV_GSM_AMPM_MASTER_TBL_F2(int iBandID)
{
	unsigned short unNV_GSM_AMPM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case GSM_DCS_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL= NV_DCS_AMPM_MASTER_TBL_SEG1_F2_I;
		break;
	case GSM_PCS_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F2_I;
		break;
	case GSM_850_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_850_AMPM_MASTER_TBL_SEG1__F2_I;
		break;
	case GSM_EGSM_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_AMPM_MASTER_TBL_SEG1_F2_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMPM_MASTER_TBL + i, (unsigned char* )&m_iNV_GSM_AMPM_MASTER_TBL_F2 + i * 32 *sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F2[0]), sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F2[0])*32, &wStatus))
		{
			return false;
		}
	}
	return true;
}
bool CQualComm_RFNVItems::Write_NV_GSM_AMPM_MASTER_TBL_F3(int iBandID)
{
	unsigned short unNV_GSM_AMPM_MASTER_TBL = 0;
	WORD wStatus = 0;
 
	switch(iBandID)
	{
	case GSM_DCS_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL= NV_DCS_AMPM_MASTER_TBL_SEG1_F3_I;
		break;
	case GSM_PCS_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_1900_AMPM_MASTER_TBL_SEG1_F3_I;
		break;
	case GSM_850_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_850_AMPM_MASTER_TBL_SEG1__F3_I;
		break;
	case GSM_EGSM_BAND_ID:
		unNV_GSM_AMPM_MASTER_TBL = NV_GSM_AMPM_MASTER_TBL_SEG1_F3_I;
		break;
	default:
		//ASSERT_MSG(false, _T("Wrong Band ID"))
		break;
	}
	for( int i = 0 ; i < 8 ; i++){
		if(!m_pcPhone->QMSL_DIAG_NV_WRITE_F(unNV_GSM_AMPM_MASTER_TBL + i, (unsigned char* )&m_iNV_GSM_AMPM_MASTER_TBL_F3 + i * 32 *sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F3[0]), sizeof(m_iNV_GSM_AMPM_MASTER_TBL_F3[0])*32, &wStatus))
		{
			return false;
		}
	}
	return true;
}
*/

bool CQualComm_RFNVItems::Get_GSM_NV_POLAR_PATH_DELAY_I(PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_uiNV_GSM_POLAR_PATH_DELAY[stBand]);
	memcpy(pByte, &m_uiNV_GSM_POLAR_PATH_DELAY[stBand], stSize);
	return true;

}

bool CQualComm_RFNVItems::Set_GSM_NV_POLAR_PATH_DELAY_I(PBYTE pByte, int stBand, size_t stSize)
{
	ASSERT(stSize == sizeof(m_uiNV_GSM_POLAR_PATH_DELAY[stBand]));
	memcpy(&m_uiNV_GSM_POLAR_PATH_DELAY[stBand], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Get_NV_GSM_DYNAMIC_RANGE(PBYTE pByte, int stBand, size_t& stSize)
{
	stSize = sizeof(m_iNV_DYNAMIC_RANGE[stBand]);
	memcpy(pByte, &m_iNV_DYNAMIC_RANGE[stBand], stSize);
	return true;

}

bool CQualComm_RFNVItems::Set_NV_GSM_DYNAMIC_RANGE_MAX(PBYTE pByte, int stBand, size_t stSize)
{
	ASSERT(stSize == sizeof(m_iNV_DYNAMIC_RANGE[stBand][0]));
	memcpy(&m_iNV_DYNAMIC_RANGE[stBand][0], pByte, stSize);
	return true;
}

bool CQualComm_RFNVItems::Set_NV_GSM_DYNAMIC_RANGE_MIN(PBYTE pByte, int stBand, size_t stSize)
{
	ASSERT(stSize == sizeof(m_iNV_DYNAMIC_RANGE[stBand][1]));
	memcpy(&m_iNV_DYNAMIC_RANGE[stBand][1], pByte, stSize);
	return true;
}
