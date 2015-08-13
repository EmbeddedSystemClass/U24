/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *   This is a part of the Qisda RF Calibration and Test library.
 *
 *   This document contains propriety information, and except with written
 *   permission of Qisda Corp., such information shall not be 
 *   published, or disclosed to others, or used for any purpose, and the   
 *   document shall not be duplicated in whole or in part.  
 *
 *   Copyright (C) 2009 Qisda Corporation
 *   All Rights Reserved.
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <locale.h>
#include <locale>

#include "..\..\..\Modules\Common\CommonUtil\CommonUtil.h"


///////////////////////////////////////////////////////////////////////////////////////////
//
//	CStr
//
///////////////////////////////////////////////////////////////////////////////////////////

//char* CStr::m_pszLocale = ::setlocale(LC_ALL, "chinese");
//locale CStr::m_loc = locale("chinese");
char* CStr::m_pszLocale = ::setlocale(LC_ALL, "english");
locale CStr::m_loc = locale("english");


bool CStr::LoadFormFile(const String &strFileName, StrVtr &vLine, bool fSkipEmptyLine)
{
	CReadFile cInputFile;
	if(!cInputFile.Open(strFileName)) { return false;}
	String strLine;
	while(cInputFile.ReadLine(strLine)) {
		if(fSkipEmptyLine && strLine.empty()) {continue;}
		vLine.push_back(strLine);
	}
	return true;
}

bool CStr::SaveToFile(const String &strFileName, StrVtr &vLine)
{
	CWriteFile cOutputFile;
	if(!cOutputFile.Open(strFileName)){return false;}
	for(ConstStrVtrItr itrLine = vLine.begin(); itrLine != vLine.end(); ++itrLine){
		cOutputFile.WriteLine(*itrLine);
	}
	return true;

}

void CStr::ToUpper(String &str)
{
	for(size_t stChar = 0; stChar < str.size(); ++stChar){
		str[stChar] = toupper(str[stChar], m_loc);
	}
}

void CStr::ToLower(String &str)
{
	for(size_t stChar = 0; stChar < str.size(); ++stChar){
		str[stChar] = tolower(str[stChar], m_loc);
	}
}

String& CStr::TrimStr(String &str, const String &strTrim)
{
	while(!str.empty()){
		bool fNoTrim = true;
		for(StrSizeT x = 0; x < strTrim.size(); ++x) {
			if(str[0] == strTrim[x]){
				fNoTrim = false;			
				str.erase(str.begin());
			}
		}
		if(fNoTrim) {break;}
	}

	while(!str.empty()){
		bool fNoTrim = true;
		for(StrSizeT x = 0; x < strTrim.size(); ++x) {
			if(str[str.size() - 1] == strTrim[x]){
				fNoTrim = false;			
				str.erase(str.end() - 1);
			}
		}
		if(fNoTrim) {break;}
	}

	return str;	
}


int CStr::ReplaceString(String &strSource, const String &strFind, const String &strReplace)
{
	StrSizeT stReplace = 0;
	StrSizeT stCurrPos = static_cast<StrSizeT>(-1);
	const StrSizeT stReplaceLen = strFind.size();

	while(true){
		stCurrPos = strSource.find(strFind, stCurrPos);
		if(NPOS(stCurrPos)){ break;}
		strSource.replace(stCurrPos, stReplaceLen, strReplace);
		stCurrPos += strReplace.size();
		++stReplace;
	}
	return static_cast<int>(stReplace);
}

bool CStr::ParseStringAll(const String &strSource, const String &strDelimiter, StrVtr &vList, bool fAppendDelimiter)
{
	bool fRet = true;
	StrSizeT stCurrPos				= 0;
	StrSizeT stFirstPos				= String::npos;
	StrSizeT stLastDelimiter		= String::npos;
	const StrSizeT stTotalLen		= strSource.size();
	const StrSizeT stDelimiterLen	= strDelimiter.size();
	StrSizeT stBufferLen			= 80;

	vList.clear();

	while(stCurrPos < stTotalLen){
		size_t x = 0;
		for(; x < stDelimiterLen; ++x) {
			StrSizeT stFind = strSource.find(strDelimiter[x], stCurrPos);
			if(NPOS(stFind)) {				
			}else {
				if(stFind <= stFirstPos)
					stFirstPos = stFind;
			}
		}
		if(NPOS(stFirstPos)){
			if(stTotalLen - stCurrPos > stBufferLen){
				StrSizeT stNum = (stTotalLen - stCurrPos) / stBufferLen;
				StrSizeT stPrevPos = stCurrPos;
				StrSizeT stMidPos = stCurrPos;
				for(StrSizeT ix = 0; ix <= stNum; ++ix) {
					stMidPos = stPrevPos + stBufferLen;
					vList.push_back(strSource.substr(stPrevPos, stMidPos - stPrevPos));
					stPrevPos = stMidPos;
				}
			}
			else
				vList.push_back(strSource.substr(stCurrPos, stTotalLen - stCurrPos));
			break;
		} else{
			stLastDelimiter = stFirstPos;
			size_t stStrLen = stFirstPos - stCurrPos;
			if(fAppendDelimiter){
				++stStrLen;
			}
			if(stStrLen > stBufferLen){
				StrSizeT stNum = stStrLen / stBufferLen;
				StrSizeT stPrevPos = stCurrPos;
				StrSizeT stMidPos = stCurrPos;
				for(StrSizeT ix = 0; ix <= stNum; ++ix) {
					stMidPos = stPrevPos + stBufferLen;
					if(stMidPos > stStrLen + stCurrPos)
						stMidPos = stStrLen + stCurrPos;
					vList.push_back(strSource.substr(stPrevPos, stMidPos - stPrevPos));
					stPrevPos = stMidPos;
				}			
			}else
				vList.push_back(strSource.substr(stCurrPos, stStrLen));
			stCurrPos = stFirstPos + 1;

			if(x >= stDelimiterLen){
				stFirstPos = String::npos;
				continue;
			}
		}

	}
	return fRet;
}

bool CStr::ParseString(const String &strSource, const String &strDelimiter, StrVtr &vList, bool fAppendDelimiter)
{
	bool fRet = true;
	StrSizeT stCurrPos				= 0;
	StrSizeT stLastDelimiter		= String::npos;
	const StrSizeT stTotalLen		= strSource.size();
	const StrSizeT stDelimiterLen	= strDelimiter.size();

	vList.clear();

	while(stCurrPos < stTotalLen){

		size_t x = 0;
		for(; x < stDelimiterLen; ++x){
			StrSizeT stFind = strSource.find(strDelimiter[x], stCurrPos);
			if(NPOS(stFind)){
			}else{
				stLastDelimiter = stFind;

				size_t stStrLen = stFind - stCurrPos;
				if(fAppendDelimiter){
					++stStrLen;
				}
				vList.push_back(strSource.substr(stCurrPos, stStrLen));
				stCurrPos = stFind + 1;
				break;
			}
		}
		if(x >= stDelimiterLen){
			break;
		}
	}
	if(stLastDelimiter + 1 < stTotalLen){
		vList.push_back(strSource.substr(stLastDelimiter + 1, stTotalLen - stLastDelimiter - 1));
	}

	return fRet;
}

size_t CStr::GetPatternCount(const String &strToCount, const String &strPattern)
{
	size_t stCount = 0;
	size_t stFindPos = 0;
	while(!NPOS(stFindPos = strToCount.find(strPattern, stFindPos))){
		++stCount;
		stFindPos += strPattern.size();
	}
	return stCount;
}


void CStr::ComposeString(const StrVtr& vList, const String& strDelimiter, String& strResult, bool fAppendEnd)
{
	typedef StrVtr::size_type StrVtrSizeT;
	strResult.erase(strResult.begin(), strResult.end());

	if(vList.empty()) {return;}

	StrVtrSizeT x = 0, stSize = vList.size() - 1;
	for(; x < stSize; ++x){
		strResult += vList[x];
		strResult += strDelimiter;
	}

	strResult += vList[x];

	if(fAppendEnd){ strResult += strDelimiter;}
}

void CStr::SplitWordVector(const String& strWord, size_t stTokenLen, StrStrVtr& vSplitWord)
{
	DoSplitWordVector(strWord.c_str(), false, stTokenLen, vSplitWord);

	StrStrVtrItr new_end = unique(vSplitWord.begin(), vSplitWord.end());
	--new_end;
	vSplitWord.erase(new_end, vSplitWord.end());
}

void CStr::DoSplitWordVector(const _TCHAR* pwsWord, bool fTailWord, size_t stTokenLen, StrStrVtr& vSplitWord)
{
	size_t stTotalLen = _tcsclen(pwsWord);

	if(stTotalLen == 0){
	} else if(stTotalLen <= stTokenLen){
		StrVtr vStr;
		vStr.push_back(String(pwsWord, stTotalLen));
		vSplitWord.push_back(vStr);	
	} else{
		size_t stBegin;
		if(fTailWord){
			stBegin = 0;
		} else{
			stBegin = stTokenLen;		
		}
		for(size_t stLen = stBegin; stLen <= stTotalLen; stLen += stTokenLen){
			StrStrVtr vTempSplitWord;
			if(stLen < stTotalLen){
				DoSplitWordVector(pwsWord + stLen, !fTailWord, stTokenLen, vTempSplitWord);
				if(stLen > 0){
					for(size_t x = 0; x < vTempSplitWord.size(); ++x){
						vTempSplitWord[x].insert(vTempSplitWord[x].begin(), String(pwsWord, stLen));
					}
				}		
			} else {
				vTempSplitWord.resize(1);
				vTempSplitWord.front().push_back(String(pwsWord, stLen));
			}
			vSplitWord.insert(vSplitWord.end(), vTempSplitWord.begin(), vTempSplitWord.end());
			if(fTailWord && stLen == 0) {
				stLen += stTokenLen;
			}
		}	
	}
}


////////////////////////////////////////////////////////////////////////////////
//
//  CFileBase
//
///////////////////////////////////////////////////////////////////////////////

#define CH_CARRIAGE_RETURN              '\r'
#define CH_NEW_LINE                     '\n'
#define WC_CARRIAGE_RETURN              L'\r'
#define WC_NEW_LINE                     L'\n'
#define TC_CARRIAGE_RETURN              _T('\r')
#define TC_NEW_LINE                     _T('\n')
#define W_UNICODE_HEADER                0xfeff
#define W_UTF8_HEADER					0xefbbbf


////////////////////////////////////////////////////////////////////////////////
//
//  CReadFile
//
///////////////////////////////////////////////////////////////////////////////

CReadFile::CReadFile(
					 bool fTextMode) :
m_fTextMode(fTextMode), 
m_fUnicode(false),
m_hFileMap(NULL),
m_pViewOfFileMap(NULL)
{
}


CReadFile::~CReadFile()
{
	Close();
}

bool CReadFile::Open(const String &strFileName)
{
	Close();	
	if(!(m_pFile = MyOpenFile(strFileName, "rb"))) {return false;}

	if(m_fTextMode){
		unsigned short wHeader;
		if(Read(&wHeader, sizeof(wHeader))){
			m_fUnicode = (W_UNICODE_HEADER == wHeader);
			if(!m_fUnicode) {Rewind(); }
		}
	}
	return true;
}

bool CReadFile::Close(void)
{
	if(NULL != m_pFile) {::fclose(m_pFile); m_pFile = NULL;}
	return true;
}

bool CReadFile::Rewind(void)
{
	if(!IsOpen()) {return false;}

	MoveFileBegin();
	return true;
}

bool CReadFile::Read(void *pvBuff, size_t stSize)
{
	if(!IsOpen()) {return false;}
	if(!(pvBuff && stSize > 0))	 {return false;}
	if(1 != ::fread(pvBuff, stSize, 1, m_pFile)) {return false;}
	return true;
}

bool CReadFile::ReadLine(String &strLine)
{
	bool fRet = false;
	strLine.erase(strLine.begin(), strLine.end());
	if(m_fUnicode && m_fTextMode){
		_TCHAR tch;
		while(Read(&tch, sizeof(tch))){
			fRet = true;
			if(TC_CARRIAGE_RETURN == tch ){continue;}
			if(TC_NEW_LINE == tch){break;}
			strLine += tch;
		}
	} else{
		char ch;
		string astrLine;
		while(Read(&ch, sizeof(ch))){
			fRet = true;
			if(CH_CARRIAGE_RETURN == ch){continue;}
			if(CH_NEW_LINE == ch){break;}
			astrLine += ch;
		}
		if(!astrLine.empty()){
#ifdef UNICODE
			strLine = CStr::ToWideString(astrLine);		
#else
			strLine = astrLine;		
#endif
		}
	}
	return fRet;
}


////////////////////////////////////////////////////////////////////////////////
//
//  CWriteFile
//
///////////////////////////////////////////////////////////////////////////////

CWriteFile::CWriteFile(bool fAppend, bool fTryStdout)
{
	SetAppend(fAppend);
	m_fTryStdout = fTryStdout;
}

CWriteFile::~CWriteFile()
{
	Close();
}

bool CWriteFile::Open(const String &strFileName)
{
	Close();

	if(strFileName.empty()){
		if(m_fTryStdout){
			m_pFile = stdout; return true;
		}
	}
	m_pFile = MyOpenFile(strFileName, m_fAppend ? "ab" : "wb");
	if(!m_pFile) {return false;}

	return true;		
}

bool CWriteFile::Close()
{
	if(stdout == m_pFile) {return true;}
	if(NULL != m_pFile)	{::fclose(m_pFile); m_pFile = NULL;}
	return true;
}

void CWriteFile::SetAppend(bool fAppend)
{
	m_fAppend = fAppend;
}


bool CWriteFile::WriteUnicodeHeader()
{
	static unsigned short wHeader = W_UNICODE_HEADER;

	return Write(&wHeader, sizeof(wHeader));
}

bool CWriteFile::Write(const void *pvBuff, size_t stSize)
{
	if(!IsOpen()) {return false;}
	if(!(pvBuff && stSize > 0)) {return false;}
	if(1 != ::fwrite(pvBuff, stSize, 1, m_pFile) ){return false;}

	return true;
}

bool CWriteFile::Write(const String &str)
{
	return Write(str.c_str(), sizeof(str[0]) * str.size());
}

bool CWriteFile::WriteLine(const String &strLine)
{
	if(!Write(strLine)) { return false;}
	if(!WriteNewLine()) { return false;}
	return true;
}

bool CWriteFile::WriteNewLine()
{
	bool fRet = true;
	static _TCHAR wchCR = WC_CARRIAGE_RETURN;
	static _TCHAR wchLF = WC_NEW_LINE;

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
	if (!Write(&wchCR, sizeof(wchCR))) { goto _exit; }
#endif
	if (!Write(&wchLF, sizeof(wchLF))) { goto _exit; }

	fRet = true;

_exit:
	return fRet;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//	FileMap
//
///////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Windows version
#ifdef __FILE_MAP_WINDOWS_VERSION

CFileMap::CFileMap():
m_hFile(INVALID_HANDLE_VALUE),
m_hMapping(NULL),
m_pvData(NULL)
{}

CFileMap::~CFileMap()
{
	Release();
}

bool CFileMap::Open(const String& strFileName)
{
	Release();
	m_hFile = ::CreateFile(strFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == m_hFile)
	{return false;}
	m_hMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(!m_hMapping)
	{return false;}

	return true;
}

long CFileMap::GetSize(void)
{
	DWORD dwSize = 0;
	dwSize = ::GetFileSize(m_hFile);
	return static_cast<long> (dwSize);
}

const void* CFileMap::ReadData(void)
{
	if (!m_pvData)
	{
		if (m_hMapping)
		{
			m_pvData = ::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0);
		}
	}
	return m_pvData;
}

void CFileMap::Release()
{
	if (m_pvData)
	{::UnmapViewOfFile(m_pvData); m_pvData = NULL;}
	if (m_hMapping)
	{::CloseHandle(m_hMapping); m_hMapping = NULL;}
	if (m_hFile)
	{::CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE;}
}

#else //none windows version

CFileMap::CFileMap():
m_pFile(NULL),
m_pvData(NULL)
{}

CFileMap::~CFileMap()
{
	Release();
}

bool CFileMap::Open(const String& strFileName)
{
	Release();
	m_pFile = CFileBase::MyOpenFile(strFileName, "rb");
	if(!m_pFile)
	{return false;}
	return true;	
}

long CFileMap::GetSize(void)
{
	long lnCurrPos = 0;
	long lnSize = 0;
	if(!m_pFile)
	{return 0;}
	lnCurrPos = ::ftell(m_pFile);
	if (0 != ::fseek(m_pFile, 0, SEEK_END))
	{return 0;}
	lnSize = ::ftell(m_pFile);
	::fseek(m_pFile, lnCurrPos, SEEK_SET);	
	return lnSize;
}

const void* CFileMap::ReadData(void)
{
	if (m_pvData)
	{return m_pvData;}
	//Try to load data
	if (!m_pFile)
	{return NULL;}
	long lnSize = GetSize();
	if (-1 == lnSize)
	{return NULL;}
	m_pvData = new BYTE[lnSize];
	if (!m_pvData)
	{return NULL;}
	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{return NULL;}
	if(1 != fread(m_pvData, static_cast<size_t>(lnSize), 1, m_pFile))
	{return NULL;}
	return m_pvData;
}

void CFileMap::Release(void)
{
	if(m_pvData)
	{ delete [] static_cast<BYTE*>(m_pvData); m_pvData = NULL;}
	if (m_pFile)
	{ ::fclose(m_pFile); m_pFile = NULL;}
}
#endif  //  __FILE_MAP_WINDOWS_VERSION


//---------------------------------------------------------------------------
//  ChineseCodeConverter
bool ChineseCodeConverter::InitData(
									const String& strDataFileName)
{
	CFileMap cFileMap;
	if (!cFileMap.Open(strDataFileName))
	{ return false;}
	if(sizeof(m_arCodeTable) != cFileMap.GetSize())
	{return false;}
	memcpy(m_arCodeTable, cFileMap.ReadData(), sizeof(m_arCodeTable));
	return true;
}

void ChineseCodeConverter::Convert(
								   const string&		strInput, 
								   string&				strOutput)
{
	size_t stLen = strInput.length();
	strOutput.resize(stLen);
	strOutput.erase(strOutput.begin(), strOutput.end());
	for (size_t x = 0; x <stLen; ++x){
		size_t stIndex = (static_cast<BYTE>(strInput[x]) << 8) + static_cast<BYTE>(strInput[x + 1]);
		strOutput[x] = m_arCodeTable[stIndex * 2];
		strOutput[x + 1] = m_arCodeTable[stIndex * 2 + 1];
	}
}


//---------------------------------------------------------------------------
//  CHTCharSet
bool CHTCharSet::IsChinesePunctuation(const _TCHAR wch)
{
	//        ¡B                   ¡²
	return (0x3001 <= wch && wch <= 0x3003)
		//       ¡q                    ¡j
		|| (0x3008 <= wch && wch <= 0x3011)
		//       ¡e                    R
		|| (0x3014 <= wch && wch <= 0x3017)
		//        ¡¦                       ¡^
		|| (0xFF07 <= wch && wch <= 0xFF09)
		|| (0xFF0C == wch)  //  ¡A
		|| (0xFF0E == wch)  //  ¡D
		|| (0xFF1A == wch)  //  ¡G
		|| (0xFF1B == wch)  //  ¡F
		|| (0xFF1F == wch)  //  ¡H
		|| (0xFF5E == wch)  //  ¡ã
		;

}



bool CHTCharSet::IsChineseSymbol(const _TCHAR wch)
{
	//        ¡B                   ¡ª
	return (0x3001 <= wch && wch <= 0x301E)
		|| (0xFE50 <= wch && wch <= 0xFE6B)     //  Small Form Variants
		|| (0xFE30 <= wch && wch <= 0xFE4F)     //  CJK Compatibility Forms
		|| IsChinesePunctuation(wch);
}

bool CHTCharSet::IsChineseChar(const _TCHAR wch)
{
	//        ¤@                   ýÔ
	return (0x4E00 <= wch && wch <= 0x9FA5)
		|| (0xF900 <= wch && wch <= 0xFA2D)     //  CJK Compatibility Ideographs
		|| (0xFA30 <= wch && wch <= 0xFA6A)     //  CJK Compatibility Ideographs
		;
}

bool CHTCharSet::IsEnglishAlphabet(const _TCHAR wch)
{
	//        ¢Ï                   ¢è
	return (0xFF21 <= wch && wch <= 0xFF3A)
		//    ¢é                   £C
		|| (0xFF41 <= wch && wch <= 0xFF5A)
		;
}

bool CHTCharSet::IsEnglishNumeric(const _TCHAR wch)
{
	//        ¢¯                   ¢¸
	return (0xFF10 <= wch && wch <= 0xFF19);
}

bool CHTCharSet::IsEnglishSymbol(const _TCHAR wch)
{
	//       ¡I                    ¡E
	return (0xFF01 <= wch && wch <= 0xFF65);
}

bool CHTCharSet::IsHalfShapeAlphabet(const _TCHAR wch)
{
	//           A                       Z
	return (0x0041 <= wch && wch <= 0x005A)
		//       a                       z
		|| (0x0061 <= wch && wch <= 0x007A)
		;
}

bool CHTCharSet::IsHalfShapeNumeric(const _TCHAR wch)
{
	//           0                       9
	return (0x0030 <= wch && wch <= 0x0039);
}

bool CHTCharSet::IsHalfShapeSymbol(const _TCHAR wch)
{
	return wch != L' ' && (0x0021 <= wch && wch <= 0x007E);
}

bool CHTCharSet::IsSymbol(const _TCHAR wch)
{
	return IsChineseSymbol(wch) || IsEnglishSymbol(wch);
}

bool CHTCharSet::IsSpace(const _TCHAR wch)
{
	return L' ' == wch
		|| L'\x3000' == wch
		;
}



////////////////////////////////////////////////////////////////////////////////
//  CExtractPatternParser
////////////////////////////////////////////////////////////////////////////////

CExtractPatternParser::CExtractPatternParser()
{
	Reset();
}


void CExtractPatternParser::SetExtractPattern( 
	const String& strExtractPattern )
{
	m_strExtract = strExtractPattern;
	m_stStrLen = m_strExtract.size();
}

void CExtractPatternParser::Reset()
{
	m_stPos = 0;
}

bool CExtractPatternParser::GetNextToken( 
	String& strToken )
{
	strToken.erase(strToken.begin(), strToken.end());
	_TCHAR c = m_strExtract[m_stPos];

	while (m_stPos < m_stStrLen && c == _T('<'))
	{
		strToken += c;
		++m_stPos;
		c = m_strExtract[m_stPos];
	}

	if (strToken.empty())
	{
		while (m_stPos < m_stStrLen && c == _T('<'))
		{
			strToken += c;
			++m_stPos;
			c = m_strExtract[m_stPos];
		}
	}
	return (!strToken.empty());
}


