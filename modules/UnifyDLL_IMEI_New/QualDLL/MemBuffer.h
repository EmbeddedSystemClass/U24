#if !defined(MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_)
#define MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_

// 2004/09/23 Embert Tsai.
// This class is created for 
#include <windows.h>
//#include "Logger.h"
#include "SimpleCriticalSection.h"
#include "SerialIO.h"

#define MEM_BUFFER_BLOCK_SIZE	16384
#define WEIRD_BYTE				0xA3
#define MEM_ARRAY_SIZE			2048
#define MEM_ARRAY_SIZE_MASK		(MEM_ARRAY_SIZE-1)

class CRCChecker
{
private:
	BYTE  *m_pbBuffer;
	DWORD m_dwBufPos;
	int	  m_nCurState;
	CSerialIO::EState m_eState;
	BYTE  m_bycmd;

public:

	CRCChecker();
	~CRCChecker();
	int   Push(BYTE bData);
	BYTE* GetBuffer();
	void  Erase();
	void  SetState(CSerialIO::EState eState) { m_eState = eState; };
	void  SetCom(BYTE bycmd)  { m_bycmd = bycmd; };
	DWORD GetCurrentBufSize() { return m_dwBufPos; };
};
#endif