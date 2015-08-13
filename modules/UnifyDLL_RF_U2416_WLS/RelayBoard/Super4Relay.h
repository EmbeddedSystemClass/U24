#ifndef _SUPER_4_RELAY_H_
#define _SUPER_4_RELAY_H_

#include "../CommonUtility/CommonUtil.h"
#include <map>
#define RB_COUNT 10


class Super4Relay
{
// Data members
public:
protected:
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
	bool m_b_onoff_slot[5];

// Member functions
public:
	Super4Relay();
	~Super4Relay();

	bool InitData(std::map<std::string, std::string>& paramMap);
	bool InitSetting();
	bool InitialRB();

	int GetRBCount();
	int GetDLLVersion();
	bool SetRB(int iRBNumber, int iSlot, bool bOn);

protected:
private:
	bool m_bInitOnOff;
	int m_iRBNumber;
};


#endif // #ifndef _SUPER_4_RELAY_H_
