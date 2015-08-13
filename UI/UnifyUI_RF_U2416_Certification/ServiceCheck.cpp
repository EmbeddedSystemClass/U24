
#include "stdafx.h"
#include "ServiceCheck.h"
#include "tripledes\tripledes.h"

int service_key_check(char* filename)
{
	int nRetCode = ERROR_INVALID_ACCESS;
	const unsigned __int64 LIMIT_DATE  = 60*24*60*60; // 60 day to sec
	const unsigned __int64 TICK_SECOND = 10000000; // 100 ns * 10,000,000 => 1 sec

	PDECRYPTBUFFER DecryptBuffer;
	PFREEBUFFER FreeBuffer;

	HMODULE hLib = ::LoadLibrary(_T("tripledes.dll"));
	if(!hLib){
		::AfxMessageBox(_T("Load tripledes fail\r\n"));	
		return 1;
	}
	DecryptBuffer = (PDECRYPTBUFFER) ::GetProcAddress(hLib,"DecryptBuffer");
	FreeBuffer = (PFREEBUFFER) ::GetProcAddress(hLib,"FreeBuffer");

	const char * key = "{39823A55-8EAB-4225-9DB8-00105111774C}";
	UINT nSize = 0;
	BYTE * buf = NULL;

	CString keyfile = CA2T(filename);
	CFile cf;
	if (cf.Open(keyfile, CFile::modeRead | CFile::shareDenyWrite)) {
		nSize = static_cast<UINT>(cf.GetLength());
		buf = new BYTE[nSize];
		cf.Read(buf, nSize);
		cf.Close();
	}
	FILETIME time_key = {0};
	BYTE* decripted = DecryptBuffer(buf,nSize,key);
	if(decripted) {
		memcpy(&time_key, &decripted[214], 8);
		FreeBuffer(decripted);
	} // if(decripted)
	FILETIME time_sys;
	GetSystemTimeAsFileTime(&time_sys);

	ULARGE_INTEGER check_key;
	ULARGE_INTEGER check_sys;
	ULARGE_INTEGER diff;
	check_key.HighPart = time_key.dwHighDateTime;
	check_key.LowPart  = time_key.dwLowDateTime;
	check_sys.HighPart = time_sys.dwHighDateTime;
	check_sys.LowPart  = time_sys.dwLowDateTime;
	diff.QuadPart = (check_sys.QuadPart - check_key.QuadPart) / TICK_SECOND;
	if (diff.QuadPart > LIMIT_DATE) {
		nRetCode = ERROR_TIMEOUT;
	}
	else {
		nRetCode = NO_ERROR;
	}

	return nRetCode;
}

