#include "stdafx.h"
#include "CommDef.h"

CString ConvertString(const char* sz_buffer)
{
	/* Check input */
	if (*sz_buffer == NULL)
	{
		return _T("");
	}

	/* Convert char* to CString */
	CString str_temp;
	int i_len = ::MultiByteToWideChar(CP_ACP, 0, sz_buffer, -1, NULL, 0);

	TCHAR *sz_buf = new TCHAR[i_len + 2];
	::MultiByteToWideChar(CP_ACP, 0, sz_buffer, -1, sz_buf, i_len);
	str_temp.Format(_T("%s"), sz_buf);
	delete[] sz_buf;

	return str_temp;
}


void CStringToChar( CString str_In, char *sz_Out )
{
	::WideCharToMultiByte(CP_ACP, 0, str_In, -1, sz_Out, str_In.GetLength() * 2, NULL, NULL);
}
