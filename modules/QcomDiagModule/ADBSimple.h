#ifndef __ADB_SIMPLE_H__
#define __ADB_SIMPLE_H__

#define _ADB_EVENT_NAME   "Global\\QAdbEventName"



class __declspec(dllexport) CSimpleADB
{
public: 
	CSimpleADB (const char* chSerial = NULL, const char* chWorkingDir = NULL); //default working dir is DLL folder.
	~CSimpleADB();

	bool SendAdbCmd (const char* chCmd, char *chReturn = NULL, long dwReturnLen=0, int timeout = 30000);
	bool SendAdbShellCmd (const char* chCmd, char *chReturn = NULL, long dwReturnLen=0, int timeout = 30000);
	bool SendFastbootCmd (const char* chCmd, char *chReturn = NULL, long dwReturnLen=0, int timeout = 30000);

protected:
	char m_chSerial[512];
	char m_chWorkingDir[512];
	PROCESS_INFORMATION processInfo;
	HANDLE m_hAdbEvent;


	bool MyExecAdbCmd (const char* chExeName, const char* chCmd, char *chReturn = NULL, long dwReturnLen=0, int timeout = INFINITE);


};



#endif //__ADB_SIMPLE_H__