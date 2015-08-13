#pragma once
#include "MD5ChecksumDefines.h"
#include "../XML/MyMSXML.h"
#include <map>

class CMD5Checksum
{
public:
	CMD5Checksum(void);
	~CMD5Checksum(void);
public:
	//interface functions for the RSA MD5 calculation
	CString GetMD5(BYTE* pBuf, UINT nLength);
	CString GetMD5(CFile& File);
	CString GetMD5(const CString& strFilePath);
	bool    CheckDirMD5(const CString& strXMLPath, CString strDir, CString& strOutMsg);

protected:
	//RSA MD5 implementation
	void Transform(BYTE Block[64]);
	void Update(BYTE* Input, ULONG nInputLen);
	CString Final();
	inline DWORD RotateLeft(DWORD x, int n);
	inline void FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);

	//utility functions
	void DWordToByte(BYTE* Output, DWORD* Input, UINT nLength);
	void ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength);

private:
	BYTE  m_lpszBuffer[64];  //input buffer
	ULONG m_nCount[2];   //number of bits, modulo 2^64 (lsb first)
	ULONG m_lMD5[4];   //MD5 checksum
	std::map<CString, CString> m_map_md5; 

};
