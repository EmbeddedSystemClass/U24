#include "stdafx.h"
#include "Utility.h"

bool PrintDiagCmd(const BYTE* data, unsigned int iSize, char* szResult, char* szDelimiter)
{
    char szTmp[1024] ;
    ::ZeroMemory(szTmp, 1024) ;

    sprintf_s(szTmp, 1024, _T("0x%02X"), (const BYTE)* data) ;
    for (unsigned int i = 1; i < iSize; ++i )
        sprintf_s(szTmp, 1024,  _T("%s%s0x%02X"), szTmp, szDelimiter, (const BYTE)* (data+i) ) ;

    strcpy_s(szResult, 1024, szTmp) ;

    return true ;
}