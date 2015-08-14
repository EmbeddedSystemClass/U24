// CMDModuleExport.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CMDModuleExport.h"

#include "..\..\Common\CommonUtil\UnicodeUtility.h"
#include "RequestUtility\Fastmd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

namespace CommonFunction
{
	bool HexStringToByteArray(const char* szHexData, BYTE*& ByteArray, size_t& Len)
	{
		CStringA cstrHexData(szHexData);
		Len = 0;

		if(cstrHexData.IsEmpty())
		{
			return false;
		}

		// If not event, append '0'
		if (0 != cstrHexData.GetLength() % 2)
		{
			cstrHexData.AppendChar('0');
		}

		ByteArray = new BYTE [cstrHexData.GetLength() / 2];

		if (NULL != ByteArray)
		{
			::ZeroMemory(ByteArray, Len);
			Len = cstrHexData.GetLength() / 2;

			for(size_t i = 0;i < Len;i++)
			{
				CStringA cstrTwoChar(cstrHexData.Left(2));
				cstrHexData = cstrHexData.Right(cstrHexData.GetLength() - 2);
				ByteArray[i] = (BYTE)strtoul(cstrTwoChar.GetString(), NULL, 16);
			}

			return true;
		}

		else
		{
			return false;
		}
	}

	void ByteArrayToHexString (BYTE* pByte, char* chString, long ByteCount)
	{
		char *pPtr = &chString[0];
		int count = 0;
		for (long i = 0; i < ByteCount; ++i)
		{
			count += wsprintf(&pPtr[count], "%02X", pByte[i]);
		}
	}

	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
	{
			int nRet = 0;
			if (pucHexBcdBuf && (iHexBcdBufSize > 0) && sDigitBuf && (iDigitBufSize>0) && iHexBcdBufSize*2 <= iDigitBufSize)
			{
				BYTE ucLowByte, ucHighByte;
				int i = 0;
				for (i = 0; i < iHexBcdBufSize; i++)
				{
					ucLowByte = *(pucHexBcdBuf+i)&0x0F;
					*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');

					ucHighByte = (*(pucHexBcdBuf+i)&0xF0)>>4;
					*(sDigitBuf+(2*i+1)) = (ucHighByte>=10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
				} 

				if (iHexBcdBufSize%2)
				{
					ucLowByte = *(pucHexBcdBuf+i)&0x0F;
					*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
				}
				nRet = (iHexBcdBufSize%2) ? (2*i+1) : (2*i);
			}
			return nRet;
	}

	/*
	int DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize)
	{
		int nRet = 0;
		if(sDigitBuf && pucHexBcdBuf && iHexBcdBufSize >= (iDigitBufSize-1)/2)
		{
			for(int i=0; i< (iDigitBufSize-1)/2; i++)
			{
				TCHAR szTemp[3] = {0};
				szTemp[0] = sDigitBuf[i*2];
				szTemp[1] = sDigitBuf[i*2+1];
				pucHexBcdBuf[(iDigitBufSize-1)/2 -i -1] = (BYTE)QSYNC_LIB::HEX2Ineger(szTemp);
			}
		}
		return nRet;
	}
	*/

	bool Common_FindKeyword (char* szTargetString, char* szKeyword)
	{
		std::string strTarget = szTargetString;
		std::string strKeyword = szKeyword;
		int pos =0;

		pos = strTarget.find(strKeyword.c_str());
		if(pos != -1)
			return true;

		return false;
	}

	std::string FileDigest(const std::string& file) 
	{

		ifstream in(file.c_str(), ios::binary);
		if (!in) {
			return "";
		}

		MD5 md5;
		std::streamsize length;
		char buffer[1024];
		while (!in.eof()) {
			in.read(buffer, 1024);
			length = in.gcount();
			if (length > 0) {
				md5.update(buffer, length);
			}
		}
		in.close();
		return md5.toString();
	}

	CMDMODULEEXPORT_API bool GetMD5_SPU(char *szFilePath, char *szMD5) 
	{
		if(!PathFileExists(szFilePath))
			return false;

		string MD5String = FileDigest(szFilePath);

		if(!MD5String.empty())
		{
			strcpy(szMD5, MD5String.c_str());
			return true;
		}

		return false;
	}
}