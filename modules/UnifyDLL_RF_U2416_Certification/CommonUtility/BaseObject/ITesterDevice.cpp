#include "stdafx.h"
#include "ITesterDevice.h"


RF_IMPLEMENT_DYNAMIC(ITesterDevice)


int ITesterDevice::g_iRFInputPort = 0;
int ITesterDevice::g_iRFOutputPort = 0;
int ITesterDevice::g_iMode = 0;
int ITesterDevice::g_iCurrentBand = -1 ;
int ITesterDevice::g_iCurrentChannel = -1 ;



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

void ITesterDevice::SetBand(int iBand)
{
    g_iCurrentBand = iBand ;
}

void ITesterDevice::SetChannel(int iChannel)
{
    g_iCurrentChannel = iChannel ;
}
