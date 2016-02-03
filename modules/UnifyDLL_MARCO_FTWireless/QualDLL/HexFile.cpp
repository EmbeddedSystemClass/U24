#include "StdAfx.h"
#include "HexFile.h"

CHexFile::CHexFile()
{
	m_address[0] = 0;
	m_address[1] = 1;
	m_nLength = 0;
	m_nPosition = 0;
}

CHexFile::~CHexFile()
{
}

BOOL CHexFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException * pError)
{
	if (!CFile::Open(lpszFileName, nOpenFlags, pError)) {
		return FALSE;
	}
	char* cbuffer = new char[18];
	ZeroMemory(cbuffer, 18);
	UINT nBytesRead = CFile::Read(cbuffer, 17);
	if (nBytesRead < 15) {
		delete [] cbuffer;
		return FALSE;
	}
	// m_address store the first address of Flash to write
	m_address[0] = TwoAsciiToByte(cbuffer + 9);
	m_address[1] = TwoAsciiToByte(cbuffer + 11);
	delete [] cbuffer;
	return TRUE;
}

UINT CHexFile::Read(char* pBuf, UINT nCount)
{
	char* cbuffer = new char[100];
	UINT nLocation = 0;
	UINT nBytesRead = 0;
	while (nLocation < nCount) { // nCount = 1017
		ZeroMemory(cbuffer, 100);
		if (m_nPosition != 0) {
			if (nCount >= (m_nLength - m_nPosition)) {  // Process not finish data in the same line
				nBytesRead = CFile::Read(cbuffer, (m_nLength - m_nPosition) * 2 + 4);
				//m_nlength=0;
				m_nPosition = 0;
				for (UINT i = 0;i < (nBytesRead - 4) / 2;i++) {
					pBuf[nLocation] = TwoAsciiToByte(cbuffer + i * 2);
					nLocation++;
				}
			}
			else {
				nBytesRead = CFile::Read(cbuffer, nCount * 2);
				m_nPosition += nBytesRead;
				for (UINT i = 0; i < (nBytesRead / 2); i++) {
					*(pBuf + nLocation) = TwoAsciiToByte(cbuffer + i * 2);
					nLocation++;
				}
			}
		}
		else {
			nBytesRead = CFile::Read(cbuffer, 3);
			if (nBytesRead < 3) {
				delete [] cbuffer;
				return 0;
			}
			BYTE nLength = TwoAsciiToByte(cbuffer + 1);
			if (nLength == 0) {
				break;
			}
			else if (nLength == 2) {
				nBytesRead = CFile::Read(cbuffer, 6 + nLength * 2 + 4);
				memset(cbuffer, 0, 100);
			}
			else if (nLength != 2) { // for test
				if (nLength + nLocation <= nCount) {
					nBytesRead = CFile::Read(cbuffer, 6 + nLength * 2 + 4);  // Reading one line except :10
					for (UINT i = 0; i < nLength; i++) {
						*(pBuf + nLocation) = TwoAsciiToByte(cbuffer + i * 2 + 6);
						nLocation++;
					}
					m_nLength = nLength;
					m_nPosition = 0;
				}
				else {
					nBytesRead = CFile::Read(cbuffer, 6 + (nCount - nLocation) * 2);
					UINT untmep = nCount - nLocation;
					for (UINT i = 0;i < untmep;i++) {
						*(pBuf + nLocation) = TwoAsciiToByte(cbuffer + i * 2 + 6);
						nLocation++;
					}
					m_nLength = nLength;
					m_nPosition = untmep;
				}
			}
		}
		if (m_nLength < 0x10) {
			break;
		}
	}
	delete [] cbuffer;
	return nLocation;
}

BYTE CHexFile::TwoAsciiToByte(char * pcascii)
{
	const UINT nmask = 0x5F; // 0101 1111
	BYTE nValue = 0;
	UINT ch = 0;
	for (int i = 0; i < 2; i++) {
		ch = pcascii[i];
		if ('a' <= ch && ch <= 'f') {
			ch = ch & nmask; // convert to upper case
		}
		int nPower = (i == 0) ? 16 : 1; // hex
		if ('A' <= ch && ch <= 'F') {
			nValue = nValue + (ch - 'A' + 10) * nPower;
		}
		else if ('0' <= ch && ch <= '9') {
			nValue = nValue + (ch - '0') * nPower;
		}
	}
	return nValue;
}

WORD CHexFile::GetHexAddrss(void)
{
	return m_address[1] + m_address[0]*256;
}
