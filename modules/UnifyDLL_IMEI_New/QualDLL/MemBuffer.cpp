//////////////////////////////////////////////////////////////////////////////////
// 2004/10/11 Embert Tsai
// Add critical section control.
//
/////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <TCHAR.H>
#include <crtdbg.h>
#include <new>
//#include "Logger.h"
#include "MemBuffer.h"
#include "SerialIO.h"
#include "CRCMachine.h"

CRCChecker::CRCChecker()
{
	m_pbBuffer = new BYTE[MEM_BUFFER_BLOCK_SIZE];
	ZeroMemory(m_pbBuffer, MEM_BUFFER_BLOCK_SIZE);
	m_dwBufPos = 0;
	// 20050823 Embert Tsai.
	m_nCurState = 0;
	m_bycmd = 0;
}

CRCChecker::~CRCChecker()
{
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
	m_dwBufPos = 0;
	m_nCurState = 0;
}

// return value
// n: Return buffer
// 0: noraml read
// -1: skip
// -2: invalid header
// -3: CRC error
// -4: undefined error
int CRCChecker::Push(BYTE bData)
{
	LPCTSTR szFunc = _T("CRCChecker::Push");

	if (m_nCurState == 0) {
		if (m_dwBufPos == 0 && m_eState == CSerialIO::EDL && bData != 0x7E && bData != 0x08) {
			m_nCurState = -1;
			return -2;
		}
		if (m_dwBufPos == 0 && m_eState == CSerialIO::EDL && (bData == 0x7E || bData != 0x08)) {
			m_nCurState = 2;
			return 0;
		}

		if (bData == ESCAPE_CHAR) {
			m_nCurState = 1;
			return 0;
		}
		WORD wCRC;
		switch (bData) {
			// #define ESCAPE_CHAR		0x7D
			// #define END_CHAR			0x7E
			// #define COMPLEMENT_CHAR	0x20
		case END_CHAR:
			if (m_dwBufPos < 2) {	// CRC is two byte long, so if length is smaller than 2 bytes, then the buffer is not correct.
				//+ CRCError
				//LogInformation(LOG_LEVEL_ERROR, _T("The length of buffer including CRC should be larger than 2."));
				//LogBuffer(LOG_LEVEL_ERROR, m_pbBuffer, m_dwBufPos);
				wCRC = 0;
			}
			else {
				wCRC = CCRCMachine::CRC16LCalculation((CHAR*)m_pbBuffer, (WORD)(m_dwBufPos * 8));
				//TRACE("CRC:%x\n", wCRC);
			}
			if (wCRC == CRC16LOK || wCRC == CRC16LOK_7225_PROBE || wCRC == CRC16LOK_8250_EMDL_PROBE) {
				return (m_dwBufPos - 2);
			}
			else {
				//LogInformation(LOG_LEVEL_ERROR, _T("CRC checking fails."));
				//LogBuffer(LOG_LEVEL_ERROR, m_pbBuffer, m_dwBufPos);
				m_dwBufPos = 0;
				return -3;
			}
			break;
		default:
			m_pbBuffer[m_dwBufPos] = bData;
			m_dwBufPos++;
			return 0;
			break;
		}

	}

	if (m_nCurState == 2) {
		m_pbBuffer[m_dwBufPos] = bData ;
		m_nCurState = 0;
		m_dwBufPos++;
		return 0;
	}

	if (m_nCurState == -1) {
		if (bData != 0x7E) {
			return -1;
		}
		else {
			m_nCurState = 0;
			return -1;
		}
	}

	if (m_nCurState == 1) {
		m_pbBuffer[m_dwBufPos] = bData ^ COMPLEMENT_CHAR;
		m_nCurState = 0;
		m_dwBufPos++;
		return 0;
	}

	_ASSERTE(FALSE);
	return -4;
}

void CRCChecker::Erase()
{
	m_dwBufPos = 0;
	m_nCurState = 0;
}

BYTE* CRCChecker::GetBuffer()
{
	return m_pbBuffer;
}
