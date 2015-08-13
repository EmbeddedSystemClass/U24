// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PICS_FILE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PICS_FILE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PICS_FILE_EXPORTS
#define PICS_FILE_API extern "C" __declspec(dllexport)
#else
#define PICS_FILE_API extern "C" __declspec(dllimport)
#endif

//#include <map>
//#include "StationDefine.h"
//#include "MyMSXML.h"

//using namespace std;

//std::map<CString,CString> m_mapPICS;

//CString Get_PICSData(CString ID,std::map<CString,CString> &m_mapPICS);
PICS_FILE_API bool Initial_PICSData ();
PICS_FILE_API bool SetValue(const char* key, const char* value);
PICS_FILE_API bool GeneratePICSFile(int szStation,char *PICSPath,char *szErrorCode,char *szErrorString);