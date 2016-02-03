#pragma once

// TODO: check this class
class CHexFile : public CFile
{
public:
	CHexFile();
	~CHexFile();

private:
	UINT m_nLength;
	UINT m_nPosition;
	BYTE m_address[2];
	BYTE TwoAsciiToByte(char* pcascii);

public:
	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException * pError= NULL);
	UINT Read(char* pBuf, UINT nCount);
	WORD GetAddress();
	WORD GetHexAddrss();
};
