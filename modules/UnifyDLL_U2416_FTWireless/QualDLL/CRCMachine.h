#ifndef CRCMACHINE_H
#define CRCMACHINE_H

#include <windows.h> //for DWORD etc. data type

#define CRC_TAB_SIZE			256             /* 2^CRC_TAB_BITS      */
#define CRC16LSEED				0xFFFF
#define CRC16LPOLYNOMIAL		0x8408
#define	CRC16LOK				0x0F47
#define	CRC16LOK_7225_PROBE		0xF024
#define	CRC16LOK_8250_EMDL_PROBE	0x9AAF

#define CRC16SEED				0xFFFF
#define CRC16POLYNOMIAL			0x1021
#define CRC16OK					0xE2F0

#define	CRC30SEED				0x3FFFFFFF
#define	CRC30POLYNOMIAL			0x6030B9C7
#define	CRC30OK					0x0B105AA5

class CCRCMachine
{
// Constructor and Destructor
public:
	CCRCMachine() {};
	~CCRCMachine() {};

// Data
protected:

private:
	static DWORD s_dwCRC30Table[ CRC_TAB_SIZE ];
	static WORD s_wCRC16Table[ CRC_TAB_SIZE ];
	static WORD	s_wCRC16LTable[ CRC_TAB_SIZE ];

// Operations
public:
	static WORD CRC16LCalculation(LPTSTR szBuffer, DWORD dwBitLen);
	static WORD CRC16Calculation(LPTSTR szBuffer, DWORD dwBitLen);
	static DWORD CRC30Calculation(LPTSTR szBuffer, DWORD dwBitLen);
	static DWORD CRC30BlockCal(LPTSTR szBuffer, DWORD dwBitLen, DWORD dwOption);
// Implementations
protected:
};

#endif


