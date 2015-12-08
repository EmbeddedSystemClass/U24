// SSSO2.h: interface for the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_)
#define AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>

class CSSSO2  
{
public:
	bool SSSO2_CheckNAL_PN(char *inIMEI,char *nPort,char *inMal,char *ErrCode, char *ErrMsg);
	bool SSSO2_UpdateIMEIStatus(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg);
	bool SSSO2_UpdateIMEIStatus_F(char *pIMEI,char *ErrCode, char *ErrMsg);  //sunlight add for leopard, 2011/01/06
    bool SSSO2_UpdateIMEIStatus_R(char *pIMEI,char *ErrCode, char *ErrMsg) ; //For Rework, AJ, 2011/04/21
	bool SSSO2_UpdateIMEIStatus_W(char *pIMEI,char *ErrCode, char *ErrMsg); //Magan 20140627
	bool SSSO2_UpdateIMEIStatus_B(char *pIMEI,char *ErrCode, char *ErrMsg); //Magan 20140627
	bool SSSO2_QueryIMEI(char *StationtState,
									char *pIMEIStuct,
									char *szMSN,
									char *szBTAddr,
									char *szMACAddr,
									char *szSWver,
									char *szHWver,
									char *ErrCode, 
									char *ErrMsg,
									char *Status_Output
									);
	bool SSSO2_QueryIMEI_V2(char *StationtState,
		char *pIMEIStuct,
		char *szMSN,
		char *szBTAddr,
		char *szMACAddr,
		char *szSWver,
		char *szHWver,
		char *szPVK,
		char *ErrCode, 
		char *ErrMsg,
		char *Status_Output
		);

	bool SSSO2_QueryFG_SN_Wolf(char *StationtState,
		char *pIMEIStuct,
		char *szMSN,
		char *szBTAddr,
		char *szMACAddr,
		char *szSWver,
		char *szHWver,
		char *szPVK,
		char *ErrCode, 
		char *ErrMsg,
		char *Status_Output
		);
	bool Initial_SSSO2Dll();
	bool SSSO2_GetProcessResult (char* Picasso, char* Function, char* Model, char *ErrCode, char *ErrMsg);
	bool SSS02_CheckPicassoInSMT(char* Picasso, char* szProcess);
	bool SSS02_CheckPiacssoInPICS(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError);
	bool SSSO2_UpdateGSNPN(char *Picasso,char *GSN,char *PN,char *ErrCode, char *ErrMsg);
	bool SSSO2_QuerySN5(char* szIMEI, char* szServiceTag, char *ErrCode, char *ErrMsg);
	bool SSS02_ReadMobileID(char* szModelName, char* szPicasso, char* szMobileID, char* szErrorCode, char* szErrMsg);
	bool SSS02_WriteMobileID(char* szModelName, char* szPicasso, char* szMobileID, char* szErrorCode, char* szErrMsg);
	bool SSS02_CheckPicassoForLuna(char* Picasso, char *ErrCode, char *ErrMsg);  //sunlight add
	CSSSO2();
	_clsO2Check *ITS_O2;
	virtual ~CSSSO2();

	/* Luna: Alex Add Start 2010/11/23 */
	bool SSSO2_UpdateIMEIST(char* szServiceTag, char* szIMEI, char* szErrCode, char* szErrMsg);
	/* Luna: Alex Add End 2010/11/23 */

	/* Luna: Alex Add Start 2010/12/07 */
	bool SSSO2_QueryST(char *StationtState,
								char *pIMEI,
								char *szMSN,
								char *szBTAddr,
								char *szMACAddr,
								char *szSWver,
								char *szHWver,
								char *ErrCode, 
								char *ErrMsg,
								char *Status_Output
								);
	/* Luna: Alex Add End 2010/12/07 */

	/*Sunlight add for wolf query PVK£¬20110823 */
	bool SSSO2_QueryPVKforAcer(char * sz_IMEI,char * sz_PVK,char *ErrCode, char *ErrMsg);

	/*Sunlight add 20111208*/
	bool SSSO2_UpdateIMEIStatus_F_New(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg);
	bool SSSO2_UpdateIMEIStatus_I_New(char *pIMEI,char *szSWver,char *ErrCode, char *ErrMsg);  
	/*Sunlight add end*/

	/*Sunlight add 20120703*/
	bool CSSSO2::SSSO2_UpdateNAL(char *pIMEI,char *szNAL,char *ErrCode, char *ErrMsg);

	/*Sunlight add 20120706*/
    bool CSSSO2::SSSO2_UpdateCustomerSW(char *pIMEI,char *szModel,char * szPartNo, char * szCustsw,char *ErrCode, char *ErrMsg);
	bool CSSSO2::SSSO2_GetAliyunByIMEI(char *pIMEI,char *szUUID,char * ErrCode, char *ErrMsg);
    
	/*Sunlight add 20120827*/
	bool CSSSO2::SSSO2_CheckCustomerPN(char *pIMEI,char * ErrCode, char *ErrMsg);

	/*Sunlight add 20120924*/
	bool CSSSO2::SSSO2_UpdateUnlockCode(char *pIMEI,
		                                      char *szUnlockN,
		                                      char *szUnlockS,
		                                      char *szUnlockC,
		                                      char *szUnlockNS,
		                                      char *szUnlockSIM,
		                                      char *szUnlockUS,
											  char *szSPC,
		                                      char *szPhoneLock,
		                                      char * ErrCode, 
		                                      char *ErrMsg);
};



#endif // !defined(AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_)
