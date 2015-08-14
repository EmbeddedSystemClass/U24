#ifndef _EXECCMD_UTIL_H_
#define _EXECCMD_UTIL_H_



/*************************  Example code ****************

void NotifyEachLine (const char* line) 
{
	printf("[NotifyEachLine]  %s  \n", line);
}

main()
{
	ExecuteCmdBlocking( "adb.exe" , "devices" , "D:\\Git\\Berlin_code\\Solution\\LMBB_Debug" , &result);

	ExecuteCmdBlocking( NULL , "a.bat" , "C:" , &result);

	ExecuteCmdCallback( "adb.exe" , "kill-server" , "D:\\Git\\Berlin_code\\Solution\\LMBB_Debug" , NotifyEachLine);

	ExecuteCmdCallback( "adb.exe" , "root" , "D:\\Git\\Berlin_code\\Solution\\LMBB_Debug" , NotifyEachLine);
*/


namespace ExecCmdUtil
{
	//typedef void (*p_fn_callback)(const char*);
	typedef void (*p_fn_callback)(string);

	//unsigned long ExecuteCmdBlockingWithTimeout(char *szWorkingDir, char **chReturn, const char *chCmd1, const char* chCmd2);
	unsigned long ExecuteCmdBlockingWithTimeout( char *szExeName, const char *szCmdLineOrFileName, char *szWorkingDir , char **chReturn , int iDuration = 5000);
	unsigned long ExecuteCmdBlocking( char *szExeName , char *szCmdLineOrFileName, char *szWorkingDir , string *pstrResult);
	unsigned long ExecuteCmdCallback( char *szExeName, char *szCmdLineOrFileName, char *szWorkingDir , p_fn_callback pcallback);
}

#endif