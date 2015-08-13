#include "stdafx.h"
#include "ITesterDevice.h"


RF_IMPLEMENT_DYNAMIC(ITesterDevice)

int ITesterDevice::g_iRFInputPort = 0;
int ITesterDevice::g_iRFOutputPort = 0;
int ITesterDevice::g_iMode = 0;

int ITesterDevice::m_h_RF_NSIG;
//nonsinging
int ITesterDevice::m_h_GSM_NSig = 0;
int ITesterDevice::m_h_DCS_NSig = 0;
int ITesterDevice::m_h_PCS_NSig = 0;
int ITesterDevice::m_h_GSM850MS_NSig =0;
int ITesterDevice::m_h_GSM900MS_NSig =0;
//singing
int ITesterDevice::m_h_GSM_Sig = 0;
int ITesterDevice::m_h_DCS_Sig = 0;
int ITesterDevice::m_h_PCS_Sig = 0;
int ITesterDevice::m_h_GSM850MS_Sig = 0;
int ITesterDevice::m_h_GSM900MS_Sig =0;

//CDMA2000
 int ITesterDevice::m_h_CDMACellular_NSig = 0;
 int ITesterDevice::m_h_CDMAPCS_NSig = 0;
 int ITesterDevice::m_h_CDMACellular_Sig = 0;
 int ITesterDevice::m_h_CDMAPCS_Sig = 0;


ITesterDevice::ITesterDevice():m_bInternalReference(true)
{
 
}
 
 
ITesterDevice::~ITesterDevice()
{
 
}

void ITesterDevice::SetRFInputPort(int iInputPort)
{
	g_iRFInputPort = iInputPort;
}

void ITesterDevice::SetRFOutputPort(int iOutputPort)
{
	g_iRFOutputPort = iOutputPort;
}

void ITesterDevice::SetMode(int iMode)
{
	g_iMode = iMode;
}

