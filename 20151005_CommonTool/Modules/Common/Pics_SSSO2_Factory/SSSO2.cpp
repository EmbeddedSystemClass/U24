// SSSO2.cpp: implementation of the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSSO2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSSSO2::CSSSO2()
{
	::CoInitialize(NULL);
}

CSSSO2::~CSSSO2()
{
	::CoUninitialize();
}

bool CSSSO2::Initial_SSSO2Dll()
{
	HRESULT hresult;
	CLSID clsid;

	hresult=CLSIDFromProgID(OLESTR("SSS_O2.clsO2Check"), &clsid);
	hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_clsO2Check),(LPVOID *) &ITS_O2);
	if(FAILED(hresult))
	{
//		WinExec("tregsvr -q SSS_O2.Dll",SW_SHOWDEFAULT);
//		WinExec("tregsvr -t SSS_O2.Dll",SW_SHOWDEFAULT);
		WinExec("regsvr32 /s SSS_O2.Dll",SW_SHOWDEFAULT);

		CoUninitialize();

		CoInitialize(NULL);

		hresult=CLSIDFromProgID(OLESTR("SSS_O2.clsO2Check"), &clsid);
		hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_clsO2Check),(LPVOID *) &ITS_O2);
		if(FAILED(hresult))
		{
			return false;
		}
	}
	return true;
}
bool CSSSO2::SSSO2_QueryFG_SN_Wolf(char *StationtState,
										 char *pIMEI,
										 char *szMSN,
										 char *szBTAddr,
										 char *szMACAddr,
										 char *szSWver,
										 char *szHWver,
										 char *szPVK,
										 char *ErrCode, 
										 char *ErrMsg,
										 char *Status_Output
										 )
{
	//return true;

	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char        _IMEI[50];
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	BOOL bResult;	
	CComBSTR	_GSN(50,""), _BT(50,""),  _SWV(50,""), _HW(50,""), _MDate(20,""), _MTime(20,""), _Carton(50,""), _DBDate(20,""), _DBTime(20,""), _Phone(50,""), _Mode(2,""), _ErrCode(50,""), _ErrDes(104,""),_Wireless_MAC(50,""),_PVK(50,"");
	_bstr_t		_temp;
	char		_szErrCode[50];

	bResult = ITS_O2->funQueryIMEI_Data(_IMEI,
		StationtState,
		&_GSN,
		&_BT,
		&_HW,
		&_SWV,
		&_MDate,
		&_MTime,
		&_Carton,
		&_DBDate,
		&_DBTime,
		&_Phone,
		&_Mode,
		&_ErrCode,
		&_ErrDes,
		&_Wireless_MAC,
		&_PVK);//Added

	//_temp = _T("123456");
	//_stprintf(szMSN,_T("%s"), (LPCTSTR)_temp);
	//_GSN.Empty();

	_temp = _GSN.Copy();
	_stprintf(szMSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(szBTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

	_temp = _Wireless_MAC.Copy();
	_stprintf(szMACAddr, _T("%s"), (LPCTSTR)_temp);
	_Wireless_MAC.Empty();

	_temp = _SWV.Copy();//Lucky
	_stprintf(szSWver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(szHWver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _PVK.Copy();//SUNLIGHT
	_stprintf(szPVK, _T("%s"), (LPCTSTR)_temp);
	_PVK.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;

}
bool CSSSO2::SSSO2_QueryIMEI_V2(char *StationtState,
							  char *pIMEI,
							  char *szMSN,
							  char *szBTAddr,
							  char *szMACAddr,
							  char *szSWver,
							  char *szHWver,
							  char *szPVK,
							  char *ErrCode, 
							  char *ErrMsg,
							  char *Status_Output
							 )
{
	//return true;

	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char        _IMEI[50];
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	BOOL bResult;	
	CComBSTR	_GSN(50,""), _BT(50,""),  _SWV(50,""), _HW(50,""), _MDate(20,""), _MTime(20,""), _Carton(50,""), _DBDate(20,""), _DBTime(20,""), _Phone(50,""), _Mode(2,""), _ErrCode(50,""), _ErrDes(104,""),_Wireless_MAC(50,""),_PVK(50,"");
	_bstr_t		_temp;
	char		_szErrCode[50];

	bResult = ITS_O2->funQueryIMEI_Data(_IMEI,
										StationtState,
										&_GSN,
										&_BT,
										&_HW,
										&_SWV,
										&_MDate,
										&_MTime,
										&_Carton,
										&_DBDate,
										&_DBTime,
										&_Phone,
										&_Mode,
										&_ErrCode,
										&_ErrDes,
										&_Wireless_MAC,
										&_PVK);//Added

	//_temp = _T("123456");
	//_stprintf(szMSN,_T("%s"), (LPCTSTR)_temp);
	//_GSN.Empty();

	_temp = _GSN.Copy();
	_stprintf(szMSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(szBTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

	_temp = _Wireless_MAC.Copy();
	_stprintf(szMACAddr, _T("%s"), (LPCTSTR)_temp);
	_Wireless_MAC.Empty();

	_temp = _SWV.Copy();//Lucky
	_stprintf(szSWver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(szHWver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _PVK.Copy();//SUNLIGHT
	_stprintf(szPVK, _T("%s"), (LPCTSTR)_temp);
	_PVK.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;
	
	return false;

}

bool CSSSO2::SSSO2_QueryIMEI(char *StationtState,
										 char *pIMEI,
										 char *szMSN,
										 char *szBTAddr,
										 char *szMACAddr,
										 char *szSWver,
										 char *szHWver,
										 char *ErrCode, 
										 char *ErrMsg,
										 char *Status_Output
										 )
{
	//return true;

	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char        _IMEI[50];
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	BOOL bResult;	
	CComBSTR	_GSN(50,""), _BT(50,""),  _SWV(50,""), _HW(50,""), _MDate(20,""), _MTime(20,""), _Carton(50,""), _DBDate(20,""), _DBTime(20,""), _Phone(50,""), _Mode(2,""), _ErrCode(50,""), _ErrDes(104,""),_Wireless_MAC(50,""),_PVK(50,"");
	_bstr_t		_temp;
	char		_szErrCode[50];

	bResult = ITS_O2->funQueryIMEI_Data(_IMEI,
		StationtState,
		&_GSN,
		&_BT,
		&_HW,
		&_SWV,
		&_MDate,
		&_MTime,
		&_Carton,
		&_DBDate,
		&_DBTime,
		&_Phone,
		&_Mode,
		&_ErrCode,
		&_ErrDes,
		&_Wireless_MAC,
		&_PVK);//Added

	//_temp = _T("123456");
	//_stprintf(szMSN,_T("%s"), (LPCTSTR)_temp);
	//_GSN.Empty();

	_temp = _GSN.Copy();
	_stprintf(szMSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(szBTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

	_temp = _Wireless_MAC.Copy();
	_stprintf(szMACAddr, _T("%s"), (LPCTSTR)_temp);
	_Wireless_MAC.Empty();

	_temp = _SWV.Copy();//Lucky
	_stprintf(szSWver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(szHWver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;

}

bool CSSSO2::SSSO2_UpdateGSNPN(char *Picasso,char *GSN,char *PN,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(12,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[10];
	short int	_iRow;
	char		_GSN[50];
	char        _PN[50];//the length of PN is 12
	ZeroMemory(_GSN,50);
	ZeroMemory(_PN,50);

	char       _PICASSO[50];//the length of PICASSO is 10
	ZeroMemory(_PICASSO,50);
	strcpy(_PICASSO,Picasso);
	strcpy(_GSN,GSN);
	strcpy(_PN,PN);

	bResult = ITS_O2->FunUpdateGSNPN(_PICASSO,_PN,_GSN,"IMEITOOL",&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("")) == 0 )
		return true;

	return false;
}
//update IMEI status to 'I'
bool CSSSO2::SSSO2_UpdateIMEIStatus(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;
	char		_SWver[50];
	char        g_HW_ver[50];
	char        _NAL[50];//the length of NAL is 15
    ZeroMemory(_SWver,50);
    ZeroMemory(g_HW_ver,50);
	ZeroMemory(_NAL,50);
	
	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	strcpy(_SWver,szSWver);
	strcpy(g_HW_ver,szHWver);
	strcpy(_NAL,szMal);

	//AfxMessageBox(_IMEI);
	//AfxMessageBox(_SWver);
	//AfxMessageBox(g_HW_ver);
	//AfxMessageBox(_NAL);
	

	bResult = ITS_O2->funUpdateIMEI_Data(_IMEI,_SWver,g_HW_ver,_NAL/*Added*/,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;
	
	return false;
}

//sunlight 2011/01/06: change IMEI status to F
bool CSSSO2::SSSO2_UpdateIMEIStatus_F(char *pIMEI,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateIMEIStatus_Data(_IMEI,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}
//
// For Rework, AJ, 2011/04/21
bool CSSSO2::SSSO2_UpdateIMEIStatus_R(char *pIMEI,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];

	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateIMEI_Rework(_IMEI,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}
//

bool CSSSO2::SSSO2_UpdateIMEIStatus_W(char *pIMEI,char *ErrCode, char *ErrMsg) //Magan 20140627
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateToW_Data(_IMEI,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}
bool CSSSO2::SSSO2_UpdateIMEIStatus_B(char *pIMEI,char *ErrCode, char *ErrMsg) //Magan 20140627
{
		if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateToB_Data(_IMEI,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}

bool CSSSO2::SSSO2_CheckNAL_PN(char *inIMEI,char *nPort,char *inMal,char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	char		_PartNumber[50];//the length of PN is 12
	char        _NAL[50];//the length of NAL is 15
	char        _IMEI[50];//the length of IMEI is 15
    ZeroMemory(_szErrCode,50);
    ZeroMemory(_PartNumber,50);
	ZeroMemory(_NAL,50);
	ZeroMemory(_IMEI,50);

	strcpy(_PartNumber,nPort);
	strcpy(_NAL,inMal);
	strcpy(_IMEI,inIMEI);
	
	bResult = ITS_O2->funCheckNALPN_Data(_IMEI,_PartNumber,_NAL,&_ErrCode,&_ErrDes);

	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, _T("400")) == 0 )
		return true;
	
	return false;

}

bool CSSSO2::SSSO2_GetProcessResult(char* Picasso, char* Function, char* Model, char *ErrCode, char *ErrMsg)
{

	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,""),_GDNG(2,"");
	char        _szGDNG[2];
	char		_szErrCode[50];
//check MMI
	bResult = ITS_O2->funGetProcessResult(Picasso,Function,Model,&_GDNG,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	_temp = _GDNG.Copy();
	_stprintf(_szGDNG, _T("%s"), (LPCTSTR)_temp);
	_GDNG.Empty();
    
	if (strncmp(_szGDNG, _T("Y"),1) != 0 )
	   return false;
	else 
		return true;
}

bool CSSSO2::SSS02_CheckPicassoInSMT(char* Picasso, char* szProcess)
{
	if(!Initial_SSSO2Dll())
	{
		return false;
		_stprintf(Picasso, _T("Initial_SSs02Dll() fail"));
	}

	bool bResult =false;
	BSTR sBSMobileID[100];

	ZeroMemory(sBSMobileID,100);
	bResult = ITS_O2->funCheckPicassoInSMT(Picasso, szProcess);

	return bResult;

}

bool CSSSO2::SSS02_CheckPicassoForLuna(char* Picasso, char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(Picasso, _T("Initial_SSs02Dll() fail"));
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		return false;
	}

	_bstr_t		_temp;

	CComBSTR	_ErrCode(12,""), _ErrDes(104,"");

	bool bResult = ITS_O2-> funCheckPicassoForLuna(Picasso, &_ErrCode, &_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s_%s"), Picasso,(LPCTSTR)_temp);
	_ErrDes.Empty();

	return bResult;

}

bool CSSSO2::SSS02_CheckPiacssoInPICS(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(szError, _T("Initial_SSs02Dll() fail"));
		return false;
	}

	BSTR sBSIMEI[100];
	BSTR sBSMobileID[100];

	ZeroMemory(sBSMobileID,100);
	ZeroMemory(sBSIMEI,100);

	int nResult = ITS_O2->funCheckPicassoInPICS(szModel, Picasso, sBSIMEI, sBSMobileID, szProcess);

	if(nResult == 1)
	{
		for (int nIndex = 0; nIndex < 15; ++nIndex)
			szIMEI[nIndex] = (*sBSIMEI)[nIndex];
	//memcpy(szIMEI, *sBSIMEI, 15);
		szIMEI[15] = 0;

		for (int nIndex = 0; nIndex < 22; ++nIndex)
			szMobileID[nIndex] = (*sBSMobileID)[nIndex];
	//memcpy(szMobileID, *sBSMobileID, 22);
		szMobileID[22] = 0;
	}

	_stprintf(szError, _T("0x%08X"), nResult);
	szError[3]=szError[9];
	szError[4]=0;

//	_itoa_s(nResult,szError,2048,10);
	if(nResult == 0)
		return true;
	else
		return false;
}

bool CSSSO2::SSSO2_QuerySN5(char* szIMEI, char* szServiceTag, char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,""), _STag(50,"");
	char		_szServiceTag[50] = {0};
	//check MMI
	bResult = ITS_O2->funQuerySN5(szIMEI,&_ErrCode,&_ErrDes,&_STag);
	_temp = _ErrCode.Copy();
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	_temp = _STag.Copy();
	_stprintf(szServiceTag, _T("%s"), (LPCTSTR)_temp);
	_STag.Empty();

	if (strcmp(ErrCode, _T("0000")) != 0 )
		return false;

	else return true;
}


bool CSSSO2::SSS02_ReadMobileID(char* szModelName, char* szPicasso, char* szMobileID, char* szErrorCode, char* szErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(szErrorCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(szErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}

	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,""), _MobileID(50, "");
	char		_szServiceTag[50] = {0};

	//check MMI
	_temp= ITS_O2->ReadMobileID(szModelName, szPicasso,&_ErrCode,&_ErrDes);
	_stprintf(szMobileID, _T("%s"), (LPCTSTR)_temp);

	_temp = _ErrCode.Copy();
	_stprintf(szErrorCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(szErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if (strcmp(szErrorCode, _T("0000")) != 0 )
		return false;

	else return true;

}
bool CSSSO2::SSS02_WriteMobileID(char* szModelName, char* szPicasso, char* szMobileID, char* szErrorCode, char* szErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(szErrorCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(szErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	bool bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	char		_szServiceTag[50] = {0};
	//check MMI
	bResult = ITS_O2->WriteMobileID(szModelName, szPicasso, szMobileID,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(szErrorCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(szErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if (strcmp(szErrorCode, _T("0000")) != 0 )
		return false;

	else return bResult;

}

/* Luna: Alex Add Start 2010/11/23 */
bool CSSSO2::SSSO2_UpdateIMEIST(char* szServiceTag, char* szIMEI, char* szErrCode, char* szErrMsg)
{
	/* Initial SSSO2 */
	if(!Initial_SSSO2Dll()) {
		_stprintf(szErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(szErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}

	BOOL bResult = false;
	CComBSTR _ErrCode(50, ""), _ErrDes(104, "");
	_bstr_t _temp;
	short int _iRow;

	char _IMEI[50];                   // IMEI Length       : 15
	char _ServiceTag[50];              // Service tagLength : 7
	ZeroMemory(_IMEI, 50);
	ZeroMemory(_ServiceTag,50);
	strcpy(_IMEI, szIMEI);
	strcpy(_ServiceTag, szServiceTag);

	bResult = ITS_O2->funUpdateIMEIInfo(_ServiceTag, _IMEI, &_iRow, &_ErrCode, &_ErrDes);

	_temp = _ErrCode.Copy();
	_stprintf(szErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(szErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if(strcmp(szErrCode, _T("0000")) != 0 ) {
		return false;
	}

	return true;
}
/* Luna: Alex Add End 2010/11/23 */

/* Luna: Alex Add Start 2010/12/07 */
bool CSSSO2::SSSO2_QueryST(char *StationtState,
										 char *pIMEI,
										 char *szMSN,
										 char *szBTAddr,
										 char *szMACAddr,
										 char *szSWver,
										 char *szHWver,
										 char *ErrCode, 
										 char *ErrMsg,
										 char *Status_Output
										 )
{
	//return true;

	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char        _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	BOOL bResult;	
	CComBSTR	_GSN(50,""), _BT(50,""),  _SWV(50,""), _HW(50,""), _MDate(20,""), _MTime(20,""), _Carton(50,""), _DBDate(20,""), _DBTime(20,""), _Phone(50,""), _Mode(2,""), _ErrCode(50,""), _ErrDes(104,""),_Wireless_MAC(50,""),_PVK(50,"");
	_bstr_t		_temp;
	char		_szErrCode[50];

	bResult = ITS_O2->funQuerySVG_Data(_IMEI,
		StationtState,
		&_GSN,
		&_BT,
		&_HW,
		&_SWV,
		&_MDate,
		&_MTime,
		&_Carton,
		&_DBDate,
		&_DBTime,
		&_Phone,
		&_Mode,
		&_ErrCode,
		&_ErrDes,
		&_Wireless_MAC,
		&_PVK);//Added

	//_temp = _T("123456");
	//_stprintf(szMSN,_T("%s"), (LPCTSTR)_temp);
	//_GSN.Empty();

	_temp = _GSN.Copy();
	_stprintf(szMSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(szBTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

	_temp = _Wireless_MAC.Copy();
	_stprintf(szMACAddr, _T("%s"), (LPCTSTR)_temp);
	_Wireless_MAC.Empty();

	_temp = _SWV.Copy();//Lucky
	_stprintf(szSWver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(szHWver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return TRUE;

	return false;

}
/* Luna: Alex Add End 2010/12/07 */

/* Wolf: Sunlight Add Start 2011/08/23 */
bool CSSSO2::SSSO2_QueryPVKforAcer(char * sz_IMEI,char * sz_PVK,char *ErrCode, char *ErrMsg)
{
	//return true;
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char   _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,sz_IMEI);
	BOOL bResult;	
	CComBSTR	_ErrCode(12,""), _ErrDes(104,""),_PVK(100,"");
	_bstr_t		_temp;
	char		_szErrCode[10];

	bResult = ITS_O2->funQueryIMEI_Data_Acer(_IMEI,
		&_ErrCode,
		&_ErrDes,
		&_PVK);


	_temp = _PVK.Copy();
	_stprintf(sz_PVK, _T("%s"), (LPCTSTR)_temp);
	_PVK.Empty();

	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return TRUE;

	else return false;

}

/*Sunlight add 20111208*/
bool CSSSO2::SSSO2_UpdateIMEIStatus_F_New(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;
	char		_SWver[50];
	char        g_HW_ver[50];
	char        _NAL[50];//the length of NAL is 15
	ZeroMemory(_SWver,50);
	ZeroMemory(g_HW_ver,50);
	ZeroMemory(_NAL,50);

	char       _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);
	strcpy(_SWver,szSWver);
	strcpy(g_HW_ver,szHWver);
	strcpy(_NAL,szMal);

	//AfxMessageBox(_IMEI);
	//AfxMessageBox(_SWver);
	//AfxMessageBox(g_HW_ver);
	//AfxMessageBox(_NAL);


	bResult = ITS_O2->funUpdateIMEIStatus_F(_IMEI,_SWver,g_HW_ver,_NAL/*Added*/,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}

/* sunlight modify 20120308, add input szSWver */
bool CSSSO2::SSSO2_UpdateIMEIStatus_I_New(char *pIMEI,char *szSWver,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char		_SWver[50];
	char       _IMEI[50];//the length of IMEI is 15

	ZeroMemory(_IMEI,50);
	ZeroMemory(_SWver,50);
	strcpy(_SWver,szSWver);

	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateIMEIStatus_I(_IMEI,_SWver,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}

/*Sunlight add end*/


/* sunlight add 20120703 */
bool CSSSO2::SSSO2_UpdateNAL(char *pIMEI,char *szNAL,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("0001"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char		_NAL[50];
	char       _IMEI[50];//the length of IMEI is 15

	ZeroMemory(_IMEI,50);
	ZeroMemory(_NAL,50);

	strcpy(_NAL,szNAL);
	strcpy(_IMEI,pIMEI);

	bResult = ITS_O2->funUpdateNAL(_IMEI,_NAL,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}
/*Sunlight add end*/


/*Sunlight add 20120706*/
bool CSSSO2::SSSO2_UpdateCustomerSW(char *pIMEI,char *szModel,char * szPartNo, char * szCustsw,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("0001"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	BOOL bResult;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");
	_bstr_t		_temp;
	char		_szErrCode[50];
	short int	_iRow;

	char       _IMEI[50];//the length of IMEI is 15
	char	   _Model[50];
	char       _Partno[50];
	char	   _SWver[250];

	ZeroMemory(_IMEI,50);
	ZeroMemory(_Model,50);
	ZeroMemory(_Partno,50);
	ZeroMemory(_SWver,250);

	strcpy(_IMEI,pIMEI);
	strcpy(_Model,szModel);
	strcpy(_Partno,szPartNo);
	strcpy(_SWver,szCustsw);

	bResult = ITS_O2->funUpdateCustomerSW(_IMEI,_Model,_Partno,_SWver,&_iRow,&_ErrCode,&_ErrDes);


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;

	return false;
}


bool CSSSO2::SSSO2_GetAliyunByIMEI(char *pIMEI,char *szUUID,char * ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("0001"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}

	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,""), _UUID(100,"");

	char        _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	//check MMI
	bResult = ITS_O2->funGetAliyunByIMEI(_IMEI,&_ErrCode,&_ErrDes,&_UUID);
	_temp = _ErrCode.Copy();
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	_temp = _UUID.Copy();
	_stprintf(szUUID, _T("%s"), (LPCTSTR)_temp);
	_UUID.Empty();

	if (strcmp(ErrCode, _T("0000")) != 0 )
		return false;

	else return true;

}


/*Sunlight add 20120827*/
bool CSSSO2::SSSO2_CheckCustomerPN(char *pIMEI,char * ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(ErrCode, _T("0001"));
		_stprintf(ErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}

	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(50,""), _ErrDes(104,"");

	char        _IMEI[50];//the length of IMEI is 15
	ZeroMemory(_IMEI,50);
	strcpy(_IMEI,pIMEI);

	//check MMI
	bResult = ITS_O2->CheckCustomerPN(_IMEI,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if (strcmp(ErrCode, _T("0000")) != 0 )
		return false;

	else return true;


}

/*Sunlight add 20120924, for OEM model, the unlock code should update to system*/
bool CSSSO2::SSSO2_UpdateUnlockCode( char *pIMEI,
											  char *szUnlockN,
											  char *szUnlockS,
											  char *szUnlockC,
											  char *szUnlockNS,
											  char *szUnlockSIM,
											  char *szUnlockUS,
											  char *szSPC,
											  char *szPhoneLock,
											  char *szErrCode, 
											  char *szErrMsg )
{
	if(!Initial_SSSO2Dll())
	{
		_stprintf(szErrCode, _T("Initial_PICS_SSS02_fail"));
		_stprintf(szErrMsg, _T("Initial_SSs02Dll() fail"));
		return false;
	}
	char    _IMEI[50];//the length of IMEI is 15
	char    _UnlockN[17];
	char    _UnlockS[17];
	char    _UnlockC[17];
	char    _UnlockNS[17];
	char    _UnlockSIM[17];
	char    _UnlockUS[17];
	char    _SPC[17];
	char    _UnlockPhoneLock[17];

	ZeroMemory(_IMEI,50);
	ZeroMemory(_UnlockN,17);
	ZeroMemory(_UnlockS,17);
	ZeroMemory(_UnlockC,17);
	ZeroMemory(_UnlockNS,17);
	ZeroMemory(_UnlockSIM,17);
	ZeroMemory(_UnlockUS,17);
	ZeroMemory(_SPC,17);
	ZeroMemory(_UnlockPhoneLock,17);

	strcpy(_IMEI,pIMEI);
	strcpy(_UnlockN,szUnlockN);
	strcpy(_UnlockS,szUnlockS);
	strcpy(_UnlockC,szUnlockC);
	strcpy(_UnlockNS,szUnlockNS);
	strcpy(_UnlockSIM,szUnlockSIM);
	strcpy(_UnlockUS,szUnlockUS);
	strcpy(_SPC,szSPC);
	strcpy(_UnlockPhoneLock,szPhoneLock);


	BOOL bResult;	
    _bstr_t		_temp;
	CComBSTR	_ErrCode(12,""), _ErrDes(104,"");
	short int	_iRow;

	bResult = ITS_O2->funUpdateCode(_IMEI,
		_UnlockN,
		_UnlockS,
		_UnlockC,
		_UnlockNS,
		_UnlockSIM,
		_UnlockUS,
		_SPC,
		_UnlockPhoneLock,
		&_iRow,
		&_ErrCode,
		&_ErrDes);//Added

	_temp = _ErrCode.Copy();
	_stprintf(szErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(szErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(szErrCode, _T("0000")) == 0 )
		return TRUE;

	return false;
}


#pragma warning(pop)