#include "stdafx.h"
#include "KeyPro.h"
#include "../../Lib/Rocky4ND/Ry4S.h"
#pragma comment(lib, "../../Lib/Rocky4ND/Ry4S.lib")

/*** for old dongle dll (Boston / detroit / ...)
#include "../../Lib/Rocky4ND/Rockey4_ND_32.h"
#pragma comment(lib, "../../Lib/Rocky4ND/Rockey4ND.lib")
***/

int CheckKeyPro()
{
	WORD handle, p1, p2, p3, p4, nRetCode;
	BYTE buffer[1024];
	DWORD lp1, lp2;

	/*** for old dongle dll (Boston / detroit / ...)
	p1 = 0x5c31;
	p2 = 0x5390;
	p3 = 0;
	p4 = 0;
	***/

	p1 = 0x9f31;
	p2 = 0x536f;
	p3 = 0x630b;
	p4 = 0x6f50;

	nRetCode = Rockey(RY_FIND, &handle, &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	TRACE(_T("RY_FIND:%d\n"), nRetCode);
	if (nRetCode != 0) {
		::AfxMessageBox(_T("Need Rocky key to use function"));
		return nRetCode;
	}
	nRetCode = Rockey(RY_OPEN, &handle, &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	TRACE(_T("RY_OPEN:%d\n"), nRetCode);
	if (nRetCode != 0) {
		::AfxMessageBox(_T("Need Rocky key to use function"));
		return nRetCode;
	}
	nRetCode = Rockey(RY_CLOSE, &handle, &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	TRACE(_T("RY_CLOSE:%d\n"), nRetCode);
	if (nRetCode != 0) {
		::AfxMessageBox(_T("Need Rocky key to use function"));
		return nRetCode;
	}

	return NO_ERROR;
}
