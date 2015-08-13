#include "stdafx.h"
#include <tchar.h>
#include <crtdbg.h>
#include "BQPacketLib.h"
#include "SerialIO.h"
//#include "Logger.h"

// 2004/09/22 Embert Tsai
// Pack CRC and add start and end characters.
int CBQPacketLib::PackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest)
{
	_ASSERTE(pvSrc != NULL);
	_ASSERTE(nSrcLen != 0);
	_ASSERTE(*pvDest == NULL);
	int nRetLen = 0;
	const BYTE *pbSrc = (const BYTE *)pvSrc;
	BYTE **pbDest = (BYTE **)pvDest;
	*pbDest = new BYTE[nSrcLen * 2 + 4 ]; //including ESCAPE character and starting and ending characters and 2 CRC characters.
	ZeroMemory(*pbDest, nSrcLen * 2 + 2 );

	//CCRCMachine oCRC;
	WORD wCRC = CCRCMachine::CRC16LCalculation((CHAR*)pbSrc, (DWORD)(nSrcLen * 8));
	BYTE loCRC = LOBYTE(wCRC);
	BYTE hiCRC = HIBYTE(wCRC);

	if (chStart != PACKET_NULL_CHAR) {
		*((*pbDest) + nRetLen++) = (BYTE)chStart;
	}

	for (int i = 0; i < nSrcLen; i ++) {
		switch (pbSrc[i]) {
		case ESCAPE_CHAR:
		case END_CHAR:
			*((*pbDest) + nRetLen++) = ESCAPE_CHAR;
			*((*pbDest) + nRetLen++) = *(pbSrc + i) ^ COMPLEMENT_CHAR;
			break;
		default:
			*((*pbDest) + nRetLen++) = *(pbSrc + i);
			break;
		}
	}

	switch (loCRC) {
	case ESCAPE_CHAR:
	case END_CHAR:
		*((*pbDest) + nRetLen++) = ESCAPE_CHAR;
		*((*pbDest) + nRetLen++) = loCRC ^ COMPLEMENT_CHAR;
		break;
	default:
		*((*pbDest) + nRetLen++) = loCRC;
		break;
	}

	switch (hiCRC) {
	case ESCAPE_CHAR:
	case END_CHAR:
		*((*pbDest) + nRetLen++) = ESCAPE_CHAR;
		*((*pbDest) + nRetLen++) = hiCRC ^ COMPLEMENT_CHAR;
		break;
	default:
		*((*pbDest) + nRetLen++) = hiCRC;
		break;
	}
	*((*pbDest) + nRetLen++) = chEnd;

	return nRetLen;
}

int CBQPacketLib::UnpackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest)
{
	_ASSERTE(pvSrc != NULL);
	_ASSERTE(nSrcLen > 2 );
	_ASSERTE(*pvDest == NULL);
	//CCRCMachine oCRC;

	int nRetLen = 0;
	const BYTE *pbSrc = (const BYTE *)pvSrc;
	BYTE **pbDest = (BYTE **)pvDest;
	*pbDest = new BYTE[nSrcLen];
	ZeroMemory(*pbDest, nSrcLen);
	int i = 0;

	if (nSrcLen <= 3) {	// including CRC and end character
		//LogInformation(LOG_LEVEL_ERROR, _T("[int CBQPacketLib::UnpackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest)] buffer length is less than 2."));
		delete[] *pbDest;
		*pbDest = NULL;
		return ERROR_CBQPACKETLIB_BUFFER_ERROR;
	}
	if (chStart == START_CHAR) {
		// buffer returned by download mode
		if ((*pbSrc) == START_CHAR) {
			i++;	// Read counter skip first start character.
		}
		else {
			//LogInformation(LOG_LEVEL_ERROR, _T("Start char is 7E, but the starting byte of buffer is not."));
			//LogBuffer(LOG_LEVEL_ERROR, pvSrc, nSrcLen);
			delete[] *pbDest;
			*pbDest = NULL;
			return ERROR_CBQPACKETLIB_CRC_ERROR;
		}
	}

	WORD wCRC;
	for (; i < nSrcLen; i++) {
		switch ( *(pbSrc + i)) {
			// #define ESCAPE_CHAR		0x7D
			// #define END_CHAR			0x7E
			// #define COMPLEMENT_CHAR	0x20
		case ESCAPE_CHAR:
			i++;
			*((*pbDest) + nRetLen++) = *(pbSrc + i) ^ COMPLEMENT_CHAR;
			break;
		case END_CHAR:
			if (nRetLen < 2) {	// CRC is two byte long, so if length is smaller than 2 bytes, then the buffer is not correct.
				//+ CRCError
				//LogInformation(LOG_LEVEL_ERROR, _T("The length of buffer including CRC should be larger than 2."));
				//LogBuffer(LOG_LEVEL_ERROR, pvSrc, nSrcLen);
				wCRC = 0;
			}
			else {
				wCRC = CCRCMachine::CRC16LCalculation((CHAR*) * pbDest, (WORD)(nRetLen * 8));
			}
			if (wCRC != CRC16LOK) {
				//LogInformation(LOG_LEVEL_ERROR, _T("CRC checking fails."));
				//LogBuffer(LOG_LEVEL_ERROR, pvSrc, nSrcLen);
				delete[] *pbDest;
				*pbDest = NULL;
				nRetLen = ERROR_CBQPACKETLIB_CRC_ERROR;
				return nRetLen;
			}
			break;
		default:
			*((*pbDest) + nRetLen++) = *(pbSrc + i);
			break;
		}
	}
	return nRetLen;
}
