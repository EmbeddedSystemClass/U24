
#ifndef _HWTOOL_UTIL_H_
#define _HWTOOL_UTIL_H_

#define PREDEF_DATE 1
#define PREDEF_FILE 2
#define PREDEF_LINE 3
#define PREDEF_STDC 4
#define PREDEF_TIME 5
#define PREDEF_TIMESTAMP 6 //Mmm dd yyyy

int PreDefMacroToDigitString(char * InString, char * OutString, int PreDefMacroType);
int AsciiToBinary(int a, int * output);
int intergePower(int base, int exponent);
int IntTo8ASCII(int iSrc, unsigned char * cOutBuffer);
int _httoi(const TCHAR * value);

void Rgb565ToRgb(byte* pDest, byte* pSrc, size_t* nSize);
void Rgb2Bmp(byte* pDest, byte* pSrc, size_t nWidth, size_t nHeight, size_t& nBufSize, size_t& nBmpWidth);
bool Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse);
bool IsPathExist(const CString& path);

#endif // _HWTOOL_UTIL_H_

