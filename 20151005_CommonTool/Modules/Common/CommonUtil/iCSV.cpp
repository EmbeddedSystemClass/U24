#include "stdafx.h"
#include "iCSV.h"


bool iCSV::GetCSVLineByKey (const TCHAR* szFilename, const TCHAR* szKey, const int iKeyIndex, TCHAR* szLine)
{
    CString    strLine, strSearchTarget ;
    CStdioFile fReadCSV; 

    if ( fReadCSV.Open(szFilename , CFile::modeRead | CFile::shareDenyNone, NULL) )
    {
        while (fReadCSV.ReadString(strLine))
        {
            AfxExtractSubString(strSearchTarget, strLine, iKeyIndex , _T(',')) ;
            
            if ( 0 == strSearchTarget.CompareNoCase(szKey) )
            {
                _tcscpy(szLine, strLine) ;
                fReadCSV.Close() ;
                return true ;
            }
        }

        fReadCSV.Close() ;
    }

    return false ;
}


bool iCSV::GetCSVItemByKey (const TCHAR* szFilename, const TCHAR* szKey, const int iKeyIndex, TCHAR* szResult, const int iTargetKey)
{
    CString    strLine, strSearchTarget, strSearchResult ;
    CStdioFile fReadCSV; 

    if ( fReadCSV.Open(szFilename , CFile::modeRead | CFile::shareDenyNone, NULL) )
    {
        while (fReadCSV.ReadString(strLine))
        {
            AfxExtractSubString(strSearchTarget, strLine, iKeyIndex , _T(',')) ;

            if ( 0 == strSearchTarget.CompareNoCase(szKey) )
            {
                AfxExtractSubString(strSearchResult, strLine, iTargetKey , _T(',')) ;
                _tcscpy(szResult, strSearchResult) ;

                fReadCSV.Close() ;
                return true ;
            }
        }

        fReadCSV.Close() ;
    }

    return false ;
}