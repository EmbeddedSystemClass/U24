#include "stdafx.h"
#include "USB4702Ctrl.h"
#include "USB4702.h"
#include "USB4702CtrlDLL_API.h"



extern CUSB4702CtrlApp theApp;

using namespace std;


USB4702DLLAPI int __stdcall USB4702_Init(int nAddress)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	LRESULT Ret = UB_init();
	if (Ret != SUCCESS) {
		return 0;
	}
	return 1;
}

USB4702DLLAPI int __stdcall USB4702_Close(int nAddress)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	char bit[2] = {0};
	for (int i = 0; i < 8; i++) {
		bit[0] = '0' + i;
		USB4702_ClearDOBit(nAddress, bit);
	}
	UB_close();

	return 1;
}

USB4702DLLAPI int __stdcall USB4702_GetADCVol(int nAddress, char * Channel, char * buf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	float vol;
	LRESULT Ret = UB_get_ADC_voltage(atoi(Channel), &vol);
	if (Ret != SUCCESS)
		return 0;
	sprintf(buf, "%f", vol);

	return 1;
}

//ytt for LED test: average 5 test item

USB4702DLLAPI int __stdcall USB4702_GetADCVolEx(int nAddress, char * Channel, char * buf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));
	float vol;
    float volSet[100]; 
	float volHigh=-10;
	float volLow =-10 ;
	float volSum=0;
	int volCount=0;

	for(int i=0;i<10;i++)
	{

		LRESULT Ret = UB_get_ADC_voltage(atoi(Channel), &vol);
		if (Ret != SUCCESS)
			return 0;
        volSet[i]= vol;
		Sleep(10);
		if(vol>= volHigh)
		{
             volHigh = vol;
		}
		if(vol<= volLow)
		{
             volLow = vol;
		}
	}

    for(int i=0;i<10;i++)
	{
        if( (volSet[i]!= volHigh) && (volSet[i]!= volLow)   )
		{
            volCount++;
		    volSum+=volSet[i];
		}
	}
    if(volCount>0)
	{
    volSum = volSum/volCount;
	}
	sprintf(buf, "%f", volSum);

	return 1;
}


USB4702DLLAPI int __stdcall USB4702_SetAOVol(int nAddress, char * Channel, char * voltage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	float vol = static_cast<float>(atof(voltage));
	LRESULT Ret = UB_set_AO_voltage(atoi(Channel), &vol);
	if (Ret != SUCCESS) {
		return 0;
	}
	return 1;
}

USB4702DLLAPI int __stdcall USB4702_GetDOVal(int nAddress, char* buf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	USHORT val;
	LRESULT Ret = UB_get_DO_value(&val);
	if (Ret != SUCCESS) {
		return 0;
	}
	sprintf(buf, "%d", val);
	return 1;
}

USB4702DLLAPI int __stdcall USB4702_SetDOVal(int nAddress, char* val)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	LRESULT Ret = UB_set_DO_value(atoi(val));
	if (Ret != SUCCESS) {
		return 0;
	}
	return 1;
}

USB4702DLLAPI int __stdcall USB4702_SetDOBit(int nAddress, char* bit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	LRESULT Ret = UB_set_DO_bit(atoi(bit));
	if (Ret != SUCCESS) {
		return 0;
	}
	return 1;
}

USB4702DLLAPI int __stdcall USB4702_ClearDOBit(int nAddress, char* bit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE(_T("%s\n"), _T(__FUNCTION__));

	LRESULT Ret = UB_clear_DO_bit(atoi(bit));
	if (Ret != SUCCESS) {
		return 0;
	}
	return 1;
}


