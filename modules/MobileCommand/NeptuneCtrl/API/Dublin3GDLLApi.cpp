#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "Dublin3GDLLApi.h"
#include "io.h"

static const int CMD_MAX_RETRY = 1;

static int MobileCmd(int nPort, int nTimeout, size_t nReqBody,BYTE* ReqBody,
					  size_t nPeerBuffer, BYTE* PeerBuffer,BYTE* OutConfirm, BYTE* OutPeerData,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++) {
		TRACE(_T("MTKCmdInOut\n"));
		nRetCode = MTKCmdInOut(nPort, nTimeout, nReqBody, ReqBody, nPeerBuffer ,PeerBuffer, OutConfirm, OutPeerData);
		/*if (nTry == 0 && nRetCode != HS_OK) {
			strncat(OutBuffer, error_code, strlen(error_code));
		}*/
		TRACE(_T("%s(%d)\n"), Func, nRetCode);
		if (nRetCode == HS_OK)
		{
			break;
		}
		else 
		{
			Sleep(500);
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall WIFI_Read_MCR(int nPort, int nTimeout,BYTE* OutConfirm, BYTE* OutPeerData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	BYTE ReqBody[] = {0x13,0x00,0x3b,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	BYTE PeerBuffer[] = {0x10,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0xc8,0xa0,0xff,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	return MobileCmd(nPort, nTimeout, sizeof(ReqBody), ReqBody, sizeof(PeerBuffer) ,PeerBuffer, OutConfirm, OutPeerData, error_code, __FUNCTION__);
}

