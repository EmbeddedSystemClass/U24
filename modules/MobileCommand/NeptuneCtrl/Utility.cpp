
#include "stdafx.h"
#include "Util.h"

int PreDefMacroToDigitString(char* InString, char* OutString, int PreDefMacroType)
{
	char InBuffer[64];
	memset(InBuffer, 0, sizeof(InBuffer));

	int InLen = 0;
	InLen = (int)strlen(InString);
	strncpy(InBuffer, InString, InLen);

	int SpaceIndex[2] ;
	int SpaceCount = 0;
	int j = 0;
	if (PreDefMacroType == PREDEF_DATE ) {
		for (int i = 0; i < InLen; i++) {
			if (InBuffer[i] == ' ') {
				if (InBuffer[i-1] != 0) { //日只有一位時會有2個空格
					SpaceIndex[SpaceCount] = i;
					SpaceCount++;
					InBuffer[i] = '\0';
				}
				else {
					InBuffer[i] = '0';//填0
				}
			}
		}
		//利用index 取出 year
		for (int i = SpaceIndex[1] + 1;i < InLen;i++) {
			OutString[j] = InBuffer[i];
			j++;
		}
		//month
		if (strcmp("Jan" , InBuffer ) == 0) {
			strcpy(OutString + j, "01");
		}
		else if (strcmp("Feb" , InBuffer ) == 0) {
			strcpy(OutString + j, "02");
		}
		else if (strcmp("Mar" , InBuffer ) == 0) {
			strcpy(OutString + j, "03");
		}
		else if (strcmp("Apr" , InBuffer ) == 0) {
			strcpy(OutString + j, "04");
		}
		else if (strcmp("May" , InBuffer ) == 0) {
			strcpy(OutString + j, "05");
		}
		else if (strcmp("Jan" , InBuffer ) == 0) {
			strcpy(OutString + j, "06");
		}
		else if (strcmp("Jul" , InBuffer ) == 0) {
			strcpy(OutString + j, "07");
		}
		else if (strcmp("Aug" , InBuffer ) == 0) {
			strcpy(OutString + j, "08");
		}
		else if (strcmp("Sep" , InBuffer ) == 0) {
			strcpy(OutString + j, "09");
		}
		else if (strcmp("Oct" , InBuffer ) == 0) {
			strcpy(OutString + j, "10");
		}
		else if (strcmp("Nov" , InBuffer ) == 0) {
			strcpy(OutString + j, "11");
		}
		else if (strcmp("Dec" , InBuffer ) == 0) {
			strcpy(OutString + j, "12");
		}
		j += 2;
		//day
		for (int i = SpaceIndex[0] + 1; i < SpaceIndex[1]; i++) {
			OutString[j] = InBuffer[i];
			j++;
		}
	}

	if (PreDefMacroType == PREDEF_TIME) {
		for (int i = 0; i < InLen; i++) {
			if (InBuffer[i] != ':') {
				OutString[j] = InBuffer[i];
				j++;
			}
		}
	}

	return 1;
}

int AsciiToBinary(int input, int *output)
{
	input = toupper(input);
	if (input >= '0' && input <= '9') {
		*output = input - '0';
		return 1;
	}
	if (input >= 'A' && input <= 'F') {
		*output = input - 'A' + 10;
		return 1;
	}
	*output = 0;

	return -1;
}

int intergePower(int base, int exponent)
{
	if (exponent == 0) {
		return 1;
	}
	else if (exponent == 1) {
		return base;
	}
	else {
		int result = base;
		for (int i = 1; i < exponent; i++) {
			result *= base;
		}
		return result;
	}
}

int _httoi(const TCHAR *value)
{
	struct CHexMap {
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] = {
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0') {
		bool found = false;
		for (int i = 0; i < HexMapL; i++) {
			if (*s == HexMap[i].chr) {
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

int IntTo8ASCII(int iSrc, unsigned char* cOutBuffer)
{
	cOutBuffer[0] = (unsigned char)((0xFF000000 & iSrc) >> 24);
	cOutBuffer[2] = (unsigned char)((0x00FF0000 & iSrc) >> 16);
	cOutBuffer[4] = (unsigned char)((0x0000FF00 & iSrc) >> 8);
	cOutBuffer[6] = (unsigned char)(0x000000FF & iSrc);
	for (int x = 0; x < 4; x++) {
		cOutBuffer[1+2*x] = cOutBuffer[2*x] % 16 ;
		if (cOutBuffer[1+2*x] < 10) {
			cOutBuffer[1+2*x] = cOutBuffer[1+2*x] + '0';
		}
		else {
			cOutBuffer[1+2*x] = cOutBuffer[1+2*x] + 'A' - 10;
		}
		cOutBuffer[2*x] = cOutBuffer[2*x] / 16 ;
		if (cOutBuffer[2*x] < 10) {
			cOutBuffer[2*x] = cOutBuffer[2*x] + '0';
		}
		else {
			cOutBuffer[2*x] = cOutBuffer[2*x] + 'A' - 10;
		}
	}

	return 1;
}

void Rgb565ToRgb(byte* pDest, byte* pSrc, size_t* nSize)
{
	size_t nConvertSize = 0;
	for (size_t i = 0, j = 0; i < *nSize; i += 2, j+= 3) { //2
		pDest[j]   = (pSrc[i] & 0x1F) << 3;						                //B	5
		pDest[j+1] = ((pSrc[i+1] & 0x07) << 5) + ((pSrc[i] & 0xE0) >> 3);  //G 6
		pDest[j+2] = pSrc[i+1] & 0xF8;											//R 5
		nConvertSize += 3;
	}
	for (size_t i = 0; i < nConvertSize; i++) {
		if (pDest[i] >= 0x80)
			pDest[i] |= 0x07;
	}

	*nSize = nConvertSize;
}

void Rgb2Bmp(byte* pDest, byte* pSrc, size_t nWidth, size_t nHeight, size_t& nBufSize, size_t& nBmpWidth)
{		//   byte* pSrc, byte* pTrg, int nWidth, int nHeight, size_t* nBufSize, size_t* nBmpWidth
	nBmpWidth = nWidth * 3;
	if ((nBmpWidth % 4) != 0) {
		nBmpWidth += 4 - (nBmpWidth % 4);
	}

	size_t nImageSize = nHeight * nBmpWidth;
	if (nBufSize > nImageSize) {
		nBufSize = nImageSize;
		memset(pDest, 0, nImageSize);
		for (size_t i = 0; i < nHeight; i++) {
			size_t nSrcPosH = i * nWidth * 3;
			size_t nOutPosH = (nHeight - i - 1) * nBmpWidth;
			for (size_t j = 0; j < nWidth * 3; j++) {
				pDest[nOutPosH + j] = pSrc[nSrcPosH + j];
			}
		}
	}
}

bool IsPathExist(const CString& path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

bool Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse)
{
	if (!IsPathExist(path)) {
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD nPipeSize = 1024;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
		::AfxMessageBox(_T("ERROR: CreatePipe fail!\r\n"));
		return false;
	}

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	//TRACE(_T("current path: %s\n"), szNowPath);
	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	//TRACE(_T("working path: %s\n"), WorkDir);
	SetCurrentDirectory(WorkDir);

	bool isOk = false;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;
	CString Command = path;
	if (!param.IsEmpty()) {
		Command = Command + _T(" ") + param;
	}
	TRACE(_T("exec: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		WaitForSingleObject(processInfo.hProcess, msTimeout);
		isOk = true;

		if (hasResponse) {
			DWORD bytesRead;
			char message[512];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, 511, &bytesRead, NULL);
			if (strlen(message) > 0) {
				param = CA2T(message);
			}
			else {
				param.Empty();
			}
		}
	}
	else {
		isOk = false;
		CString error_msg(_T("ERROR: Execute fail!\r\ncmd:"));
		error_msg += Command;
		::AfxMessageBox(error_msg);
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	CloseHandle(hWrite);
	SetCurrentDirectory(szNowPath);

	return isOk;
}