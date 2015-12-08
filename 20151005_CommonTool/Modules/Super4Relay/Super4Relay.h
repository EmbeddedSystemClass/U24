#ifndef _SUPER_4_RELAY_H_
#define _SUPER_4_RELAY_H_


#ifdef SUPER4RELAY_EXPORTS
#define SUPER4RELAY_API __declspec(dllexport)
#else
#define SUPER4RELAY_API __declspec(dllimport)
#endif


#define RB_COUNT 10


class SUPER4RELAY_API Super4Relay
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


// Member functions
public:
	Super4Relay();
	~Super4Relay();

	bool InitialRB();

	int GetRBCount();
	int GetDLLVersion();
	bool SetRB(int iRBNumber, int iSlot, bool bOn);

protected:
private:
	void TrancateFilePath(char* szFilePath);
};


#endif // #ifndef _SUPER_4_RELAY_H_
