// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLLMFC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLLMFC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLMFC_EXPORTS
#define DLLMFC_API __declspec(dllexport)
#else
#define DLLMFC_API __declspec(dllimport)
#endif

// This class is exported from the dllMFC.dll
class DLLMFC_API CdllMFC {
public:
	CdllMFC(void);
	// TODO: add your methods here.
};

extern DLLMFC_API int ndllMFC;

DLLMFC_API int fndllMFC(void);
