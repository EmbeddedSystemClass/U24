// QcomDiagModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QcomDiagModule.h"


CQcomDiagModule::CQcomDiagModule(int nCOMPort):CInterfaceBase(nCOMPort)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CQcomDiagModule::~CQcomDiagModule(void)
{
}



bool TransferCharToBool(const char* szBool)
{
	CStringA strb = szBool;
	strb.MakeLower();
	bool bRes = (strb == "true") ||  (strb == "t");
	return bRes;
}
//Transfer two char to byte and store it into ByteArray
bool HexStringToByteArray(const char *szHexData, BYTE *&ByteArray, size_t &Len)
{
	CStringA cstrHexData(szHexData);
	Len = 0;

	if(cstrHexData.IsEmpty())
	{
		return false;
	}

	//If not event, append '0'
	if(0 != cstrHexData.GetLength() % 2)
	{
		cstrHexData.AppendChar('0');
	}

	ByteArray = new BYTE [cstrHexData.GetLength() / 2];

	if(NULL != ByteArray)
	{
		::ZeroMemory(ByteArray, Len);
		Len = cstrHexData.GetLength() / 2;

		for(size_t i = 0;i < Len;i++)
		{
			CStringA cstrTwoChar(cstrHexData.Left(2));
			cstrHexData = cstrHexData.Right(cstrHexData.GetLength() - 2);
			ByteArray[i] = (BYTE) strtoul(cstrTwoChar.GetString(), NULL, 16);
		}

		return true;
	}

	else
	{
		return false;
	}
}

// caller takes the responsibility to prepare chString buffer
void ByteArrayToHexString (BYTE* pByte, char* chString, long ByteCount)
{
	char *pPtr = &chString[0];
	int count = 0;
	for (long i=0; i<ByteCount; ++i)
	{
		count += wsprintf (&pPtr[count], "%02X", pByte[i]);
	}

}