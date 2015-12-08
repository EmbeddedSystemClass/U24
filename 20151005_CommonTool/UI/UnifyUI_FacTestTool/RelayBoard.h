#pragma once

#define RB_COUNT 10

class CRelayBoard
{
public:
	CRelayBoard(void);
	~CRelayBoard(void);

	bool InitialRB();
	bool IsInit();

	int GetRBCount();
	int GetDLLVersion();
	bool SetRB(int iRBNumber, int iSlot, bool bOn);

	bool SetRBAll(int iRBNumber, bool bOn);

private:
	HMODULE m_hRBDLL;

	typedef int (__stdcall *pFnListRelays)(char stringArray[][20]);
	pFnListRelays m_pFnListRelays;

	typedef int (__stdcall *pFnSetRelays)(char* relayBoard, unsigned char mask, unsigned char set);
	pFnSetRelays m_pFnSetRelays;

	typedef int (__stdcall *pFnDLLVersion)();
	pFnDLLVersion m_pFnDLLVersion;

	char m_szRBName[RB_COUNT][20];
	int m_iRBCount;

};
